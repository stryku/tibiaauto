#pragma once

#include "tibiaauto_util.h"

/**
* message types
* 1 - test
* 2 - send packet
* 3 - send text message
* 4 - process id delcaration
* 100 - attacked creature
* 101 - hmm rune location
* 102 - gfb rune location
* 103 - sd rune location
* 104 - explo rune location
* 105 - self UH
* 106 - fluid mana
* 107 - fluid life
*
* 1001 - recv info; format: 4b-type; 4b-nicklen; 4b-messlen; X-nick; X-mess
*/

class TIBIAAUTOUTIL_API CIpcMessage
{
public:
	CIpcMessage();
	void send();

	int messageType;
	char payload[1024];
	time_t tm;
	static HANDLE hPipe; //separate for each loaded dll instances
};

