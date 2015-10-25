/****************************************************************
**
** Attal : Lords of Doom
**
** attalServer.h
** the server : manage connections
**
** Version : $Id: attalServer.h,v 1.72 2013/11/22 12:32:36 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QList>
#include <QSignalMapper>
#include <QTcpServer>

// application specific includes
#include "libCommon/attalSocket.h"
#include "libCommon/fightResultStatus.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"

class GenericArtefact;
class GenericBase;
class GenericEvent;
class GenericInsideBuilding;
class GenericLordArtefact;
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
	AttalPlayerSocket();

	/** Destructor */
	~AttalPlayerSocket();

	/** Return the playe rassociated to the socket */
	GenericPlayer * getPlayer() { return _player; }

	/** Associate a player to the socket */
	void setPlayer( GenericPlayer * player ) { _player = player; }

	bool canSee( int row, int col );

	bool canSee( GenericCell * cell );

private:
	GenericPlayer * _player;

};


/// XXX TODO: should be reimplemented, may be we don't need to inheritate now...
/** Server */
class AttalServer : public QTcpServer
{
	Q_OBJECT
public:

enum ConnectionInfo {
	SK_KICKED,
	SK_WRONG_VERSION
};

	/** Constructor */
	AttalServer( int , QHostAddress address = QHostAddress::Any );
	
	/** Destructor */
	virtual ~AttalServer();

	/* engine methods */

	/** Start the game */
	void startGame( const QList<GenericPlayer *> & list);

	/** Start a fight */
	void startFight( GenericPlayer * attacker, GenericLord * myLord, GenericPlayer * opponent, GenericLord * opponentLord );

	/** Start a fight */
	void startFight( GenericPlayer * attacker, GenericLord * myLord, GenericFightUnit * opponentUnits[MAX_UNIT] );

	void moveUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightCell * cell );
	void updateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightUnit * unit );
	void damageUnit( GenericPlayer * player,
			 CLASS_FIGHTER attacker,
			 int numAttacker,
			 CLASS_ATTACK_TYPE attackType,
			 CLASS_FIGHTER defender,
			 int numDefender,
			 uint damage );
	void activateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num );

	/** Begin turn */
	void beginTurn( const QList<GenericPlayer *> & list, GenericPlayer * player );

	void sendLordPlTurn( GenericPlayer * player, int count );

	void playerLose( const QList<GenericPlayer *> & list, GenericPlayer * player );

	void playerWin( const QList<GenericPlayer *> & list, GenericPlayer * player );
	
	/* send to player info about team of every player */
	void sendGamePlayersTeam( GenericPlayer * player, const QList<GenericPlayer *> & list );

	void sendGameCalendar( const QList<GenericPlayer *> & list, Calendar * calendar);

	void sendGameCalendar( GenericPlayer * player, Calendar * calendar );

	/** End game */
	void sendEndGame( const QList<GenericPlayer *> &list );
	
	void sendTavernInfo( GenericPlayer * player, uchar nbLord, GenericBase * base );
	
	void sendTavernLord( GenericPlayer * player, GenericLord * lord , GenericBase * base );

	/** End fight */
	void endFight(GenericPlayer * attackPlayer, GenericPlayer * defendPlayer, FightResultStatus result );

	/** Send id to connected player */
	void sendConnectionId( char num );

	/** Send info about 'player' to 'dest' */
	void sendConnectionPlayer( GenericPlayer * dest, GenericPlayer * player );

	/** Send ressources to the player */
	void sendPlayerResource( GenericPlayer * player, char ressource, int nb );
	
	/** Send prices to the player */
	void sendPlayerPrice( GenericPlayer * player, char ressource, int price );
	
	/** Send prices to the player */
	void sendPlayerPrices( GenericPlayer * player );

	/** Send ressources to the player */
	void sendPlayerResources( GenericPlayer * player );

	/** Send msg (chat) to every player */
	void sendMessage( QString msg );

	void sendMessage( GenericPlayer * player, QString msg );
	
	/** Send msg (chat) to game's player */
	void sendMessage( const QList<GenericPlayer *> & list, QString msg );

	/** Send msg (chat) to fighting players */
	void sendFightMessage( QString msg, GenericPlayer * attackPlayer, GenericPlayer * defendPlayer );

	/** Send mvt to the player */
	void sendMvt( const QList<GenericPlayer *> & list, int lord, int i, int j );

	/** Send size of the map */
	void sendSizeMap(  const QList<GenericPlayer *> & list, int h, int w );

	/** Send a cell */
	void sendCell( GenericPlayer * player, GenericCell * cell );

	/** Send a game message to the player */
	void sendAskNone( const QList<GenericPlayer *> & list, const QString & msg, uchar type );

	void sendAskNone( GenericPlayer * player, const QString & msg, uchar type );

	void sendAskChest(GenericPlayer * player);

	/** Ask for new skill to the player */
	void sendAskLevel( GenericPlayer * dest, char lord, char level, char primSkill, char skill1, char skill2 ) {
		findSocket( dest )->sendAskLevel( lord, level, primSkill, skill1, skill2 );
	}
	
	void sendAskCreatureJoin(GenericPlayer * player, GenericMapCreature * creature );
	
	void sendAskCreatureMercenary(GenericPlayer * player, GenericMapCreature * creature );
		
	void sendAskCreatureFlee(GenericPlayer * player, GenericMapCreature * creature );

	/** Send new lord */
	void newLord( GenericPlayer * player, GenericLord * lord );

	void newLord( const QList<GenericPlayer *> & list, GenericLord * lord );

	void sendLordVisit( GenericLord * lord, GenericPlayer * player, bool state );

	/** update Lord data*/
	//void updateLord( GenericLord * lord );

	/**uodate lord units (send to one player) */
	void updateUnit(GenericPlayer * player, GenericLord * lord , int num);

	/**uodate lord units (send to all players) */
	void updateUnit( const QList<GenericPlayer *> & list, GenericLord * lord , int num );
	
	void updateUnits(GenericPlayer * player, GenericLord * lord );
	
	void updateUnits( const QList<GenericPlayer *> & list, GenericLord * lord );

	/* update  base units (flag==0)
	 * lord units (flag==1)
	 * or both (flag==2)
	 */
	//void updateLordBaseUnits( GenericBase * base, GenericLord * lord ,int flag);
	
	/** Send new base */
	void newBase( GenericPlayer * player, GenericBase * base );

	/** Send owner of base */
	void ownBase( GenericPlayer * player, GenericBase * base );

	void ownBase( const QList<GenericPlayer *> & list, GenericBase * base );

	/** Send new building */
	void newBuilding( GenericPlayer * player, GenericBuilding * build );

	/** Send owner of building */
	void ownBuilding( GenericPlayer * player, GenericBuilding * build );

	void ownBuilding( const QList<GenericPlayer *> & list, GenericBuilding * build );

	/** update base (new buildings...) */
	void updateBaseBuilding( GenericPlayer * player, GenericBase * base, GenericInsideBuilding * building );

	void updateBaseBuilding( const QList<GenericPlayer *> & list, GenericBase * base, GenericInsideBuilding * building  );

	void sendBaseUnit( const QList<GenericPlayer *> & list, GenericBase * base, Creature * creature, int number, int pos );

	void sendBaseUnits( const QList<GenericPlayer *> & list, GenericBase * base );

	void sendBaseProduction( const QList<GenericPlayer *> & list, GenericBase * base );

	void sendBaseProduction( GenericPlayer * player, GenericBase * base );

	void sendBaseState( GenericPlayer * player, GenericBase * base, GenericBase::BaseState type, bool state );

	void sendBaseStates( GenericPlayer * player, GenericBase * base );

	void sendBaseResource( GenericPlayer * player, GenericBase * base,char ressource, int nb );

	void sendBaseResources( GenericPlayer * player, GenericBase * base );

	void sendBuildingResource( GenericPlayer * player, GenericBuilding * build,char ressource, int nb );

	void sendBuildingResources( GenericPlayer * player, GenericBuilding * build );

	void sendLordExchange( GenericPlayer * player, GenericLord * lord1, GenericLord * lord2 );

	void sendExchangeUnit( const QList<GenericPlayer *> & list, GenericLord * lord1, uchar unit1, GenericLord * lord2, uchar unit2 );
	
	void sendExchangeArtefact( const QList<GenericPlayer *> & list, GenericLord * lord1, int item, GenericLord * lord2 );

	void sendExchangeBaseUnitCl( const QList<GenericPlayer *> & list, GenericBase * base, uchar unit1, GenericLord * lord, uchar unit2 );

	/** Get data from socket */
	void getSocketData( int num );

	void reReadSocketData( int num );

	/** Tell if there is some data to read on socket */
	bool isData( int num );

	/** Clear server */
	void clear() {};

	void sendLordRemove( const QList<GenericPlayer *> & list, GenericLord * lord );

	void sendLordRemove( GenericPlayer * player, GenericLord * lord );

	void sendCreatureRemove( const QList<GenericPlayer *> & list, GenericCell * cell );

	void setGarrison( const QList<GenericPlayer *> & list, GenericLord * lord, bool state );

	void ownArtefact( GenericLordArtefact * artefact, GenericPlayer * player );

	void newCreature( GenericPlayer * player, GenericMapCreature * creature );
	
	void updateCreature( const QList<GenericPlayer *> & list, GenericMapCreature * creature );

	void newEvent( GenericPlayer * player, GenericEvent * event );

	void delEvent( const QList<GenericPlayer *> & list, GenericEvent * event );

	void sendLordCharac( GenericPlayer * player, GenericLord * lord, LordCharac charac );

	void sendLordCharacs( GenericPlayer * player, GenericLord * lord );

	/* server methods */

	/** Return the nb of connections */
	uint getNbSocket() { return _theSockets.count(); }

	int readInt();
	int readInt( int num );

	unsigned char readChar();
	unsigned char readChar( int num );

	uchar getCla1();
	uchar getCla2();
	uchar getCla3();
	
	uchar getCla1( int num );
	uchar getCla2( int num );
	uchar getCla3( int num );

	AttalPlayerSocket * findSocket( GenericPlayer * player );

	void printInfo();

	GenericPlayer * getPlayer( int numsock );

	void copyData( int num, AttalSocket* data );
	void copyData( AttalSocket* data );
	AttalSocketData getData( int num );

	void closeConnectionPlayer( AttalPlayerSocket * uneso, ConnectionInfo info );

	void closeConnectionPlayer( QString name, ConnectionInfo info );

	void closeConnectionSocket(AttalPlayerSocket * uneso);

	void unmapSockets();
	
	void handleConnectionName( int num );

	void handleConnectionVersion( int num );

	void handleMessage( int num );

	void handleCommand( int num , const QString & cmd );
	
	void sendConnectionName(const QString & name, int num);

signals:
	void sig_readEvent( int );
	void sig_newPlayer( AttalPlayerSocket * );
	void sig_newData( int, AttalSocketData );
	void sig_endConnection( QString name );
	void sig_result( int id, bool result);
	void sig_endGame( int );
	void sig_stop();

public slots:
	/** Slot who reads socket */
	void slot_readSocket( int );
	
	void slot_socketClose();

protected:
	virtual void incomingConnection ( int socketDescriptor );

private:
	int _num;
	QList<AttalPlayerSocket *> _theSockets;
	QList<AttalPlayerSocket *> _oldSockets;
	QSignalMapper * _mapper;
};


#endif // ATTALSERVER_H   
