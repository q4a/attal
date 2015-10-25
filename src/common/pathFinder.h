/****************************************************************
**
** Attal : Lords of Doom
**
** pathFinder.h
** Compute pathes in the map
**
** Version : $Id: pathFinder.h,v 1.10 2003/01/03 17:00:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/08/2000
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

 
#ifndef PATHFINDER_H
#define PATHFINDER_H


// generic include files
// include files for QT
#include <qlist.h>
#include <qstack.h>
// application specific includes
#include "common/log.h"
#include "common/genericCell.h"
#include "common/genericMap.h"
#include "common/genericFightMap.h"
#include "common/genericFightCell.h"

#define COST_DIRECT 2
#define COST_DIAG 3

/** Structure for representing a path on the adventure map */
struct PathCell {
	GenericCell * cell;
	PathCell * prev;
       	int dist;

	friend bool sup( PathCell * a, PathCell * b )
        { return (a->dist > b->dist); }
};

/** structure of pathCell allowing to get the smallest */
class Pile : public QList<PathCell>
{
public:
	/** Constructor */
	Pile();

	/** Return the pathcell with smallest dist */
	PathCell * takeSmallest();

};

/** Structure for representing a path on the fight map */
struct PathFightCell {
	GenericFightCell * cell;
	PathFightCell * prev;
       	int dist;

	friend bool sup( PathFightCell * a, PathFightCell * b )
        { return (a->dist>b->dist); }
};

/*
class FightPile : public QList<PathFightCell>
{
public:
	** Constructor *
	FightPile();

	** Return the pathcell with smallest dist *
	PathFightCell * takeSmallest();

};
*/

/*              ------------------------------
 *                         PathFinder
 *              ------------------------------ */



/** Find the shortest path from A to B on the map */
class PathFinder
{

public:
	/** Constructor */
	PathFinder();

	/** Constructor */
	PathFinder( int h, int w, GenericMap * m );

	/** Destructor */
	~PathFinder();

        /** Reinit the pathfinder with current map */
	void reinit( GenericMap  * m );

	/** Precompute paths */
	void computePath( GenericCell * start );

	/** Return true if there is a path between 'start' and 'dest' */
	bool isPath( GenericCell * dest );

	int getDist( GenericCell * dest );

	/** Return true if there is a 'near path' between 'start' and 'dest' */
	bool isNearPath( GenericCell * dest );

	/** Return the 'near cell' */
	GenericCell * getNearCell( GenericCell * dest );

	/** Give next cell to go for dest */
	QStack<GenericCell> * giveCells( GenericCell * dest );
	
	/** Give next near cell to go for dest */	
	QStack<GenericCell> * giveNearCells( GenericCell * dest );
	
	/** Clear path */
	void clear();
	
	/** Change map */
	void newMap( int h, int w, GenericMap * m );
	
private:
	int _width, _height;	
	PathCell ** _table;
	PathCell * _start;
	
	void compute( Pile *, GenericCell *, PathCell *, int cost = 0 );
};

/*              ------------------------------
 *                         PathFightFinder
 *              ------------------------------ */



/** Find the shortest path from A to B on the map */
class PathFightFinder
{
 
public:
	/** Constructor */
	PathFightFinder( int w, int h, GenericFightMap * m );
	
	/** Destructor */
	~PathFightFinder();
	
        /** Reinit the pathfinder with current map */
	void reinit( GenericFightMap  * m );

	/** Precompute paths */
	void computePath( GenericFightCell * start );

	/** Give next cell to go for X, Y */
	QStack<GenericFightCell> * giveCells( GenericFightCell * dest );
	
private:
	int _width, _height;	
	PathFightCell ** _table;
	PathFightCell * _start;
};
  
#endif // PATHFINDER_H   
