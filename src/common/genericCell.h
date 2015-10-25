/****************************************************************
**
** Attal : Lords of Doom
**
** genericCell.h
**  generic class for managing (not graphical) cells
**
** Version : $Id: genericCell.h,v 1.30 2003/04/06 20:37:25 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

 
#ifndef GENERICCELL_H
#define GENERICCELL_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
#include <qxml.h>
// application specific includes
#include "common/define.h"
#include "common/artefact.h"

class GenericLord;
class GenericBuilding;
class GenericBase;
class GenericEvent;
class GenericMapCreature;
class GenericMap;

/*              ------------------------------
 *                         GenericCell
 *              ------------------------------ */


/** generic class for managing (not graphical) cells */
class GenericCell
{

public:
	/**  Constructor */
	GenericCell( int row = 0, int col = 0 );

	/** Destructor */
	virtual ~GenericCell();

	/** Set the moving coeff */
	void setCoeff( int c ) { _coeff = c; }

	/** \return Returns the moving coeff */
	int getCoeff();

	/** Set type of the cell */
	virtual void setType( const int type );

	/** Return type of the cell */
	int getType() const { return _type; }

	/** \return Returns the transition of this cell */
	int getTransition() { return _transition; }

	/** Set type of transition */
	virtual void setTransition( int transi ) { _transition = transi; }

	/** \return Returns the type of transition cell */
	int getTransitionCellType() {
		return _transitionCellType;
	}

	/** Set the type of transition for this cell */
	virtual void setTransitionCellType( const int type ) {
		_transitionCellType = type;
	}

	/** \return Returns the type of decoration of this cell */
	int getDecoration() { return _decoration; }

	/** Sets the type of decoration for this cell */
	virtual void setDecoration( int decoration ) {
		_decoration = decoration;
	}

	/** Return row of the cell */
	int getRow() const { return _row; }

	/** Return col of the cell */
	int getCol() const { return _col; }

	/** Set lord on the cell */
	void setLord( GenericLord * lord ) { _lord = lord; }

	/** Return lord on the cell (if any) */
	GenericLord * getLord() const { return _lord; }

	/** Set building on the cell */
	void setBuilding( GenericBuilding * build ) { _building = build; _base = 0; }

	/** Return building on the cell (if any) */
	GenericBuilding * getBuilding() const { return _building; }

	/** \return Returns the base on this cell (if any) */
	GenericBase * getBase() { return _base; }

	/** Sets a base on this cell */
	void setBase( GenericBase * base ) { _base = base; _building = 0; }

	/** \return Returns the creature on this cell (if any) */
	GenericMapCreature * getCreature() { return _creature; }

	/** Sets a creature on this cell */
	void setCreature( GenericMapCreature * creature ) { _creature = creature; }

	/** Tell if the cell is free (for moving) */
	bool isFree();

	/** \return Returns the artefact on this cell (if any) */
	GenericArtefact * getArtefact() { return _artefact; }

	/** Sets an artefact on this cell */
	void setArtefact( GenericArtefact * artefact ) { _artefact = artefact; }

	GenericEvent * getEvent() { return _event; }

	void setEvent( GenericEvent * event ) { _event = event; }

	/** \return Tells if we can stop (=go and stop) on this cell */
	bool isStoppable() { return _stop; }

	/** Sets this cell 'stoppable' or not */
	void setStoppable( bool state ) { _stop = state; }

protected:
	bool _stop;
	int _type;
	int _coeff;
	int _row , _col;
	int _transition;
	int _transitionCellType;
	int _decoration;
	GenericLord * _lord;
	GenericBuilding * _building;
	GenericBase * _base;
	GenericEvent * _event;
	GenericArtefact * _artefact;
	GenericMapCreature * _creature;
};

/** Manage model of cell */
class CellModel
{
public:
	/** Constructor */
	CellModel( QString name = "", int coeff = 1 ) { _name = name; _coeff = coeff; }

	/** \return Returns the name of this model */
	QString getName() { return _name; }

	/** Sets the name of this model */
	void setName( QString name ) { _name = name; }

	/** \return Returns the coefficient of this cell */
	int getCoeff() { return _coeff; }

	/** Sets the coeff of this cell */
	void setCoeff( int coeff ) { _coeff = coeff; }

protected:
	QString _name;
	int _coeff;
};

/** Handle a list of models of cells */
class CellModelList : public QList<CellModel>
{

public:
	/** Constructor */
	CellModelList();

	/** Save list */
	void save();

	/** Init list */
	bool init();

	/** Print list (for debug) */
	void print();

	/** Return coeff of cell type 'num' */
	int getCoeffAt( int num ) { return QList<CellModel>::at( num )->getCoeff(); }

	/** Return name of cell type 'num' */
	QString getNameAt( int num ) { return QList<CellModel>::at( num )->getName(); }

};



/*              ------------------------------
 *                         CellModelHandler
 *              ------------------------------ */



/** Parser for CellModelList */
class CellModelHandler : public QXmlDefaultHandler
{

public:
	/** Constructor */
	CellModelHandler( CellModelList * list );

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
	CellModelList * _list;
	CellModel * _model;
	QString _errorProt, _name;
	int _coeff;
	enum State {
		StateInit,
		StateDocument,
		StateTile,
		StateName,
		StateCoeff
	};
	State _state;
};

int computeTransition( GenericMap * theMap, GenericCell * cell );

int computeTransitionCellType( GenericMap * theMap, GenericCell * cell );



#endif // GENERICCELL_H
