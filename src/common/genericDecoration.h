/****************************************************************
**
** Attal : Lords of Doom
**
** genericDecoration.h
** manage decoration
**
** Version : $Id: genericDecoration.h,v 1.6 2002/11/01 13:42:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/06/2001
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

 
#ifndef GENERICDECORATION_H
#define GENERICDECORATION_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
#include <qxml.h>
// application specific include files
 
 
/*              ------------------------------
 *                         GenericDecoration
 *              ------------------------------ */



/** comment for the class */
class GenericDecoration
{

public:
	/** Constructor */
	GenericDecoration();

	/** Destructor */
	virtual ~GenericDecoration();

	/** \return Returns the name of this type of decoration */
	QString getName() { return _name; }

	/** Sets a name to the decoration */
	void setName( QString name ) { _name = name; }

	/** \return Returns some information about this decoration */
	QString getInfo() { return _info; }

	/** Sets some info about this decoration */
	void setInfo( QString info ) { _info = info; }

	/** \return Returns the technical effect of this decoration */
	int getTechnicEffect() { return _technicEffect; }

	/** Sets the technical effect of this decoration */
	void setTechnicEffect( int technicEffect ) { _technicEffect = technicEffect; }

	/** \return Returns the effect on lord's mvt */
	int getMoveEffect() { return _moveEffect; }

	/** Sets the effect on lord's mvt */
	void setMoveEffect( int moveEffect ) { _moveEffect = moveEffect; }

	/** \return Returns the factor of mvt effect */
	int getMoveFactor() { return _moveFactor; }

	/** Sets the factor of mvt effect */
	void setMoveFactor( int moveFactor ) { _moveFactor = moveFactor; }

	/** \return Returns the width of the decoration */
	int getDispoWidth() { return _width; }

	/** \return Returns the width of the decoration */
	int getDispoHeight() { return _height; }

	/** Sets the size of decoration */
	void setDispoSize( int height, int width );

	/** \return Returns the disposition of the cell (row, col) */
	int getDispo( int row, int col );

	/** Sets the disposition of the cell (row, col). 0->nothing, 1->main row and 2-> occupied */
	void setDispo( int row, int col, int type );

	/** \return Returns the row of the main cell of this decoration */
	int getMainRow() { return _mainRow; }

	/** \return Returns the col of the main cell of this decoration */
	int getMainCol() { return _mainCol; }

	/** Saves the data of a decoration */
	void save( QTextStream * ts, int indent );

private:
	void clear();

	QString _name, _info;
	int _technicEffect, _moveEffect;
	int _moveFactor;
	int _width, _height;
	int _mainCol, _mainRow;
	int ** _dispo;
};

/** List for managing the different types of decorations */
class DecorationList : public QList<GenericDecoration>
{
public:
	/** Constructor */
	DecorationList();

	/** Init list */
	bool init();

	/** Save the list on data file */
	bool save();
};

/** Parser for the DecorationList */
class DecorationHandler : public QXmlDefaultHandler
{
public:
	/** Constructor */
	DecorationHandler( DecorationList * list );

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
	DecorationList * _list;
	GenericDecoration * _decor;
	int _height, _width;

	QString _errorProt;

	enum State {
		StateInit,
		StateDocument,
		StateDecoration,
		StateDisposition,
		StateName,
		StateInfo,
		StateMove,
		StateMoveEffect,
		StateMoveFactor,
		StateTechnic,
		StateTechnicEffect,
	};
	State _state;
};

#endif // GENERICDECORATION_H
