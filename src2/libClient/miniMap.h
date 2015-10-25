/****************************************************************
**
** Attal : Lords of Doom
**
** miniMap.h
** draw and manage the mini-map
**
** Version : $Id: miniMap.h,v 1.18 2008/06/15 18:34:22 lusum Exp $
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


#ifndef MINIMAP_H
#define MINIMAP_H


// generic include files
// include files for QT
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <QRect>

// application specific include files
#include "libCommon/genericCell.h"

#include "libClient/map.h"

/*              ------------------------------
 *                         MiniMap
 *              ------------------------------ */

class MiniMap : public QWidget
{
	Q_OBJECT
public:
	MiniMap( Map * map, QWidget * parent = NULL );
	~MiniMap();
public slots:
	void slot_mapviewScrolled( double x_percent, double y_percent );
	void slot_mapviewResized( double x_percent, double y_percent );
	void redrawCell( int row, int col );
	void redrawMap( Map * map );
signals:
	void sig_mouseReleasedMinimap( double, double );
protected:
	void _repaintMapPoint( uint row, uint col );
	void paintEvent( QPaintEvent * event );
	void mousePressEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
	
	static const int _defaultHeight;
	static const int _defaultWidth;

private:	
	void setMapSize( uint width, uint height );

	uint _oldHeight; // HACK
	uint _oldWidth; // HACK
	
	Map * _map;
	QPixmap * _minimap;
	static QPen _pen;
	QPainter _mappainter;
	QRect _viewportrect;
};

#endif // MINIMAP_H
