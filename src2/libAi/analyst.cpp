/****************************************************************
 **
 ** Attal : Lords of Doom
 **
 ** analyst.cpp
 ** analyse the game and play
 **
 ** Version : $Id: analyst.cpp,v 1.106 2014/06/22 19:27:31 lusum Exp $
 **
 ** Author(s) : Pascal Audoux - Sardi Carlo
 **
 ** Date : 06/02/2001
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

#include "analyst.h"

// generic include files
#include <algorithm>
#include <math.h>
#include <stdlib.h>
// include files for QT
// application specific include files
#include "libCommon/artefactManager.h"
#include "libCommon/attalSocket.h"
#include "libCommon/attalTemplates.h"
#include "libCommon/calendar.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericChest.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericMap.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/pathFinder.h"
#include "libCommon/priceMarket.h"
#include "libCommon/quest.h"
#include "libCommon/questManager.h"

extern QString VERSION;

//
// ----- Analyst -----
//

Analyst::Analyst()
: QThread(), GameData()
{
	_socket = NULL;
	_map = new GenericMap();
	_player = new GenericPlayer( _map );

	initSocket();
	
	reinitAi();

}

Analyst::~Analyst()
{
	TRACE("~Analyst socket %p", _socket);

	delete _socket;

	while( ! _players.isEmpty() ) {
		delete _players.takeFirst();
	}
}

void Analyst::initSocket()
{
	_socket = new AttalSocket();
	Q_ASSERT( _socket );

	connect( this, SIGNAL(finished()), this, SLOT(quit()));
	connect( this, SIGNAL(finished()), this, SLOT(deleteLater()));
	connect( this, SIGNAL(destroyed()), this , SLOT(quit()));

	connect( _socket, SIGNAL( readyRead() ), this ,SLOT( slot_readSocket() ) );
	connect( _socket, SIGNAL( disconnected()),SLOT(slot_connectionClosed()));
	connect( _socket, SIGNAL( connected()),SLOT(slot_connected()));
	connect( _socket, SIGNAL( error( QAbstractSocket::SocketError ) ), SLOT( slot_error( QAbstractSocket::SocketError ) ) );
}

void Analyst::reinitAi()
{
	reinit();

	while( ! _players.isEmpty() ) {
		delete _players.takeFirst();
	}

	_player->cleanData();
	_map->clearData();

	_nbLord = 0;
	_nbLordRead = 0;
	_fight = NULL;

	_numTurn=-1;
	_numlords=0;
	_totalTurn = 0 ;

	_maxExplRadius = 8;

	srand( time( NULL ) );

	_SuffisantPower = 1000;

}

void Analyst::connectAi(  QString host, int port )
{
	TRACE("connectAi socket %p, host %s , port %d", _socket, qPrintable(host) , port );
	TRACE("connectAi state %d", _socket->state() );
	ialogV("connectAi socket %p, host %s , port %d", _socket, qPrintable(host) , port );
	ialogV("connectAi socket %p, host %s , port %d", _socket, qPrintable(host) , port );

	if( _socket->state() == QAbstractSocket::UnconnectedState  ) {
		_socket->connectToHost( host, port );
	}
}

void Analyst::disconnectAi()
{
	_socket->disconnectFromHost();
	/* faster but dirty , _socket->abort() */
	reinit();
}

void Analyst::initLords()
{
	uint nbLords = DataTheme.lords.count();
	for( uint i = 0; i < nbLords; i++ ) {
		AiLord * lord = new AiLord( i );
		_lords.append((GenericLord *) lord );
	}

}

void Analyst::slot_connectionClosed()
{
	TRACE("ai disconnected");
}

void Analyst::slot_connected()
{
	TRACE("connected");
}

void Analyst::slot_error( QAbstractSocket::SocketError error )
{
	logEE( "Cannot connect to server" );
	switch( error ) {
		case QAbstractSocket::ConnectionRefusedError: 
			logEE( "Connection Refused" );
			break;
		case QAbstractSocket::HostNotFoundError:
			logEE( "Host not found" );
			break;
		case QAbstractSocket::SocketAccessError: 
			logEE( "Access to the socket failed" );
			break;
		default:
			logEE( "Other socket error" );
			break;
	}
	//maybe useless
	disconnectAi(); 
}


void Analyst::slot_readSocket()
{
	_socket->readData();
	ialogV( "Socket received %d | %d | %d", getCla1(), getCla2(), getCla3() );

	switch( getCla1() ) {
		case SO_MSG:
			socketMsg();
			break;
		case SO_CONNECT:
			socketConnect();
			break;
		case SO_MVT:
			socketMvt();
			break;
		case SO_TECHNIC:
			socketTechnic();
			break;
		case SO_FIGHT:
			socketFight();
			break;
		case SO_QR:
			socketQR();
			break;
		case SO_EXCH:
			socketExch();
			break;
		case SO_MODIF:
			socketModif();
			break;
		case SO_TURN:
			socketTurn();
			break;
		case SO_GAME:
			socketGame();
			break;
		default:
			ialogC( "Socket class unknown" );
			printSocket;
			break;
	}


	if( _socket->bytesAvailable() > 0 )
		slot_readSocket();
}

void Analyst::socketMsg()
{
	ialogV( "SO_MSG" );
	/*int cla2 =*/ _socket->getCla2();
	QString msg;
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		msg[i] = _socket->readChar();
	}

}

void Analyst::socketConnect()
{
	ialogV( "SO_CONNECT" );
	switch( _socket->getCla2() ) {
		case C_CONN_OK:
			ialogN( "Receive: Connect Ok" );
			_socket->sendConnectionVersion( VERSION );
			_socket->sendConnectionName( "IA" );
			break;
		case C_CONN_ID:
			{
				_player->setNum(_socket->readChar());
				ialogN( "Receive: Connect Id = %d", _player->getNum() );
			}
			break;
		case C_CONN_NAME:	
			socketConnectName();
			break;
		case C_CONN_PLAYER:
			ialogN( "Receive: Connect Player" );
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketConnectName()
{

	QString res;
	uint len = _socket->readChar();
	for( uint i = 0; i < len; i++) {
		res.append( _socket->readChar() );
	}
	_player->setConnectionName(res);
	ialogN( "Receive: Connect Name %s", qPrintable(res));
}

void Analyst::socketMvt()
{
	ialogV( "SO_MVT" );
	uchar lord = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	GenericCell * newCell;

	try {
		newCell = _map->at( row, col );
	} catch (const  char * err ) {
		//avoid inMap
		logEE("%s", err);
		return;
	}

	AiLord * theLord;

	theLord = (AiLord * ) getLord( lord );

	theLord->setCell( newCell );


	/// TODO: manage enter inside buildings
	if( newCell->getBuilding() != 0 ) {
		if( newCell->getBuilding()->getOwner() != theLord->getOwner()) {
			newCell->getBuilding()->setOwner(theLord->getOwner()); 
		}	
		if( theLord->getOwner() == _player ) {
			/// TODO: enter( _player->getSelectedLord(), newCell->getBuilding() );
		}
	} else if( newCell->getBase() != 0 ) {
		if( theLord->getOwner() == _player ) {
			enterBase(theLord, newCell->getBase());
		}
	}
}


void Analyst::socketTechnic()
{
	ialogV( "SO_TECHNIC" );
}

void Analyst::socketFight()
{
	ialogV( "SO_FIGHT" );

	switch( _socket->getCla2() ) {
		case C_FIGHT_INIT:
			socketFightInit();
			break;
		case C_FIGHT_END:
			socketFightEnd();
			break;
		default:
			if( _fight ) {
				_fight->handleFightSocket();
			} else {
				logEE( "Should not happen" );
			}
			break;
	}
}

void Analyst::socketFightInit()
{
	ialogN( "FIGHT INIT" );
	TRACE( "FIGHT INIT" );
	if( ! _fight ) {
		_fight = new FightAnalyst( this, _socket );
	}
	_fight->handleFightSocket();
}

void Analyst::socketFightEnd()
{
	ialogN( "FIGHT END" );
	TRACE( "ai: FIGHT END" );

	_fight->updateUnits();
	delete _fight;
	_fight = NULL;
	if(InTurn()){ 
		ialogV( "FIGHT END creature" );
		sendLordTurn( C_ANALYST_SAME_LORD );
	}

}

void Analyst::socketQR()
{
	ialogV( "SO_QR" );
	switch( _socket->getCla2() ) {
		case C_QR_MSG_NEXT:
			socketQRMsgNext();
			break;
		case C_QR_MSG_END: 
			socketQRMsgEnd();
			break;
		case C_QR_LEVEL: 
			{
				ialogV( "level" );
				//_socket->sendAnswer( 0 );
			} break;
		case C_QR_CHEST: 
			ialogV( "Chest" );
			_socket->sendAnswerEnum( 0 );
			sendLordTurn( C_ANALYST_SAME_LORD );
			break;
		case C_QR_CREATURE_FLEE: 
			_socket->sendAnswerYesNo( true );
			sendLordTurn( C_ANALYST_SAME_LORD );
			break;
		case C_QR_CREATURE_MERCENARY:
			_socket->sendAnswerYesNo( false );
			sendLordTurn( C_ANALYST_SAME_LORD );
			break;
		case C_QR_CREATURE_JOIN: 
			_socket->sendAnswerYesNo( true );
			sendLordTurn( C_ANALYST_SAME_LORD );
			break;
		case C_QR_ANSWER:
			logEE( "Should not happen" );
			break;
		default:
			printSocket;
			break;
	}

}

void Analyst::socketQRMsgNext()
{
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		_msg.append( _socket->readChar() );
	}
	ialogV( "Information: %s", qPrintable(_msg) );
}

void Analyst::socketQRMsgEnd()
{
	/*uchar type = */_socket->readChar();
	socketQRMsgNext();
	ialogV( "Information: %s", qPrintable(_msg) );
	_msg = "";
}

void Analyst::socketExch()
{
	ialogV( "SO_EXCH" );

	switch( _socket->getCla2() ) {
		case C_EXCH_START:
			exchangeStart();
			break;
		case C_EXCH_ARTEFACT:
			exchangeArtefact();
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::exchangeStart()
{
	/// XXX: not finished...
	//uchar idLord1 = _socket->readChar();
	//uchar idLord2 = _socket->readChar();
	//AiLord * lord1 = _lords.at( idLord1 );
	//AiLord * lord2 = _lords.at( idLord2 );
}

void Analyst::exchangeArtefact()
{
	uchar idLord1 = _socket->readChar();
	int item = _socket->readInt();
	uchar idLord2 = _socket->readChar();

	exchangeArtefactLord( idLord1, idLord2, item);
}

void Analyst::socketModif()
{
	ialogV( "SO_MODIF" );
	switch( _socket->getCla2() ) {
		case C_MOD_MAP:
			socketModifMap();
			break;
		case C_MOD_CELL:
			socketModifCell();
			break;
		case C_MOD_PLAYER:
			socketModifPlayer();
			break;
		case C_MOD_LORD:
			socketModifLord();
			break;
		case C_MOD_BASE:
			socketModifBase();
			break;
		case C_MOD_BUILD:
			socketModifBuilding();
			break;
		case C_MOD_ARTEFACT:
			socketModifArtefact();
			break;
		case C_MOD_CREATURE:
			socketModifCreature();
			break;
		case C_MOD_EVENT:
			socketModifEvent();
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketModifMap()
{
	int h = _socket->readInt();
	int w = _socket->readInt();
	_map->newUnknownMap( h, w );
}

void Analyst::socketModifCell()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int type = _socket->readInt();
	uchar diversification = _socket->readChar();
	int a4 = _socket->readInt();
	int a5 = _socket->readInt();
	int a6 = _socket->readInt();
	int a7 = _socket->readInt();

	_map->changeCell( row, col, type, a4, a5, a6, a7, diversification );

}

void Analyst::socketModifLord()
{
	switch( _socket->getCla3() ) {
		case C_LORD_VISIT:
			socketModifLordVisit();
			break;
		case C_LORD_NEW:
			socketModifLordNew();
			break;
		case C_LORD_CHARAC:
			socketModifLordCharac();
			break;
		case C_LORD_UNIT:
			socketModifLordUnit();
			break;
		case C_LORD_REMOVE:
			socketModifLordRemove();
			break;
	}
}

void Analyst::socketModifLordVisit()
{
	uchar num = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();
	uchar present = _socket->readChar();

	TRACE("Analyst::socketModifLordVisit() num %d, row %d, col %d, id %d, present %d", num, row, col, id , present );
	
	/* AiLord * lord = (AiLord *) */  modifLordVisit( row, col, id, num, present );

}

void Analyst::socketModifLordNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();

	/* AiLord * lord = (AiLord *) */ modifLordNew( row, col, id );
}

void Analyst::socketModifLordUnit()
{
	uchar id = _socket->readChar();
	uchar pos = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();
	uchar move = _socket->readChar();
	int health = _socket->readInt();

	modifLordUnit( id ,pos , race, level, nb, move, health );

}

void Analyst::socketModifLordRemove()
{

	TRACE("Analyst::socketModifLordRemove()");

	int idLord = _socket->readChar();
	AiLord * lord = (AiLord *) _lords.at( idLord );
	//GenericPlayer * player = lord->getOwner();
	if(lord) {
		lord->removeFromGame();
	}

}

void Analyst::socketModifLordCharac()
{
	char lord = _socket->readChar();
	char charac = _socket->readChar();
	int nb = _socket->readInt();
	
	modifLordCharac( lord , charac, nb );
}

void Analyst::socketModifLordGarrison()
{
	/*uchar lord =*/ _socket->readChar();
	/*uchar state =*/ _socket->readChar();
}

void Analyst::socketModifLordMachine()
{
	uchar lord = _socket->readChar();
	uchar id = _socket->readChar();
	if(_lords.at( lord )) {
		_lords.at( lord )->addMachine( id );
	}
}

void Analyst::socketModifPlayer()
{
	switch( _socket->getCla3() ) {
		case C_PLAY_RESS: 
			socketModifRess( _player , _socket);
			break;
		case C_PLAY_PRICE: {
			uchar ress = _socket->readChar();
			uint price = _socket->readInt();
			_player->getPriceMarket()->setResourcePrice( ress, price);
		}
		 break;
	}
}

void Analyst::socketModifBase()
{
	switch( _socket->getCla3() ) {
		case C_BASE_NEW:
			socketModifBaseNew();
			break;
		case C_BASE_OWNER:
			socketModifBaseOwner();
			break;
		case C_BASE_NAME:
			socketModifBaseName();
			break;
		case C_BASE_BUILDING:
			socketModifBaseBuilding();
			break;
		case C_BASE_UNIT:
			socketModifBaseUnit();
			break;
		case C_BASE_RESS:
			socketModifBaseResources();
			break;
		case C_BASE_PRODUCTION:
			socketModifBaseProduction();
			break;
		case C_BASE_STATE:
			socketModifBaseState();
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketModifBaseNew()
{
	uchar race = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();
	int nb = _socket->readChar();
	QList<uchar> forbidlist;

	for( int i = 0; i < nb; i++ ) {
		forbidlist.append( _socket->readChar() );
	}

	getNewBase( race, row, col, id, nb, forbidlist);

}

void Analyst::socketModifBaseOwner()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar playerNum = _socket->readChar();
	
	modifBaseOwner( row, col,  playerNum );

}

void Analyst::socketModifBaseName()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uint length = _socket->readInt();
	QString name;

	for( uint i = 0; i < length; i++ ) {
		name[i] = _socket->readChar();
	}

	modifBaseName( row, col, name );
}

void Analyst::socketModifBaseResources()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	TRACE("Analyst::socketModifBaseResources row %d, col  %d", row,col );

	try {
		socketModifRess(_map->at( row, col )->getBase() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}

}

void Analyst::socketModifBaseProduction()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();
	
	modifBaseProduction( row, col, race , level, number );
}

void Analyst::socketModifBaseState()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int  type = _socket->readInt();
	bool state = _socket->readChar();
	
	modifBaseState( row, col, type,	state );
}

void Analyst::socketModifBaseBuilding()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar level = _socket->readChar();
	bool create = (bool)_socket->readChar();

	modifBaseBuilding( row, col, level, create );
}

void Analyst::socketModifBaseUnit()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar pos = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();

	modifBaseUnit( row, col, pos, race , level, number );
}

void Analyst::socketModifBuilding()
{
	switch( _socket->getCla3() ) {
		case C_BUILD_NEW: 		
			socketModifBuildingNew();
			break;
		case C_BUILD_OWNER: 	
			socketModifBuildingOwner();
			break;
		case C_BUILD_RESS:
			socketModifBuildingResources();
			break;
		default:
			printSocket;
			logDD("Should not happens");
			break;
	}
}

void Analyst::socketModifBuildingNew()
{
	uchar type = _socket->readChar();
	int id = _socket->readInt();
	int row = _socket->readInt();
	int col = _socket->readInt();

	getNewBuilding( type, id , row, col);

}

void Analyst::socketModifBuildingOwner()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int playNum = _socket->readInt();

	modifBuildingOwner( row, col, playNum );

}

void Analyst::socketModifBuildingResources()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	TRACE("Analyst::socketModifBuildingOwner row %d, col  %d", row,col);

	try {
		socketModifRess( _map->at( row, col )->getBuilding() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void Analyst::socketModifArtefact()
{
	switch( _socket->getCla3() ) {
		case C_ART_DELLORD: 
			{
				uint type = _socket->readInt();
				char lord = _socket->readChar();
				_lords.at( lord )->getArtefactManager()->removeArtefactByType( type );
			} break;
		case C_ART_ADDLORD: 
			{
				uint type = _socket->readInt();
				char lord = _socket->readChar();
				if( ! _lords.at( lord )->getArtefactManager()->hasArtefactType( type ) ) {
					_lords.at( lord )->getArtefactManager()->addArtefact( type );
				}
			} break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketModifEvent()
{
	switch( _socket->getCla3() ) {
		case C_EVENT_NEW: 
			socketModifEventNew();
			break;
		case C_EVENT_DEL:
			socketModifEventDel();
			break;
	}
}

void Analyst::socketModifEventNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	GenericEvent::EventType type = (GenericEvent::EventType) _socket->readChar();
	GenericEvent * event;

	uchar nbParam;
	uchar typeArtefact;
	uchar typeBonus;
	int id;
	GenericArtefact * artefact;
	GenericBonus * bonus;
	GenericChest * chest;

	switch( type ) {
		case GenericEvent::EventArtefact:
			id = _socket->readInt();
			typeArtefact = _socket->readChar();

			event = getNewArtefact();
			artefact = event->getArtefact();
			artefact->setId( id );
			artefact->setType( typeArtefact );
			break;

		case GenericEvent::EventBonus:
			typeBonus = _socket->readChar();
			nbParam = _socket->readChar();

			event = getNewBonus();
			bonus = event->getBonus();
			bonus->setType( (GenericBonus::BonusType) typeBonus );
			for( uint i = 0; i < nbParam; i++ ) {
				bonus->addParam( _socket->readInt() );
			}
			bonus->setupBonus();
			break;

		case GenericEvent::EventChest:
			nbParam = _socket->readChar();

			event = getNewChest();
			chest = event->getChest();
			for( uint i = 0; i < nbParam; i++ ) {
				chest->addParam( _socket->readInt() );
			}
			break;

		default: // this makes sure that setCell isn't called without event being initialized, however possibly the old behavior was desired?
		case GenericEvent::EventNone:
			return;
			break;

	}

	event->setCell( _map->at( row, col ) );
	_map->at( row, col )->setEvent( (GenericEvent *) event );
}

void Analyst::socketModifEventDel()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	removeEvent( row, col );
}

void Analyst::socketModifCreature()
{
	switch( _socket->getCla3() ) {
		case C_CRE_NEW:
			socketModifCreatureNew();
			break;
		case C_CRE_UPDATE:
			socketModifCreatureUpdate();
			break;
		case C_CRE_RESS: 
			socketModifCreatureRess();
			break;
		case C_CRE_DEL:
			socketModifCreatureDel();
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketModifCreatureNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();
	bool looking = (bool) _socket->readChar();

	getNewMapCreature( row, col, race, level, nb, looking);
}

void Analyst::socketModifCreatureUpdate()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int nb = _socket->readInt();
	GenericMapCreature * creature = _map->at( row, col )->getCreature();
	if( creature ) {
		creature->setCategoryNumber( nb );
	}
}

void Analyst::socketModifCreatureDel()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	removeMapCreature( row, col );
}

void Analyst::socketModifCreatureRess()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	try {
		socketModifRess( _map->at( row, col )->getCreature() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void Analyst::socketGame()
{
	ialogN( "SO_GAME" );
	switch( _socket->getCla2() ) {
		case C_GAME_END :
			socketGameEnd();
			break;
		case C_GAME_LOST :
			socketGameLost();
			break;
		case C_GAME_WIN :
			socketGameWin();
			break;
		case C_GAME_BEGIN :
			socketGameBegin();
			break;
		case C_GAME_INFO :
			socketGameInfo();
			break;
		case C_GAME_TAVERN : 
			socketGameTavern();
			break;
		case C_GAME_CALENDAR:
			socketGameCalendar();
			break;
		default : 
			logEE( "Should not happens" );
			printSocket;
			break;
	}

}

void Analyst::socketGameEnd()
{
	reinitAi();
	ialogN( "SO_GAME_END" );
}

void Analyst::socketGameLost()
{
	TRACE( "SO_GAME_LOST" );

	int nb = _socket->readChar();
	if(nb == _player->getNum()) {
		TRACE("I'm the loser, i'm num %d",nb);
		ialogN("I'm the loser, i'm num %d",nb);
	} else {
		TRACE("player %d has lost",nb);
		ialogN("player %d has lost",nb);
	}
	ialogN( "SO_GAME_LOST" );
}

void Analyst::socketGameWin()
{
	int nb = _socket->readChar();
	if(nb == _player->getNum()) {
		ialogN("I'm the winner, i'm num %d",nb);
	} else {
		ialogN("player %d has win",nb);
	}
	ialogN( "SO_GAME_WIN" );
}

void Analyst::socketGameBegin()
{
	ialogN( "SO_GAME_ BEGIN" );
	
	int nb = _socket->readChar();
	
	TRACE("Analyst::beginGame int nb %d", nb);

	setPlayerNumber( nb );
}

void Analyst::socketGameTavern()
{
	ialogV( "SO_GAME_TAVERN " );
	TRACE( "SO_GAME_TAVERN " );
	switch( _socket->getCla3() ) {
		case C_TAVERN_INFO:  
			socketGameTavernInfo();
			break;
		case C_TAVERN_LORD: 
			socketGameTavernLord();
			break;
		default:
			printSocket;
			break;
	}

}

void Analyst::socketGameTavernInfo()
{
	TRACE( "Analyst::socketGameTavernInfo");
	int row = _socket->readInt();
	int col = _socket->readInt();
	_nbLord = _socket->readChar();
	GenericBase * base =	_map->at( row, col )->getBase();
	if( base != _basetav ) {
		return;
	}
	_nbLordRead = 0;
	_lordtav.clear();
	if( _nbLordRead < _nbLord && _basetav && InTurn() ) {
		_socket->askTavernLord( _basetav, _nbLordRead );
	}
}

void Analyst::socketGameTavernLord()
{
	TRACE( "Analyst::socketGameTavernLord");
	int row = _socket->readInt();
	int col = _socket->readInt();
	uint idLord = _socket->readInt();
	GenericBase * base =	_map->at( row, col )->getBase();
	if( base != _basetav ) {
		return;
	}
	AiLord * lord = new AiLord( idLord );
	_lordtav.append( lord );
	if(_player->numLord() < 2 * _player->numBase() && InTurn() ) {
		if(_player->canBuy(DataTheme.lords.at(idLord)) && !_basetav->isVisitorLord()) {
			_socket->sendLordBuy( lord , row, col);
		}
	}
}

void Analyst::socketGameInfo()
{
	ialogN( "SO_GAME_INFO " );
	switch( _socket->getCla3() ) {
		case C_INFOPLAYER_TEAM: 
			{
				uchar player = _socket->readChar();	
				uchar teamId = _socket->readChar();
				if( getPlayer( player ) ) {
					getPlayer( player )->setTeam( teamId );
				}
				if( player == _player->getNum() ) {
					_player->setTeam( teamId );
				}
			} break;
		case C_INFOPLAYER_NAME:
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketGameCalendar()
{
	_calendar->setDateByType( 0 , _socket->readInt());
	_calendar->setDateByType( 1 , _socket->readInt());
	_calendar->setDateByType( 2 , _socket->readInt());
	_calendar->setDateByType( 3 , _socket->readInt());
	_calendar->setDateByType( 4 , _socket->readInt());
}

void Analyst::socketTurn()
{
	ialogV( "SO_TURN" );

	switch( _socket->getCla2() ) {
		//case C_TURN_BEG:
		//	socketTurnBegin();
		//	break;
		case C_TURN_PLAY:
			socketTurnPlay();
			break;
		case C_TURN_END:
			ialogC( "Should not happen (SO_TURN/C_TURN_END)" );
			break;
		case C_TURN_LORD:
			ialogC( "Should not happen (SO_TURN/C_TURN_LORD)" );
			break;
		case C_TURN_PLORD:
			socketTurnLord();
			break;
		default:
			printSocket;
			break;
	}
}

void Analyst::socketTurnPlay()
{
		
	uint num = _socket->readChar();
	TRACE( "Analyst::socketTurnPlay Player: %d,get num %d", num,_player->getNum() );
	if( num == (uint) _player->getNum() ) {
		socketTurnBegin();
	} else {
		ialogV( "Player: %d,get num %d", num,_player->getNum() );
	}
}


void Analyst::socketTurnBegin()
{
	_totalTurn++ ;
	_currentLord = 0;
	ialogN( "Start turn num. %d" , _totalTurn );
	TRACE("Analyst::socketTurnBegin Start turn num. %d" , _totalTurn );
	if( _calendar->getDay() == 1 ) {
		//_player->newWeek();
	}
	_numTurn = 8;

	for( int j = 0; j < _lords.count(); ++j ) {
		AiLord * tmplord = (AiLord *) _lords.at(j);
		if(  tmplord->getOwner() &&  tmplord->getOwner() != _player  && tmplord->getOwner()->getTeamId() != _player->getTeamId() ) {
			for( int i = 0; i < (int)_player->numBase(); ++i ) {
				GenericBase * base = _player->getBase( i );
				if( GenericMap::computeDist( tmplord->getCell(), base->getCell() ) < 8 /* hardcoded */ ) {
					manageBase( base );
				}
			}
		}
	}
	
	//after enemies
	if( _player->numBase() > 0 ){
		for( int i = 0; i < (int)_player->numBase(); i++ ) {
			//random base, so not use a predefinite pattern
			GenericBase * base = _player->getBase( rand() % _player->numBase() );
			manageBase( base );
		}
	}
	sendLordTurn( C_ANALYST_SAME_LORD );
}

void Analyst::sendLordTurn( int opt )
{
	TRACE("SendLordTurn opt %d", opt );

	if( opt == C_ANALYST_NEXT_LORD ) {
		_numTurn = 8;
	}
	_socket->sendLordTurn( opt );
}

void Analyst::socketTurnLord()
{
	int typelordturn = _socket->readInt();
	int numLord = _player->numLord();
	ialogV("lord number %d",typelordturn);
	TRACE("typelordturn %d numlord %d",typelordturn,numLord);
	//int lordCount = _lords.count();
	TRACE("Start turn currLord %d, numTurn %d", _currentLord, _numTurn);
	if(InTurn()){
		if( typelordturn  == C_ANALYST_NEXT_LORD ) { 
			_currentLord++;
		}


		if( _currentLord >= numLord ){
			ialogN("End turn currLord %d, numTurn %d", _currentLord, _numTurn);
			TRACE("End turn currLord %d, numTurn %d", _currentLord, _numTurn);
			_numTurn=-1;
			_socket->sendTurnEnd();
		} else if( _numTurn == 0 || ((AiLord *)_player->getLord( _currentLord))->isStop() ) {
		((AiLord *)_player->getLord( _currentLord))->setStop( false );
		sendLordTurn( C_ANALYST_NEXT_LORD );
		} else {
			playLordTurn( _currentLord ) ;
		}	
	}
}

void Analyst::playLordTurn( int nlord )
{
	analyzeLord( (AiLord *)  _player->getLord( nlord ) );
}

void Analyst::analyzeLord( AiLord * lord )
{
	ialogV("Analyze lord id %d", lord->getId());
	TRACE("Analyze lord id %d", lord->getId());

	int movePoints = lord->getCharac( MOVE );
	GenericCell * cell = lord->getCell();
	GenericCell * destinationCell;
	int	lordTurn = C_ANALYST_SAME_LORD;

	lord->initParams();
	lord->setStatus( _status );
	lord->setCurPrio(lord->getPriority(PR_LAST));
	lord->setDestCell( cell );

	/* if a lord is already in a base */
	if( cell->getBase() != 0 ) {
		if( lord->getOwner() == _player ) {
			enterBase(lord, cell->getBase());
		}
	}

	/* if lord is powerful, prefer to fight and explore instead cumulate units */
	if(lord->computeForceIndicator(true) > _SuffisantPower){
		lord->setPriority(PR_BASEOWNER,(PR_EXPLORE*10)+5);
		lord->setPriority(PR_CREAT,(PR_BASE*10)-1);
		//ialogV("prio %d",lord->getPower());
		//ialogV("creat %d",lord->getPriority(PR_CREAT));
	} else {
		lord->setPriority(PR_BASEOWNER,(PR_BASEOWNER*10));
		lord->setPriority(PR_CREAT,(PR_CREAT*10));
	}

	PathFinder * path = _map->getPath();

	path->reinit( _map );
	path->computePath( cell );

	if(!path->isNearPath( cell ) ){
		ialogN("no path");
		movePoints = 0;
	}	

	if( movePoints  <= 0 ) {
		ialogV("Turn pass");
		lordTurn = C_ANALYST_NEXT_LORD;

	} else {

		analyzeLordMap( lord );
		_status = lord->getStatus();
		destinationCell = lord->getDestCell();

		if(destinationCell  == cell){
			ialogN("Destination cell = cell");
			TRACE("Destination cell = cell");
			lordTurn = C_ANALYST_NEXT_LORD;
		}

		ialogV("status %d", _status );
		ialogV("startRow %d, startCol %d",cell->getRow(),cell->getCol());
		ialogV("destRow %d, destCol %d",destinationCell->getRow(),destinationCell->getCol());
		TRACE("status %d", _status );
		TRACE("startRow %d, startCol %d",cell->getRow(),cell->getCol());
		TRACE("destRow %d, destCol %d",destinationCell->getRow(),destinationCell->getCol());

		int movePt = lord->getCharac( MOVE );
		QStack<GenericCell *> * cellPathStack = NULL;

		if( _status == FREE ) {
			// if no possibility to move, destination cell = starting cell and sanity check
			if(path->isPath(destinationCell)==false)  {
				ialogV(" no path ");
				destinationCell  = cell;
				goto finish;
			}

			if( path->isPath(destinationCell)==true && destinationCell  != cell){
				cellPathStack = path->giveCells( destinationCell );
			}

		} else if( _status == NOFREE )	{
			if( path->isNearPath(destinationCell)==true && destinationCell  != cell && destinationCell->isStoppable() ){
				cellPathStack = path->giveNearCells( destinationCell );
				cellPathStack->prepend( destinationCell );
			}
		}

		if( cellPathStack ) {
			QList<GenericCell *> list;
			GenericCell * tmp = cell;
			if( !cellPathStack->isEmpty() ) {
				while( ! cellPathStack->isEmpty() ) {
					GenericCell * movCell = cellPathStack->pop();
					int cost = path->computeCostMvt( tmp,movCell );
					if( ( cost != -1 ) && ( cost <= movePt ) && movCell!=cell /* workaround for a strange path function */ ) {
						list.append( movCell );
						tmp = movCell;
						movePt = movePt - cost;
					}
				}
				if( !list.isEmpty() ) {
					_socket->sendMvts( lord->getId(), list );
				} else {
					//due to workaround
				}
			} else {
				logEE("stack empty ");
				logEE("status %d", _status );
				logEE("startRow %d, startCol %d",cell->getRow(),cell->getCol());
				logEE("destRow %d, destCol %d",destinationCell->getRow(),destinationCell->getCol());
			}
			delete cellPathStack;
			cellPathStack = NULL;
		}

	}

finish:
	_numTurn--;
	ialogV("Turn finish %d",lordTurn);
	TRACE("Turn finish %d id %d ",lordTurn, lord->getId());

	sendLordTurn( lordTurn );
}

void Analyst::analyzeLordMap( AiLord * lord )
{
	int i;
	//GenericCell * cell = NULL;

	uint opPower = 0;
	//int cost;

	lord->setStatus( FREE );
	/* the priority with lower value is the most important*/

	for( i = 0; i < _lords.count(); i++ ) {
		AiLord * tmplord = 0;
		tmplord = (AiLord *) _lords.at(i);
		if( tmplord && ( lord->getPriority(PR_ENEMY) <= lord->getCurPrio() )  ) {
			if( tmplord->getCell() ) {
				if(!tmplord->getOwner() ) {
					ialogN( "Try Enemy (no owner)");
					manageMeetingsLord( lord, tmplord );
				} else if( tmplord->getOwner() != _player  && tmplord->getOwner()->getTeamId() != _player->getTeamId() ) {
					ialogN( "Try Enemy Lord");
					manageMeetingsLord(lord, tmplord );
				}
			} 
		}
	} //for
	for( i = 0; i < _bases.count(); i++ ) {
		GenericBase * base = 0;
		base = _bases.at(i);
		if( base && (lord->getPriority(PR_BASE)	<= lord->getCurPrio() )  )	{
			if(!base->getOwner()) {
				ialogN( "Try Base (no owner)");
				manageMeetingsThings(lord,base->getCell() ,PR_BASE);
			} else if( base->getOwner() != _player && base->getOwner()->getTeamId() != _player->getTeamId()) {
				ialogN( "Try Base Enemy");
				manageMeetingsThings(lord, base->getCell(),PR_BASE);
			}
		} 
		if( base && (lord->getPriority(PR_BASEOWNER)	<= lord->getCurPrio() )  )	{
			if( base->getOwner() == _player && base->getState( GenericBase::BASE_UNIT_BOUGHT ) ) {
				ialogN( "Try Base Owned");
				manageMeetingsThings(lord,base->getCell() ,PR_BASEOWNER);
			}
		} 
	} //for
	for( i = 0; i < _events.count(); i++ ) {
		GenericEvent * event = 0;
		event = _events.at(i);
		if( event){
			if( event->getType()==GenericEvent::EventArtefact && ( lord->getPriority(PR_ART) < lord->getCurPrio() )  )	{
				ialogN( "Try Event-Artefact");
				manageMeetingsThings(lord, event->getCell() , PR_ART );
			} 
			if( event->getType()==GenericEvent::EventBonus && ( lord->getPriority(PR_BONUS) < lord->getCurPrio() ) )	{
				ialogN( "Try Event-Bonus");
				manageMeetingsThings(lord, event->getCell() , PR_BONUS );
			} 
			if( event->getType()==GenericEvent::EventChest && ( lord->getPriority(PR_CHEST)< lord->getCurPrio() ) )	{
				ialogN( "Try Event-chest");
				manageMeetingsThings(lord, event->getCell() , PR_CHEST );
			}
			ialogV( "row %d, col %d",event->getCell()->getRow(),event->getCell()->getCol());
		}
	} //for
	for( i = 0; i < _buildings.count(); i++ ) {
		GenericBuilding * building = 0;
		building = _buildings.at(i);
		if( building && (lord->getPriority(PR_BUILD) <= lord->getCurPrio()) ) {
			if(!building->getOwner()) {
				ialogN( "Try Build (no owner)");
				manageMeetingsThings(lord,building->getCell() ,PR_BUILD);
			} else if( building->getOwner() != _player && building->getOwner()->getTeamId() != _player->getTeamId()) {
				ialogN( "Try Build Enemy");
				manageMeetingsThings(lord, building->getCell() ,PR_BUILD_ENEMY );
			} else {
				ialogV( "My Build,ignore");
			}
		} 
	} //for
	for( i = 0; i < _creatures.count(); i++ ) {
		GenericMapCreature * creature = 0;
		creature = _creatures.at(i);
		if( creature ){
			if	(lord->getPriority(PR_CREAT) < lord->getCurPrio()) {
				ialogN( "Try Creat");
				ialogV( "row %d, col %d",creature->getCell()->getRow(),creature->getCell()->getCol());
				opPower = creature->computeForceIndicator( false );
				/* adjust enemy power, be more careful with creatures */
				opPower = (opPower * 150) / 100; 
				manageMeetings(lord,creature->getCell() ,lord->getPriority(PR_CREAT),opPower,false);
			} 
		}
	} //for

	//ai explore map 
	if ( lord->getPriority(PR_EXPLORE) <= lord->getCurPrio()) {
		ialogN( "Try Explore");
		GenericCell * dest = NULL;
		dest = findNotExploredCell(lord->getCell());
		if(dest){
			manageMeetings(lord,dest ,lord->getPriority(PR_EXPLORE),0,false);
		} else {
			dest = randomPath( lord->getCell(), lord->getCharac( MOVE )  );
			manageMeetings(lord,dest ,lord->getPriority(PR_EXPLORE),0,false);
		}
	}

}


void Analyst::manageBase(GenericBase * base)
{
	TRACE("Analyst::manageBase");

	_basetav = 0;
	// scan all buildings looking for creatures
	uint count = base->getBuildingCount();
	uint nbuil;
	ialogN("Try buy");
	for( nbuil = 0; nbuil < count ; nbuil++ ) {
		GenericInsideBuilding * building=base->getBuilding( nbuil );
		InsideAction * action = DataTheme.bases.at( base->getRace() )->getBuildingModel( building->getLevel() )->getAction();
		if(action){		
			switch( action->getType() ) {
				case INSIDE_CREA:
					manageBaseCreature(base , action);
					break;
				case INSIDE_MARKET:
					manageBaseMarket();
					break;
				case INSIDE_TAVERN:
					{
						_basetav = base;
						_socket->askTavernInfo( base );
					}	break;
				default:
					break;
			}
		}
	}

	int bought = 0;
	GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
	int modelCount = baseModel->getBuildingCount();
	InsideBuildingModel * model;

	for( int i = 0; i < modelCount; i++ ) {
		GenericInsideBuilding * building= base->getBuildingByType( i );
		if( _player && _socket &&  !building && bought == 0) {
			model = DataTheme.bases.at( base->getRace() )->getBuildingModel( i ) ;
			/* ai don't buy buildings that not implement actions */
			if (model->getAction()) {
				if(model->getAction()->getType() != INSIDE_NONE) {
					if( _player->canBuy( model ) && base->canBuildBuilding( model ) ) {
						_socket->requestBuilding( base, i, true );
						ialogN("Request Building model %d",i);
						//logDD("Request Building model %d",i);
						bought = 1;
					} else {
						//logDD("Can't buy, Not enough resources to buy this building" );
					}
				}
			}
		}
	}		
	if( base->getCell()->getLord() ) {
		enterBase( (AiLord *)base->getCell()->getLord(), base );
	}	
}

void Analyst::manageBaseMarket()
{

	/* ugly algho, hardcoded values, but fast and it work ... */
	int ress = -1;
	int qty = 0;
	for( int i = 0 ; i < DataTheme.resources.count(); i++ ) {
		if(_player->getResourceList()->getValue( i )> 1000 && _player->getResourceList()->getValue( i )> qty){
			ress = i;
			qty =_player->getResourceList()->getValue( i ) ;
		}
		for( int i = 0 ; i < DataTheme.resources.count(); i++ ) {
			if( ress != -1 ) {
				if( _player->getResourceList()->getValue( i ) < 20 ) {
					//logDD("exchange  ress %d , i %d",ress,i);
					_socket->sendBaseMarket(ress,i,20);
				}
			}
		}
	}

}


void Analyst::manageBaseCreature(GenericBase * base, InsideAction *action)
{

	int race = action->getParam( 0 );
	int level = action->getParam( 1 );
	Creature * creature = DataTheme.creatures.at( race, level);
	int prodBase = base->getCreatureProduction( creature );
	int prodPlayer = _player->computeBuyCreatureMax( creature );
	//logDD(" creature %p, prodBase%d, prodPlayer %d", creature, prodBase, prodPlayer );
	int num = std::min( prodBase, prodPlayer );
	if(_player->canBuy( creature, num )  && num > 0){
		ialogN("BUY  num creat. %d, race %d ,level %d",num,race, level);
		//logDD("BUY  num %d, race %d ,level %d",num,race, level);
		_player->buy(creature,num);
		_socket->sendBaseUnitBuy( base, creature, num);
		//base->buyCreature( creature, num );
		base->setState( GenericBase::BASE_UNIT_BOUGHT, true); 
	}

}

void Analyst::enterBase(AiLord * lord,GenericBase * base )
{
	int i;
	int j;
	ialogN("Enter base");

	base->enter( lord );
	for( i = 0; i < MAX_UNIT; i++ ) {
		for( j = i; j < MAX_UNIT; j++ ) {
			if( lord->getUnit( i ) && lord->getUnit( j ) && i != j ) {
				if( lord->getUnit(i)->getCreature() == lord->getUnit(j)->getCreature() ) {
					_socket->sendExchangeUnit( lord, i, lord, j );
				}
			}
		}
	}
	if( lord->countUnits() < MAX_UNIT ) {
		for( i = 0; i < MAX_UNIT; i++ ) {
			for( j = 0; j < MAX_UNIT; j++ ) {
				if( lord->getUnit( j ) && base->getGarrisonUnit( i ) ) {
					if( lord->getUnit( j )->getCreature() == base->getGarrisonUnit( i )->getCreature() ) {
						_socket->sendExchangeBaseUnit( base, i, lord, j );
					}
				}
			}
		}
		for( i = 0; i < MAX_UNIT; i++ ) {
			for( j = 0; j < MAX_UNIT; j++ ) {
				if( !lord->getUnit( j ) && base->getGarrisonUnit( i ) ) {
					_socket->sendExchangeBaseUnit( base, i, lord, j );
					//ialogN("Exch ,pos i %d ,pos j %d",i,j);
				} 
			}
		}
	}
	base->setState( GenericBase::BASE_UNIT_BOUGHT,false);
	base->out( lord );
}

void Analyst::manageMeetingsLord( AiLord * lord, AiLord * opponent )
{
	ialogV( "Enemy lord row %d, col %d",opponent->getCell()->getRow(), opponent->getCell()->getCol());
	
	LordPriority prioType = PR_ENEMY;
	GenericCell * cell  = opponent->getCell();

	bool flee = true;
	uint opPower = opponent->computeForceIndicator(false);
	uint myPower = lord->computeForceIndicator(true);
	if( myPower < opPower ) {
		PathFinder * path = _map->getPath();

		for( int i = 0; i < (int)_player->numBase(); ++i ) {
			GenericBase * base = _player->getBase( i );
			if( GenericMap::computeDist( lord->getCell(), base->getCell() ) < 7 /* hardcoded */ && path->isPath( base->getCell() ) ) {
				manageMeetingsThings(lord,base->getCell() ,PR_BASEOWNER);
				lord->setStop( true );
				return;
			}
		}
		
		cell = fleePath(lord);

	}

	manageMeetings(lord, cell ,lord->getPriority(prioType),opPower,flee);
}

void Analyst::manageMeetingsThings( AiLord * lord, GenericCell * cell, LordPriority prioType )
{
	ialogV( "Things row %d, col %d",cell->getRow(), cell->getCol());
	manageMeetings(lord, cell ,lord->getPriority(prioType),0,false);
}

void Analyst::manageMeetings( AiLord * lord, GenericCell * cell, int prior, uint opPower, bool isFlee )
{
	int cost;
	uint myPower = lord->computeForceIndicator(true);
	int status;
	int movePoints = lord->getCharac( MOVE );
	PathFinder * path = _map->getPath();

	if(opPower>0) {
		ialogN("opPower %d, myPower %d",opPower,myPower);	
		/* now ai try to match more powerful enemy, not so right, cause is difficult for ai to attack */
		tuneAiPower( opPower );

		/* if creature is more powerful than us, ignore it, obv. only if opPower > 0 ( opPower == 0 , not an enemy ) */	
		if( myPower < opPower && !isFlee)
			return;
	}


	/* if no path to target or cell is not stoppable, ignore it */	
	if(!path->isNearPath( cell ) || !cell->isStoppable() )
		return;

	if( cell->isFree() && cell->getType() != 0 ) {
		cost = path->getDist( cell );
		status = FREE;
	} else {
		GenericCell * nearcell;
		nearcell=path->getNearCell(	cell );
		/* workaround for getNearCell bug? */
		if( nearcell->getRow() > 0 && nearcell->getCol() > 0) {
			cost = path->getDist( nearcell );
		} else {
			return;
		}
		status = NOFREE;
	}

	// if they are more targets with same priority, use that with minimal cost
	// or reset mincost every time we change priority
	if( lord->getCurPrio() != prior ) {
		lord->setMincost(movePoints+100);
	}

	/* is the target is reachable or not, go */
	if( ( cost < lord->getMincost() ) && ( cost > 0 ) && ( myPower > opPower ) ) {
		lord->setMincost(cost);
		lord->setCurPrio(prior);
		lord->setDestCell(cell);
		//ialogN("Meeting");
		lord->setStatus(status);
		return;

		/* flee if enemy is more powerful and near */
	} else if( cost < movePoints &&  myPower < opPower && isFlee) {

		lord->setCurPrio(prior);
		lord->setDestCell(cell);
		lord->setStatus(status);
		return;
	}
	return;
}

GenericCell * Analyst::fleePath( AiLord * lord )
{
	GenericCell * cell = lord->getCell();
	GenericCell * ret = cell;
	GenericCell *  tmp = NULL;
	QList<GenericCell *> list = _map->giveRadiusCell(  cell , 8 );
	PathFinder * path = _map->getPath();

	int maxdist = 0;
	for (int i = 0; i < list.size(); ++i) {
		tmp = list.at( i );
		if( list.at( i ) == cell ) {
			continue;
		}
		int dist = GenericMap::computeDist( lord->getDestCell(), tmp );
		if( path->isPath( tmp ) && dist > maxdist ) {
			maxdist = dist;
			ret = tmp;
		}
	}
	return ret;
}

GenericCell * Analyst::randomPath( GenericCell * cell, int move )
{

	int currRow= cell->getRow();
	int currCol= cell->getCol();
	GenericCell *rcell;
	PathFinder * path = _map->getPath();


	int destRow,destCol;
	int i,j;
	if( move < 2) {
		return cell;
	}

	int semimove = move/ 2;
	int count = 0;
	char direction = 1;

	while( count <= 20 ) {
		i=(int)  (rand() % semimove);
		j=(int)  (rand() % semimove);
		//even subtract, odd add
		direction = ( rand() % 2 ) == 0 ? 1 : -1;
		destRow = currRow + i * direction;
		destCol = currCol + j* direction;
		//logDD(" move %d, destRow  %d = currRow  %d + i %d * direction %d ; destCol %d currCol %d  , j %d", move,  destRow ,currRow, i ,direction,	destCol ,currCol , j);
		if( _map->inMap(destRow,destCol)) {
			if( path->isPath(_map->at(destRow, destCol)) ) {
				rcell= _map->at(destRow,destCol);
				if( rcell != cell ) {
					return rcell;
				}
			}
		}
		count++;
	}
	return cell;

}

GenericCell * Analyst::findNotExploredCell(GenericCell * cell)
{
	TRACE("Analyst::findNotExploredCell cell %p", cell );
	GenericCell * dest = 0;
	QList<GenericCell *> list = _map->giveRadiusCell( cell , (int) _maxExplRadius );
	PathFinder * path = _map->getPath();

	for (int i = 0; i < list.size(); ++i) {

		dest =	list.at(i);
		if( dest->getType() == 0 ) {
			if( path->isNearPath(dest)==true && dest != cell && dest->isStoppable() ){
				return dest;
			}
		}
	}

	if( _maxExplRadius > _map->getWidth() || _maxExplRadius > _map->getHeight() ) {
		_maxExplRadius = 8;
	}

	_maxExplRadius++;
	return NULL;

}

void Analyst::tuneAiPower( uint opPower ) 
{
	/* tune ai based on enemy stength */
	if(opPower > _SuffisantPower  ) {
		_SuffisantPower = (int) (opPower * 1.5);
		ialogV("suffisantPower %d - opPower %d",_SuffisantPower,opPower);
	} else {
		//start exploring
		_SuffisantPower = 10;
	}
}

uchar Analyst::getCla1() 
{
	return _socket->getCla1();
}

uchar Analyst::getCla2() 
{
	return _socket->getCla2();
}

uchar Analyst::getCla3() 
{
	return _socket->getCla3();
}
