/****************************************************************
**
** Attal : Lords of Doom
**
** editorHandler.cpp
** scenario parser for the editor
**
** Version : $Id: editorHandler.cpp,v 1.1 2001/02/09 18:07:31 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/02/2001
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

#include "editorHandler.h"
 
// generic include files
// include files for QT
// application specific include files
#include "editor/screen.h"

/** add comments here */
EditorHandler::EditorHandler( Screen * screen )
{
	_screen = screen;
}


bool EditorHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_player = 0;
	_state = StateInit;
	return true;
}

bool EditorHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "scenario" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "name" && _state == StateDocument ) {
		_state = StateName;
	} else if( qName == "theme" && _state == StateDocument ) {
		_state = StateTheme;
	} else if( qName == "description" && _state == StateDocument ) {
		_state = StateDescription;
	} else if( qName == "map" && _state == StateDocument ) {
		_state = StateMap;
		_buffer = new QByteArray();
		_tempStream = new QTextStream( *_buffer, IO_WriteOnly );
	} else if( qName == "width" && _state == StateMap ) {
		_state = StateWidth;
	} else if ( qName == "height" && _state == StateMap ) {
		_state = StateHeight;
	} else if( qName == "base" && _state == StateDocument ) {
		_state = StateBase;
	} else if( qName == "building" && _state == StateDocument ) {
		_state = StateBuilding;
	} else if( qName == "lord" && _state == StateDocument ) {
		_state = StateLord;
	} else if( qName == "col" && _state == StateLord ) {
		_state = StateLordCol;
	} else if( qName == "row" && _state == StateLord ) {
		_state = StateLordRow;
	} else if( qName == "player" && _state == StateDocument ) {		
		_state = StatePlayer;
	} else if( qName == "name" && _state == StatePlayer ) {		
		_state = StatePlayerName;
	} else if( qName == "bases" && _state == StatePlayer ) {		
		_buffer = new QByteArray();
		_tempStream = new QTextStream( *_buffer, IO_WriteOnly );
		_state = StatePlayerBases;
	} else if( qName == "buildings" && _state == StatePlayer ) {		
		_buffer = new QByteArray();
		_tempStream = new QTextStream( *_buffer, IO_WriteOnly );
		_state = StatePlayerBuildings;
	} else if( qName == "lords" && _state == StatePlayer ) {		
		_buffer = new QByteArray();
		_tempStream = new QTextStream( *_buffer, IO_WriteOnly );
		_state = StatePlayerLords;		
	} else {
		// error
		return false;
	}
	return true;
}

bool EditorHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateName:
		_state = StateDocument;
		break;
	case StateTheme:
		_state = StateDocument;		
		break;
	case StateDescription:
		_state = StateDocument;		
		break;
	case StateMap:
		delete _tempStream;
		delete _buffer;
		_state = StateDocument;		
		break;
	case StateWidth:
		_state = StateMap;
		break;
	case StateHeight:
		_state = StateMap;
		break;
	case StateBase:
		_state = StateDocument;		
		break;
	case StateBuilding:
		_state = StateDocument;		
		break;
	case StateLord:
		_state = StateDocument;		
		break;
	case StateLordCol:		
		_state = StateLord;
		break;
	case StateLordRow:
		_state = StateLord;
		break;
	case StatePlayer:
		_player++;
		_state = StateDocument;		
		break;		
	case StatePlayerName:
		_state = StatePlayer;		
		break;		
	case StatePlayerBases: {
		delete _tempStream;
		delete _buffer;
		_state = StatePlayer;		
		break;		
	}
	case StatePlayerBuildings: {
		delete _tempStream;
		delete _buffer;		
		_state = StatePlayer;		
		break; 
	}
	case StatePlayerLords: {
		delete _tempStream;
		delete _buffer;		
		_state = StatePlayer;		
		break; 
	}		
	default:
		// do nothing
		break;
    }
    return true;	
}

bool EditorHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		break;
	case StateTheme:
		break;
	case StateDescription:
		break;
	case StateMap:
		*_tempStream << ch_simplified;
		break;
	case StateWidth:
		break;
	case StateHeight:
		break;
	case StateBase:
		break;
	case StateBuilding:
		break;
	case StateLordCol:
		break;
	case StateLordRow:
		break;
	case StatePlayerName:
		break;
	case StatePlayerBases:
		break;
	case StatePlayerBuildings:
		break;
	case StatePlayerLords:
		break;
	default:
		return false;
    }

    return true;
}


bool EditorHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


