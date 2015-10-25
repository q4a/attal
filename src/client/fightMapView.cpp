/****************************************************************
**
** Attal : Lords of Doom
**
** fightMapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: fightMapView.cpp,v 1.3 2001/11/04 21:53:10 audoux Exp $
**
** Author(s) : Cyrille Verrier
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

#include "fightMapView.h"

 
// generic include files
// include files for QT
// application specific includes
#include "common/log.h"
#include "client/lord.h"


/** subclass of QCanvasView that handles mouse event ... */
FightMapView::FightMapView( FightMap *map, QWidget * parent, const char * name, WFlags f )
	: QCanvasView( map, parent, name, f )
{
	viewport()->setMouseTracking( true ) ;
	_press = false;
}


/** handles mouse move event */
void FightMapView::contentsMouseMoveEvent( QMouseEvent *e)
{
	QCanvasItemList list = canvas()->collisions(e->pos());
	for( unsigned int i=0; i<list.count() ; i++ ) {	
		if( list[i]->rtti() == FightCell::RTTI ) {
			emit sig_mouseMoved( (FightCell *) list[i] );
		}
	}
}


/*
 * handles mouse press event
 * emit a signal to say which cell is selected
 */
void FightMapView::contentsMousePressEvent( QMouseEvent *e )
{
	QCanvasItemList list = canvas()->collisions(e->pos());
	for( unsigned int i=0; i<list.count() ; i++ ) {	
		if( list[i]->rtti() == FightCell::RTTI ) {
			if( e->button() == RightButton ) 
				emit sig_mouseRightPressed( (FightCell *) list[i] );
			else
				emit sig_mouseLeftPressed( (FightCell *) list[i] );
			
		}
	}
	_press = true;
}

/** handles mouse press event */

void FightMapView::contentsMouseReleaseEvent( QMouseEvent *e )
{
	_press = false;
	if( e->button() == RightButton )
		emit sig_mouseReleased();
}
