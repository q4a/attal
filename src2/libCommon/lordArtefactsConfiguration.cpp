/****************************************************************
**
** Attal : Lords of Doom
**
** lordArtefactsConfiguration.cpp
** Configuration of artefacts for lords
**
** Version : $Id: lordArtefactsConfiguration.cpp,v 1.9 2009/10/28 11:54:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "lordArtefactsConfiguration.h"

// generic include files
// include files for QT
#include <QFile>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

extern QString DATA_PATH;

//
// ----- ArtefactPosition -----
//

ArtefactPosition::ArtefactPosition()
{
	_name = "Unknown";
	_nb = 0;
}

ArtefactPosition::~ArtefactPosition()
{
	while( ! _coordX.isEmpty() ) {
		delete _coordX.takeFirst();
	}
	while( ! _coordY.isEmpty() ) {
		delete _coordY.takeFirst();
	}
}

void ArtefactPosition::setNumber( uint nb )
{
	while( ! _coordX.isEmpty() ) {
		delete _coordX.takeFirst();
	}
	while( ! _coordY.isEmpty() ) {
		delete _coordY.takeFirst();
	}
	_nb = nb;
	for( uint i = 0; i < nb; i++ ) {
		_coordX.append( new uint(0) );
		_coordY.append( new uint(0) );
	}
	if( _nb == 0 ) {
		_coordX.append( new uint(0) );
		_coordY.append( new uint(0) );
	}
}

uint ArtefactPosition::getNumber()
{
	return _nb;
}

void ArtefactPosition::setCoord( uint nb, uint x, uint y )
{
	if( nb < _nb ) {
		*( _coordX.at( nb ) ) = x;
		*( _coordY.at( nb ) ) = y;
	} else if( _nb == 0 ) {
		*( _coordX.at( 0 ) ) = x;
		*( _coordY.at( 0 ) ) = y;
	}
}

uint ArtefactPosition::getCoordX( uint nb )
{
	if( _nb < nb ) {
		return *( _coordX.at( nb ) );
	} else {
		return *( _coordX.at( 0 ) );
	}
}

uint ArtefactPosition::getCoordY( uint nb )
{
	if( _nb < nb ) {
		return *( _coordY.at( nb ) );
	} else {
		return *( _coordY.at( 0 ) );
	}
}


//
// ----- LordartefactsConfiguration -----
//

LordArtefactsConfiguration::LordArtefactsConfiguration()
{
}

LordArtefactsConfiguration::~LordArtefactsConfiguration()
{
	clear();
}

void LordArtefactsConfiguration::appendPosition( ArtefactPosition * position )
{
	_artefacts.append( position );
}

uint LordArtefactsConfiguration::getNbPosition()
{
	return _artefacts.count();
}

QString LordArtefactsConfiguration::getPositionName( uint pos )
{
	QString ret = "";
	if( (int)pos < _artefacts.count() ) {
		ret = _artefacts.at( pos )->getName();
	}

	return ret;
}

uint LordArtefactsConfiguration::getPositionNumber( uint pos )
{
	uint ret = 0;

	if( (int)pos < _artefacts.count() ) {
		ret = _artefacts.at( pos )->getNumber();
	}

	return ret;
}

void LordArtefactsConfiguration::setPositionCoord( uint pos, uint nb, uint x, uint y )
{
	if( (int)pos < _artefacts.count() ) {
		_artefacts.at( pos )->setCoord( nb, x, y );
	}
}

uint LordArtefactsConfiguration::getPositionCoordX( uint pos, uint nb )
{
	uint ret = 0;

	if( (int)pos < _artefacts.count() ) {
		ret = _artefacts.at( pos )->getCoordX( nb );
	}

	return ret;
}

uint LordArtefactsConfiguration::getPositionCoordY( uint pos, uint nb )
{
	uint ret = 0;

	if( (int)pos < _artefacts.count() ) {
		ret = _artefacts.at( pos )->getCoordY( nb );
	}

	return ret;
}

bool LordArtefactsConfiguration::init()
{
	clear();
	ArtefactsConfigurationHandler handler( this );
	QFile file( DATA_PATH + "lordArtefacts.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "lordArtefacts.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

void LordArtefactsConfiguration::clear()
{
	while( ! _artefacts.isEmpty() ) {
		delete _artefacts.takeFirst();
	}
}

//
// ----- ArtefactsConfigurationHandler -----
//

ArtefactsConfigurationHandler::ArtefactsConfigurationHandler( LordArtefactsConfiguration * config )
{
	_config = config;
}

bool ArtefactsConfigurationHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_config->clear();

	_state = StateInit;
	return true;
}

bool ArtefactsConfigurationHandler::startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes& atts)
{
	if( qName == "lordArtefacts" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "artefact" && _state == StateDocument ) {
		_state = StateArtefact;
		_position = new ArtefactPosition();
		_cpt = 0;
	} else if( qName == "name" && _state == StateArtefact ) {
		_state = StateName;
	} else if( qName == "number" && _state == StateArtefact ) {
		_state = StateNumber;
		_position->setNumber( atts.value( "value" ).toInt() );
	} else if( qName == "position" && _state == StateNumber ) {
		_state = StatePosition;
		_position->setCoord( _cpt, atts.value( "x" ).toInt(), atts.value( "y" ).toInt() );
		_cpt++;
	} else {
		// error
		return false;
	}
	return true;
}

bool ArtefactsConfigurationHandler::endElement( const QString&, const QString&, const QString& )
{
	switch ( _state ) {
	case StateArtefact:
		_config->appendPosition( _position );
		_state = StateDocument;
		break;
	case StateName:
		_state = StateArtefact;
		break;
	case StateNumber:
		_state = StateArtefact;
		break;
	case StatePosition:
		_state = StateNumber;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;
}

bool ArtefactsConfigurationHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_position->setName( ch_simplified );
		break;
	default:
	    return false;
    }

    return true;
}


bool ArtefactsConfigurationHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

