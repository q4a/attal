/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightCell.cpp
** this class draws a hexagonal fightCell 
**
** Version : $Id: graphicalFightCell.cpp,v 1.11 2006/11/03 18:28:58 fdarling Exp $
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

#include "graphicalFightCell.h"

 
// generic include files
#include <stdlib.h>
#include <math.h>
// include files for QT
#include <QPainter>
// application specific include
#include "conf.h"

#include "libCommon/genericFightCell.h"
#include "libCommon/log.h"

const double GraphicalFightCell::_sqrt3 = sqrt(3);
const int GraphicalFightCell::_size = 15;

int VShift = 150;
int HShift = 0;

/** this class draws a hexagonal fightCell */
GraphicalFightCell::GraphicalFightCell( int row, int col, QGraphicsScene * canvas, bool horizontalDraw )
	: QGraphicsPolygonItem( 0,canvas )
{
	_horizontal = horizontalDraw;
	setColor( QColor( "grey" ) );

	_pa =  QPolygonF(7);
	if( _horizontal ) {
 		_pa[0] = QPoint( int(_size * _sqrt3) , 4 * _size);
		_pa[1] = QPoint( 0, 3 * _size );
		_pa[2] = QPoint( 0,  _size );
		_pa[3] = QPoint( int( _size * _sqrt3), 0 );
		_pa[4] = QPoint( int( 2 * _size * _sqrt3), _size );
		_pa[5] = QPoint( int( 2 * _size * _sqrt3), 3 * _size  );
		_pa[6] = _pa[0];

		if( row % 2 == 0 ) {
			setPos( int (HShift + 2*_sqrt3*_size*col ), int ( 3*_size*row) + VShift );
		} else {
			setPos( int (HShift + 2*_sqrt3*_size*(col + 0.5) ), int ( 3*_size*row) + VShift );
		}
	} else {
		_pa[0] = QPoint( 4 * _size, int(_size * _sqrt3) );
		_pa[1] = QPoint( 3 * _size, 0 );
		_pa[2] = QPoint( _size, 0 );
		_pa[3] = QPoint( 0, int(_size * _sqrt3) );
		_pa[4] = QPoint( _size, int(2 * _size * _sqrt3) );
		_pa[5] = QPoint( 3 * _size, int(2 * _size * _sqrt3) );
		_pa[6] = _pa[0];

		if( col % 2 == 0 ) {
			setPos( int ( 3*_size*col), int (VShift + 2*_sqrt3*_size*row ) );
		} else {
			setPos( int ( 3*_size*col), int (VShift + 2*_sqrt3*_size*(row+0.5) ) );
		}
	}
	setPolygon( _pa );

	setEnabled( true );
	setVisible( true );
	//collisions( true );

	setZValue( CAN_F_GROUND );
	show();
}

void GraphicalFightCell::paint( QPainter * p, const QStyleOptionGraphicsItem * /* option */, QWidget * /* widget */)
{
	if(enabled()) {
		p->setPen( _color );
		QPolygonF array = polygon();
		//array.translate( (int)x(), (int)y() );
		p->drawPolygon( array );
	}
}

QPointF GraphicalFightCell::getCenter()
{
	return boundingRect().center();
}

