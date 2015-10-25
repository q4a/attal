/****************************************************************
**
** Attal : Lords of Doom
**
** genericCell.h
**  generic class for managing (not graphical) cells
**
** Version : $Id: genericCell.h,v 1.18 2008/04/14 23:06:14 lusum Exp $
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
#include <QColor>
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>

// application specific includes
#include "libCommon/define.h"

class QTextStream;

class GenericBase;
class GenericBuilding;
class GenericEvent;
class GenericLord;
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

	/** Coeff is a  movement modifier based on type of terrain 
		if coeff is minor to zero no movement is allowed
		higher is the coeff, more difficult is the movement
	 **/

	/** Set the moving coeff */
	void setCoeff( int c ) { _coeff = c; _realCoeff = _coeff; }

	/** \return Returns the moving coeff */
	int getCoeff();

	/** Set type of the cell */
	virtual void setType( const int type );

	/** Return type of the cell */
	int getType() const { return _type; }

	virtual void setDiversification( uint divers );

	void setRandomDiversification();

	uint getDiversification() { return _divers; }

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
	uint getDecorationGroup() { return _decorationGroup; }
	
	/** \return Returns the item of decoration of this cell */
	uint getDecorationItem() { return _decorationItem; }

	/** Sets the type of decoration for this cell */
	virtual void setDecoration( uint group, uint item );

	/** Return row of the cell */
	inline int getRow() const { return _row; } // voy a CAMBIAR (no he CAMBIADO) al 'uint's

	/** Return col of the cell */
	inline int getCol() const { return _col; } // voy a CAMBIAR (no he CAMBIADO) al 'uint's

	/** Set lord on the cell */
	void setLord( GenericLord * lord ) { _lord = lord; }

	/** Return lord on the cell (if any) */
	GenericLord * getLord() const { return _lord; }

	/** Set building on the cell */
	void setBuilding( GenericBuilding * build );

	/** Return building on the cell (if any) */
	GenericBuilding * getBuilding() const { return _building; }

	/** \return Returns the base on this cell (if any) */
	GenericBase * getBase() { return _base; }

	/** Sets a base on this cell */
	void setBase( GenericBase * base );

	/** \return Returns the creature on this cell (if any) */
	GenericMapCreature * getCreature() { return _creature; }

	/** Sets a creature on this cell */
	void setCreature( GenericMapCreature * creature ) { _creature = creature; }

	/** Tell if the cell is free (for moving) */
	bool isFree();

	GenericEvent * getEvent() { return _event; }

	void setEvent( GenericEvent * event ) { _event = event; }

	/** \return Tells if we can stop (=go and stop) on this cell */
	bool isStoppable() { return _stop; }

	/** Sets this cell 'stoppable' or not */
	void setStoppable( bool state ) { _stop = state; }

protected:
	bool _stop;
	int _type;
	uint _divers;
	int _coeff;
	int _realCoeff; //optimisation to avoid an if and to inline getCoeff
	int _row , _col; // voy a CAMBIAR (no he CAMBIADO) al 'uint's
	int _transition;
	int _transitionCellType;
	uint _decorationGroup, _decorationItem;
	GenericLord * _lord;
	GenericBuilding * _building;
	GenericBase * _base;
	GenericEvent * _event;
	GenericMapCreature * _creature;
};

/** Manage model of cell */
class CellModel
{
public:
	/** Constructor */
	CellModel( const QString & name = "", int coeff = 1 );

	/** Destructor */
	virtual ~CellModel();

	/** \return Returns the name of this model */
	QString getName() { return _name; }

	/** Sets the name of this model */
	void setName( const QString & name ) { _name = name; }

	/** \return Returns the coefficient of this cell */
	int getCoeff() { return _coeff; }

	/** Sets the coeff of this cell */
	void setCoeff( int coeff ) { _coeff = coeff; }
	
	void addDiversification( uint weight );

	void removeDiversification( uint num );

	uint getDiversificationNumber();

	uint getDiversification( uint num );

	uint getRandomDiversification();

	QColor getColor() { return _color; }

	void setColor( const QColor & color ) { _color = color; }

	void save( QTextStream * ts, int indent );

protected:
	QString _name;
	int _coeff;
	QColor _color;
	QList<uint *> _diversification;
};

/** Handle a list of models of cells */
class CellModelList : public QList<CellModel *>
{

public:
	/** Constructor */
	CellModelList();

	/** Destructor */
	virtual ~CellModelList();

	/** Save list */
	bool save();

	/** Init list */
	bool init();

	void clear();

	/** Print list (for debug) */
	void print();

	/** Return coeff of cell type 'num' */
	int getCoeffAt( int num ) { return QList<CellModel *>::at( num )->getCoeff(); }

	/** Return name of cell type 'num' */
	QString getNameAt( int num ) { return QList<CellModel *>::at( num )->getName(); }

	void save( QTextStream * ts, int indent );
	
	/** these methods return the graphical dimensions of a cell **/

	uint getHeight() { return _height; }

	void setHeight( uint height ) { _height = height; }
	
	uint getWidth() { return _width; }

	void setWidth( uint width ) { _width = width; }

	/** end dimensions **/
	
private:
	uint _height, _width;
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
	int _red, _green, _blue;
	int _coeff;
	enum State {
		StateInit,
		StateDocument,
		StateWidth,
		StateHeight,
		StateTile,
		StateName,
		StateCoeff,
		StateColor,
		StateColorRed,
		StateColorGreen,
		StateColorBlue,
		StateDiversification
	};
	State _state;
};

int computeTransition( GenericMap * theMap, GenericCell * cell );

int computeTransitionCellType( GenericMap * theMap, GenericCell * cell );

/** Compute and change road type for Cell and its neibours */
void computeAndChangeRoadType( GenericMap* theMap, GenericCell* cell, uint decorationGroup ); 

#endif // GENERICCELL_H
