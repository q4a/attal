/****************************************************************
**
** Attal : Lords of Doom
**
** map.h
** Manage the global view
**
** Version : $Id: map.h,v 1.13 2002/01/09 20:13:19 audoux Exp $
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
#include "qarray.h"

// application specific includes
#include "common/genericMap.h"
#include "client/graphicalMap.h"
 
class QString;

/*              ------------------------------
 *                         Map
 *              ------------------------------ */



/** comment for the class */
class Map : public GraphicalMap , public GenericMap
{
public:
	/** Construtor */
	Map( QObject * parent = 0 , const char * name = 0 );

	/// XXX: Does we need a destructor for theCells ??
	/** Destructor */
	~Map();
	
	bool load( QTextStream * ts, int width, int heigth );
	
	/** Clear the map */
	void clearMap();
	
	/** Change the type of cell on the map */
	void changeCell( int i, int j, int typ, int transition, int typtra, int decoration );
	
	/** Create new map */
	void newMap( int h, int w );	
	
	/** Create new unknown map */
	void newUnknownMap( int h, int w );
	
	/** Resize the map to the right size */
	void autoSize() {
		resize( _width * 30, _height * 30 );
	}
};
 
#endif // MAP_H
