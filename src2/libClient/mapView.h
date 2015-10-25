/****************************************************************
**
** Attal : Lords of Doom
**
** mapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: mapView.h,v 1.16 2010/01/20 07:23:38 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

 
#ifndef MAPVIEW_H
#define MAPVIEW_H
 
 
// generic include files
// include files for QT
#include <QGraphicsView>
#include <QMouseEvent>

// application specific includes
#include "libClient/map.h"
#include "libCommon/genericCell.h"
 
/*              ------------------------------
 *                         MapView
 *              ------------------------------ */



/** subclass of QCanvasSprite that handles mouse event ... in the map */
class MapView : public QGraphicsView
{
	Q_OBJECT
public:
	/** Constructor */
	MapView( Map * map, QWidget * parent = 0 );

	void goToPosition(GenericCell * cell);

	void setScrollBarVisible( bool state );
	
public slots:
	void slot_Center(double x, double y);
	
	void slot_Center(int row, int col);
	
	void slot_Center( GenericCell * cell);
	
	void slot_enableGame( bool enable );
	
	void updateMap();
	
	void updateMapRect( QRectF rect );

signals:
	/** Signal if the mouse has moved */
	void sig_mouseMoved( GenericCell *cell );
	
	/** Signal if the mouse has been pressed (right button)*/
	void sig_mouseRightPressed( GenericCell *cell );
	
	/** Signal if the mouse has been pressed (left button)*/
	void sig_mouseLeftPressed( GenericCell *cell );
	
	void sig_mouseReleased();
	
	void sig_viewportResized( double, double );
	void sig_viewportScrolled( double, double );

protected:
	/** reimplemented : handles mouse event */
	void mouseMoveEvent ( QMouseEvent * event );
	void mouseReleaseEvent( QMouseEvent * event );
	void mousePressEvent( QMouseEvent * event );
	void mouseDoubleClickEvent( QMouseEvent * event );
	void resizeEvent( QResizeEvent * event );
	void scrollContentsBy( int dx, int dy );
	
	bool _press;
	Map * _map;
};
 
#endif // MAPVIEW_H   
