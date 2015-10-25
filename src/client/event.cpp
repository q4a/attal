/****************************************************************
**
** Attal : Lords of Doom
**
** event.cpp
** Displays event on map
**
** Version : $Id: event.cpp,v 1.1 2002/12/22 19:55:35 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/12/2002
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

#include "event.h"

// generic include files
// include files for QT
// application specific include files
#include "conf.h"
#include "client/imageTheme.h"
#include "client/cell.h"

extern ImageTheme ImageTheme;


Event::Event( QCanvas * map )
:QCanvasSprite( ImageTheme.events[ 0 ], map )
{
	collisions( true );
	setFrame( 0 );
	setZ( CAN_CREATURE );
	show();
}

void Event::setCell( GenericCell * cell )
{
	GenericEvent::setCell( cell );
	QRect rect = ( (Cell*)cell )->boundingRect();
	move( rect.x() , rect.y() + rect.height() - boundingRect().height() );
	show();
        canvas()->update();
}

void Event::setType( GenericEvent::EventType type )
{
	GenericEvent::setType( type );
	setSequence( ImageTheme.events[ (int)type ] );
	setFrame( 0 );
}




