/****************************************************************
**
** Attal : Lords of Doom
**
** mapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: mapView.h,v 1.10 2002/01/05 14:34:01 audoux Exp $
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
#include <qcanvas.h>

// application specific includes
#include "client/map.h"
#include "client/cell.h"
 
/*              ------------------------------
 *                         MapView
 *              ------------------------------ */



/** subclass of QCanvasSprite that handles mouse event ... in the map */
class MapView : public QCanvasView
{
	Q_OBJECT
public:
	/** Constructor */
	MapView( Map * map, QWidget * parent = 0, const char * name = 0, WFlags f = 0 );

signals:
	/** Signal if the mouse has moved */
	void sig_mouseMoved( Cell *cell );
	
	/** Signal if the mouse has been pressed (right button)*/
	void sig_mouseRightPressed( Cell *cell );
	
	/** Signal if the mouse has been pressed (left button)*/
	void sig_mouseLeftPressed( Cell *cell );
	
	void sig_mouseReleased();
			
protected:
	/** reimplemented : handles mouse event */
	void contentsMouseMoveEvent ( QMouseEvent * event );
	void contentsMouseReleaseEvent( QMouseEvent * event );
	void contentsMousePressEvent( QMouseEvent * event );
	void contentsMouseDoubleClickEvent( QMouseEvent * event );
	
	bool _press;
	Map * _map;
};
 
#endif // MAPVIEW_H   
