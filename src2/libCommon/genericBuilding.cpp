/****************************************************************
**
** Attal : Lords of Doom
**
** genericBuilding.cpp
** in order to manage buildings...
**
** Version : $Id: genericBuilding.cpp,v 1.39 2009/11/19 22:14:10 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

#include "genericBuilding.h"

 
// generic include files
// include files for QT
#include <QObject>
#include <QStringList>
#include <QTextStream>

// application specific include files
#include "libCommon/action.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericLord.h"
#include "libCommon/dataTheme.h"
#include "libCommon/condition.h"

extern QString DATA_PATH;

//
// ----- GenericBuilding -----
//

GenericBuilding::GenericBuilding( int type )
	: _id( 0 ), _player( 0 )
{
	_type = type;
	_currentCell = 0;
	_condition = 0;
	_ress = new GenericResourceList();
	
	getInitResources();
}

GenericBuilding::~GenericBuilding()
{
	delete _ress;
}

void GenericBuilding::getInitResources()
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	_ress->setListValues( model->getResourceList() );
}

bool GenericBuilding::load( QTextStream * )
{
	return true;
}

void GenericBuilding::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<building type=\"" << _type << "\">" << endl;
	
	indentation( ts, indent );
	*ts << "\t<id>" << _id << "</id>" << endl;

	indentation( ts, indent );
	*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

	indentation( ts, indent );
	*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<resource type=\"" << j << "\">";
		(*ts) << _ress->getValue( j );
		(*ts) << "</resource>" << endl;
	}

	indentation( ts, indent );
	*ts << "</building>" << endl;
	*ts << flush;
}

void GenericBuilding::enter( GenericLord * lord )
{
	if( _lords.indexOf( lord ) == -1 ) {
		_lords.append( lord );
	}
}

QList<Action *> GenericBuilding::getActionList( Action::ActionType type )
{
	GenericBuildingModel * _model = DataTheme.buildings.at( _type );
	return _model->getActionList( type  );
}

QString GenericBuilding::getName()
{
	GenericBuildingModel * _model = DataTheme.buildings.at( _type );
	return _model->getName();
}

int GenericBuilding::getVision()
{
	int ret = 3;

	if( _type < DataTheme.buildings.count() ) {
		ret = DataTheme.buildings.at( _type )->getVision();
	}

	return ret;
}

QString GenericBuilding::getBuildDescription()
{

	GenericBuildingModel * _model = DataTheme.buildings.at( _type );

	QString ret = "";
	ret = _model->getBuildAutoDescription();
	if( _ress->isRess() ) {
		ret +=  QObject::tr("Now") + " ";
		ret += _ress->getAutoDescription();
	}
	return ret;
}			
	
uint GenericBuilding::getNbFrame()
{
	GenericBuildingModel * _model = DataTheme.buildings.at( _type );
	return _model->getNbFrame();
}

int GenericBuilding::getAnimFreq()
{
	GenericBuildingModel * _model = DataTheme.buildings.at( _type );
	return _model->getAnimFreq();
}

void GenericBuilding::setPosition( GenericCell * cell )
{
	
	TRACE("void GenericBuilding::setPosition( GenericCell * cell ) row %d, cell col %d",cell->getRow(), cell->getCol());
  
	if( _currentCell != 0 ) {
		_currentCell->setBuilding( 0 );
	}
	_currentCell = cell;
	cell->setBuilding( this );
}

bool GenericBuilding::hasBeenVisited()
{
	return( ! _lords.isEmpty() );
}

bool GenericBuilding::hasBeenVisited( GenericLord * lord )
{
	return( _lords.indexOf( lord ) != -1 );
}

uint GenericBuilding::getDoorRow()
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	if( model ) {
		return model->getDoorRow();
	} else {
		return 0;
	}
}

uint GenericBuilding::getDoorCol()
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	if( model ) {
		return model->getDoorCol();
	} else {
		return 0;
	}
}

uint GenericBuilding::getDispoHeight()
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	if( model ) {
		return model->getHeight();
	} else {
		return 0;
	}
}

uint GenericBuilding::getDispoWidth()
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	if( model ) {
		return model->getWidth();
	} else {
		return 0;
	}
}

GenericMapDisposition::DispositionType GenericBuilding::getDisposition( uint row, uint col )
{
	GenericBuildingModel * model = DataTheme.buildings.at( _type );
	if( model ) {
		return model->getDisposition( row, col );
	} else {
		return GenericMapDisposition::OCCUPIED;
	}
}

//
// ----- GenericBuildingModel -----
//

GenericBuildingModel::GenericBuildingModel()
: _nbFrame( 1 ), _animFreq( 1 ), _vision( 3 )
{
	_actionList = new QList<Action *>;
	
	_ress = new GenericResourceList();
	
	GenericMapDisposition::init( 5, 5 );
	setDisposition( 4, 0, DOOR );
}

GenericBuildingModel::~GenericBuildingModel()
{
	while( ! _actionList->isEmpty() ) {
		delete _actionList->takeFirst();
	}
	delete _actionList;
	delete _ress;
}

QList<Action *> GenericBuildingModel::getActionList( Action::ActionType type )
{
	QList<Action *> ret;

	for( int i = 0; i < _actionList->count(); i++ ) {
		if( _actionList->at( i )->getType() == type ) {
			ret.append( _actionList->at( i ) );
		}
	}
	return ret;
}

QString GenericBuildingModel::getBuildAutoDescription()
{
	QString tmp = getName() + "\n";

	for( int type = Action::DATE; type< Action::LAST; type++ ){
		QList<Action *>  action = getActionList((Action::ActionType) type);
		for( int i = 0; i < action.count(); i++ ) {
			tmp += action.at( i )->getDescription( true );
		}
	}
	if( _ress->isRess() ) {
		tmp +=  QObject::tr("Start") + " ";
		tmp += _ress->getAutoDescription();
	}

	return tmp;
}

void GenericBuildingModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<building>" << endl;

	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<vision>" << _vision << "</vision>" << endl;

	indentation( ts, indent );
	*ts << "\t<description>" << _description << "</description>" << endl;
	
	indentation( ts, indent );	
	*ts << "\t<frame>" << _nbFrame  << "</frame>" << endl;

	indentation( ts, indent );
	*ts << "\t<animation>" << _animFreq << "</animation>" << endl;

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		*ts << "\t<resource type=\"" << j << "\">";
		*ts << _ress->getValue( j );
		*ts << "\t</resource>" << endl;
	}

	for( int i = 0; i < _actionList->count(); ++i ) {
		_actionList->at( i )->save( ts, indent );
	}

	GenericMapDisposition::save( ts, indent+1 );

	indentation( ts, indent );

	indentation( ts, indent );
	*ts << "</building>" << endl;
	*ts << flush;
}


//
// ----- BuildingList -----
//

BuildingList::BuildingList()
{
}

BuildingList::~BuildingList()
{
	clear();
}

bool BuildingList::init()
{
	clear();
	BuildingHandler handler( this );
	QFile file( DATA_PATH + "buildings.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "buildings.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

void BuildingList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

bool BuildingList::save()
{
	
	QString filename = DATA_PATH + "buildings.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable( filename ) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE buildings>" << endl;
	ts << "<buildings>" << endl;

	GenericBuildingModel * building;
	for( int i = 0; i < count(); i++ ) {
		building = at( i );
		if( building ) {
			building->save( &ts, 1 );
		}
	}

	ts << "</buildings>" << endl;

	file.close();

	return true;
	
}



//
// ----- BuildingHandler -----
//


BuildingHandler::BuildingHandler( BuildingList * list )
{
	_list = list;
}

bool BuildingHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();

	_state = StateInit;
	return true;
}

bool BuildingHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes& atts )
{
	if( qName == "buildings" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "building" && _state == StateDocument ) {
		_state = StateBuilding;
		_building = new GenericBuildingModel();
	} else if( qName == "name" && _state == StateBuilding ) {
		_state = StateName;
	} else if( qName == "vision" && _state == StateBuilding ) {
		_state = StateVision;
	} else if( qName == "description" && _state == StateBuilding ) {
		_state = StateDescription;
	} else if( qName == "disposition" && _state == StateBuilding ) {
		_state = StateDisposition;
		_height = atts.value( "height" ).toUInt();
		_width = atts.value( "width" ).toUInt();
		_building->GenericMapDisposition::init( _height, _width );
	} else if( qName == "animation" && _state == StateBuilding ) {
		_state = StateAnim;
	} else if( qName == "frame" && _state == StateBuilding ) {
		_state = StateFrame;
	} else if( qName == "action" && _state == StateBuilding ) {
		_state = StateAction;
		_action = new Action();
		_action->setType( (Action::ActionType)( atts.value( "type" ).toInt() ) );
		_action->setCoeff( ( atts.value( "coeff" ).toInt() ) );
	} else if( qName == "resource" && _state == StateBuilding ) {
		_state = StateResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "elementary" && _state == StateAction ) {
		_state = StateElementary;
		_elementary = new ElementaryAction();
		_elementary->setType( (ElementaryAction::ElementaryType)( atts.value( "type" ).toInt() ) );
		_elementary->setArg( atts.value( "arg" ).toInt() );
		_elementary->setArg1( atts.value( "arg1" ).toInt() );
		/*
	} else if( qName == "arg" && _state == StateElementary ) {
		_state = StateElementaryArg;
	} else if( qName == "value" && _state == StateElementary ) {
		_state = StateElementaryValue;*/
	} else {
		// error
		return false;
	}
	
	return true;
}

bool BuildingHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
		case StateBuilding:
			_list->append( _building );
			_state = StateDocument;
			break;
		case StateName:
			_state = StateBuilding;
			break;
		case StateVision:
			_state = StateBuilding;
			break;
		case StateDescription:
			_state = StateBuilding;
			break;
		case StateDisposition:
			_state = StateBuilding;
			break;
		case StateAnim:
			_state = StateBuilding;
			break;
		case StateFrame:
			_state = StateBuilding;
			break;
		case StateAction:
			_state = StateBuilding;
			_building->addAction( _action );
			break;
		case StateElementary:
			_state = StateAction;
			_action->addElementaryAction( _elementary );
			break;
		case StateResource:
			_state = StateBuilding;
			break;/*
		case StateElementaryArg:
			_state = StateElementary;
			break;
		case StateElementaryValue:
			_state = StateElementary;
			break;*/
		default:
			// do nothing
			break;
	}

	return true;
}

bool BuildingHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
		case StateName:
			_building->setName( ch_simplified );
			break;
		case StateVision:
			_building->setVision( ch_simplified.toInt() );
			break;
		case StateDescription:
			_building->setDescription( ch_simplified );
			break;
		case StateDisposition:{
			QStringList li = ch_simplified.split( " " );
			GenericMapDisposition::DispositionType type;
			for( uint i = 0; i < _height; i++ ) {
				for( uint j = 0; j < _width; j++ ) {
					type = GenericMapDisposition::DispositionType( li[ i * _width + j ].toInt() );
					_building->setDisposition( i, j, type );
				}
			}
			} break;
		case StateFrame:
			_building->setNbFrame( ch_simplified.toInt() );
			break;
		case StateAnim:
			_building->setAnimFreq( ch_simplified.toInt() );
			break;
		case StateResource:
			_building->getResourceList()->setValue( _res, ch_simplified.toInt() );
			break;
		case StateElementary:
			_elementary->setCoeff( ch_simplified.toInt() );
			break;
		/*
		case StateElementaryValue:
			//_building->setType( ch_simplified.toInt() );
			_elementary->setCoeff( ch_simplified.toInt() );
			break;
		case StateElementaryArg:
			_elementary->setArg( ch_simplified.toInt() );
			break;*/
		default:
			return false;
	}
	
	return true;
}

bool BuildingHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		.arg( exception.message() )
		.arg( exception.lineNumber() )
		.arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}
