/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: fightEngine.cpp,v 1.23 2003/02/03 22:32:51 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/01/2001
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

#include "fightEngine.h"

// generic include files
#include <stdlib.h>
// include files for QT
// application specific include files
#include "common/dataTheme.h"
#include "common/genericCell.h"
#include "common/genericPlayer.h"
#include "common/genericLord.h"
#include "common/log.h"

#include "server/gameData.h"

extern DataTheme DataTheme;

FightEngine::FightEngine( AttalServer * server )
{
	_server = server;
	_currentUnit = 0;

	_attackPlayer = 0;
	_attackLord = 0;
	_defendPlayer = 0;
	_defendLord = 0;
	_defendCell = 0;
	_analyst = 0;
	_fake = 0;
	//_troops = 0;
	_map = 0;
	_creatureLord = new GenericLord();
	_creatureLord->setId( 0 );
}

void FightEngine::print()
{
	logDD("att play %d", _attackPlayer );
	logDD("att lord %d", _attackLord );
	logDD("def play %d", _defendPlayer );
	logDD("def lord %d", _defendLord );
}

void FightEngine::init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericPlayer * defendPlayer, GenericLord * defendLord )
{
	_attackPlayer = attackPlayer;
	_attackLord = attackLord;
	_defendPlayer = defendPlayer;
	_defendLord = defendLord;
	_currentUnit = 0;

	_fightCreature = false;
	_server->startFight( _attackLord, _defendPlayer, _defendLord );

	if( _map ) {
		delete _map;
	}

	_map = new GenericFightMap();
	_map->newFightMap( 9, 15 );

	setupUnits();

	newTurn();
}

void FightEngine::init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericFightUnit * defendUnit[MAX_UNIT], GameData * data )
{
	int i;
	_attackPlayer = attackPlayer;
	_attackLord = attackLord;
	_defendPlayer = 0;
	_defendLord = _creatureLord;
	_fightCreature = true;

	if( _analyst ) {
		delete _analyst;
	}
	if( _fake ) {
		delete _fake;
	}
	_fake = new FakeSocket;
	_analyst = new FightAnalyst( *data, this );
	_analyst->setSocket( _fake );
	_server->startFight( _attackLord, defendUnit );

	for( i = 0; i < MAX_UNIT; i++ ) {
		_defendLord->setUnit( i, defendUnit[i] );
	}

	_analyst->initCreatures( _defendLord );
	_fake->sendFightLord( FIGHTER_ATTACK, _attackLord );
	_analyst->handleFightData( _fake );
	for( i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) ) {
			_fake->sendFightNewUnit( FIGHTER_ATTACK, i, _attackLord->getUnit( i ) );
			_analyst->handleFightData( _fake );
		}
	}

	if( _map ) {
		delete _map;
	}
	_map = new GenericFightMap();
	_map->newFightMap( 9, 15 );

	setupUnits();
	newTurn();
}

void FightEngine::handleFakeSocket( FakeSocket * data )
{
	_server->copyData( (AttalSocket*) data );
	handleSocket( _defendPlayer );
}

void FightEngine::handleSocket( GenericPlayer * player )
{
	if( ( player == _attackPlayer ) || ( player == _defendPlayer ) ) {
		switch( _server->getCla2() ) {
		case C_FIGHT_INIT:
			logEE( "Should not happen FightEngine : FIGHT_INIT" );
			break;
		case C_FIGHT_CREATURE:
			break;
		case C_FIGHT_LORD:
			break;
		case C_FIGHT_CELL:
			break;
		case C_FIGHT_UNIT:
			break;
		case C_FIGHT_MODUNIT:
			break;
		case C_FIGHT_MOVE:
			socketMove();
			break;
		case C_FIGHT_ENDMOVE:
			if( _currentUnit ) {
				postcomputeUnit();

				//if( _troops.remove( _currentUnit ) )
				//	logDD( "Remove current unit" );
				//_troops.last();
				//GenericFightUnit *u = _currentUnit;
				nextUnit( 1 );


			}
			break;
		case C_FIGHT_ACTIVE:
			logEE( "Should not happen" );
			break;
		case C_FIGHT_DISTATTACK:{
			uchar cla = _server->readChar();
			uchar num = _server->readChar();
			handleDistAttack( giveUnit( (CLASS_FIGHTER)cla, num ) );
		}
			break;
		case C_FIGHT_WAIT:
			nextUnit( 0 );
			break;
		case C_FIGHT_FLEE:{
			char result = ( player == _attackPlayer ) ? FIGHTER_DEFENSE_WIN : FIGHTER_ATTACK_WIN;
			result |= FIGHTER_FLED;
			endFight( result );
		}
			break;
		case C_FIGHT_DEFEND:
			if( _currentUnit ) {
				nextUnit( 1 );
				/// XXX: for the moment, "defend" do nothing...
			}
			break;
		case C_FIGHT_DAMAGE:
			break;
		case C_FIGHT_END:
			break;
		default:
			logEE( "Should not happen" );
			break;
		}
	} else {
		logEE( "Should not happen : fight socket from non fighting player" );
	}
}

void FightEngine::socketMove()
{
	uchar cla = _server->readChar();
	uchar num = _server->readChar();
	int row = _server->readInt();
	int col = _server->readInt();
	handleMove( giveUnit( (CLASS_FIGHTER)cla,
			      num ),
		    _map->at( row, col ) );
}

void FightEngine::setupUnits()
{
	/// XXX: improve algo for initial placement of units...
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) ) {
			moveUnit( _attackLord->getUnit( i ), _map->at( i, 0 ) );
		}
		if( _defendLord->getUnit( i ) ) {
			moveUnit(  _defendLord->getUnit( i ), _map->at( i, 14 ) );
		}
	}
}

void FightEngine::newTurn()
{
	_currentUnit = 0;

	// initialize values (movement)
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) ) {
			_attackLord->getUnit( i )->setMaxMove();

		}
		if( _defendLord->getUnit( i ) ) {
			_defendLord->getUnit( i )->setMaxMove();
		}
	}

	// setup list of units;
	orderTroops();

	activateUnit( _troops.last() );
}

void FightEngine::endTurn()
{
	char result = isFightFinished();
	
	if( result ) {
		endFight( result );
	} else {
		newTurn();
	}
}

void FightEngine::endFight( char result )
{
	_server->endFight( _attackPlayer, _defendPlayer, result );
	if( _fightCreature ) {
		_fake->sendFightEnd( result );
		_analyst->handleFightData( _fake );
	}
	emit sig_endFight( result );
}

void FightEngine::moveUnit( GenericFightUnit * unit, GenericFightCell * cell )
{
	if( ! _fightCreature ) {
		_server->moveUnit( _defendPlayer, giveClass( unit ), giveNum( unit ), cell );
	} else {
		_fake->sendFightUnitMove( giveClass( unit ), giveNum( unit ), cell );
		_analyst->handleFightData( _fake );
	}
	_server->moveUnit( _attackPlayer, giveClass( unit ), giveNum( unit ), cell );

	if( giveClass( unit ) == FIGHTER_ATTACK ) {
		unit->goTo( cell );
	} else if( giveClass( unit ) == FIGHTER_DEFENSE ) {
		unit->goTo( cell );
	}
}

void FightEngine::fightUnit( GenericFightUnit * unitAtt, GenericFightUnit * unitDef )
{
	long int damage, bonus;

	if( giveClass( unitAtt ) == giveClass( unitDef ) ) {
		logEE( "Allied units can't fight eachother !" );
		return;
	}

	if( unitAtt->getAttack() > 0 ) {
		damage = ( ( rand() % unitAtt->getAttack() ) + 1 ) * unitAtt->getNumber();
	} else {
		damage = unitAtt->getNumber();
	}

	bonus = giveLord( unitAtt )->getCharac( ATTACK ) - giveLord( unitDef )->getCharac( DEFENSE );
	bonus *= ( giveLord( unitAtt )->getCharac( ATTACK ) ) > ( giveLord( unitDef )->getCharac( DEFENSE ) ) ? 5 : -2;

	long luck = 0;
	if( unitAtt->getLuck() != 0 ) {
		if( unitAtt->getLuck() > 0 ) {
			luck = ( rand() % unitAtt->getLuck() );
		} else {
			luck = - ( rand() % unitAtt->getLuck() );
		}
	}
	bonus += ( luck * 5 );

	bonus = ( bonus > 400 ) ? 400 : ( bonus < -100 ) ? -100 : bonus;

	damage += (damage * bonus) / 100;

	QString msg;
	int oldNum = unitDef->getNumber();
	unitDef->hit( damage );

	msg.sprintf( "%s hit %s : %ld damages %i creatures died", unitAtt->getCreature()->getName().latin1(), unitDef->getCreature()->getName().latin1(), damage,oldNum - unitDef->getNumber() );
	_server->sendFightMessage( msg, _attackPlayer, _defendPlayer );

	//_server->updateUnit( _attackPlayer, giveClass( unitDef ), giveNum( unitDef ), unitDef );
	//_server->updateUnit( _defendPlayer, giveClass( unitDef ), giveNum( unitDef ), unitDef );
	_server->damageUnit( _attackPlayer, giveClass( unitDef ), giveNum( unitDef ), damage );
	if( ! _fightCreature ) {
		_server->damageUnit( _defendPlayer, giveClass( unitDef ), giveNum( unitDef ), damage );
	} else {
		_fake->sendFightDamageUnit( giveClass( unitDef ), giveNum( unitDef ), damage );
		_analyst->handleFightData( _fake );
	}

}

void FightEngine::nextUnit( int remove )
{
	GenericFightUnit * unit;
	uint pos;
	bool found = false;

	if( remove ) {
		pos = _troops.findRef( _currentUnit );
		_troops.removeRef( _currentUnit );
		if( pos == _troops.count() ) {
			pos = 0;
		}

		if( _troops.count() > 0 ) {
			_troops.at( pos );
		}
	}

	while( ( !found ) && ( _troops.count() > 0 ) ) {
		unit = _troops.prev();
		if( !unit ) {
			unit = _troops.last();
		}

		if( unit->getNumber() <= 0 ) {
			pos = _troops.findRef( unit );
			_troops.removeRef( unit );
			if( pos == _troops.count() ) {
				pos = 0;
			}
			if( _troops.count() > 0 ) {
				_troops.at( pos );
			}
		} else {
			found = true;
		}
	}

	if( found ) {
		activateUnit( unit );
	} else {
		endTurn();
	}
}

void FightEngine::activateUnit( GenericFightUnit * unit )
{
	_currentUnit = unit;
	_map->initPath( _currentUnit );

	precomputeUnit();
	_server->activateUnit( _attackPlayer, giveClass( _currentUnit ), giveNum( _currentUnit ) );
	if( _defendPlayer ) {
		_server->activateUnit( _defendPlayer, giveClass( _currentUnit ), giveNum( _currentUnit ) );
	} else {
		_fake->sendFightActivate( giveClass( _currentUnit ), giveNum( _currentUnit ) );
		_analyst->handleFightData( _fake );
	}
}

char FightEngine::isFightFinished()
{
	int sumAtt = 0, sumDef = 0;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = _attackLord->getUnit( i );
		if( unit ) {
			sumAtt += unit->getNumber();
		}

		unit = _defendLord->getUnit( i );
		if( unit ) {
			sumDef += unit->getNumber();
		}
	}

	char result = 0;
	if( sumAtt <= 0 ) {
		result |= FIGHTER_DEFENSE_WIN;
		//_server->sendLordRemove( _attackLord );
		//XXX: Remove lord from server memory
	}
	if( sumDef <= 0 ) {
		result |= FIGHTER_ATTACK_WIN;
		//_server->sendLordRemove( _defendLord );
		//XXX: Remove lord from server memory
	}
	return result;
}

void FightEngine::orderTroops()
{
	GenericFightUnit * unit;

	_troops.clear();
	for( int i = 0; i < MAX_UNIT; i++ ) {
		unit = _attackLord->getUnit( i );
		if( unit && ( unit->getNumber() > 0 ) ) {
			_troops.add( unit );
		}
		unit = _defendLord->getUnit( i );
		if( unit && ( unit->getNumber() > 0 ) ) {
			_troops.add( unit );
		}
	}
}


int FightEngine::giveNum( GenericFightUnit * unit )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( ( _attackLord->getUnit( i ) == unit ) || ( _defendLord->getUnit( i ) == unit ) ) {
			return i;
		}
	}

	logEE( "Should not happen" );
	return -1;
}

CLASS_FIGHTER FightEngine::giveClass( GenericFightUnit * unit )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) == unit ) {
			return FIGHTER_ATTACK;
		}
		if( _defendLord->getUnit( i ) == unit ) {
			return FIGHTER_DEFENSE;
		}
	}

	logEE( "Should not happen" );
	return FIGHTER_ATTACK;
}

GenericFightUnit * FightEngine::giveUnit( CLASS_FIGHTER fighter, int num )
{
	GenericFightUnit * unit;

	if( fighter == FIGHTER_ATTACK ) {
		unit = _attackLord->getUnit( num );
	} else {
		unit = _defendLord->getUnit( num );
	}

	return unit;
}

GenericLord * FightEngine::giveLord( GenericFightUnit * unit )
{
	GenericLord * ret;

	if( giveClass( unit ) == FIGHTER_ATTACK ) {
		ret = _attackLord;
	} else {
		ret = _defendLord;
	}
	
	return ret;
}

void FightEngine::handleMove( GenericFightUnit * unit, GenericFightCell * cell )
{
	if( unit != _currentUnit ) {
		logEE( "The unit moving is not active !!" );
		return;
	}

	if( ! _map->areNeighbour( unit->getCell(), cell ) ) {
		logEE( "The dest cell is not a neighbour cell of the unit" );
		logEE( "%d-%d <> %d-%d", unit->getCell()->getRow(),
		       unit->getCell()->getCol(),
		       cell->getRow(),
		       cell->getCol() );
		return;
	}

	switch( cell->getAccess() ) {
	case UNKNOWN_ACCESS:
		logEE( "Should not happen (UNKNOWN_ACCESS)" );
		break;
	case NEAR:
		moveUnit( unit, cell );
		break;
	case NEAR_OCCUPIED:
		fightUnit( unit, cell->getUnit() );
		break;
	case FAR:
		logEE( "Should not happen (FAR)" );
		break;
	case FAR_OCCUPIED:
		logEE( "Should not happen (FAR_OCCUPIED)" );
		break;
	default:
		logEE( "Should not happen" );
		break;
	}
}

void FightEngine::handleDistAttack( GenericFightUnit * unit )
{
	// XXX: do some test before !!
	fightUnit( _currentUnit, unit );
}

/***************************************************************************/

PlayingTroops::PlayingTroops( bool order )
{
	_order = order;
	setAutoDelete( false );
}

GenericFightUnit * PlayingTroops::take()
{
	GenericFightUnit * ret = last();
	remove();
	return ret;
}

int PlayingTroops::compareItems( QCollection::Item i1, QCollection::Item i2 )
{
	int ret = 0;

	GenericFightUnit * unit1 = (GenericFightUnit *)i1;
	GenericFightUnit * unit2 = (GenericFightUnit *)i2;

	if( unit1->getMove() > unit2->getMove() ) {
		ret = 1;
	} else if( unit1->getMove() < unit2->getMove() ) {
		ret = -1;
	}

	if( !_order ) {
		ret = -ret;
	}

	return ret;
}
