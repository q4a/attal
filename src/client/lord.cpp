/****************************************************************
**
** Attal : Lords of Doom
**
** lord.cpp
** 
**
** Version : $Id: lord.cpp,v 1.14 2002/12/15 12:41:05 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 25/08/2000
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

#include "lord.h"

 
// generic include files
// include files for QT
// application specific include files
#include "common/attalSocket.h"
#include "common/pathFinder.h"

/** add comments here */
Lord::Lord( Map * map )
	:GraphicalLord( (QCanvas *)map ),
	 GenericLord()
{
	_map = map;
	_dest = 0;
	_showPath = new GraphicalPath( map );
}

void Lord::moveTo( GenericCell * cell )
{
	GenericLord::moveTo( cell );
	GraphicalLord::goTo( cell );
}

void Lord::setPosition( GenericCell * cell )
{
	GenericLord::setPosition( cell );
	GraphicalLord::setPosition( (Cell *)cell );
}

void Lord::showPath( bool state )
{
	if( state ) {
		computePath( _dest );
	} else {
		if(_showPath){
		_showPath->clear();
		}
	}
}

void Lord::computePath( GenericCell * cell )
{
	_dest = cell;
	_showPath->clear();
	if( cell && (cell != _currentCell) ) {
		_map->getPath()->reinit( _map );
		_map->getPath()->computePath( _currentCell );
		_showPath->initPath( cell );	
	}
}

void Lord::followPath( AttalSocket * socket )
{
	int cost;
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
	}
}

void Lord::setVisible( bool state )
{
	GenericLord::setVisible( state );
	GraphicalLord::setVisible( state );
}

void Lord::cleanPath()
{
	_showPath->clear();
}
