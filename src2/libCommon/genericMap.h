/****************************************************************
**
** Attal : Lords of Doom
**
** genericMap.h
**  generic class for managing (not graphical) map
**
** Version : $Id: genericMap.h,v 1.17 2008/07/14 22:29:49 lusum Exp $
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
#include <QTextStream>
// application specific includes
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

class QString;
class QTextStream;
class PathFinder;
class GenericBase;
class GenericLord;
class GenericDecoration;
 
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
	uint getHeight() { return _height; }
	
	/** Return width of the map */
	uint getWidth() { return _width; }

	/** Return cell of the map */
	GenericCell * at( uint i, uint j );
	
	/** */
	virtual void newUnknownMap( int h, int w );

	virtual void newMapType( int h, int w, int typ );

	/** Return the path finder */
	PathFinder * getPath() { return _path; }

	/** Load a map file */
	virtual bool load( QTextStream *, int width, int heigth );
		
	/** Load a map */
	virtual bool load( const QString filename );

	virtual void changeCell( int i, int j, int typ,
		      int transition, int typtra, uint decorationGroup, uint decorationItem, uchar diversification );
	
	/** Save a map */
	void save( QTextStream * );

	/** Clear map */
	virtual void clear();
	
	virtual void clearData();

	/** Compute the tiles where we can't stop because of this base */
	void computeStoppable( GenericBase * base );
	
	/** Compute the tiles where we can't stop because of this building */
	void computeStoppable( GenericBuilding * building );

	void computeStoppable( GenericDecoration * deco , GenericCell * cell);

	/** Compute the tiles where we can't stop because there is a base on it */
	void computeStoppable();
	
	static int computeDist( GenericCell * cell1, GenericCell * cell2 );
	
	static inline int computeDist( int row1, int col1 , int row2, int col2 );
	
	bool isPlaceBaseFree( int race , GenericCell * cell);
	
	bool isPlaceBuilFree( int type , GenericCell * cell);

	bool isPlaceDecoFree( GenericDecoration * deco , GenericCell * cell);

	bool inMap( uint row, uint col );

	QList<GenericCell *> giveRadiusCell( GenericCell * cell, int radius );

	/** Returns the minimal cost for the next movement of this lord */
	int computeMinimalNextCost( GenericLord * lord );

protected:
	uint _height, _width;
	GenericCell *** _theCells;
	PathFinder * _path;
};
 
#endif // GENERICMAP_H   
