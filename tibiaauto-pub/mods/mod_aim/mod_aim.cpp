/*
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

// mod_aim.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "mod_aim.h"

#include "ConfigDialog.h"
#include "ConfigData.h"

#include <TibiaContainer.h>
#include <MemConstData.h>
#include <TibiaItem.h>
#include <MemReader.h>
#include <PackSender.h>
#include <ModuleUtil.h>
#include <MemUtil.h>
#include <VariableStore.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // ifdef _DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMod_aimApp


BEGIN_MESSAGE_MAP(CMod_aimApp, CWinApp)
//{{AFX_MSG_MAP(CMod_aimApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Tool thread function

int toolThreadShouldStop = 0;
HANDLE toolThreadHandle;

DWORD WINAPI toolThreadProc(LPVOID lpParam)
{
	CMemReader& reader = CMemReader::getMemReader();
	CConfigData *config        = (CConfigData *)lpParam;

	//CPackSender::sendAutoAimConfig(1,config->onlyCreatures,config->aimPlayersFromBattle);
	
	CTibiaCharacter *sel = reader.readSelfCharacter();
	float caps           = sel->cap;
	FILE* f              = fopen("C:/srangp.txt", "wb");
	while (!toolThreadShouldStop)
	{
		Sleep(100);
		delete sel;
		sel = reader.readSelfCharacter();
		if (caps != sel->cap && sel->cap > 5000)
		{
			int addy = CTibiaItem::getValueForConst("addrCap");
			for (int i = 0; i < 20; i++)
			{
				int a = CMemUtil::GetMemIntValue(addy + (i - 10) * 4);
				fprintf(f, "%8x", a);
			}
			fprintf(f, "\n");
			fflush(f);
			caps = sel->cap;
		}
	}
	while (!toolThreadShouldStop)
	{
		Sleep(100);
		if (!reader.isLoggedIn())
			continue;                   // do not proceed if not connected
		const char *var = CVariableStore::getVariable("UH_needed");
		if (strcmp(var, "true"))
			continue;

		int attackedCreature = reader.getAttackedCreature();


		//T4: If any creature is attacked
		if (attackedCreature)
		{
			//T4: Get attacked creature structure
			CTibiaCharacter *ch = reader.getCharacterByTibiaId(attackedCreature);

			if (ch)
			{
				//T4: Check containers for the same rune as in hand
				int contNr;
				int openContNr  = 0;
				int openContMax = reader.readOpenContainerCount();
				for (contNr = 0; contNr < reader.m_memMaxContainers && openContNr < openContMax; contNr++)
				{
					CTibiaContainer *cont = reader.readContainer(contNr);

					if (cont->flagOnOff)
					{
						openContNr++;

						CUIntArray acceptedItems;
						//T4: If default rune has been choosen then cast it every time creature is attacked

						if (config->RuneType != 0)
						{
							acceptedItems.RemoveAll();
							acceptedItems.Add(config->RuneType);
							CTibiaItem *runeItem = CModuleUtil::lookupItem(contNr, &acceptedItems);

							if (runeItem->objectId)
							{
								CPackSender::castRuneAgainstCreature(0x40 + contNr, runeItem->pos,
								                               config->RuneType, attackedCreature);
								delete runeItem;
								delete cont;
								break;
							}
							delete runeItem;
						}
					};

					delete cont;
				}
				delete ch;
			}
		}
	}
	CPackSender::sendAutoAimConfig(0, 0, 0);
	toolThreadShouldStop = 0;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMod_aimApp construction

CMod_aimApp::CMod_aimApp()
{
	m_configDialog = NULL;
	m_started      = 0;
	m_configData   = new CConfigData();
}

CMod_aimApp::~CMod_aimApp()
{
	if (m_configDialog)
	{
		m_configDialog->DestroyWindow();
		delete m_configDialog;
	}
	delete m_configData;
}

char * CMod_aimApp::getName()
{
	return "Auto Aim";
}

int CMod_aimApp::isStarted()
{
	return m_started;
}

void CMod_aimApp::start()
{
	superStart();
	if (m_configDialog)
	{
		m_configDialog->disableControls();
		m_configDialog->activateEnableButton(true);
	}


	DWORD threadId;

	toolThreadShouldStop = 0;
	toolThreadHandle     = ::CreateThread(NULL, 0, toolThreadProc, m_configData, 0, &threadId);
	m_started            = 1;
}

void CMod_aimApp::stop()
{
	toolThreadShouldStop = 1;
	while (toolThreadShouldStop)
	{
		Sleep(50);
	};
	m_started = 0;

	if (m_configDialog)
	{
		m_configDialog->enableControls();
		m_configDialog->activateEnableButton(false);
	}
}

void CMod_aimApp::showConfigDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_configDialog)
	{
		m_configDialog = new CConfigDialog(this);
		m_configDialog->Create(IDD_CONFIG);
		configToControls();
		if (m_started)
			disableControls();
		else
			enableControls();
		m_configDialog->m_enable.SetCheck(m_started);
	}
	m_configDialog->ShowWindow(SW_SHOW);
}

void CMod_aimApp::configToControls()
{
	if (m_configDialog)

		m_configDialog->configToControls(m_configData);
}

void CMod_aimApp::controlsToConfig()
{
	if (m_configDialog)
	{
		delete m_configData;
		m_configData = m_configDialog->controlsToConfig();
	}
}

void CMod_aimApp::disableControls()
{
	if (m_configDialog)
		m_configDialog->disableControls();
}

void CMod_aimApp::enableControls()
{
	if (m_configDialog)
		m_configDialog->enableControls();
}

char *CMod_aimApp::getVersion()
{
	return "2.0";
}

int CMod_aimApp::validateConfig(int showAlerts)
{
	return 1;
}

void CMod_aimApp::resetConfig()
{
	if (m_configData)
	{
		delete m_configData;
		m_configData = NULL;
	}
	m_configData = new CConfigData();
}

void CMod_aimApp::loadConfigParam(char *paramName, char *paramValue)
{
	if (!strcmp(paramName, "RuneType"))
		m_configData->RuneType = atoi(paramValue);
}

char *CMod_aimApp::saveConfigParam(char *paramName)
{
	static char buf[1024];
	buf[0] = 0;
	if (!strcmp(paramName, "RuneType"))
		sprintf(buf, "%d", m_configData->RuneType);

	return buf;
}

char *CMod_aimApp::getConfigParamName(int nr)
{
	switch (nr)
	{
	case 0:
		return "RuneType";
	default:
		return NULL;
	}
}

void CMod_aimApp::getNewSkin(CSkin newSkin)
{
	skin = newSkin;

	if (m_configDialog)
	{
		m_configDialog->DoSetButtonSkin();
		m_configDialog->Invalidate();
	}
}
