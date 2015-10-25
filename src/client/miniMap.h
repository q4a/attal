/****************************************************************
**
** Attal : Lords of Doom
**
** miniMap.h
** draw and manage the mini-map
**
** Version : $Id: miniMap.h,v 1.4 2003/01/16 17:40:25 lusum Exp $
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

 
#ifndef MINIMAP_H
#define MINIMAP_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
#include <qpixmap.h>
// application specific includes
#include "common/genericMap.h" 

 
/*              ------------------------------
 *                         MiniMap
 *              ------------------------------ */



/** comment for the class */
class MiniMap : public QWidget
{
 
public:
	/** Constructor */
	MiniMap( QWidget * parent = 0, const char * name = 0 );
	
	/** Redraw the mini map */
	void redrawMap( GenericMap * map );
	
	void redrawCell( GenericMap * map, int i , int j );
	
protected:
	virtual void paintEvent( QPaintEvent * );
	virtual void resizeEvent( QResizeEvent * );
	
private:
	QPixmap * _qp;
	GenericMap * _map; 
};
 
#endif // MINIMAP_H   
