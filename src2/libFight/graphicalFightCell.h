/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightCell.h
** this class draws a hexagonal fightCell 
**
** Version : $Id: graphicalFightCell.h,v 1.8 2006/11/03 18:28:58 fdarling Exp $
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
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPolygonF>
#include <QColor>

// application specific includes
 
 
/*              ------------------------------
 *                     GraphicalFightCell
 *              ------------------------------ */



/** this class draws a hexagonal fightCell */
class GraphicalFightCell : public QGraphicsPolygonItem
{ 
public:
	/** Constructor */
	GraphicalFightCell( int row, int col, QGraphicsScene * canvas, bool horizontalDraw = false);

	/** Return the center point of the hexa cell */
	QPointF getCenter();
	
	void setColor( QColor color ) { _color = color; }

	bool enabled() {return _enabled; }

	void  setEnabled ( bool enabled) { _enabled = enabled; }
	
private:
	/** Reimplemented not to fill the hexa */
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
	
	static const double _sqrt3;
	static const int _size;
	QPolygonF  _pa;
	QColor _color;
	bool _horizontal;
	bool _enabled;
};
 
#endif // GRAPHICALFIGHTCELL_H   

























