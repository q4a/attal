/****************************************************************
**
** Attal : Lords of Doom
**
** genericLord.cpp
** manages a lord and its units
**
** Version : $Id: genericLord.cpp,v 1.66 2009/11/04 22:26:57 lusum Exp $
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

#include "genericLord.h"


// generic include files
#include <stdlib.h>

// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/categoryManager.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/log.h"
#include "libCommon/lordCategoryModel.h"
#include "libCommon/pathFinder.h"
#include "libCommon/unit.h"
#include "libCommon/warMachine.h"

extern QString DATA_PATH;

LordCharac detectCharac( const QString & type )
{
	LordCharac ret = ATTACK;

	if( type == "attack" ) {
		ret = ATTACK;
	} else if( type == "defense" ) {
		ret = DEFENSE;
	} else if( type == "power" ) {
		ret = POWER;
	} else if( type == "knowledge" ) {
		ret = KNOWLEDGE;
	} else if( type == "move" ) {
		ret = MOVE;
	} else if( type == "movemax" ) {
		ret = MAXMOVE;
	} else if( type == "technicpoint" ) {
		ret = TECHNICPOINT;
	} else if( type == "maxtechnicpoint" ) {
		ret = MAXTECHNICPOINT;
	} else if( type == "morale" ) {
		ret = MORALE;
	} else if( type == "luck" ) {
		ret = LUCK;
	} else if( type == "vision" ) {
		ret = VISION;
	} else if( type == "experience" ) {
		ret = EXPERIENCE;
	} else if( type == "charisma" ) {
		ret = CHARISMA;
	} else if( type == "level" ) {
		ret = LEVEL;
	} else {
		logEE( "Unknown Lord's Characteristic: %s", qPrintable(type) );
	}

	return ret;
}

QString getCharacName( LordCharac charac )
{
	QString ret = "";
	switch( charac ) {
	case ATTACK:
		ret = "attack";
		break;
	case DEFENSE:
		ret = "defense";
		break;
	case POWER:
		ret = "power";
		break;
	case KNOWLEDGE:
		ret = "knowledge";
		break;
	case MOVE:
		ret = "move";
		break;
	case MAXMOVE:
		ret = "movemax";
		break;
	case TECHNICPOINT:
		ret = "technicpoint";
		break;
	case MAXTECHNICPOINT:
		ret = "maxtechnicpoint";
		break;
	case MORALE:
		ret = "morale";
		break;
	case LUCK:
		ret = "luck";
		break;
	case VISION:
		ret = "vision";
		break;
	case EXPERIENCE:
		ret = "experience";
		break;
	case CHARISMA:
		ret = "charisma";
		break;
	case LEVEL:
		ret = "level";
		break;
	default:
		break;
	}

	return ret;
}

//
// ----- GenericLord -----
//

/** add comments here */
GenericLord::GenericLord( int id )
	: _player( 0 ),
	  _currentCell( NULL ),
		_dest( NULL ),
		_hasBook( false ),
	  _move( 0 ),
	  _maxMove( 0 ),
	  _technicPoints( 0 ),
	  _maxTechnicPoints( 0 ),
	  _morale( 0 ),
	  _luck( 0 ),
	  _experience( 0 ),
	  _power( 0 ),
	  _knowledge( 0 ),
	  _attack( 0 ),
	  _defense( 0 ),
	  _vision( 0 ),
	  _charisma( 0 ),
	  _level( 0 ),
		_visible( true ), 
		_sleeping( false )
{
	_artefactManager = new ArtefactManager( this );
	
	_model = 0;

	setLordModelConstr( id );
}
	
GenericLord::GenericLord( int id, GenericMapCreature * creature )
	: _player( 0 ),
	  _currentCell( NULL ),
		_dest( NULL ),
		_hasBook( false ),
	  _move( 0 ),
	  _maxMove( 0 ),
	  _technicPoints( 0 ),
	  _maxTechnicPoints( 0 ),
	  _morale( 0 ),
	  _luck( 0 ),
	  _experience( 0 ),
	  _power( 0 ),
	  _knowledge( 0 ),
	  _attack( 0 ),
	  _defense( 0 ),
	  _vision( 0 ),
	  _charisma( 0 ),
	  _level( 0 ),
		_visible( true ), 
		_sleeping( false )
{
	_artefactManager = new ArtefactManager( this );
	
	_model = 0;

	setLordModelCreature( id, creature );
}

GenericLord::~GenericLord()
{
	clearUnits();
	delete _artefactManager;
}

void GenericLord::clearUnits()
{
	//TRACE(" Clear units ");

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] ) {
			delete _units[i];
			_units[i] = NULL;
		}
	}
}

void GenericLord::reinit()
{
	TRACE(" Reinit ");

	setId( _id );
	setOwner( NULL );
	_currentCell = 0;
}

QString GenericLord::getName()
{
	QString ret = "";
	if( _id != 0 ) {
		ret = _model->getName();
	}
	return ret;
}

QString GenericLord::getCategoryName()
{
	QString ret = "";
	if( _id != 0 ) {
		ret = _model->getCategory()->getName();
	}
	return ret;
}

int GenericLord::getRace()
{
	int ret = 0;
	if( _id != 0 ) {
		ret = _model->getRace();
	}
	return ret;
}

void GenericLord::newTurn()
{
	setBaseCharac( MOVE, getCharac( MAXMOVE ) );
}

bool GenericLord::setLordModelInit(int id )
{
	_id = id;
	if( _id >= DataTheme.lords.count() ) {
		logEE( "Lord model id %d doesn't exist", _id );
		_id = 0;
		_model = 0;
		return false;
	}

	_model = DataTheme.lords.at( _id );
	return true;
}

void GenericLord::setLordModelCreature( int id, GenericMapCreature * creature )
{
	if( !setLordModelInit( id )) {
		return;		
	}

 //XXX to change using creature instread lord param	
	updateLordModelCharac();
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( creature->getStack( i ) > 0 ) {
			_units[i] = new GenericFightUnit( creature->getCreature() );
			_units[i]->setNumber( creature->getStack( i ) );
		} else {
			_units[i] = NULL;
		}
	}
	
	updateLordModelMachines();
}

void GenericLord::setLordModelConstr( int id )
{

	if( !setLordModelInit( id )) {
		return;		
	}
	updateLordModelCharac();
	
	updateLordModelUnits();

	updateLordModelMachines();
}

void GenericLord::setId( int id )
{
	if( !setLordModelInit( id )) {
		return;		
	}
	updateLordModelCharac();

	clearUnits();

	updateLordModelUnits();
	
	updateLordModelMachines();

	/// XXX: artefacts ???
}

void GenericLord::updateLordModelCharac()
{
	_move = _model->getBaseCharac( MOVE );
	_maxMove = _model->getBaseCharac( MAXMOVE );
	_luck = _model->getBaseCharac( LUCK );
	_experience = 0;
	_attack = _model->getBaseCharac( ATTACK );
	_defense = _model->getBaseCharac( DEFENSE );
	_hasBook = _model->isBook();
	_vision = _model->getBaseCharac( VISION );
	_level = _model->getBaseCharac( LEVEL );
	newTurn();
}

void GenericLord::updateLordModelUnits()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _model->getUnit( i ) ) {
			GenericFightUnit * unit = new GenericFightUnit( *_model->getUnit( i ) );
			_units[i] = unit;
		} else {
			//necessary to initialise the _units vector
			_units[i] = NULL;
		}
	}
}

void GenericLord::updateLordModelMachines()
{
	_machines.clear();
	for( uint j = 0; j < _model->getMachineNumber(); j++ ) {
		addMachine( getMachine( j ) );
	}

}

void GenericLord::setCell( GenericCell * cell )
{
	getOut();
	if( _currentCell ) {
		_currentCell->setLord( NULL );
	}

	_currentCell = cell ;

	if( _currentCell ) {
		_currentCell->setLord( (GenericLord *)this );
	} 
}
	
void GenericLord::setOwner( GenericPlayer * player ) { 
	
	TRACE("GenericLord::setOwner id %d", getId() );

	if( _player ) {
		_player->removeLord( this );
	}
	_player = player; 
}

void GenericLord::getOut()
{
	if( _currentCell ) {
		_currentCell->setLord( NULL );
		GenericBase * base = _currentCell->getBase();
		if( base != 0 ) {
			if( base->getGarrisonLord() == this ) {
				base->removeGarrisonLord();
			} else if( base->getVisitorLord() == this ) {
				base->removeVisitorLord();
			}
		}
	}
}

void GenericLord::setVisible( bool state )
{
	if( ! state ) {
		if( _currentCell ) {
			_currentCell->setLord( 0 );
		}
		//_currentCell = 0;
	}
	_visible = state;
}

void GenericLord::endTurn()
{
	_move = _maxMove;
}

QString GenericLord::getLordDescription()
{
	QString ret = "";
	ret += getName() + "\n";
	ret += QString("Id %1 \n").arg( _id );
	ret += QString("MaxMove %1 \n").arg(_maxMove); 
	ret += QString("Vision %1 \n").arg(_vision); 
	ret += QString("Experience %1 \n").arg(_experience); 
	ret += QString("Attack %1 \n").arg(_attack); 
	ret += QString("Defense %1 \n").arg(_defense); 
	ret += QString("Move %1 \n").arg(_move); 

	return ret;
}

void GenericLord::printLordData()
{

	logDD(" id %d", _id );
	logDD(" model %p", _model );
	logDD(" move %d", _move );
	logDD(" maxMove %d", _maxMove );
	logDD(" luck %d", _luck);
	logDD(" experience %d", _experience );
	logDD(" attack %d", _attack );
	logDD(" defense %d", _defense );
	logDD(" hasBook %d", _hasBook );
	logDD(" vision %d", _vision );
	logDD(" level %d", _level );

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] ) {
			logDD( " units %p", _units[i]);
		}
	}
}

/*!

*/

bool GenericLord::addCreatures( Creature * creature, int number )
{
	bool ret = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = _units[i];
		if( unit == 0 ) {
			unit = new GenericFightUnit( creature );
			unit->setNumber( number );
			_units[i] = unit;
			ret = true;
			break;
		} else if( ( unit->getLevel() == creature->getLevel() )
			&& ( unit->getRace() == creature->getRace() ) ) {
			unit->addNumber( number );
			ret = true;
			break;
		}
	}

	return ret;
}

void GenericLord::updateUnit( uchar pos ,uchar race, uchar level,	int nb, uchar move, int health)
{
GenericFightUnit * uni = getUnit( pos );
	
	if( nb != 0 ){
		if( !uni ){
			uni = new GenericFightUnit();
		}

		//XXX bugged?
		uni->setCreature( race, level );

		if(health > uni->getMaxHealth()) {
			logEE("bug, health %d, maxhealth %d", health, uni->getMaxHealth());
			delete uni;
			return;
		}	
		
		uni->setNumber( nb );
		uni->setMove( move );
		uni->setHealth( health );

	}	else {
		if( uni ) {
			delete uni;
			uni = NULL;
		}
	}

	setUnit( pos, uni );
}

int GenericLord::countUnits()
{
int num=0;
for( int i = 0; i < MAX_UNIT; i++ ) {
	if( _units[i] ) {
		num++;
	}
}
return num;
}



bool GenericLord::load( QTextStream * )
{
	/// XXX: no args ?
	return true;
}

void GenericLord::save( QTextStream * ts, int indent )
{
	uint i;

	indentation( ts, indent );
	*ts << "<lord id=\"" << _id << "\">" << endl;

	if( _currentCell ) {
		indentation( ts, indent );
		*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

		indentation( ts, indent );
		*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;
	}

	for( i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] ) {
			indentation( ts, indent );
			*ts << "\t<unit pos=\"" << i << "\">" << endl;
			indentation( ts, indent );
			*ts << "\t\t<race>" << _units[i]->getRace() << "</race>" << endl;
			indentation( ts, indent );
			*ts << "\t\t<level>" << _units[i]->getLevel() << "</level>" << endl;
			indentation( ts, indent );
			*ts << "\t\t<number>" << _units[i]->getNumber() << "</number>" << endl;
			indentation( ts, indent );
			*ts << "\t\t<health>" << _units[i]->getHealth() << "</health>" << endl;
			indentation( ts, indent );
			*ts << "\t</unit>" << endl;
		}
	}

	_artefactManager->save( ts, indent + 1 );

	/**
		Here we will call "May day!! :) characteristics are upon us"
		*/

	LordCharac characIndex = ATTACK;
	for( int index = (int)ATTACK; index < (int)LAST; ++index ) {
		characIndex = (LordCharac)index;
		if( _model->getBaseCharac( characIndex ) != getBaseCharac( characIndex )  ||  characIndex == MOVE  ) {
			indentation( ts, indent+1 );
			*ts << "<characteristic type=\"" << getCharacName( characIndex ) << "\">";
			*ts << getBaseCharac( characIndex );
			*ts << "</characteristic>" << endl;
		}
	}

	for( int j = 0; j < _machines.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<machine>" << _machines.at( j ) << "</machine>" << endl;
	}

	indentation( ts, indent );
	*ts << "</lord>" << endl;
	*ts << flush;
}

int GenericLord::getCharac( LordCharac type )
{
/// XXX: compute with artefacts, skills...
int ret = 0;
ret = getBaseCharac( type );
ret = _artefactManager->computeCharacModif( type, ret );

return ret;
}

int GenericLord::getBaseCharac( LordCharac type )
{
int ret = 0;
switch( type ) {
	case ATTACK:
		ret = _attack;
		break;
	case DEFENSE:
		ret = _defense;
		break;
	case MOVE:
		ret = _move;
		break;
	case MAXMOVE:
		ret = _maxMove;
		break;
	case LUCK:
		ret = _luck;
		break;
	case VISION:
		ret = _vision;
		break;
	case EXPERIENCE:
		ret = _experience;
		break;
	case LEVEL:
		ret = _level;
		break;
	default:
		break;
}

return ret;
}

void GenericLord::setBaseCharac( LordCharac type, int value )
{
switch( type ) {
	case ATTACK:
		_attack = value;
		break;
	case DEFENSE:
		_defense = value;
		break;
	case MOVE:
		_move = value;
		break;
	case MAXMOVE:
		_maxMove = value;
		break;
	case LUCK:
		_luck = value;
		break;
	case VISION:
		_vision = value;
		break;
	case EXPERIENCE:
		_experience = value;
		break;
	case LEVEL:
		_level = value;
		break;
	default:
		break;
}
}

void GenericLord::increaseBaseCharac( LordCharac type, int value )
{
switch( type ) {
	case ATTACK:
		_attack += value;
		break;
	case DEFENSE:
		_defense += value;
		break;
	case MOVE:
		_move += value;
		break;
	case MAXMOVE:
		_maxMove += value;
		break;
	case LUCK:
		_luck += value;
		break;
	case VISION:
		_vision += value;
		break;
	case EXPERIENCE:
		_experience += value;
		break;
	case LEVEL:
		_level += value;
		break;
	default:
		break;
}
}

void GenericLord::decreaseBaseCharac( LordCharac type, int value )
{
switch( type ) {
	case ATTACK:
		if( value < _attack ) {
			_attack -= value;
		} else {
			_attack = 0;
		}
		break;
	case DEFENSE:
		if( value < _defense ) {
			_defense -= value;
		} else {
			_defense = 0;
		}
		break;
	case MOVE:
		if( value < _move ) {
			_move -= value;
		} else {
			_move = 0;
		}
		break;
	case MAXMOVE:
		if( value < _maxMove ) {
			_maxMove -= value;
		} else {
			_maxMove = 0;
		}
		break;
	case LUCK:
		if( value < _luck ) {
			_luck -= value;
		} else {
			_luck = 0;
		}
		break;
	case VISION:
		if( value < _vision ) {
			_vision -= value;
		} else {
			_vision = 0;
		}
		break;
	case EXPERIENCE:
		if( value < _experience ) {
			_experience -= value;
		} else {
			_experience = 0;
		}
		break;		
	case LEVEL:
		if( value < _level ) {
			_level -= value;
		} else {
			_level = 0;
		}
		break;
	default:
		break;
}
}

void GenericLord::removeFromGame()
{
	TRACE("GenericLord::removeFromGame() id %d", getId());
	setOwner( NULL );

	if( getCell() ) {
		getCell()->setLord( NULL );
		GenericBase * base = getCell()->getBase();
		if( getCell()->getBase() ) {
			if( base->getGarrisonLord() == this ) {
				base->removeGarrisonLord();
			}
			if( base->getVisitorLord() == this ) {
				base->removeVisitorLord();
			}
		}
		_currentCell = NULL;
	}
}

bool GenericLord::isGarrison()
{		
	GenericBase * base = getCell()->getBase();
		
	if( base != 0 ) {
			if( base->getGarrisonLord() == this ) {
				return true;
			}
	}
	return false;
}

void GenericLord::addMachine( int id )
{
_machines.append( id );
}

uint GenericLord::getMachineNumber()
{
uint ret = _machines.count();

return ret;
}

uint GenericLord::getMachine( uint num )
{
uint ret = 0;

if( (int)num < _machines.count() ) {
	ret = _machines.at( num );
}

return ret;
}

void GenericLord::removeMachine( uint num )
{
_machines.removeAt( num );
}

bool GenericLord::hasMachine( int id )
{
bool ret = false;

for( int i = 0; i < _machines.count(); i++ ) {
	if( _machines.at( i ) == id ) {
		ret = true;
	}
}

return ret;
}

bool GenericLord::hasMachineType( int type )
{
bool ret = false;

for( int i = 0; i < _machines.count(); i++ ) {
	int id = _machines.at( i );
	WarMachine * machine = DataTheme.machines.at( id );
	if( machine ) {
		if( machine->getType() == type ) {
			ret = true;
		}
	}
}

return ret;
}

uint GenericLord::computeForceIndicator( bool real)
{
uint ret = 0;
uint number = 0;

GenericFightUnit * unit;
for( int i = 0; i < MAX_UNIT; ++i ) {
	unit = _units[i];
	if( unit ) {
		if( real ) {
			number = unit->getNumber();
		} else {
			number = DataTheme.getRandomInCategory( (uchar)DataTheme.computeCategory( unit->getNumber() ) );
		}
		ret += unit->getMaxHealth() * number;
	}
}

return ret;
}

uint GenericLord::getCost( uint res )
{
uint ret = 0;

if( (int)res < DataTheme.resources.count() ) {
	ret = _cost[res];
}

return ret;
}

void GenericLord::setCost( uint res, uint cost )
{
if( (int)res < DataTheme.resources.count() ) {
	_cost[res] = cost;
}

}

uint GenericLord::computeCreatureMaxManaged()
{
//XXX TODO
return MAX_UNIT;
}

//
// ----- GenericLordModel -----
//

GenericLordModel::GenericLordModel()
: _name( "" ),
_category( 0 ),
_move( 0 ),
_maxMove( 0 ),
_technicPoints( 0 ),
_maxTechnicPoints( 0 ),
_morale( 0 ),
_luck( 0 ),
_experience( 0 ),
_power( 0 ),
_knowledge( 0 ),
_attack( 0 ),
_defense( 0 ),
_knowledgePoints( 0 ),
_vision( 0 ),
_charisma( 0 ),
_level( 0 )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i] = 0;
	}
	for( int i = 0; i < MAX_RESS; i++ ) {
		_cost[i] = 0;
	}
}

GenericLordModel::~GenericLordModel()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if(_units[i]) {
			delete _units[i];
			_units[i] = 0;
		}
	}
}

uint GenericLordModel::getRace()
{
	uint ret = 0;

	if( _category ) {
		ret = _category->getRace();
	}

	return ret;
}


void GenericLordModel::setBaseCharac( LordCharac type, int value )
{
	switch( type ) {
		case ATTACK:
			_attack = value;
			break;
		case DEFENSE:
			_defense = value;
			break;
		case POWER:
			break;
		case KNOWLEDGE:
			break;
		case MOVE:
			_move = value;
			break;
		case MAXMOVE:
			_maxMove = value;
			break;
		case TECHNICPOINT:
			break;
		case MAXTECHNICPOINT:
			break;
		case MORALE:
			break;
		case LUCK:
			_luck = value;
			break;
		case VISION:
			_vision = value;
			break;
		case EXPERIENCE:
			_experience = value;
			break;
		case CHARISMA:
			break;
		case LEVEL:
			_level = value;
			break;
		default:
			break;
	}
}

int GenericLordModel::getBaseCharac( LordCharac type )
{
	int ret = 0;
	switch( type ) {
		case ATTACK:
			ret = _attack;
			break;
		case DEFENSE:
			ret = _defense;
			break;
		case POWER:
			break;
		case KNOWLEDGE:
			break;
		case MOVE:
			ret = _move;
			break;
		case MAXMOVE:
			ret = _maxMove;
			break;
		case TECHNICPOINT:
			break;
		case MAXTECHNICPOINT:
			break;
		case MORALE:
			break;
		case LUCK:
			ret = _luck;
			break;
		case VISION:
			ret = _vision;
			break;
		case EXPERIENCE:
			ret = _experience;
			break;
		case CHARISMA:
			break;
		case LEVEL:
			ret = _level;
			break;
		default:
			break;
	}

	return ret;
}

void GenericLordModel::addMachine( int id )
{
	_machines.append( id );
}

uint GenericLordModel::getMachineNumber()
{
	uint ret = _machines.count();

	return ret;
}

uint GenericLordModel::getMachine( uint num )
{
	uint ret = 0;

	if( (int)num < _machines.count() ) {
		ret = _machines.at( num );
	}

	return ret;
}

int GenericLordModel::countUnits()
{
	int num = 0;
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] != 0 ) {
			num++;
		}
	}
	return num;
}

uint GenericLordModel::getCost( uint res )
{
	uint ret = 0;

	if( (int)res < DataTheme.resources.count() ) {
		ret = _cost[res];
	}

	return ret;
}

void GenericLordModel::setCost( uint res, uint cost )
{
	if( (int)res < DataTheme.resources.count() ) {
		_cost[res] = cost;
	}

}

QString GenericLordModel::getLordDescription()
{
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();
	QString text = "";
	
	for( uint i = 0; i < nbRes; ++i ) {
		if( getCost( i ) != 0 ) {
			if( text != "" ) {
				text += " + ";
			}
			text += QString::number( getCost( i ) )
				+ " " + themeResources.getRessource( i );
		}
	}
	if( text == "" ) {
		text = "0";
	}
	text =  "Cost: "  + text;
	text +=  "\n" ;
	return text;

}
	

void GenericLordModel::removeMachine( uint num )
{
	_machines.removeAt( num );
}

void GenericLordModel::save( QTextStream & ts, int indent )
{
	uint j;

	indentation( &ts, indent );
	ts << "<lord category=\"" << DataTheme.getLordCategory( _category ) << "\">" << endl;
	indentation( &ts, indent + 1 );
	ts << "<name>" << getName() << "</name>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"attack\">" << getBaseCharac( ATTACK ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"defense\">" << getBaseCharac( DEFENSE ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"movemax\">" << getBaseCharac( MAXMOVE ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"vision\">" << getBaseCharac( VISION ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"level\">" << getBaseCharac( LEVEL ) << "</characteristic>" << endl;
	for( int  k = 0; k < DataTheme.resources.count(); k++ ) {
		if( getCost( k ) > 0 ) {
			indentation( &ts, indent );
			ts << "\t<cost resource=\"" << k << "\">";
			ts << getCost( k ) << "</cost>" << endl;
		}
	}


	for( j = 0; j < MAX_UNIT; j++ ) {
		GenericFightUnit * unit = getUnit( j );
		if( unit ) {
			indentation( &ts, indent + 1 );
			ts << "<unit>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<race>" << unit->getRace() << "</race>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<level>" << unit->getLevel() << "</level>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<number>" << unit->getNumber() << "</number>" << endl;
			indentation( &ts, indent + 1 );
			ts << "</unit>" << endl;
		} else {
			indentation( &ts, indent + 1 );
			ts << "<unit>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<race>0</race>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<level>0</level>" << endl;
			indentation( &ts, indent + 2 );
			ts << "<number>0</number>" << endl;
			indentation( &ts, indent + 1 );
			ts << "</unit>" << endl;
		}
	}

	for( j = 0; j < getMachineNumber(); j++ ) {
		indentation( &ts, indent + 1 );
		ts << "<machine>" << getMachine( j ) << "</machine>" << endl;
	}
	indentation( &ts, indent );
	ts << "</lord>" << endl;
}


//
// ----- LordList -----
//

LordList::LordList()
{
	
}

LordList::~LordList()
{
	clear();
}

bool LordList::init()
{
	clear();
	LordHandler handler( this );
	QFile file( DATA_PATH + "lords.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "lords.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool LordList::save()
{
	QString filename = DATA_PATH + "lords.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE lords>" << endl;
	ts << "<lords>" << endl;

	GenericLordModel * lord;
	for( int i = 1; i < count(); i++ ) {
		lord = at( i );
		if( lord ) {
			lord->save( ts, 1 );
		}
	}

	ts << "</lords>" << endl;

	file.close();

	return true;
}

void LordList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

//
// ----- LordHandler -----
//

LordHandler::LordHandler( LordList * list )
{
	_list = list;
}

bool LordHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	GenericLordModel * lord = new GenericLordModel();
	_list->append( lord );

	_state = StateInit;
	return true;
}

bool LordHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "lords" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "lord" && _state == StateDocument ) {
		_state = StateLord;
		_lord = new GenericLordModel();
		_lord->setCategory( DataTheme.lordCategories.at( atts.value( "category" ).toUInt() ) );
		_numUnit = 0;
	} else if ( qName == "name" && _state == StateLord ) {
		_state = StateName;
	} else if( qName == "characteristic" && _state == StateLord ) {
		_state = StateCharac;
		_charac = detectCharac( atts.value( "type" ) );
	} else if( qName == "cost" && _state == StateLord ) {
		_state = StateCost;
		_resource = atts.value( "resource" ).toInt();
	} else if ( qName == "unit" && _state == StateLord ) {
		_state = StateUnit;
		_unit = new GenericFightUnit();
		_race = 0;
		_level = 0;
	} else if ( qName == "race" && _state == StateUnit ) {
		_state = StateUnitRace;
	} else if ( qName == "level" && _state == StateUnit ) {
		_state = StateUnitLevel;
	} else if ( qName == "number" && _state == StateUnit ) {
		_state = StateNumber;
	} else if( qName == "machine" && _state == StateLord ) {
		_state = StateMachine;
	} else {
		// error
		return false;
	}
	return true;
}

bool LordHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateLord:
		_state = StateDocument;
		_list->append( _lord );
		break;
	case StateName:
		_state = StateLord;
		break;
	case StateCharac:
		_state = StateLord;
		break;
	case StateCost:
		_state = StateLord;
		break;
	case StateUnit:
		_state = StateLord;
		_unit->setCreature( _race, _level );
		if( _unit->getNumber() > 0 ) {
			_lord->setUnit( _numUnit, _unit );
		} else {
			_lord->setUnit( _numUnit, 0 );
			delete _unit;
		}
		_numUnit++;
		break;
	case StateUnitRace:
		_state = StateUnit;
		break;
	case StateUnitLevel:
		_state = StateUnit;
		break;
	case StateNumber:
		_state = StateUnit;
		break;
	case StateMachine:
		_state = StateLord;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool LordHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_lord->setName( ch_simplified );
		break;
	case StateCharac:
		_lord->setBaseCharac( _charac, ch_simplified.toInt() );
		break;
	case StateCost:
		_lord->setCost( _resource, ch_simplified.toInt() );
		break;
	case StateUnitRace:
		_race = ch_simplified.toInt();
		break;
	case StateUnitLevel:
		_level = ch_simplified.toInt();
		break;
	case StateNumber:
		_unit->setNumber( ch_simplified.toInt() );
		break;
	case StateMachine:
		_lord->addMachine( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool LordHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}


