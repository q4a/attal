/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocket.h
** socket for Attal over network
**
** Version : $Id: attalSocket.h,v 1.58 2003/02/03 22:18:37 audoux Exp $
**
** Author(s) : Pascal Audoux
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


#ifndef ATTALSOCKET_H
#define ATTALSOCKET_H


// generic include files
// include files for QT
#include <qsocket.h>
// application specific include
#include "common/genericLord.h"
#include "common/log.h"

class Creature;
class GenericBase;
class GenericBuilding;
class GenericEvent;
class GenericInsideBuilding;
class GenericPlayer;
class GenericCell;
class GenericFightCell;
class GenericLord;
class GenericFightUnit;
class GenericArtefact;
class GenericMapCreature;


/*              ------------------------------
 *                         AttalSocket
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
	C_CONN_PLAYER
};

enum CLASS_EXCH {
	C_EXCH_START,
	C_EXCH_UNIT,
	C_EXCH_BASEUNIT,
	C_EXCH_BASEUNITCL
};

enum CLASS_QR {
	C_QR_NONE,
	C_QR_LEVEL,
	C_QR_ANSWER
};

enum CLASS_PLAYER {
	C_PLAY_RESS
};

enum CLASS_LORD {
	C_LORD_VISIT,
	C_LORD_NEW,
	C_LORD_BUY,
	C_LORD_MOVE,
	C_LORD_MAXMOVE,
	C_LORD_SP,
	C_LORD_MAXSP,
	C_LORD_MORALE,
	C_LORD_LUCK,
	C_LORD_EXP,
	C_LORD_ATT,
	C_LORD_DEF,
	C_LORD_POW,
	C_LORD_KNOW,
	C_LORD_VISION,
	C_LORD_UNIT,
	C_LORD_REMOVE,
	C_LORD_GARRISON,
	C_LORD_MACHINE
};

enum CLASS_BASE {
	C_BASE_NEW,
	C_BASE_OWNER,
	C_BASE_BUILDING,
	C_BASE_UNIT
};

enum CLASS_BUILD {
	C_BUILD_NEW,
	C_BUILD_OWNER
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
	C_TURN_BEG,
	C_TURN_PLAY,
	C_TURN_END,
	C_TURN_LORD,
	C_TURN_PLORD
};

enum CLASS_GAME {
	C_GAME_BEG,
	C_GAME_LOST,
	C_GAME_END
};

enum CLASS_FIGHTER {
	FIGHTER_ATTACK,
	FIGHTER_DEFENSE
};

//bit 0 = 1 -> FIGHTER_ATTACK won
//bit 1 = 1 -> FIGHTER_DEFENSE won
//bit 2 = 1 -> Winner wins because the other fighter has escaped
#define FIGHTER_ATTACK_WIN 1
#define FIGHTER_DEFENSE_WIN 2
#define FIGHTER_FLED 4

enum CLASS_ARTEFACT {
	C_ART_NEWMAP,
	C_ART_NEWLORD,
	C_ART_DELMAP,
	C_ART_DELLORD,
	C_ART_ADDLORD
};

enum CLASS_CREATURE {
	C_CRE_NEW,
	C_CRE_DEL
};

enum CLASS_EVENT {
	C_EVENT_NEW,
	C_EVENT_DEL
};


/** comment for the class */
class AttalSocket : public QSocket
{
 
public:
	/** Constructor */
        AttalSocket();

	/** Send message trough socket */
	void sendMessage( QString msg );
	
	/** Send connection ok */
	void sendConnectionOk();
	
	/** Send connection id */
	void sendConnectionId( char id );
	
	/** Send connection name */
	void sendConnectionName( QString name );

	/** Send connection player */
	void sendConnectionPlayer( GenericPlayer * player );
	
	/** Start exchange between lords */
	void sendExchangeStart( GenericLord * lord1, GenericLord * lord2 );

	/** Exchange 2 units between one or two lords */
	void sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );

	/** Exchange 2 units between one base and a lord or inside a base */
	void sendExchangeBaseUnit( GenericBase * base, int unit1, GenericLord * lord2, int unit2 );
	
	void sendExchangeBaseUnitCl( GenericBase * base, int unit1, GenericLord * lord2, int unit2 );
	
	/** Send ressources of a player */
	void sendPlayerResource( char res, int nb );
	
	/** Send 'Game begins' info */
	void sendBegGame( char nb );

	/** Send the 'Player lost' info */
	void sendLostGame( GenericPlayer * player );

	/** Send the 'Game ends' info */
	void sendEndGame();

	/** Send 'Turn begins' info */
	void sendTurnBeg();
	
	/** Send info about who's playing */
	void sendTurnPlaying( GenericPlayer * player );
	
	/** Send 'Turn ends' info */
	void sendTurnEnd();

	void sendLordPlTurn(int count);

	void sendLordTurn(int flag);
	
	/** Send the size of the map */
	void sendSizeMap( int h, int w );
	
	/** Send a cell */
	void sendCell( GenericCell * cell );
	
	/** Send a movement of a lord */
	void sendMvt( int lord, int cell_i, int cell_j );

	/** A lord is using a technic */
	void sendTechnic( int lord, int numTechnic );

	/** Fight is starting */
	void sendFightInit( CLASS_FIGHTER fighter, GenericLord * lord );

	void sendFightCreature();

	/** Send a fight cell */
	void sendFightCell( GenericFightCell * cell );

	void sendFightLord( CLASS_FIGHTER fighter, GenericLord * lord );

	void sendFightNewUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit );

	void sendFightModifyUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit );

	void sendFightDamageUnit( CLASS_FIGHTER fighter, char num, int damage );

	void sendFightUnitMove( CLASS_FIGHTER fighter, int num, GenericFightCell * cell );

	/** End of a movement */
	void sendFightUnitEndMove();

	/** Activate a unit for fighting */
	void sendFightActivate( CLASS_FIGHTER fighter, char unit );

	/** Damages for the unit */
	void sendFightUnitDamage( CLASS_FIGHTER fighter, char unit, int damage );

	void sendFightDistAttack( CLASS_FIGHTER fighter, char unit );

	void sendFightWait();

	void sendFightFlee();

	void sendFightDefend();

	/** Fight ends */
	void sendFightEnd( char result );

	/** Send message of info */
	void sendAskNone( QString msg );

	/** Ask skill when level upgrade */
	void sendAskLevel( char lord, char level, char primSkill, char skill1, char skill2 );

	/** Send answer to question */
	void sendAnswer( char resp );
	
	/** Lord is visiting */
	void sendLordVisit( GenericLord * lord );

	/** New lord */
	void sendLordNew( GenericLord * lord );

	/** Buy lord */
	void sendLordBuy( GenericLord * lord );

	void sendLordBaseCharac( GenericLord * lord, LordCharac charac );

	/** Send units */
	void sendLordUnits( GenericLord * lord );

	/** Send unit 'num' */
	void sendLordUnit( GenericLord * lord, int num );

	void sendLordRemove( GenericLord * lord );

	void sendLordGarrison( GenericLord * lord, bool state );

	void sendLordMachine( GenericLord * lord, int id );

	/** New base */
	void sendBaseNew( GenericBase * base );

	/** New owner for the base */
	void sendBaseOwner( GenericBase * base, GenericPlayer * player );

	void sendBaseBuilding( GenericBase * base, GenericInsideBuilding * building, bool create );

	/** New building */
	void sendBuildingNew( GenericBuilding * build );

	/** New unit to base */
	void sendBaseUnit( GenericBase * base, GenericFightUnit * unit );

	/** New unit to base */
	void sendBaseUnit( GenericBase * base, Creature * creature, int number );

	/** New owner for the building */
	void sendBuildingOwner( GenericBuilding * build, GenericPlayer * player );

	/** New artefact */
        void sendArtefactNew( GenericArtefact * artefact );

	void sendArtefactDel( GenericArtefact * artefact );

	void sendArtefactLord( GenericArtefact * artefact, GenericLord * lord, bool state );

	/** new creature on map */
	void sendNewCreature( GenericMapCreature * creature );

	/** delete creature of map */
	void sendDelCreature( GenericCell * cell );

	/** new event on map */
	void sendNewEvent( GenericEvent * event );

	/** delete event on map */
	void sendDelEvent( GenericCell * cell );

	// Request client -> server
	void requestBuilding( GenericBase * base, int building );
	
	uchar getCla1() { return _bufIn[0]; }
	uchar getCla2() { return _bufIn[1]; }
	uchar getCla3() { return _bufIn[2]; }
	
	void readData();
	int readInt();
	unsigned char readChar();
	void reReadData();

	void copyData( AttalSocket * data );
	
protected:
	virtual void send();
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

	char _bufIn[256], _bufOut[256] ;

	int _lenIn, _lenOut;
};
 
#endif // ATTALSOCKET_H
