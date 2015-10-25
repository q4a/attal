/****************************************************************
**
** Attal : Lords of Doom
**
** insideBase.h
** display the base and its buildings
**
** Version : $Id: insideBase.h,v 1.13 2003/01/12 20:28:04 audoux Exp $
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

 
#ifndef INSIDEBASE_H
#define INSIDEBASE_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>
#include <qpainter.h>
// application specific include files
#include "client/insideBuilding.h"

class GenericInsideBuilding;
class GenericBase;
class QPixmap;

/*              ------------------------------
 *                         InsideBase
 *              ------------------------------ */


/** Canvas for inside base */
class InsideBase : public QCanvas
{

public:
	/** Constructor */
	InsideBase( QObject * parent = 0, const char * name = 0 );

	/** Clear the canvas */
	void clear();

	/** Set base to this object */
	void setBase( GenericBase * base );

	/** Adds a building to the base (graphics) */
	void addBuilding( GenericInsideBuilding * building );

	/** Adds a building to the base (graphics) */
	void delBuilding( GenericInsideBuilding * building );

protected:
	void drawBackground( QPainter& painter, const QRect & clip );

private:
	QList<InsideBuilding> _list;
	QPixmap * _background;
	GenericBase * _base;
};

/** canvas view for inside base  */
class InsideBaseView : public QCanvasView
{
	Q_OBJECT
public:
	/** Constructor */
	InsideBaseView( InsideBase * base, QWidget * parent = 0, const char * name = 0, WFlags f = 0 );

signals:
	/** Signal if a building is clicked */
	void sig_building( GenericInsideBuilding * );
	
protected:
	/** reimplemented : handles mouse event */
	void contentsMouseMoveEvent( QMouseEvent * e );
	void contentsMouseReleaseEvent( QMouseEvent * e );
	
	InsideBuilding * _selected;
};
 
#endif // INSIDEBASE_H   
