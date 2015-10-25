/****************************************************************
**
** Attal : Lords of Doom
**
** mapCreature.cpp
** Manages creatures on map
**
** Version : $Id: mapCreature.cpp,v 1.7 2006/11/03 18:28:57 fdarling Exp $
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

#include "libCommon/genericCell.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"

MapCreature::MapCreature( QGraphicsScene * map )
	:AttalSprite( (*(ImageTheme.mapCreatures[ 0 ][ 0 ]))[0], map )
{
	setFrame( 0 );
	setZValue( CAN_CREATURE );
}

void MapCreature::setCell( GenericCell * cell )
{
	GenericMapCreature::setCell( cell );
	setPos( cell->getCol() * DataTheme.tiles.getWidth() , ( cell->getRow() + 1 ) * DataTheme.tiles.getHeight() - boundingRect().height() );
}

void MapCreature::setCreature( int race, int level )
{
	GenericMapCreature::setCreature( race, level );
	setSequence( ImageTheme.mapCreatures[ race ][ level ]->getList(_lookingRight?1:0) );
	setFrame( 0 );
}

void MapCreature::setLookingRight( bool state )
{
	GenericMapCreature::setLookingRight( state );
	setSequence( ImageTheme.mapCreatures[ _creature->getRace() ][ _creature->getLevel() ]->getList(_lookingRight?1:0) );
	setFrame( 0 );
}


