/****************************************************************
**
** Attal : Lords of Doom
**
** fightMapView.cpp
** subclass of QCanvasSprite that handles mouse event ...
**
** Version : $Id: fightMapView.h,v 1.9 2008/05/05 17:12:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 17/10/2000
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

 
#ifndef FIGHTMAPVIEW_H
#define FIGHTMAPVIEW_H
 
 
// generic include files
// include files for QT
#include <QGraphicsView>
#include <QMouseEvent>

// application specific includes
#include "libFight/fightMap.h"
#include "libFight/fightCell.h"

 
 
/*              ------------------------------
 *                         FightMapView
 *              ------------------------------ */



/** subclass of QCanvasView that handles mouse event ... */
class FightMapView : public QGraphicsView
{
	Q_OBJECT
	
public:
	/** Constructor */
	FightMapView( FightMap * map, QWidget * parent = 0, const char * name = 0, Qt::WFlags f = 0 );
		
signals:
	/** Indicates where the mouse has been moved */
	void sig_mouseMoved( FightCell * cell, bool isUnit );
	
	/** Indicates where the mouse has been presses with right button */
	void sig_mouseRightPressed( FightCell * cell );
	
	/** Indicates where the mouse has been presses with left button */
	void sig_mouseLeftPressed( FightCell * cell , bool isUnit);
	
	/** Indicated that the mouse has been released */
	void sig_mouseReleased();

public slots:
	void updateView();
	
protected:
	/** reimplemented : handles mouse event */
	void mouseMoveEvent ( QMouseEvent *e );
	void mouseReleaseEvent( QMouseEvent *e );
	void mousePressEvent( QMouseEvent *e );
	
};
 
#endif // FIGHTMAPVIEW_H   
