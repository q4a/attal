/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightCell.cpp
** this class draws a hexagonal fightCell 
**
** Version : $Id: graphicalFightCell.cpp,v 1.8 2002/07/20 16:00:02 audoux Exp $
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
#include <qpainter.h>
// application specific include
#include "conf.h"
#include "common/genericFightCell.h"

const int GraphicalFightCell::RTTI = 44;
const double GraphicalFightCell::_sqrt3 = sqrt(3);
const int GraphicalFightCell::_size = 15;

int VShift = 120;

/** this class draws a hexagonal fightCell */
GraphicalFightCell::GraphicalFightCell( int row, int col, QCanvas * canvas )
	: QCanvasPolygon( canvas )
{
	_color = gray;
	
	_pa =  QPointArray(7);
	_pa[0] = QPoint( 4 * _size, int(_size * _sqrt3) );
	_pa[1] = QPoint( 3 * _size, 0 );
	_pa[2] = QPoint( _size, 0 );
	_pa[3] = QPoint( 0, int(_size * _sqrt3) );
	_pa[4] = QPoint( _size, int(2 * _size * _sqrt3) );
	_pa[5] = QPoint( 3 * _size, int(2 * _size * _sqrt3) );
	_pa[6] = QPoint( 4 * _size, int(_size * _sqrt3) );
	setPoints( _pa );
	
	setEnabled( true );
	setVisible( true );
	collisions( true );
	
	if( col % 2 == 0 ) {
		move( 3*_size*col , VShift + 2*_sqrt3*_size*row );	
	}	
	else {
		move( 3*_size*col, VShift + 2*_sqrt3*_size*(row+0.5) );
	}
	
	setZ( CAN_F_GROUND );
	show();
}

void GraphicalFightCell::drawShape( QPainter & p )
{
	p.setPen( _color );
	p.drawPolyline( _pa );	
}

QPoint GraphicalFightCell::getCenter()
{
	return boundingRect().center();
}












