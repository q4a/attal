/****************************************************************
**
** Attal : Lords of Doom
**
** mapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: mapView.cpp,v 1.23 2010/01/20 07:23:38 lusum Exp $
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
#include <QMouseEvent>
// application specific includes
#include "libCommon/log.h"
#include "libCommon/genericCell.h"

/** class handles mouse event ... */
MapView::MapView( Map * map, QWidget * parent )
	: QGraphicsView( map, parent ) 
{
	viewport()->setMouseTracking( false ) ;
	//not optimized, but avoid a bug in the background ( cell visible not drawn )
	setViewportUpdateMode ( QGraphicsView::FullViewportUpdate );
	_press = false;
	_map = map;
}


/** handles mouse event */
void MapView::mouseMoveEvent( QMouseEvent * event )
{	
	QPointF pos = mapToScene(event->pos());	
	if (scene()->sceneRect().contains( pos )) { // possibly unnecessary, but for safety...
		uint row = (uint)pos.y() / DataTheme.tiles.getHeight();
		uint col = (uint)pos.x() / DataTheme.tiles.getWidth() ;
		GenericCell * cell;
		try {
			cell = ((Map*)scene())->at( row, col );
			emit sig_mouseMoved( cell );
		} catch ( const char * err ) {
			//logEE("%s", err);
			return;
		}

	}
	
	//logDD(" pos x %d, y %d", (int) pos.x(),(int) pos.y());
	//logDD(" view pos x %d, y %d", (int) event->pos().x(),(int) event->pos().y());
}

void MapView::setScrollBarVisible( bool state )
{
	if( state ) {
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	} else {
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	}

}

void MapView::mousePressEvent( QMouseEvent * event )
{
	QPointF pos = mapToScene(event->pos());
	if (scene()->sceneRect().contains( pos )) { // possibly unnecessary, but for safety...
		uint row = (uint)pos.y() / DataTheme.tiles.getHeight();
		uint col = (uint)pos.x() / DataTheme.tiles.getWidth() ;
		GenericCell * cell;
		try {
			cell = ((Map*)scene())->at( row, col );
			if( event->button() == Qt::RightButton ) {
				emit sig_mouseRightPressed( cell );
			} else {
				emit sig_mouseLeftPressed( cell );
			}
			_press = true;
		} catch ( const char * err ) {
			//logEE("%s", err);
		}
	}
}

void MapView::mouseReleaseEvent( QMouseEvent * )
{
	_press = false;
	emit sig_mouseReleased();
}

void MapView::mouseDoubleClickEvent( QMouseEvent * event )
{
	mousePressEvent( event );
	mouseReleaseEvent( event );
}

void MapView::slot_Center( GenericCell * cell) 
{
	slot_Center( cell->getRow(), cell->getCol());
}

void MapView::slot_Center( int row, int col) 
{
	if( _map->inMap( row, col )) {
		this->centerOn( col * DataTheme.tiles.getWidth(), row * DataTheme.tiles.getHeight() );
	}
}

void MapView::slot_Center(double x, double y) {
	centerOn(_map->width() * x, _map->height() * y);
}
	
void MapView::slot_enableGame( bool enable )
{
	viewport()->setMouseTracking( enable ) ;
}

void MapView::resizeEvent( QResizeEvent * event )
{
	QGraphicsView::resizeEvent( event );
	//emit sig_viewportResized( viewport()->width() / sceneRect().width(), viewport()->height() / sceneRect().height() ); // NOTE correct way of doing it
	emit sig_viewportResized( width() / sceneRect().width(), height() / sceneRect().height() ); // HACK to workaround a stupid Qt 4.2 bug... They released it too early...
}

void MapView::scrollContentsBy(int dx, int dy)
{
	QGraphicsView::scrollContentsBy( dx, dy );
	
	emit sig_viewportScrolled(
	std::max(0.0, (double)mapToScene(rect()).boundingRect().x() / (double)sceneRect().width()),
	std::max(0.0, (double)mapToScene(rect()).boundingRect().y() / (double)sceneRect().height()));
	
}

//no more used
void MapView::updateMap()
{
	TRACE("updateMap");
	updateMapRect(sceneRect());
}

//no more used
void MapView::updateMapRect( QRectF rect )
{
	TRACE("updateMapRect rect.x() %f, rect.y() %f, width() %f, height() %f  ", rect.x(), rect.y(), rect.width(), rect.height());
	QList<QRectF> rectlist;
	rectlist.append( rect );
	_map->updateMapRect(sceneRect());
}
