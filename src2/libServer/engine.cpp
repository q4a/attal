/****************************************************************
**
** Attal : Lords of Doom
**
** engine.cpp
** the game engine !
**
* Version : $Id: engine.cpp,v 1.161 2013/11/22 12:32:36 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 03/10/2000
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

#include "engine.h"


// generic include files
#include <stdlib.h>
#include <limits>

// include files for QT
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QXmlDefaultHandler>

// application specific includes
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/campaign.h"
#include "libCommon/dataTheme.h"
#include "libCommon/define.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"
#include "libCommon/pathFinder.h"
#include "libCommon/priceMarket.h"
#include "libCommon/questManager.h"


#include "libServer/questionManager.h"
#include "libServer/scenarioDescription.h"
#include "libServer/tavernManager.h"

Engine::Engine( AttalServer * serv )
	:QThread(), GameData()
{
	_currentPlayer = NULL;
	_fight = NULL;
	_server = serv;
	_state = NOT_PLAYING;
	_isProcessing = false;
	_question = new QuestionManager();
	_campaign = NULL;
	_num = 0;
	_tavern = new TavernManager( this );

	//connect( _server, SIGNAL( sig_readEvent( int ) ), SLOT( slot_readSocket( int ) ) );
	connect( _server, SIGNAL( sig_newData( int, AttalSocketData ) ), SLOT( slot_readData( int, AttalSocketData ) ) );
	connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), SLOT( slot_newPlayer( AttalPlayerSocket * ) ) );
	//connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ) );
	connect( _server, SIGNAL( sig_endConnection( QString ) ), SLOT( slot_endConnection( QString ) ) );
	connect( this, SIGNAL( sig_result( int, bool ) ),_server, SIGNAL( sig_result( int , bool) ) );
	connect( this, SIGNAL( sig_endGame( int ) ),_server, SIGNAL( sig_endGame( int ) ) );
}

Engine::~Engine()
{
	TRACE("Engine::~Engine()");

	endGame();
	if( _question ) {
		delete _question;
	}
	if( _tavern ) {
		delete _tavern;
	}
	if(_fight){
		delete _fight;
	}
	_fight = NULL;
}

void Engine::run() 
{
}

void Engine::slot_readData( int num, AttalSocketData data )
{
	//_server->reReadSocketData( num );
	_dataQueue.enqueue( data );

	manageSocketState( num );
	
}

void Engine::manageSocketState( int num )
{
	if(! _isProcessing ) {
		_isProcessing = true;
		_currentData = _dataQueue.dequeue();
		_num = num;

		switch( _state ) {
			case NOT_PLAYING:
				stateNotPlaying( num );
				break;
			case IN_GAME:
				stateInGame( num );
				break;
			case IN_QUESTION:
				stateInQuestion( num );
				break;
			case IN_FIGHT_CREATURE:
			case IN_FIGHT_LORD:
				stateInFight( num );
				break;
		}

		_isProcessing = false;

		if( !_dataQueue.isEmpty() ) {
			manageSocketState( num );
		}
	}

}

void Engine::slot_endConnection( QString name )
{
	for( int i = 0; i < _players.count(); i++ ) {
		if( _players.at( i )->getConnectionName() == name ) {
			_players.removeAll( _players.at( i ) );
		}
	}
}

void Engine::handleMessage( int num )
{
	QString msg;
	uchar d = readChar();
	for( uint i = 0; i < d; i++ ) {
		msg[i] = readChar();
	}
	if( msg.contains(": /")) { 
		QStringList list = msg.split(": /");
		handleCommand( num, list.at(1) );
	} else {
		_server->sendMessage( _players, msg );
	}
}

void Engine::handleCommand( int num,  const QString & cmd )
{
	QString msg = "";

	if( cmd.startsWith("gameinfo") ) {
		msg = getScenarioName() + "\n";
		msg +="Turn " +  QString::number( getTurn() );
		_server->sendMessage( _server->getPlayer( num ) , msg );
	} else if( cmd.startsWith("gamelist") ) {
		uint nbPlayers = (uint) _players.count();
		for( uint i = 0; i < nbPlayers; ++i ) {
			msg  = _players[i]->getConnectionName();
			_server->sendMessage( _server->getPlayer( num ), msg );
		}
	} else {
	}


}

void Engine::stateNotPlaying( int num )
{

	TRACE("Engine::stateNotPlaying");

	uchar c = getCla1();
	switch( c ) {
		case SO_MSG:
			handleMessage( num );
			break;
		case SO_CONNECT:
			break;
		default:
			logEE( "Game not started...%d", c );
			printSocket;
			break;	
	}
}

void Engine::stateInGame( int num )
{
	if( num == _players.indexOf( _currentPlayer ) ) {
		uchar cla1 = getCla1();
		switch( cla1 ) {
		case SO_MSG:
			handleMessage( num );
			break;
		case SO_MVT:
			handleInGameMvt( num );
			break;
		case SO_QR:
			logEE( "Should not happen (Server : SO_QR/... state must be IN_QUESTION)" );
			break;
		case SO_TECHNIC:
			logDD( "Not yet implemented" );
			break;
		case SO_EXCH:
			handleInGameExchange();
			break;
		case SO_FIGHT:
			logEE( "Should not happen : state must be IN_FIGHT");
			break;
		case SO_GAME:{
			uchar cla2 = getCla2();
			switch( cla2 ) {
			case C_GAME_TAVERN:
				handleGameTavern();
				break;
			default:
				logDD( "Not yet implemented %d",cla2 );
				printSocket;
				break;
			}
								 }
			break;
		case SO_MODIF:
			handleInGameModif();
			break;
		case SO_TURN:
			handleInGameTurn();
			break;
		case SO_CONNECT:
			break;
		default:
			logEE( "Unknown socket_class from %d , cla1 %d", num , cla1);
		}
	} else {
		switch( getCla1() ) {
		case SO_MSG:
			handleMessage( num );
			break;
		default:
			logEE( "This player should not play now %d, should play %d", num,  _players.indexOf( _currentPlayer ));
			logEE( "Num. players %d, currentPlayer %p", _players.count(), _currentPlayer );
			logEE( "Players %d name %s, currentPlayer name %s", num, qPrintable( _players.at(num)->getConnectionName() ), qPrintable( _currentPlayer->getConnectionName() ) );
			printSocket;
			break;
		}
	}
}

void Engine::handleGameTavern()
{
	TRACE("Engine::handleGameTavern");

	switch( getCla3() ) {
	case C_TAVERN_INFO:
		handleGameTavernInfo();
		break;
	case C_TAVERN_LORD:
		handleGameTavernLord();
		break;
	default:
		printSocket;
		break;
	}
}

void Engine::handleGameTavernInfo()
{
	TRACE("Engine::handleGameTavernInfo");
	
	try {
		uint row = readInt();
		uint col = readInt();

		GenericCell * cell = _map->at( row, col );
		GenericBase * base = cell->getBase();

		if( base ) {
			_server->sendTavernInfo( _currentPlayer, _tavern->getTavernLordNumber( base ), base );
		} else {
			logEE( "Should be a base ??" );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
		return;
	}

}

void Engine::handleGameTavernLord()
{
		TRACE("Engine::handleGameTaverLord");
	
		try {
			uint row = readInt();
			uint col = readInt();
			uchar numLord = readChar();

			GenericCell * cell = _map->at( row, col );
			GenericBase * base = cell->getBase();

			if( base ) {
				_server->sendTavernLord( _currentPlayer, _tavern->getTavernLord( base, numLord ), base );
			} else {
				logEE( "Should be a base ??" );
			}
		} catch ( const char * err ) {
			logEE("%s", err);
			return;
		}
}

void Engine::stateInQuestion( uint num )
{
	uchar cla1 = getCla1();
	uchar cla2 = getCla2();

	if( num == (uint) _players.indexOf( _currentPlayer ) ) {
		switch( cla1 ) {
		case SO_MSG:
			handleMessage( num );
			break;
		case SO_QR:
			switch( cla2 ) {
			case C_QR_ANSWER:
				handleAnswer();
				break;
			default:
				logEE( "Should not happen (Server : SO_QR/...)" );
				break;
			}
		case SO_TURN:
				switch( getCla2() ) {
					case C_TURN_LORD:
						break;
				}
			break;
		default:
			printSocket;
			break;
		}
	}
}

void Engine::handleAnswer()
{
	switch( getCla3() ) {
	case ANSWER_ENUM:{
		uchar choice = readChar();
		if( choice == 0 ) {
			_currentPlayer->getResourceList()->increaseValue( 0, 1000 );
			_server->sendPlayerResource( _currentPlayer, 0, _currentPlayer->getResourceList()->getValue( 0 ) );
		} else	{
			switch( _question->getType() ) {
			case C_QR_CHEST: {
				GenericLord * lord = _question->getLord();
				if( lord ) {
					manageIncreaseExperience( lord, 2500 );
				} else {
					logEE( "Lord in _question should not be NULL" );
				}
				} break;
			default:
				logEE( "should not happen" );
			}
		}
		_state = IN_GAME;
		}
		break;
	case ANSWER_YESNO:
		switch( _question->getType() ) {
		case C_QR_CREATURE_JOIN:
			handleAnswerCreatureJoin();
			break;
		case C_QR_CREATURE_MERCENARY:
			handleAnswerCreatureMercenary();
			break;
		case C_QR_CREATURE_FLEE:
			handleAnswerCreatureFlee();
			break;
		default:
			logEE( "Should not happen" );
			printSocket;
			break;
		}
	}
}

void Engine::handleAnswerCreatureJoin()
{
	bool accept = (bool) readChar();


	GenericMapCreature * creature = _question->getCreature();
	uchar race = creature->getRace();
	uchar level = creature->getLevel();
	
	GenericLord * lord = _question->getLord();
	
	if( accept ) {
		_state = IN_GAME;
		GenericFightUnit * unit = 0;
		int place = -1;
				
		for( uint i = 0; i < MAX_UNIT; i++ ) {
			unit = lord->getUnit( i );
			if( unit ) {
				if( ( unit->getRace() == race ) && ( unit->getLevel() == level ) ) {
					place = i;
					break;
				}
			} else {
				place = i;
				break;
			}
		}
		
		if( place != -1 ) {
			unit = lord->getUnit( place );
			
			if( ! unit ) {
				//place free, create new unit
				unit = new GenericFightUnit( race, level );
			}
			//else unit of same kind already present, add num
			unit->addNumber( creature->getCreatureNumber() );
			//necessary if unit is already present?
			lord->setUnit( place, unit );
			_server->updateUnit( _currentPlayer, lord, place );
		} else {
		 //no place	
			_server->sendAskNone( _currentPlayer, tr( "You have no place for recruiting new creatures"), QR_INFO  );
		}

		removeCreature( creature );
	} else {

		if( creature->isFleeing() ) {
			//creature is fleeing
			_question->setType( C_QR_CREATURE_FLEE );
			_server->sendAskCreatureFlee( _currentPlayer,  creature );

		} else {
			//creature fight
			startFight( lord->getId(), _question->getCreature() );
		}
	}
}

void Engine::handleAnswerCreatureMercenary()
{
	bool accept = (bool) readChar();
	GenericMapCreature * creature = _question->getCreature();
	GenericLord * lord = _question->getLord();
	
	uchar race = creature->getRace();
	uchar level = creature->getLevel();
	
	if( accept ) {
		_state = IN_GAME;
		GenericFightUnit * unit = 0;
		int place = -1;
				
		for( uint i = 0; i < MAX_UNIT; i++ ) {
			unit = lord->getUnit( i );
			
			if( unit ) {
				if( ( unit->getRace() == race ) && ( unit->getLevel() == level ) ) {
					place = i;
					break;
				}
			} else {
				place = i;
				break;
			}
		}
		
		if( place >= 0 ) {
			if( _currentPlayer->canBuy( creature->getCreature(), creature->getCreatureNumber() ) ) {
				_currentPlayer->buy( creature->getCreature(), creature->getCreatureNumber() );
				_server->sendPlayerResources( _currentPlayer );
				
				unit = lord->getUnit( place );
				if( ! unit ) {
					unit = new GenericFightUnit( race, level );
				}
				unit->addNumber( creature->getCreatureNumber() );
				lord->setUnit( place, unit );
				_server->updateUnit( _currentPlayer, lord, place );
			} else {
				_server->sendAskNone( _currentPlayer, tr( "You have not enough resources for recruiting this creatures" ), QR_INFO );
			}
		} else {
			_server->sendAskNone( _currentPlayer, tr( "You have no place for recruiting new creatures" ), QR_INFO );
		}
		removeCreature( creature );
	} else {
		GenericMapCreature * creature = _question->getCreature();
		
		if( creature->isFleeing() ) {
			_question->setType( C_QR_CREATURE_FLEE );
			_server->sendAskCreatureFlee(_currentPlayer, creature );
		} else {
			startFight( _question->getLord()->getId(), _question->getCreature() );
		}
	}
}

void Engine::handleAnswerCreatureFlee()
{
	bool accept = (bool) readChar();
	
	if( accept ) {
		_state = IN_GAME;
		GenericMapCreature * creature = _question->getCreature();
		removeCreature( creature );
	} else {
		startFight( _question->getLord()->getId(), _question->getCreature() );
	}
}

void Engine::handleInGameMvt( int num )
{
	if( getCla2() == C_MVT_ONE ) {
		uchar lord = readChar();
		uint row = readInt();
		uint col = readInt();
		GenericCell * destCell;
		try {
		 	destCell = _map->at( row, col );
		} catch( const char * err) {
			logEE("%s",err);
			return;
		}
		GenericLord * movingLord = _currentPlayer->getLordById( lord );

		handleOneMove( movingLord, destCell, num );
	} else if( getCla2() == C_MVT_MULTI ) {
		uchar idlord = readChar();
		uint nbCell = readInt();
		uint i, row, col;
		QList<GenericCell *> list;
		GenericCell * tmpCell;
		for( i = 0; i < nbCell; i++ ) {
			row = readInt();
			col = readInt();
			try {
				tmpCell = _map->at( row, col );
				list.append( tmpCell );
			} catch( const char * err) {
				logEE("%s",err);
			}
		}

		GenericLord * movingLord = _currentPlayer->getLordById( idlord );
		if(movingLord == NULL) {
			logEE("Engine::handleInGameMvt, error, no lord id %d nbcell %d", idlord, nbCell);
			for( int i = 0; i < _players.count(); i++ ) {
				logDD("Player %d", i );
				_players.at( i )->printLords();
			}
			return;
		}
		for( i = 0; i < nbCell; i++ ) {
			bool ret = handleOneMove( movingLord, list.at( i ) , num );
			if( ! ret ) {
				list.clear();
				break;
			}
		}
	} else {
		logEE( "Should not happen" );
	}
}

bool Engine::handleOneMove( GenericLord * movingLord, GenericCell * destCell, int /*num*/ )
{
	/// XXX : to improve : all clients concerned

	bool ret = false;

	if(destCell == NULL) {
		logEE("Engine::handleOneMove, error, no cell, movingLord %p", movingLord);
		return ret;
	}
	
	if(movingLord == NULL) {
		logEE("Engine::handleOneMove, error, no lord, destCell %p", destCell);
		return ret;
	}

	GenericCell * start = movingLord->getCell();
	uint lordRow = start->getRow();
	uint lordCol = start->getCol();
	uint row = destCell->getRow();
	uint col = destCell->getCol();
	
	TRACE( "Engine::handleOneMove (%d,%d) to (%d,%d)",lordRow, lordCol, row, col );

	if( ! ( ( lordRow   <= (row+1) ) &&
	    ( ( lordRow + 1 ) >= row ) &&
	    ( lordCol <= (col+1) ) &&
	    ( ( lordCol + 1 ) >= col ) ) ) {
	    	logWW( "Player should not request this mvt for lord (%d,%d) to (%d,%d)",
		lordRow, lordCol, row, col );
		return ret;
	}
	
	if(	destCell->getCoeff() < 0  || !destCell->isStoppable()){
		logWW("move not allowed %d, %d", row,col);
		logWW("coeff %d, isStoppable %d", destCell->getCoeff(),destCell->isStoppable());
		return ret;
	}

	int cost = PathFinder::computeCostMvt( start, destCell );
	if( cost!=0 && cost > movingLord->getCharac( MOVE ) ) {
		_server->sendLordCharac( _currentPlayer, movingLord, MOVE );
		logWW( "not enough mvt pt : %d < %d", movingLord->getCharac( MOVE ), cost );
		return ret;
	}
	
	GenericPlayer * tempPlayer;
	for( int index = 0; index < _players.count(); index++ ) {
		tempPlayer = _players.at( index );
		if( tempPlayer != _currentPlayer ) {
			if( !tempPlayer->canSee( destCell )  && tempPlayer->canSee( movingLord->getCell() ) ) {
				_server->sendLordRemove( tempPlayer, movingLord );
			}
		}
	}

	if( destCell->getLord() ) {
		movingOnLord( movingLord, destCell );
	} else if( destCell->getEvent() ) {
		movingOnEvent( movingLord, destCell );
	} else if( destCell->getBuilding() ) {
		movingOnBuilding( movingLord, destCell );
	} else if( destCell->getBase() ) {
		movingOnBase( movingLord, destCell );
	} else if( destCell->getCreature() ) {
		movingOnCreature( movingLord, destCell );
	} else {
		movingOnFreeCell( movingLord, destCell );
		ret = true;
	}

	return ret;
}

void Engine::movingOnLord( GenericLord * movingLord, GenericCell * destCell )
{

	TRACE("Engine::movingOnLord");

	GenericLord * destLord = destCell->getLord();

	if( destLord == movingLord ) {
		logEE("Engine::Move on same lord");
		return;
	}

	if( destLord->getOwner() == _currentPlayer ) {
		_server->sendLordExchange( _currentPlayer, movingLord, destLord );
		//no decrease move?
	} else if (destLord->getOwner()->getTeamId() == _currentPlayer->getTeamId()){
		//lord of friend team
	} else {
		//enemy lord
		decreaseMove( movingLord, destCell );
		startFight( movingLord->getId(), destLord );
	}
}

void Engine::movingOnEvent( GenericLord * movingLord, GenericCell * destCell )
{

	TRACE("Engine::movingOnEvent");
	
	decreaseMove( movingLord, destCell );

	GenericEvent * event = destCell->getEvent();
	switch( event->getType() ) {
	case GenericEvent::EventArtefact:
		movingOnArtefact( movingLord, destCell );
		break;
	case GenericEvent::EventBonus:
		movingOnBonus( movingLord, destCell );
		break;
	case GenericEvent::EventChest:
		movingOnChest( movingLord, destCell );
		break;
	default:
		logEE( "Should not happen" );
		break;
	}
}

void Engine::movingOnArtefact( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnArtefact");

	GenericEvent * event = destCell->getEvent();
	movingLord->getArtefactManager()->addArtefact( event->getArtefact()->getType() );
	destCell->setEvent( 0 );
	_server->delEvent( _players, event );
	_server->ownArtefact( movingLord->getArtefactManager()->getArtefactByType( event->getArtefact()->getType() ), _currentPlayer );

	removeEvent( event );
}

void Engine::movingOnBonus( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnBonus");

	GenericEvent * event = destCell->getEvent();
	GenericBonus * bonus = event->getBonus();
	destCell->setEvent( 0 );
	_server->delEvent( _players, event );

	switch( bonus->getType() ) {
	case GenericBonus::BonusResource:
		movingOnBonusResource( bonus );
		break;
	case GenericBonus::BonusPrimSkill:
		movingOnBonusPrimSkill( movingLord, bonus );
		break;
	case GenericBonus::BonusSkill:
		break;
	case GenericBonus::BonusSpell:
		break;
	}

	removeEvent( event );
}

void Engine::movingOnBonusResource( GenericBonus * bonus )
{
	uint typeResource = bonus->getParam( 0 );
	uint numberType = bonus->getParam( 1 );
	uint number = 0;

	if( numberType == 0 ) {
		number = bonus->getParam( 2 );
	} else if( numberType == 1 ) {
		uint nbVar = bonus->getParam( 2 );
		for( uint i = 0; i < nbVar; ++i ) {
			bonus->getParam( 3 + i );
		}
	} else {
		/// Not finished
		//uint min = bonus->getParam( 2 );
		//uint max = bonus->getParam( 3 );
		//uint step = bonus->getParam( 4 );
	}

	if( number > 0 ) {
		_currentPlayer->getResourceList()->increaseValue( typeResource, number );
		_server->sendPlayerResource( _currentPlayer, typeResource, _currentPlayer->getResourceList()->getValue( typeResource ) );
	}
}

void Engine::movingOnBonusPrimSkill( GenericLord * movingLord, GenericBonus * bonus )
{
	uint skill = bonus->getParam( 0 );
	uint value = bonus->getParam( 1 );
	LordCharac charac;

	switch( skill ) {
	case 0:
		charac = ATTACK;
		break;
	case 1:
		charac = DEFENSE;
		break;
	case 2:
		charac = POWER;
		break;
	case 3:
		charac = KNOWLEDGE;
		break;
	case 4:
		charac = MORALE;
		break;
	case 5:
		charac = LUCK;
		break;
	default:
		logEE( "Should not happen" );
		charac = ATTACK;
		break;
	}

	movingLord->increaseBaseCharac( charac, value );
	_server->sendLordCharac( movingLord->getOwner(), movingLord, charac );
}

void Engine::movingOnChest( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnChest");

	GenericEvent * event = destCell->getEvent();

	_question->setLord( movingLord );
	_question->setType( C_QR_CHEST );
	_server->sendAskChest(_currentPlayer);
	_state = IN_QUESTION;
	destCell->setEvent( 0 );
	_server->delEvent( _players, event );
	removeEvent( event );
}



void Engine::movingOnBuilding( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnBuilding");

	// XXX: reread (cost computed ?...
	GenericBuilding * building = destCell->getBuilding();

	if( handleBuildingEnter( building, movingLord ) ) {
		if( ! _currentPlayer->hasBuilding( building ) ) {
		
			if( building->getOwner() ) {
				building->getOwner()->removeBuilding( building );
			}
			_currentPlayer->addBuilding( building );
			building->setOwner( _currentPlayer );

			_server->sendBuildingResources( _currentPlayer , building );
			_server->ownBuilding( _players, building);
		}

		decreaseMove( movingLord, destCell );
		movingLord->setCell( destCell );
		moveLord( movingLord, destCell );
	}
}

void Engine::movingOnBase( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnBase");

	GenericBase * base = destCell->getBase();

	GenericLord * oplord =  base->getGarrisonLord();
	if( oplord ) {
		_server->newLord( _currentPlayer, oplord );
		decreaseMove( movingLord, destCell );
		startFight( movingLord->getId(), oplord );
		return;
	}

	decreaseMove( movingLord, destCell );
	movingLord->setCell( destCell );

	if( ! _currentPlayer->hasBase( base )  ) {
		GenericPlayer * tempPlayer;

		if( base->getOwner() && base->getOwner()->getTeamId() == _currentPlayer->getTeamId()  ) {
			decreaseMove( movingLord, destCell );
			movingLord->setCell( destCell );
			moveLord( movingLord, destCell );
			return;
		}

		if( base->getOwner() ){
			base->getOwner()->removeBase( base );
		}

		_currentPlayer->addBase( base );
		base->setOwner( _currentPlayer );
		
		//update production for new player
		_server->sendBaseProduction( _currentPlayer, base  );

		QList<Action *> list = base->getActionList( Action::CHANGEOWNER );
		if( !list.isEmpty() ) {
			for( int i = 0; i < list.count(); i++ ) {
				handleAction( list.at(i), _currentPlayer, base->getResourceList() );
			}
		}

		_server->sendBaseResources( _currentPlayer, base );

		for( int index = 0; index < _players.count(); index++ ) {
			tempPlayer = _players.at( index );
			updatePlayerPrices(tempPlayer);
			_server->sendPlayerPrices( tempPlayer );
		}
		_server->ownBase( _players, base );
	}	

	moveLord( movingLord, destCell );

}

void Engine::movingOnCreature( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnCreature");

	decreaseMove( movingLord, destCell );

	GenericMapCreature * creature = destCell->getCreature();
	uint lordForce = movingLord->computeForceIndicator(true);
	uint creatureForce = creature->computeForceIndicator(true);
	
	if( creatureForce > 0 ) {
		double ratio = double( lordForce ) / double( creatureForce );
		
		CreatureAction action = computeCreatureAction( creature, ratio );
		
		switch( action ) {
		case CreatureJoin:
			_question->setType( C_QR_CREATURE_JOIN );
			_question->setLord( movingLord );
			_question->setCreature( creature );
			_state = IN_QUESTION;
			_server->sendAskCreatureJoin(_currentPlayer, creature );
			break;
		case CreatureMercenary:
			_question->setType( C_QR_CREATURE_MERCENARY );
			_question->setLord( movingLord );
			_question->setCreature( creature );
			_state = IN_QUESTION;
			_server->sendAskCreatureMercenary(_currentPlayer, creature );
			break;
		case CreatureFlee:
			_question->setType( C_QR_CREATURE_FLEE );
			_question->setLord( movingLord );
			_question->setCreature( creature );
			_state = IN_QUESTION;
			_server->sendAskCreatureFlee(_currentPlayer, creature );
			break;
		case CreatureFight:
			startFight( movingLord->getId(), creature );
			break;
		}	
	}
}

void Engine::movingOnFreeCell( GenericLord * movingLord, GenericCell * destCell )
{
	TRACE("Engine::movingOnFreeCell");

	uint i;
	int lordRow = movingLord->getCell()->getRow();
	int lordCol = movingLord->getCell()->getCol();
		
	decreaseMove( movingLord, destCell );

	QList<GenericCell *> removedCells = _currentPlayer->removeLordVision( movingLord );
	movingLord->setCell( destCell );
	QList<GenericCell *> addedCells = _currentPlayer->addLordVision( movingLord );
	updateMapVision( removedCells, addedCells );

	uint nbPlayers = _players.count();
	GenericPlayer * tempPlayer;

	for( i = 0; i < nbPlayers; ++i ) {
		tempPlayer = _players.at( i );
		if( tempPlayer != _currentPlayer ) {
			if( ( ! tempPlayer->canSee( lordRow, lordCol ) )
				&& ( tempPlayer->canSee( destCell ) ) ) {
				_server->sendLordVisit( movingLord, tempPlayer, true );
			}
			if( ( tempPlayer->canSee( lordRow, lordCol ) )
				&& ( ! tempPlayer->canSee( destCell ) ) ) {
				_server->sendLordVisit( movingLord, tempPlayer, false );
			}
		}
	}
	moveLord( movingLord, destCell );
}

void Engine::handleInGameExchange()
{
	switch( getCla2() ) {
	case C_EXCH_UNIT:
		exchangeUnits();
		break;
	case C_EXCH_UNIT_SPLIT:
		exchangeUnitSplit();
		break;
	case C_EXCH_ARTEFACT:
		exchangeArtefact();
		break;
	case C_EXCH_BASEUNIT:
		exchangeBaseUnits();
		break;
	default:
		printSocket;
		break;
	}
}

void Engine::exchangeUnits()
{
	uchar idLord1 = readChar();
	uchar idUnit1 = readChar();
	uchar idLord2 = readChar();
	uchar idUnit2 = readChar();
	GenericLord * lord1 = 0;
	GenericLord * lord2 = 0;

	TRACE("Engine::exchangeUnits idLord1 %d, idUnit1 %d, idLord2 %d, idUnit2 %d", idLord1,idUnit1,idLord2,idUnit2);
	if( idLord1 ) {
		lord1 = _currentPlayer->getLordById( idLord1 );
	}
	if( idLord2 ) {
		lord2 = _currentPlayer->getLordById( idLord2 );
	}

	if( lord1 && lord2 ) {
		if(lord1->getOwner() != lord2->getOwner()) {
			///startFight( lord1->getId(), lord2 ); ///todo ??
			return;
		}

		if( idUnit1 > MAX_UNIT || idUnit2 > MAX_UNIT ) {
			return;
		}

		GenericFightUnit * unit1 = lord1->getUnit( idUnit1 );
		GenericFightUnit * unit2 = lord2->getUnit( idUnit2 );
		TRACE(" unit1 %p, unit2 %p", unit1,unit2);

		if(!unit1 && !unit2) {
			return;
		}

		if( unit1 ) {
			if( unit2 ) {
				if( ( unit1->getRace() == unit2->getRace() ) &&
						unit1->getLevel() == unit2->getLevel() ) {
					unit2->addNumber( unit1->getNumber() );
					lord1->setUnit( idUnit1, 0 );
					delete unit1;
				} else {
					lord1->setUnit( idUnit1, unit2 );
					lord2->setUnit( idUnit2, unit1 );
				}
			} else {
				if(lord1->countUnits() > 1 || lord1 == lord2){
					lord2->setUnit( idUnit2, unit1 );
					lord1->setUnit( idUnit1, 0 );
				} else {
					return;
				}
			}
		} else if (!unit1) {
			if(lord2->countUnits() > 1 || lord1 == lord2){
				lord1->setUnit( idUnit1, unit2 );
				lord2->setUnit( idUnit2, 0 );
			} else {
				return;
			}
		} else {
			return;
		}
		_server->updateUnits( _players, lord1 );
		_server->updateUnits( _players, lord2 );
	}
}

void Engine::exchangeUnitSplit()
{
	uchar idLord = readChar();
	uchar idUnit = readChar();
	uchar num1 = readChar();
	uchar pos2 = readChar();
	uchar num2 = readChar();
	GenericLord * lord = NULL;
	long int otherHealth = 0;

	TRACE("Engine::exchangeUnitSplit idLord %d, idUnit %d, pos1 %d, num1 %d, pos2 %d, num2 %d", idLord, idUnit, num1, pos2, num2);
	if( idLord ) {
		lord = _currentPlayer->getLordById( idLord );

		if( idUnit > MAX_UNIT && pos2 > MAX_UNIT ) {
			return;
		}

		GenericFightUnit * unit = lord->getUnit( idUnit );
		GenericFightUnit * unit2 = lord->getUnit( pos2 );

		TRACE(" unit %p", unit );

		//split unit in 2 units
		if( ! unit2 && num2  ) {
			unit2 = new GenericFightUnit( unit->getRace() , unit->getLevel() );
			unit2->setMove( unit->getMove() );
			//only one unit don't have max health, otherwise we remove some health in the split
			//ex: unit1: 7 units, max health 9/12 . old split: unit1: 6 units health 9/12 , unit2: 1 unit health 9/12
			//new split: unit1: 6 units health 9/12 , unit2: 1 unit health 12/12
			unit2->setHealth( unit->getMaxHealth() );
		}

		//join unit in unit2
		if( unit && !num1  ) {
			otherHealth = unit->getHealth();
			logDD("other1 %d", otherHealth);
			delete unit;
			unit = NULL;
		}	
		
		//join unit2 in unit
		if( unit2 && !num2  ) {
			otherHealth = unit2->getHealth();
			logDD("other2 %d", otherHealth);
			delete unit;
			unit = NULL;
		}	
		
		if( unit ) {
			unit->setNumber( num1 );
			//in join use the minimum health of two units
			//buggy wait a moment.....
			unit->setHealth( std::min( unit->getHealth(), otherHealth ));
		}

		if( unit2 ) {
			unit2->setNumber( num2 );
			unit2->setHealth( std::min( unit2->getHealth(), otherHealth ));
		}

		lord->setUnit( idUnit, unit );
		lord->setUnit( pos2, unit2 );
		_server->updateUnits( _players, lord );
	}

}

void Engine::exchangeArtefact()
{
	uchar idLord1 = readChar();
	int item = readInt();
	uchar idLord2 = readChar();
	
	TRACE("Engine::exchangeArtefact idLord1 %d, item %d, idLord2 %d", idLord1, item ,idLord2);

	GenericLord * lord1 = 0;
	GenericLord * lord2 = 0;
	
	if( idLord1 ) {
		lord1 = _currentPlayer->getLordById( idLord1 );
	}
	if( idLord2 ) {
		lord2 = _currentPlayer->getLordById( idLord2 );
	}
	
	if( lord1 && lord2 ) {
		ArtefactManager * manag1 = lord1->getArtefactManager();
		ArtefactManager * manag2 = lord2->getArtefactManager();
		
		GenericLordArtefact * artefact = manag1->getArtefact( item );
		
		manag1->removeArtefact( item );
		manag2->addArtefact( artefact );
		
		_server->sendExchangeArtefact( _players, lord1, item, lord2 );
	}
}

void Engine::exchangeBaseUnits()
{
	uchar idBase = readChar();
	uchar idUnit1 = readChar();
	uchar idLord = readChar();
	uchar idUnit2 = readChar();
	
	TRACE("Engine::exchangeBaseUnits idBase %d, idUnit1 %d, idLord %d, idUnit2 %d", idBase, idUnit1 ,idLord, idUnit2);

	GenericBase * base = _currentPlayer->getBaseById( idBase );
	GenericLord * lord = 0; 
	if(idLord && idLord < GenericLord::MAX_LORDS){
		lord = _currentPlayer->getLordById( idLord );
	}
	GenericFightUnit * uni1 = 0;
	GenericFightUnit * uni2 = 0;

	if(base){
		if(idUnit1<=MAX_UNIT){
			uni1 = base->getUnit( idUnit1);
		}
		if(lord){
			if(idUnit2<=MAX_UNIT){
				uni2 = lord->getUnit( idUnit2 );
			}

			if(!uni1 && !uni2) {
				return;
			}

			if( uni1 && uni2 ) {
				if( uni1->getCreature() != uni2->getCreature() ) {
					lord->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni2->addNumber( uni1->getNumber() );
					base->setUnit( idUnit1, 0 );
					delete uni1;
				}
			} else if (!uni1) {
				if(lord->countUnits() > 1){
					lord->setUnit( idUnit2, 0 );
					base->setUnit( idUnit1, uni2 );
				} else {
					return;
				}
			} else if (!uni2) {
				lord->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		} else {

			if(idUnit2<=MAX_UNIT){
				uni2 = base->getUnit( idUnit2);
			}
			
			if(!uni1 && !uni2) {
				return;
			}

		if( uni1 && uni2 ) {
			if( uni1->getCreature() != uni2->getCreature() ) {
					base->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni2->addNumber( uni1->getNumber() );
					base->setUnit( idUnit1, 0 );
					delete uni1;
				}
			} else {
				base->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		}	

		if( lord ) {
			_server->updateUnits( _players, lord );
		}
		_server->sendBaseUnits( _players, base  );
	}

}

void Engine::handleInGameModif()
{
	switch( getCla2() ) {
	case C_MOD_MAP:
	case C_MOD_CELL:
	case C_MOD_PLAYER:
		logDD( "Not yet implemented" );
		break;
	case C_MOD_BASE:
		handleInGameModifBase();
		break;
	case C_MOD_BUILD:
		logDD( "Not yet implemented" );
		break;
	case C_MOD_LORD:
		handleInGameModifLord();
		break;
	default:
		printSocket;
		break;
	}
}

void Engine::handleInGameModifBase()
{
	switch( getCla3() ) {
	case C_BASE_BUILDING: 
		handleInGameModifBaseBuilding();
		break;
	case C_BASE_UNIT_BUY: 
		handleInGameModifBaseUnit();
		break;
	case C_BASE_MARKET: 
		handleInGameModifBaseMarket();
		break;
	default:
		logDD( "Not yet implemented" );
		printSocket;
		break;
	}
}

void Engine::handleInGameModifBaseBuilding()
{
	int id = readChar();
	int building = readChar();
	bool isBuy = readChar();
	
	GenericBase * base = _currentPlayer->getBaseById( id );
	if( base ) {
		GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
		InsideBuildingModel * buildingModel = baseModel->getBuildingModel( building );
		GenericInsideBuilding * tmpbuil = base->getBuildingByType( building );

		if( isBuy == true && !tmpbuil ) {
			if( base->getState( GenericBase::BASE_BUY ) && _currentPlayer->canBuy( buildingModel ) && base->canBuildBuilding( buildingModel) ) {
				for( int i = 0; i < DataTheme.resources.count(); i++ ) {
					if( buildingModel->getCost( i ) != 0 ) {
						_currentPlayer->getResourceList()->decreaseValue( i, buildingModel->getCost( i ) );
						_server->sendPlayerResource(_currentPlayer, i, _currentPlayer->getResourceList()->getValue( i ) );
					}
				}
				base->addBuilding( building );
				base->setState( GenericBase::BASE_BUY, false );
				
				_server->updateBaseBuilding( _players, base, base->getBuildingByType( building ) );
				
				_server->sendBaseState( _currentPlayer, base, GenericBase::BASE_BUY, false );

				if( buildingModel->getAction() ) {
					if( buildingModel->getAction()->getType() == INSIDE_MARKET ) {
						updatePlayerPrices( _currentPlayer );
						_server->sendPlayerPrices( _currentPlayer );
					}
				}
			}
		} else if( isBuy == false && tmpbuil ) {
			if( base->getState( GenericBase::BASE_SELL )  && buildingModel->getAction() && (buildingModel->getAction()->getType() != INSIDE_VILLAGE) ) {
				for( int i = 0; i < DataTheme.resources.count(); i++ ) {
					if( buildingModel->getCost( i ) != 0 ) {
						_currentPlayer->getResourceList()->increaseValue( i, buildingModel->getCost( i )/2 );
						_server->sendPlayerResource( _currentPlayer,i, _currentPlayer->getResourceList()->getValue( i ) );
					}
				}
				base->removeBuilding( tmpbuil );
				base->setState( GenericBase::BASE_SELL, false );
				
				_server->sendBaseState( _currentPlayer, base, GenericBase::BASE_SELL, false );
			
				_server->updateBaseBuilding( _players, base , tmpbuil);
			}
		}
	}
}

void Engine::handleInGameModifBaseUnit()
{
	int row = readInt();
	int col = readInt();
	uchar race = readChar();
	uchar level = readChar();
	int number = readInt();


	GenericCell * cell;
	try {
		cell = _map->at( row, col );
	} catch( const char * err) {
		logEE("%s",err);
		return;
	}

	GenericBase * base = cell->getBase();
	if( base ) {
		Creature * creature = DataTheme.creatures.at( race, level );
		if( base->getCreatureProduction( creature ) >= number  && number != 0 ) {
			if( _currentPlayer->canBuy( creature, number ) ) {
				_currentPlayer->buy( creature, number );
				base->addGarrison( creature, number );
				base->buyCreature( creature, number );
				_server->sendPlayerResources( _currentPlayer );
				if( base->isGarrisonLord()) {
					_server->updateUnits( _players, base->getGarrisonLord()	);
				} else {
					_server->sendBaseUnits( _players, base  );
				}
				//only can see
				_server->sendBaseProduction( _players, base );
			}
		}
	}
}

void Engine::handleInGameModifBaseMarket()
{
	//XXX : not full implemented
	int res[2];
	res[0] = readInt();
	res[1] = readInt();
	int value = readInt();
	float cupr;
	int i;
	
	int reserve = _currentPlayer->getResourceList()->getValue( res[0] );
	ResourceModel * model;
	
	for( i = 0; i < 2 ; i++ ) {
		model = DataTheme.resources.get( res[i] );
		if( model->isPreservable() ) {
			return;
		}
	}

		
	if( res[0] != res[1] ) {
		PriceMarket * realPrice = _currentPlayer->getPriceMarket();
		cupr = realPrice->getResourceInResourceFloat( res[0],res[1] );
		TRACE("res[0] %d, res[1] %d, value %d, cupr %f",res[0],res[1],value, cupr);
		if( value > reserve ) {
			value = reserve;
		}
		_currentPlayer->getResourceList()->increaseValue( res[1], (int) (value*cupr));
		_currentPlayer->getResourceList()->decreaseValue( res[0], value);
		
		for( i = 0; i < 2 ; i++ ) {
			_server->sendPlayerResource( _currentPlayer, res[i], _currentPlayer->getResourceList()->getValue(res[i]));
		}
	}

}

void Engine::handleInGameModifLord()
{
	switch( getCla3() ) {
	case C_LORD_GARRISON: 
		handleInGameModifLordGarrison();
		break;
	case C_LORD_UNIT: 
		handleInGameModifLordUnit();
		break;
	case C_LORD_BUY:
		handleInGameModifLordBuy();
		break;
	default:
		logDD( "Not yet implemented (modif lord)" );
		printSocket;
		break;
	}
}

void Engine::handleInGameModifLordGarrison()
{
	uchar idLord = readChar();
	bool garrison = ( readChar() == (uchar)1 );
	
	TRACE("Engine::handleInGameModifLordGarrison idLord %d, gerrison %d", idLord, garrison );

	GenericLord * lord = _currentPlayer->getLordById( idLord );
	
	/// XXX: test if it is possible...
	if( lord && lord->getCell()->getBase() != 0 ) {
		lord->setVisible( !garrison );
		GenericBase * base = lord->getCell()->getBase();
		if( ( lord == base->getGarrisonLord() ) ||
		( lord == base->getVisitorLord() ) ) {
			if( ( garrison && ( lord == base->getVisitorLord() ) )
			|| ( ( lord == base->getGarrisonLord() ) && !garrison ) ) {
				base->exchangeLords();
				_server->setGarrison( _players, lord, garrison );
			}
		} else if( ( base->getGarrisonLord() == 0 ) || ( base->getVisitorLord() == 0 ) ) {
			if( garrison ) {
				if( base->getGarrisonLord() ) {
					base->exchangeLords();
				}
				base->setGarrisonLord( lord );
				_server->setGarrison( _players, lord, true );
			} else {
				if( base->getVisitorLord() ) {
					base->exchangeLords();
				}
				base->setVisitorLord( lord );
				_server->setGarrison( _players, lord, false );
			}
		}
	}
}

void Engine::handleInGameModifLordUnit()
{
	uchar id = readChar();
	uchar pos = readChar();
	uchar race = readChar();
	uchar level = readChar();
	uint nb = readInt();
	uchar move = readChar();
	int health = readInt();
	
	GenericLord * lord = 0; 
	GenericFightUnit * unit = 0;
		
	if( id && id < GenericLord::MAX_LORDS ) {
		lord = _currentPlayer->getLordById( id );
	}

	/*
	if( num < 0 || nb < 0 || move < 0 || health < 0 ) {
		logEE("Error, negative values");
	}*/

	unit =lord->getUnit( pos );
	if( ! unit ) {
		unit = new GenericFightUnit( race, level );
		unit->setMove( move );
		unit->setHealth( health );
	}
	if( (uint) unit->getNumber() < nb ) {
		/* no cheat possible */
		return;
	}

	if( nb == 0 ){
		if( lord->countUnits() > 1 ) {
			unit->setNumber( nb );
		}
	} else {
		unit->setNumber( nb );
	}

	lord->setUnit( pos, unit );

	_server->updateUnit( _currentPlayer,  lord, pos );

	if( unit->getNumber() == 0 ){
		delete unit;
		unit = NULL;
		lord->setUnit( pos, unit );
	}	
}

void Engine::handleInGameModifLordBuy()
{
	uchar id = readChar();
	uint row = readInt();
	uint col = readInt();

	GenericCell * cell;
	try {
		cell = _map->at( row, col );
	} catch( const char * err) {
		logEE("%s",err);
		return;
	}

	/* add controls for cheats */
	TRACE("Engine::handleInGameModifLordBuy id %d, row %d ,col %d", id, row, col);

	if( cell->getLord() ) {
		//placement cell with a lord
		_server->sendMessage( _currentPlayer , QString( tr("You cannot buy the lord, the cell is already occupied") ));
		return;
	}
	
	GenericLordModel * lordModel = DataTheme.lords.at(id);
	if( !_currentPlayer->canBuy( lordModel ) ) {
		_server->sendMessage( _currentPlayer , QString( tr("Not enough resources to buy the lord") ));
		return;
	}

	if( setLord2Player( id , _currentPlayer->getNum()) ) {
		//XXX: strange procedure, quite obscure, to change
		GenericLord * lord = _currentPlayer->getLord( _currentPlayer->numLord() - 1 	);
		lord->setId( id );
		lord->setCell( cell );

		_server->newLord( _players,  lord  );

		//a new lord bought start with max movements
		lord->setBaseCharac( MOVE, lord->getCharac( MAXMOVE ) );
		_server->sendLordCharac( _currentPlayer, lord, MOVE );

		//operations to compute the cost of lord bought
		for( int i = 0; i < DataTheme.resources.count(); i++ ) {
			if( lordModel->getCost( i ) != 0 ) {
				_currentPlayer->getResourceList()->decreaseValue( i, lordModel->getCost( i ) );
				_server->sendPlayerResource( _currentPlayer, i, _currentPlayer->getResourceList()->getValue( i ) );
			}
		}

	} else {
		logEE("Lord2player failed");
	}
}

bool Engine::handleBuildingEnter( GenericBuilding * building, GenericLord * lord )
{
	TRACE("Engine::handleBuildingEnter");

	bool enter = false;
	QList<Action *> list;

	if( ! building->hasBeenVisited() ) {
		if( building->getCondition() ) {
		}
		enter = true;
		list = building->getActionList( Action::FIRSTTIME );

		if( list.isEmpty() ) {
			list = building->getActionList( Action::FIRSTTIMELORD );
		}
		if( list.isEmpty() ) {
			list = building->getActionList( Action::NEXTTIME );
		}
		if( list.isEmpty() ) {
			list = building->getActionList( Action::DATE );
		}
		for( int i = 0; i < list.count(); i++ ) {
			handleAction( list.at( i ), lord );
			handleAction( list.at( i ), lord->getOwner() );
		}
		building->enter( lord );
	} else if( ! building->hasBeenVisited( lord ) ) {
		if( building->getCondition() ) {
		}
		enter = true;
		list = building->getActionList( Action::FIRSTTIMELORD );
		if( list.isEmpty() ) {
			list = building->getActionList( Action::NEXTTIME );
		}
		for( int i = 0; i < list.count(); i++ ) {
			handleAction( list.at( i ), lord );
		}
		building->enter( lord );
	} else {
		if( building->getCondition() ) {
		}
		enter = true;
		list = building->getActionList( Action::NEXTTIME );
		for( int i = 0; i < list.count(); i++ ) {
			handleAction( list.at( i ), lord );
		}
		building->enter( lord );
	}
	return enter;
}

void Engine::handleAction( Action * action, GenericLord * lord )
{
	for( int i = 0; i < action->getElementaryNumber(); i++ ) {
		ElementaryAction * _elementary = action->getElementaryAction( i );

		switch( _elementary->getType() ) {
			case ElementaryAction::ATTACK:
				lord->increaseBaseCharac( ATTACK, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, ATTACK );
				break;
			case ElementaryAction::DEFENSE:
				lord->increaseBaseCharac( DEFENSE, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, DEFENSE );
				break;
			case ElementaryAction::POWER:
				break;
			case ElementaryAction::KNOWLEDGE:
				break;
			case ElementaryAction::MOVE:
				lord->increaseBaseCharac( MOVE, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, MOVE );
				break;
			case ElementaryAction::MAXMOVE:
				lord->increaseBaseCharac( MAXMOVE, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, MAXMOVE );
				break;
			case ElementaryAction::TECHNICPOINT:
				break;
			case ElementaryAction::MAXTECHNICPOINT:
				break;
			case ElementaryAction::MORALE:
				lord->increaseBaseCharac( MORALE, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, MORALE );
				break;
			case ElementaryAction::LUCK:
				lord->increaseBaseCharac( LUCK, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, LUCK );
				break;
			case ElementaryAction::VISION:
				lord->increaseBaseCharac( VISION, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, VISION );
				break;
			case ElementaryAction::EXPERIENCE:
				lord->increaseBaseCharac( EXPERIENCE, _elementary->getCoeff() );
				manageIncreaseExperience( lord,  _elementary->getCoeff());
				//_server->sendLordCharac( _currentPlayer, lord, EXPERIENCE );
				break;
			case ElementaryAction::SKILL:
				break;
			case ElementaryAction::ARTEFACT:
				break;
			case ElementaryAction::UNIT:
				break;				
			case ElementaryAction::CHARISMA:
				break;
			default:
				break;
		}
	}
}

void Engine::handleInGameTurn()
{
	switch( getCla2() ) {
		case C_TURN_PLAY:
			logEE( "Should not happen (Server : SO_TURN/C_TURN_PLAY)" );
			break;
		case C_TURN_LORD:{
			int type = readInt();
			_server->sendLordPlTurn( _currentPlayer, type );
			}
			break;
		case C_TURN_PLORD:
			logEE( "Should not happen (Server : SO_TURN/C_TURN_PLORD)" );
			break;
		case C_TURN_END:
			nextPlayer();
			break;
		default:
			printSocket;
			break;
	}
}

void Engine::stateInFight( int num )
{
	uchar cla1;
		
	if( _fight ) {
		cla1 = getCla1();
		switch( cla1 ) {
		case SO_MSG:
			handleMessage( num );
			break;
		case SO_FIGHT:
			_fight->handleSocket( _players.at( num ), _currentData );
			break;
		case SO_TURN:
				switch( getCla2() ) {
					case C_TURN_LORD:
						break;
				}
				break;
		default:
			printSocket;
			break;
		}
	} else {
		logEE( "Should not happen, _fight == 0" );
	}
}

void Engine::appendPlayersList( QList<AttalPlayerSocket *> sockets )
{
	for( int i = 0; i < sockets.count(); i++ ) {
		addPlayer( sockets.at( i ) );
	}
}

void Engine::addPlayer( AttalPlayerSocket * socket )
{
	GenericPlayer * player = socket->getPlayer(); 
	addPlayer( player );
}

void Engine::addPlayer( GenericPlayer * player )
{
	_players.append( player );
}

void Engine::removePlayer( AttalPlayerSocket * socket )
{
	GenericPlayer * player = socket->getPlayer(); 
	removePlayer( player );
}

void Engine::removePlayer( GenericPlayer * player )
{
	_players.removeAll( player );
}

void Engine::slot_newPlayer( AttalPlayerSocket * socket )
{
	_players.append( socket->getPlayer() );
}

void Engine::reinit()
{
	_state = NOT_PLAYING;
	GameData::reinit();

	_server->clear();
}

void Engine::startCampaign()
{
	//no more used
	/*if( _campaign ) {
		QString filename;
		uint nbScen = _campaign->getScenarioNumber();
		
		for( uint i = 0; i < nbScen; i++ ) {
			filename = _campaign->getScenario( i );
			if( loadGame( CAMPAIGN_PATH + filename , false) ) {
				startGame();
			}
		}
	}*/
}
	
void Engine::deleteCampaign()
{
	TRACE("Engine::deleteCampaign %p", _campaign);
	
	if( _campaign ) {
		delete _campaign;
		_campaign = NULL;
	}
}

void Engine::startGame()
{
	TRACE("Engine::StartGame");

	if( _players.count() > (int)_server->getNbSocket() ) {
		return;
	}

	_state = IN_GAME;
	
	// send begin game signal
	_server->startGame( _players );

	_server->sendSizeMap( _players, _map->getHeight(), _map->getWidth() );

	for( int i = 0; i < _players.count(); i++ ) {
		GenericPlayer * player = _players.at( i );
		player->setMap( _map );
		player->setNum( i );
		player->setAlive( true );

		// send to player info about team of every player
		_server->sendGamePlayersTeam( player, _players );
		//calendar
		_server->sendGameCalendar( player, _calendar);

		/* vision */
		player->initMapVision();

		for( uint j = 0; j < player->numLord(); ++j ) {
			GenericLord * lord = player->getLord( j );
			QList<GenericCell *> removed;
			QList<GenericCell *> added = player->addLordVision( lord );
		}

		for( uint j = 0; j < player->numBase(); ++j ) {
			GenericBase * base = player->getBase( j );
			QList<GenericCell *> removed;
			QList<GenericCell *> added = player->addBaseVision( base );
		}

		for( uint j = 0; j < player->numBuilding(); ++j ) {
			QList<GenericCell *> removed;
			QList<GenericCell *> added = player->addBuildingVision( player->getBuilding( j ) );
		}

		for( uint col = 0; col < _map->getWidth(); ++col ) {
			for( uint row = 0; row < _map->getHeight(); ++row ) {
				if( player->canSee( row, col ) ) {
					updateCellVision( _map->at( row, col ), player );
				}
			}
		}

		/* end vision */

		/* resources, production and prices */
		for( uint j = 0; j < player->numBase(); ++j ) {
			GenericBase * base = player->getBase( j );
			_server->sendBaseStates( player, base );
			_server->sendBaseResources( player, base );
			_server->sendBaseProduction( player, base  );
		}

		for( uint j = 0; j < player->numBuilding(); ++j ) {
			GenericBuilding * build = player->getBuilding( j );
			_server->sendBuildingResources( player, build );
		}
		
		for( uint j = 0; j < player->numLord(); ++j ) {
			GenericLord * lord = player->getLord( j );
			_server->sendLordCharacs(  player, lord );
		}

		for( int j = 0; j < DataTheme.resources.count(); ++j ) {
			_server->sendPlayerResource( player, j, player->getResourceList()->getValue( j ) );
		}

		updatePlayerPrices(player);
		_server->sendPlayerPrices( player );

		/* end resources, production and prices */
	}


	_map->computeStoppable();

  //here start currentPlayer	
	_currentPlayer = _players.at( _currPlayNum );
	
	_server->beginTurn( _players, _currentPlayer );

	//send scenario description
	_server->sendAskNone( _players, getScenarioDescription(), QR_SCEN_DESC );
	
	TRACE("Engine::StartGame End");
}

void Engine::endGameExternal()
{
	endGame();
}

void Engine::endGameInternal()
{
	endGame();
	emit sig_endGame( _gameId );
}

void Engine::endGame()
{
	TRACE("Engine::endGame");

	_state = NOT_PLAYING;
	if(_server) {
		if(_fight){
			disconnect( _fight , 0, 0, 0 );
			if( !_fight->hasEnded() ) {
				_fight->endFight();
				slot_endFight( _fight->getResult() );
			}
			delete _fight;
		}
		_fight = NULL;
		_server->sendEndGame( _players );
	}
	
	_dataQueue.clear();
}

void Engine::startFight()
{
	if( !_fight ) {
		_fight = new FightEngine( _server );
	}
	_state = IN_FIGHT_LORD;
	_fight->init( _players.at( 0 ), _players.at( 0 )->getLord( 0 ), _players.at( 1 ), _players.at( 1 )->getLord( 0 ) );
}

void Engine::startFight( int lordAttack, GenericLord *lordDefense )
{
	TRACE("Engine::startFight lordAttack %d, lordDefense id %d", lordAttack, lordDefense->getId());

	if( !_fight ) {
		_fight = new FightEngine( _server );
		QObject::connect( _fight, SIGNAL( sig_endFight( FightResultStatus ) ), SLOT( slot_endFight( FightResultStatus ) ) );
	}
	_state = IN_FIGHT_LORD;
	_fight->init( _currentPlayer, _currentPlayer->getLordById( lordAttack ), lordDefense->getOwner(), lordDefense );
}

void Engine::startFight( int lordAttack, GenericMapCreature * creature )
{
	TRACE("Engine::startFight lordAttack %d, creature %p", lordAttack, creature );

	if( !_fight ) {
		_fight = new FightEngine( _server );
		QObject::connect( _fight, SIGNAL( sig_endFight( FightResultStatus ) ), SLOT( slot_endFight( FightResultStatus ) ) );
	}
	_state = IN_FIGHT_CREATURE;
	
	_fight->init( _currentPlayer, _currentPlayer->getLordById( lordAttack ), (GameData *)this, creature );

	TRACE( "start fight finished" );
}

void Engine::slot_endFight( FightResultStatus result )
{
	TRACE("Engine::slot_endFight");

	if( _state == NOT_PLAYING ) {
		return;
	}

	GenericLord * loser, * winner;

	if( result.hasDefenseWin() ) {
		loser = _fight->getAttackLord();
		winner = _fight->getDefendLord();
	} else {
		loser = _fight->getDefendLord();
		winner = _fight->getAttackLord();
	}

	if( _state == IN_FIGHT_CREATURE ) {
		if( loser == _fight->getDefendLord() ) {
			GenericMapCreature * creature = _fight->getDefendCreature();
			_currentPlayer->getResourceList()->addResources( creature->getResourceList() );
			_server->sendPlayerResources( _currentPlayer );

			QString msg = creature->getResourceList()->getAutoDescription();
			if( !msg.isEmpty() ) {
				msg.prepend( "You gain:");
				_server->sendMessage( _currentPlayer , msg );
			}

			removeCreature( creature );

			uint experience = _fight->getExperience( winner );
			manageIncreaseExperience( winner, experience );
		} else {
			_server->sendLordRemove( _players, loser );
			loser->removeFromGame();
		}
	//else IN_FIGHT_LORD only??
	} else {
		GenericBase * garrBase = loser->getCell()->getBase();

		_server->sendLordRemove( _players, loser );
		loser->removeFromGame();
		uint experience = _fight->getExperience( winner );
		manageIncreaseExperience( winner, experience );

		if( winner->getOwner() == _currentPlayer ) {
			if( garrBase ) {
				movingOnBase( winner, garrBase->getCell() );
			}
		}
	}

	updatePlayers();
	
	if(_fight){
		delete _fight;
	}
	_fight = NULL;

	_state = IN_GAME;
}

void Engine::manageIncreaseExperience( GenericLord * lord, int experience )
{
	TRACE("manageIncreaseExperience %d", experience);
	uint currentLevel;
	int nextLevelExp;
		
	//currentLevel = DataTheme.lordExperience.computeLevelForExperience( lord->getCharac( EXPERIENCE ) );
	currentLevel = lord->getBaseCharac( LEVEL );
	int lordExperience = lord->getCharac( EXPERIENCE );
	experience += lordExperience;
		
	while( experience > 0 ) {
		if( currentLevel < DataTheme.lordExperience.getLevelNumber() ) {
			nextLevelExp = DataTheme.lordExperience.getLevel( currentLevel + 1 );

			if( experience >= nextLevelExp ) {
				lord->increaseBaseCharac( EXPERIENCE, experience - nextLevelExp );
				GenericLordModel * model = DataTheme.lords.at( lord->getId() );
				LordCharac charac = model->getCategory()->getRandomEvolution();
				lord->increaseBaseCharac( charac, 1 );
				_server->sendLordCharac( lord->getOwner(), lord, charac );
				currentLevel++;
				lord->setBaseCharac( EXPERIENCE, experience );
				lord->setBaseCharac( LEVEL, currentLevel );
				experience -= nextLevelExp;
			} else {
				lord->setBaseCharac( EXPERIENCE, experience );
				break;
			}				
		} else {
			break;
		}
	}
	_server->sendLordCharac( lord->getOwner(), lord, LEVEL );
	_server->sendLordCharac( lord->getOwner(), lord, EXPERIENCE );
	
}

bool Engine::saveGame( const QString & filename )
{
	QString filename2 = filename;

	if( filename2.isNull() ) {
		return false;
	}
	
	if( !filename2.contains(".gam")){
		filename2.append(".gam");
	}

	QFile f( filename2 );

	if (! f.open(QIODevice::WriteOnly) ) {
		logEE("Could not open file %s for writing\n", qPrintable(filename) );
		return false;
	}
	
	QTextStream ts( &f );
	GameData::save( &ts );
	f.close();
		
	if(getCampaign()) {
		saveCampaign( filename2 );
	}
	
	return true;
}

bool Engine::saveCampaign( const QString & filename )
{
	QString filenamecmp = filename;
	QString filename2;

	if( filenamecmp.contains(".gam")){
		filenamecmp.remove(".gam");
	}

	if( !filenamecmp.contains(".cms")){
		filenamecmp.append(".cms");	
	}
	
	//filenamecmp = QFileDialog::getSaveFileName( this, "",CAMPAIGN_PATH ,  "*.cms" );

	filename2 = filename.section(QDir::separator(),-1,-1);

	QFile f( filenamecmp );

	if (! f.open(QIODevice::WriteOnly) ) {
		logEE( "Could not open file %s for writing\n", qPrintable( filename2 ) );
		return false;
	}

	//don't use pointers, it is a temporary copy used to avoid to mess up with original campaign class
	Campaign campaign( *getCampaign() );

	campaign.replaceScenario( campaign.getCurrentScenario() , filename2 );

	QTextStream ts( &f );

	campaign.save( & ts );

	f.close();

	return true;

}

void Engine::newDay()
{
	_calendar->newDay();
	
	TRACE("Engine::newDay, turn %d", _calendar->getTurn());

	_server->sendGameCalendar( _players, _calendar);
	checkNewWeek();
}

void Engine::checkNewWeek()
{
	TRACE("Engine::checkNewWeek");

	if( _calendar->getDay() == 1 ) {
		// new week
		updateProduction();
		updateCreatures();
	}
}

void Engine::updateProduction()
{
	TRACE("Engine::updateProduction");

	uint nbItems = _bases.count();
	for( uint i = 0; i < nbItems; ++i ) {
		_bases.at( i )->initCreatureProduction();
		
		//only can see
		_server->sendBaseProduction( _players, _bases.at( i )  );
	}
}

void Engine::updateCreatures()
{
	TRACE("Engine::updateCreatures");

	uint nbItems = _creatures.count();
	for( uint i = 0; i < nbItems; ++i ) {
		_creatures.at( i )->grow();
		_server->updateCreature( _players, _creatures.at( i ) ); /// XXX:  we could update only if 'category' has changed...
	}
}

void Engine::nextPlayer()
{
 TRACE("Engine::nextPlayer");

	uint i; 

	//check if a player can continue to play
	updatePlayers();

	//check win
	if( ! enoughPlayers() ) {
		checkResult();
		endGameInternal();
		return;
	}

	if( checkMainQuest() ) {
		endGameInternal();
		return;
	}

	//update player state
	//refactor, code is messy
	checkPlayerShouldPlay();

	TRACE("Engine::nextPlayer _currentPlayer %p", _currentPlayer );

	for( i = 0; i < _currentPlayer->numBuilding(); i++ ) {
		handleBuildingTurn( _currentPlayer, _currentPlayer->getBuilding( i ) );
	}
	for( i = 0; i < _currentPlayer->numBase(); i++ ) {
		handleBaseTurn( _currentPlayer, _currentPlayer->getBase( i ) );
	}
	for( i = 0; i < _currentPlayer->numLord(); i++ ) {
		handleCreatTurn( _currentPlayer, _currentPlayer->getLord( i ) );
	}

	_currentPlayer->newTurn();

	for( uint j = 0; j < _currentPlayer->numLord(); ++j ) {
		_server->sendLordCharac( _currentPlayer, _currentPlayer->getLord( j ), MOVE );
	}

	_server->beginTurn( _players, _currentPlayer );

	TRACE("Engine::nextPlayer end _state %d", _state);
}

//change function name, confuse?
void Engine::checkPlayerShouldPlay() 
{	
	TRACE("Engine::checkPlayerShouldPlay");

	nextCurrentPlayer();

	int i = 0;

	while( !_currentPlayer->isAlive() ) {
		nextCurrentPlayer();
		//index placed for safety reason, should be not necessary
		i++;
		if( i == _players.count() ){
			logEE("endless loop");
			break;
		}
	}

}

void Engine::nextCurrentPlayer() 
{

	int num = _players.indexOf( _currentPlayer );
	num++;
	if( num == _players.count() ) {
		_currentPlayer = _players.at( 0 ); 
		_currPlayNum = 0;
		newDay();
	} else {
		_currentPlayer = _players.at( num );
		_currPlayNum = num;
	}
}

void Engine::handleBuildingTurn( GenericPlayer * player, GenericBuilding * building )
{
	QList<Action *> list = building->getActionList( Action::DATE );
	handleActionListDate( list, player, building->getResourceList()	);
	_server->sendBuildingResources( player , building );
}

void Engine::handleBaseTurn( GenericPlayer * player, GenericBase * base )
{
	
	QList<Action *>  list = base->getActionList( Action::DATE );
	handleActionListDate( list, player, base->getResourceList() );
	_server->sendBaseResources( player , base );
	_server->sendBaseState( player, base, GenericBase::BASE_BUY, true );
	_server->sendBaseState( player, base, GenericBase::BASE_SELL, true );
	
	for( uint i = 0; i < base->getBuildingCount(); i++ ) {
		GenericInsideBuilding * inbuil = base->getBuilding(i);
		for( int j = 0; j < DataTheme.resources.count(); j++ ) {
			player->getResourceList()->decreaseValue( j,inbuil->getMantCost(j));
			_server->sendPlayerResource( player,j, player->getResourceList()->getValue(j)  );
		}
	}
	
	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		player->getResourceList()->increaseValue( j, base->collectRessource(j));
		_server->sendPlayerResource( player, j, player->getResourceList()->getValue(j)  );
	}
}

void Engine::handleCreatTurn( GenericPlayer * player, GenericLord * lord )
{
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = lord->getUnit(i);
		if( unit ) {
			for( int j = 0; j < DataTheme.resources.count(); j++ ) {
				if(unit->getMantCost (j)) {
					int mant = ( unit->getMantCost( j ) * unit->getNumber() );
#if 0
					int ress = player->getResource(j);
					uint decr = (mant - ress) / unit->getMantCost( j ) ;
					decr = decr > 0 ? decr : 0;
					if (decr > 0) {
						decr = decr > unit->getNumber() ? unit->getNumber() : unit->getNumber()-decr;
						unit->setNumber(decr);
						lord->setUnit(i,unit);
					}
#endif	
					player->getResourceList()->decreaseValue( j, mant );
					_server->sendPlayerResource( player, j, player->getResourceList()->getValue( j )  );
				}
			}
		}
	}
	//_server->updateUnits(lord);
}

void Engine::handleActionListDate( QList<Action *> list, GenericPlayer * player, GenericResourceList * rlist )
{
	for( int i = 0; i < list.count(); i++ ) {

		// this procedure is used to estabilish a production every coeff days ( a % b )
		// coeff zero excluded, obv.
		
		int coeff = list.at( i )->getCoeff();
		coeff = ( coeff == 0 ) ? 1 : coeff;
		int isTurn = _calendar->getCurrentAbsoluteDate() % coeff ;
		 if( isTurn	== 0 ) {
			handleAction( list.at( i ), player, rlist);
		}
	}
}

void Engine::handleAction( Action * action, GenericPlayer * player, GenericResourceList * rlist )
{
	GenericResourceList * plist = player->getResourceList();

	for( int i = 0; i < action->getElementaryNumber(); i++ ) {
		ElementaryAction * elementary = action->getElementaryAction( i );
		switch( elementary->getType() ) {
			case ElementaryAction::RESSOURCE:
				TRACE("Engine::handleAction RESSOURCE, arg %d, coeff %d", elementary->getArg(), elementary->getCoeff() );
				if( DataTheme.resources.get( elementary->getArg() )->isGlobal() ) {
					plist->increaseValue( elementary->getArg() , elementary->getCoeff() );
					_server->sendPlayerResource( player, elementary->getArg(), plist->getValue( elementary->getArg() ) );
				} else {
					//XXX no update to player? Explain better in documentation
					rlist->increaseValue( elementary->getArg() , plist->getValue( elementary->getArg() ) );
				}
				break;
			case ElementaryAction::RANDRESSOURCE:
				TRACE("Engine::handleAction RANDRESSOURCE, arg %d, coeff %d", elementary->getArg(), elementary->getCoeff() );
				if( DataTheme.resources.get( elementary->getArg() )->isGlobal() ) {
					plist->updateRandGrowth( elementary->getArg(), elementary->getCoeff() );
					_server->sendPlayerResource( player, elementary->getArg(), plist->getValue( elementary->getArg() ) );
				} else {
					rlist->updateRandGrowth( elementary->getArg(), elementary->getCoeff() );
				}
				break;			
			case ElementaryAction::RESOURCEPERC:
				{
					TRACE("Engine::handleAction RESOURCEPERC, arg %d, coeff %d", elementary->getArg(), elementary->getCoeff() );
					if( DataTheme.resources.get( elementary->getArg() )->isGlobal() ) {
						plist->modPercValue( elementary->getArg(), elementary->getCoeff() );
						_server->sendPlayerResource( player, elementary->getArg(), plist->getValue( elementary->getArg() ) );
					} else {
						rlist->modPercValue( elementary->getArg(), elementary->getCoeff() );
					}
				}
				break;
			case ElementaryAction::RESOURCEFROM:
				{
					TRACE("Engine::handleAction RESOURCEFROM, arg %d, coeff %d", elementary->getArg(), elementary->getCoeff() );
					TRACE("Engine::handleAction RESOURCEFROM, arg1 %d", elementary->getArg1() );
					if( ! rlist ) {
						return;
					}
					int increase = rlist->getValue( elementary->getArg() ) / elementary->getCoeff();
					TRACE("Engine::increase RESOURCEFROM,  %d", increase );
					plist->increaseValue( elementary->getArg1(), increase  );
					_server->sendPlayerResource( player, elementary->getArg(), player->getResourceList()->getValue( elementary->getArg1() ) );
					if( !DataTheme.resources.get( elementary->getArg1() )->isGlobal() ) {
						rlist->setValue( elementary->getArg(), plist->getValue( elementary->getArg() ) );
					}
				}
				break;
			default:
				break;
		}
	}
}

bool Engine::loadCampaign( const QString & filename )
{
	bool ret = true;
	
	if( _campaign ) {
		delete _campaign;
	}
	_campaign = new Campaign();
	if( !_campaign->load( filename ) ) {
		delete _campaign;
		_campaign = NULL;
		ret = false;
	}
	
	return ret;
}

int Engine::getNeededPlayersFast( const QString & filename )
{
	static QString filenamest = "";
	static int scenPlayers = 0;

	if( filenamest != filename ) {
		ScenarioDescription * desc = new ScenarioDescription();
		desc->load( filename );
		scenPlayers = desc->getNbPlayers();
		filenamest = filename;
		delete desc;
	}

	TRACE( "Engine::checkGamePlayers real players %d ", _players.count() );
	TRACE( "Engine::checkGamePlayers scenario players %d ", scenPlayers);
	TRACE( "Engine::checkGamePlayers filename %s ", qPrintable( filename ) );
	
	return scenPlayers - _players.count();
}

uint Engine::getAlivePlayersNumber()
{
	uint ret = 0;

	for( int i = 0; i < _players.count(); i++ ) {
		if( _players.at( i )->isAlive() ) {
			ret++;
		}
	}

	return ret;
}

void Engine::checkResult()
{
 TRACE("Engine::checkResult");

 bool ret = false;

	for( int i = 0; i < _players.count(); i++ ) {
		if( /*!_players.at( i )->isRuledByAi()  && */ _players.at( i )->isAlive() ) {
			_server->playerWin( _players, _players.at( i ) );
			ret = true;
		}
	}
	emit sig_result(  _gameId, ret );

}
	
void Engine::updatePlayers()
{
	TRACE("Engine::updatePlayers");

	for( int i = 0; i < _players.count(); i++ ) {
		GenericPlayer * player = _players.at(i);
		if( player->isAlive() && ( player->numBase() == 0 ) and ( player->numLord() == 0 ) ) {
			player->setAlive( false );
			for( uint i = 0; i < player->numBuilding(); i++ ) {
				GenericBuilding * build =	player->getBuilding( i );
				build->setOwner( NULL );
				_server->ownBuilding( _players , build );
			}

			_server->playerLose( _players, player );
		}
	}

}

bool Engine::enoughPlayers()
{

	uint ret = getAlivePlayersNumber();
	uint num = _players.count();
	
	TRACE("Engine::enoughPlayers num players %d, alive %d", num, ret );

	if( num == 1 && ret == 1 ) {
		 return true;
	}
	
	if( num > 1 && ret > 1 ) {
		return true;
	}

	return false; 
}

void Engine::updateMapVision( QList<GenericCell *> & removed, QList<GenericCell *> & added )
{
	for( int i = 0; i < added.count(); i++ ) {
		GenericCell * cell = added.at( i );
		if( removed.indexOf( cell ) == -1 ) {
			updateCellVision( cell, _currentPlayer );
		}
	}
}

void Engine::updateCellVision( GenericCell * cell, GenericPlayer * player )
{
	_server->sendCell( player , cell );

	if( cell->getLord() ) {
		_server->newLord( player , cell->getLord() );
	}

	if( cell->getCreature() ) {
		_server->newCreature( player , cell->getCreature() );
	}

	if( cell->getBase() ) {
		_server->newBase( player , cell->getBase() );
		if(cell->getBase()->getOwner()){
			_server->ownBase( player , cell->getBase());
		}
	}

	if( cell->getBuilding() ) {
		_server->newBuilding( player, cell->getBuilding() );
		if(cell->getBuilding()->getOwner()){
			_server->ownBuilding( player , cell->getBuilding() );
		}
	}

	if( cell->getEvent() ) {
		_server->newEvent( player, cell->getEvent() );
	}/// XXX: other cell attr to send...
}

Engine::CreatureAction Engine::computeCreatureAction( GenericMapCreature * creature, double ratio )
{
	CreatureAction ret = CreatureFight;

	switch( creature->getBehaviour() ) {
		case GenericMapCreature::Obedient:
			ret = CreatureJoin;
			break;
		case GenericMapCreature::Friendly:
			if( ratio < 0.75 ) {
				ret = CreatureFight;
			} else if( ratio < 1.25 ) {
				ret = CreatureMercenary;
			} else {
				ret = CreatureJoin;
			}
			break;
		case GenericMapCreature::Neutral:
			if( ratio < 1.0 ) {
				ret = CreatureFight;
			} else if( ratio < 1.5 ) {
				ret = CreatureFlee;
			} else if( ratio < 2.0 ) {
				ret = CreatureMercenary;
			} else {
				ret = CreatureJoin;
			}
			break;
		case GenericMapCreature::Aggressive:
			if( ratio < 1.5 ) {
				ret = CreatureFight;
			} else if( ratio < 3.0 ) {
				ret = CreatureFlee;
			} else {
				ret = CreatureMercenary;
			}
			break;
		case GenericMapCreature::Hostile:
			if( ratio < 2.5 ) {
				ret = CreatureFight;
			} else {
				ret = CreatureFlee;
			}
			break;
	}

	if( ( ! creature->isFleeing() ) && ( ret == CreatureFlee ) ) {
		ret = CreatureFight;
	}

	return ret;
}

bool Engine::checkMainQuest()
{
	TRACE("Engine::checkMainQuest");

	QuestData data;
	QuestCondition * failCondition, * successCondition;

	data.setPlayer( _currentPlayer );
	data.setCurrentTurn( _calendar->getTurn() );
	data.setCurrentAbsoluteDate( _calendar->getCurrentAbsoluteDate() );

	Quest * main = _quests->getMainQuest();

	if( main ) {
		failCondition = main->getFailCondition();
		successCondition = main->getSuccessCondition();

		uint nbLord = _currentPlayer->numLord();
		for( uint i = 0; i < nbLord; i++ ) {
			data.setLord( _currentPlayer->getLord( i ) );

			if( failCondition ) {
				if( failCondition->check( &data ) ) {
					_currentPlayer->setAlive( false );
					_server->playerLose( _players, _currentPlayer );

					for( int i = 0; i < _players.count(); i++ ) {
						if( _players.at( i )->isAlive() && _players.at( i ) != _currentPlayer ) {
							_server->playerWin( _players, _players.at( i ) );
						}
					}

					if(!_currentPlayer->isRuledByAi()) {
						emit sig_result( _gameId, false);
					}
					return true;
				}
			}

			if( successCondition ) {
				if( successCondition->check( &data ) ) {
					_server->playerWin( _players, _currentPlayer );

					for( int i = 0; i < _players.count(); i++ ) {
						if( _players.at( i )->isAlive() && _players.at( i ) != _currentPlayer ) {
							_server->playerLose( _players, _players.at( i ) );
						}
					}

					if(!_currentPlayer->isRuledByAi()) {
						emit sig_result( _gameId, true);
					}
					return true;
				}
			}

		}

	}

	return false;

}

void Engine::updatePlayerPrices( GenericPlayer * player)
{

	uint nbases;
	int mincost,tmpr;
	uint nbRes = DataTheme.resources.count();

	PriceMarket * pricePlayer = player->getPriceMarket();

	for( uint i = 0; i < nbRes; i++ ) {
		nbases = player->numBase();
		mincost = std::numeric_limits<int>::max();
		for( uint j = 0 ; j < nbases ; j++){
			//update prices so that the player have the minor cost of all market it own
			PriceMarket * price = DataTheme.bases.at( player->getBase( j )->getRace())->getPriceMarket();
			tmpr = price->getResourcePrice( i ) ;
			if( tmpr < mincost ) {
				mincost = tmpr;
			}
		}
		pricePlayer->setResourcePrice( i, mincost );
	}

}

void Engine::moveLord( GenericLord * movingLord, GenericCell * cell )
{
	uchar lord = movingLord->getId();

  //cell and movingLord->getCell() are the same thing	
	int row = movingLord->getCell()->getRow();
	int col = movingLord->getCell()->getCol();

	cell->setLord( movingLord );
	
	_server->sendMvt( _players, lord, row, col );

}

void Engine::decreaseMove( GenericLord * movingLord, GenericCell * cell )
{
	GenericCell * start = movingLord->getCell();
	int cost = PathFinder::computeCostMvt( start, cell );

	movingLord->decreaseBaseCharac( MOVE, cost );
	_server->sendLordCharac( _currentPlayer, movingLord, MOVE );
}

void Engine::removeCreature( GenericMapCreature * creature )
{
	GenericCell * cell = creature->getCell();
	_server->sendCreatureRemove( _players, cell );

	cell->setCreature( NULL);
	removeMapCreature( creature );
}

inline uchar Engine::getCla1()
{
	return _currentData.getCla1();
}

inline uchar Engine::getCla2()
{
	return _currentData.getCla2();
}

inline uchar Engine::getCla3()
{
	return _currentData.getCla3();
}

inline int Engine::readInt()
{
	return _currentData.readInt();
}

inline unsigned char Engine::readChar()
{
	return _currentData.readChar();
}
