/****************************************************************
**
** Attal : Lords of Doom
**
** fightAnalyst.cpp
** analyst for the fight phase
**
** Version : $Id: fightAnalyst.cpp,v 1.17 2003/02/13 20:15:18 lusum Exp $
**
** Author(s) : Pascal Audoux
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
#include <math.h>
#include <stdlib.h>
// include files for QT
// application specific include files
#include "server/fightEngine.h"
#include "server/gameData.h"

#include "common/dataTheme.h"
#include "common/genericFightMap.h"
#include "common/genericLord.h"
#include "common/genericPlayer.h"
#include "common/unit.h"
#include "common/log.h"

Log aifLog;
extern DataTheme DataTheme;

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

}

void FakeData::addPlayer()
{
	_players.append( new GenericPlayer( 0 ) );
}


/*
FightAnalyst::FightAnalyst( Analyst * analyst )
*/
FightAnalyst::FightAnalyst( GameData * data )
{
	_ownData = false;
	_data = data;
	_lordAttack = 0;
	_lordDefense = 0;
	_map = 0;
	_engine = 0;
	_isCreature = false;
}

FightAnalyst::FightAnalyst( GameData data, FightEngine * engine )
{
	int i;
	_ownData = true;
	_lordAttack = 0;
	_lordDefense = 0;
	_map = 0;
	_isCreature = true;

	_data = (GameData *) new FakeData();
	_data->reinit();

	for( i = 0; i < data.getNbPlayer(); i++ ) {
		_data->addPlayer();
	}
	for( i = 0; i < DataTheme.lords.count(); i++ ) {
		if( data.getLord( i )->getOwner() ) {
			_data->setLord2Player( i, data.getLord( i )->getOwner()->getNum() );
		}
	}
	_engine = engine;
}

FightAnalyst::~FightAnalyst()
{
	if( _map ) {
		delete _map;
	}
	if( _ownData ) {
		delete _data;
	}
}

void FightAnalyst::handleFightData( FakeSocket * data )
{
	_socket->copyData( (AttalSocket*) data );
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
		_lordDefense = 0;
		//ialogV( "attack" );
		aifLog.print( "attack" );
	} else {
		//_lordDefense = _analyst->getLord( lord );
		_lordDefense = _data->getLord( lord );
		myLord = _lordDefense;
		_lordAttack = 0;
		//ialogV( "defense" );
		aifLog.print( "defense" );
	}

	for( int i = 0; i < MAX_UNIT; i ++ ) {
		if( _fighter == FIGHTER_ATTACK ) {
			_unitsAtt[i] = myLord->getUnit( i );
			_unitsDef[i] = 0;
		} else {
			_unitsDef[i] = myLord->getUnit( i );
			_unitsAtt[i] = 0;
		}
	}

	if( _map ) {
		delete _map;
	}
	_map = new GenericFightMap();
	/// XXX: hardcoded size !!
	_map->newFightMap( 9, 15 );
}

void FightAnalyst::initCreatures( GenericLord * lord )
{
	_fighter = FIGHTER_DEFENSE;
	_lordDefense = lord;
	for( int i = 0; i < MAX_UNIT; i ++ ) {
		_unitsDef[i] = lord->getUnit( i );
		_unitsAtt[i] = 0;
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

	GenericFightUnit * unit = new GenericFightUnit();
	unit->setCreature( race, level );
	unit->setNumber( nb );
	unit->setMove( move );
	unit->setHealth( health );

	GenericFightUnit * oldUnit = 0;
	if( pos < MAX_UNIT ) {
		if( cla == FIGHTER_ATTACK ) {
			oldUnit = _unitsAtt[pos];
			if( oldUnit ) {
				delete oldUnit;
			}
			_unitsAtt[pos] = unit;
		} else {
			oldUnit = _unitsDef[pos];
			if( oldUnit ) {
				delete oldUnit;
			}
			_unitsDef[pos] = unit;
		}
	}
}

void FightAnalyst::handleMove()
{
	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();

	if( (CLASS_FIGHTER)cla == FIGHTER_ATTACK ) {
		_unitsAtt[ num ]->goTo( _map->at( row, col ) );
	} else {
		_unitsDef[ num ]->goTo( _map->at( row, col ) );
	}
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
	CLASS_FIGHTER cla = (CLASS_FIGHTER) _socket->readChar();
	uchar pos = _socket->readChar();
	int damages = _socket->readInt();

	if( cla == FIGHTER_ATTACK ) {
		_unitsAtt[ pos ]->hit( damages );
	} else {
		_unitsDef[ pos ]->hit( damages );
	}
}

void FightAnalyst::handleActive()
{
	CLASS_FIGHTER cla = (CLASS_FIGHTER) _socket->readChar();
	uchar pos = _socket->readChar();

	if( ( cla == _fighter ) && ( pos < MAX_UNIT ) ) {
		if( cla == FIGHTER_ATTACK ) {
			playUnit( _unitsAtt[pos], pos, 0 );
		} else {
			playUnit( _unitsDef[pos], pos, 0 );
		}
	}
}



/*
===================================================================
Read the unit structure and the unit position in fightmap, look for
position and strenght of opponent units and move toward or attack
the most powerful enemy unit; power=(num*att)
===================================================================
*/

void FightAnalyst::playUnit( GenericFightUnit * unit, int pos ,int flags)
{
	int opponentRow,opponentCol;
	int powerOppUnit[MAX_UNIT];
	int att, num, i;
	GenericFightUnit * opponentUnit[MAX_UNIT];
	CLASS_FIGHTER opponentClass;
	GenericFightCell * current;
		
	_map->initPath( unit );

	if(unit->isDistAttack()){
		_distAttack=true;
	} else {
		_distAttack=false;
	}

	if( _fighter == FIGHTER_DEFENSE ) {
		for( i = 0; i < MAX_UNIT; i++ ) {
			opponentUnit[i] = _unitsAtt[i];
		}
		opponentClass = FIGHTER_ATTACK;
	} else {
		for( i = 0; i < MAX_UNIT; i++ ) {
			opponentUnit[i] = _unitsDef[i];
		}
		opponentClass = FIGHTER_DEFENSE;
	}

	// look for the most powerful enemy unit
	for( i = 0; i < MAX_UNIT; i++ ) {
		if( opponentUnit[i] ) {
			num = opponentUnit[i]->getNumber();
			att = opponentUnit[i]->getAttack();
			powerOppUnit[i]=num*att;
		} else {
			powerOppUnit[i] = 0;
		}
	}

	int movePoints = unit->getMove();
	int startRow = unit->getCell()->getRow();
	int startCol = unit->getCell()->getCol();

	int dist=0;
	int con = 0;
	int y = 0;
	bool nea = false;

	// check the distance of enemy unit
	for( i = 0; i < MAX_UNIT; i++ ) {
		if( powerOppUnit[i] > con ) {
			opponentRow = opponentUnit[i]->getCell()->getRow();
			opponentCol = opponentUnit[i]->getCell()->getCol();
			if(opponentRow>=0 && opponentCol>=0)
			{
				dist=_map->at( opponentRow, opponentCol )->getDist();
				if( (dist <= movePoints && dist!=0) || unit->isDistAttack()==true ){
					con = powerOppUnit[i];
					y = i ;
					nea = true;
				}
			}
			else 
			{
				powerOppUnit[i]=0;
			}
		}
	}

	_opponentUnit=opponentUnit[y];	
	_opponentCell=opponentUnit[y]->getCell();

if(unit->isDistAttack()==false){
	// look if they are enemy near units
	if( nea == true ) {
		opponentRow = opponentUnit[y]->getCell()->getRow();
		opponentCol = opponentUnit[y]->getCell()->getCol();
	} else {
		for( i = 0; i < MAX_UNIT; i++ ) {
			if( powerOppUnit[i] > con ) {
				con = powerOppUnit[i];
				y = i ;
				nea = true;
			}
		}


		// move toward enemy unit
		int rotate=0;
		opponentRow = opponentUnit[y]->getCell()->getRow();
		opponentCol = opponentUnit[y]->getCell()->getCol();
		do{
			if( ( rotate % 2 ) == 0 ) {
				if( ( opponentCol - startCol ) > 0 ) {
					opponentCol -= 1;
				} else {
					opponentCol += 1;
				}
			}	else {
				if( ( opponentRow - startRow ) > 0 ) {
					opponentRow -= 1;
				} else {
					opponentRow += 1;
				}
			}
			rotate++;
		}while(_map->at( opponentRow, opponentCol )->getDist() > movePoints  || _map->at( opponentRow, opponentCol )->getDist()==0);
	}
	_opponentCell=_map->at(opponentRow,opponentCol);
}

	// movement algorithm

if(flags==0) {	
	if( unit->isDistAttack() == false ) {
		QStack<GenericFightCell> path = _map->computePath( unit, _map->at( opponentRow, opponentCol ) );
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
		if( _ownData ) {
			FakeSocket socket;
			socket.sendFightDistAttack( opponentClass, y );
			_engine->handleFakeSocket( &socket );
		} else {
			_socket->sendFightDistAttack( opponentClass, y );
		}
	}
	if( _ownData ) {
		FakeSocket socket;
		socket.sendFightUnitEndMove();
		_engine->handleFakeSocket( &socket );
	} else {
		_socket->sendFightUnitEndMove();
	}
	}
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



void FightAnalyst::updateUnits( void )
{
	GenericLord *oldLord;
	GenericFightUnit * oldUnit, * newUnit;
	uint  i;

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
		} else if (oldUnit) {
			oldLord->setUnit(i,0);
		}
	}

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
			} else if (oldUnit) {
				oldLord->setUnit(i,0);
			}
	}
}


