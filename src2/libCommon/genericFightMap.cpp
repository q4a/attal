/****************************************************************
**
** Attal : Lords of Doom
**
** fightMap.cpp
** Manage the global view
**
** Version : $Id: genericFightMap.cpp,v 1.28 2008/04/14 23:06:14 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include <QFile>
#include <QString>
#include <QTextStream>
// application specific include
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/unit.h"
#include "libCommon/creature.h"

inline bool isEven( int number )
{
	return( ( number % 2 ) == 0 );
}

//
// ----- GenericFightMap -----
//

GenericFightMap::GenericFightMap()
{
	_height = 0;
	_width = 0;
	
	_genericFightCells = 0;
}

GenericFightMap::~GenericFightMap()
{
	reinit();
	if(_genericFightCells != 0){
		for( int i=0; i < _height; i++ ) {
			for( int j = 0; j < _width; j++ ) {
				delete _genericFightCells[i][j];
				_genericFightCells[i][j] = 0;
			}
			delete [] _genericFightCells[i];
		}
		delete [] _genericFightCells;
	}
	_genericFightCells = 0;
}

void GenericFightMap::newFightMap( int h, int w, bool horizontalDraw )
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

	_horizontalDraw = horizontalDraw;
}

void GenericFightMap::reinit()
{
	TRACE("GenericFightMap::reinit");

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			if(_genericFightCells[i][j]->getUnit())	{
				_genericFightCells[i][j]->getUnit()->setCell(0);
				_genericFightCells[i][j]->setUnit( NULL );
				_genericFightCells[i][j]->setHeadUnit( NULL );
			}
		}
	}
}

void GenericFightMap::clearPath()
{
	TRACE("GenericFightMap::clearPath");
	
	GenericFightCell * cell = NULL;
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			cell = _genericFightCells[i][j];
			if (cell) {
				_genericFightCells[i][j]->setAccess( AttalCommon::UNKNOWN_ACCESS );
				_genericFightCells[i][j]->setType( AttalCommon::NORMAL );
				_genericFightCells[i][j]->setDist( 0 );
				_genericFightCells[i][j]->setHeadUnit( NULL );
			} else {
				logEE("error : cell null i ,%d j, %d",i,j);
			}
		}
	}
}

void GenericFightMap::initPath( GenericFightUnit * unit )
{
	TRACE("GenericFightMap::initPath");
	TRACE("GenericFightUnit race %d, level %d", unit->getRace(), unit->getLevel());
	//int checksum = 0;
	GenericFightCell * current = unit->getCell();
	GenericFightCell * cell = NULL;
	FightPile pile( this, unit->getMove(), unit );

	GenericFightMap::clearPath();

	current->setAccess( AttalCommon::NONE );
	current->setDist( 0 );

	pile.appendNeighbours( current );
	while( ! pile.isEmpty() ) {
		current = pile.takeSmallest();
		pile.appendNeighbours( current );
	}

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			cell = _genericFightCells[i][j];
			if (cell) {
				if( _genericFightCells[i][j]->getAccess() == AttalCommon::UNKNOWN_ACCESS ) {
					if ( _genericFightCells[i][j]->getUnit() ) {
						_genericFightCells[i][j]->setAccess( AttalCommon::FAR_OCCUPIED );
					} else {
						_genericFightCells[i][j]->setAccess( AttalCommon::FAR_FREE );
					}
					//checksum += (int ) _genericFightCells[i][j]->getAccess();
				} else {
				}
			} else {
				logEE("error : cell null i ,%d j, %d",i,j);
			}
		}
	}	
	//logDD(" checksum %d", checksum);
}

void GenericFightMap::printPath()
{
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			logDD("%d-%d = %d (%d)", i, j, _genericFightCells[i][j]->getDist(), _genericFightCells[i][j]->getAccess() );
		}
	}		
}

QStack<GenericFightCell *> GenericFightMap::computePath( GenericFightUnit *, GenericFightCell * cell )
{
	QStack<GenericFightCell *> path;
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
	
	current = getNeighbour1( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}
	current = getNeighbour2( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}
	current = getNeighbour3( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}
	current = getNeighbour4( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}
	current = getNeighbour5( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}
	current = getNeighbour6( cell );
	if( current && ( current->getDist() == dist )
		&& ( current->getAccess () != AttalCommon::NONE )
		&& ( current->getAccess () != AttalCommon::NEAR_OCCUPIED ) ) {
		ret = current;
	}

	if( ! ret ) {
		logEE( "Should not happen : no cell found ??" );
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour1( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( row > 0 ) {
			ret = at( row - 1, col );
		}
	}
	else
	{
		if ( !isEven ( row ) )
		{
			if (( row > 0 ) && ( col < _width - 1 ))
			{
				ret = at ( row - 1, col + 1 );
			}
		}
		else if ( row > 0 )
		{
			ret = at ( row - 1, col );
		}
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour2( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( isEven( col ) ) {
			if( ( row > 0 ) && ( col < _width-1 ) ){
				ret = at( row - 1, col + 1 );
			}
		} else {
			if( col < _width-1 ) {
				ret = at( row, col + 1 );
			}
		}
	}
	else
	{
		if ( col < _width - 1)
		{
			ret = at ( row , col + 1 );
		}
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour3( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( isEven( col ) ) {
			if( col < _width - 1 ) {
				ret = at( row, col + 1 );
			}
		} else {
			if( ( col < _width - 1 ) && ( row < _height - 1 ) ) {
				ret = at( row + 1, col + 1 );
			}
		}
	}
	else
	{
		if ( !isEven ( row ) )
		{
			if (( row < _height - 1 ) && ( col < _width - 1 ))
			{
				ret = at ( row + 1, col + 1 );
			}
		}
		else if ( row < _height - 1 )
		{
			ret = at ( row + 1, col );
		}
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour4( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( row < _height - 1 ) {
			ret = at( row + 1, col );
		}
	}
	else
	{
		if ( isEven ( row ) )
		{
			if (( row < _height - 1 ) && ( col > 0 ))
			{
				ret = at ( row + 1, col - 1 );
			}
		}
		else if ( row < _height - 1 )
		{
			ret = at ( row + 1, col );
		}
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour5( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( isEven( col ) ) {
			if( col > 0 ) {
				ret = at( row, col - 1 );
			}
		} else {
			if( ( row < _height - 1 ) && ( col > 0 ) ) {
				ret = at( row + 1, col - 1 );
			}
		}
	}
	else
	{
		if ( col > 0 )
		{
			ret = at ( row , col - 1);
		}
	}

	return ret;
}

GenericFightCell * GenericFightMap::getNeighbour6( GenericFightCell * cell )
{
	int row = cell->getRow();
	int col = cell->getCol();
	GenericFightCell * ret = 0;

	if ( _horizontalDraw )
	{
		if( isEven( col ) ) {
			if( ( row > 0 ) && ( col > 0 ) ) {
				ret = at( row - 1, col - 1 );
			}
		} else {
			if( col > 0 ) {
				ret = at( row, col - 1 );
			}
		}
	}
	else
	{
		if ( isEven ( row ) )
		{
			if (( row > 0 ) && ( col > 0 ))
			{
				ret = at ( row - 1, col - 1 );
			}
		}
		else if ( row > 0 )
		{
			ret = at ( row - 1, col );
		}
	}

	return ret;
}

bool GenericFightMap::isUpperLevel( GenericFightCell * cell1, GenericFightCell * cell2 ) 
{
	bool ret = false;
	
	 if ( ( getNeighbour1( cell1 ) == cell2 ) ||
	    ( getNeighbour6( cell1 ) == cell2 ) 
 			){
		ret = true;
	}

	return ret;
}

bool GenericFightMap::isSameLevel( GenericFightCell * cell1, GenericFightCell * cell2 ) 
{
	bool ret = false;	

	 if ( ( getNeighbour2( cell1 ) == cell2 ) ||
	    ( getNeighbour5( cell1 ) == cell2 ) 
 			){
		ret = true;
	}

	return ret;
}

bool GenericFightMap::isLowerLevel( GenericFightCell * cell1, GenericFightCell * cell2 ) 
{
	bool ret = false;	

	 if ( ( getNeighbour3(cell1 ) == cell2 )  ||
	    ( getNeighbour4( cell1 ) == cell2 ) 
 			){
		ret = true;
	}

	return ret;
	
}

/* Neighbour map

 6  / \ 1*
 5  | | 2
 4* \ / 3*
	
*/

bool GenericFightMap::areNeighbours( GenericFightCell * cell1, GenericFightCell * cell2 )
{
	bool ret = false;

	if( !cell1 || !cell2 ) {
		return ret;
	}

	if( ( getNeighbour1( cell1 ) == cell2 ) ||
	    ( getNeighbour2( cell1 ) == cell2 ) ||
	    ( getNeighbour3( cell1 ) == cell2 ) ||
	    ( getNeighbour4( cell1 ) == cell2 ) ||
	    ( getNeighbour5( cell1 ) == cell2 ) ||
	    ( getNeighbour6( cell1 ) == cell2 ) ||
 			(	cell1 == cell2 )){
		ret = true;
	}

	return ret;
}

bool GenericFightMap::isPath( GenericFightUnit * unit , GenericFightCell * cell )
{
	QStack<GenericFightCell *> path;
	int dist = cell->getDist();
	GenericFightCell * current = cell;

	path.push( cell );
	for( int i = dist - 1; i > 0; i-- ) {
		current = giveNeighbourOnPath( current, i );
		path.push( current );
	}
	current = path.pop();

	if( current != cell || path.count() == 0 ) {
		if( ! areNeighbours( unit->getCell(), current ) ) {
			return false;
		}

	}
	return true;
}

bool GenericFightMap::inMap( int row, int col )
{
	if( row < _height && row >= 0 && col >= 0 && col < _width ) {
		return true;
	} else {
		//logEE("Point not in fight map row %d , col %d , height %d, width %d", row, col , _height, _width);
		return false;
	}
	
}

GenericFightCell * GenericFightMap::getHeadCell( GenericFightCell * cell , bool isLookingToRight )
{
	GenericFightCell* headCell = NULL;
	
	if( isLookingToRight ) {
		headCell = getNeighbour2( cell );
	} else	{
		headCell = getNeighbour5( cell );
	}

	return headCell;

}

/*********************************************/
FightPile::FightPile( GenericFightMap * map, int limit,	GenericFightUnit* unit )
{
	_map = map;
	_limit = limit;
	_unit = unit;
	_unitsize = _unit->getCreature()->getSize();
}

GenericFightCell * FightPile::takeSmallest()
{
	FightPile::iterator i;
	GenericFightCell * smallest = first();
	GenericFightCell * current;

	for (i = begin(); i != end(); ++i) {
		 current = *i;
		if( smallest->getDist() > current->getDist() ) {
			smallest = current;
		}
	}
	
	removeAll(smallest);
	return smallest;
}

void FightPile::appendNeighbours( GenericFightCell * cell )
{
	int dist = cell->getDist() + 1;

	handleNeighbour( _map->getNeighbour1( cell ), dist );
	handleNeighbour( _map->getNeighbour2( cell ), dist );
	handleNeighbour( _map->getNeighbour3( cell ), dist );
	handleNeighbour( _map->getNeighbour4( cell ), dist );
	handleNeighbour( _map->getNeighbour5( cell ), dist );
	handleNeighbour( _map->getNeighbour6( cell ), dist );
}

void FightPile::handleNeighbour( GenericFightCell * neighbour, int dist )
{

	if( neighbour ) {

		bool isObstacle = testObstacle( neighbour );

		GenericFightUnit * headUnit = testHeadFree( neighbour );

		bool isOccupiedByAnotherUnit = false;

		if ( neighbour->getUnit ()  && ( neighbour->getUnit ()->isAlive () ) )
		{
			isOccupiedByAnotherUnit = ( neighbour->getUnit () != _unit );
		}

		switch( neighbour->getAccess() ) {
		case AttalCommon::UNKNOWN_ACCESS:
			if( isObstacle && !neighbour->getUnit() ) {
				neighbour->setAccess( AttalCommon::NONE );
				break;
			}
			if( dist <= _limit ) {
				if( isOccupiedByAnotherUnit || headUnit ) {
					neighbour->setAccess( AttalCommon::NEAR_OCCUPIED );
					neighbour->setDist( dist );
					if( headUnit ) {
						neighbour->setHeadUnit( headUnit );
					}
				} else {
					neighbour->setAccess( AttalCommon::NEAR_FREE );
					neighbour->setDist( dist );
					append( neighbour );
				}
			} else if( isOccupiedByAnotherUnit || headUnit ) {
				neighbour->setAccess( AttalCommon::FAR_OCCUPIED );
				neighbour->setDist( dist );
				if( headUnit ) {
					neighbour->setHeadUnit( headUnit );
				}
			} else {
				neighbour->setAccess( AttalCommon::FAR_FREE );
				neighbour->setDist( dist );
			}
			break;
		case AttalCommon::NONE:
			break;
		case AttalCommon::NEAR_FREE:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
				append( neighbour );
			}
			break;
		case AttalCommon::NEAR_OCCUPIED:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			break;
		case AttalCommon::FAR_FREE:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			if( neighbour->getDist() <= _limit ) {
				neighbour->setAccess( AttalCommon::NEAR_FREE );
				append( neighbour );
			}
			break;
		case AttalCommon::FAR_OCCUPIED:
			if( dist < neighbour->getDist() ) {
				neighbour->setDist( dist );
			}
			if( neighbour->getDist() <= _limit ) {
				neighbour->setAccess( AttalCommon::NEAR_OCCUPIED );
			}
			break;
		default:
			logEE( "Should not happen" );
			break;
		}
	}
}

bool FightPile::testObstacle( GenericFightCell * cell )
{
	bool ret = false;
	if( cell->getType() == AttalCommon::OBSTACLE ) {
		ret = true;
	}
	
	if( _unitsize == 2 ) {
		GenericFightCell* headCell = _map->getHeadCell( cell, _unit->isLookingToRight() );
		if( headCell ) {
			if ( headCell->getType () == AttalCommon::OBSTACLE )	{
				ret = true;
			}
		} else {
			ret = true;
		}
	}

	return ret;
}

GenericFightUnit * FightPile::testHeadFree( GenericFightCell * cell )
{
	GenericFightUnit * ret = 0;

	// If unit is 2-cells we can have problems with its head

	if( _unitsize == 2 ) {
		GenericFightCell* headCell = _map->getHeadCell( cell, _unit->isLookingToRight() );
		if( headCell )	{
			if( ( headCell->getUnit () ) &&
					( headCell->getUnit () != _unit ) 
					&& ( headCell->getUnit ()->isAlive () ) ) {
				ret = headCell->getUnit();
			}
		}
	}

	return ret;
}
