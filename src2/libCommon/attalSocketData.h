/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocketData.h
** socket data for Attal over network
**
** Version : $Id: attalSocketData.h,v 1.12 2008/06/09 11:27:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 31/10/2000
**
** Licence :    
**	This program is free software; you can redistribute it and/or modify
**   	it under the terms of the GNU General Public License as published by
**     	the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**	This program is distributed in the hope that it will be useful,
** 	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
****************************************************************/


#ifndef ATTALSOCKETDATA_H
#define ATTALSOCKETDATA_H


// generic include files
// include files for QT
// application specific include
#include "libCommon/log.h"

/*              ------------------------------
 *                         AttalSocketData
 *              ------------------------------ */

enum SOCKET_CLASS {
	SO_MSG, // C-S : For messages (chat)
	SO_CONNECT, // C-S
	SO_MVT, // C-S
	SO_TECHNIC, // C-S
	SO_FIGHT, // C-S
	SO_QR, // C-S
	SO_EXCH, // C-S
	SO_MODIF, // C-S
	SO_TURN, // C-S
	SO_GAME  // C-S
};

enum CLASS_MVT {
	C_MVT_ONE,
	C_MVT_MULTI
};

enum CLASS_MODIF {
	C_MOD_MAP,
	C_MOD_CELL,
	C_MOD_PLAYER,
	C_MOD_LORD,
	C_MOD_BASE,
	C_MOD_BUILD,
	C_MOD_ARTEFACT,
	C_MOD_CREATURE,
	C_MOD_EVENT
};

enum CLASS_CONNECT {
	C_CONN_OK,
	C_CONN_ID,
	C_CONN_NAME,
	C_CONN_VERSION,
	C_CONN_PLAYER
};

enum CLASS_MSG {
	C_MSG_NORM,
	C_MSG_FIGHT,
	C_MSG_CONN
};

enum CLASS_EXCH {
	C_EXCH_START,
	C_EXCH_UNIT,
	C_EXCH_ARTEFACT,
	C_EXCH_BASEUNIT,
	C_EXCH_BASEUNITCL,
	C_EXCH_UNIT_SPLIT
};

enum CLASS_QR {
	C_QR_MSG_NEXT,
	C_QR_MSG_END,
	C_QR_LEVEL,
	C_QR_CHEST,
	C_QR_QUEST,
	C_QR_CREATURE_FLEE,
	C_QR_CREATURE_MERCENARY,
	C_QR_CREATURE_JOIN,
	C_QR_ANSWER
};

enum CLASS_PLAYER {
	C_PLAY_RESS,
	C_PLAY_PRICE
};

enum CLASS_LORD {
	C_LORD_VISIT,
	C_LORD_NEW,
	C_LORD_BUY,
	C_LORD_UNIT,
	C_LORD_REMOVE,
	C_LORD_GARRISON,
	C_LORD_MACHINE,
	C_LORD_CHARAC
};

enum CLASS_BASE {
	C_BASE_NEW,
	C_BASE_OWNER,
	C_BASE_NAME,
	C_BASE_BUILDING,
	C_BASE_UNIT,
	C_BASE_UNIT_BUY,
	C_BASE_MARKET,
	C_BASE_RESS,
	C_BASE_PRODUCTION,
	C_BASE_STATE
};

enum CLASS_BUILD {
	C_BUILD_NEW,
	C_BUILD_OWNER,
	C_BUILD_RESS
};

enum CLASS_FIGHT {
	C_FIGHT_INIT,
	C_FIGHT_CREATURE,
	C_FIGHT_LORD,
	C_FIGHT_CELL,
	C_FIGHT_UNIT,
	C_FIGHT_MODUNIT,
	C_FIGHT_MOVE,
	C_FIGHT_ENDMOVE,
	C_FIGHT_ACTIVE,
	C_FIGHT_DISTATTACK,
	C_FIGHT_WAIT,
	C_FIGHT_FLEE,
	C_FIGHT_DEFEND,
	C_FIGHT_DAMAGE,
	C_FIGHT_END,
};

enum CLASS_TURN {
	C_TURN_PLAY,
	C_TURN_END,
	C_TURN_LORD,
	C_TURN_PLORD
};

enum CLASS_GAME {
	C_GAME_BEGIN,
	C_GAME_LOST,
	C_GAME_WIN,
	C_GAME_END,
	C_GAME_INFO,
	C_GAME_TAVERN,
	C_GAME_CALENDAR
};

enum CLASS_FIGHTER {
	FIGHTER_ATTACK,
	FIGHTER_DEFENSE
};

enum CLASS_ATTACK_TYPE {
	ATTACK_SHOOT,
	ATTACK_FIGHT
};

enum CLASS_ANSWER {
	ANSWER_YESNO,
	ANSWER_ENUM
};

enum CLASS_GAME_INFO {
	C_INFOPLAYER_TEAM,
	C_INFOPLAYER_NAME
};

enum CLASS_GAME_TAVERN {
	C_TAVERN_INFO,
	C_TAVERN_LORD
};

enum CLASS_ARTEFACT {
	C_ART_DELLORD,
	C_ART_ADDLORD
};

enum CLASS_CREATURE {
	C_CRE_NEW,
	C_CRE_UPDATE,
	C_CRE_RESS,
	C_CRE_DEL
};

enum CLASS_EVENT {
	C_EVENT_NEW,
	C_EVENT_DEL
};

enum CLASS_ANALYST {
	C_ANALYST_NEXT_LORD,
	C_ANALYST_SAME_LORD
};

enum QR_TYPE {
	QR_SCEN_DESC,
	QR_INFO
};

class AttalSocketData
{
 
public:
	/** Constructor */
	AttalSocketData();
	
	~AttalSocketData();
	
	uchar getCla1() { return _bufIn[0]; }
	uchar getCla2() { return _bufIn[1]; }
	uchar getCla3() { return _bufIn[2]; }
	
	int readInt();
	unsigned char readChar();
	void reReadData();

	void setData( AttalSocketData * data );
	
	AttalSocketData getData();
	
	void printIn();
	void printOut();

	char _bufIn[256], _bufOut[256] ;
	
	int _lenIn, _lenOut;

protected:
	void appendInt( int );
	void appendChar( unsigned char );
	void appendChar( uchar c1, uchar c2 ) {
		appendChar( c1 );
		appendChar( c2 );
	}
	
	void appendChar( uchar c1, uchar c2, uchar c3 ) {
		appendChar( c1 );
		appendChar( c2 );
		appendChar( c3 );
	}
	
	void appendChar( uchar c1, uchar c2, uchar c3, uchar c4 ) {
		appendChar( c1, c2 );
		appendChar( c3, c4 );
	}
	void appendChar( uchar c1, uchar c2, uchar c3, uchar c4, uchar c5 ) {
		appendChar( c1, c2, c3, c4 );
		appendChar( c5 );
	}
	void appendChar( uchar c1, uchar c2, uchar c3, uchar c4, uchar c5, uchar c6 ) {
		appendChar( c1, c2 );
		appendChar( c3, c4 );
		appendChar( c5, c6 );
	}
	void init( uchar c1, uchar c2 = 0, uchar c3 = 0 );
};
 
#endif // ATTALSOCKETDATA_H
