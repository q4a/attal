/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalCell.h
** this class draws an animated cell
**
** Version : $Id: graphicalCell.h,v 1.9 2006/11/11 19:38:01 fdarling Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include <QGraphicsScene>

// application specific include
#include "libCommon/genericCell.h"
#include "libCommon/dataTheme.h"

#include "libClient/attalSprite.h"

class Lord;
class Cell;

/*              ------------------------------
 *                         GraphicalCell
 *              ------------------------------ */



/** create an animated cell from pixmaps at position row,col */
class GraphicalCell : public AttalSprite
{
public:
	/** Constructor */
	GraphicalCell( int row, int col , QGraphicsScene * canvas);
};

/** Manages a decoration on the cell */
class Decoration : public AttalSprite
{
public:
	/** Constructor */
	Decoration( Cell * parent, QGraphicsScene * canvas);

	static const int RTTI;

	enum { 
		Type = QGraphicsItem::UserType + 2345 
	};

	int type() const { return Type; } // Enable the use of qgraphicsitem_cast with this item.

	/** Return true if the point is on the cell */
	bool hit( const QPointF & p ) const;

	/** Set decoration type */
	void setDecoration( uint group, uint item );

private:
	Cell * _parent;
};



#endif // GRAPHICALCELL_H
