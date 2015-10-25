/****************************************************************
**
** Attal : Lords of Doom
**
** mapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: mapView.cpp,v 1.11 2002/01/05 14:34:01 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 08/08/2000
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

#include "mapView.h"

 
// generic include files
// include files for QT
// application specific includes
#include "common/log.h"
#include "client/cell.h"
#include "client/lord.h"


/** subclass of QCanvasSprite that handles mouse event ... */
MapView::MapView( Map * map, QWidget * parent, const char * name, WFlags f )
	: QCanvasView( map, parent, name, f ) 
{
	viewport()->setMouseTracking( true ) ;
	_press = false;
}


/** handles mouse event */
void MapView::contentsMouseMoveEvent( QMouseEvent * event )
{
	QCanvasItemList list = canvas()->collisions( event->pos() );
	for( unsigned int i = 0; i < list.count(); i++ ) {	
		if( list[i]->rtti() == Cell::RTTI ) {
			Cell * cell = (Cell *) list[i];
			if( cell->hit( event->pos() ) ) { 
				//emit sig_mouseMoved( (Cell*) (_map->at( cell->getRow(), cell->getCol() ) ) );
				emit sig_mouseMoved( cell->getParent() );
			}
		}
	}
}

void MapView::contentsMousePressEvent( QMouseEvent * event )
{
	QCanvasItemList list = canvas()->collisions( event->pos() );
	for( unsigned int i = 0; i < list.count(); i++ ) {	
		if( list[i]->rtti() == Cell::RTTI ) {
			Cell * cell = (Cell *) list[i];
			if( ( cell->hit( event->pos() ) ) && ( cell->getParent() == cell ) ) {
				if( event->button() == RightButton ) {
					emit sig_mouseRightPressed( cell );
				} else {
					emit sig_mouseLeftPressed( cell );
				}
			}
		}
	}
	_press = true;
}

void MapView::contentsMouseReleaseEvent( QMouseEvent * event )
{
	_press = false;
	emit sig_mouseReleased();
}

void MapView::contentsMouseDoubleClickEvent( QMouseEvent * event )
{
	contentsMousePressEvent( event );
	contentsMouseReleaseEvent( event );
}
