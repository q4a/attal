/****************************************************************
**
** Attal : Lords of Doom
**
** genericFightMap.h
**  generic class for managing (not graphical) fightMap
**
** Version : $Id: genericFightMap.h,v 1.11 2002/10/21 20:55:18 audoux Exp $
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
#include <qlist.h>
#include <qstack.h>
// application specific include
#include "common/genericFightCell.h" 


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
	GenericFightCell * at( int i, int j )
		{ return _genericFightCells[i][j]; } 

	/** Create new fight map */
	virtual void newFightMap( int h, int w );

	virtual void reinit();
	
	virtual void initPath( GenericFightUnit * unit );
	void printPath();
	virtual void clearPath();
	
	QStack<GenericFightCell> computePath( GenericFightUnit * unit, GenericFightCell * cell );
	
	GenericFightCell * getN( GenericFightCell * cell );
	GenericFightCell * getNE( GenericFightCell * cell );
	GenericFightCell * getSE( GenericFightCell * cell );
	GenericFightCell * getS( GenericFightCell * cell );
	GenericFightCell * getSW( GenericFightCell * cell );
	GenericFightCell * getNW( GenericFightCell * cell );	
	
	bool areNeighbour( GenericFightCell * cell1, GenericFightCell * cell2 );
	
protected:
	GenericFightCell * giveNeighbourOnPath( GenericFightCell * cell, int dist );
	
	int _height, _width;
	PathFightFinder * _path;
	
	GenericFightCell *** _genericFightCells;
};

/** pseudo-pile of GenericFightCell (in fact, a list..) */
class FightPile : public QList<GenericFightCell>
{
public:
	/** Constrcutor */
	FightPile( GenericFightMap * map, int limit );

	/** \return Returns the 'smallest' cell */
	GenericFightCell * takeSmallest();

	/** Appends neighbours of a cell */
	void appendNeighbours( GenericFightCell * cell );
private:
	void handleNeighbour( GenericFightCell * neighbour, int dist );
	
	GenericFightMap * _map;
	int _limit;
};

#endif // GENERICFIGHTMAP_H   
