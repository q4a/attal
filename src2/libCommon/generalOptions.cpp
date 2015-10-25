/****************************************************************
**
** Attal : Lords of Doom
**
** generalOptions.cpp
** general options of the theme
**
** Version : $Id: generalOptions.cpp,v 1.14 2009/09/11 17:01:06 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 06/08/2004
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

#include "generalOptions.h"
 
// generic include files
// include files for QT
#include <QFile>
#include <QTextStream>
// application specific include files
#include "libCommon/log.h"

extern QString DATA_PATH;

//
// ----- GeneralOptions -----
//

QString GeneralOptions::getVisionManagementName( VISION_MANAGEMENT type )
{
	QString ret;
	
	switch( type ) {
	case VISION_ONCE:
		ret = "Vision once";
		break;
	case VISION_REAL:
		ret = "Vision real";
		break;
	}
	
	return ret;

}

GeneralOptions::GeneralOptions()
{
	_calendar = NULL;
	clear();
}

GeneralOptions::~GeneralOptions()
{
	if(_calendar) {
		delete _calendar;
		_calendar = NULL;
	}
}
	
void GeneralOptions::clear()
{
	_visionManagement = VISION_ONCE;
	if(_calendar) {
		delete _calendar;
		_calendar = NULL;
	}

}

bool GeneralOptions::save()
{
	QString filename = DATA_PATH + "general.dat";
	QFile file( filename );
	int j,i;

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE general>" << endl;
	ts << "<general>" << endl;

	ts << "\t<vision>" << (uint)_visionManagement << "</vision>" << endl;
	ts << "\t<calendar>" << endl;
	for( i = 0; i < 3; i++ ) {
		ts << "\t\t<level num=\"" << i << "\""; 
		ts <<  " value=\"" ;
		ts << _calendar->getLevelValue( i );
		ts <<  "\">" ;
		ts	<< endl;
		for( j = 0; j < (int) _calendar->getLevelValue(i); j++ ) {
			ts << "\t\t\t<name num=\"" << j << "\">";
			ts << _calendar->getLevelName( i, j );
			ts << "</name>" << endl;
		}
	ts << "\t\t</level>" << endl;
	}


	ts << "\t</calendar>" << endl;
	ts << "</general>" << endl;

	file.close();

	return true;
}
	
bool GeneralOptions::init()
{
	clear();
	GeneralOptionsHandler handler( this );
	QFile file( DATA_PATH + "general.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "general.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}
	
void GeneralOptions::setCalendar(CalendarModel * calendar)
{
	if(_calendar) {
		delete _calendar;
		_calendar = 0;
	}
	_calendar = calendar;
}


//
// ----- GeneralOptionsHandler -----
//

GeneralOptionsHandler::GeneralOptionsHandler( GeneralOptions * general )
{
	_general = general;
	_num = -1;
	_levelC = 0;
	_value = 0;
}

bool GeneralOptionsHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_general->clear();
	
	_state = StateInit;
	return true;
}

bool GeneralOptionsHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "general" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "vision" && _state == StateDocument ) {
		_state = StateVision;
	} else if ( qName == "calendar" && _state == StateDocument ) {
		_state = StateCalendar;
		_calendar = new CalendarModel();
	} else if ( qName == "level" && _state == StateCalendar ) {
		if( atts.index( "num" ) != -1 ) {
			_levelC = atts.value( "num" ).toInt();
		} else {
			_levelC = 0;
		}
		if( atts.index( "value") != -1  ){
			_value = atts.value( "value" ).toInt();
		} else {
			_value = 0;
		}
		_state = StateCalendarLevel;
	} else if ( qName == "name" && _state == StateCalendarLevel ) {
		if( atts.index( "num" ) != -1 ) {
			_num = atts.value( "num" ).toInt();
		} else {
			_num = -1;
		}
		_state = StateCalendarName;
	} else {
		// error
		return false;
	}
	return true;
}

bool GeneralOptionsHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateVision:
		_state = StateDocument;
		break;
	case StateCalendar:
		_general->setCalendar(_calendar);
		_state = StateDocument;
		break;
	case StateCalendarLevel:
		_state = StateCalendar;
		break;
	case StateCalendarName:
		_state = StateCalendarLevel;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool GeneralOptionsHandler::characters( const QString & ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateVision:
		_general->setVisionManagement( ( GeneralOptions::VISION_MANAGEMENT ) ch_simplified.toInt() );
		break;
	case StateCalendarLevel:
		_calendar->setLevelValue(_levelC,_value);
		break;
	case StateCalendarName:
		{
		_calendar->setLevelName(_levelC,_num ,ch_simplified);
		}
		break;
	default:
	    return false;
	}

    return true;
}


bool GeneralOptionsHandler::fatalError( const QXmlParseException & exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		.arg( exception.message() )
		.arg( exception.lineNumber() )
		.arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}

