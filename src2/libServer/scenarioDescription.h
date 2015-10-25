/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioDescription.h
** Desciption of a scenario
**
** Version : $Id: scenarioDescription.h,v 1.4 2008/09/07 16:08:58 lusum Exp $
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

 
#ifndef SCENARIODESCRIPTION_H
#define SCENARIODESCRIPTION_H
 
 
// generic include files
// include files for QT
#include <QFileInfo>
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files
#include "libCommon/parser.h"
 
 
/*              ------------------------------
 *                     ScenarioDescription
 *              ------------------------------ */

class ScenarioDescription
{
 
public:
	/** Constructor */
	ScenarioDescription();
	
	void clear();
	
	bool load( const QString & fileName );
	
	uint getWidth() { return _width; }
	
	void setWidth( uint width ) { _width = width; }
	
	uint getHeight() { return _height; }
	
	void setHeight( uint height ) { _height = height; }
	
	uint getNbPlayers() { return _nbPlayers; }
	
	void setNbPlayers( uint nbPlayers ) { _nbPlayers = nbPlayers; }
	
	const QString & getName() { return _name; }
	
	void setName( const QString & name ) { _name = name; }
	
	const QString & getDescription() { return _description; }
	
	void setDescription( const QString & description ) { _description = description; }
	
	QString getFileName() { return _fileName; }
	
protected:
	QString _name, _description;
	uint _height, _width, _nbPlayers;
	QString _fileName;
};

/*              ------------------------------
 *                 ScenarioDescriptionParser
 *              ------------------------------ */

class ScenarioDescriptionParser : public QXmlDefaultHandler
{
public:
	/** Constructor */
	ScenarioDescriptionParser( ScenarioDescription * desc );
	
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

protected:
	void manageCondition( const QString & type );

	void manageConditionParam( const QString & param );

private:
	ScenarioDescription * _desc;
 	QString _errorProt;
	uint _otherCpt;
	
	ScenarioState _state;	
};
 
#endif // SCENARIODESCRIPTION_H   
