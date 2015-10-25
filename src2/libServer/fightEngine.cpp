/****************************************************************
**
** Attal : Lords of Doom
**
** fightEngine.cpp
** managing server-side fight
**
** Version : $Id: fightEngine.cpp,v 1.47 2009/09/11 12:49:32 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include "libCommon/attalCommon.h"
#include "libCommon/dataTheme.h"
#include "libCommon/gameData.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"


FightEngine::FightEngine( AttalServer * server )
{
	_server = server;
	_currentUnit = NULL;
	_attackPlayer = NULL;
	_attackLord = NULL;
	_defendPlayer = NULL;
	_defendLord = NULL;
	_defendCreature = NULL;
	_creatureLord = NULL;
	_analyst = NULL;
	_fake = NULL;
	//_troops = 0;
	_map = NULL;
	_num = 0;
	_hasEnded = false;
	_defendExp = 0;
	_attackExp = 0;
	_result.clear();
}

FightEngine::~FightEngine()
{
	if(_fake)
		delete _fake;
	if(_analyst)
		delete _analyst;
	if( _map ) {
		delete _map;
	}
	if(_creatureLord) {
		delete _creatureLord;
	}
}

void FightEngine::print()
{
	logDD("att play %p", _attackPlayer );
	logDD("att lord %p", _attackLord );
	logDD("att lord cell %p", _attackLord->getCell() );
	logDD("def play %p", _defendPlayer );
	logDD("def lord %p", _defendLord );
	logDD("def lord cell %p", _defendLord->getCell() );
}

void FightEngine::init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericPlayer * defendPlayer, GenericLord * defendLord )
{
	_attackPlayer = attackPlayer;
	_attackLord = attackLord;
	_defendPlayer = defendPlayer;
	_defendLord = defendLord;

	//signals to clients
	_server->startFight( _attackPlayer, _attackLord, _defendPlayer, _defendLord );

	initMap();
	setupUnits();
	newTurn();
}

void FightEngine::init( GenericPlayer * attackPlayer, GenericLord * attackLord, GameData * data, GenericMapCreature * creature )
{
	
	/* _creatureLord used only in combacts with creatures */
	_defendCreature = creature;
	_creatureLord = new GenericLord( 0, creature );

	int i;
	_attackPlayer = attackPlayer;
	_attackLord = attackLord;
	//useless, but useful to clarify the code
	_defendPlayer = NULL;
	_defendLord = _creatureLord;

	//initialisation ai
	_fake = new FakeSocket;
	_analyst = new FightAnalyst( data, this, _fake );
	//signals to clients
	_server->startFight( _attackPlayer, _attackLord, _defendPlayer /*NULL*/, _defendLord );
	
	_analyst->initCreatures( _defendLord );
	_fake->sendFightLord( FIGHTER_ATTACK, _attackLord );
	_analyst->handleFightData( _fake );

	for( i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) ) {
			_fake->sendFightNewUnit( FIGHTER_ATTACK, i, _attackLord->getUnit( i ) );
			_analyst->handleFightData( _fake );
		}
	}
	//end initialisation ai

	initMap();
	setupUnits();
	newTurn();
}

void FightEngine::handleFakeSocket( FakeSocket * data )
{
	//_server->copyData( (AttalSocket*) data );
	
	handleSocket( _defendPlayer, data->getData() );
}

void FightEngine::handleSocket( GenericPlayer * player, AttalSocketData data )
{

	_currentData = data;

	if( ( player == _attackPlayer ) || ( player == _defendPlayer ) ) {
		_currentPlayer = player;
		switch( getCla2() ) {
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
			socketFightMove();
			break;
		case C_FIGHT_ENDMOVE:
			socketFightEndMove();
			break;
		case C_FIGHT_ACTIVE:
			logEE( "Should not happen" );
			break;
		case C_FIGHT_DISTATTACK:
			socketFightDistAttack();
			break;
		case C_FIGHT_WAIT:
			nextUnit( 1 );
			break;
		case C_FIGHT_FLEE:
			socketFightFlee( player );
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

void FightEngine::socketFightFlee(GenericPlayer * player)
{
	if( player == _attackPlayer ) {
		_result.setDefenseWin();
	} else {
		_result.setAttackWin();
	}
	_result.setFled();
	endFight();
}

void FightEngine::socketFightEndMove()
{
	if( _currentUnit ) {
		postcomputeUnit();

		//if( _troops.remove( _currentUnit ) )
		//	logDD( "Remove current unit" );
		//_troops.last();
		//GenericFightUnit *u = _currentUnit;
		nextUnit( 1 );
	}

}

void FightEngine::socketFightDistAttack()
{
	uchar cla = readChar();
	uchar num = readChar();

	TRACE("FightEngine::socketFightDistAttack cla %d, num %d",cla,num);
	
	GenericFightUnit * unit =  giveUnit( (CLASS_FIGHTER)cla, num );

	if(_currentUnit->isDistAttack()){
			fightUnit( _currentUnit, unit, ATTACK_SHOOT );
	}
}

void FightEngine::socketFightMove()
{
	uchar cla = readChar();
	uchar num = readChar();
	int row = readInt();
	int col = readInt();
		
	TRACE("FightEngine::socketFightMove cla %d, num %d , row %d ,col %d ,", cla, num, row, col);

	if(row > 9 || col > 15)
		logEE("bug! cla %d, num %d , row %d ,col %d ,", cla, num, row, col);
	
	handleMove( giveUnit( (CLASS_FIGHTER)cla, num ), _map->at( row, col ) );
}

void FightEngine::initMap()
{
	_map = new GenericFightMap();
	_map->newFightMap( 9, 15 );
}

void FightEngine::setupUnits()
{
	TRACE("FightEngine::setupUnits");

	/// XXX: improve algo for initial placement of units...
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _attackLord->getUnit( i ) ) {
			_attackLord->getUnit ( i )->setLookingToRight( true );
			_attackLord->getUnit ( i )->setFightMap ( _map );
			moveUnit( _attackLord->getUnit( i ), _map->at( i, 0 ) );
		}
		if( _defendLord->getUnit( i ) ) {
			_defendLord->getUnit ( i )->setLookingToRight( false );
			_defendLord->getUnit ( i )->setFightMap ( _map );
			moveUnit(  _defendLord->getUnit( i ), _map->at( i, 14 ) );
		}
	}
}

void FightEngine::newTurn()
{
	TRACE("FightEngine::newTurn");

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
	TRACE("FightEngine::endTurn");

	computeFightResultStatus();

	if( _result.isFightFinished() ) {
		endFight();
	} else {
		newTurn();
	}
}

void FightEngine::updateUnits()
{

	TRACE("FightEngine::updateUnits");

	GenericFightUnit  * newUnit;
	uint i;

	if(_attackLord) {
		for( i = 0; i < MAX_UNIT; i++) {
			newUnit = giveUnit( FIGHTER_ATTACK, i );
			if(newUnit){
				if(newUnit->getNumber()==0){
					delete newUnit;
					newUnit=NULL;
					_attackLord->setUnit(i,NULL);
				}
			}
		}
	}
	
	if( _defendLord ) {
		if( !_defendCreature ) {
			for( i = 0; i < MAX_UNIT; i++) {
				newUnit = giveUnit( FIGHTER_DEFENSE, i );
				if(newUnit){
					if(newUnit->getNumber()==0){
						delete newUnit;
						newUnit=NULL;
						_defendLord->setUnit(i,NULL);
					}
				}
			}
		} else {
			int num = 0;
			for( i = 0; i < MAX_UNIT; i++) {
				newUnit = giveUnit( FIGHTER_DEFENSE, i );
				if(newUnit){
					num = newUnit->getNumber();
				} else {
					num = 0;
				}
				_defendCreature->setStack(i,num);
			}
		}
	}

}

void FightEngine::endFight()
{
	TRACE("FightEngine::endFight");
	
	updateUnits();
	_server->endFight( _attackPlayer, _defendPlayer, _result );

	if( _creatureLord ) {
		_fake->sendFightEnd( _result );
		_analyst->handleFightData( _fake );
	}

	_currentUnit = NULL;
	_hasEnded = true;
	emit sig_endFight( _result );
}

void FightEngine::moveUnit( GenericFightUnit * unit, GenericFightCell * cell )
{
	TRACE("FightEngine::moveUnit");

	if( ! _creatureLord ) {
		_server->moveUnit( _defendPlayer, giveClass( unit ), giveNum( unit ), cell );
	} else {
		_fake->sendFightUnitMove( giveClass( unit ), giveNum( unit ), cell );
		_analyst->handleFightData( _fake );
	}
	_server->moveUnit( _attackPlayer, giveClass( unit ), giveNum( unit ), cell );

	unit->goTo( cell );
}

void FightEngine::fightUnit( GenericFightUnit * unitAtt, GenericFightUnit * unitDef, CLASS_ATTACK_TYPE attackType )
{
	TRACE("FightEngine::fightUnit");

	long int damage, bonus;

	if (unitAtt->canAttack()) {
		if( giveClass( unitAtt ) == giveClass( unitDef ) ) {
			logEE( "Allied units can't fight eachother !" );
			return;
		}

		if( unitAtt->getAttack() > 0 ) {
			damage = ( ( rand() % unitAtt->getAttack() ) + 1 ) * unitAtt->getNumber();
		} else {
			damage = unitAtt->getNumber();
		}

		GenericLord * lordAtt = giveLord( unitAtt );
		GenericLord * lordDef = giveLord( unitDef ); 
		GenericPlayer * playAtt = lordAtt->getOwner();
		GenericPlayer * playDef = lordDef->getOwner();
		QString playAttName = playAtt ? playAtt->getConnectionName() : "server";
		QString playDefName = playDef ? playDef->getConnectionName() : "server";

		bonus = lordAtt->getCharac( ATTACK ) - lordDef->getCharac( DEFENSE );
		bonus *= lordAtt->getCharac( ATTACK ) > lordDef->getCharac( DEFENSE )  ? 5 : -2;

		/* luck management */
		long luck = 0;
		if( unitAtt->getLuck() != 0 ) {
			if( unitAtt->getLuck() > 0 ) {
				luck = ( rand() % unitAtt->getLuck() );
			} else {
				luck = - ( rand() % unitAtt->getLuck() );
			}
		}
		bonus += ( luck * 5 );
		/* end luck management */

		bonus = ( bonus > 400 ) ? 400 : ( bonus < -100 ) ? -100 : bonus;

		damage += (damage * bonus) / 100;

		damage = ( damage >= 0 ) ? damage : 0;

		QString msg;
		int oldNum = unitDef->getNumber();
		//logDD("hit server");
		unitDef->hit( damage );

		QString attName = unitAtt->getCreature()->getName();
		QString defName = unitDef->getCreature()->getName();

		msg.sprintf( "%s ( %s ) hit %s ( %s ) : %ld damages %li creatures died", qPrintable( attName ), qPrintable(playAttName), qPrintable( defName ), qPrintable( playDefName), damage,oldNum - unitDef->getNumber() );
		logDD("%s" , qPrintable(msg));
		_server->sendFightMessage( msg, _attackPlayer, _defendPlayer );

		// manage experience earned
		uint exp = ( oldNum - unitDef->getNumber() ) * unitDef->getMaxHealth();
		if( lordDef == _attackLord ) {
			_defendExp += exp;
		} else {
			_attackExp += exp;
		}

		TRACE("Experience %d, oldNum %d, unitDef->getNumber %d, unitDef->maxhealth %d", exp, oldNum, unitDef->getNumber(), unitDef->getMaxHealth());
		TRACE("defexp %d, attexp %d", _defendExp, _attackExp);

		//_server->updateUnit( _attackPlayer, giveClass( unitDef ), giveNum( unitDef ), unitDef );
		//_server->updateUnit( _defendPlayer, giveClass( unitDef ), giveNum( unitDef ), unitDef );
		_server->damageUnit( _attackPlayer,
				giveClass ( unitAtt ),
				giveNum ( unitAtt ),
				attackType,
				giveClass( unitDef ),
				giveNum( unitDef ),
				damage );
		if( ! _creatureLord ) {
			_server->damageUnit( _defendPlayer,
					giveClass ( unitAtt ),
					giveNum ( unitAtt ),
					attackType,
					giveClass( unitDef ),
					giveNum( unitDef ),
					damage ); 
		} else {
			_fake->sendFightDamageUnit
				( giveClass ( unitAtt ),
					giveNum ( unitAtt ),
					attackType,
					giveClass( unitDef ),
					giveNum( unitDef ),
					damage );
			_analyst->handleFightData( _fake );
		}
		unitAtt->finishAttack();
	}
}

void FightEngine::nextUnit( int remove )
{
	TRACE("FightEngine::nextUnit");

	//this function search next unit in _troops list and remove used unit
	GenericFightUnit * unit;
	int pos;
	bool found = false;

	computeFightResultStatus();

	//update units before end of fight
	if( _result.isFightFinished() ) {
		endFight();
		return;
	}

	//remove current unit from list
	if( remove ) {
		pos = _troops.indexOf( _currentUnit );
		_troops.removeAt( pos );
		if( pos == _troops.count() ) {
			pos = 0;
		}

		if( _troops.count() > 0 ) {
			_troops.at( pos );
		}
	}

	for ( int i = 0;  i < _troops.count(); i++  ) {
		unit = _troops.at(i);
		if( !unit ) {
			unit = _troops.last();
		}

		if( unit->getNumber() <= 0 ) {
			pos = _troops.indexOf( unit );
			_troops.removeAt( pos );
			if( pos == _troops.count() ) {
				pos = 0;
			}
			if( _troops.count() > 0 ) {
				_troops.at( pos );
			}
		} else {
			found = true;
		}
		if ( found ) {
			break;
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
	_currentUnit->activateAttack();

	precomputeUnit();
	_server->activateUnit( _attackPlayer, giveClass( _currentUnit ), giveNum( _currentUnit ) );
	if( _defendPlayer ) {
		_server->activateUnit( _defendPlayer, giveClass( _currentUnit ), giveNum( _currentUnit ) );
	} else {
		_fake->sendFightActivate( giveClass( _currentUnit ), giveNum( _currentUnit ) );
		_analyst->handleFightData( _fake );
	}
}

void FightEngine::computeFightResultStatus()
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

	if( sumAtt <= 0 ) {
		_result.setDefenseWin();
		//_server->sendLordRemove( _attackLord );
		//XXX: Remove lord from server memory
	}
	if( sumDef <= 0 ) {
		_result.setAttackWin();
		//_server->sendLordRemove( _defendLord );
		//XXX: Remove lord from server memory
	}
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
	
	TRACE("FightEngine::handleMove cell access %d, row %d , col %d", cell->getAccess(), cell->getRow(), cell->getCol());

	if( unit != _currentUnit ) {
		logEE( "The unit moving is not active !!" );
		return;
	}
    
	GenericFightCell* neib = NULL;

	// 2 cell unit test  
	if ( unit->getCreature()->getSize () == 2 ){

		neib = _map->getHeadCell( cell , unit->isLookingToRight() );

		if( ! _map->areNeighbours( unit->getCell(), cell ) &&  ! _map->areNeighbours( neib, cell )) {
			logEE( "The dest cell is not a neighbour cell of the unit" );
			printStatus( unit , cell );
			return;
		}
	} else {
	// 1 cell unit test
		if( ! _map->areNeighbours( unit->getCell(), cell ) ) {
			logEE( "The dest cell is not a neighbour cell of the unit" );
			printStatus( unit , cell );
			return;
		}
	}

	switch( cell->getAccess() ) {
	case AttalCommon::UNKNOWN_ACCESS:
		logEE( "Should not happen (UNKNOWN_ACCESS)" );
		break;
	case AttalCommon::NEAR_FREE:
		TRACE("NEAR_FREE");
		moveUnit( unit, cell );
		break;
	case AttalCommon::NEAR_OCCUPIED: 
		{
			TRACE("NEAR_OCCUPIED");
			GenericFightUnit * cellUnit = cell->getUnit();
			if( cellUnit ){
				if( cellUnit != unit){
					fightUnit( unit, cellUnit, ATTACK_FIGHT );
				} else if( neib ) {
					cellUnit = neib->getUnit();
					if( cellUnit ){
						if( cellUnit != unit){
							fightUnit( unit, cellUnit, ATTACK_FIGHT );
						}
					}
				}
			} else {
				logEE("NEAR_OCCUPIED access error, 2 unit cell \n");
			}
		}
		break;
	case AttalCommon::FAR_FREE:
		logEE( "Should not happen (AttalCommon::FAR_FREE)" );
		printStatus( unit , cell );
		break;
	case AttalCommon::FAR_OCCUPIED:
		logEE( "Should not happen (FAR_OCCUPIED)" );
		printStatus( unit , cell );
		break;
	case AttalCommon::NONE:
		logEE( "Should not happen (NONE)" );
		printStatus( unit , cell );
		break;
	default:
		logEE( "Should not happen" );
		printStatus( unit , cell );
		break;
	}
}

uint FightEngine::getExperience( GenericLord * lord )
{
	/// XXX: we could have 'lord' in parameter and know the winner...
	uint ret = 0;

	if( lord == _attackLord ) {
		ret = _attackExp;
	} else if( lord == _defendLord ) {
		ret = _defendExp;
	}

	TRACE("FightEngine::getExperience %d",ret );

	return ret;
}

void FightEngine::printStatus( GenericFightUnit * unit, GenericFightCell * cell )
{
#ifdef QT_DEBUG
	unit->display();
#endif
	logEE( "start <> dest");
	logEE( "%d-%d <> %d-%d", unit->getCell()->getRow(),
					unit->getCell()->getCol(),
					cell->getRow(),
					cell->getCol() 
					);

}

uchar FightEngine::getCla1()
{
	return _currentData.getCla1();
}

uchar FightEngine::getCla2()
{
	return _currentData.getCla2();
}

uchar FightEngine::getCla3()
{
	return _currentData.getCla3();
}

int FightEngine::readInt()
{
	return _currentData.readInt();
}

unsigned char FightEngine::readChar()
{
	return _currentData.readChar();
}
