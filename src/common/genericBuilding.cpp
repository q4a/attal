/****************************************************************
**
** Attal : Lords of Doom
**
** genericBuilding.cpp
** in order to manage buildings...
**
** Version : $Id: genericBuilding.cpp,v 1.28 2002/11/08 16:53:56 audoux Exp $
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

#include "genericBuilding.h"

 
// generic include files
// include files for QT
#include <qstringlist.h>
#include <qtextstream.h>
// application specific include files
#include "common/action.h"
#include "common/genericCell.h"
#include "common/genericLord.h"
#include "common/dataTheme.h"
#include "common/condition.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;

//
// ----- GenericBuilding -----
//

GenericBuilding::GenericBuilding()
	: _id( 0 ), _type( 0 ), _player( 0 )
{
	_currentCell = 0;
	_lords.setAutoDelete( false );
	_condition = 0;
}

GenericBuilding::~GenericBuilding()
{

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
	*ts << "\t<col>" << _currentCell->getCol() << "</col>" << endl;

	indentation( ts, indent );
	*ts << "\t<row>" << _currentCell->getRow() << "</row>" << endl;

	indentation( ts, indent );
	*ts << "</building>" << endl;
}

void GenericBuilding::enter( GenericLord * lord )
{
	if( _lords.findRef( lord ) == -1 ) {
		_lords.append( lord );
	}
}

QList<Action> GenericBuilding::getActionList( Action::ActionType type )
{
	QList<Action> ret;
	ret.setAutoDelete( false );

	GenericBuildingModel * _model = DataTheme.buildings.at( _id );
	for( uint i = 0; i < _model->getActionList()->count(); i++ ) {
		if( _model->getActionList()->at( i )->getType() == type ) {
			ret.append( _model->getActionList()->at( i ) );
		}
	}
	return ret;
}

QString GenericBuilding::getName()
{
	GenericBuildingModel * _model = DataTheme.buildings.at( _type );
	return _model->getName();
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
	return( _lords.findRef( lord ) != -1 );
}

//
// ----- GenericBuildingModel -----
//

GenericBuildingModel::GenericBuildingModel()
	: _type( 0 ), _nbFrame( 1 ), _animFreq( 1 )
{
	_actionList = new QList<Action>;
	_actionList->setAutoDelete( true );
	_doorCol = 0;
	_doorRow = 0;
}

void GenericBuildingModel::setDispo( int row, int col, int disp )
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

//
// ----- BuildingList -----
//

BuildingList::BuildingList()
{
	setAutoDelete( true );
}

bool BuildingList::init()
{
	clear();
	BuildingHandler handler( this );
	QFile file( DATA_PATH + "buildings.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "buildings.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

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

bool BuildingHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "buildings" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "building" && _state == StateDocument ) {
		_state = StateBuilding;
		_building = new GenericBuildingModel();
	} else if( qName == "name" && _state == StateBuilding ) {
		_state = StateName;
	} else if( qName == "description" && _state == StateBuilding ) {
		_state = StateDescription;
	} else if( qName == "disposition" && _state == StateBuilding ) {
		_state = StateDisposition;
	} else if( qName == "animation" && _state == StateBuilding ) {
		_state = StateAnim;
	} else if( qName == "frame" && _state == StateBuilding ) {
		_state = StateFrame;
	} else if( qName == "action" && _state == StateBuilding ) {
		_state = StateAction;
		_action = new Action();
		_action->setType( (Action::ActionType)( atts.value( "type" ).toInt() ) );
	} else if( qName == "elementary" && _state == StateAction ) {
		_state = StateElementary;
		_elementary = new ElementaryAction();
		_elementary->setType( (ElementaryAction::ElementaryType)( atts.value( "type" ).toInt() ) );
		_elementary->setArg( atts.value( "arg" ).toInt() );
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

bool BuildingHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
		case StateBuilding:
			_list->append( _building );
			_state = StateDocument;
			break;
		case StateName:
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
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
		case StateName:
			_building->setName( ch_simplified );
			break;
		case StateDescription:
			_building->setDescription( ch_simplified );
			break;
		case StateDisposition:{
			QStringList li = QStringList::split( " ", ch_simplified );
			for( int i = 0; i < 5; i++ ) {
				for( int j = 0; j < 5; j++ ) {
					_building->setDispo( i, j, li[ i*5 + j ].toInt() );
				}
			}}
			break;
		case StateFrame:
			_building->setNbFrame( ch_simplified.toInt() );
			break;
		case StateAnim:
			_building->setAnimFreq( ch_simplified.toInt() );
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
