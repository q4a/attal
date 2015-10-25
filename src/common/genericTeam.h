/****************************************************************
**
** Attal : Lords of Doom
**
** genericTeam.h
** manage team
**
** Version : $Id: genericTeam.h,v 1.3 2002/10/20 10:28:59 audoux Exp $
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

 
#ifndef GENERICTEAM_H
#define GENERICTEAM_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
#include <qxml.h>
// application specific include files
 

/*              ------------------------------
 *                         GenericTeam
 *              ------------------------------ */



/** comment for the class */
class GenericTeam
{

public:
	/** Constructor */
	GenericTeam();

	/** Destructor */
	virtual ~GenericTeam();

	/** \return Returns the name of the team */
	QString getName() { return _name; }

	/** Sets the name of the team */
	void setName( QString name ) { _name = name; }

	/** \return Returns the 'red value' of the team's color */
	int getRed() { return _red; }

	/** Sets the 'red value' of the team's color */
	void setRed( int red ) { _red = red; }

	/** \return Returns the 'blue value' of the team's color */
	int getBlue() { return _blue; }

	/** Sets the 'blue value' of the team's color */
	void setBlue( int blue ) { _blue = blue; }

	/** \return Returns the 'green value' of the team's color */
	int getGreen() { return _green; }

	/** Sets the 'green value' of the team's color */
	void setGreen( int green ) { _green = green; }

private:
	QString _name;
	int _red, _green, _blue;
};

/** List of teams */
class TeamList : public QList<GenericTeam>
{
public:
	/** Constructor */
	TeamList();

	/** Init list */
	bool init();

	/** Save info in data file */
	bool save();
};

/** Parser for the TeamList */
class TeamHandler : public QXmlDefaultHandler
{
public:
	/** Constructor */
	TeamHandler( TeamList * list );

	/** \return Return the error protocol if parsing failed */
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
	TeamList * _list;
	GenericTeam * _team;

	QString _errorProt;

	enum State {
		StateInit,
		StateDocument,
		StateTeam,
		StateName,
		StateRed,
		StateGreen,
		StateBlue,
	};
	State _state;
};

#endif // GENERICTEAM_H
