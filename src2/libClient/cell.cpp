/****************************************************************
**
** Attal : Lords of Doom
**
** cell.cpp
**
** Version : $Id: cell.cpp,v 1.12 2006/11/11 19:38:01 fdarling Exp $
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
#include "libCommon/log.h"

/** Constructor : some initializations */
Cell::Cell( int row , int col , QGraphicsScene * canvas )
	: GraphicalCell( row, col ,canvas ), 
	  GenericCell( row, col )
{
	_decorationCell = 0;
}

Cell::~Cell()
{
	delete _decorationCell;
}

void Cell::setDecoration( uint group, uint item )
{
	GenericCell::setDecoration( group, item );

	delete _decorationCell;
	_decorationCell = NULL;

	if( group ) {
		_decorationCell = new Decoration( this, scene() );
		_decorationCell->setDecoration( group, item );
	}
}
