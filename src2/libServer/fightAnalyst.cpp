/****************************************************************
**
** Attal : Lords of Doom
**
** fightAnalyst.cpp
** analyst for the fight phase
**
** Version : $Id: fightAnalyst.cpp,v 1.39 2008/11/12 18:21:30 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 05/01/2002
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

#include "fightAnalyst.h"

// generic include files
#undef __USE_ISOC99
#include <math.h>
#include <stdlib.h>

// include files for QT
// application specific include files
#include "libServer/fightEngine.h"

#include "libCommon/gameData.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericFightMap.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"


Log aifLog;

FakeSocket::FakeSocket()
: AttalSocket()
{
}

void FakeSocket::send()
{
	for( int i = 0; i < 255; i++ ) {
		_bufIn[i] = _bufOut[i+1];
	}
	_lenIn = 3;
	_bufOut[1] = 0;
	_bufOut[2] = 0;
	_bufOut[3] = 0;
	_lenOut = 4;
}

//
// ----- FakeData -----
//

class FakeData : public GameData
{
public:
	FakeData();

	~FakeData();

	virtual void addPlayer();
};

FakeData::FakeData()
: GameData()
{
}

FakeData::~FakeData()
{
	while( ! _players.isEmpty() ) {
		delete _players.takeFirst();
	}
}

void FakeData::addPlayer()
{
	_players.append( new GenericPlayer( 0 ) );
}


/*
FightAnalyst::FightAnalyst( Analyst * analyst )
*/
FightAnalyst::FightAnalyst( GameData * data, AttalSocket * socket )
{
	_socket = socket;
	_ownData = false;
	_data = data;
	_lordAttack = NULL;
	_lordDefense = NULL;
	_map = NULL;
	_engine = NULL;
	_isCreature = false;
}

FightAnalyst::FightAnalyst( GameData * data, FightEngine * engine, AttalSocket * socket  )
{
	_socket = socket;
	_ownData = true;
	_lordAttack = NULL;
	_lordDefense = NULL;
	_map = NULL;
	_isCreature = true;

	_data = (GameData *) new FakeData();
	_data->reinit();

	for( uint i = 0; i < data->getNbPlayer(); i++ ) {
		_data->addPlayer();
	}

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		if( data->getLord( i )->getOwner() ) {
			_data->setLord2Player( i, data->getLord( i )->getOwner()->getNum() );
		}
	}
	_engine = engine;
}

FightAnalyst::~FightAnalyst()
{
	//_map and other data set to 0 cause a weird behavior that cause a method ( endMove ) executed after destrutor
	int i;
	if( _map ) {
		delete _map;
	}
	_map = NULL;

	if( _ownData ) {
		if(_fighter == FIGHTER_ATTACK){
			for(i = 0;i<= MAX_UNIT;i++){
				if(_unitsDef[i]){
					_unitsDef[i]->setFightMap(0);
					delete _unitsDef[i];
				}
			}
		} else {
			for(i = 0;i<= MAX_UNIT;i++){
				if(_unitsAtt[i]){
					_unitsAtt[i]->setFightMap(0);
					delete _unitsAtt[i];
				}
			}
		}
		delete _data;
	}
	_data = NULL;
	for(i = 0;i<= MAX_UNIT;i++){
		_unitsAtt[i]= NULL;
		_unitsDef[i]= NULL;
	}
	_ownData = false;
}

void FightAnalyst::handleFightData( FakeSocket * data )
{
	_socket->setData( (AttalSocket*) data );
	handleFightSocket();
}

void FightAnalyst::handleFightSocket()
{
	switch( _socket->getCla2() ) {
	case C_FIGHT_INIT:
		handleInit();
		break;
	case C_FIGHT_CREATURE:
		break;
	case C_FIGHT_LORD:
		handleOpponent();
		break;
	case C_FIGHT_CELL:
		handleCell();
		break;
	case C_FIGHT_UNIT:
		handleNewUnit();
		break;
	case C_FIGHT_MODUNIT:
		break;
	case C_FIGHT_MOVE:
		handleMove();
		break;
	case C_FIGHT_ENDMOVE:
		break;
	case C_FIGHT_ACTIVE:
		handleActive();
		break;
	case C_FIGHT_DISTATTACK:
		break;
	case C_FIGHT_WAIT:
		break;
	case C_FIGHT_FLEE:
		break;
	case C_FIGHT_DEFEND:
		break;
	case C_FIGHT_DAMAGE:
		handleDamage();
		break;
	case C_FIGHT_END:
		if(!_isCreature){
			updateUnits();
		}
		break;
	}
}

void FightAnalyst::handleInit()
{
	_fighter = (CLASS_FIGHTER) _socket->readChar();
	uchar lord = _socket->readChar();

	GenericLord * myLord;
	if( _fighter == FIGHTER_ATTACK ) {
		//_lordAttack = _analyst->getLord( lord );
		_lordAttack = _data->getLord( lord );
		myLord = _lordAttack;
		_lordDefense = NULL;
		//ialogV( "attack" );
		aifLog.print( "attack" );
	} else {
		//_lordDefense = _analyst->getLord( lord );
		_lordDefense = _data->getLord( lord );
		myLord = _lordDefense;
		_lordAttack = NULL;
		//ialogV( "defense" );
		aifLog.print( "defense" );
	}

	for( int i = 0; i < MAX_UNIT; i ++ ) {
		if( _fighter == FIGHTER_ATTACK ) {
			_unitsAtt[i] = myLord->getUnit( i );
			_unitsDef[i] = NULL;
		} else {
			_unitsDef[i] = myLord->getUnit( i );
			_unitsAtt[i] = NULL;
		}
	}

	if( _map ) {
		delete _map;
	}
	_map = 0;
	_map = new GenericFightMap();
	/// XXX: hardcoded size !!
	_map->newFightMap( 9, 15 );

}

void FightAnalyst::initCreatures( GenericLord * lord )
{
	_fighter = FIGHTER_DEFENSE;
	_lordDefense = lord;

	GenericFightUnit * newUnit, * lordUnit;

	for( int i = 0; i < MAX_UNIT; i ++ ) {
		lordUnit = lord->getUnit( i );
		if( lordUnit ) {
			/// XXX: make copy constructor and operator =
	 		newUnit = new GenericFightUnit( lordUnit->getRace(), lordUnit->getLevel() );
			newUnit->setNumber( lordUnit->getNumber() );
			newUnit->setMove( lordUnit->getMove() );
			newUnit->setHealth( lordUnit->getHealth() );
			_unitsDef[i] = newUnit;
		} else {
			_unitsDef[i] = NULL;
		}
		_unitsAtt[i] = NULL;
	}
	if( _map ) {
		delete _map;
	}
	_map = new GenericFightMap();
	/// XXX: hardcoded size !!
	_map->newFightMap( 9, 15 );
}

void FightAnalyst::handleCell()
{
	// XXX:
}

void FightAnalyst::handleNewUnit()
{
	CLASS_FIGHTER cla = (CLASS_FIGHTER) _socket->readChar();
	uchar pos = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();
	uchar move = _socket->readChar();
	int health = _socket->readInt();

	GenericFightUnit * unit = new GenericFightUnit( race, level );
	unit->setNumber( nb );
	unit->setMove( move );
	unit->setHealth( health );

	GenericFightUnit * oldUnit = NULL;
	if( pos < MAX_UNIT ) {
		if( cla == FIGHTER_ATTACK ) {
			oldUnit = _unitsAtt[pos];
			if( oldUnit ) {
				oldUnit->setFightMap(0);
				delete oldUnit;
			}
			_unitsAtt[pos] = unit;
			_unitsAtt[pos]->setFightMap ( _map );
			_unitsAtt[pos]->setLookingToRight( true );
			_map->at( pos, 1 )->setUnit(_unitsAtt[pos]);
		} else {
			oldUnit = _unitsDef[pos];
			if( oldUnit ) {
				oldUnit->setFightMap(0);
				delete oldUnit;
			}
			_unitsDef[pos] = unit;
			_unitsDef[pos]->setFightMap ( _map );
			_unitsDef[pos]->setLookingToRight( false );
			_map->at( pos, 14 )->setUnit(_unitsDef[pos]);
		}
	}
}

void FightAnalyst::handleMove()
{
	TRACE("FightAnalyst::handleMove" );

	uchar cla = _socket->readChar();
	uchar pos = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();

	if( ! checkValidUnit( pos, (CLASS_FIGHTER)cla) ) {
		return;
	}

	GenericFightUnit * unit = getUnit( pos, (CLASS_FIGHTER)cla );
	if ( !unit->getFightMap () )
	{
		unit->setFightMap ( _map );
	}else {
		if(_map !=unit->getFightMap ()){
			logEE("error, pos %d", pos);
		}
	}
	unit->goTo( _map->at( row, col ) );
}

void FightAnalyst::handleOpponent()
{
	GenericLord * opponentLord;

	uchar id = _socket->readChar();
	uchar attack = _socket->readChar();
	uchar defense = _socket->readChar();

 	//opponentLord = _analyst->getLord( id );
	opponentLord = _data->getLord( id );
	opponentLord->setBaseCharac( ATTACK, attack );
	opponentLord->setBaseCharac( DEFENSE, defense );

	//XXX:
}

void FightAnalyst::handleDamage()
{
	TRACE("FightAnalyst::handleDamage" );
	/* XXX: unused ?? uchar claAtt = */ _socket->readChar();
	/* XXX: unused ?? uchar posAtt = */ _socket->readChar();

	/* XXX: unused ?? CLASS_ATTACK_TYPE type = (CLASS_ATTACK_TYPE) */ _socket->readChar ();

	uchar claDef = _socket->readChar();
	uchar posDef = _socket->readChar();
	int damages = _socket->readInt();
	
	if( ! checkValidUnit( posDef, (CLASS_FIGHTER)claDef) ) {
		return;
	}

	GenericFightUnit * unit = getUnit( posDef, (CLASS_FIGHTER)claDef );
	unit->hit( damages );

}

void FightAnalyst::handleActive()
{
	
	CLASS_FIGHTER cla = (CLASS_FIGHTER) _socket->readChar();
	uchar pos = _socket->readChar();
	
	TRACE("FightAnalyst::handleActive class %d, pos %d", cla, pos );

	
	if( ! checkValidUnit( pos, cla) ) {
		return;
	}

	if( ( cla == _fighter ) && ( pos < MAX_UNIT ) ) {
		GenericFightUnit * unit = getUnit( pos, (CLASS_FIGHTER)cla );
		playUnit( unit, pos );
	} else {
		TRACE("Skip active" );
	}
}



/*
===================================================================
Read the unit structure and the unit position in fightmap, look for
position and strenght of opponent units and move toward or attack
the most powerful enemy unit; power=(num*att)
===================================================================
*/

void FightAnalyst::playUnit( GenericFightUnit * unit, int pos)
{
	TRACE("FightAnalyst::playUnit( GenericFightUnit * unit %p, int pos %d)", unit, pos );

	int i;
	int maxPower = 0;
	int unitPower = 0;
	int epos = -1;
	bool nea = false;
	bool twosize = false;

	if( unit->getCreature()->getSize() == 2 ) {
		twosize = true;
	}

	GenericFightCell * destination = NULL;

	_map->initPath( unit );

	// check if exist an enemy unit reachable and if not, save position of most powerful
	for( i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * enemyUnit = getOpponentUnit(i, _fighter );
		if( enemyUnit ) {
			unitPower = getPower( enemyUnit );
			if( unitPower > maxPower ) {
				GenericFightCell * enemyCell = enemyUnit->getCell();
				GenericFightCell * headCell = NULL;
				if( twosize ) {
					headCell = _map->getHeadCell( enemyCell, unit->isLookingToRight() );
				}
				if( enemyCell->getAccess() == AttalCommon::NEAR_OCCUPIED || unit->isDistAttack()==true ){
					maxPower = unitPower;
					destination = enemyCell; 
					nea = true;
				} else if ( twosize && headCell && headCell->getAccess() == AttalCommon::NEAR_OCCUPIED ){
					GenericFightUnit * headUnit = headCell->getHeadUnit();
					GenericFightUnit * normalUnit = headCell->getUnit();
					if( headUnit && headUnit == enemyUnit ) {
						maxPower = unitPower;
						destination = headCell; 
						nea = true;
					} else if ( normalUnit && normalUnit == enemyUnit ) {
						maxPower = unitPower;
						destination = headCell; 
						nea = true;
					}
				}
				epos = i ;
			}
		}
	}
  
	if( unit->isDistAttack() == false && nea == false ) {
		destination = getValidDestination( unit, epos );
	}

	if(  !destination || epos== -1 ) {
		endMove();
		TRACE("FightAnalyst::playUnit ( finish no dest )" );
		return;
	}

	// movement algorithm
	if( unit->isDistAttack() == false ) {
		GenericFightCell * current;
		QStack<GenericFightCell *> path = _map->computePath( unit, destination );
		while( ! path.isEmpty() ) {
			current = path.pop();
			if( _ownData ) {
				FakeSocket socket;
				socket.sendFightUnitMove( _fighter, pos, current );
				_engine->handleFakeSocket( &socket );
			} else {
				_socket->sendFightUnitMove( _fighter, pos, current );
			}
		}
	} else {
		// distant attack
		if( _ownData  ) {
			FakeSocket socket;
			socket.sendFightDistAttack( getOpponentClass(), epos );
			_engine->handleFakeSocket( &socket );
		} else {
			_socket->sendFightDistAttack( getOpponentClass(), epos );
		}
	}

	endMove();
	
	TRACE("FightAnalyst::playUnit ( finish )" );

}

void FightAnalyst::endMove()
{
	TRACE("FightAnalyst::endMove");
	if( _ownData ) {
		FakeSocket socket;
		socket.sendFightUnitEndMove();
		_engine->handleFakeSocket( &socket );
	} else {
		_socket->sendFightUnitEndMove();
	}
	/* 
	 * caused by a weird behaviour when creature gain, this class is destroyed
	 *  before this code, so _map is 0 and is useless clearPath
	 * ( the destructor is executed before this method )
	 */
	if(_map)
		_map->clearPath();
}

GenericFightUnit * FightAnalyst::getUnit( int num, CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		return _unitsAtt[num];
	} else {
		return _unitsDef[num];
	}
}

GenericFightUnit * FightAnalyst::getOpponentUnit( int num, CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		return _unitsDef[num];
	} else {
		return _unitsAtt[num];
	}
}


CLASS_FIGHTER FightAnalyst::getOpponentClass()
{
	if( _fighter == FIGHTER_DEFENSE ) {
		return FIGHTER_ATTACK;
	} else {
		return FIGHTER_DEFENSE;
	}
}

void FightAnalyst::updateUnits( void )
{
	TRACE( "FightAnalyst::updateUnits" );
	
	GenericLord *oldLord;
	GenericFightUnit * oldUnit, * newUnit;
	uint  i;

	if(_lordAttack){
		oldLord = _data->getLord( _lordAttack->getId() );
		for( i = 0; i < MAX_UNIT; i++ ) {
			newUnit = getUnit( i, FIGHTER_ATTACK );
			if(newUnit){
				if(newUnit->getNumber()==0){
					newUnit=0;
				}
			}
			oldUnit = oldLord->getUnit( i );
			if( newUnit && oldUnit ) {
				oldUnit->setNumber( newUnit->getNumber()  );
				oldUnit->setMove( newUnit->getMove() );
				oldUnit->setHealth( newUnit->getHealth() );
				oldUnit->setFightMap( 0 );
			} else if (oldUnit) {
				oldLord->setUnit(i,0);
			}
		}
	}

	if( _lordDefense ) {
		oldLord = _data->getLord( _lordDefense->getId() );
		for( i = 0; i < MAX_UNIT; i++) {
			newUnit = getUnit( i, FIGHTER_DEFENSE );
			if(newUnit){
				if(newUnit->getNumber()==0){
					newUnit=0;
				}
			}
			oldUnit = oldLord->getUnit( i );
			if( newUnit && oldUnit ) {
				oldUnit->setNumber( newUnit->getNumber()  );
				oldUnit->setMove( newUnit->getMove() );
				oldUnit->setHealth( newUnit->getHealth() );
				oldUnit->setFightMap( 0 );
			} else if (oldUnit) {
				oldLord->setUnit(i,0);
			}
		}
	}
}

int FightAnalyst::getPower( GenericFightUnit * unit )
{
	int power = 0;
	if(unit) 
		power +=unit->getAttack() * unit->getNumber();

	return power;
}

bool FightAnalyst::checkValidUnit( int num , CLASS_FIGHTER cla ) 
{
	TRACE("FightAnalyst::checkValidUnit" );

	bool ret = true;

	if( (CLASS_FIGHTER)cla == FIGHTER_ATTACK ) {
		if( !_unitsAtt[ num ] ) {
			ret = false;
		}
	} else {
		if( !_unitsDef[ num ] ) {
			ret = false;
		}
	}
	if( !ret ) {
		logEE("error not exist unit num %d, cla %d", num, cla );
	}
	return ret;
}

GenericFightCell * FightAnalyst::getValidDestination( GenericFightUnit * /*unit*/ , int opponentPos)
{
	int opponentRow = 0,opponentCol = 0;
	int position = opponentPos;
	GenericFightCell * enemyCell = getOpponentUnit(position, _fighter )->getCell();
	opponentRow = enemyCell->getRow();
	opponentCol = enemyCell->getCol();
	float dist = 100000;
	float tdist = 0;
	GenericFightCell * dcell = NULL;


	if( !_map->inMap( opponentRow,opponentCol ) ) {
		logEE(" request movement outside map, row %d, col %d", opponentRow, opponentCol);
		return NULL;
	}
	

	for( int i = 0; i < _map->getHeight(); i++ ) {
		for( int j = 0; j < _map->getWidth(); j++ ) {
			GenericFightCell * cell = _map->at( i, j );
			if( cell->getAccess() ==  AttalCommon::NEAR_FREE ) {
				tdist = sqrt( pow(i - opponentRow ,2 )  ) + pow(j- opponentCol, 2 ); 
				if( tdist <  dist ) {
					dist = tdist;
					dcell = cell ;
				}
			}	
		}
	}

	return dcell;
}
