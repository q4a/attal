/****************************************************************
**
** Attal : Lords of Doom
**
** miniMap.cpp
** draw and manage the mini-map
**
** Version : $Id: miniMap.cpp,v 1.39 2008/11/09 18:49:31 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo - Forest Darling
**
** Date : 05/12/2000
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

#include "miniMap.h"


// generic include files
// include files for QT
////#include <QBrush>
////#include <QResizeEvent>

// application specific includes
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericPlayer.h"

QPen MiniMap::_pen = QPen( Qt::white, 1, Qt::DashLine );
const int MiniMap::_defaultHeight = 150;
const int MiniMap::_defaultWidth = 150;

MiniMap::MiniMap( Map * map, QWidget * parent )
{
	_map = map; // it gets set anyways in redrawMap();
	_minimap = NULL;
	_oldHeight = _map->getHeight();
	_oldWidth = _map->getWidth();
	redrawMap( _map );
	_viewportrect.setRect( 0, 0, 1, 1 );
	
	connect( parent, SIGNAL( sig_cellChanged( int, int )), SLOT( redrawCell( int , int ) ) );
	connect( parent, SIGNAL( sig_redrawMap( Map * )), SLOT( redrawMap( Map * ) ) );
	// TODO possible set the background to black? fill the _minimap with black?
	/*QPalette palette;
	palette.setColor( backgroundRole(), Qt::black );
	setPalette( palette );
	setAutoFillBackground( true );*/
	//setAttribute( Qt::WA_PaintOutsidePaintEvent, true ); // TODO is it needed?
}

MiniMap::~MiniMap( )
{
	if( _minimap ) delete _minimap;
}

void MiniMap::redrawMap(Map * map)
{
	_map = map;
	uint width = _map->getWidth();
	uint height = _map->getHeight();
	
	setMapSize( width , height );
	
	if (_minimap) delete _minimap;
	_minimap = new QPixmap(  width , height );
	
	if ( !_minimap->isNull() ) {
		_mappainter.begin( _minimap );
		
		for( uint row = 0; row < height; row++ ) {
			for( uint col = 0; col < width; col++ ) {
				_repaintMapPoint( row, col );
			}
		}
		_mappainter.end();
	}
	update();
}

void MiniMap::redrawCell( int row, int col )
{
	int mwidth =_map->getWidth();
	int mheight =_map->getHeight();
	if( /*!isHidden() && */ mwidth && mheight ) { // TODO is the !isHidden() really needed?
		_mappainter.begin( _minimap );
		_repaintMapPoint( row , col );
		_mappainter.end();
		update( col * width() / mwidth, row * height() / mheight, width() / mwidth + 1, height() / mheight + 1 );
	}
}

void MiniMap::_repaintMapPoint( uint row, uint col )
{
	static QColor c = Qt::red; // to show unimplemented colors
	
	if( _map->getHeight() && _map->getWidth() ) {
		GenericCell * cell = _map->at( row, col );
		
		int type = cell->getType();
		if( type < DataTheme.tiles.count() ) {
			if( type ) {
				c = DataTheme.tiles.at( type )->getColor();
				if( !cell->isStoppable() ) {
					//c = c.dark();
				}
			} else {
				c = Qt::black;
			}
		} else {
			logEE( "Each cell should have a type" );
		}
		
		if( cell->getBase() ) {
			if( cell->getBase()->getOwner() ) {
				c = DataTheme.teams.at( cell->getBase()->getOwner()->getTeamId() )->getColor();
			} else {
				c = Qt::gray;
			}
		}

		if( cell->getBuilding() ) {
			if( cell->getBuilding()->getOwner() ) {
				c = DataTheme.teams.at( cell->getBuilding()->getOwner()->getTeamId() )->getColor();
			} else {
				c = Qt::gray;
			}
		}

		if( cell->getLord() ) {
			if( cell->getLord()->getOwner() ) {
				c = DataTheme.teams.at( cell->getLord()->getOwner()->getTeamId() )->getColor();
			}
		}
		if( cell->getCreature() ) {
			c = Qt::gray;
		}
	}
	
	_mappainter.setPen( c );
	_mappainter.drawPoint( col, row );
}

void MiniMap::paintEvent(QPaintEvent * /*event*/)
{
	QPainter p( this );
	p.setPen( _pen );
	
	if( _minimap->isNull() ) {
		p.fillRect( 0, 0, width(), height(), Qt::black);
	} else {
		p.drawPixmap( 0, 0, width(), height(), *_minimap );
	}
	p.drawRect( _viewportrect.x(), _viewportrect.y(), _viewportrect.width() - 1, _viewportrect.height() - 1 );
}

void MiniMap::mousePressEvent( QMouseEvent * event )
{
	emit sig_mouseReleasedMinimap( (double)event->x() / (double)width(), (double)event->y() / (double)height() );
}

void MiniMap::mouseMoveEvent( QMouseEvent * event )
{
	mousePressEvent( event );
}

void MiniMap::slot_mapviewScrolled(double x_percent, double y_percent) {
	update( _viewportrect );
	_viewportrect.moveTo( (int)(width() * x_percent), (int)(height() * y_percent) );
	update( _viewportrect );
}

void MiniMap::slot_mapviewResized( double width_percent, double height_percent ) { // TODO fix the rect overlapping for a slight performance gain
	int newheight, newwidth;

	if( width_percent >= 1.0 ) {
		newwidth = width();
	} else {
		newwidth = (int)( width() * width_percent );
	}
	if( height_percent >= 1.0 ) {
		newheight = height();
	} else {
		newheight = (int)( height() * height_percent );
	}

	if( newwidth != _viewportrect.width() || newheight != _viewportrect.height() ) {
		update( _viewportrect );
		_viewportrect.setWidth( newwidth );
		_viewportrect.setHeight( newheight );
		update( _viewportrect );
	}
}

void MiniMap::setMapSize( uint/* width */, uint /*height*/ )
{
	// In either case, I doubt the "std::max( 0..." is necessary because there aren't any negative numbers involved and it would never round to below zero, but it can't hurt. It's possible that uint<-->int casting could be causing problems?
	// This one does the calculation based on the GenericMap's aspect ratio.
	/*setFixedSize(
				std::max( 0, (int)std::min( (double)width / (double)height * (double)_defaultWidth, (double)_defaultWidth ) ),
				std::max( 0, (int)std::min( (double)height / (double)width * (double)_defaultHeight, (double)_defaultHeight ) ) );*/
	// This one does it based on the GraphicalMap's aspect ratio, which can be set while the GenericMap is still empty. The MapView has a scrollable area before a map is even loaded...
	setFixedSize(
				std::max( 0, (int)std::min( (double)_map->width() / (double)_map->height() * (double)_defaultWidth, (double)_defaultWidth ) ),
				std::max( 0, (int)std::min( (double)_map->height() / (double)_map->width() * (double)_defaultHeight, (double)_defaultHeight ) ) );
}


