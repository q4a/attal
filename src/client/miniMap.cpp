/****************************************************************
**
** Attal : Lords of Doom
**
** miniMap.cpp
** draw and manage the mini-map
**
** Version : $Id: miniMap.cpp,v 1.11 2003/01/16 17:40:25 lusum Exp $
**
** Author(s) : Pascal Audoux
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
#include <qpainter.h>
#include <qbrush.h>
// application specific includes
#include "common/log.h"
#include "common/dataTheme.h"

extern DataTheme DataTheme;

/** add comments here */
MiniMap::MiniMap( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	setMaximumSize( 200, 200 );
	setBackgroundColor( Qt::black );
	
	_qp = new QPixmap( 200, 200 );

}

/*!

*/

void MiniMap::redrawMap( GenericMap * map )
{
	int w = map->getWidth();
	int h = map->getHeight();

	if( isVisible() && ( w > 0 ) && ( h > 0 ) ) {
		_map = map;


		int sizeH = 200/w;
		int sizeV = 200/h;

		QPainter paint( _qp );
		QBrush brush;
		for( int i = 0; i < h; i++ ) {
			for( int j = 0; j < w; j++ ) {
				// XXX: to improve (color in theme ?)
				if( map->at( i, j )->getType() < (int)( DataTheme.tiles.count() ) ) {
					if( map->at( i, j )->getType() == 0 ) {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::black );
					} else if( map->at( i, j )->getCoeff() < 0 ) {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::blue );
					} else {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::gray );
					}
				} else {
					logEE( "Each cell should have a type" );
				}
			}
		}
		bitBlt( this, 0, 0, _qp );
	}
}


void MiniMap::redrawCell( GenericMap * map , int i, int j)
{
	int w = map->getWidth();
	int h = map->getHeight();

	if( isVisible() && ( w > 0 ) && ( h > 0 ) ) {
		_map = map;


		int sizeH = 200/w;
		int sizeV = 200/h;

		QPainter paint( _qp );
		QBrush brush;
				// XXX: to improve (color in theme ?)
				if( map->at( i, j )->getType() < (int)( DataTheme.tiles.count() ) ) {
					if( map->at( i, j )->getType() == 0 ) {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::black );
					} else if( map->at( i, j )->getCoeff() < 0 ) {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::blue );
					} else {
						paint.fillRect( j*sizeH, i*sizeV, sizeH, sizeV, Qt::gray );
					}
				} else {
					logEE( "Each cell should have a type" );
				}
		bitBlt( this, 0, 0, _qp );
	}
}



void MiniMap::paintEvent( QPaintEvent * event )
{
	QWidget::paintEvent( event );
	bitBlt( this, 0, 0, _qp );
}

void MiniMap::resizeEvent( QResizeEvent * event )
{
	QWidget::resizeEvent( event );
	_qp->resize( event->size() );
	_qp->fill( Qt::black );
	bitBlt( this, 0, 0, _qp );
}
