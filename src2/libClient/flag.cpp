/****************************************************************
**
** Attal : Lords of Doom
**
** flag.cpp
** manage display of flags on map
**
** Version : $Id: flag.cpp,v 1.5 2008/04/10 20:11:57 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/08/2004
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

#include "flag.h"
 
// generic include files
// include files for QT
#include <QString>
// application specific include files
#include "conf.h"

#include "libCommon/genericPlayer.h"

#include "libClient/imageTheme.h"

/** add comments here */
Flag::Flag( QGraphicsScene * canvas )
: AttalSprite( ImageTheme.getMapFlag( 0, 0 ), canvas ) 
{
	_type = 0;
	_teamId = 0;
	
 	setFrame( 0 );
	setZValue( CAN_LORD + 1 );
}

Flag::~Flag()
{
}

void Flag::setFlagType( uint type )
{
	_type = type;
	setSequence( ImageTheme.getMapFlag( _type, _teamId ) );
	setFrame( 0 );
}
	
void Flag::setOwner( GenericPlayer * player )
{
	if( player ) {
		_teamId = player->getTeamId();
		setSequence( ImageTheme.getMapFlag( _type, _teamId ) );
		setFrame( 0 );
	}
}


