/****************************************************************
**
** Attal : Lords of Doom
**
** fightMapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: fightMapView.cpp,v 1.14 2008/05/05 17:12:46 lusum Exp $
**
** Author(s) : Cyrille Verrier - Sardi Carlo
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
#include <QMouseEvent>
// application specific includes
#include "libCommon/log.h"
#include "libClient/lord.h"
#include "libFight/fightUnit.h"

/** subclass of QCanvasView that handles mouse event ... */
FightMapView::FightMapView( FightMap *map, QWidget * parent, const char * /* name */, Qt::WFlags /* f */ )
	: QGraphicsView( map, parent )
{
	viewport()->setMouseTracking( true ) ;
}


/** handles mouse move event */
void FightMapView::mouseMoveEvent( QMouseEvent *e)
{
	QPointF pos = mapToScene(e->pos());
	QList<QGraphicsItem *> list = scene()->items( pos );
	uint nbItems = (uint)list.count();
	
	for( unsigned int i = 0; i < nbItems; i++ ) {	
		if( list[i]->type() == FightUnit::RTTI ) {
			if(((FightUnit*)list[ i ] )->hit( pos ) ){
				emit sig_mouseMoved( (FightCell *) ((FightUnit*)list[i])->getCell(), true);
				return;
			}
		} else if( list[i]->type() == FightCell::RTTI ) {
			emit sig_mouseMoved( (FightCell *) list[i] , false);
			return;
		}
	}
	emit sig_mouseMoved( NULL, true );
}

/*
 * handles mouse press event
 * emit a signal to say which cell is selected
 */
void FightMapView::mousePressEvent( QMouseEvent * e )
{
	QPointF pos = mapToScene(e->pos());
	QList<QGraphicsItem *> list = scene()->items( pos );
	uint nbItems = (uint)list.count();
	
	for( unsigned int i=0; i< nbItems; i++ ) {	
		if( list[i]->type() == FightUnit::RTTI ) {
			if(((FightUnit*)list[ i ] )->hit( pos ) ){
				if( e->button() == Qt::LeftButton ) { 
					emit sig_mouseLeftPressed( (FightCell *) ((FightUnit*)list[i])->getCell() , true);
					return;
				} else {
					emit sig_mouseRightPressed( (FightCell *) ((FightUnit*)list[i])->getCell() );
					return;
				}
			}
		} else if( list[i]->type() == FightCell::RTTI ) {
			if( e->button() != Qt::RightButton ) {
				emit sig_mouseLeftPressed( (FightCell *) list[i], false);
				return;
			}
		}
	}
}

/** handles mouse release event */

void FightMapView::mouseReleaseEvent( QMouseEvent *e )
{
	if( e->button() == Qt::RightButton )
		emit sig_mouseReleased();
}

void FightMapView::updateView()
{
	QList<QRectF> rectlist;
	rectlist.append( sceneRect() );
	updateScene( rectlist );
}
