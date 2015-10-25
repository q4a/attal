/****************************************************************
**
** Attal : Lords of Doom
**
** attalServer.cpp
** the server : manage connections
**
** Version : $Id: attalServer.cpp,v 1.118 2013/11/22 12:32:36 lusum Exp $
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

#include "attalServer.h"

// generic include files
// include files for QT
#include <QTimer>
#include <QTcpSocket>

// application specific includes
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/log.h"
#include "libCommon/priceMarket.h"
#include "libCommon/unit.h"

extern QString VERSION;

//
// ----- AttalPlayerSocket -----
//

AttalPlayerSocket::AttalPlayerSocket( )
	: AttalSocket(), _player( 0 )
{
	_player = new GenericPlayer();
	TRACE( "AttalPlayerSocket::AttalPlayerSocket, _player %p", _player);
}

AttalPlayerSocket::~AttalPlayerSocket( )
{
	TRACE( "AttalPlayerSocket::~AttalPlayerSocket, _player %p", _player);
	if( _player ) {
		delete _player; 
		_player = NULL;
	}
}

bool AttalPlayerSocket::canSee( int row, int col )
{
	bool ret = false;

	if( _player ) {
		ret = _player->canSee( row, col );
	}

	return ret;
}

bool AttalPlayerSocket::canSee( GenericCell * cell )
{
	bool ret = false;

	if( _player ) {
		ret = _player->canSee( cell );
	}

	return ret;
}

/*
 * NOTE: All send fuction must be in the form sendSomething( destinationPlayer/s , data )
*/
//
// ----- AttalServer -----
//

AttalServer::AttalServer( int port, QHostAddress address )
	: QTcpServer( 0 )
{	
	listen( address, port );
	_num = -1;
	_mapper = new QSignalMapper( this );
	connect( _mapper, SIGNAL( mapped( int ) ), SIGNAL( sig_readEvent( int ) ) );	
	connect( this, SIGNAL( sig_readEvent( int ) ), SLOT( slot_readSocket( int ) ) );
}

AttalServer::~AttalServer()
{
	this->close();

	while (!_theSockets.isEmpty()) {
		AttalPlayerSocket * uneso = _theSockets.takeFirst();
		disconnect(uneso, 0, 0, 0);
		delete uneso;
	}
	while (!_oldSockets.isEmpty()) {
		AttalPlayerSocket * uneso = _oldSockets.takeFirst();
		disconnect(uneso, 0, 0, 0);
		delete uneso;
	}
}

void AttalServer::incomingConnection( int socketDescriptor )
{
	TRACE("AttalServer::incomingConnection( int  socketDescriptor %d)",socketDescriptor);

	AttalPlayerSocket * uneso = new AttalPlayerSocket();
	uneso->setSocketDescriptor( socketDescriptor );

	_theSockets.append( uneso );
	_mapper->setMapping( uneso, _theSockets.count()-1 );
	connect( uneso, SIGNAL( readyRead() ), _mapper, SLOT( map() ) );
	connect( uneso, SIGNAL( disconnected() ), SLOT( slot_socketClose() ) );

	uneso->sendConnectionOk();
}

void AttalServer::slot_socketClose()
{
	TRACE("AttalServer:slot_socketClose" );

	//no more data
	_num = -1;

	QString connectionName = "";

	AttalPlayerSocket * uneso =(AttalPlayerSocket*)sender();
	if( uneso->getPlayer()) {
		TRACE("Server:close connection player " );
		TRACE("Disconnect: %s", qPrintable( uneso->getPlayer()->getConnectionName() ) );
		sendMessage(uneso->getPlayer()->getConnectionName() + "  disconnect");
		connectionName = uneso->getPlayer()->getConnectionName();
	}
	closeConnectionSocket( uneso );
	emit sig_endConnection( connectionName );
	emit sig_stop();
}

void AttalServer::closeConnectionSocket(AttalPlayerSocket * uneso)
{

	TRACE("AttalServer::closeConnectionSocket" );
	
	while (!_oldSockets.isEmpty()) {
		delete _oldSockets.takeFirst();
	}
	
	unmapSockets();

	int it = _theSockets.indexOf( uneso );
	if (it != -1) {
		_theSockets.removeAt( it );
	}
	//deferred removal caused by problems of SEGFAULT, no problems of memory leak ( only remain one socket in _oldSockets list and deleted at the end )
	_oldSockets.append( uneso);

	uint nbSockets = (uint) _theSockets.count();

	for( uint i = 0; i < nbSockets; ++i ) {
		_mapper->setMapping( _theSockets[ i ], i );
		_theSockets[ i ]->sendConnectionId( i );
	}

	TRACE("AttalServer::closeConnectionSocket end " );
}

void AttalServer::unmapSockets()
{	
	uint oldNbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < oldNbSockets; ++i ) {
		AttalPlayerSocket * sock  = NULL;
		sock = (AttalPlayerSocket *)	_mapper->mapping( i );
		if( sock ) {
			_mapper->removeMappings( sock );
		}
	}
}

void AttalServer::closeConnectionPlayer( AttalPlayerSocket * uneso, ConnectionInfo info )
{
	if( uneso->getPlayer() ) {
		switch( info ) {
			case SK_KICKED:
				sendMessage(uneso->getPlayer()->getConnectionName() + "  kicked");
				break;
			case SK_WRONG_VERSION:
				sendMessage(uneso->getPlayer()->getConnectionName() + "  wrong client version");
				sendMessage(QString("This server use version "+VERSION ));
				break;
			default:
				break;
		}
	}

	if(uneso) {
		uneso->close();
	}
}

void AttalServer::closeConnectionPlayer( QString name, ConnectionInfo info )
{
	TRACE("AttalServer::closeConnectionPlayer " );

	AttalPlayerSocket * uneso = NULL;
	
	uint nbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		if( _theSockets[ i ]->getPlayer()->getConnectionName() == name ) {
			uneso =  _theSockets[ i];
			closeConnectionPlayer( uneso, info );
			break;
		}
	}

}

void AttalServer::slot_readSocket( int num )
{
	AttalSocketData tmpData;

	getSocketData( num );
	tmpData = getData( num );

	//logDD( "Server socket received %d | %d | %d", getCla1(), getCla2(), getCla3() );
	uchar c = getCla1();
	switch( c ) {
	case SO_CONNECT:
		switch( getCla2() ) {
		case C_CONN_NAME:
			handleConnectionName( num );
			break;
		case C_CONN_VERSION:
			handleConnectionVersion( num );
			break;
		case C_CONN_PLAYER:
			logEE( "Should not happen (Server : SO_CONNECT/C_CONN_PLAYER)" );
			break;
		case C_CONN_ID:
			logEE( "Should not happen (Server : SO_CONNECT/C_CONN_ID)" );
			break;
		case C_CONN_OK:
			logEE( "Should not happen (Server : SO_CONNECT/C_CONN_OK)" );
			break;
		}
		break;
	case SO_MSG:
		handleMessage( num );
		break;
	default:
		//logEE( "Game not started...%d", c );
		break;
	}
		
	emit sig_newData( num, tmpData );

	//socket can be destroyed after sig_newData, so it could be no more sock to control ( _num = -1 )
	if( _num != -1 && isData( _num ) ) {
		slot_readSocket( _num );
	}

}

void AttalServer::handleConnectionName( int num )
{
	TRACE("AttalServer::handleConnectionName");

	QString res;
	QString name;
	/// XXX:  CHECK build of 'res'
	_theSockets[ num ]->getPlayer()->setNum( num );
	uint len = readChar();
	
	for( uint i = 0; i < len; i++) {
		res.append( readChar() );
	}
	name = res;
		
	if(  name == QString("IA") ) {
		_theSockets[ num ]->getPlayer()->setRuledByAi( true );
	}

	for( int j = 0; j < _theSockets.count(); j++ ) {
		if(  j != num && _theSockets[ j ]->getPlayer()->getConnectionName() == name ) {
			name = res + QString("_%1").arg(j);
		}
	}

	_theSockets[ num ]->getPlayer()->setConnectionName( name );

	TRACE("old name player %s", qPrintable( res ) );
	TRACE("current name %s", qPrintable( name ) );

	sendConnectionId( num );
	sendConnectionName( name , num);

	emit sig_newPlayer( _theSockets[ num ] );
}

void AttalServer::handleConnectionVersion( int num )
{
	TRACE("AttalServer::handleConnectionVersion");

	QString res;
	
	uint len = readChar();
	
	for( uint i = 0; i < len; i++) {
		res.append( readChar() );
	}

	if(  res != VERSION ) {
		AttalPlayerSocket * uneso = _theSockets.at( num );
		closeConnectionPlayer( uneso, SK_WRONG_VERSION  );
	}

}

void AttalServer::handleMessage( int num )
{
	QString msg;
	uchar d = readChar();
	for( uint i = 0; i < d; i++ ) {
		msg[i] = readChar();
	}
	if( msg.contains(": /")) { 
		sendMessage( _theSockets.at( num )->getPlayer(), msg );
		QStringList list = msg.split(": /");
		handleCommand( num, list.at(1) );
	}
}

void AttalServer::handleCommand( int num,  const QString & cmd )
{
	QString msg = "";

	if( cmd.startsWith("list") ) {
		uint nbSockets = (uint) _theSockets.count();
		for( uint i = 0; i < nbSockets; ++i ) {
			msg  = _theSockets[i ]->getPlayer()->getConnectionName();
			msg += " " + _theSockets[i ]->peerAddress().toString();
			msg += "\n";
			sendMessage( _theSockets.at( num )->getPlayer(), msg );
		}
	} else if( cmd.startsWith("kick") ) {
		QStringList list = cmd.split(" ");
		QString name = list.at( 1 );
		closeConnectionPlayer( name, AttalServer::SK_KICKED );
	} else if( cmd.startsWith("version") ) {
		msg = "This server use version "+VERSION;
		sendMessage( _theSockets.at( num )->getPlayer(), msg );
	}
}

void AttalServer::sendConnectionId( char num )
{
	_theSockets.at( num )->sendConnectionId( num );
}

void AttalServer::sendConnectionPlayer( GenericPlayer * dest, GenericPlayer * player )
{
	findSocket( dest )->sendConnectionPlayer( player );
}

void AttalServer::sendConnectionName(const QString & name, int num )
{
	_theSockets.at( num )->sendConnectionName( name );
}

void AttalServer::sendMessage( QString msg )
{
	uint nbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		_theSockets[ i ]->sendMessage( msg );
	}
}

void AttalServer::getSocketData( int num )
{
	_num = num;
	_theSockets.at( num )->readData();
}

void AttalServer::reReadSocketData( int num )
{
	_num = num;
	_theSockets.at( num )->reReadData();
}

void AttalServer::copyData( AttalSocket* data )
{
	_theSockets.at( _num )->setData( data );
}

uchar AttalServer::getCla1()
{
	return _theSockets.at( _num )->getCla1();
}

uchar AttalServer::getCla2()
{
	return _theSockets.at( _num )->getCla2();
}

uchar AttalServer::getCla3()
{
	return _theSockets.at( _num )->getCla3();
}

int AttalServer::readInt()
{
	return _theSockets.at( _num )->readInt();
}

unsigned char AttalServer::readChar()
{
	return _theSockets.at( _num )->readChar();
}

void AttalServer::copyData( int num, AttalSocket* data )
{
	_theSockets.at( num )->setData( data );
}

AttalSocketData AttalServer::getData( int num )
{
	return _theSockets.at( num )->getData();
}

uchar AttalServer::getCla1( int num )
{
	return _theSockets.at( num )->getCla1();
}

uchar AttalServer::getCla2( int num )
{
	return _theSockets.at( num )->getCla2();
}

uchar AttalServer::getCla3( int num )
{
	return _theSockets.at( num )->getCla3();
}

int AttalServer::readInt( int num )
{
	return _theSockets.at( num )->readInt();
}

unsigned char AttalServer::readChar( int num )
{
	return _theSockets.at( num )->readChar();
}

bool AttalServer::isData( int num )
{
	return ( !_theSockets.isEmpty() && _theSockets.at(num)->bytesAvailable() > 0);
}

AttalPlayerSocket * AttalServer::findSocket( GenericPlayer * player )
{
	if( !player ) {
		return 0;
	}

	uint nbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		if( _theSockets[ i ]->getPlayer() == player ) {
			return _theSockets[ i ];
		}
	}
	
	logEE( "AttalServer::findSocket Socket not found " );
	logEE("findSocket player %p, player name %s", player, qPrintable( player->getConnectionName() ) );
	printInfo();
	return 0;
}

GenericPlayer * AttalServer::getPlayer( int numsock )
{
	if( numsock < _theSockets.count() ) {
		return _theSockets[ numsock ]->getPlayer();
	}
	return NULL;
}

void AttalServer::printInfo()
{
	uint nbSockets = (uint) _theSockets.count();
	logEE( "Num Sockets %d", nbSockets );
	
	for( uint i = 0; i < nbSockets; ++i ) {
		GenericPlayer * player = _theSockets[ i ]->getPlayer();
		logEE("Info %d player %p, player name %s", i , player, qPrintable( player->getConnectionName() ) );
	}
}

/*!

*/


/* 
 * engine specific methods 
 */

void AttalServer::startGame( const QList<GenericPlayer *> & list )
{
	TRACE("AttalServer::startGame");

	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		TRACE("Socket %p", socket);
		TRACE("Name %s", qPrintable(list.at(k)->getConnectionName()));
		if( socket ) {
			socket->sendBegGame( nbSockets );
		}
	}
}

void AttalServer::playerLose( const QList<GenericPlayer *> & list, GenericPlayer * player )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendLostGame( player );
		}
	}
}

void AttalServer::playerWin( const QList<GenericPlayer *> & list, GenericPlayer * player )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendWinGame( player );
		}
	}
}

/* send to player info about team of every player */
void AttalServer::sendGamePlayersTeam( GenericPlayer * player, const QList<GenericPlayer *> & list )
{
	AttalPlayerSocket * socket = findSocket( player );
	uint nbSockets = (uint) list.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		if( socket ) {
			socket->sendGameInfoPlayerTeam( list.at(i) );
		}
	}
}

void AttalServer::sendGameCalendar( const QList<GenericPlayer *> & list, Calendar * calendar)
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendGameCalendar( calendar );
		}
	}
}

void AttalServer::sendGameCalendar( GenericPlayer * player, Calendar * calendar)
{
	QList<GenericPlayer *> list;
	list.append( player );
	sendGameCalendar( list, calendar );
}

void AttalServer::sendEndGame( const QList<GenericPlayer *> & list )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendEndGame();
		}
	}
}

void AttalServer::sendTavernInfo( GenericPlayer * player, uchar nbLord, GenericBase * base )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendTavernInfo( nbLord, base );
	}
}

void AttalServer::sendTavernLord( GenericPlayer * player, GenericLord * lord, GenericBase * base  )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendTavernLord( lord, base );
	}
}

void AttalServer::beginTurn( const QList<GenericPlayer *> & list, GenericPlayer * player )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendTurnPlaying( player );
		}
	}
}

void AttalServer::sendLordPlTurn( GenericPlayer * player, int count )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendLordPlTurn( count );
	}
}

void AttalServer::startFight( GenericPlayer * attacker,GenericLord * myLord, GenericPlayer * opponent, GenericLord * opponentLord )
{
	int i;
	AttalPlayerSocket * attackerSocket = findSocket( attacker );

	if( !opponent ) {
		//creature
		attackerSocket->sendFightInit( FIGHTER_ATTACK, myLord );
		attackerSocket->sendFightCreature();
		for( int i = 0; i < MAX_UNIT; i++ ) {
			if( opponentLord->getUnit( i ) ) {
				attackerSocket->sendFightNewUnit( FIGHTER_DEFENSE, i, opponentLord->getUnit( i ) );
			}
		}
	} else {
		//lord
		attackerSocket->sendFightInit( FIGHTER_ATTACK, myLord );
		attackerSocket->sendFightLord( FIGHTER_DEFENSE, opponentLord );
		for( i = 0; i < MAX_UNIT; i++ ) {
			if( opponentLord->getUnit( i ) ) {
				attackerSocket->sendFightNewUnit( FIGHTER_DEFENSE, i, opponentLord->getUnit( i ) );
			}
		}

		AttalPlayerSocket * opponentSocket = findSocket( opponent );
		opponentSocket->sendFightInit( FIGHTER_DEFENSE, opponentLord );
		opponentSocket->sendFightLord( FIGHTER_ATTACK, myLord );
		for( i = 0; i < MAX_UNIT; i++ ) {
			if( myLord->getUnit( i ) ) {
				opponentSocket->sendFightNewUnit( FIGHTER_ATTACK, i, myLord->getUnit( i ) );
			}
		}
	}

}

void AttalServer::startFight( GenericPlayer * attacker,GenericLord * myLord, GenericFightUnit * opponentUnits[MAX_UNIT] )
{
	AttalPlayerSocket * attackerSocket = findSocket( attacker );
	attackerSocket->sendFightInit( FIGHTER_ATTACK, myLord );
	attackerSocket->sendFightCreature();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( opponentUnits[i] ) {
			attackerSocket->sendFightNewUnit( FIGHTER_DEFENSE, i, opponentUnits[i] );
		}
	}
}

void AttalServer::moveUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightCell * cell )
{
	AttalPlayerSocket * socket = findSocket( player );
	socket->sendFightUnitMove( fighter, num, cell );
}

void AttalServer::updateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightUnit * unit )
{
	AttalPlayerSocket * socket = findSocket( player );
	socket->sendFightModifyUnit( fighter, num, unit );
}

void AttalServer::damageUnit( GenericPlayer * player,
			      CLASS_FIGHTER attacker,
			      int numAttacker,
			      CLASS_ATTACK_TYPE attackType,
			      CLASS_FIGHTER defender,
			      int numDefender,
			      uint damage )
{
	AttalPlayerSocket * socket = findSocket( player );
	socket->sendFightDamageUnit( attacker,
				   numAttacker,
				   attackType,
				   defender,
				   numDefender,
				   damage );
}

void AttalServer::activateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num )
{
	AttalPlayerSocket * socket = findSocket( player );
	socket->sendFightActivate( fighter, num );
}

void AttalServer::endFight(GenericPlayer * attackPlayer, GenericPlayer * defendPlayer, FightResultStatus result )
{
	AttalPlayerSocket * socket = findSocket( attackPlayer );
	if( socket ) {
		socket->sendFightEnd( result );
	}

	if( defendPlayer ) {
		socket = findSocket( defendPlayer );
		if( socket ) {
			socket->sendFightEnd( result );
		}
	}
}

void AttalServer::sendMessage( const QList<GenericPlayer *> & list, QString msg )
{
	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		sendMessage( list.at(k), msg );
	}
}

void AttalServer::sendMessage( GenericPlayer * player, QString msg )
{
	AttalPlayerSocket * socket = findSocket(player);
	socket->sendMessage( msg );
}

void AttalServer::sendFightMessage( QString msg, GenericPlayer * attackPlayer, GenericPlayer * defendPlayer )
{
	AttalPlayerSocket * socket = findSocket( attackPlayer );
	socket->sendFightMessage( msg );

	if( defendPlayer ) {
		socket = findSocket( defendPlayer );
		socket->sendFightMessage( msg );
	}
}

void AttalServer::sendPlayerResource( GenericPlayer * player ,char ressource, int nb )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		if( DataTheme.resources.get( ressource )->isGlobal() ) {
			socket->sendPlayerResource( ressource, nb );
		}
	}
}

void AttalServer::sendPlayerPrice( GenericPlayer * player , char ressource, int price )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendPlayerPrice( ressource, price );
	}
}

void AttalServer::sendPlayerResources( GenericPlayer * player )
{
	AttalPlayerSocket * socket = findSocket( player );
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( socket ) {
			if( DataTheme.resources.get( i )->isGlobal() ) {
				socket->sendPlayerResource( i, player->getResourceList()->getValue( i ) );
			}
		}
	}
}

void AttalServer::sendPlayerPrices( GenericPlayer * player )
{
	AttalPlayerSocket * socket = findSocket( player );
	uint nbRes = DataTheme.resources.count();
	int price;
	for( uint i = 0; i < nbRes; i++ ) {
		price = player->getPriceMarket()->getResourcePrice( i );
		if( socket ) {
			socket->sendPlayerPrice( i, price );
		}
	}
}

void AttalServer::sendSizeMap( const QList<GenericPlayer *> & list, int h, int w )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendSizeMap( h, w );
		}
	}
	
}

void AttalServer::sendCell( GenericPlayer * player, GenericCell * cell )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendCell( cell );
	}
}

void AttalServer::sendAskNone( const QList<GenericPlayer *> & list, const QString & msg, uchar type )
{
	AttalPlayerSocket * socket; 
	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendAskNone( msg, type );
		}
	}
}

void AttalServer::sendAskNone( GenericPlayer * player, const QString & msg, uchar type )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendAskNone( msg, type );
	}
}

void AttalServer::sendAskChest(GenericPlayer * player)
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendAskChest();
	}
}

void AttalServer::sendAskCreatureJoin( GenericPlayer * player, GenericMapCreature * creature )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendAskCreatureJoin( creature );
	}
}
	
void AttalServer::sendAskCreatureMercenary( GenericPlayer * player, GenericMapCreature * creature )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendAskCreatureMercenary( creature );
	}
}

void AttalServer::sendAskCreatureFlee( GenericPlayer * player,GenericMapCreature * creature )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendAskCreatureFlee( creature );
	}
}

void AttalServer::updateUnits( GenericPlayer * player,GenericLord * lord )
{
	AttalPlayerSocket * socket = findSocket( player );
	
	if( lord && socket ) {
		if( player == lord->getOwner() ) {
			socket->sendLordUnits( lord );
		} else {
			/// XXX: would be better to send category of units and not real values
			socket->sendLordUnits( lord );
		}
	}

}

void AttalServer::updateUnits( const QList<GenericPlayer *> & list, GenericLord * lord )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( lord ) {
			if( socket && socket->canSee( lord->getCell() ) ) {
				socket->sendLordUnits( lord );
			}
		}
	}
}


void AttalServer::updateUnit( GenericPlayer * player,GenericLord * lord , int num)
{
	AttalPlayerSocket * socket = findSocket( player );
	
	if( lord  && socket) {
		if( player == lord->getOwner() ) {
			socket->sendLordUnit( lord , num );
		} else {
			/// XXX: would be better to send category of units and not real values
			socket->sendLordUnit( lord , num);
		}
	}
}

void AttalServer::updateUnit( const QList<GenericPlayer *> & list, GenericLord * lord , int num )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( lord ) {
			if( socket && socket->canSee( lord->getCell() ) ) {
				socket->sendLordUnit( lord , num );
			}
		}
	}
}

void AttalServer::newLord( const QList<GenericPlayer *> & list, GenericLord * lord )
{
	GenericPlayer * player = 0;

	uint nbPlayers = (uint) list.count();
	for( uint k = 0; k < nbPlayers; ++k ) {
		player = list.at( k );
		if( lord ) {
			if( player && player->canSee( lord->getCell() ) ) {
				newLord( player, lord );
			}
		}
	}

}

void AttalServer::newLord( GenericPlayer * player, GenericLord * lord )
{
	AttalPlayerSocket * socket = findSocket( player );

	if( lord && socket ) {
		if( player == lord->getOwner() ) {
			socket->sendLordNew( lord );
			socket->sendLordBaseCharac( lord, MOVE );
			socket->sendLordBaseCharac( lord, MAXMOVE );
			socket->sendLordBaseCharac( lord, TECHNICPOINT );
			socket->sendLordBaseCharac( lord, MAXTECHNICPOINT );
			socket->sendLordBaseCharac( lord, MORALE );
			socket->sendLordBaseCharac( lord, LUCK );
			socket->sendLordBaseCharac( lord, EXPERIENCE );
			socket->sendLordBaseCharac( lord, ATTACK );
			socket->sendLordBaseCharac( lord, DEFENSE );
			socket->sendLordBaseCharac( lord, POWER );
			socket->sendLordBaseCharac( lord, KNOWLEDGE );
			socket->sendLordBaseCharac( lord, CHARISMA );
			socket->sendLordUnits( lord );

			for( uint i = 0; i < lord->getArtefactManager()->getArtefactNumber(); i++ ) {
				socket->sendArtefactLord( lord->getArtefactManager()->getArtefact( i ), true );
			}
			for( uint j = 0; j < lord->getMachineNumber(); j++ ) {
				socket->sendLordMachine( lord, lord->getMachine( j ) );
			}
		} else {
			socket->sendLordVisit( lord );
			/// XXX: would be better to send category of units and not real values
			socket->sendLordUnits( lord );
		}
	}
}

void AttalServer::sendLordVisit( GenericLord * lord, GenericPlayer * player, bool state )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendLordVisit( lord, state );
	}
}

/*
void AttalServer::updateLordBaseUnits( GenericBase * base ,GenericLord * lord , int flag)
{
	uint nbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		if( _theSockets[ i ] != _currentSocket ) {
			//_theSockets[ i ]->sendExchangeBaseUnitCl( base, unit1, lord, unit2 );
			if( flag == 0 || flag == 2 ) {
				for( int i = 0; i < MAX_UNIT; ++i ) {
					_theSockets[ i ]->sendBaseUnit( base, base->getUnit( i ) );
				}
			}	
			if( flag == 1 || flag == 2 ) {
				_theSockets[ i ]->sendLordUnits( lord );
			}
		}
	}
}
	
void AttalServer::updateLord( GenericLord * lord )
{
	uint nbSockets = (uint) _theSockets.count();
	for( uint i = 0; i < nbSockets; ++i ) {
		_theSockets[ i ]->sendLordBaseCharac( lord, MOVE );
		_theSockets[ i ]->sendLordBaseCharac( lord, MAXMOVE );
		_theSockets[ i ]->sendLordBaseCharac( lord, TECHNICPOINT );
		_theSockets[ i ]->sendLordBaseCharac( lord, MAXTECHNICPOINT );
		_theSockets[ i ]->sendLordBaseCharac( lord, MORALE );
		_theSockets[ i ]->sendLordBaseCharac( lord, LUCK );
		_theSockets[ i ]->sendLordBaseCharac( lord, EXPERIENCE );
		_theSockets[ i ]->sendLordBaseCharac( lord, ATTACK );
		_theSockets[ i ]->sendLordBaseCharac( lord, DEFENSE );
		_theSockets[ i ]->sendLordBaseCharac( lord, POWER );
		_theSockets[ i ]->sendLordBaseCharac( lord, KNOWLEDGE );
		_theSockets[ i ]->sendLordUnits( lord );

		for( uint j = 0; j < lord->getArtefactManager()->getArtefactNumber(); j++ ) {
			_theSockets[ i ]->sendArtefactLord( lord->getArtefactManager()->getArtefact( j ), true );
		}
		for( uint k = 0; k < lord->getMachineNumber(); k++ ) {
			_theSockets[ i ]->sendLordMachine( lord, lord->getMachine( k ) );
		}
	}
}
*/

void AttalServer::sendLordRemove( const QList<GenericPlayer *> & list, GenericLord * lord )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	//logDD(" nbSockets %d", nbSockets );
	for( uint k = 0; k < nbSockets; ++k ) {
		//logDD(" k %d", k );
		socket = findSocket(list.at(k));
		if( socket && socket->canSee( lord->getCell() ) ) {
			socket->sendLordRemove( lord );
		}
	}
}

void AttalServer::sendLordRemove( GenericPlayer * player, GenericLord * lord )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket && socket->canSee( lord->getCell() ) ) {
		socket->sendLordRemove( lord );
	}
}

void AttalServer::sendCreatureRemove( const QList<GenericPlayer *> & list, GenericCell * cell )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket && socket->canSee( cell ) ) {
			socket->sendDelCreature( cell );
		}
	}
}

void AttalServer::setGarrison( const QList<GenericPlayer *> & list, GenericLord * lord, bool state )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket ) {
			socket->sendLordGarrison( lord, state );
		}
	}
}


void AttalServer::newBase( GenericPlayer * player, GenericBase * base )
{

	AttalPlayerSocket * socket = findSocket( player );

	if( socket ) {
		socket->sendBaseNew( base );
		for( uint i = 0; i < base->getBuildingCount(); i++ ) {
			socket->sendBaseBuilding( base, base->getBuilding( i ), true );
		}
		for(int i = 0; i < MAX_UNIT; i++ ) {
			socket->sendBaseUnit( base, base->getUnit(i), i );
		}
		socket->sendBaseName( base );
	}
}

void AttalServer::updateBaseBuilding( const QList<GenericPlayer *> & list, GenericBase * base, GenericInsideBuilding * building  )
{
	GenericPlayer * player = 0;

	uint nbPlayers = (uint) list.count();
	for( uint k = 0; k < nbPlayers; ++k ) {
		player = list.at( k );
		if( base ) {
			if( player->canSee( base->getCell() ) ) {
				updateBaseBuilding( player, base, building );
			}
		}
	}

}

void AttalServer::sendBaseStates( GenericPlayer * player, GenericBase * base )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket && socket->canSee( base->getCell() )) {
		for( int i = 0; i < GenericBase::BASE_LAST ; ++i ) {
			bool state = base->getState( (GenericBase::BaseState) i );
			TRACE("AttalServer::sendBaseStates( player %p, base %p, type %d, state %d )", player, base, i, state );
			socket->sendBaseState( base, (GenericBase::BaseState) i , state );
		}
	}
}

void AttalServer::sendBaseState( GenericPlayer * player, GenericBase * base, GenericBase::BaseState type, bool state )
{
	TRACE("AttalServer::sendBaseState( player %p, base %p, type %d, state %d )", player, base, type, state );
	AttalPlayerSocket * socket = findSocket( player );
	if( socket && socket->canSee( base->getCell() )) {
		socket->sendBaseState( base, type , state );
	}
}

void AttalServer::updateBaseBuilding( GenericPlayer * player, GenericBase * base, GenericInsideBuilding * building  )
{
	AttalPlayerSocket * socket = findSocket( player );

	if( socket ) {
		bool found = false;
		for( uint i = 0; i < base->getBuildingCount(); i++ ) {
			GenericInsideBuilding * build = base->getBuilding( i );
			if( build == building ) {
				socket->sendBaseBuilding( base, build, true );
				found = true;
			}
		}
		if( ! found ) {
			socket->sendBaseBuilding( base, building, false );
		}
	}
}

void AttalServer::sendBaseUnit( const QList<GenericPlayer *> & list, GenericBase * base, Creature * creature, int number, int pos )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket && socket->canSee( base->getCell() ) ) {
			socket->sendBaseUnit( base, creature, number, pos );
		}
	}
}

void AttalServer::sendBaseUnits( const QList<GenericPlayer *> & list, GenericBase * base )
{
	for(int i = 0; i < MAX_UNIT; i++ ) {
		if( base->getUnit(i) ) {
			sendBaseUnit( list, base, base->getUnit(i)->getCreature(), base->getUnit(i)->getNumber() , i );
		} else {
			sendBaseUnit( list, base, 0, 0, i );
		}
	}
}

void AttalServer::sendBaseResource( GenericPlayer * player, GenericBase * base,char ressource, int nb )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket && socket->canSee( base->getCell() )) {
		socket->sendBaseResource( base, ressource, nb );
	}
}

void AttalServer::sendBaseResources( GenericPlayer * player, GenericBase * base )
{
	AttalPlayerSocket * socket = findSocket( player );
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( socket && socket->canSee( base->getCell() )) {
			socket->sendBaseResource( base, i, base->getResourceList()->getValue( i ) );
		}
	}
}

void AttalServer::sendBaseProduction( GenericPlayer * player, GenericBase * base )
{
 	AttalPlayerSocket * socket = findSocket( player );

	if( socket && socket->canSee( base->getCell() ) ) {
		int creatCount = base->getCreatureNumber();
		for( int i = 0; i < creatCount; i ++) {
			socket->sendBaseProduction(base , base->getCreatureProduced(i));
		}
	}

}

void AttalServer::sendBaseProduction( const QList<GenericPlayer *> & list, GenericBase * base )
{
	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		sendBaseProduction( list.at(k), base );
	}

}

void AttalServer::sendLordExchange( GenericPlayer * player,  GenericLord * lord1, GenericLord * lord2 )
{
 	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendExchangeStart( lord1, lord2 );
	}
}

void AttalServer::sendExchangeUnit( const QList<GenericPlayer *> & list, GenericLord * lord1, uchar unit1, GenericLord * lord2, uchar unit2 )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket && ( socket->canSee( lord1->getCell() ) || socket->canSee( lord2->getCell() )) ) {
			socket->sendExchangeUnit( lord1, unit1, lord2, unit2 );
		}
	}
}

void AttalServer::sendExchangeArtefact( const QList<GenericPlayer *>  & /*list*/, GenericLord * /* lord1 */, int /* item */, GenericLord * /* lord2 */ )
{
	/// XXX: _currentSocket->sendExchangeArtefact( lord1, item, lord2 ); unused ??
}

void AttalServer::sendExchangeBaseUnitCl( const QList<GenericPlayer *> & list, GenericBase * base, uchar unit1, GenericLord * lord, uchar unit2 )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( base && socket && socket->canSee( base->getCell() ) ) {
			socket->sendExchangeBaseUnitCl( base, unit1, lord, unit2 );
		}
	}
}

void AttalServer::ownBase( GenericPlayer * player, GenericBase * base )
{
 	AttalPlayerSocket * socket = findSocket( player );

	/// XXX: owner could be computed in AttalSocket
	GenericPlayer * owner = base->getOwner();
	if( socket ) {
		socket->sendBaseOwner( base, owner );
	}
}

void AttalServer::ownBase( const QList<GenericPlayer *> & list, GenericBase * base )
{
	GenericPlayer * player = 0;

	uint nbPlayers = (uint) list.count();
	for( uint k = 0; k < nbPlayers; ++k ) {
		player = list.at( k );
		if( base ) {
			if( player->canSee( base->getCell() ) ) {
				ownBase( player, base );
			}
		}
	}
}

/*!

*/

void AttalServer::newBuilding( GenericPlayer * player,  GenericBuilding * build )
{
 	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendBuildingNew( build );
	}
}

void AttalServer::ownBuilding( const QList<GenericPlayer *> & list, GenericBuilding * build )
{
	GenericPlayer * player = 0;

	uint nbPlayers = (uint) list.count();
	for( uint k = 0; k < nbPlayers; ++k ) {
		player = list.at( k );
		if( build ) {
			if( player->canSee( build->getCell() ) ) {
				ownBuilding( player, build );
			}
		}
	}

}

void AttalServer::ownBuilding(  GenericPlayer * player, GenericBuilding * build )
{
 	AttalPlayerSocket * socket = findSocket( player );

	/// XXX: owner could be computed in AttalSocket
	GenericPlayer * owner = build->getOwner();
	if( socket && socket->canSee( build->getCell() ) ) {
		socket->sendBuildingOwner( build, owner );
	}
}

void AttalServer::sendBuildingResource( GenericPlayer * player, GenericBuilding * build,char ressource, int nb )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket && socket->canSee( build->getCell() ) ) {
		socket->sendBuildingResource( build, ressource, nb );
	}
}

void AttalServer::sendBuildingResources( GenericPlayer * player, GenericBuilding * build )
{
	AttalPlayerSocket * socket = findSocket( player );
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( socket && socket->canSee( build->getCell() )  ) {
			socket->sendBuildingResource( build, i, build->getResourceList()->getValue( i ) );
		}
	}
}

void AttalServer::ownArtefact( GenericLordArtefact * artefact, GenericPlayer * player )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendArtefactLord( artefact, true );
	}
}

void AttalServer::newCreature( GenericPlayer * player, GenericMapCreature * creature )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendNewCreature( creature );
	}
}

void AttalServer::updateCreature( const QList<GenericPlayer *> & list, GenericMapCreature * creature )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket && socket->canSee( creature->getCell() ) ) {
			socket->sendUpdateCreature( creature );
		}
	}

}

void AttalServer::newEvent( GenericPlayer * player, GenericEvent * event )
{
	AttalPlayerSocket * socket = findSocket( player );

	if( socket && socket->canSee( event->getCell() ) ) {
		socket->sendNewEvent( event );
	}
}

void AttalServer::delEvent( const QList<GenericPlayer *> & list, GenericEvent * event )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket->canSee( event->getCell() ) ) {
			socket->sendDelEvent( event );
		}
	}

}

void AttalServer::sendMvt( const QList<GenericPlayer *> & list, int lord, int i, int j )
{
	AttalPlayerSocket * socket; 

	uint nbSockets = (uint) list.count();
	for( uint k = 0; k < nbSockets; ++k ) {
		socket = findSocket(list.at(k));
		if( socket && socket->canSee( i, j ) ) {
			socket->sendMvt( lord, i, j );
		}
	}
}

void AttalServer::sendLordCharac( GenericPlayer * player, GenericLord * lord, LordCharac charac )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		socket->sendLordBaseCharac( lord, charac );
	}
}

void AttalServer::sendLordCharacs( GenericPlayer * player, GenericLord * lord )
{
	AttalPlayerSocket * socket = findSocket( player );
	if( socket ) {
		for( int i = 0; i < LAST ; ++i ) {
			socket->sendLordBaseCharac( lord, ( LordCharac ) i );
		}
	}
}
