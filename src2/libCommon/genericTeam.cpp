/****************************************************************
**
** Attal : Lords of Doom
**
** genericTeam.cpp
** manage team
**
** Version : $Id: genericTeam.cpp,v 1.12 2009/10/22 17:18:29 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2001
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

#include "genericTeam.h"
 
// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"

extern QString DATA_PATH;

//
// ----- GenericTeam -----
//

GenericTeam::GenericTeam( uint num )
: _num( num )
{
	_name = "";
	_red = 0;
	_green = 0;
	_blue = 0;
}

GenericTeam::~GenericTeam()
{

}

QColor GenericTeam::getColor()
{
	return QColor( _red, _green, _blue );
}


//
// ----- TeamList -----
//

TeamList::TeamList()
{
	
}

TeamList::~TeamList()
{
	clear();
}

bool TeamList::init()
{
	clear();
	TeamHandler handler( this );
	QFile file( DATA_PATH + "teams.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "teams.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}
	
	return true;
}
		
bool TeamList::save()
{
	QString filename = DATA_PATH + "teams.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable( filename ) );
		return false;
	}
	
	QTextStream ts( &file );
		
	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE teams>" << endl;
	ts << "<teams>" << endl;
	
	for( int i = 0; i < count(); i++ ) {
		GenericTeam * team = at( i );
		ts << "\t<team>" << endl;
		ts << "\t\t<name>" << team->getName() << "</name>" << endl;
		ts << "\t\t<red>" << team->getRed() << "</red>" << endl;
		ts << "\t\t<green>" << team->getGreen() << "</green>" << endl;
		ts << "\t\t<blue>" << team->getBlue() << "</blue>" << endl;
		ts << "\t</team>" << endl;
	}
	
	ts << "</teams>" << endl;
	
	file.close();
	
	return true;
}

void TeamList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

//
// ----- TeamHandler -----
//


TeamHandler::TeamHandler( TeamList * list )
{
	_list = list;
}

bool TeamHandler::startDocument()
{
	_errorProt = "";
	_list->clear();	
	_state = StateInit;
	
	return true;
}
	
bool TeamHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & )
{
	if( qName == "teams" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "team" && _state == StateDocument ) {
		_state = StateTeam;
		_team = new GenericTeam( _list->count() );
	} else if ( qName == "name" && _state == StateTeam ) {
		_state = StateName;	
	} else if ( qName == "red" && _state == StateTeam ) {
		_state = StateRed;
	} else if ( qName == "green" && _state == StateTeam ) {
		_state = StateGreen;
	} else if ( qName == "blue" && _state == StateTeam ) {
		_state = StateBlue;
	} else {
		return false;
	}
	return true;
}

bool TeamHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateTeam:
		_state = StateDocument;
		_list->append( _team );
		break;
	case StateName:
		_state = StateTeam;
		break;
	case StateRed:
		_state = StateTeam;
		break;
	case StateGreen:
		_state = StateTeam;
		break;
	case StateBlue:
		_state = StateTeam;
		break;
	default:
		// do nothing 
		break;
	}
	return true;		
}
	
bool TeamHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_team->setName( ch_simplified );
		break;
	case StateRed:
		_team->setRed( ch_simplified.toInt() );
		break;
	case StateGreen:
		_team->setGreen( ch_simplified.toInt() );
		break;
	case StateBlue:
		_team->setBlue( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;	
}

bool TeamHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}
