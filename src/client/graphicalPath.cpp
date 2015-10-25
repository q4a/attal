/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalPath.cpp
** Visible path on the map
**
** Version : $Id: graphicalPath.cpp,v 1.11 2003/04/06 20:41:45 audoux Exp $
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
// application specific include files
#include "conf.h"
#include "common/pathFinder.h"
#include "client/imageTheme.h"

extern ImageTheme ImageTheme;

ArrowNumber arrow;

//
// ----- GraphicalPath -----
//

GraphicalPath::GraphicalPath( Map * map )
{
	_map = map;
	setAutoDelete( true );
}

void GraphicalPath::initPath( GenericCell * cell )
{
	if( _map->getPath()->isPath( cell ) ) {
		QStack<GenericCell> * path = _map->getPath()->giveCells( cell );
		GenericCell * start;
		if( ! path->isEmpty() ) {
			start = path->pop();

			while( ! path->isEmpty() ) {
				GenericCell * movCell = path->pop();
				GraphicalPathCell * grCell = new GraphicalPathCell( _map );
				grCell->setPosition( (Cell*)movCell );
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
		QStack<GenericCell> * path = _map->getPath()->giveNearCells( cell );
		GenericCell * start;
		if( ! path->isEmpty() ) {
			start = path->pop();

			while( ! path->isEmpty() ) {
				GenericCell * movCell = path->pop();
				GraphicalPathCell * grCell = new GraphicalPathCell( _map );
				grCell->setPosition( (Cell*)movCell );
				append( grCell );
			}
			delete path;
		} else {
			delete path;
		}
		GraphicalPathCell * grCell = new GraphicalPathCell( _map );
		grCell->setPosition( (Cell*)cell );
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

//
// ----- GraphicalPathCell -----
//

GraphicalPathCell::GraphicalPathCell( QCanvas * canvas )
	: QCanvasSprite( ImageTheme.pathCells, canvas )
{
	_row = 0;
	_col = 0;
   	collisions( false );
 	setFrame( 0 );
	setZ( CAN_ARROW );
	show();  
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

void GraphicalPathCell::setPosition( Cell * cell )
{
	QRect rect = cell->boundingRect();
	move( rect.x(), rect.y() + rect.height() - boundingRect().height() );
	
	canvas()->update();	
	
	_row = cell->getRow();
	_col = cell->getCol();
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


