/****************************************************************
**
** Attal : Lords of Doom
**
** genericTeam.h
** manage team
**
** Version : $Id: genericTeam.h,v 1.6 2005/11/11 20:47:48 audoux Exp $
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
#include <QColor>
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files
 

/*              ------------------------------
 *                         GenericTeam
 *              ------------------------------ */


class GenericTeam
{

public:
	/** Constructor */
	GenericTeam( uint num );

	/** Destructor */
	virtual ~GenericTeam();
	
	uint getNumber() { return _num; }

	/** \return Returns the name of the team */
	QString getName() { return _name; }

	/** Sets the name of the team */
	void setName( const QString & name ) { _name = name; }

	/** \return Returns the 'red value' of the team's color */
	uint getRed() { return _red; }

	/** Sets the 'red value' of the team's color */
	void setRed( uint red ) { _red = red; }

	/** \return Returns the 'blue value' of the team's color */
	uint getBlue() { return _blue; }

	/** Sets the 'blue value' of the team's color */
	void setBlue( uint blue ) { _blue = blue; }

	/** \return Returns the 'green value' of the team's color */
	uint getGreen() { return _green; }

	/** Sets the 'green value' of the team's color */
	void setGreen( uint green ) { _green = green; }
	
	QColor getColor();

private:
	QString _name;
	uint _num;
	uint _red, _green, _blue;
};

/** List of teams */
class TeamList : public QList<GenericTeam *>
{
public:
	/** Constructor */
	TeamList();

	/** Destructor */
	virtual ~TeamList();

	/** Init list */
	bool init();

	/** Save info in data file */
	bool save();

	void clear();
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
