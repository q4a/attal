/****************************************************************
**
** Attal : Lords of Doom
**
** artefact.cpp
** manages artefacts
**
** Version : $Id: artefact.cpp,v 1.8 2002/11/11 17:41:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 18/08/2000
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

#include "artefact.h"
 
// generic include files
// include files for QT
// application specific include files
#include "common/dataTheme.h"
#include "common/genericCell.h"
#include "common/genericLord.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;

//
// ----- GenericArtefact -----
//

GenericArtefact::GenericArtefact()
	: _id( 0 ), _type( 0 )
{
	_isOnMap = false;
	_cell = 0;
	_lord = 0;
	_pos = 0;
}

GenericArtefact::~GenericArtefact()
{
}

void GenericArtefact::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<artefact type=\"" << _type << "\">" << endl;

	if( _cell ) {
		indentation( ts, indent );
		*ts << "\t<col>" << _cell->getCol() << "</col>" << endl;

		indentation( ts, indent );
		*ts << "\t<row>" << _cell->getRow() << "</row>" << endl;
	}

	indentation( ts, indent );
	*ts << "</artefact>" << endl;
}

QString GenericArtefact::getName()
{
	return DataTheme.artefacts.at( _type )->getName();
}

int GenericArtefact::getModelPosition()
{
	return DataTheme.artefacts.at( _type )->getPosition();
}

GenericCell * GenericArtefact::getCell()
{
	return _cell;
}

void GenericArtefact::setCell( GenericCell * cell )
{
	_cell = cell;
	if( cell ) {
		_isOnMap = true;
	} else {
		_isOnMap = false;
	}
	_lord = 0;
}

GenericLord * GenericArtefact::getLord()
{
	return _lord;
}

void GenericArtefact::setLord( GenericLord * lord )
{
	_lord = lord;
	_isOnMap = false;
	_cell = 0;
}

int GenericArtefact::getPosition()
{
	int ret = -1;
	if( ! _isOnMap ) {
		ret = _pos;
	}
	return ret;
}

void GenericArtefact::setPosition( int position )
{
	if( ! _isOnMap ) {
		_pos = position;
	} else {
		_pos = -1;
	}
}

//
// ----- GenericArtefactModel -----
//

GenericArtefactModel::GenericArtefactModel()
	:_name( "" ), _position( 0 )
{
	
}

void GenericArtefactModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<artefact>" << endl;

	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;

	indentation( ts, indent );
	*ts << "\t<position>" << _position << "</position>" << endl;

	indentation( ts, indent );
	*ts << "\t<action>" << "</action>" << endl;
	
	indentation( ts, indent );
	*ts << "</artefact>" << endl;
}

//
// ----- ArtefactList -----
//

ArtefactList::ArtefactList()
{
	setAutoDelete( true );
}

bool ArtefactList::init()
{
	clear();
	ArtefactHandler handler( this );
	QFile file( DATA_PATH + "artefacts.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "artefacts.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

bool ArtefactList::save()
{
	QString filename = DATA_PATH + "artefacts.dat";
	QFile file( filename );

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE artefacts>" << endl;
	ts << "<artefacts>" << endl;

	for( uint i = 0; i < count(); i++ ) {
		at( i )->save( &ts, 1 );
	}

	ts << "</artefacts>" << endl;

	file.close();

	return true;
}
//
// ----- ArtefactHandler -----
//

ArtefactHandler::ArtefactHandler( ArtefactList * list )
{
	_list = list;
}

bool ArtefactHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	
	_state = StateInit;
	return true;
}

bool ArtefactHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "artefacts" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "artefact" && _state == StateDocument ) {
		_state = StateArtefact;
		_artefact = new GenericArtefactModel();
	} else if( qName == "name" && _state == StateArtefact ) {
		_state = StateName;
	} else if( qName == "position" && _state == StateArtefact ) {
		_state = StatePosition;
	} else if( qName == "action" && _state == StateArtefact ) {
		_state = StateAction;		
	} else {
		// error
		return false;
	}
	return true;
}

bool ArtefactHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateArtefact:
		_list->append( _artefact );
		_state = StateDocument;
		break;
	case StateName:
		_state = StateArtefact;
		break;
	case StatePosition:
		_state = StateArtefact;
		break;
	case StateAction:
		_state = StateArtefact;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool ArtefactHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_artefact->setName( ch_simplified );
		break;
	case StatePosition:
		_artefact->setPosition( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool ArtefactHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}




