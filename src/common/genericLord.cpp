/****************************************************************
**
** Attal : Lords of Doom
**
** genericLord.cpp
** manages a lord and its units
**
** Version : $Id: genericLord.cpp,v 1.46 2003/04/25 18:59:33 audoux Exp $
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

#include "genericLord.h"


// generic include files
#include <stdlib.h>
// include files for QT
#include <qtextstream.h>
// application specific include files
#include "common/artefact.h"
#include "common/dataTheme.h"
#include "common/genericPlayer.h"
#include "common/log.h"
#include "common/pathFinder.h"
#include "common/unit.h"
#include "common/warMachine.h"
#include "common/categoryManager.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;

LordCharac detectCharac( QString type )
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
	} else {
		logEE( "Unknown Lord's Characteristic: %s", type.latin1() );
	}

	return ret;
}

//
// ----- GenericLord -----
//

/** add comments here */
GenericLord::GenericLord()
	: _id( 0 ),
	  _player( 0 ),
	  _currentCell( 0 ),
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
	  _vision( 0 )

{
	_visible = true;
	_sleeping = false;
	
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i] = 0;
	}

	_machines.setAutoDelete( true );
	_model = 0;
}

GenericLord::~GenericLord()
{

}

void GenericLord::reinit()
{
	setId( _id );
	_player = 0;
	_currentCell = 0;
}

int GenericLord::numArtefact()
{
	return _artefacts.count();
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
		ret = _model->getCategory();
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

void GenericLord::setId( int id )
{
	_id = id;
	if( (uint)_id >= DataTheme.lords.count() ) {
		logEE( "Lord id %d doesn't exist", _id );
		_id = 0;
		_model = 0;
		return;
	}

	_model = DataTheme.lords.at( _id );
	_move = _model->getBaseCharac( MOVE );
	_maxMove = _model->getBaseCharac( MAXMOVE );
	_technicPoints = _model->getBaseCharac( TECHNICPOINT );
	_maxTechnicPoints = _model->getBaseCharac( MAXTECHNICPOINT );
	_morale = _model->getBaseCharac( MORALE );
	_luck = _model->getBaseCharac( LUCK );
	_experience = 0;
	_power = _model->getBaseCharac( POWER );
	_knowledge = _model->getBaseCharac( KNOWLEDGE );
	_attack = _model->getBaseCharac( ATTACK );
	_defense = _model->getBaseCharac( DEFENSE );
	_hasBook = _model->isBook();
	_vision = _model->getBaseCharac( VISION );

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] ) {
			delete _units[i];
			_units[i] = _model->getUnit( i );;
		}
	}

	_machines.clear();
	for( int j = 0; j < _model->getMachineNumber(); j++ ) {
		addMachine( getMachine( j ) );
	}
}

void GenericLord::moveTo( GenericCell * cell )
{
	getOut();
	if( _currentCell ) {
		_currentCell->setLord( NULL );
	}

	_currentCell = cell ;
	cell->setLord( (GenericLord *)this );
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

/*!

*/

bool GenericLord::addCreatures( Creature * creature, int number )
{
	bool ret = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = _units[i];
		if( unit == 0 ) {
			unit = new GenericFightUnit();
			unit->setCreature( creature );
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

void GenericLord::clearUnits()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] != 0 ) {
			delete _units[i];
			_units[i] = 0;
		}
	}
}
	

int GenericLord::countUnits()
{
	int num=0;
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _units[i] != 0 ) {
			num++;
		}
	}
	return num;
}
	
void GenericLord::addArtefact( GenericArtefact * artefact )
{
	_artefacts.append( artefact );
}

GenericArtefact * GenericLord::getArtefact( int num )
{
	return _artefacts.at( num );
}

void GenericLord::removeArtefact( int num )
{
	_artefacts.remove( num );
}

void GenericLord::removeArtefact( GenericArtefact * artefact )
{
	_artefacts.remove( artefact );
}

void GenericLord::clearArtefacts()
{
	_artefacts.clear();
}

bool GenericLord::load( QTextStream * ts )
{
	
	return true;
}

void GenericLord::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<lord id=\"" << _id << "\">" << endl;

	if( _currentCell ) {
		indentation( ts, indent );
		*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

		indentation( ts, indent );
		*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;
	}

	for( int i = 0; i < MAX_UNIT; i++ ) {
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
			*ts << "\t</unit>" << endl;
		}
	}

	for( int j = 0; j < _machines.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<machine>" << _machines.at( j ) << "</machine>" << endl;
	}

	indentation( ts, indent );
	*ts << "</lord>" << endl;
}

int GenericLord::getLevel()
{
	/// XXX: TODO
	return 3;
}

int GenericLord::getCharac( LordCharac type )
{
	/// XXX: compute with artefcat, skills...
	int ret = 0;
	switch( type ) {
		case ATTACK:
			ret = _attack;
			break;
		case DEFENSE:
			ret = _defense;
			break;
		case POWER:
			ret = _power;
			break;
		case KNOWLEDGE:
			ret = _knowledge;
			break;
		case MOVE:
			ret = _move;
			break;
		case MAXMOVE:
			ret = _maxMove;
			break;
		case TECHNICPOINT:
			ret = _technicPoints;
			break;
		case MAXTECHNICPOINT:
			ret = _maxTechnicPoints;
			break;
		case MORALE:
			ret = _morale;
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
	}

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
		case POWER:
			ret = _power;
			break;
		case KNOWLEDGE:
			ret = _knowledge;
			break;
		case MOVE:
			ret = _move;
			break;
		case MAXMOVE:
			ret = _maxMove;
			break;
		case TECHNICPOINT:
			ret = _technicPoints;
			break;
		case MAXTECHNICPOINT:
			ret = _maxTechnicPoints;
			break;
		case MORALE:
			ret = _morale;
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
		case POWER:
			_power = value;
			break;
		case KNOWLEDGE:
			_knowledge = value;
			break;
		case MOVE:
			_move = value;
			break;
		case MAXMOVE:
			_maxMove = value;
			break;
		case TECHNICPOINT:
			_technicPoints = value;
			break;
		case MAXTECHNICPOINT:
			_maxTechnicPoints = value;
			break;
		case MORALE:
			_morale = value;
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
		case POWER:
			_power += value;
			break;
		case KNOWLEDGE:
			_knowledge += value;
			break;
		case MOVE:
			_move += value;
			break;
		case MAXMOVE:
			_maxMove += value;
			break;
		case TECHNICPOINT:
			_technicPoints += value;
			break;
		case MAXTECHNICPOINT:
			_maxTechnicPoints += value;
			break;
		case MORALE:
			_morale += value;
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
		case POWER:
			if( value < _power ) {
				_power -= value;
			} else {
				_power = 0;
			}
			break;
		case KNOWLEDGE:
			if( value < _knowledge ) {
				_knowledge -= value;
			} else {
				_knowledge = 0;
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
		case TECHNICPOINT:
			if( value < _technicPoints ) {
				_technicPoints -= value;
			} else {
				_technicPoints = 0;
			}
			break;
		case MAXTECHNICPOINT:
			if( value < _maxTechnicPoints ) {
				_maxTechnicPoints -= value;
			} else {
				_maxTechnicPoints = 0;
			}
			break;
		case MORALE:
			if( value < _morale ) {
				_morale = value;
			} else {
				_morale = 0;
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
	}
}

int GenericLord::computeCostMvt( GenericCell * cell )
{
	return computeCostMvt( _currentCell, cell );
}

int GenericLord::computeCostMvt( GenericCell * start, GenericCell * end )
{
	int ret = -1;

	int r1 = start->getRow();
	int r2 = end->getRow();
	int c1 = start->getCol();
	int c2 = end->getCol();

	if( ( abs( c1 - c2 ) < 2 ) && ( abs( r1 - r2 ) < 2 ) ) {
		if( c1 != c2 ) {
			if( r1 != r2 ) {
				ret = COST_DIAG;
			} else {
				ret = COST_DIRECT;
			}
		} else {
			if( r1 != r2 ) {
				ret = COST_DIRECT;
			}
		}
	}

	return ret;
}

void GenericLord::removeFromGame()
{
	if( getOwner() ) {
		getOwner()->removeLord( this );
		setOwner( 0 );
	}
	if( getCell() ) {
		getCell()->setLord( 0 );
		GenericBase * base = getCell()->getBase();
		if( getCell()->getBase() ) {
			if( base->getGarrisonLord() == this ) {
				base->setGarrisonLord( 0 );
			}
			if( base->getVisitorLord() == this ) {
				base->setVisitorLord( 0 );
			}
		}
		_currentCell = 0;
	}
}

void GenericLord::addMachine( int id )
{
	_machines.append( new int( id ) );
}

uint GenericLord::getMachineNumber()
{
	uint ret = _machines.count();

	return ret;
}

uint GenericLord::getMachine( uint num )
{
	uint ret = 0;

	if( num < _machines.count() ) {
		ret = * ( _machines.at( num ) );
	}

	return ret;
}

void GenericLord::removeMachine( uint num )
{
	_machines.remove( num );
}

bool GenericLord::hasMachine( int id )
{
	bool ret = false;

	for( uint i = 0; i < _machines.count(); i++ ) {
		if( ( *( _machines.at( i ) ) ) == id ) {
			ret = true;
		}
	}

	return ret;
}

bool GenericLord::hasMachineType( int type )
{
	bool ret = false;

	for( uint i = 0; i < _machines.count(); i++ ) {
		int id = *( _machines.at( i ) );
		WarMachine * machine = DataTheme.machines.at( id );
		if( machine ) {
			if( machine->getType() == type ) {
				ret = true;
			}
		}
	}

	return ret;
}

//
// ----- GenericLordModel -----
//

GenericLordModel::GenericLordModel()
	: _race( 0 ),
	_name( "" ),
	_category( "" ),
	_move( 0 ),
	_maxMove( 0 ),
	_technicPoints( 0 ),
	_maxTechnicPoints( 0 ),
	_morale( 0 ), 
	_luck( 0 ),
	_power( 0 ), 
	_knowledge( 0 ),
	_attack( 0 ),
	_defense( 0 ),
	_knowledgePoints( 0 ),
	_vision( 0 )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i] = 0;
	}
	_machines.setAutoDelete( true );
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
			_power = value;
			break;
		case KNOWLEDGE:
			_knowledge = value;
			break;
		case MOVE:
			_move = value;
			break;
		case MAXMOVE:
			_maxMove = value;
			break;
		case TECHNICPOINT:
			_technicPoints = value;
			break;
		case MAXTECHNICPOINT:
			_maxTechnicPoints = value;
			break;
		case MORALE:
			_morale = value;
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
			ret = _power;
			break;
		case KNOWLEDGE:
			ret = _knowledge;
			break;
		case MOVE:
			ret = _move;
			break;
		case MAXMOVE:
			ret = _maxMove;
			break;
		case TECHNICPOINT:
			ret = _technicPoints;
			break;
		case MAXTECHNICPOINT:
			ret = _maxTechnicPoints;
			break;
		case MORALE:
			ret = _morale;
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
	}

	return ret;
}

void GenericLordModel::addMachine( int id )
{
	_machines.append( new int( id ) );
}

uint GenericLordModel::getMachineNumber()
{
	uint ret = _machines.count();

	return ret;
}

uint GenericLordModel::getMachine( uint num )
{
	uint ret = 0;

	if( num < _machines.count() ) {
		ret = * ( _machines.at( num ) );
	}

	return ret;
}

void GenericLordModel::removeMachine( uint num )
{
	_machines.remove( num );
}

void GenericLordModel::save( QTextStream & ts, int indent )
{
	int j;

	indentation( &ts, indent );
	ts << "<lord name=\"" << getName() << "\">" << endl;
	indentation( &ts, indent + 1 );
	ts << "<race>" << DataTheme.creatures.getRace( getRace() )->getName() << "</race>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<category>" << getCategory() << "</category>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"attack\">" << getBaseCharac( ATTACK ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"defense\">" << getBaseCharac( DEFENSE ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"power\">" << getBaseCharac( POWER ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"knowledge\">" << getBaseCharac( KNOWLEDGE ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"movemax\">" << getBaseCharac( MAXMOVE ) << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<characteristic type=\"vision\">" << getBaseCharac( VISION ) << "</characteristic>" << endl;

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
	setAutoDelete( true );

}

bool LordList::init()
{
	clear();
	LordHandler handler( this );
	QFile file( DATA_PATH + "lords.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "lords.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}
	
	return true;
}

bool LordList::save()
{
	QString filename = DATA_PATH + "lords.dat";
	QFile file( filename );

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE lords>" << endl;
	ts << "<lords>" << endl;

	GenericLordModel * lord;
	for( uint i = 1; i < count(); i++ ) {
		lord = at( i );
		if( lord ) {
			lord->save( ts, 1 );
		}
	}

	ts << "</lords>" << endl;

	file.close();

	return true;
}

/***************************************************************************/

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

bool LordHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "lords" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "lord" && _state == StateDocument ) {
		_state = StateLord;
		_lord = new GenericLordModel();
		_lord->setName( atts.value( "name" ) );
		_numUnit = 0;
	} else if ( qName == "race" && _state == StateLord ) {
		_state = StateRace;
	} else if ( qName == "category" && _state == StateLord ) {
		_state = StateCategory;
	} else if( qName == "characteristic" && _state == StateLord ) {
		_state = StateCharac;
		_charac = detectCharac( atts.value( "type" ) );
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

bool LordHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateLord:
		_state = StateDocument;
		_list->append( _lord );
		break;
	case StateRace:
		_state = StateLord;
		break;
	case StateCategory:
		_state = StateLord;
		break;
	case StateCharac:
		_state = StateLord;
		break;
	case StateUnit:
		_state = StateLord;
		_unit->setCreature( _race, _level );
		if( _unit->getNumber() > 0 ) {
			_lord->setUnit( _numUnit, _unit );
		} else {
			_lord->setUnit( _numUnit, 0 );
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
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateRace:
		_lord->setRace( DataTheme.creatures.giveNumRace( ch_simplified ) );
		break;
	case StateCategory:
		_lord->setCategory( ch_simplified );
		break;
	case StateCharac:
		_lord->setBaseCharac( _charac, ch_simplified.toInt() );
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


