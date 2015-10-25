/****************************************************************
**
** Attal : Lords of Doom
**
** map.h
** Manage the global view
**
** Version : $Id: map.h,v 1.18 2008/07/01 21:41:36 lusum Exp $
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

 
#ifndef MAP_H
#define MAP_H
 
 
// generic include files
// include files for QT
#include <QTextStream>
#include <QPainter>
#include <QRectF>
// application specific includes
#include "libCommon/genericMap.h"
#include "libCommon/dataTheme.h"

#include "libClient/graphicalMap.h"

class GraphicalPath;

/*              ------------------------------
 *                         Map
 *              ------------------------------ */



/** comment for the class */
class Map : public GraphicalMap , public GenericMap
{
	Q_OBJECT
public:
	/** Construtor */
	Map( QObject * parent = 0 , const char * name = 0 );

	/// XXX: Does we need a destructor for theCells ??
	/** Destructor */
	~Map();
	
	bool load( QTextStream * ts, int width, int heigth );
	
	/** Clear the map */
	virtual void clear();

	/** Change the type of cell on the map */
	void changeCell( int i, int j, int typ, int transition, int typtra, uint decorationGroup, uint decorationItem , uchar diversification );
	
	/** Create new unknown map */
	void newUnknownMap( int h, int w );	
	
	/** Create new map */
	void newMapType( int h, int w, int typ );
	
	void updateMap();

	void updateMapRect( QRectF rect );
	
	/** Resize the map to the right size */
	void autoSize() {
		setSceneRect( QRectF ( 0 , 0, _width * DataTheme.tiles.getWidth(), _height * DataTheme.tiles.getHeight()) );
	}
	
	void activateGrid();

	/** Return the graphical path finder */
	GraphicalPath * getGraphicalPath() { return _showPath; }

signals: 
	void sig_cellChanged( int , int );

public slots:
	void clearPath();

protected:
	void drawBackground( QPainter * painter , const QRectF & rect );
	void drawForeground( QPainter * painter , const QRectF & rect );
private:
	bool _isGrid;
	GraphicalPath * _showPath;
};
 
#endif // MAP_H
