/****************************************************************
**
** Attal : Lords of Doom
**
** fightMap.cpp
** Manage the global view
**
** Version : $Id: genericFightMap.cpp,v 1.10 2003/01/06 21:51:22 audoux Exp $
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

#include "genericFightMap.h"

 
// generic include files
#include <stdlib.h>
// include files for QT
#include <qstring.h>
#include <qstack.h>
#include <qfile.h>
#include <qtextstream.h>
// application specific include
#include "common/log.h"
#include "common/pathFinder.h"
#include "common/unit.h"

bool isEven( int number )
{
	return( ( number % 2 ) == 0 );
}

bool isOdd( int number )
{
	return( ( number % 2 ) == 1 );
}



/** add comments here */
GenericFightMap::GenericFightMap()
{
	_height = 0;
	_width = 0;
	
	_genericFightCells = 0;
}

GenericFightMap::~GenericFightMap()
{
       	for( int i=0; i < _height; i++ ) {
		delete [] _genericFightCells[i];
	}
	delete [] _genericFightCells;
}




/*!

*/

void GenericFightMap::newFightMap( int h, int w )
{
	_height = h;
	_width = w;
	
	_genericFightCells = new GenericFightCell **[_height];
	
	for( int i = 0; i < _height; i++ ) {
		_genericFightCells[i] = new GenericFightCell * [_width];
		for( int j = 0; j < _width; j++ ) {
			GenericFightCell * cell = new GenericFightCell( i, j );
			_genericFightCells[i][j] = cell;
		}
	}
}

void GenericFightMap::reinit()
{
	/// XXX: to rewrite... not to use...
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			_genericFightCells[i][j]->setUnit( 0 );
		}
	}
}

void GenericFightMap::clearPath()
{
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			_genericFightCells[i][j]->setAccess( UNKNOWN_ACCESS );
			_genericFightCells[i][j]->setDist( 0 );
		}
	}	
}

void GenericFightMap::initPath( GenericFightUnit * unit )
{
	GenericFightCell * current = unit->getCell();
	FightPile pile( this, unit->getMove() );

	GenericFightMap::clearPath();

	current->setAccess( NONE );
	current->setDist( 0 );

	pile.appendNeighbours( current );
	while( ! pile.isEmpty() ) {
		current = pile.takeSmallest();
		pile.appendNeighbours( current );
	}

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			if( _genericFightCells[i][j]->getAccess() == UNKNOWN_ACCESS ) {
				if ( _genericFightCells[i][j]->getUnit() ) {
					_genericFightCells[i][j]->setAccess( FAR_OCCUPIED );
				} else {
					_genericFightCells[i][j]->setAccess( FAR );
				}
			}
		}
	}	
}

void GenericFightMap::printPath()
{
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			logDD("%d-%d = %d (%d)", i, j, _genericFightCells[i][j]->getDist(), _genericFightCells[i][j]->getAccess() );
		}
	}		
}

QStack<GenericFightCell> GenericFightMap::computePath( GenericFightUnit * unit, GenericFightCell * cell )
{
	QStack<GenericFightCell> path;
	int dist = cell->getDist();
	GenericFightCell * current = cell;
	
	path.push( cell );
	for( int i = dist - 1; i > 0; i-- ) {
		current = giveNeighbourOnPath( current, i );
		path.push( current );
	}
	//path.push( unit->getCell() );
	
	return path;
	
}

GenericFightCell * GenericFightMap::giveNeighbourOnPath( GenericFightCell * cell, int dist )
{
	GenericFightCell * current;
	GenericFightCell * ret = 0;
	
	current = getN( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	current = getNE( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	current = getSE( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	current = getS( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	current = getSW( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	current = getNW( cell );
	if( current && ( current->getDist() == dist ) ) {
		ret = current;
	}
	
	if( ! ret ) {
		logEE( "Should not happen : no cell found ??" );
	}
	
	return ret;
}

GenericFightCell * GenericFightMap::getN( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;
	
	if( row > 0 ) {
		ret = at( row - 1, col );
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNE( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;
	
	if( isEven( col ) ) {
		if( ( row > 0 ) && ( col < _width-1 ) ){
			ret = at( row - 1, col + 1 );
		}
	} else {
		if( col < _width-1 ) {
			ret = at( row, col + 1 );
		}
	}
	
	return ret;
}

GenericFightCell * GenericFightMap::getSE( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;
	
	if( isEven( col ) ) {
		if( col < _width - 1 ) {
			ret = at( row, col + 1 );
		}
	} else {
		if( ( col < _width - 1 ) && ( row < _height - 1 ) ) {
			ret = at( row + 1, col + 1 );
		}
	}	
	
	return ret;
}

GenericFightCell * GenericFightMap::getS( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if( row < _height - 1 ) {
		ret = at( row + 1, col );
	} 
		
	return ret;
}

GenericFightCell * GenericFightMap::getSW( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;
		
	if( isEven( col ) ) {
		if( col > 0 ) {
			ret = at( row, col - 1 );
		}
	} else {
		if( ( row < _height - 1 ) && ( col > 0 ) ) {
			ret = at( row + 1, col - 1 );
		}
	}
	
	return ret;
}

GenericFightCell * GenericFightMap::getNW( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;
	
	if( isEven( col ) ) {
		if( ( row > 0 ) && ( col > 0 ) ) {
			ret = at( row - 1, col - 1 );
		}
	} else {
		if( col > 0 ) {
			ret = at( row, col - 1 );
		}
	}
	
	return ret;
}

bool GenericFightMap::areNeighbour( GenericFightCell * cell1, GenericFightCell * cell2 )
{
	bool ret = false;
	
	if( ( getN( cell1 ) == cell2 ) ||
	    ( getNE( cell1 ) == cell2 ) ||
	    ( getSE( cell1 ) == cell2 ) ||
	    ( getS( cell1 ) == cell2 ) ||
	    ( getSW( cell1 ) == cell2 ) ||
	    ( getNW( cell1 ) == cell2 ) ) {
		ret = true;
	}
	
	return ret;
}

/*********************************************/
FightPile::FightPile( GenericFightMap * map, int limit )
{
	_map = map;
	_limit = limit;
}

GenericFightCell * FightPile::takeSmallest()
{
	GenericFightCell * smallest = first();
	int minimum = at();
	GenericFightCell * current = next();
	
	while( current ) {
		if( smallest->getDist() > current->getDist() ) {
			smallest = current;
			minimum = at();
		}
		current = next();
	}
	
	return take( minimum ); 
}

void FightPile::appendNeighbours( GenericFightCell * cell )
{
	int dist = cell->getDist() + 1;
	
	handleNeighbour( _map->getN( cell ), dist );
	handleNeighbour( _map->getNE( cell ), dist );
	handleNeighbour( _map->getSE( cell ), dist );	
	handleNeighbour( _map->getS( cell ), dist );
	handleNeighbour( _map->getSW( cell ), dist );	
	handleNeighbour( _map->getNW( cell ), dist );	
}

void FightPile::handleNeighbour( GenericFightCell * neighbour, int dist )
{
	if( neighbour ) {
		if( neighbour->getType() == OBSTACLE ) {
			neighbour->setAccess( NONE );
		}

		switch( neighbour->getAccess() ) {
		case UNKNOWN_ACCESS:
			if( dist <= _limit ) {
				if( neighbour->getUnit() ) {
					neighbour->setAccess( NEAR_OCCUPIED );
					neighbour->setDist( dist );
				} else {
					neighbour->setAccess( NEAR );
					neighbour->setDist( dist );
					append( neighbour );
				}
			} else if( neighbour->getUnit() ) {
				neighbour->setAccess( FAR_OCCUPIED );
				neighbour->setDist( dist );
			} else {
				neighbour->setAccess( FAR );
				neighbour->setDist( dist );
			}
			break;
		case NONE:
			break;
		case NEAR:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
				append( neighbour );
			}
			break;
		case NEAR_OCCUPIED:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			break;
		case FAR:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			if( neighbour->getDist() <= _limit ) {
				neighbour->setAccess( NEAR );
				append( neighbour );
			}
			break;
		case FAR_OCCUPIED:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			if( neighbour->getDist() <= _limit ) {
				neighbour->setAccess( NEAR_OCCUPIED );
			}
			break;
		default:
			logEE( "Should not happen" );
			break;
		}
	}
}

