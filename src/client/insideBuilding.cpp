/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.cpp
** display building on the inside view
**
** Version : $Id: insideBuilding.cpp,v 1.7 2002/09/20 23:56:29 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/11/2001
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

#include "insideBuilding.h"
 
// generic include files
// include files for QT
// application specific include files
#include "client/imageTheme.h"
#include "conf.h"
#include "common/log.h"
#include "common/genericInsideBuilding.h"

extern ImageTheme ImageTheme;

const int InsideBuilding::RTTI = 52;

InsideBuilding::InsideBuilding( int type, GenericInsideBuilding * build, QCanvas * canvas )
	: QCanvasSprite( ImageTheme.insideBuildings[type], canvas )
{
	_building = build;
 	collisions( false );
 	setFrame( 2 * _building->getLevel() );
	setEnabled( true );
	show();
}

void InsideBuilding::move( int x, int y )
{
	QCanvasSprite::move( x, y );
	setZ( int( y / 10 ) );
}

void InsideBuilding::select()
{
	setFrame( (_building->getLevel() * 2) + 1 );
}

void InsideBuilding::deselect()
{
	setFrame( _building->getLevel() * 2 );
}
