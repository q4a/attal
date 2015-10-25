/****************************************************************
**
** Attal : Lords of Doom
**
** unit.cpp
**  manages unit (army)
**
** Version : $Id: unit.cpp,v 1.26 2009/11/04 22:28:11 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QFile>
#include <QTextStream>
// application specific include files
#include "libCommon/creature.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericFightCell.h"
#include "libCommon/genericFightMap.h"
#include "libCommon/log.h"

extern QString DATA_PATH;

//
// ----- GenericFightUnit -----
//

GenericFightUnit::GenericFightUnit()
	: _number( 0 ),
	  _move  ( 0 ),
	  _health( 0 ),
	  _experience( 0 ),
	  _race  ( 0 ),
	  _level ( 0 ),
	  _currentFightCell( 0 ),
	  _currentNeibCell( 0 ),
	  _creature( 0 ),
	  _map ( 0 ),
	  _hasAttack ( true )
{
}
	
GenericFightUnit::GenericFightUnit( QString name )
	: _number( 0 ),
	  _move  ( 0 ),
	  _health( 0 ),
	  _experience( 0 ),
	  _race  ( 0 ),
	  _level ( 0 ),
	  _currentFightCell( 0 ),
	  _currentNeibCell( 0 ),
	  _creature( 0 ),
	  _map ( 0 ),
	  _hasAttack ( true )
{
	setCreature( name );
}

GenericFightUnit::GenericFightUnit( uint race, uint level )
	: _number( 0 ),
	  _move  ( 0 ),
	  _health( 0 ),
	  _experience( 0 ),
	  _race  ( 0 ),
	  _level ( 0 ),
	  _currentFightCell( 0 ),
	  _currentNeibCell( 0 ),
	  _creature( 0 ),
	  _map ( 0 ),
	  _hasAttack ( true )
{
	setCreature( race, level );
}

GenericFightUnit::GenericFightUnit( Creature * creature )
	: _number( 0 ),
	  _move  ( 0 ),
	  _health( 0 ),
	  _experience( 0 ),
	  _race  ( 0 ),
	  _level ( 0 ),
	  _currentFightCell( 0 ),
	  _currentNeibCell( 0 ),
	  _creature( 0 ),
	  _map ( 0 ),
	  _hasAttack ( true )
{
	setCreature( creature );
}

GenericFightUnit::~GenericFightUnit()
{
	_map = 0;
}


void GenericFightUnit::setCreature( QString name )
{
	_race = DataTheme.creatures.findRace( name );
	_level = DataTheme.creatures.findLevel( name );
	_creature = DataTheme.creatures.at( _race, _level );
	checkAndSetDefaultCharac( _creature );
}

void GenericFightUnit::setCreature( uint race, uint level )
{
	_race = race;
	_level = level;
	_creature = DataTheme.creatures.at( _race, _level );
	checkAndSetDefaultCharac( _creature );
}

void GenericFightUnit::setCreature( Creature * creature )
{
	_creature = creature;
	_race = _creature->getRace();
	_level = _creature->getLevel();
	checkAndSetDefaultCharac( _creature );
}

/* hack? to not reset health to max health in parser.cpp, i will keep private and hope it will not break anything */
void GenericFightUnit::checkAndSetDefaultCharac( Creature * creature )
{
	if( _move == 0 ) {
		_move = creature->getMaxMove();
	}
	if( _health == 0 ) {
		_health = creature->getMaxHealth();
	}
}


/*!
  display on the log infos about unit
*/

void GenericFightUnit::display()
{
	
	if( _creature ) {
		logDD("Unit race : %d - level : %d - number : %d", _race, _level, _number );

		logDD("Attack : %d - Defense : %d", _creature->getAttack(), _creature->getDefense() );
		logDD("Health : %d / %d", _health, _creature->getMaxHealth() );
		logDD("Move : %d / %d", _move, _creature->getMaxMove() );
		logDD("Far Attack : %s", _creature->getDistAttack()?"true":"false" );
		logDD("Damages [%d - %d]", _creature->getMinDamages(), _creature->getMaxDamages() );
		logDD("Morale : %d, Luck : %d", _creature->getMorale(), _creature->getLuck() );
	} else {
		logEE("Creature not exist" );
	}

}

QString GenericFightUnit::getUnitDescription()
{
	QString text;		

	if( _creature ) {
		text = "";
		text = _creature->getName();
		text += "\n\n";
		text += "Number: " + QString::number( getNumber() );
		text += "\n";
		text += "HitPoints: " + QString::number ( getHealth ())
			+ "/" + QString::number ( getMaxHealth () );
		text += "\n";
		text += "Attack: " + QString::number ( getAttack () );
		text += "\n";
		if (isDistAttack ())
		{
			text += "Distance attack: " + QString::number ( getDistAttack ());
			text += "\n";
		}

		text += "Defense: " + QString::number (getDefense ());

		text += "\n";
		text += "Morale: " + QString::number (getMorale ());

		text += "\n";
		text += "Luck: " + QString::number (getLuck ());

		text += "\n";
		text += "Damage: Min/Max " + QString::number (getMinDamages ()) +
				"/" + QString::number (getMaxDamages ());
		text += "\n";
		
		text += "Experience: " + QString::number ( getExperience () );
		text += "\n";
		
		text += "MovePoints: " + QString::number ( getMove() );
		text += "\n";

	} else {
		logEE("Creature not exist" );
	}

	return text;

}

void GenericFightUnit::setMaxMove()
{
	_move = _creature->getMaxMove();
}

int GenericFightUnit::hit( long int nb )
{
	if( _number == 0 ) {
		logEE( "Unit has already been destroyed" );
		return 0;
	}
	if( _health < 0 ) {
		logEE( "Unit has already been destroyed" );
		logEE( "Now : %d creatures, h = %d", _number, _health );
		return 0;
	}

	long int number = _number;
	long int oldNum = _number;
	long int oldH = _health;
	long int maxH = _creature->getMaxHealth();

	long int point = ( (_number-1) * maxH ) + _health - nb;
	number = point / maxH;
	_health = point % maxH;

	if( _health == 0 ) {
		_health = maxH;
	} else {
		number++;
	}

	if( number < 0 ) {
		_number = 0;
	} else {
		_number = number;
	}

	if( _health <= 0 ) {
		_number = 0;
	}

	logDD( "Before hit : %d creatures, h = %d", oldNum, oldH );
	logDD( "Hit : %d", nb );
	logDD( "Now : %d creatures, h = %d", _number, _health );
	return ( int ) ( oldNum - _number );
}

void GenericFightUnit::goTo( GenericFightCell * cell )
{
	if( _currentFightCell ) {
		_currentFightCell->setUnit( 0 );

		if ( _currentNeibCell )
		{
			_currentNeibCell->setUnit ( 0 );
		}
	}

	_currentFightCell = cell;
	_currentFightCell->setUnit( this );

	_currentNeibCell = getNeighbour( cell );

	if ( _currentNeibCell )
	{
		_currentNeibCell->setUnit ( this );
	}

}

GenericFightCell * GenericFightUnit::getNeighbour( GenericFightCell * cell )
{

	GenericFightCell * neib = 0;
	if ( _creature->getSize () != 2 )
	{
		return neib;
	}
	if( cell ) {
		if ( _lookingToRight )
		{
			neib = _map->getNeighbour2 ( cell );
		}	else {
			neib = _map->getNeighbour5 ( cell );
		}
	}
	return neib;

}

int GenericFightUnit::getAttack()
{
	if( _creature ) {
		return _creature->getAttack();
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getDefense()
{
	if( _creature ) {
		return _creature->getDefense();	
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getDistAttack()
{
	if( _creature ) {
		return _creature->getDistAttack();	
	} else  {
		return 0; 
	}
}

bool GenericFightUnit::isDistAttack()
{
	if( _creature ) {
		return _creature->isDistAttack();
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getMaxHealth()
{
	if( _creature ) {
		return _creature->getMaxHealth();	
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getMaxMove()
{
	if( _creature ) {
		return _creature->getMaxMove();	
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getMorale()
{
	if( _creature ) {
		return _creature->getMorale();
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getLuck()
{
	if( _creature ) {
		return _creature->getLuck();
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getMinDamages()
{
	if( _creature ) {
		return _creature->getMinDamages();
	} else  {
		return 0; 
	}
}

int GenericFightUnit::getMaxDamages()
{
	if( _creature ) {
		return _creature->getMaxDamages();
	} else  {
		return 0; 
	}
}
int GenericFightUnit::getMantCost(int ress)
{
	if( _creature ) {
		return _creature->getMantCost(ress);
	} else  {
		return 0; 
	}
}

bool GenericFightUnit::isLookingToRight ()
{
	return _lookingToRight;
}

void GenericFightUnit::setLookingToRight ( bool mirror )
{
	_lookingToRight = mirror;
}

void GenericFightUnit::setFightMap ( GenericFightMap* map )
{
	_map = map;
}

