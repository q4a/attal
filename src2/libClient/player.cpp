/****************************************************************
**
** Attal : Lords of Doom
**
** player.cpp
** manage player
**
** Version : $Id: player.cpp,v 1.14 2009/09/05 17:51:21 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/01/2001
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

#include "player.h"
 
// generic include files
// include files for QT
#include <QPixmap>
// application specific include files
#include "libCommon/log.h"

#include "libClient/graphicalPath.h"
#include "libClient/imageTheme.h"
#include "libClient/lord.h"

/** add comments here */
Player::Player( GenericMap * theMap )
	: GenericPlayer( theMap )
{
}

/*!

*/

void Player::setSelectedLord( GenericLord * lord )
{

  TRACE("setSelectLord %p",lord );

	unSelectLord();
	
	selectLord( lord );
}

void Player::unSelectLord()
{
	Map * gmap  = dynamic_cast<Map*>(_map);
	if( !gmap ) {
		return;
	}
	
	if( _selectedLord ){
		GraphicalPath * gpath = gmap->getGraphicalPath();
		gpath->clearPath();	
		((Lord *)_selectedLord)->highlight( false );	
	}
	
	GenericPlayer::unSelectLord();

}

void Player::selectLord( GenericLord * lord )
{
	TRACE("selectLord %p",lord );
	Map * gmap  = dynamic_cast<Map*>(_map);
	if( !gmap ) {
		return;
	}

	if( lord ) {
		GraphicalPath * gpath = gmap->getGraphicalPath();
		gpath->computePath( lord->getCell(), lord->getDestination(), lord->getCharac( MOVE ), lord->getCharac( MAXMOVE ));	
		((Lord *)lord)->highlight( true );	
		GenericPlayer::setSelectedLord( lord );
	}
}
