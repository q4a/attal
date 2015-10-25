/****************************************************************
**
** Attal : Lords of Doom
**
** specifications.h
** Manage specifications of the game
**
** Version : $Id: specifications.h,v 1.3 2002/10/20 20:12:49 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/05/2002
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


#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H


// generic include files
// include files for QT
#include <qxml.h>
#include <qlist.h>
#include <qstring.h>
// application specific include files


class SpecificationArtefact;

/**              ------------------------------
 *                        Specifications
 **              ------------------------------ */

class Specifications
{
public:
	/** Constructor */
	Specifications();

	/** Init the specification */
	bool init();

	/** Clears the specif */
	void clear();

	/** Save the specif */
	void save();

	/** Add a specif. artefact */
	void add( SpecificationArtefact * );

	/** \return Returns the nb of specif artefact */
	uint getArtefactNumber();

	/** \return Returns the specif artefact nb 'num' */
	SpecificationArtefact * getArtefact( uint num );

protected:
	QList<SpecificationArtefact> _listArtefacts;
};


/*              ------------------------------
 *                   SpecificationArtefact
 *              ------------------------------ */



/** Artefact of specification */
class SpecificationArtefact
{
public:
	/** Constructor */
	SpecificationArtefact();

	/** \return Returns the type of the artefact */
	int getType() { return _type; }

	/** Sets type of artefact */
	void setType( int type ) { _type = type; }

	/** \return Returns pos 'X' */
	int getX() { return _x; }

	/** Sets pos 'X' */
	void setX( int x ) { _x = x; }

	/** \return Returns pos 'Y' */
	int getY() { return _y; }

	/** Sets pos 'Y' */
	void setY( int y ) { _y = y; }

	/** \return Returns name of the artefact */
	QString getName() { return _name; }

	/** Sets name of the artefact */
	void setName( QString name ) { _name = name; }

protected:
	int _type, _x, _y;
	QString _name;
};

/*              ------------------------------
 *                         SpecificationsHandler
 *              ------------------------------ */

/** Parser for Specifications */
class SpecificationsHandler : public QXmlDefaultHandler
{

public:
	/** Constructor */
	SpecificationsHandler( Specifications * spec );

	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();

	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define Start elements and associated actions */
	bool startElementArtefact( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );

	/** Define End elements and associated actions */
	bool endElementArtefact( const QString& namespaceURI, const QString& localName, const QString& qName );

	/** Define what to do of characters */
	bool characters( const QString& ch );

	/** Define what to do of characters */
	bool charactersArtefact( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );

private:
	QString _errorProt;
	Specifications * _spec;
	SpecificationArtefact * _artefact;

	enum State {
		StateInit,
		StateDocument,
		StateArtefact
	};
	enum StateForArtefact {
		StateInitArtefact,
		StateType,
		StateX,
		StateY,
		StateName
	};
	State _state;
	StateForArtefact _stateArtefact;
};


#endif // SPECIFICATIONS_H
