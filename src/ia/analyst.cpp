/****************************************************************
**
** Attal : Lords of Doom
**
** analyst.cpp
** analyse the game and play
**
** Version : $Id: analyst.cpp,v 1.44 2003/04/07 08:50:08 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <stdlib.h>
// include files for QT
// application specific include files
#include "common/attalSocket.h"
#include "common/calendar.h"
#include "common/dataTheme.h"
#include "common/genericBase.h"
#include "common/genericBuilding.h"
#include "common/genericLord.h"
#include "common/genericMap.h"
#include "common/genericEvent.h"
#include "common/genericMapCreature.h"
#include "common/genericPlayer.h"
#include "common/pathFinder.h"

#include "server/fightAnalyst.h"

#include "ia/aiInterface.h"


extern DataTheme DataTheme;

//
// ----- Analyst -----
//

Analyst::Analyst()
: GameData()
{
	_fight = 0;
	_socket = 0;
	_calendar = new Calendar();
	_calendar->reinit();

	// XXX: put this code in a 'reinit' method ?
	_map = new GenericMap();
	_player = new GenericPlayer( _map );

	/*_lords = new GenericLord * [ DataTheme.lords.count() ];
	for( uint i = 0; i < DataTheme.lords.count(); i++ ) {
		_lords[i] = new GenericLord();
		_lords[i]->setId( i );
	}*/

}

/*
GenericLord * Analyst::getLord( uint num )
{
	GenericLord * lord = 0;

	if( num < DataTheme.lords.count() ) {
		lord = _lords[num];
	}

	return lord;
}
*/
void Analyst::socketMsg()
{
	ialogV( "SO_MSG" );
}


/*!

*/

void Analyst::socketConnect()
{
	ialogV( "SO_CONNECT" );
	switch( _socket->getCla2() ) {
	case C_CONN_OK:
		ialogV( "Receive: Connect Ok" );
		_socket->sendConnectionName( "IA" );
		break;
	case C_CONN_ID:{
		_player->setNum(_socket->readChar());
		ialogV( "Receive: Connect Id = %d", _player->getNum() );
		}
		break;
	case C_CONN_NAME:
		ialogV( "Receive: Connect Name" );
		break;
	case C_CONN_PLAYER:
		ialogV( "Receive: Connect Player" );
		break;
	}
}

void Analyst::socketMvt()
{
	ialogV( "SO_MVT" );
	uchar lord = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();

	GenericLord * theLord = getLord( lord );

	int cost = theLord->computeCostMvt( _map->at( row, col ) );
	if( cost >= 0 ) {
		theLord->decreaseBaseCharac( MOVE, cost );
	}

	theLord->moveTo( _map->at( row, col ) );

	if( _map->computeMinimalNextCost( theLord ) > theLord->getCharac( MOVE ) ) {
		theLord->setBaseCharac( MOVE, 0 );
	}
	

	/// TODO: manage enter inside buildings and bases
	if( _map->at( row, col )->getBuilding() != 0 ) {
			if( _map->at(row, col)->getBuilding()->getOwner() != theLord->getOwner()) {
			 _map->at(row, col)->getBuilding()->setOwner(theLord->getOwner()); 
			}	
		if( theLord->getOwner() == _player ) {
			/// TODO: enter( _player->getSelectedLord(), _map->at( row, col )->getBuilding() );
		}
	} else if( _map->at( row, col )->getBase() != 0 ) {
		if( theLord->getOwner() == _player ) {
			/// TODO: enter( _player->getSelectedLord(), _map->at( row, col )->getBase() );
			enterBase(theLord, _map->at( row, col )->getBase());
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
		ialogV( "INIT" );
		if( ! _fight ) {
			_fight = new FightAnalyst( this );
			_fight->setSocket( _socket );
		}
		_fight->handleFightSocket();
		break;
	case C_FIGHT_END:
		ialogV( "END" );
		_socket->sendLordTurn( 0 );
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


/*!

*/

void Analyst::socketQR()
{
	ialogV( "SO_QR" );
}


/*!

*/

void Analyst::socketExch()
{
	ialogV( "SO_EXCH" );

	switch( _socket->getCla2() ) {
	case C_EXCH_START:
		exchangeStart();
		break;
	case C_EXCH_UNIT:
	  	exchangeUnits();
		break;
	case C_EXCH_BASEUNITCL:
	  	exchangeBaseUnits();
		break;
	default:
		break;
	}
}

void Analyst::exchangeStart()
{
	uchar idLord1 = _socket->readChar();
	uchar idLord2 = _socket->readChar();
	GenericLord * lord1 = _lords.at( idLord1 );
	GenericLord * lord2 = _lords.at( idLord2 );
	/// XXX: not finished ???
}
void Analyst::exchangeUnits()
{
	uchar idLord1 = _socket->readChar();
	uchar idUnit1 = _socket->readChar();
	uchar idLord2 = _socket->readChar();
	uchar idUnit2 = _socket->readChar();
	GenericLord * lord1 = 0;
	GenericLord * lord2 = 0;
	if( idLord1 ) {
		lord1 = _lords.at( idLord1 );
	}
	if( idLord2 ) {
		lord2 = _lords.at( idLord2 );
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
		}
	}
}

void Analyst::exchangeBaseUnits()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar idUnit1 = _socket->readChar();
	uchar idLord = _socket->readChar();
	uchar idUnit2 = _socket->readChar();
	GenericBase * base = 0;
	GenericLord * lord = 0;
	GenericFightUnit * uni1 = 0;
	GenericFightUnit * uni2 = 0;

	base = _map->at( row, col )->getBase();
	if( idLord && ( idLord < 255 ) ) {
		lord = _lords.at( idLord );
	}

	if( base ) {
		if( idUnit1 <= MAX_UNIT ){
			uni1 = base->getUnit( idUnit1 );
		}
		if( lord ) {
			if( idUnit2 <= MAX_UNIT ){
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
			if( idUnit2 <= MAX_UNIT ) {
				uni2 = base->getUnit( idUnit2 );
			}
			if( uni1 && uni2 ) {
				if( uni1->getCreature() != uni2->getCreature() ) {
					base->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni1->addNumber( uni2->getNumber() );
					lord->setUnit( idUnit2, 0 );
					delete uni2;
				}
			} else {
				base->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		}
	}
}




/*!

*/

void Analyst::socketModif()
{
	ialogV( "SO_MODIF" );
	switch( _socket->getCla2() ) {
	case C_MOD_MAP:{
		int h = _socket->readInt();
		int w = _socket->readInt();
		_map->newUnknownMap( h, w );
		}
		break;
	case C_MOD_CELL:{
		int a1 = _socket->readInt();
		int a2 = _socket->readInt();
		int a3 = _socket->readInt();
		int a4 = _socket->readInt();
		int a5 = _socket->readInt();
		int a6 = _socket->readInt();
		_map->changeCell( a1, a2, a3, a4, a5, a6 );
		}
		break;
	case C_MOD_LORD:
		socketModifLord();
		break;
	case C_MOD_PLAYER:
		socketModifPlayer();
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
	}
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
	case C_LORD_MOVE:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_lords.at( lord )->setBaseCharac( MOVE, nb );
			break;
		}
	case C_LORD_MAXMOVE:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_lords.at( lord )->setBaseCharac( MAXMOVE, nb );
			break;
		}
	case C_LORD_SP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_lords.at( lord )->setBaseCharac( TECHNICPOINT, nb );
			break;
		}
	case C_LORD_MAXSP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_lords.at( lord )->setBaseCharac( MAXTECHNICPOINT, nb );
			break;
		}
	case C_LORD_MORALE:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( MORALE, nb );
			break;
		}
	case C_LORD_LUCK:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( LUCK, nb );
			break;
		}
	case C_LORD_EXP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_lords.at( lord )->setBaseCharac( EXPERIENCE, nb );
			break;
		}
	case C_LORD_ATT:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( ATTACK, nb );
			break;
		}
	case C_LORD_DEF:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( DEFENSE, nb );
			break;
		}
	case C_LORD_POW:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( POWER, nb );
			break;
		}
	case C_LORD_KNOW:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_lords.at( lord )->setBaseCharac( KNOWLEDGE, nb );
		}
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

	/// XXX: memory leak no new -> use _lords (cf game.cpp too !!)
	GenericLord * lord = _lords.at( id );

	//new GenericLord();
	//lord->setActive( true );
	//lord->setId( id );
	/// XXX: change
	lord->setOwner( 0 );
	lord->moveTo( _map->at( row, col ) );
	//_lords.at( id ) = lord;

}

void Analyst::socketModifLordNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();

	GenericLord * lord = _lords.at( id );
	lord->moveTo( _map->at( row, col ) );
	lord->setOwner( _player );

	_player->addLord( lord );
}

void Analyst::socketModifLordUnit()
{
	uchar id = _socket->readChar();
	uchar num = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();

	GenericFightUnit * unit = new GenericFightUnit();
	unit->setCreature( race, level );
	unit->setNumber( nb );
	unit->setMove( _socket->readChar() );
	unit->setHealth( _socket->readInt() );

	_lords.at( id )->setUnit( num, unit );
	/// XXX: remove/delete old unit ??
}

void Analyst::socketModifLordRemove()
{
	int idLord = _socket->readChar();
	GenericLord * lord = _lords.at( idLord );
	GenericPlayer * player = lord->getOwner();
	if( player ) {
		player->removeLord( lord );
		if( player == _player ) {
			_socket->sendLordTurn( 1 );
		} else {
			_socket->sendLordTurn( 0 );
		}
	}

	GenericCell * cell = lord->getCell();
	if( cell ) {
		cell->setLord( 0 );
	}
	lord->reinit();

	//XXX: Remove lord from map
	delete lord;
	//if( lord ) lord->getOwner()->removeLord( lord );
}

void Analyst::socketModifPlayer()
{
	switch( _socket->getCla3() ) {
	case C_PLAY_RESS: {
		uchar ress = _socket->readChar();
		_player->setResource( ress, _socket->readInt() );
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
	case C_BASE_BUILDING:
		socketModifBaseBuilding();
		break;
	case C_BASE_UNIT:
		socketModifBaseUnit();
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

	GenericBase * base = new GenericBase();
	base->setRace( race );
	base->setPosition( _map->at( row, col ) );
	_map->computeStoppable( base );
	base->setId( id );/// XXX: to inv ??
	base->isUnitBought( false );

	for( int i = 0; i < nb; i++ ) {
		base->addForbiddenBuilding( _socket->readChar() );
	}
}

void Analyst::socketModifBaseOwner()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	GenericBase * base = (GenericBase*)_map->at( row, col )->getBase();
	if( _socket->readChar() == _player->getNum() ) {
		_player->addBase( base );
		base->setOwner( _player );
	} else {
		/// XXX: improve management of base of other player
		if( base->getOwner() == _player ) {
			base->setOwner( 0 );
			_player->removeBase( (GenericBase*)base );
		}
	}
}

void Analyst::socketModifBaseBuilding()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar level = _socket->readChar();
	bool create = (bool)_socket->readChar();

	if( _map->at( row, col )->getBase() ) {
		GenericInsideBuilding * building = new GenericInsideBuilding();
		building->setRace( _map->at( row, col )->getBase()->getRace() );
		building->setLevel( level );
		if( create ) {
			_map->at( row, col )->getBase()->addBuilding( building );
		} else {
			_map->at( row, col )->getBase()->removeBuilding( building );
		}
	} else {
		logEE( "Base not found" );
	}
}

void Analyst::socketModifBaseUnit()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	if( _map->at( row, col )->getBase() ) {
		GenericBase * base = (GenericBase *)_map->at( row, col )->getBase();
		uchar race = _socket->readChar();
		uchar level = _socket->readChar();
		int number = _socket->readInt();
		Creature * creature = DataTheme.creatures.at( race, level );
		base->addGarrison( creature, number );
	}
}

void Analyst::socketModifBuilding()
{
	switch( _socket->getCla3() ) {
		case C_BUILD_NEW: {
			GenericBuilding * building = new GenericBuilding();
			int row = _socket->readInt();
			int col = _socket->readInt();
			building->setPosition( _map->at( row, col ) );
			uchar type = _socket->readChar();
			building->setType( type );
		}
		break;
		case C_BUILD_OWNER: {
			int row = _socket->readInt();
			int col = _socket->readInt();
			if( _socket->readChar() == _player->getNum() ) {
				_player->addBuilding( _map->at( row, col )->getBuilding() );
			}
		}
	}
}

void Analyst::socketModifArtefact()
{
	switch( _socket->getCla3() ) {
		case C_ART_NEWMAP: {
				GenericArtefact * artef = new GenericArtefact();
				artef->setId( _socket->readInt() );
				artef->setType( _socket->readChar() );
				int row = _socket->readInt();
				int col = _socket->readInt();
				artef->setCell( _map->at( row, col ) );
				_map->at( row, col )->setArtefact( artef );
				_artefacts.append( artef );
			}
			break;
		case C_ART_NEWLORD: {
				GenericArtefact * artef = new GenericArtefact();
				artef->setId( _socket->readInt() );
				artef->setType( _socket->readChar() );
				artef->setPosition( 0 );
				char lord = _socket->readChar();
				artef->setLord( _lords.at( lord ) );
				_lords.at( lord )->addArtefact( artef );
			}
			break;
		case C_ART_DELMAP: {
				int id = _socket->readInt();
				if( getArtefactById( id )->isOnMap() ) {
					getArtefactById( id )->getCell()->setArtefact( 0 );
				}
				getArtefactById( id )->setLord( 0 );
			}
			break;
		case C_ART_DELLORD: {
				int id = _socket->readInt();
				char lord = _socket->readChar();
				_lords.at( lord )->removeArtefact( getArtefactById( id ) );
			}
			break;
		case C_ART_ADDLORD: {
				int id = _socket->readInt();
				char lord = _socket->readChar();
				_lords.at( lord )->addArtefact( getArtefactById( id ) );
				getArtefactById( id )->setLord( _lords.at( lord ) );
			}
			break;
	}
}

GenericArtefact * Analyst::getArtefactById( int id )
{
	GenericArtefact * ret = 0;
	for( uint i = 0; i < _artefacts.count(); i++ ) {
		if( _artefacts.at( i )->getId() == id ) {
			ret = _artefacts.at( i );
		}
	}
	return ret;
}

void Analyst::socketModifEvent()
{
	switch( _socket->getCla3() ) {
	case C_EVENT_NEW: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar type = _socket->readChar();
		GenericEvent * event = new GenericEvent();
		event->setCell( _map->at( row, col ) );
		event->setType( (GenericEvent::EventType)type );
		_map->at( row, col )->setEvent( (GenericEvent*)event );
		}
		break;
	case C_EVENT_DEL: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		GenericEvent * event = _map->at( row, col )->getEvent();
		if( event ) {
			delete event;
			_map->at( row, col )->setEvent( 0 );
		}
		}
		break;
	}
}

void Analyst::socketModifCreature()
{
	switch( _socket->getCla3() ) {
	case C_CRE_NEW: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar race = _socket->readChar();
		uchar level = _socket->readChar();
		int nb = _socket->readInt();
		GenericMapCreature * creature = new GenericMapCreature();
		creature->setCreature( race, level );
		creature->setCategoryNumber( nb );
		creature->setCell( _map->at( row, col ) );
		_map->at( row, col )->setCreature( creature );
		}
		break;
	case C_CRE_DEL: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		GenericMapCreature * crea = _map->at( row, col )->getCreature();
		if( crea ) {
			delete crea;
			_map->at( row, col )->setCreature( 0 );
		}
		}
		break;
	}
}

void Analyst::socketTurn()
{
	ialogV( "SO_TURN" );

	switch( _socket->getCla2() ) {
	case C_TURN_BEG:
		socketTurnBegin();
		break;
	case C_TURN_PLAY:
		ialogV( "Player: %d", _socket->readChar() );
		break;
	case C_TURN_END:
		ialogC( "Should not happen (SO_TURN/C_TURN_END)" );
		break;
	case C_TURN_LORD:
		ialogC( "Should not happen (SO_TURN/C_TURN_LORD)" );
		break;
	case C_TURN_PLORD:{
		int nlord = _socket->readInt();
		_numlords = _player->numLord();
		if( nlord >= _numlords ){
			_socket->sendTurnEnd();
		} else {
			playLordTurn(nlord);
		}
		} break;
	}
}

void Analyst::socketTurnBegin()
{
	ialogV( "Start turn" );
	_calendar->newDay();
	if( _calendar->getDay() == 1 ) {
		_player->newWeek();
	}
	_player->newTurn();
	_fightCpt = 0;

	if( _player->numBase() > 0 ){
		for( int i = 0; i < (int)_player->numBase(); i++ ) {
			GenericBase * base = _player->getBase( i );
			manageBase( base );
		}
	}
	_socket->sendLordTurn( 0 );
}

void Analyst::playLordTurn( int nlord )
{
	_fightCpt = 0;
	analyzeLord( _player->getLord( nlord ) );
}

void Analyst::analyzeLord( GenericLord * lord )
{
	int cost;
	_movePoints = lord->getCharac( MOVE );
	GenericCell * cell = lord->getCell();
	int startRow = cell->getRow();
	int startCol = cell->getCol();
	int i, j;
	int myPower = getPower( lord );
	int opPower;
	_nbTry = 0;
	int rowFlee = 1;
	int colFlee = 1;
	_startApprRow = 0;
	_startApprCol = 0;

	_destinationCell  = _map->at( startRow + 1, startCol+1 );
	_prior=7;	
	_mincost=1000;
	_map->getPath()->reinit( _map );
	_map->getPath()->computePath( cell );

	if(_movePoints>0){
		if(_fightCpt==0){
			for(  i = 0; i < _map->getHeight(); i++ ) {
			for(  j = 0; j < _map->getWidth(); j++ ) {
				if( _map->at(i, j)->getLord() && (PR_ENEMY <= _prior) && _map->at(i, j)->getCoeff()!=-1)	{
					if( _map->at(i, j)->getLord()->getOwner() != _player ) {
						opPower=getRandPower(_map->at(i, j)->getLord());
						ialogV("opPower %d ,myPower %d", opPower,myPower);
						ialogV("row, col %d, %d", i,j);
						cost=_map->getPath()->getDist(_map->at(i, j));
						ialogV( "Try enemy");
						logDD( "Try enemy");
						_nbTry=1;
						if(_prior>PR_ENEMY){
							_mincost=1000;
						}
					if( cost<_mincost && cost<=_movePoints && cost!=-1 && myPower>opPower) {
						ialogV( "Enemy");
			 			_fightCpt=1;

						_destinationCell = _map->at(i, j);
						_mincost=cost;
						_prior=PR_ENEMY;
					} else if(cost<(_movePoints+5) && cost!=-1) {
							if((startRow-i)>0){ 
								_startApprRow=i;
								if(myPower<opPower) {
								_startApprRow=startRow;
								}
							} else {
								_startApprRow=startRow;
								if(myPower<opPower) {
									rowFlee=-1;
								}
							}
						  if((startCol-j)>0){
							 	_startApprCol=j;
								if(myPower<opPower) {
								_startApprCol=startCol;
								}
							} else {
								_startApprCol=startCol;
								if(myPower<opPower) {
									colFlee=-1;
								}
							}
								if(myPower<opPower) {
									_prior=PR_ENEMY;
								}
					} 	
				}	
			} else if( _map->at(i, j)->getBase() && (PR_BASE	<=	_prior)&& _map->at(i, j)->getCoeff()!=-1)	{
				if( _map->at(i, j)->getBase()->getOwner() != _player || _map->at(i, j)->getBase()->isUnitBought()) {
					cost=_map->getPath()->getDist(_map->at(i, j));
					ialogV( "Try Base");
					logDD( "Try Base");

					_nbTry=1;
					if(_prior>PR_BASE){

						_mincost=1000;
					}
					if( cost<_mincost && cost<=_movePoints && cost!=-1) {
						ialogV( "Base");
						ialogV("row, col %d, %d", i,j);

						_destinationCell = _map->at(i, j);
						_mincost=cost;
						_prior=PR_BASE;
						_fightCpt=1;
					} else if(cost<(_movePoints+4) && cost!=-1) {
						if(startRow>i){
							_startApprRow=i;
						} else {
							_startApprRow=startRow;
						}
						if(startCol>j){
							_startApprCol=j;
						} else {
							_startApprCol=startCol;
						}
					}
				}
			}	else if( _map->at(i, j)->getBuilding() && (PR_BUILD <= _prior)&& _map->at(i, j)->getCoeff()!=-1)	{
				if( _map->at(i, j)->getBuilding()->getOwner() != _player ) {
					cost=_map->getPath()->getDist(_map->at(i, j));
					ialogV( "Try Build");
					logDD( "Try Build");

					_nbTry=1;
					if(_prior>PR_BUILD){
						_mincost=1000;
					}
					if( cost<_mincost && cost<=_movePoints && cost!=-1) {
						ialogV( "Build");
						ialogV("row, col %d, %d", i,j);

						_destinationCell = _map->at(i, j);
						_mincost=cost;
						_prior=PR_BUILD;
						_fightCpt=1;
					} else if(cost<(_movePoints+5) && cost!=-1) {
						if(startRow>i){ 
							_startApprRow=i;
						} else {
							_startApprRow=startRow;
						}
						if(startCol>j){
							_startApprCol=j;
						} else {
							_startApprCol=startCol;
						}
					}
				}
			}	else if( _map->at(i, j)->getArtefact() && (PR_ART < _prior)&& _map->at(i, j)->getCoeff()!=-1)	{
				cost=_map->getPath()->getDist(_map->at(i, j));
				ialogV( "Try Artefact");
				_nbTry=1;

				if(_prior>PR_ART){
					_mincost=1000;
				}
				if( cost<_mincost && cost<=_movePoints && cost!=-1) {
					ialogV( "Artefact");
					ialogV("row, col %d, %d", i,j);
					
					_destinationCell = _map->at(i, j);
					_mincost=cost;
					_prior=PR_ART;
					_fightCpt=1;
				} else if(cost<(_movePoints+5) && cost!=-1) {
					if(startRow>i){ 
						_startApprRow=i;
					} else {
						_startApprRow=startRow;
					}
					if(startCol>j){
						_startApprCol=j;
					} else {
						_startApprCol=startCol;
					}
				}
			
			}	else if( _map->at(i, j)->getCreature() && (PR_CREAT < _prior)&& _map->at(i, j)->getCoeff()!=-1)	{
				cost=_map->getPath()->getDist(_map->at(i, j));
				ialogV( "Try Creature");
				_nbTry = 1;

				if(_prior>PR_CREAT){
					_mincost=1000;
				}
				if( cost<_mincost && cost<=_movePoints && cost!=-1) {
					ialogV( "Creature");
					ialogV("row, col %d, %d", i,j);
					
					_destinationCell = _map->at(i, j);
					_mincost=cost;
					_prior=PR_CREAT;
					_fightCpt=1;
				} else if(cost<(_movePoints+5) && cost!=-1) {
					if(startRow>i){ 
						_startApprRow=i;
					} else {
						_startApprRow=startRow;
					}
					if(startCol>j){
						_startApprCol=j;
					} else {
						_startApprCol=startCol;
					}
				}
			}	else if( _map->at(i, j)->getEvent() && (PR_EVENT < _prior)&& _map->at(i, j)->getCoeff()!=-1)	{
				cost=_map->getPath()->getDist(_map->at(i, j));
				ialogV( "Try Event");
				_nbTry = 1;

				if(_prior>PR_EVENT){
					_mincost=1000;
				}
				if( cost<_mincost && cost<=_movePoints && cost!=-1) {
					ialogV( "Event");
					ialogV("row, col %d, %d", i,j);
					
					_destinationCell = _map->at(i, j);
					_mincost=cost;
					_prior=PR_EVENT;
					_fightCpt=1;
				} else if(cost<(_movePoints+5) && cost!=-1) {
					if(startRow>i){ 
						_startApprRow=i;
					} else {
						_startApprRow=startRow;
					}
					if(startCol>j){
						_startApprCol=j;
					} else {
						_startApprCol=startCol;
					}
				}
			}		
			}//for i
			}//for j
		}//_fightcpt
	
	_nbTry=_fightCpt;

	int sameTry=0;
	int moreTry=0;


	if(  _fightCpt == 0 )  {
		long he = (long)_map->getHeight();
		long wi = (long)_map->getWidth();
	do{
			do{
				if(_startApprRow){
					i=_startApprRow + (rowFlee)*(int)(_movePoints*(rand()/(RAND_MAX+1.0)));
					moreTry++;
					if(moreTry>9){
						i=_startApprRow;
					}	
				} else {
					i=(int) (he*(rand()/(RAND_MAX+1.0)));
				}
			}while(i>=(int) he || i<0);
					moreTry=0;
			do{
				if(_startApprCol){
					j=_startApprCol+(colFlee)*(int)(_movePoints*(rand()/(RAND_MAX+1.0)));
					moreTry++;
					if(moreTry>9){
						j=_startApprCol;
					}	
				} else {
					j=(int) (he*(rand()/(RAND_MAX+1.0)));
				}
			}while(j>=(int) wi || j<0 );

				if( i == startRow && j == startCol ){
					sameTry++;
				} else {
					if(_map->getPath()->getDist(_map->at(i, j))<=_movePoints && (_map->getPath()->isPath(_map->at(i, j))==true))  {
						sameTry=10;
					}
				}
		} while( (_map->getPath()->getDist(_map->at(i, j))>_movePoints || _map->getPath()->isPath(_map->at(i, j))==false) && sameTry<10 );
		_destinationCell = _map->at( i, j );
	}

	/* if no possibility to move, destination cell = starting cell */
 	if(_fightCpt==0){
		if(_map->getPath()->getDist(_destinationCell)>_movePoints || (_map->getPath()->isPath(_destinationCell)==false))  {
			_destinationCell  = cell;
		}
 	}
		
	if(_map->getPath()->isPath(_destinationCell)==true && _destinationCell  != cell){
		QStack<GenericCell> * path = _map->getPath()->giveCells( _destinationCell );
		while( ! path->isEmpty() ) {
			GenericCell * movCell = path->pop();
			_socket->sendMvt(lord->getId(), movCell->getRow(), movCell->getCol() );
		}
	delete path;
	}


//	followPath( cell, _destinationCell, lord, _movePoints, 1 );
	}
	_socket->sendLordTurn((1+_fightCpt));
}

/*!

*/

void Analyst::socketGame()
{
	ialogV( "SO_GAME" );
	if( _socket->getCla2() == C_GAME_END ) {
		_player->cleanData();
		_map->cleanData();
		_calendar->reinit();
	} else if( _socket->getCla2() == C_GAME_LOST ) {
		int nb = _socket->readChar();
		ialogV("player %d has lost",nb);
		_player->cleanData();
		_map->cleanData();
	}
}

int Analyst::getRandPower( GenericLord * lord )
{
	int power = 0;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * lordUnit = lord->getUnit( i );
		if( lordUnit ) {
			uint number = DataTheme.getRandomInCategory( (uchar)DataTheme.computeCategory( lordUnit->getNumber() ) );
			power += lordUnit->getCreature()->getAttack() * number;
		}
	}
	return power;
}

int Analyst::getPower( GenericLord * lord )
{
	int power = 0;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * lordUnit = lord->getUnit( i );
		if( lordUnit ) {
			power += lordUnit->getCreature()->getAttack() * lordUnit->getNumber();
		}
	}
	return power;
}


void Analyst::manageBase(GenericBase * base)
{
	// scan all buildings looking for creatures
	int count = base->getBuildingCount();
	ialogV("TRY BUY");
	for( int i = 0; i < count; i++ ) {
		GenericInsideBuilding * building=base->getBuilding( i );
		InsideAction * action = DataTheme.bases.at( base->getRace() )->getBuildingModel( building->getLevel() )->getAction();
		if(action){		
			int race = action->getParam( 0 );
			int level = action->getParam( 1 );
			Creature * creature = DataTheme.creatures.at( race, level);
			int max =_player->computeBuyCreatureMax( creature );
			for(int  j = base->getCreatureProduction(creature);j>0; j-- ) {
				if(_player->canBuy( creature, j ) ){
					ialogN("BUY  num %d",j);
					_player->buy(creature,j);
					_socket->sendBaseUnit( base, creature, j );
					base->buyCreature( creature, j );
					base->isUnitBought(true); 
					break;
				}
			}
		}
	}

	GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
	int modelCount = baseModel->getBuildingCount();

	for( int i = 0; i < modelCount; i++ ) {
		GenericInsideBuilding * building= base->getBuilding( i );
		if( _player && _socket &&  !building) {
			if( _player->canBuy( DataTheme.bases.at( base->getRace() )->getBuildingModel( i ) ) ) {
				_socket->requestBuilding( base, i );
				ialogN("Buil");
				break;
			} else {
				logEE("Can't buy, Not enough ressources to buy this building" );
			}
		} else {
			logEE( "Player or socket not initialized for InsideActionAllBuildings" );
		}
	}
}



void Analyst::enterBase(GenericLord * lord,GenericBase * base )
{

	base->enter( lord );
	if(lord->countUnits()<MAX_UNIT){
		for(int i=0;i<MAX_UNIT;i++){
			for(int j=0;j<MAX_UNIT;j++){
				if(!lord->getUnit(j) && base->getGarrisonUnit(i)){
					lord->setUnit(j,base->getGarrisonUnit(i));
					base->setGarrisonUnit(i,0);
					_socket->sendExchangeBaseUnit( base, i, lord, j );
					//ialogN("Exch ,pos i %d ,pos j %d",i,j);
				}
				if(lord->getUnit(j) && base->getGarrisonUnit(i)){
					if(lord->getUnit(j)->getCreature()==base->getGarrisonUnit(i)->getCreature()){
						lord->getUnit(j)->addNumber(base->getGarrisonUnit(i)->getNumber());
						base->setGarrisonUnit(i,0);
						_socket->sendExchangeBaseUnit( base, i, lord, j );
					}
				}
			}
		base->isUnitBought(false);
		}
	}
	base->out( lord );
}


int Analyst::followPath(GenericCell *icell,GenericCell *dcell,GenericLord *lord,int maxm,int flags)
{
	
	int k;
	/* start row*/
	int startRow=icell->getRow();
	/* start col*/
	int startCol=icell->getCol();
	/* dest row*/
	int	destRow=dcell->getRow();
	/* dest col*/
	int destCol=dcell->getCol();

	int add;
	/*coeff*/
	int coeff=0;
	/*cost direct, cost diag*/
	int cost;
	/*current row*/
	int m=startRow;
	/*current col*/
	int n=startCol;
	/*row*/
	int i;
	/*col*/
	int j;
	/* temp row*/
	int tm;
	/*temp col */
	int tn;
	/*temp cost */
	int tempCost;
	int mincost;
	int cont;
	
	/* memory move data*/
	int counter=0;
	
	struct memory {
	int lastrw;
	int lastcl;
	int ladd;
	} lastMove[maxm];
	
	lastMove[0].lastrw=startRow;
	lastMove[0].lastcl=startCol;
	lastMove[0].ladd=1000;
	
	if(  dcell->getCoeff() == -1) {
		return NOREACH;
	}
	
	ialogV("dist %d",_map->getPath()->getDist(dcell));

	do {
		tempCost=-1;
		mincost=1000;
		
		for(i=m-1;i<m+2;i++) {
			for(j=n-1;j<n+2;j++) {
				if(i<0 || j<0 || i>_map->getWidth() || j>_map->getHeight() || (i==m && j==n) ){
					continue;
				}
				cont=0;
				for(k=0;k<counter;k++) {
					if(lastMove[k].lastrw==i && lastMove[k].lastcl==j && (lastMove[k].ladd< (abs(destRow-i)+abs(destCol-j))))
							cont=1;
				}
				if(cont==1)	continue;			
				coeff=_map->at(i, j)->getCoeff();
				
				if(_map->at(i,j)->isStoppable()==false)	{
					coeff=-1;
					logDD("Stoppable row %d,col %d",i,j);
				}
				//if(_map->at(i,j)->isFree()  && i!=destRow && j!=destCol) {
				//	coeff=-1;
				//	}
				
				if(coeff!=-1) {
					if(i!=m && j!=n) cost=COST_DIAG;	
					else cost=COST_DIRECT;
					if(i==destRow && j==destCol && _nbTry==1) 
					{
						coeff=0;
						cost=0;
					}
					cost=cost+coeff;
					add=(abs(destRow-i)+abs(destCol-j))+cost;
					if( add<mincost ){
						tm=i;
						tn=j;
						mincost=add;
						tempCost=cost;
					}
				}
			}
		}
			
		if(tempCost!=-1){
		m=tm;
		n=tn;
		maxm =maxm-tempCost;
		}
		if(maxm>=0) {
			logDD( "Mov startRow %d startCol %d tm ,%d tn, %d destRow  %d destCol %d",startRow,startCol,tm,tn,destRow,destCol);
			logDD( "maxm %d",maxm);
			if(flags){
				_socket->sendMvt(lord->getId(), m, n );
			}
		}
		if(mincost==1000 || maxm<0) {
			if(!flags) return NOREACH;
			if(_fightCpt && (m!=destRow || n!=destCol)) {
				_fightCpt=0;
			}
	 	 lastMove[counter].lastrw=m;
		 lastMove[counter].lastcl=n;
		 lastMove[counter].ladd=(abs(destRow-m)+abs(destCol-n));
			n=destCol;
			m=destRow;
			counter++;
		}
	} while( n != destCol  || m!=destRow);

	if(maxm>=0 && tempCost!=-1) {
		return REACH;
	}
	return NOREACH;
}


