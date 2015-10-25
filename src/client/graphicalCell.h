/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalCell.h
** this class draws an animated cell
**
** Version : $Id: graphicalCell.h,v 1.7 2002/10/20 16:15:49 audoux Exp $
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

 
#ifndef GRAPHICALCELL_H
#define GRAPHICALCELL_H
 
 
// generic include files

// include files for QT
#include <qcanvas.h>

// application specific include
#include "common/genericCell.h"

class Lord;
class Cell;

/*              ------------------------------
 *                         GraphicalCell
 *              ------------------------------ */



/** create an animated cell from pixmaps at position row,col */
class GraphicalCell : public QCanvasSprite
{
public:
	/** Constructor */
	GraphicalCell( int row, int col , QCanvas * canvas);

	/** destructor */
	virtual ~GraphicalCell();

	/** Return true if the point is on the cell */
	bool hit( const QPoint & p ) const;

	/** Return RTTI */
	int rtti() const { return RTTI; };

	static const int RTTI;

	/** \return Returns parent cell */
	virtual Cell * getParent() { return ( (Cell*)this ); }

private:
	QCanvasPixmapArray * _frames;

	static const int _size;
};

/** Manage a graphical transition on cell */
class Transition : public GraphicalCell
{
public:
	/** Constructor */
	Transition( Cell * parent, QCanvas * canvas);

	/** Sets transition type */
	void setTransition( int transition, int type );

	/** \return Returns parent cell */
	Cell * getParent() { return _parent; }

private:
	Cell * _parent;
};

/** Manages a decoration on the cell */
class Decoration : public GraphicalCell
{
public:
	/** Constructor */
	Decoration( Cell * parent, QCanvas * canvas);

	/** Set decoration type */
	void setDecoration( int decoration );

	/** \return Returns parent cell */
	Cell * getParent() { return _parent; }

private:
	Cell * _parent;
};



#endif // GRAPHICALCELL_H
