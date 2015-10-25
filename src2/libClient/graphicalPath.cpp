/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalPath.cpp
** Visible path on the map
**
** Version : $Id: graphicalPath.cpp,v 1.17 2008/03/30 19:54:04 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/01/2001
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

#include "graphicalPath.h"
 
// generic include files
// include files for QT
#include <QStack>
// application specific include files
#include "conf.h"

#include "libCommon/pathFinder.h"
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"

ArrowNumber arrow;

//
// ----- GraphicalPath -----
//

GraphicalPath::GraphicalPath( Map * map )
{
	_map = map;
	_numCell = NULL;
}

GraphicalPath::~GraphicalPath()
{
	clearPath();
}

void GraphicalPath::initPath( GenericCell * cell )
{
	if( _map->getPath()->isPath( cell ) ) {
		QStack<GenericCell *> * path = _map->getPath()->giveCells( cell );
		GenericCell * start;
		if( ! path->isEmpty() ) {
			start = path->pop();

			while( ! path->isEmpty() ) {
				GenericCell * movCell = path->pop();
				GraphicalPathCell * grCell = new GraphicalPathCell( _map );
				grCell->setPosition( movCell );
				append( grCell );
			}
			delete path;
			int prevRow = start->getRow();
			int prevCol = start->getCol();
			for( int i = 0; i < int( count() - 1 ); i++ ) {
				at( i )->computeFrame( prevRow, prevCol, at( i+1 )->getRow(), at( i+1 )->getCol() );
				prevRow = at( i )->getRow();
				prevCol = at( i )->getCol();
			}
		} else {
			delete path;
		}
	} else if( _map->getPath()->isNearPath( cell ) && (  cell->isStoppable() ) ) {
		QStack<GenericCell *> * path = _map->getPath()->giveNearCells( cell );
		GenericCell * start;
		if( path->isEmpty() ) {
			path->push( _map->getPath()->getStartCell() );
		}

		start = path->pop();

		while( ! path->isEmpty() ) {
			GenericCell * movCell = path->pop();
			GraphicalPathCell * grCell = new GraphicalPathCell( _map );
			grCell->setPosition( movCell );
			append( grCell );
		}
		delete path;

		GraphicalPathCell * grCell = new GraphicalPathCell( _map );
		grCell->setPosition( cell );
		append( grCell );
		int prevRow = start->getRow();
		int prevCol = start->getCol();
		for( int j = 0; j < int( count() - 1 ); j++ ) {
			at( j )->computeFrame( prevRow, prevCol, at( j+1 )->getRow(), at( j+1 )->getCol() );
			prevRow = at( j )->getRow();
			prevCol = at( j )->getCol();
		}
	}
}

void GraphicalPath::computePath( GenericCell * start, GenericCell * dest, int movePt, int maxMove )
{
	clearPath();
	
	if( !dest || ( dest == start ) ) {
		return;
	}
	
	PathFinder * path =	_map->getPath();
	_numCell = new NumPathCell( _map );
	path->reinit( _map );
	path->computePath( start );
	initPath( dest );	

	// manage far arrows
	int cumcost = 0;
	int oldmovePt = movePt;
	int cost;
	int row = start->getRow();
	int col = start->getCol();
	for( int i = 0; i < count(); i++ ) {	
		GraphicalPathCell * pathCell = at( i );
		if( pathCell ) {
			cost = PathFinder::computeCostMvt( _map->at( row, col ), _map->at( pathCell->getRow(), pathCell->getCol() ) );
			cumcost += cost;
			if( cost > movePt ) {
				pathCell->setFar( true );
			}
			movePt -= cost;
			row = pathCell->getRow();
			col = pathCell->getCol();
			_numCell->setPosition( _map->at( row, col ) );
		}
	}
	if( maxMove > 0 ) {
		cumcost -= oldmovePt;
		int turns = 0;
		if( cumcost <= 0 ) {
			turns = 1;
		} else {
			turns += (cumcost / maxMove) + 2; 
		}
		_numCell->setPlainText( QString("%1").arg( turns ) );
		_numCell->show();
	}
}

QList<GenericCell*> GraphicalPath::followPath( GenericCell * start, int movePt )
{
	/*int cost;
	int movePt = getCharac( MOVE );
	/// XXX: compute movement points...
	int row = _currentCell->getRow();
	int col = _currentCell->getCol();

	while( ! _showPath->isEmpty() ) {
		GraphicalPathCell * cell = _showPath->getFirst();
		cost = computeCostMvt( _map->at( row, col ), _map->at( cell->getRow(), cell->getCol() ) );
		if( ( cost != -1 ) && ( cost <= movePt ) ) {
			socket->sendMvt( getId(), cell->getRow(), cell->getCol() );
			row = cell->getRow();
			col = cell->getCol();
			movePt = movePt - cost;
		}
		_showPath->removeFirst();
	}*/
	int cost;
	/// XXX: compute movement points...
	int row = start->getRow();
	int col = start->getCol();
	
	TRACE("GraphicalPath::followPath movePt %d, row %d, col %d", movePt, row, col);
	QList<GenericCell *> list;

	while( ! isEmpty() ) {
		GraphicalPathCell * cell = first();
		cost = PathFinder::computeCostMvt( _map->at( row, col ), _map->at( cell->getRow(), cell->getCol() ) );
		if( ( cost != -1 ) && ( cost <= movePt ) ) {
			row = cell->getRow();
			col = cell->getCol();
			list.append( _map->at( row, col ) );
			movePt = movePt - cost;
		}
		removeFirstCell();
	}

	clearNum();

	return list;
}

void GraphicalPath::clearNum ()
{
	if( _numCell ) {
		delete _numCell;
		_numCell = NULL;
	}
}

void GraphicalPath::clearPath() 
{
	TRACE("GraphicalPath::clearPath");

	while (!isEmpty()) {
		removeFirstCell();
	}
	
	clearNum();

	clear();
}

void GraphicalPath::removeFirstCell()
{
	GraphicalPathCell * grCell = 0;

	grCell =  takeFirst();
	delete grCell;
}

//
// ----- GraphicalPathCell -----
//

GraphicalPathCell::GraphicalPathCell( QGraphicsScene * canvas )
	: AttalSprite( ImageTheme.pathCells, canvas )
{
	_row = 0;
	_col = 0;
 	setFrame( 0 );
	setZValue( CAN_ARROW );
}

GraphicalPathCell::~GraphicalPathCell()
{
}

int computeNum( int row, int col ) 
{
	int ret;
	if( row == - 1 ) {
		if( col == -1 ) {
			ret = 1;
		} else if( col == 0 ) {
			ret = 2;
		} else {
			ret = 3;
		}
	} else if( row == 0 ) {
		if( col == -1 ) {
			ret = 4;
		} else {
			ret = 5;
		}
	} else {
		if( col == -1 ) {
			ret = 6;
		} else if( col == 0 ) {
			ret = 7;
		} else {
			ret = 8;
		}	
	}
	return ret;
}

void GraphicalPathCell::computeFrame( int prevRow, int prevCol, int nextRow, int nextCol )
{
	int prev = computeNum( prevRow - getRow(), prevCol - getCol() );
	int next = computeNum( nextRow - getRow(), nextCol - getCol() );

	setFrame( arrow.getFrame( prev, next ) );
}

void GraphicalPathCell::setFar( bool state )
{
	if( state ) {
		if( frame() < 33 ) {
			setFrame( frame() + 33 );
		}
	} else {
		if( frame() > 32 ) {
			setFrame( frame() - 33 );
		}
	}
}

void GraphicalPathCell::setPosition( GenericCell * cell )
{
	setPos( cell->getCol() * DataTheme.tiles.getWidth() , (cell->getRow() + 1) * DataTheme.tiles.getHeight() - boundingRect().height() );
	
	_row = cell->getRow();
	_col = cell->getCol();
	
	setZValue( CAN_ARROW );
	setVisible(true);
}

//
// ----- NumPathCell -----
//

NumPathCell::NumPathCell( QGraphicsScene * scene )
	: QGraphicsTextItem(0, scene )
{
	_row = 0;
	_col = 0;
	setZValue( CAN_NUM_ARROW );
	setDefaultTextColor ( QColor( 255,255,255) );
}

void NumPathCell::setPosition( GenericCell * cell )
{
	setPos( cell->getCol() * DataTheme.tiles.getWidth() , (cell->getRow() + 1) * DataTheme.tiles.getHeight() - boundingRect().height() );
	
	_row = cell->getRow();
	_col = cell->getCol();
	
	setZValue( CAN_NUM_ARROW );
	setVisible(true);
}

//
// ----- ArrowNumber -----
//

ArrowNumber::ArrowNumber() 
{
	for( int i = 0; i < 8; i++ ) {
		for( int j = 0; j < 8; j++ ) {
			_arrow[i][j] = 0;
		}
	}
	_arrow[0][2] = 27;
	_arrow[0][4] = 9;
	_arrow[0][5] = 30;
	_arrow[0][6] = 11;
	_arrow[0][7] = 10;
	_arrow[1][5] = 14;
	_arrow[1][6] = 13;
	_arrow[1][7] = 12;
	_arrow[2][0] = 31;
	_arrow[2][3] = 17;
	_arrow[2][5] = 16;
	_arrow[2][6] = 15;
	_arrow[2][7] = 28;
	_arrow[3][2] = 6;
	_arrow[3][4] = 7;
	_arrow[3][7] = 8;
	_arrow[4][0] = 20;
	_arrow[4][3] = 19;
	_arrow[4][5] = 18;
	_arrow[5][0] = 26;
	_arrow[5][1] = 3;
	_arrow[5][2] = 4;
	_arrow[5][4] = 5;
	_arrow[5][7] = 29;
	_arrow[6][0] = 24;
	_arrow[6][1] = 1;
	_arrow[6][2] = 2;
	_arrow[7][0] = 22;
	_arrow[7][1] = 23;
	_arrow[7][2] = 32;
	_arrow[7][3] = 21;
	_arrow[7][5] = 25;
}

int ArrowNumber::getFrame( int prev, int next ) 
{
	return _arrow[prev-1][next-1];
}
