/****************************************************************
**
** Attal : Lords of Doom
**
** genericFightMap.h
**  generic class for managing (not graphical) fightMap
**
** Version : $Id: genericFightMap.h,v 1.18 2008/04/14 23:06:14 lusum Exp $
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

 
#ifndef GENERICFIGHTMAP_H
#define GENERICFIGHTMAP_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QStack>
// application specific include
#include "libCommon/genericFightCell.h" 


class PathFightFinder;
class GenericFightUnit;

/*              ------------------------------
 *                         GenericFightMap
 *              ------------------------------ */



/** generic class for managing (not graphical) cells */
class GenericFightMap
{

public:
	/** Constructor */
	GenericFightMap();

	/** Destructor */
	virtual ~GenericFightMap();

	/** Return cell of the fightMap */
	inline GenericFightCell * at( int i, int j )
		{ return _genericFightCells[i][j]; }

	/** Create new fight map */
	virtual void newFightMap( int h, int w, bool horizontalDraw = false );

	virtual void reinit();

	virtual void initPath( GenericFightUnit * unit );
	void printPath();
	virtual void clearPath();

	QStack<GenericFightCell *> computePath( GenericFightUnit * unit, GenericFightCell * cell );

	bool isPath( GenericFightUnit * unit , GenericFightCell * cell );

	bool isLowerLevel( GenericFightCell * cell1, GenericFightCell * cell2 );
	bool isUpperLevel( GenericFightCell * cell1, GenericFightCell * cell2 );
	bool isSameLevel( GenericFightCell * cell1, GenericFightCell * cell2 );

	GenericFightCell * getNeighbour1( GenericFightCell * cell );
	GenericFightCell * getNeighbour2( GenericFightCell * cell );
	GenericFightCell * getNeighbour3( GenericFightCell * cell );
	GenericFightCell * getNeighbour4( GenericFightCell * cell );
	GenericFightCell * getNeighbour5( GenericFightCell * cell );
	GenericFightCell * getNeighbour6( GenericFightCell * cell );

	bool areNeighbours( GenericFightCell * cell1, GenericFightCell * cell2 );

	int getHeight () { return _height; }

	int getWidth () { return _width; }

	bool inMap( int row, int col );

	GenericFightCell * getHeadCell( GenericFightCell * cell , bool isLookingToRight );

protected:
	GenericFightCell * giveNeighbourOnPath( GenericFightCell * cell, int dist );

	int _height, _width;
	PathFightFinder * _path;

	bool _horizontalDraw;

	GenericFightCell *** _genericFightCells;
};

/** pseudo-pile of GenericFightCell (in fact, a list..) */
class FightPile : public QList<GenericFightCell *>
{
public:
	/** Constrcutor */
	FightPile( GenericFightMap * map, int limit, GenericFightUnit* unit );

	/** \return Returns the 'smallest' cell */
	GenericFightCell * takeSmallest();

	/** Appends neighbours of a cell */
	void appendNeighbours( GenericFightCell * cell );
private:
	void handleNeighbour( GenericFightCell * neighbour, int dist );

	GenericFightUnit * testHeadFree( GenericFightCell * cell );

	bool testObstacle( GenericFightCell * cell );

	GenericFightMap * _map;
	int _limit;
	int _unitsize;
	GenericFightUnit * _unit;
};

#endif // GENERICFIGHTMAP_H   
