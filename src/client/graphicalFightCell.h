/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightCell.h
** this class draws a hexagonal fightCell 
**
** Version : $Id: graphicalFightCell.h,v 1.5 2001/06/16 14:17:59 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 07/12/2000
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

 
#ifndef GRAPHICALFIGHTCELL_H
#define GRAPHICALFIGHTCELL_H
 
 
// generic include files


// include files for QT
#include <qcanvas.h>
#include <qcolor.h>
// application specific includes
 
 
/*              ------------------------------
 *                     GraphicalFightCell
 *              ------------------------------ */



/** this class draws a hexagonal fightCell */
class GraphicalFightCell : public QCanvasPolygon
{ 
public:
	/** Constructor */
	GraphicalFightCell( int row, int col, QCanvas * canvas);

	/** Return RTTI */
	int rtti() const { return RTTI; };
	
	static const int RTTI;
	
	/** Return the center point of the hexa cell */
	QPoint getCenter();
	
	void setColor( QColor color ) { _color = color; }
private:
	/** Reimplemented not to fill the hexa */
	void drawShape ( QPainter & p );
	
	static const double _sqrt3;
	static const int _size;
	QPointArray  _pa;
	QColor _color;
};
 
#endif // GRAPHICALFIGHTCELL_H   

























