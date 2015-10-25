/****************************************************************
**
** Attal : Lords of Doom
**
** mapCreature.cpp
** Manages creatures on map
**
** Version : $Id: mapCreature.cpp,v 1.2 2002/09/14 08:57:54 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/09/2002
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

#include "mapCreature.h"
 
// generic include files
// include files for QT
// application specific include files
#include "conf.h"

#include "client/imageTheme.h"
#include "client/cell.h"

extern ImageTheme ImageTheme;

MapCreature::MapCreature( QCanvas * map )
	:QCanvasSprite( ImageTheme.mapCreatures[ 0 ], map )
{
	collisions( true );
	setFrame( 0 );
	setZ( CAN_CREATURE );
	show();
}

void MapCreature::setCell( GenericCell * cell )
{
	GenericMapCreature::setCell( cell );
	QRect rect = ( (Cell*)cell )->boundingRect();
	move( rect.x() , rect.y() + rect.height() - boundingRect().height() );
	show();
        canvas()->update();
}

void MapCreature::setCreature( int race, int level )
{
	GenericMapCreature::setCreature( race, level );
	setSequence( ImageTheme.mapCreatures[ race ] );
	setFrame( level );
}


