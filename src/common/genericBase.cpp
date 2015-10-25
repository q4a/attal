/****************************************************************
**
** Attal : Lords of Doom
**
** genericBase.cpp
** in order to manage buildings...
**
** Version : $Id: genericBase.cpp,v 1.38 2003/04/10 15:36:47 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/08/2000
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

#include "genericBase.h"

 
// generic include files
// include files for QT
#include <qtextstream.h>
#include <qstringlist.h>
// application specific include files
#include "common/action.h"
#include "common/creature.h"
#include "common/dataTheme.h"
#include "common/genericCell.h"
#include "common/genericInsideBuilding.h"
#include "common/genericPlayer.h"
#include "common/log.h"
#include "common/unit.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;


//
// ----- GenericBase -----
//

GenericBase::GenericBase()
	: _id( 0 ),
	  _race( 0 ),
	 _name(""),
	 _player( 0 )
{
	_canBuild = true;
	_currentCell = 0;
	_visitorLord = 0;
	_garrisonLord = 0;
	_population = 0;

	for( int i = 0; i < MAX_UNIT ; i++ ) {
		_units[i] = 0;
	}

	_buildings.setAutoDelete( true );
	_forbidden.setAutoDelete( true );
}

GenericBase::~GenericBase()
{

}

void GenericBase::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<base>" << endl;

	indentation( ts, indent );
	*ts << "\t<type>" << _race << "</type>" << endl;

	indentation( ts, indent );
	*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

	indentation( ts, indent );
	*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;

	for( uint i = 0; i < _buildings.count(); i++ ) {
		indentation( ts, indent );
		*ts << "\t<building>" << _buildings.at( i )->getLevel() << "</building>" << endl;
	}

	for( uint j = 0; j < _forbidden.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<forbidden>" << * ( _forbidden.at( j ) ) << "</forbidden>" << endl;
	}

	indentation( ts, indent );
	*ts << "</base>" << endl;
}



void GenericBase::setPosition( GenericCell * cell )
{
	if( _currentCell != 0 ) {
		_currentCell->setBase( 0 );
	}
	_currentCell = cell;
	_currentCell->setBase( this );
}

/*!
  compute the gold earned by the castle...
*/

int GenericBase::collectRessource( int num )
{
	int ret = 0;
	switch( num ) {
	case 0 :
		ret = 0;
		break;
	}
	return ret;
}

bool GenericBase::load( QTextStream * ts )
{
	return true;
}

void GenericBase::exchangeLords()
{
	GenericLord * temp = _visitorLord;
	_visitorLord = _garrisonLord;
	_garrisonLord = temp;
}

int GenericBase::getDoorRow()
{
	if( (uint)_race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDoorRow();
	} else {
		return 0;
	}
}

int GenericBase::getDoorCol()
{
	if( (uint)_race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDoorCol();
	} else {
		return 0;
	}
}

void GenericBase::getInitPopul()
{

	if( (uint)_race < DataTheme.bases.count() ) {
		_population = DataTheme.bases.at( _race )->getPopulation();
	} else {
		_population = 0;
	}

}

int GenericBase::getDispo( int row, int col )
{
	if( (uint)_race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getDispo( row, col );
	} else {
		return 0;
	}
}

QString GenericBase::getModelName()
{
	if( (uint)_race < DataTheme.bases.count() ) {
		return DataTheme.bases.at( _race )->getModelName();
	} else {
		return "";
	}			
}

void GenericBase::addBuilding( GenericInsideBuilding * build )
{
	if( ! _buildings.containsRef( build ) ) {
		_buildings.append( build );
	}
}

void GenericBase::addBuilding( int level )
{
	GenericInsideBuilding * building = new GenericInsideBuilding();
	building->setRace( _race );
	building->setLevel( level );
	addBuilding( building );
}

GenericInsideBuilding * GenericBase::getBuilding( uint num )
{
	GenericInsideBuilding * ret = 0;

	if( num < getBuildingCount() ) {
		return _buildings.at( num );
	}

	return ret;
}

GenericInsideBuilding * GenericBase::getBuildingByType( uint type )
{
	GenericInsideBuilding * ret = 0;

	for( int i = 0; i < (int)_buildings.count(); i++ ) {
		if( _buildings.at( i )->getLevel() == type ) {
			ret = _buildings.at( i );
		}
	}

	return ret;
}

bool GenericBase::hasBuildingType( int type )
{
	bool ret = false;

	for( uint i = 0; i < _buildings.count(); i++ ) {
		if( _buildings.at( i )->getLevel() == type ) {
			ret = true;
			i =  _buildings.count();
		}
	}

	return ret;
}

void GenericBase::removeBuildingType( int type )
{
	for( int i = 0; i < (int)_buildings.count(); i++ ) {
		if( _buildings.at( i )->getLevel() == type ) {
			_buildings.remove( i );
			i = -1;
		}
	}
}

void GenericBase::removeBuilding( GenericInsideBuilding * building )
{
	_buildings.remove( building );
}

void GenericBase::removeBuilding( int level )
{
	GenericInsideBuilding * building = new GenericInsideBuilding();
	building->setRace( _race );
	building->setLevel( level );
	removeBuilding( building );
}

uint GenericBase::getForbiddenBuildingsNumber()
{
	return _forbidden.count();
}

int GenericBase::getForbiddenBuilding( uint num )
{
	int ret = 0;

	if( num < _forbidden.count() ) {
		ret = * ( _forbidden.at( num ) );
	}

	return ret;
}

void GenericBase::addForbiddenBuilding( int num )
{
	_forbidden.append( new int(num) );
}

bool GenericBase::isForbidden( int num )
{
	bool ret = false;

	for( uint i = 0; i < _forbidden.count(); i++ ) {
		if( ( * ( _forbidden.at( i ) ) ) == num ) {
			ret = true;
		}
	}

	return ret;
}

int GenericBase::getCreatureProduction( Creature * creature )
{
	return _counter.getCreatureCount( creature );
}

void GenericBase::initCreatureProduction()
{
	_counter.clear();

	GenericBaseModel * baseModel = DataTheme.bases.at( _race );
	for( int i = 0; i < (int)_buildings.count(); i++ ) {
		GenericInsideBuilding * building = _buildings.at( i );
		InsideBuildingModel * model = baseModel->getBuildingModel( building->getLevel() );
		InsideAction * action = model->getAction();
		// XXX: change '3' in an enum...
		if( action ) {
			if( action->getType() == 3 ) {
				int race = action->getParam( 0 );
				int level = action->getParam( 1 );
				Creature * creature = DataTheme.creatures.at( race, level );
				logDD("param number %d", action->getParam( 2 ) );
				_counter.addCreature( creature, action->getParam( 2 ) );
			}
		}
	}
}

void GenericBase::buyCreature( Creature * creature, int number )
{
	_counter.decreaseCreature( creature, number );
}

bool GenericBase::canAddGarrison( Creature * creature )
{
	bool ret = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( isGarrisonLord() ) {
			GenericFightUnit * tempUnit = getGarrisonLord()->getUnit( i );
			if( ( tempUnit == 0 ) || ( tempUnit->getCreature() == creature ) ) {
				ret = true;
			}
		} else {
			if( ( getUnit( i ) == 0 ) || ( getUnit( i )->getCreature() == creature ) ) {
				ret = true;
			}
		}
	}

	return ret;
}

void GenericBase::addGarrison( Creature * creature, int number )
{
	if( isGarrisonLord() ) {
		getGarrisonLord()->addCreatures( creature, number );
	} else {
		for( int i = 0; i < MAX_UNIT; i++ ) {
			if( getUnit( i ) == 0 ) {
				GenericFightUnit * unit = new GenericFightUnit();
				unit->setCreature( creature );
				unit->setNumber( number );
				setUnit( i, unit );
				break;
			} else if( getUnit( i )->getCreature() == creature ) {
				GenericFightUnit * unit = getUnit( i );
				unit->addNumber( number );
				break;
			}
		}
	}
}

int GenericBase::getVision()
{
	int ret = 3;

	if( (uint)_race < DataTheme.bases.count() ) {
		ret = DataTheme.bases.at( _race )->getVision();
	}

	return ret;
}

//
// ----- GenericBaseModel -----
//

GenericBaseModel::GenericBaseModel()
: _race( 0 ), _doorRow( 0 ), _doorCol( 0 ), _vision( 3 )
{
	_buildings.setAutoDelete( true );
}

void GenericBaseModel::setDispo( int row, int col, int disp )
{
	if( ( row > 4 ) || ( row < 0 ) || ( col > 4 ) || (col < 0 ) ) {
		return;
	}

	_dispo[row][col] = disp;
	if( disp == 1 ) {
		_doorCol = col;
		_doorRow = row;
	}
}

void GenericBaseModel::addBuilding( InsideBuildingModel * building )
{
	_buildings.append( building );
}


InsideBuildingModel * GenericBaseModel::getBuildingModel( int num )
{
	return _buildings.at( num );
}

void GenericBaseModel::removeBuilding( int num )
{
	if( num < _buildings.count() ) {
		_buildings.remove( num );
	}
}

//
// ----- BaseList -----
//

BaseList::BaseList()
{
	setAutoDelete( true );
	_type.setAutoDelete( true );
}

bool BaseList::init()
{
	clear();
	BaseHandler handler( this );
	QFile file( DATA_PATH + "bases.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "bases.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

GenericBaseModel * BaseList::at( QString typeName )
{
	GenericBaseModel * ret = first();
	
	QListIterator<GenericBaseModel> ite( *this );
	
	for( ite.toFirst(); ite.current(); ++ite ) {
		if( ite.current()->getModelName() == typeName ) {
			ret = ite.current();
		}
	}	

	return ret;
}

// void BaseList::clear()
// {
// 	QList<GenericBaseModel>::clear();
// 	_type.clear();
// }

// int BaseList::addType( QString typeName )
// {
// 	QListIterator<QString> ite( _type );
// 	int cpt = 0, ret = -1;
	
// 	for( ite.toFirst(); ite.current(); ++ite ) {
// 		if( *(ite.current()) == typeName ) {
// 			ret = cpt;
// 			break;
// 		}
// 		cpt++;
// 	}
// 	if( ret == -1 ) {
// 		ret = cpt;
// 		_type.append( new QString( typeName ) );
// 	}
	
// 	return ret;	
// }


//
// ----- BaseHandler -----
//


BaseHandler::BaseHandler( BaseList * list )
{
	_list = list;
}

bool BaseHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	_race = 0;
	_level = 0;

	_state = StateInit;
	return true;
}

bool BaseHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "bases" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "base" && _state == StateDocument ) {
		_state = StateBase;
		_base = new GenericBaseModel();
		_base->setRace( _race );
	} else if( qName == "name" && _state == StateBase ) {
		_state = StateName;
	} else if( qName == "disposition" && _state == StateBase ) {
		_state = StateDisposition;
	} else if( qName == "population" && _state == StateBase ) {
		_state = StatePopulation;
	} else if( qName == "building" && _state == StateBase ) {
		_building = new InsideBuildingModel();
		_building->setRace( _race );
		_building->setLevel( _level );
		_level++;
		_state = StateBuilding;
	} else if( qName == "name" && _state == StateBuilding ) {
		_state = StateBuildingName;
	} else if( qName == "description" && _state == StateBuilding ) {
		_state = StateBuildingDescription;
	} else if( qName == "require" && _state == StateBuilding ) {
		_state = StateBuildingRequire;
	} else if( qName == "replace" && _state == StateBuilding ) {
		_state = StateBuildingReplace;
	} else if( qName == "cost" && _state == StateBuilding ) {
		if( atts.index( "ressource" ) != -1 ) {
			_ress = atts.value( "ressource" ).toInt();
		} else {
			_ress = 0;
		}
		_state = StateBuildingCost;
	} else if( qName == "mcost" && _state == StateBuilding ) {
		if( atts.index( "ressource" ) != -1 ) {
			_mress = atts.value( "ressource" ).toInt();
		} else {
			_mress = 0;
		}
		_state = StateBuildingMantCost;
	} else if( qName == "x" && _state == StateBuilding ) {
		_state = StateBuildingX;
	} else if( qName == "y" && _state == StateBuilding ) {
		_state = StateBuildingY;
	} else if( qName == "action" && _state == StateBuilding ) {
		_action = new InsideAction();
		if( atts.index( "type" ) != -1 ) {
			_action->setType( atts.value( "type" ).toInt() );
		} else {
			_action->setType( 0 );
		}
		_state = StateBuildingAction;
	} else if( qName == "param" && _state == StateBuildingAction ) {
		_state = StateBuildingActionParam;
	} else {
		// error
		return false;
	}
	return true;
}

bool BaseHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateBase:
		_list->append( _base );
		_race++;
		_level = 0;
		_state = StateDocument;
		break;
	case StateName:
		_state = StateBase;
		break;
	case StateDisposition:
		_state = StateBase;
		break;
	case StatePopulation:
		_state = StateBase;
		break;
	case StateBuilding:
		_base->addBuilding( _building );
		_state = StateBase;
		break;
	case StateBuildingName:
		_state = StateBuilding;
		break;
	case StateBuildingDescription:
		_state = StateBuilding;
		break;
	case StateBuildingRequire:
		_state = StateBuilding;
		break;
	case StateBuildingReplace:
		_state = StateBuilding;
		break;
	case StateBuildingCost:
		_state = StateBuilding;
		break;
	case StateBuildingMantCost:
		_state = StateBuilding;
		break;
	case StateBuildingX:
		_state = StateBuilding;
		break;
	case StateBuildingY:
		_state = StateBuilding;
		break;
	case StateBuildingAction:
		_state = StateBuilding;
		_building->setAction( _action );
		break;
	case StateBuildingActionParam:
		_state = StateBuildingAction;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool BaseHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_base->setModelName( ch_simplified );
		break;
	case StateDisposition:{
		QStringList li = QStringList::split( " ", ch_simplified );
		for( int i = 0; i < 5; i++ ) {
			for( int j = 0; j < 5; j++ ) {
				_base->setDispo( i, j, li[ i*5 + j ].toInt() );
			}
		}	}
		break;
	case StatePopulation:
		_base->setPopulation( ch_simplified.toInt() );
		break;
	case StateBuildingName:
		_building->setName( ch_simplified );
		break;
	case StateBuildingDescription:
		_building->setDescription( ch_simplified );
		break;
	case StateBuildingRequire:
		_building->addRequirement( ch_simplified.toInt() );
		break;
	case StateBuildingReplace:
		_building->setReplacement( ch_simplified.toInt() );
		break;
	case StateBuildingCost:
		_building->setCost( _ress, ch_simplified.toInt() );
		break;
	case StateBuildingMantCost:
		_building->setMantCost( _mress, ch_simplified.toInt() );
		break;
	case StateBuildingX:
		_building->setX( ch_simplified.toInt() );
		break;
	case StateBuildingY:
		_building->setY( ch_simplified.toInt() );
		break;
	case StateBuildingActionParam:
		_action->addParam( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool BaseHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


