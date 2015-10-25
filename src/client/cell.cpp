/****************************************************************
**
** Attal : Lords of Doom
**
** cell.cpp
**
** Version : $Id: cell.cpp,v 1.10 2002/01/01 19:11:48 audoux Exp $
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

#include "cell.h"
 
// generic include files
#include <stdio.h>
// include files for QT
// application specific includes
#include "common/log.h"

/** Constructor : some initializations */
Cell::Cell( int row , int col , QCanvas * canvas )
	: GraphicalCell( row, col ,canvas ), 
	  GenericCell( row, col )
{
	_transitionCell = 0;
	_decorationCell = 0;
	_canvas = canvas;
}

Cell::~Cell()
{
	if( _transitionCell ) {
		delete _transitionCell;
	}
	
	if( _decorationCell ) {
		delete _decorationCell;
	}
}

void Cell::setType( const int type )
{
	setFrame( type );
	GenericCell::setType( type );
}

void Cell::setTransitionCellType( int type )
{
	GenericCell::setTransitionCellType( type );
	if( _transitionCell ) {
		delete _transitionCell;
		_transitionCell = 0;
	}
	if( _transition ) {
		_transitionCell = new Transition( this, _canvas );
		_transitionCell->setTransition( _transition, type );
	}
}

void Cell::setDecoration( int decoration )
{
	GenericCell::setDecoration( decoration );
	if( _decorationCell ) {
		delete _decorationCell;
		_decorationCell = 0;
	}
	if( _decoration ) {
		_decorationCell = new Decoration( this, _canvas );
		_decorationCell->setDecoration( _decoration );
	}
}

/** Print Cell info */
void Cell::displayCell() const
{
	printf("Cell [%d][%d]\n", getRow(), getCol() );
      
}

