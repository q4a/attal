/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: specifications.cpp,v 1.2 2002/05/08 09:50:17 audoux Exp $
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

#include "specifications.h"

// generic include files
// include files for QT

// application specific include files
#include "common/log.h"

extern QString DATA_PATH;

//
// ----- Specifications -----
//

Specifications::Specifications()
{
	_listArtefacts.setAutoDelete( true );
}

bool Specifications::init()
{
	clear();
	SpecificationsHandler handler( this );
	QFile file( DATA_PATH + "specifications.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "specifications.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

void Specifications::clear()
{
	_listArtefacts.clear();
}

void Specifications::save()
{
	logDD("Not yet implemented.");
}


void Specifications::add( SpecificationArtefact * artefact )
{
	_listArtefacts.append( artefact );
}

uint Specifications::getArtefactNumber()
{
	return _listArtefacts.count();
}

SpecificationArtefact * Specifications::getArtefact( uint num )
{
	SpecificationArtefact * ret = 0;

	if( num < _listArtefacts.count() ) {
		ret = _listArtefacts.at( num );
	}

	return ret;
}

//
// ----- SpecificationArtefact -----
//

SpecificationArtefact::SpecificationArtefact()
{
	_x = 0;
	_y = 0;
	_name = "";
	_type = 0;
}


//
// ----- SpecificationsHandler -----
//

SpecificationsHandler::SpecificationsHandler( Specifications * spec )
{
	_spec = spec;
}

bool SpecificationsHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_spec->clear();
	_state = StateInit;
	return true;
}

bool SpecificationsHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "specifications" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "artefact" && _state == StateDocument ) {
		_state = StateArtefact;
		_stateArtefact = StateInitArtefact;
		_artefact = new SpecificationArtefact();
	} else if( _state == StateArtefact ) {
		startElementArtefact( namespaceURI, localName, qName, atts );
	} else {
		// error
		return false;
	}
	return true;
}

bool SpecificationsHandler::startElementArtefact( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "x" && _stateArtefact == StateInitArtefact ) {
		_stateArtefact = StateX;
	} else if ( qName == "y" && _stateArtefact == StateInitArtefact ) {
		_stateArtefact = StateY;
	} else if ( qName == "type" && _stateArtefact == StateInitArtefact ) {
		_stateArtefact = StateType;
	} else if ( qName == "name" && _stateArtefact == StateInitArtefact ) {
		_stateArtefact = StateName;
	} else {
		// error
		return false;
	}
	return true;
}

bool SpecificationsHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateDocument:
		_state = StateInit;
		break;
	case StateArtefact:
		endElementArtefact( namespaceURI, localName, qName );
		break;
	default:
	    // do nothing
	    break;
    }
    return true;
}

bool SpecificationsHandler::endElementArtefact( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch( _stateArtefact ) {
	case StateInitArtefact:
		_spec->add( _artefact );
		_state = StateDocument;
		break;
	case StateType:
		_stateArtefact = StateInitArtefact;
		break;
	case StateX:
		_stateArtefact = StateInitArtefact;
		break;
	case StateY:
		_stateArtefact = StateInitArtefact;
		break;
	case StateName:
		_stateArtefact = StateInitArtefact;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;
}

bool SpecificationsHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateArtefact:
		return charactersArtefact( ch_simplified );
		break;
	default:
	    return false;
    }

    return true;
}

bool SpecificationsHandler::charactersArtefact( const QString& ch )
{
	switch( _stateArtefact ) {
	case StateX:
		_artefact->setX( ch.toInt() );
		break;
	case StateY:
		_artefact->setY( ch.toInt() );
		break;
	case StateType:
		_artefact->setType( ch.toInt() );
		break;
	case StateName:
		_artefact->setName( ch );
		break;
	default:
	    return false;
    }

    return true;
}


bool SpecificationsHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


