/****************************************************************
**
** Attal : Lords of Doom
**
** attalServer.h
** the server : manage connections
**
** Version : $Id: attalServer.h,v 1.52 2003/02/12 21:38:17 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/10/2000
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


#ifndef ATTALSERVER_H
#define ATTALSERVER_H


// generic include files
// include files for QT
#include <qlist.h>
#include <qserversocket.h>
#include <qsignalmapper.h>
// application specific includes
#include "common/attalSocket.h"
#include "common/genericCell.h"
#include "common/genericLord.h"
#include "common/genericPlayer.h"

class Engine;
class GenericArtefact;
class GenericBase;
class GenericEvent;
class GenericInsideBuilding;
class GenericMapCreature;
class Creature;

/*              ------------------------------
 *                        AttalServer
 *              ------------------------------ */

/** Socket with info about the player */
class AttalPlayerSocket : public AttalSocket
{
public:
	/** Constructor */
	AttalPlayerSocket( Engine * engine );

	/** Return the playe rassociated to the socket */
	GenericPlayer * getPlayer() { return _player; }

	/** Associate a player to the socket */
	void setPlayer( GenericPlayer * player ) { _player = player; }

	bool canSee( int row, int col );

	bool canSee( GenericCell * cell );

private:
	GenericPlayer * _player;
	Engine * _engine;

};

/** Server */
class AttalServer : public QServerSocket
{
	Q_OBJECT
public:
	/** Constructor */
	AttalServer( int );

	void setEngine( Engine * engine ) { _engine = engine; }

	/** Start the game */
	void startGame();

	/** Start a fight */
	void startFight( GenericLord * myLord, GenericPlayer * opponent, GenericLord * opponentLord );

	/** Start a fight */
	void startFight( GenericLord * myLord, GenericFightUnit * opponentUnits[MAX_UNIT] );

	void moveUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightCell * cell );
	void updateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightUnit * unit );
	void damageUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, int damage );
	void activateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num );

	/** Begin turn */
	void beginTurn();

	void sendLordPlTurn(int count);

	void playerLose( GenericPlayer * player );

	/** End game */
	void endGame();

	/** End fight */
	void endFight(GenericPlayer * attackPlayer, GenericPlayer * defendPlayer, char result );

	/** Send id to connected player */
	void sendConnectionId( char num );

	/** Send info about 'player' to 'dest' */
	void sendConnectionPlayer( GenericPlayer * dest, GenericPlayer * player );

	/** Send ressources to the player */
	void sendPlayerResource( char ressource, int nb );

	/** Send ressources to the player */
	void sendPlayerResources( GenericPlayer * player );

	/** Send msg (chat) to every player */
	void sendMessage( QString msg );

	/** Send msg (chat) to fighting players */
	void sendFightMessage( QString msg, GenericPlayer * attackPlayer, GenericPlayer * defendPlayer );

	/** Send mvt to the player */
	void sendMvt( GenericPlayer * dest, int lord, int i, int j );

	/** Send size of the map */
	void sendSizeMap( int h, int w );

	/** Send a cell */
	void sendCell( GenericCell * cell );

	/** Send a game message to the player */
	void sendAskNone( QString msg );

	/** Ask for new skill to the player */
	void sendAskLevel( GenericPlayer * dest, char lord, char level, char primSkill, char skill1, char skill2 ) {
		findSocket( dest )->sendAskLevel( lord, level, primSkill, skill1, skill2 );
	}

	/** Send new lord */
	void newLord( GenericLord * lord );

	/** update Lord data*/
	void updateLord( GenericLord * lord );

	/* update  base units (flag==0)
	 * lord units (flag==1)
	 * or both (flag==2)
	 */

	void updateLordBaseUnits( GenericBase * base, GenericLord * lord ,int flag);
	
	/** Send new base */
	void newBase( GenericBase * base );

	/** Send owner of base */
	void ownBase( GenericBase * base, GenericPlayer * player );

	/** Send new building */
	void newBuilding( GenericBuilding * build );

	/** Send owner of building */
	void ownBuilding( GenericBuilding * build, GenericPlayer * player );

	/** update base (new buildings...) */
	void updateBaseBuilding( GenericBase * base, GenericInsideBuilding * building );

	void sendBaseUnit( GenericBase * base, Creature * creature, int number );

	void sendLordExchange( GenericLord * lord1, GenericLord * lord2 );

	void sendExchangeUnit( GenericLord * lord1, uchar unit1, GenericLord * lord2, uchar unit2 );

	void sendExchangeBaseUnitCl( GenericBase * base, uchar unit1, GenericLord * lord, uchar unit2 );

	/** Manage new connection */
	void newConnection ( int socket );

	/** Get data from socket */
	void getSocketData( int num );

	/** Tell if there is some data to read on socket */
	bool isData( int num );

	/** Clear server */
	void clear() {};

	void sendLordRemove( GenericLord * lord );

	void sendCreatureRemove( GenericCell * cell );

	void setGarrison( GenericLord * lord, bool state );

	/** Set the current player */
	void setCurrentPlayer( GenericPlayer * player ) {
		_currentSocket = findSocket( player );
	}

	void newArtefactMap( GenericArtefact * artefact );

	void ownArtefact( GenericArtefact * artefact, GenericLord * lord, GenericPlayer * player );

	void newCreature( GenericMapCreature * creature );

	void delCreature( GenericCell * cell );

	void newEvent( GenericEvent * event );

	void delEvent( GenericCell * cell );

	void sendLordCharac( GenericPlayer * player, GenericLord * lord, LordCharac charac );

	/** Return the nb of connections */
	uint getNbSocket() { return _theSockets.count(); }

	int readInt();

	unsigned char readChar();

	uchar getCla1();
	uchar getCla2();
	uchar getCla3();

	AttalPlayerSocket * findSocket( GenericPlayer * player );

	void copyData( AttalSocket* data );

signals:
	void sig_readEvent( int );
	void sig_newPlayer( AttalPlayerSocket * );
	void sig_endConnection( QString name );

public slots:
	void slot_socketClose();
	
private:
	int _num;
	Engine * _engine;
	QList<AttalPlayerSocket> _theSockets;
	QSignalMapper * _mapper;
	AttalPlayerSocket * _currentSocket;
};


#endif // ATTALSERVER_H   
