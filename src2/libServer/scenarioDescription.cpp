/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioDescription.cpp
** Desciption of a scenario
**
** Version : $Id: scenarioDescription.cpp,v 1.4 2009/10/28 11:54:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2004
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

#include "scenarioDescription.h"
 
// generic include files
// include files for QT
#include <QFile>
// application specific include files
#include "libCommon/log.h"

//
// ----- ScenarioDescription -----
//

ScenarioDescription::ScenarioDescription()
{
	clear();
}


void ScenarioDescription::clear()
{
	_nbPlayers = 0;
	_width = 0;
	_height = 0;
	_name = QObject::tr( "Unknown" );
	_description = "";
}
	
bool ScenarioDescription::load( const QString & fileName )
{
	_fileName = fileName;
	ScenarioDescriptionParser handler( this );
	QFile file( fileName );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( fileName ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

//
//  ----- ScenarioDescriptionParser -----
//

ScenarioDescriptionParser::ScenarioDescriptionParser( ScenarioDescription * desc )
{
	_desc = desc;
}

bool ScenarioDescriptionParser::startDocument()
{
	// at the beginning of parsing: do some initialization
	_desc->clear();
	_errorProt = "";
	_state = StateInit;
	return true;
}

bool ScenarioDescriptionParser::startElement( const QString& /* namespaceURI */, const QString& /* localName */, const QString& qName, const QXmlAttributes& atts )
{
	int number;
	bool ret = true;

	if( qName == "scenario" && _state == StateInit ) {
		_state = StateDocument;
		_otherCpt = 0;
		_desc->clear();
		_desc->setNbPlayers( atts.value( "nbPlayer" ).toInt() );
	} else if( qName == "attalversion" && _state == StateDocument ) {
		_state = StateAttalVersion;
	} else if( qName == "name" && _state == StateDocument ) {
		_state = StateName;
	} else if( qName == "description" && _state == StateDocument ) {
		_state = StateDescription;
	} else if( qName == "currentplayer" && _state == StateDocument ) {
		_state = StateCurrentPlayer;
	} else if( qName == "calendar" && _state == StateDocument ) {
		_state = StateCalendar;
	} else if( qName == "value" && _state == StateCalendar ) {
		_state = StateCalendarDate;
		number = atts.value( "type" ).toInt();
	} else if( qName == "startcalendar" && _state == StateCalendar ) {
		_state = StateCalendarStart;
	} else if( qName == "value" && _state == StateCalendarStart ) {
		_state = StateCalendarStartDate;
		number = atts.value( "type" ).toInt();
	} else if( qName == "turn" && _state == StateCalendar ) {
		_state = StateCalendarTurn;
	} else if( qName == "map" && _state == StateDocument ) {
		_state = StateMap;
	} else if( qName == "width" && _state == StateMap ) {
		_state = StateWidth;
	} else if( qName == "height" && _state == StateMap ) {
		_state = StateHeight;
	} else if( ( _state == StateDocument ) || ( _state == StateDocumentOther ) ) {
		_state = StateDocumentOther;
		_otherCpt++;
	} else if( ( _state == StateMap ) || ( _state == StateMapOther ) ) {
		_state = StateMapOther;
		_otherCpt++;
	}
	return ret;
}

bool ScenarioDescriptionParser::endElement( const QString& /* namespaceURI */, const QString& /* localName */, const QString& /* qName */)
{
	bool ret = true;

	switch ( _state ) {
	case StateName:
		_state = StateDocument;
		break;
	case StateAttalVersion:
		_state = StateDocument;
		break;
	case StateDescription:
		_state = StateDocument;
		break;
	case StateCurrentPlayer:
		_state = StateDocument;
		break;
	case StateCalendar:
		_state = StateDocument;
		break;
	case StateCalendarDate:
		_state = StateCalendar;
		break;
	case StateCalendarStart:
		_state = StateCalendar;
		break;
	case StateCalendarStartDate:
		_state = StateCalendarStart;
		break;
	case StateCalendarTurn:
		_state = StateCalendar;
		break;
	case StateMap:
		_state = StateFinish;
		break;
	case StateWidth:
		_state = StateMap;
		break;
	case StateHeight:
		_state = StateMap;
		break;
	case StateDocumentOther:
		if( _otherCpt == 1 ) {
			_state = StateDocument;
		} 
		_otherCpt--;
	case StateMapOther:
		if( _otherCpt == 1 ) {
			_state = StateMap;
		} 
		_otherCpt--;
		break;
	default:
		break;
    }
    return ret;
}

bool ScenarioDescriptionParser::characters( const QString & ch )
{
	bool ret = true;
	QString ch_simplified = ch.simplified();

	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_desc->setName( ch_simplified );
		break;
	case StateDescription:
		_desc->setDescription( ch_simplified );
		break;
	case StateWidth:
		_desc->setWidth( ch_simplified.toUInt() );
		break;
	case StateHeight:
		_desc->setHeight( ch_simplified.toUInt() );
		break;
	default:
		break;
	}
	return ret;
}



bool ScenarioDescriptionParser::fatalError( const QXmlParseException& exception )
{
     logEE( "state %d", _state );

    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

		
		
		






