/****************************************************************
**
** Attal : Lords of Doom
**
** lordExperience.h
** Manage the levels of experience
**
** Version : $Id: lordExperience.h,v 1.4 2006/02/28 12:55:43 lusum Exp $
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


#ifndef LORDEXPERIENCE_H
#define LORDEXPERIENCE_H


// generic include files
// include files for QT
#include <QList>
#include <QXmlDefaultHandler>
// application specific include files


/*              ------------------------------
 *                       LordExperience
 *              ------------------------------ */


class LordExperience
{

public:
	/** Constructor */
	LordExperience();

	/** Save level of experience list */
	bool save();

	/** Init level of experience list */
	bool init();

	void clear();

	uint getLevelNumber();

	uint getLevel( uint num );

	void appendLevel( uint value );

	void appendLevelByDiff( uint diff );
	
	uint computeLevelForExperience( uint experience );

protected:
	QList<uint>  _levels;
};


/*              ------------------------------
 *                    LordExperienceHandler
 *              ------------------------------ */


class LordExperienceHandler : public QXmlDefaultHandler
{

public:
	/** Constructor */
	LordExperienceHandler( LordExperience * list );

	/**  Return the error protocol if parsing failed */
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
	LordExperience * _list;
	QString _errorProt, _name;
	enum State {
		StateInit,
		StateDocument,
		StateLevel,
	};
	State _state;
};

#endif // LORDEXPERIENCE_H
