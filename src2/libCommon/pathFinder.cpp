/****************************************************************
**
** Attal : Lords of Doom
**
** pathFinder.cpp
** Compute pathes in the map  
**
** Version : $Id: pathFinder.cpp,v 1.11 2009/11/04 22:26:58 lusum Exp $
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

#include "pathFinder.h"

 
// generic include files
#include <algorithm>
#include <stdlib.h>
// include files for QT
// application specific includes

/**  */
PathFinder::PathFinder()
{
	_width = 0;
	_height = 0;
}

PathFinder::PathFinder( int w, int h, GenericMap * m )
{
	newMap( h, w, m );
}

/** free the table */
PathFinder::~PathFinder()
{
	clear();
}

GenericCell * PathFinder::getStartCell()
{
	GenericCell * ret = 0;

	if( _start ) {
		ret = _start->cell;
	}

	return ret;
}

void PathFinder::newMap( int h, int w, GenericMap * m )
{
	_width = w;
	_height = h;
	_start = 0;
	int i;
	
	_table = new PathCell *[h];		
	for( i = 0; i < h; i++ ) {
		_table[i] = new PathCell[w];
 	}
		
	for( i = 0; i < h; i++ ) {
		for( int j = 0; j < w; j++ ) {
			_table[i][j].cell = m->at( i, j );
			_table[i][j].prev = 0;
			_table[i][j].dist = 0;
		}
	}	
}

void PathFinder::reinit( GenericMap * )
{
	/// XXX: remove map or used in overload ?
	_start = 0;
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			_table[i][j].prev = 0;
			_table[i][j].dist = 0;
		}
	}	
}

void PathFinder::clear()
{ 
	// XXX: delete [][] 
	for( int i = 0; i < _height; i++ ) {
		delete [] _table[i] ;
	}	
	
	if( _height > 0 ) 
		delete [] _table;	
	
	_height = 0;
	_width = 0;
}

void PathFinder::compute( Pile * p, GenericCell * c, PathCell * work, int cost )
{
	//if( c && ( c->getCoeff() >= 0 ) && ( c->isFree() ) ) {
	//if( c && ( c->getCoeff() >= 0 ) && c->isStoppable() ) {
	if( c && ( c->getCoeff() > 0 ) && c->isStoppable() && c->isFree() ) {
		if( (_table[c->getRow()][c->getCol()].dist == 0) || ( ( std::max(work->dist,0) + c->getCoeff() + cost ) < _table[c->getRow()][c->getCol()].dist ) ) {
			_table[c->getRow()][c->getCol()].dist = ( std::max(work->dist,0) + c->getCoeff() + cost );
			_table[c->getRow()][c->getCol()].prev = work;
			if( p->indexOf( &_table[c->getRow()][c->getCol()] ) == -1 ) {
				p->append( &_table[c->getRow()][c->getCol()] );
			}
		} 
	}
}

void PathFinder::computePath( GenericCell * start )
{
	if( ! start ) {
		logEE( "computePath on a NULL genericCell..." );
		return;
	}

	_start = &_table[start->getRow()][start->getCol()];
	_start->dist = 0;//-1;
	
	Pile pile;
	PathCell * work;	
	GenericCell * neighbour;
	int row, col;
	
	pile.append( _start );
	while( pile.count() > 0 ) {
		work = pile.takeSmallest();
		row = work->cell->getRow();
		col = work->cell->getCol();
			
		if( col > 0 ) {
			neighbour = _table[row][col-1].cell;
			compute( &pile, neighbour, work, COST_DIRECT );
			
			if( row > 0 ) {
				neighbour = _table[row-1][col-1].cell;
				compute( &pile, neighbour, work, COST_DIAG );	
			}
			
			if( row < _height-1 ) {
				neighbour = _table[row+1][col-1].cell;
				compute( &pile, neighbour, work, COST_DIAG );		
			}
			
		}
		
		if( col < _width - 1 ) {
			neighbour = _table[row][col+1].cell;
			compute( &pile, neighbour, work, COST_DIRECT );	
			
			if( row > 0 ) {
				neighbour = _table[row-1][col+1].cell;
				compute( &pile, neighbour, work, COST_DIAG );		
			}
			
			if( row < _height-1 ) {
				neighbour = _table[row+1][col+1].cell;
				compute( &pile, neighbour, work, COST_DIAG );	
			}
			
		}
		
		if( row > 0 ) {
			neighbour = _table[row-1][col].cell;
			compute( &pile, neighbour, work, COST_DIRECT );	
		}
		
		if( row < _height - 1 ) {
			neighbour = _table[row+1][col].cell;
			compute( &pile, neighbour, work, COST_DIRECT );	
		}
		
	}		
}

bool PathFinder::isPath( GenericCell * dest )
{
	if( _start && ( _start->cell == dest ) ) {
		return true;
	}
	if( _table[dest->getRow()][dest->getCol()].dist <= 0 ) {
		return false;
	}
	return true;
}

int PathFinder::getDist( GenericCell * dest )
{
	return  _table[dest->getRow()][dest->getCol()].dist;
}

bool PathFinder::isNearPath( GenericCell * dest )
{
	bool ret = false;
	
	if( dest->getCoeff() > 0 ) {
		int row = dest->getRow();
		int col = dest->getCol();
		if( row > 0 ) {
			ret = ret || isPath( _table[row-1][col].cell );
			if( col > 0 )
				ret = ret || isPath( _table[row-1][col-1].cell );	
			if( col < _width - 1 )
				ret = ret || isPath( _table[row-1][col+1].cell );
		}
		if( col > 0 )
			ret = ret || isPath( _table[row][col-1].cell );	
		if( col < _width - 1 )
			ret = ret || isPath( _table[row][col+1].cell );
	
		if( row < _height - 1 ) {
			ret = ret || isPath( _table[row+1][col].cell );
			if( col > 0 )
				ret = ret || isPath( _table[row+1][col-1].cell );	
			if( col < _width - 1 )
				ret = ret || isPath( _table[row+1][col+1].cell );
		}
		
	}
	
	return ret;
}


QStack<GenericCell *> * PathFinder::giveCells( GenericCell * dest )
{
	if( !isPath( dest ) )
		return 0;

	QStack<GenericCell *> * ret = new QStack<GenericCell *>;

	PathCell * cur;
	int i=0;
	cur = & _table[dest->getRow()][dest->getCol()];

	ret->push( cur->cell );

	while( cur != _start ) {
		i++;
		cur = cur->prev;
		ret->push( cur->cell );
	}

	return ret;
}

QStack<GenericCell *> * PathFinder::giveNearCells( GenericCell * dest )
{
	if( !isNearPath( dest ) ) {
		return 0;	
	}
	
	return( giveCells( getNearCell( dest ) ) );
}

GenericCell * PathFinder::getNearCell( GenericCell * dest )
{
	PathCell temp= { NULL,  NULL, -1};
	
	int row = dest->getRow();
	int col = dest->getCol();
	
	if( row > 0 ) {
		if( isPath( _table[row-1][col].cell ) ) {
			if( temp.dist != -1 ) {
				if( _table[row-1][col].dist < temp.dist )
					temp = _table[row-1][col];
			} else {
				temp = _table[row-1][col];
			}
		}
		if( col > 0 ) {
			if( isPath( _table[row-1][col-1].cell ) ) {
				if( temp.dist != -1 ) {
					if( _table[row-1][col-1].dist < temp.dist )
						temp = _table[row-1][col-1];
				} else 
					temp = _table[row-1][col-1];
				
			}	
			
		}
			
		if( col < _width - 1 )
			if( isPath( _table[row-1][col+1].cell ) ) {
				if( temp.dist != -1 ) {
					if( _table[row-1][col+1].dist < temp.dist )
						temp = _table[row-1][col+1];
				} else 
					temp = _table[row-1][col+1];
				
			}	
	}
	if( col > 0 )
		if( isPath( _table[row][col-1].cell ) ) {
			if( temp.dist != -1 ) {
				if( _table[row][col-1].dist < temp.dist )
					temp = _table[row][col-1];
			} else 
				temp = _table[row][col-1];
		}
	
	if( col < _width - 1 )
		if( isPath( _table[row][col+1].cell ) ) {
			if( temp.dist != -1 ) {
				if( _table[row][col+1].dist < temp.dist )
					temp = _table[row][col+1];
			} else 
				temp = _table[row][col+1];
		}
	
	if( row < _height - 1 ) {
		if( isPath( _table[row+1][col].cell ) ) {
			if( temp.dist != -1 ) {
				if( _table[row+1][col].dist < temp.dist )
					temp = _table[row+1][col];
			} else 
				temp = _table[row+1][col];
		}
		if( col > 0 )
			if( isPath( _table[row+1][col-1].cell ) ) {
				if( temp.dist != -1 ) {
					if( _table[row+1][col-1].dist < temp.dist )
						temp = _table[row+1][col-1];
				} else 
					temp = _table[row+1][col-1];
			}
		if( col < _width - 1 )
			if( isPath( _table[row+1][col+1].cell ) ) {
				if( temp.dist != -1 ) {
					if( _table[row+1][col+1].dist < temp.dist )
						temp = _table[row+1][col+1];
				} else 
					temp = _table[row+1][col+1];
			}
	}

	return temp.cell;	
}

int PathFinder::computeCostMvt( GenericCell * start, GenericCell * end )
{
	int ret = 0;

	if( ( start->getCoeff() != -1 ) && ( end->getCoeff() != -1 ) ) {
		int r1 = start->getRow();
		int r2 = end->getRow();
		int c1 = start->getCol();
		int c2 = end->getCol();

		if( ( abs( c1 - c2 ) < 2 ) && ( abs( r1 - r2 ) < 2 ) ) {
			if( c1 != c2 ) {
				if( r1 != r2 ) {
					ret = COST_DIAG;
				} else {
					ret = COST_DIRECT;
				}
			} else {
				if( r1 != r2 ) {
					ret = COST_DIRECT;
				}
			}
			ret = ret * ( start->getCoeff() + end->getCoeff() );
			ret = ret / 2;
		} else {
			ret = -1;
		}
	}

	return ret;
}

/**  Constructs an empty Pile */
Pile::Pile()
{
}


/** take the smallest element of the list */

PathCell * Pile::takeSmallest()
{
	int nbItems = count();
	int mini = 0;
	PathCell * smallest = value( 0 );
	PathCell * cur;
	for( int i = 1; i < nbItems; i++ ) {
		cur = value( i );
		if( sup( smallest, cur ) ) {
			smallest = cur;
			mini = i;
		}
	}
	return takeAt( mini );

	/*
	int mini;
	PathCell * sm = first();
	mini = at();
	PathCell * cur = next();
	
	while( cur ) {
		if ( sup(sm, cur) ) {
			sm = cur;
			mini = at();
		}
		cur = next();
	}
	return take( mini );*/
}


/**  */
PathFightFinder::PathFightFinder( int w, int h, GenericFightMap * m )
{
	_width = w;
	_height = h;
	_start = 0;
	int i;
	
	_table = new PathFightCell * [h];
	for( i = 0; i < h; i++ ) {
		_table[i] = new PathFightCell[w];
	}
	for( i = 0; i < h; i++ ) {
		for( int j = 0; j < w; j++ ) {
			_table[i][j].cell = m->at( i, j );
			_table[i][j].prev = 0;
			_table[i][j].dist = 0;
		}
	}
}

/** free the _table */
PathFightFinder::~PathFightFinder()
{
	for( int i = 0; i < _height; i++ ) {
		delete [] _table[i] ;
	}
	delete [] _table;
	
}

void PathFightFinder::reinit( GenericFightMap * )
{
	/// XXX: unused genericFightMap or for overloading
	_start = 0;
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			_table[i][j].prev = 0;
			_table[i][j].dist = 0;
		}
	}	
}

void PathFightFinder::computePath( GenericFightCell * )
{
// 	_start = &_table[start->getRow()][start->getCol()];
// 	_start->dist = -1;
	
// 	FightPile pile;
// 	PathFightCell * work;	
// //	GenericFightCell * voisin;
	
// 	pile.append( _start );
// 	while( pile.count() > 0 ) {
// 		work = pile.takeSmallest();
				
// // 		voisin = work->cell->getN();
// // 		COMPUTE_CELL;
// // 		voisin = work->cell->getNE();
// // 		COMPUTE_CELL;
// // 		voisin = work->cell->getSE();
// // 		COMPUTE_CELL;
// // 		voisin = work->cell->getS();
// // 		COMPUTE_CELL;
// // 		voisin = work->cell->getSW();
// // 		COMPUTE_CELL;
// // 		voisin = work->cell->getNW();
// // 		COMPUTE_CELL;
// 	}
}

QStack<GenericFightCell *> * PathFightFinder::giveCells( GenericFightCell * dest )
{
	QStack<GenericFightCell *> * ret = new QStack<GenericFightCell *>;
	PathFightCell * cur;
	int i=0;
	
	cur = & _table[dest->getRow()][dest->getCol()];
	ret->push( cur->cell );
	do {
		i++;
		cur = cur->prev;
		ret->push( cur->cell );
	} while( cur != _start );
		
	return ret;
}


// /**  Constructs an empty FightPile */
// FightPile::FightPile()
// {
// 	setAutoDelete( false );
// }


// /** take the smallest element of the list */

// PathFightCell * FightPile::takeSmallest()
// {
// 	int mini;
// 	PathFightCell * sm = first();
	
// 	mini = at();
// 	PathFightCell * cur = next();
	
// 	while( cur ) {
// 		if ( sup(sm, cur) ) {
// 			sm = cur;
// 			mini = at();
// 		}
// 		cur = next();
// 	}
// 	return take( mini );
// }
