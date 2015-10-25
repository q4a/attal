/****************************************************************
**
** Attal : Lords of Doom
**
** lord.cpp
** 
**
** Version : $Id: lord.cpp,v 1.20 2008/06/26 23:31:15 lusum Exp $
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
#include <QList>

// application specific include files


/** add comments here */
Lord::Lord( Map * map, int id )
	:GraphicalLord( (QGraphicsScene *)map ),
	 GenericLord( id )
{
}

Lord::~Lord()
{
}

void Lord::setCell( GenericCell * cell )
{
	GenericLord::setCell( cell );
	GraphicalLord::setCell( cell );
}

void Lord::highlight( bool state)
{
	GraphicalLord::highlight( state );
}

void Lord::setVisible( bool state )
{
	GenericLord::setVisible( state );
	GraphicalLord::setVisible( state );
}

void Lord::setOwner( GenericPlayer * player )
{
	GenericLord::setOwner( player );
	GraphicalLord::setOwner( player );
}
