/****************************************************************
**
** Attal : Lords of Doom
**
** insideBase.cpp
** display the base and its buildings
**
** Version : $Id: insideBase.cpp,v 1.15 2003/01/12 20:27:58 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/10/2001
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

#include "insideBase.h"
 
// generic include files
// include files for QT
// application specific include files
#include "client/insideBuilding.h"
#include "client/imageTheme.h"
#include "common/genericInsideBuilding.h"
#include "common/genericBase.h"
#include "common/log.h"

extern QString IMAGE_PATH;
extern ImageTheme ImageTheme;

//
// ----- InsideBase -----
//
InsideBase::InsideBase( QObject * parent, const char * name )
    : QCanvas( parent , name )
{
	resize( 960, 720 );
	_list.setAutoDelete( true );
	_background = 0;
	_base = 0;
}

void InsideBase::clear()
{
	_list.clear();
	_base = 0;
}

void InsideBase::setBase( GenericBase * base )
{
	if( _base != base ) {
		clear();
		_base = base;
		setBackgroundPixmap( * ImageTheme.getInsideBase( _base->getRace() ) );
		_background = ImageTheme.getInsideBase( _base->getRace() );

		for( uint i = 0; i < base->getBuildingCount(); i++ ) {
			addBuilding( base->getBuilding( i ) );
		}
	}
}

void InsideBase::addBuilding( GenericInsideBuilding * building )
{
	/// XXX: add some checks (building not already added)
	InsideBuilding * inside = new InsideBuilding( _base->getRace(), building, this );
	inside->move( building->getX(), building->getY() );
	inside->show();
	_list.append( inside );
}

void InsideBase::delBuilding( GenericInsideBuilding * building )
{
	InsideBuilding * inside = 0;

	for( uint i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->getBuilding() == building ) {
			inside = _list.at( i );
			break;
		}
	}
	if( inside ) {
		_list.remove( inside );
	}
}

void InsideBase::drawBackground( QPainter& painter, const QRect& clip )
{
	if( _background ) {
		painter.fillRect( clip, Qt::black );
        	painter.drawPixmap( clip.x(), clip.y(), * _background, clip.x(), clip.y(), clip.width(), clip.height() );
	} else {
		painter.fillRect( clip, Qt::black );
	}
}

//
// ----- InsideBaseView -----
//

InsideBaseView::InsideBaseView( InsideBase * base, QWidget * parent, const char * name, WFlags f )
	: QCanvasView( base , parent, name, f )
{
	viewport()->setMouseTracking( true ) ;
	_selected = 0;
}

/** handles mouse move event */
void InsideBaseView::contentsMouseMoveEvent( QMouseEvent * e )
{
	QCanvasItemList list = canvas()->collisions(e->pos());
	for( unsigned int i = 0; i < list.count(); i++ ) {	
		if( list[i]->rtti() == InsideBuilding::RTTI ) {
			if( _selected ) {
				if( _selected != (InsideBuilding*)list[i] ) {
					_selected->deselect();
					_selected = (InsideBuilding*)list[i];
					_selected->select();
					canvas()->update();
				} 				
			} else {
				_selected = (InsideBuilding*)list[i];
				_selected->select();
				canvas()->update();
			}
		}
	}
	if( ( list.count() == 0 ) && _selected ) {
		_selected->deselect();
		_selected = 0;
		canvas()->update();
	}
}


/*
 * handles mouse press event
 */
void InsideBaseView::contentsMouseReleaseEvent( QMouseEvent * e )
{
	QCanvasItemList list = canvas()->collisions(e->pos());
	for( unsigned int i = 0; i < list.count(); i++ ) {	
		if( list[i]->rtti() == InsideBuilding::RTTI ) {
			if( e->button() == LeftButton ) {
				emit sig_building( ((InsideBuilding *)list[i])->getBuilding() );
			}
		}
	}
}

