/****************************************************************
**
** Attal : Lords of Doom
**
** unit.cpp
**  manages unit (army)
**
** Version : $Id: unit.cpp,v 1.23 2002/12/17 12:53:25 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2000
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

#include "unit.h"

 // generic include files
#include <stdio.h>
// include files for QT
#include <qfile.h>
#include <qtextstream.h>
// application specific include files
#include "common/creature.h"
#include "common/dataTheme.h"
#include "common/genericFightCell.h"
#include "common/log.h"

extern QString DATA_PATH;
extern DataTheme DataTheme;

//
// ----- GenericFightUnit -----
//

GenericFightUnit::GenericFightUnit()
	: _number( 0 ),
	  _move  ( 0 ),
	  _health( 0 ),
	  _race  ( 0 ),
	  _level ( 0 ),
	  _currentFightCell( 0 ),
	  _creature( 0 )
{
}

void GenericFightUnit::setCreature( QString name )
{
     _race = DataTheme.creatures.findRace( name );
     _level = DataTheme.creatures.findLevel( name );
     _creature = DataTheme.creatures.at( _race, _level );
     _move = _creature->getMaxMove();
     _health = _creature->getMaxHealth();
}

void GenericFightUnit::setCreature( int race, int level )
{
     _race = race;
     _level = level;
     _creature = DataTheme.creatures.at( _race, _level );
     _move = _creature->getMaxMove();
     _health = _creature->getMaxHealth();
}

void GenericFightUnit::setCreature( Creature * creature )
{
	_creature = creature;
	_race = _creature->getRace();
	_level = _creature->getLevel();
	_move = _creature->getMaxMove();
	_health = _creature->getMaxHealth();
}

/*!
  display on the log infos about unit
*/

void GenericFightUnit::display()
{
	logDD("Unit race : %d - level : %d - number : %d", _race, _level, _number );
	
	logDD("Attack : %d - Defense : %d", _creature->getAttack(), _creature->getDefense() );
	logDD("Health : %d / %d", _health, _creature->getMaxHealth() );
	logDD("Move : %d / %d", _move, _creature->getMaxMove() );
	logDD("Far Attack : %s", _creature->getDistAttack()?"true":"false" );
	logDD("Damages [%d - %d]", _creature->getMinDamages(), _creature->getMaxDamages() );
	logDD("Morale : %d, Luck : %d", _creature->getMorale(), _creature->getLuck() );

}

void GenericFightUnit::setMaxMove()
{
	_move = _creature->getMaxMove();
}
int GenericFightUnit::hit( long int nb )
{
	if( _number <= 0 ) {
		logEE( "Unit has already been destroyed" );
		return 0;
	}

	int oldNum = _number;
	int oldH = _health;
	int maxH = _creature->getMaxHealth();

	int point = ( (_number-1) * maxH ) + _health - nb;
	_number = point / maxH;
	_health = point % maxH;

	if( _health == 0 ) {
		_health = maxH;
	} else {
		_number++;
	}

	if( _number < 0 ) {
		_number = 0;
	}

	logDD( "Before hit : %d creatures, h = %d", oldNum, oldH );
	logDD( "Hit : %d", nb );
	logDD( "Now : %d creatures, h = %d", _number, _health );
	return ( oldNum - _number );
}

void GenericFightUnit::goTo( GenericFightCell * cell ) 
{
	if( _currentFightCell ) {
		_currentFightCell->setUnit( 0 );
	}
	_currentFightCell = cell;
	_currentFightCell->setUnit( this );
}

int GenericFightUnit::getAttack()
{
	return _creature->getAttack();
}
	
int GenericFightUnit::getDefense()
{
	return _creature->getDefense();	
}

int GenericFightUnit::getDistAttack()
{
	return _creature->getDistAttack();	
}
	
bool GenericFightUnit::isDistAttack()
{
	return _creature->isDistAttack();
}
	
int GenericFightUnit::getMaxHealth()
{
	return _creature->getMaxHealth();	
}
	
int GenericFightUnit::getMaxMove()
{
	return _creature->getMaxMove();	
}
	
int GenericFightUnit::getMorale()
{
	return _creature->getMorale();
}
	
int GenericFightUnit::getLuck()
{
	return _creature->getLuck();
}

int GenericFightUnit::getMinDamages()
{
	return _creature->getMinDamages();
}
		
int GenericFightUnit::getMaxDamages()
{
	return _creature->getMaxDamages();
}
int GenericFightUnit::getMantCost(int ress)
{
	return _creature->getMantCost(ress);
}
