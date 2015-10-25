/****************************************************************
**
** Attal : Lords of Doom
**
** genericMap.h
**  generic class for managing (not graphical) map
**
** Version : $Id: genericMap.h,v 1.14 2002/11/16 16:35:18 lusum Exp $
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

 
#ifndef GENERICMAP_H
#define GENERICMAP_H
 
 
// generic include files
// include files for QT
// application specific includes
#include "common/genericCell.h"
#include "common/log.h"

class QString;
class QTextStream;
class PathFinder;
class GenericBase;
class GenericLord;
 
/*              ------------------------------
 *                         GenericMap
 *              ------------------------------ */



/** generic class for managing (not graphical) cells */
class GenericMap
{
 
public:
	/** Constructor */
	GenericMap();
	
	/** Destructor */
	virtual ~GenericMap();

	/** Return heigth of the map */
	int getHeight() { return _height; }
	
	/** Return width of the map */
	int getWidth() { return _width; }

	/** Return cell of the map */
	GenericCell * at( int i, int j )
		{ return _theCells[i][j]; }

	/** */
	virtual void newUnknownMap( int h, int w );

	/** Return the path finder */
	PathFinder * getPath() { return _path; }

	/** Load a map file */
	virtual bool load( QTextStream *, int width, int heigth );
		
	/** Load a map */
	virtual bool load( const QString filename );

	virtual void changeCell( int i, int j, int typ,
		      int transition, int typtra, int decoration );
	
	/** Save a map */
	void save( QTextStream * );

	/** Clear map */
	void clear();
	
	void cleanData();

	/** Compute the tiles where we can't stop because of this base */
	void computeStoppable( GenericBase * base );

	/** Compute the tiles where we can't stop because there is a base on it */
	void computeStoppable();

	/** Returns the minimal cost for the next movement of this lord */
	int computeMinimalNextCost( GenericLord * lord );

protected:
	int _height, _width;
	GenericCell *** _theCells;
	PathFinder * _path;
};
 
#endif // GENERICMAP_H   
