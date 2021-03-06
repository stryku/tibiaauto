// ConfigData.cpp: implementation of the CConfigData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mod_cavebot.h"
#include "ConfigData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // ifdef _DEBUG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigData::CConfigData()
{
	int i;
	for (i = 0; i < MAX_WAYPOINTCOUNT; i++)
	{
		waypointList[i].x = 0;
		waypointList[i].y = 0;
		waypointList[i].z = 0;
	}
	for (i = 0; i < MAX_MONSTERLISTCOUNT; i++)
	{
		monsterList[i][0] = 0;
	}
	for (i = 0; i < MAX_IGNORECOUNT; i++)
	{
		ignoreList[i][0] = 0;
	}
	for (i = 0; i < MAX_DEPOTTRIGGERCOUNT; i++)
	{
		depotTrigger[i].itemName[0] = 0;
		depotTrigger[i].when = 0;
		depotTrigger[i].remain = 0;
	}

	monsterCount              = 0;
	ignoreCount               = 0;
	autoFollow                = 1;
	attackAllMonsters         = 0;
	lootFood                  = 0;
	lootGp                    = 0;
	lootWorms                 = 0;
	mode                      = 0;
	suspendOnEnemy            = 1;
	suspendOnNoMove           = 1;
	waypointSelectMode        = 1;
	mapUsed                   = 1;
	capacityLimit             = 50;
	lootCustom                = 1;
	lootWhileKill             = 0;
	lootInBags                = 1;
	attackRange               = 5;
	stickToMonster            = 0;
	eatFromCorpse             = 0;
	unreachableAfter          = 15;
	suspendAfterUnreachable   = 10;
	attackOnlyAttacking       = 1;
	standStill                = 2;
	forceAttackAfterAttack    = 1;
	gatherLootStats           = 0;
	debug                     = 1;
	weaponTrain               = 0;
	weaponFight               = 0;
	fightWhenSurrounded       = 1;
	fightWhenAlien            = 1;
	trainingMode              = 0;
	bloodHit                  = 1;
	trainingActivate          = 0;
	lootFromFloor             = 0;
	attackHpAbove             = 0;
	backattackRunes           = 0;
	shareAlienBackattack      = 0;
	depotDropInsteadOfDeposit = 0;
	depotCap                  = 0;
	dontAttackPlayers         = 1;
	pausingEnable             = 1;
	radius                    = 1;
	selectedWaypoint          = -1;

	dropNotLooted = 0;
	for (i = 0; i < MAX_DROPLISTCOUNT; i++)
	{
		dropList[i][0] = '\0';
	}
	dropListCount                = 0;
	dropWhenCapacityLimitReached = 0;
	dropOnlyLooted               = 0;

	strcpy(modPriorityStr, "1");
	strcpy(depotModPriorityStr, "2");
	stopByDepot           = 0;
	depositLootedItemList = 0;
}

CConfigData::~CConfigData()
{
}
