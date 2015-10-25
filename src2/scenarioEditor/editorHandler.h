/****************************************************************
**
** Attal : Lords of Doom
**
** editorHandler.h
** scenario parser for the editor
**
** Version : $Id: editorHandler.h,v 1.2 2005/10/30 22:21:01 audoux Exp $
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

 
#ifndef EDITORHANDLER_H
#define EDITORHANDLER_H
 
 
// generic include files
// include files for QT
#include <QXmlDefaultHandler>
#include <QTextStream>
// application specific include files
 

class Screen;

/**              ------------------------------
 *                         EditorHandler
 **              ------------------------------ */


class EditorHandler : public QXmlDefaultHandler
{
 
public:
	/** Constructor */
	EditorHandler( Screen * screen );
	
	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();
	
	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );
	
	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );
	
	/** Define what to do of characters */
	bool characters( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );
	
private:
	Screen * _screen;
	QTextStream * _tempStream;
	QByteArray * _buffer;
	QString _errorProt;
	int _player;
		enum State {
		StateInit,
		StateDocument,
		StateName,
		StateTheme,
		StateDescription,
		StateMap,
		StateWidth,
		StateHeight,
		StateBase,
		StateBuilding,
		StateLord,
		StateLordCol,
		StateLordRow,
		StatePlayer,
		StatePlayerName,
		StatePlayerBases,
		StatePlayerBuildings,
		StatePlayerLords
	};
	State _state;
};
 
#endif // EDITORHANDLER_H   
