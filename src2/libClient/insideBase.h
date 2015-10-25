/****************************************************************
**
** Attal : Lords of Doom
**
** insideBase.h
** display the base and its buildings
**
** Version : $Id: insideBase.h,v 1.14 2008/07/01 00:18:56 lusum Exp $
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
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// application specific include files
#include "libClient/insideBuilding.h"

class GenericInsideBuilding;
class GenericBase;
class QPixmap;

/*              ------------------------------
 *                         InsideBase
 *              ------------------------------ */


/** Canvas for inside base */
class InsideBase : public QGraphicsScene
{

public:
	/** Constructor */
	InsideBase( QObject * parent = 0, GenericBase * base = 0 );

	/** Destructor */
	virtual ~InsideBase();

	/** Clear the canvas */
	void clear();
	
	void reinit();

	/** Set base to this object */
	void setBase( GenericBase * base );

	/** Adds a building to the base (graphics) */
	void addBuilding( GenericInsideBuilding * building );

	/** Delete building of a base (graphics) */
	void delBuilding( GenericInsideBuilding * building );
	
	/** Delete building of a base (graphics) */
	void delBuilding( uint level );
	
	void setNewBase( bool newBase ) { _newBase = newBase; }
	
	bool getNewBase() { return _newBase; }

protected:
	void drawBackground( QPainter * painter, const QRectF & rect );

	void drawItems( QPainter * painter, int numItems,  QGraphicsItem *items[],const QStyleOptionGraphicsItem options[] , QWidget *widget);

private:
	QList<InsideBuilding *> _list;
	QPixmap * _background;
	GenericBase * _base;
	bool _newBase;
};

/** canvas view for inside base  */
class InsideBaseView : public QGraphicsView
{
	Q_OBJECT
public:
	/** Constructor */
	InsideBaseView( InsideBase * baseScene, QWidget * parent = 0 );

	/** Destructor */
	~InsideBaseView();
	
signals:
	/** Signal if a building is clicked */
	void sig_building( GenericInsideBuilding * );

	void sig_update();
	
public slots:
	void newMessage( QString string);

	void slot_removeMessage();
	
protected:
	/** reimplemented : handles mouse event */
	void mouseMoveEvent( QMouseEvent * e );
	void mouseReleaseEvent( QMouseEvent * e );
	void scrollContentsBy( int dx, int dy );
	void drawForeground( QPainter * painter , const QRectF & rect );
	
	QList<QString> _listM;

	InsideBuilding * _selected;
	InsideBase * _scene;
};
 
#endif // INSIDEBASE_H   
