/****************************************************************
**
** Attal : Lords of Doom
**
** engine.cpp
** the game engine !
**
** Version : $Id: engine.cpp,v 1.102 2003/05/08 11:02:37 audoux Exp $
**
** Author(s) : Pascal Audoux
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
// include files for QT
#include <qfile.h>
#include <qxml.h>
#include <qtextstream.h>
#include <qmessagebox.h>
// application specific includes
#include "common/unit.h"
#include "common/artefact.h"
#include "common/dataTheme.h"
#include "common/genericEvent.h"
#include "common/genericMapCreature.h"
#include "common/genericInsideBuilding.h"

#include "server/parser.h"

extern DataTheme DataTheme;

Engine::Engine( AttalServer * serv )
	:GameData()
{
	_counter=0;
	_currentPlayer = 0;
	_fight = 0;
	_server = serv;
	_state = NOT_PLAYING;
	_calendar = new Calendar();
	_isCreature = false;

	QObject::connect( _server, SIGNAL( sig_readEvent( int ) ), SLOT( slot_readSocket( int ) ) );
	QObject::connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), SLOT( slot_newPlayer( AttalPlayerSocket * ) ) );
	QObject::connect( _server, SIGNAL( sig_endConnection( QString ) ), SLOT( slot_endConnection( QString ) ) );
}

void Engine::slot_readSocket( int num )
{
	_server->getSocketData( num );

	switch( _state ) {
	case NOT_PLAYING:
		stateNotPlaying( num );
		break;
	case IN_GAME:
		stateInGame( num );
		break;
	case IN_FIGHT:
		stateInFight( num );
		break;
	}

	if( _server->isData( num ) ) {
		slot_readSocket( num );
	}
}

void Engine::slot_endConnection( QString name )
{
	for( uint i = 0; i < _players.count(); i++ ) {
		if( _players.at( i )->getName() == name ) {
			_players.remove( _players.at( i ) );
		}
	}
}

void Engine::handleMessage()
{
	QString msg;
	uchar d = _server->readChar();
	for( uint i = 0; i < d; i++ ) {
		msg.ref( i ) = _server->readChar(); /// XXX: WINDOWS bug (was msg[i] =)
	}
	_server->sendMessage( msg );
}

void Engine::stateNotPlaying( int num )
{
	uchar c = _server->getCla1();
	switch( c ) {
	case SO_MSG:
		handleMessage();
		break;
	case SO_CONNECT:
		switch( _server->getCla2() ) {
		case C_CONN_NAME:{
			QString res;
			/// XXX:  CHECK build of 'res'
			_players.at( num )->setNum( num );
			uint len = _server->readChar();
			for( uint i = 0; i < len; i++) {
				res.append( _server->readChar() );
			}
			/// XXX: be sure that name is unique...
			_players.at( num )->setName( res );
			_server->sendConnectionId( num );
			emit sig_newPlayer( _server->findSocket( _players.at( num ) ) );}
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
	default:
		logEE( "Game not started...%d", c );
	}
}

void Engine::stateInGame( int num )
{
	if( num == _players.find( _currentPlayer ) ) {
		switch( _server->getCla1() ) {
		case SO_MSG:
			handleMessage();
			break;
		case SO_MVT:
			handleInGameMvt( num );
			break;
		case SO_QR:
			switch( _server->getCla2() ) {
			case C_QR_ANSWER:
				break;
			default:
				logEE( "Should not happen (Server : SO_QR/...)" );
			}
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
		case SO_GAME:
			logDD( "Not yet implemented" );
			break;
		case SO_MODIF:
			handleInGameModif();
			break;
		case SO_TURN:
			switch( _server->getCla2() ) {
			case C_TURN_BEG:
				logEE( "Should not happen (Server : SO_TURN/C_TURN_BEG)" );
				break;
			case C_TURN_PLAY:
				logEE( "Should not happen (Server : SO_TURN/C_TURN_PLAY)" );
				break;
			case C_TURN_LORD:{
				int type=_server->readInt();
				if(type==1){
						_counter++;
					}
						_server->sendLordPlTurn(_counter);
				 }
				break;
			case C_TURN_PLORD:
				logEE( "Should not happen (Server : SO_TURN/C_TURN_PLORD)" );
				break;
			case C_TURN_END:
				nextPlayer();
				break;
			}
			break;
		case SO_CONNECT:
			switch( _server->getCla2() ) {
			case C_CONN_NAME:
				_players.at( num )->setNum( num );
				_players.at( num )->setName( "Player" );
				_server->sendConnectionId( num );
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
		default:
			logEE( "Unknown socket_class from %d", num );
		}
	} else {
		switch( _server->getCla1() ) {
		case SO_MSG:
			handleMessage();
			break;
		default:
			logEE( "this player should not play now" );
			break;
		}
	}
}

void Engine::handleInGameMvt( int num )
{
	/// XXX : check validity of mvt
	/// XXX : to improve : all clients concerned
	uchar lord = _server->readChar();
	int row = _server->readInt();
	int col = _server->readInt();

	int i,j;
	GenericLord * movingLord = _currentPlayer->getLordById( lord );
	GenericPlayer * tempPlayer;

	if( ! ( ( ( movingLord->getCell()->getRow() - 1 ) <= row ) &&
	    ( ( movingLord->getCell()->getRow() + 1 ) >= row ) &&
	    ( ( movingLord->getCell()->getCol() - 1 ) <= col ) &&
	    ( ( movingLord->getCell()->getCol() + 1 ) >= col ) ) ) {
	    	logEE( "Player should not request this mvt for lord %d %d %d %d",
		movingLord->getCell()->getRow(), movingLord->getCell()->getCol(), row, col );
		return;
	}

	int cost = movingLord->computeCostMvt( _map->at( row, col ) );
	if( cost > movingLord->getCharac( MOVE ) ) {
		logDD( "not enough mvt pt : %d < %d", movingLord->getCharac( MOVE ), cost );
		return;
	}


	if( _map->at( row, col )->getLord() ) {
		if( _map->at( row, col )->getLord()->getOwner() == _currentPlayer ) {
			_server->sendLordExchange( movingLord, _map->at( row, col )->getLord() );
		} else {
			movingLord->decreaseBaseCharac( MOVE, cost );
			startFight( lord, _map->at( row, col )->getLord() );
			_isCreature = false;
		}
	} else if( _map->at( row, col )->getArtefact() ) {
		GenericArtefact * artef = _map->at( row, col )->getArtefact();
		_currentPlayer->getLordById( lord )->addArtefact( artef );
		_map->at( row, col )->setArtefact( 0 );
		_server->ownArtefact( artef, _currentPlayer->getLordById( lord ), _currentPlayer );
	} else if( _map->at( row, col )->getBuilding() ) {
		// XXX: reread (cost computed ?...
		GenericBuilding * building = _map->at( row, col )->getBuilding();
		if( handleBuildingEnter( building, _currentPlayer->getLordById( lord ) ) ) {
			_currentPlayer->getLordById( lord )->moveTo( _map->at( row, col ) );
			if( ! _currentPlayer->hasBuilding( building ) ) {
				for( i = 0; i < _players.count(); i++ ) {
					tempPlayer = _players.at( i );
					int tmpnbuil=tempPlayer->numBuilding();
					for( j = 0; j < tmpnbuil; j++ ) {
						if(tempPlayer->getBuilding(j)==building)
						{
							tempPlayer->removeBuilding(j);
						}
					}
				}
					_currentPlayer->addBuilding( building );
			}
			_map->at( row, col )->setLord( _currentPlayer->getLordById( lord ) );
			_server->sendMvt( _players.at( num ), lord, row, col );
		}
	} else if( _map->at( row, col )->getBase() ) {
		// test if we have to fight before...
		GenericBase * base = _map->at( row, col )->getBase();
		if( ! _currentPlayer->hasBase( base ) ) {
			if( base->getOwner() ) {
				base->getOwner()->removeBase( base );
			}
			_currentPlayer->addBase( base );
			base->setOwner( _currentPlayer );
			_server->ownBase( base, _currentPlayer );
		}
		movingLord->decreaseBaseCharac( MOVE, cost );
		movingLord->moveTo( _map->at( row, col ) );
		_map->at( row, col )->setLord( _currentPlayer->getLordById( lord ) );
		_server->sendMvt( _players.at( num ), lord, row, col );
	} else if( _map->at( row, col )->getCreature() ) {
		/// XXX: handle friendly map creatures...
		movingLord->decreaseBaseCharac( MOVE, cost );
		startFight( lord, _map->at( row, col )->getCreature() );
		_isCreature = true;
	} else {
		movingLord->decreaseBaseCharac( MOVE, cost );
		QList<GenericCell> removedCells = _currentPlayer->removeLordVision( movingLord );
		movingLord->moveTo( _map->at( row, col ) );
		QList<GenericCell> addedCells = _currentPlayer->addLordVision( movingLord );
		updateMapVision( removedCells, addedCells );
		_map->at( row, col )->setLord( _currentPlayer->getLordById( lord ) );
		_server->sendMvt( _players.at( num ), lord, row, col );
	}
}

void Engine::handleInGameExchange()
{
	switch( _server->getCla2() ) {
	case C_EXCH_UNIT:
		exchangeUnits();
		break;
	case C_EXCH_BASEUNIT:
		exchangeBaseUnits();
		break;
	default:
		break;
	}
}

void Engine::exchangeUnits()
{
	uchar idLord1 = _server->readChar();
	uchar idUnit1 = _server->readChar();
	uchar idLord2 = _server->readChar();
	uchar idUnit2 = _server->readChar();
	GenericLord * lord1 = 0;
	GenericLord * lord2 = 0;
	if( idLord1 ) {
		lord1 = _currentPlayer->getLordById( idLord1 );
	}
	if( idLord2 ) {
		lord2 = _currentPlayer->getLordById( idLord2 );
	}

	/// XXX: check player of lord ?
	if( lord1 && lord2 ) {
		GenericFightUnit * unit1 = lord1->getUnit( idUnit1 );
		GenericFightUnit * unit2 = lord2->getUnit( idUnit2 );

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
				lord2->setUnit( idUnit2, unit1 );
				lord1->setUnit( idUnit1, 0 );
			}
			_server->sendExchangeUnit( lord1, idUnit1, lord2, idUnit2 );
		}
	}
}

void Engine::exchangeBaseUnits()
{
	uchar idBase = _server->readChar();
	uchar idUnit1 = _server->readChar();
	uchar idLord = _server->readChar();
	uchar idUnit2 = _server->readChar();
	
	GenericBase * base = _currentPlayer->getBaseById( idBase );
	GenericLord * lord = 0; 
	if(idLord && idLord<255){
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
			if( uni1 && uni2 ) {
				if( uni1->getCreature() != uni2->getCreature() ) {
					lord->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni1->addNumber( uni2->getNumber() );
					lord->setUnit( idUnit2, 0 );
					delete uni2;
				}
			} else if (!uni1) {
				lord->setUnit( idUnit2, 0 );
				base->setUnit( idUnit1, uni2 );
			} else if (!uni2) {
				lord->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		} else {
			if(idUnit2<=MAX_UNIT){
				uni2 = base->getUnit( idUnit2);
			}
		if( uni1 && uni2 ) {
			if( uni1->getCreature() != uni2->getCreature() ) {
					base->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni1->addNumber( uni2->getNumber() );
					base->setUnit( idUnit2, 0 );
					delete uni2;
				}
			} else {
				base->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		}	
			_server->sendExchangeBaseUnitCl(base,idUnit1,lord,idUnit2);
	}
}

void Engine::handleInGameModif()
{
	switch( _server->getCla2() ) {
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
	}
}

void Engine::handleInGameModifBase()
{
	switch( _server->getCla3() ) {
	case C_BASE_BUILDING: {
		int id = _server->readChar();
		int building = _server->readChar();

		GenericBase * base = _currentPlayer->getBaseById( id );
		GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
		InsideBuildingModel * buildingModel = baseModel->getBuildingModel( building );

		if( base->canBuild() && _currentPlayer->canBuy( buildingModel ) ) {
			for( uint i = 0; i < MAX_RESS; i++ ) {
				if( buildingModel->getCost( i ) != 0 ) {
					_currentPlayer->decreaseResource( i, buildingModel->getCost( i ) );
					_server->sendPlayerResource( i, _currentPlayer->getResource( i ) );
				}
			}
			base->addBuilding( building );
			base->canBuild( false );
			_server->updateBaseBuilding( base, base->getBuildingByType( building ) );
		}
		} break;
	case C_BASE_UNIT: {
		int row = _server->readInt();
		int col = _server->readInt();
		if( _map->at( row, col )->getBase() ) {
			GenericBase * base = _map->at( row, col )->getBase();
			uchar race = _server->readChar();
			uchar level = _server->readChar();
			int number = _server->readInt();
			Creature * creature = DataTheme.creatures.at( race, level );
			if( base->getCreatureProduction( creature ) >= number ) {
				if( _currentPlayer->canBuy( creature, number ) ) {
					_currentPlayer->buy( creature, number );
					base->addGarrison( creature, number );
					base->buyCreature( creature, number );
					_server->sendPlayerResources( _currentPlayer );
					_server->sendBaseUnit( base, creature, number );
				}
			}
		}
		} break;
	default:
		logDD( "Not yet implemented" );
		break;
	}
}

void Engine::handleInGameModifLord()
{
	switch( _server->getCla3() ) {
	case C_LORD_GARRISON: {
			uchar idLord = _server->readChar();
			bool garrison = ( _server->readChar() == (uchar)1 );
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
						_server->setGarrison( lord, garrison );
					}
				} else if( ( base->getGarrisonLord() == 0 ) ||
				( base->getVisitorLord() == 0 ) ) {
					if( garrison ) {
						if( base->getGarrisonLord() ) {
							base->exchangeLords();
						}
						base->setGarrisonLord( lord );
						_server->setGarrison( lord, true );
					} else {
						if( base->getVisitorLord() ) {
							base->exchangeLords();
						}
						base->setVisitorLord( lord );
						_server->setGarrison( lord, false );
					}
				}
			}
		}
		break;
	default:
		logDD( "Not yet implemented (modif lord)" );
		break;
	}
}

bool Engine::handleBuildingEnter( GenericBuilding * building, GenericLord * lord )
{
	bool enter = false;
	QList<Action> list;

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
		for( uint i = 0; i < list.count(); i++ ) {
			handleBuildingAction( list.at( i ), lord );
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
		for( uint i = 0; i < list.count(); i++ ) {
			handleBuildingAction( list.at( i ), lord );
		}
		building->enter( lord );
	} else {
		if( building->getCondition() ) {
		}
		enter = true;
		list = building->getActionList( Action::NEXTTIME );
		for( uint i = 0; i < list.count(); i++ ) {
			handleBuildingAction( list.at( i ), lord );
		}
		building->enter( lord );
	}
	return enter;
}

void Engine::handleBuildingAction( Action * action, GenericLord * lord )
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
				lord->increaseBaseCharac( POWER, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, POWER );
				break;
			case ElementaryAction::KNOWLEDGE:
				lord->increaseBaseCharac( KNOWLEDGE, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, KNOWLEDGE );
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
				lord->increaseBaseCharac( TECHNICPOINT, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, TECHNICPOINT );
				break;
			case ElementaryAction::MAXTECHNICPOINT:
				lord->increaseBaseCharac( MAXTECHNICPOINT, _elementary->getCoeff() );
				_server->sendLordCharac( _currentPlayer, lord, MAXTECHNICPOINT );
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
				_server->sendLordCharac( _currentPlayer, lord, EXPERIENCE );
				/// XXX: check exp for level...
				break;
			case ElementaryAction::SKILL:
				break;
			case ElementaryAction::ARTEFACT:
				break;
			case ElementaryAction::UNIT:
				break;				
			default:
				break;
		}
	}
}

void Engine::stateInFight( int num ) 
{
	if( _fight ) {
		switch( _server->getCla1() ) {
		case SO_MSG:
			handleMessage();
			break;
		case SO_FIGHT:
			_fight->handleSocket( _players.at( num ) );
			break;
		default:
			logEE( "Should not happen" );
			break;
		}
	} else {
		logEE( "Should not happen, _fight == 0" );
	}
}

void Engine::slot_newPlayer( AttalPlayerSocket * player )
{
	_players.append( player->getPlayer() );
}

void Engine::reinit()
{
	_state = NOT_PLAYING;
	GameData::reinit();

	//XXX: to change !! (cf DataTheme)
	//_lords.clear();
	//_lords.init();
	// XXX: logEE( "lords not reinitialized" );

	_server->clear();
}

void Engine::startGame()
{
	if( _players.count() > _server->getNbSocket() ) {
		return;
	}

	uint i, j;

	_state = IN_GAME;
	_server->startGame();
	_calendar->reinit();
	_calendar->newDay();

	_server->sendSizeMap( _map->getHeight(), _map->getWidth() );

	for( i = 0; i < _players.count(); i++ ) {
		_players.at( i )->setMap( _map );
		_players.at( i )->setNum( i );
		_currentPlayer = _players.at( i );
		_currentPlayer->initMapVision();
		_server->setCurrentPlayer( _currentPlayer );
		for( j = 0; j < _currentPlayer->numLord(); j++ ) {
			QList<GenericCell> removed;
			QList<GenericCell> added = _currentPlayer->addLordVision( _currentPlayer->getLord( j ) );
			updateMapVision( removed, added );
			_server->newLord( _currentPlayer->getLord( j ) );

		}
		for( j = 0; j < _currentPlayer->numBase(); j++ ) {
			QList<GenericCell> removed;
			QList<GenericCell> added = _currentPlayer->addBaseVision( _currentPlayer->getBase( j ) );
			updateMapVision( removed, added );
			_server->ownBase( _currentPlayer->getBase( j ), _currentPlayer );
		}

		for( j = 0; j < _currentPlayer->numBuilding(); j++ ) {
			QList<GenericCell> removed;
			QList<GenericCell> added = _currentPlayer->addBuildingVision( _currentPlayer->getBuilding( j ) );
			updateMapVision( removed, added );
			_server->ownBuilding( _currentPlayer->getBuilding( j ), _currentPlayer );
		}

		for( j = 0; j < DataTheme.ressources.count(); j++ ) {
			_server->sendPlayerResource( j, _currentPlayer->getResource( j ) );
		}


	}
	_currentPlayer = _players.at( 0 );
	_server->setCurrentPlayer( _currentPlayer );

	// XXX: already done a few lines before ???
	_currentPlayer = _players.at( 0 );
	_server->setCurrentPlayer( _currentPlayer );
	_currentPlayer->newWeek();
	_currentPlayer->newTurn();
	_server->beginTurn();
}

void Engine::endGame()
{
	_state = NOT_PLAYING;
	_server->endGame();
}

void Engine::startFight()
{
	if( !_fight ) {
		_fight = new FightEngine( _server );
	}
	_state = IN_FIGHT;
	_fight->init( _players.at( 0 ), _players.at( 0 )->getLord( 0 ), _players.at( 1 ), _players.at( 1 )->getLord( 0 ) );
}

void Engine::startFight( int lordAttack, GenericLord *lordDefense )
{
	if( !_fight ) {
		_fight = new FightEngine( _server );
		QObject::connect( _fight, SIGNAL( sig_endFight( char ) ), SLOT( slot_endFight( char ) ) );
	}
	_fight->setDefendCell( lordDefense->getCell() );
	_state = IN_FIGHT;
	_fight->init( _currentPlayer, _currentPlayer->getLordById( lordAttack ), lordDefense->getOwner(), lordDefense );
}

void Engine::startFight( int lordAttack, GenericMapCreature * creature )
{
	if( !_fight ) {
		_fight = new FightEngine( _server );
		QObject::connect( _fight, SIGNAL( sig_endFight( char ) ), SLOT( slot_endFight( char ) ) );
	}
	_state = IN_FIGHT;
	_fight->setDefendCell( creature->getCell() );
	GenericFightUnit * units[MAX_UNIT];
	for( int i = 0; i < MAX_UNIT; i++ ) {
		units[i] = 0;
	}
	units[0] = new GenericFightUnit();
	units[0]->setCreature( creature->getCreature() );
	/// XXX: to change, of course
	units[0]->setNumber( 10 );

	_fight->init( _currentPlayer, _currentPlayer->getLordById( lordAttack ), units, (GameData *)this );

	/// XXX: destroy units...
	logDD( "start fight finished" );
}

void Engine::slot_endFight( char result )
{
	//XXX: Handle result (flee, and so on...)
	GenericLord * loser, * winner;

	if( ( result & FIGHTER_DEFENSE_WIN ) == FIGHTER_DEFENSE_WIN ) {
		loser = _fight->getAttackLord();
		winner = _fight->getDefendLord();
	} else {
		loser = _fight->getDefendLord();
		winner = _fight->getAttackLord();
	}
	/*if( !_isCreature || (loser!=_fight->getDefendLord())){
		_server->sendLordRemove( loser );
		loser->removeFromGame();
	}*/
	if( _isCreature ) {
		if( loser == _fight->getDefendLord() ) {
			GenericCell * cell = _fight->getDefendCell();
			_server->sendCreatureRemove( cell );
			GenericMapCreature * creature = cell->getCreature();
			cell->setCreature( 0 );
			delete( creature );
		} else {
			_server->sendLordRemove( loser );
			loser->removeFromGame();
		}
	} else {
		_server->sendLordRemove( loser );
		loser->removeFromGame();
	}
	//winner->

	_state = IN_GAME;
}

bool Engine::saveGame( QString filename )
{
    	QFile f( filename );

	if (! f.open(IO_WriteOnly) ) {
		logEE("Could not open file %s for writing\n", filename.latin1() );
		return false;
	}
/*
	QTextStream ts( &f );
	_map->save( &ts );
	ts << _players.count() << endl;
	for( uint i = 0; i < _players.count(); i++ ) {
		_players.at(i)->save( &ts );
	}
	f.close();
*/
	logEE( "Save not implemented !" );
	return true;
}


void Engine::nextPlayer()
{
	_counter=0;
	uint num = _players.find( _currentPlayer );
	num++;
	if( num == _players.count() ) {
		_currentPlayer = _players.at( 0 );
		_calendar->newDay();
	} else {
		_currentPlayer = _players.at( num );
	}

	if( ! _currentPlayer->isAlive() ) {
		nextPlayer();
	}

	if( ! _currentPlayer->hasLost() ) {
		_server->setCurrentPlayer( _currentPlayer );
		uint i;


		for( i = 0; i < _currentPlayer->numBuilding(); i++ ) {
			handleBuildingTurn( _currentPlayer, _currentPlayer->getBuilding( i ) );
		}
		for( i = 0; i < _currentPlayer->numBase(); i++ ) {
			handleBaseTurn( _currentPlayer, _currentPlayer->getBase( i ) );
		}
		for( i = 0; i < _currentPlayer->numLord(); i++ ) {
			handleCreatTurn( _currentPlayer, _currentPlayer->getLord( i ) );
		}
		if( _calendar->getDay() == 1 ) {
			_currentPlayer->newWeek();
		}
		_currentPlayer->newTurn();
		_server->beginTurn();
	} else {
		_server->playerLose( _currentPlayer );
		// XXX: add better test for end of game (quests, and so on)
		if( ( getAlivePlayersNumber() < 2 ) && ( getAlivePlayersNumber() < _players.count() ) ) {
			endGame();
		} else {
			nextPlayer();
		}
	}
}

void Engine::handleBuildingTurn( GenericPlayer * player, GenericBuilding * building )
{
	QList<Action> list = building->getActionList( Action::DATE );
	for( uint i = 0; i < list.count(); i++ ) {
		handleBuildingAction( list.at( i ), player );
	}
}

void Engine::handleBaseTurn( GenericPlayer * player, GenericBase * base )
{
	
	for( uint i = 0; i < base->getBuildingCount(); i++ ) {
		GenericInsideBuilding * inbuil = base->getBuilding(i);
		for( uint j = 0; j < MAX_RESS; j++ ) {
			int mant=player->getResource( j ) - inbuil->getMantCost(j);
			if(mant<0) mant=0;
			player->setResource( j,mant );
			_server->sendPlayerResource( j, player->getResource(j)  );
		}
	}
	for( uint j = 0; j < MAX_RESS; j++ ) {
		player->setResource( j, player->getResource( j ) + base->collectRessource(j));
		_server->sendPlayerResource( j, player->getResource(j)  );
	}
}

void Engine::handleCreatTurn( GenericPlayer * player, GenericLord * lord )
{
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = lord->getUnit(i);
		if( unit ) {
			for( uint j = 0; j < MAX_RESS; j++ ) {
				int mant = player->getResource( j ) - ( unit->getMantCost( j ) * unit->getNumber() );
				if( mant < 0 ) {
					mant = 0;
				}
				player->setResource( j, mant );
				_server->sendPlayerResource( j, player->getResource( j )  );
			}
		}
	}
}

void Engine::handleBuildingAction( Action * action, GenericPlayer * player )
{
	for( int i = 0; i < action->getElementaryNumber(); i++ ) {
		ElementaryAction * elementary = action->getElementaryAction( i );
		switch( elementary->getType() ) {
			case ElementaryAction::RESSOURCE:
				player->setResource( elementary->getArg(), player->getResource( elementary->getArg() ) + elementary->getCoeff() );
				_server->sendPlayerResource( elementary->getArg(), player->getResource( elementary->getArg() ) );
				break;
			case ElementaryAction::RANDRESSOURCE:
				player->setResource( elementary->getArg(), player->getResource( elementary->getArg() ) + elementary->getCoeff()*(int) ((2.0*rand())/(RAND_MAX+1.0)) );
				_server->sendPlayerResource( elementary->getArg(), player->getResource( elementary->getArg() ) );
				break;
			default:
				break;
		}
	}
}

bool Engine::loadGame( QString filename )
{
	for( uint i = 0; i < _players.count(); i++ )
	{
		_currentPlayer = _players.at( i );
		_currentPlayer->cleanData();
	}

	reinit();

	ScenarioParser handler( (GameData*)this );
	QFile file( filename );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", filename.latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	if( _players.count() != (uint)_nbPlayer ) {
		QMessageBox::critical( 0, "Error with scenario", "Wrong number of connected players" );
		_state = NOT_PLAYING;
		GameData::reinit();
		for( uint i = 0; i < _players.count(); i++ ) {
			_currentPlayer = _players.at( i );
			logDD( "replayers %d ",  _players.count());
			logDD( "relords %d ",_currentPlayer->numLord()  );
		}
		return false;
	} else {
		return true;
	}
}

uint Engine::getAlivePlayersNumber()
{
	uint ret = 0;

	for( uint i = 0; i < _players.count(); i++ ) {
		if( _players.at( i )->isAlive() ) {
			ret++;
		}
	}

	return ret;
}

void Engine::updateMapVision( QList<GenericCell> & removed, QList<GenericCell> & added )
{
	for( uint i = 0; i < added.count(); i++ ) {
		GenericCell * cell = added.at( i );
		if( removed.findRef( cell ) == -1 ) {
			_server->sendCell( cell );
			if( cell->getCreature() ) {
				_server->newCreature( cell->getCreature() );
			}
			if( cell->getBase() ) {
				_server->newBase( cell->getBase() );
			}
			if( cell->getBuilding() ) {
				_server->newBuilding( cell->getBuilding() );
			}
			if( cell->getArtefact() ) {
				_server->newArtefactMap( cell->getArtefact() );
			}/// XXX: other cell attr to send...
		}
	}
}




