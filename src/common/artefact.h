/****************************************************************
**
** Attal : Lords of Doom
**
** artefact.h
** manages artefacts
**
** Version : $Id: artefact.h,v 1.9 2002/10/20 20:12:48 audoux Exp $
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


#ifndef GENERICARTEFACT_H
#define GENERICARTEFACT_H


// generic include files
// include files for QT
#include <qlist.h>
#include <qxml.h>
#include <qstring.h>
// application specific include files

class QTextStream;
class GenericCell;
class GenericLord;

/**              ------------------------------
 *                         GenericArtefact
 **              ------------------------------ */


class GenericArtefact
{
public:
	/** Constrcutor */
	GenericArtefact();

	/** Destructor */
	virtual ~GenericArtefact();

	/** Saves the artefact data */
	void save( QTextStream * ts, int indent = 0 );

	/** \return Returns the id of the artefact */
	int getId() { return _id; }

	/** Sets the id of the artefact */
	void setId( int id ) { _id = id; }

	/** \return Returns the type of the artefact */
	int getType() { return _type; }

	/** Sets the type of the artefact */
	virtual void setType( int type ) { _type = type; }

	/** Get the name of the artefact */
	QString getName();

	/** Get the position of the artefact */
	int getModelPosition();

	/** \return Returns the position on the lord of this artefact */
	int getPosition();

	/** Sets the position on the lord of this artefact */
	void setPosition( int pos );

	/** \return Returns the cell where is the artefact on the map (0 if any) */
	GenericCell * getCell();

	/** Sets cell on the map where this artefact is */
	virtual void setCell( GenericCell * cell );

	/** \return Returns the lord who owns this artefact (if any) */
	GenericLord * getLord();

	/** Sets the lord who owns this artefact (if any) */
	virtual void setLord( GenericLord * lord );

	/** \return Tells if this artefact is on map */
	bool isOnMap() { return _isOnMap; }

protected:
	int _id, _type, _pos;
	bool _isOnMap;
	GenericCell * _cell;
	GenericLord * _lord;
};

/** Model for artefact */
class GenericArtefactModel
{

public:
	/** Constructor */
	GenericArtefactModel();

	/** Get the name of the artefact */
	QString getName() { return _name; }

	/** Set the name of the artefact */
	void setName( QString name ) { _name = name; }

	/** Get type of the artefact */
	int getPosition() { return _position; }

	/** Set type of the artefact */
	void setPosition( int position ) { _position = position; }

	/** Save this model on file */
	void save( QTextStream * ts, int indent );


private:
	QString _name;
	int _position;
};

/** List of GenericArtefactModel */
class ArtefactList : public QList<GenericArtefactModel>
{
public:
	/** Constructor */
	ArtefactList();

	/** Return base corresponding to 'type' */
	GenericArtefactModel * at( int type ) { return QList<GenericArtefactModel>::at( type ); }

	/** Init list */
	bool init();

	bool save();
};

/** Parser of artefact list */
class ArtefactHandler : public QXmlDefaultHandler
{

public:
	/** Construtor */
	ArtefactHandler( ArtefactList * list );

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
	QString _errorProt;
	ArtefactList * _list;
	GenericArtefactModel * _artefact;

	enum State {
		StateInit,
		StateDocument,
		StateArtefact,
		StateName,
		StateAction,
		StatePosition,
	};
	State _state;
};




#endif // GENERICARTEFACT_H
