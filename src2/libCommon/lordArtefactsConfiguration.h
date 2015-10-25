/****************************************************************
**
** Attal : Lords of Doom
**
** lordArtefactsConfiguration.h
** Configuration of artefacts for lords
**
** Version : $Id: lordArtefactsConfiguration.h,v 1.3 2005/11/11 20:47:48 audoux Exp $
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


#ifndef LORDARTEFACTSCONFIGURATION_H
#define LORDARTEFACTSCONFIGURATION_H


// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files


class ArtefactPosition
{
public:
	/** Constructor */
	ArtefactPosition();

	/** Destructor */
	virtual ~ArtefactPosition();

	void setName( const QString & name ) {
		_name = name;
	}

	QString getName() {
		return _name;
	}

	/** nb = 0 -> infinite */
	void setNumber( uint nb );

	uint getNumber();

	void setCoord( uint nb, uint x, uint y );

	uint getCoordX( uint nb );

	uint getCoordY( uint nb );

protected:
	QString _name;
	QList<uint *> _coordX;
	QList<uint *> _coordY;
	uint _nb;
};

/*              ------------------------------
 *                 LordArtefactsConfiguration
 *              ------------------------------ */



/** comment for the class */
class LordArtefactsConfiguration
{

public:
	/** Constructor */
	LordArtefactsConfiguration();

	/** Destructor */
	virtual ~LordArtefactsConfiguration();

	/* nb = 0 -> infinite */
	void appendPosition( ArtefactPosition * position );

	uint getNbPosition();

	QString getPositionName( uint pos );

	uint getPositionNumber( uint pos );

	void setPositionCoord( uint pos, uint nb, uint x, uint y );

	uint getPositionCoordX( uint pos, uint nb );

	uint getPositionCoordY( uint pos, uint nb );

	bool init();

	void clear();

protected:
	QList<ArtefactPosition *> _artefacts;
};

/** Parser of artefact's position list */
class ArtefactsConfigurationHandler : public QXmlDefaultHandler
{

public:
	/** Construtor */
	ArtefactsConfigurationHandler( LordArtefactsConfiguration * config );

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
	LordArtefactsConfiguration * _config;
	ArtefactPosition * _position;
	uint _cpt;

	enum State {
		StateInit,
		StateDocument,
		StateArtefact,
		StateName,
		StateNumber,
		StatePosition,
	};
	State _state;
};

#endif // LORDARTEFACTSCONFIGURATION_H
