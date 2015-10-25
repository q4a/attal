/****************************************************************
**
** Attal : Lords of Doom
**
** lordExperience.cpp
** Manage the levels of experience
**
** Version : $Id: lordExperience.cpp,v 1.10 2009/09/11 17:01:06 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/05/2004
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

#include "lordExperience.h"

// generic include files
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"

extern QString DATA_PATH;

LordExperience::LordExperience()
{

}

bool LordExperience::save()
{
	QString filename = DATA_PATH + "experience.dat";
	QFile file( filename );
	uint lastLevel = 0;

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE experience>" << endl;
	ts << "<experience>" << endl;

	uint nbLevels = _levels.count();
	for( uint i = 0; i < nbLevels; i++ ) {
		ts << "\t<level>";
		ts << (  _levels.at( i )  ) - lastLevel;
		ts << "</level>" << endl;
		lastLevel =  _levels.at( i ) ;
	}

	ts << "</experience>" << endl;

	file.close();

	return true;
}

bool LordExperience::init()
{
	clear();
	LordExperienceHandler handler( this );
	QString filename = DATA_PATH + "experience.dat";
	QFile file( filename );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( filename ), qPrintable( handler.errorProtocol() ) );
		return false;
	}
	return true;
}

void LordExperience::clear()
{
	_levels.clear();
}

uint LordExperience::getLevelNumber()
{
	return _levels.count() + 1;
}

uint LordExperience::getLevel( uint num )
{
	uint ret = 0;

	if( ( num > 0 ) && ( num < (uint)_levels.count() + 1 ) ) {
		ret =  _levels.at( num-1 );
	}

	return ret;
}

void LordExperience::appendLevel( uint value )
{
	_levels.append( value );
}

void LordExperience::appendLevelByDiff( uint diff )
{
	uint value = 0;
	if( _levels.count() > 0 ) {
		value =  _levels.at( _levels.count() - 1 );
	}
	_levels.append( diff + value );
}

uint LordExperience::computeLevelForExperience( uint experience )
{
	uint ret;
	uint i;
	uint nbLevels = _levels.count();
	
	ret = nbLevels;
	if( experience <  ( _levels.at( 0 ) ) ) {
		ret = 0;
	} else {
		for( i = 1; i < nbLevels; i++ ) {
			if( ( experience <  ( _levels.at( i ) ) ) && ( experience >=  ( _levels.at( i-1 ) ) ) )  {
				ret = i;
				break;
			}
		}
	}
	
	return ret;	
}

//
// ----- LordExperienceHandler -----
//

LordExperienceHandler::LordExperienceHandler( LordExperience * list )
{
	_list = list;
}

bool LordExperienceHandler::startDocument()
{
	_errorProt = "";
	_list->clear();
	_state = StateInit;
	return true;
}

bool LordExperienceHandler::startElement( const QString &, const QString &, const QString& qName, const QXmlAttributes & )
{
	if( qName == "experience" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "level" && _state == StateDocument ) {
		_state = StateLevel;
	} else {
		// error
		return false;
	}
	return true;
}

bool LordExperienceHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateLevel:
		_state = StateDocument;
		break;
	default:
		// do nothing
		break;
    }
    return true;
}

bool LordExperienceHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateLevel:
		_list->appendLevelByDiff( ch_simplified.toUInt() );
		break;
	default:
	    return false;
    }

    return true;
}

bool LordExperienceHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

