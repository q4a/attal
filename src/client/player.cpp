/****************************************************************
**
** Attal : Lords of Doom
**
** player.cpp
** manage player
**
** Version : $Id: player.cpp,v 1.10 2002/08/11 17:39:28 audoux Exp $
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
#include <qpixmap.h>
// application specific include files
#include "common/log.h"

#include "client/game.h"
#include "client/imageTheme.h"
#include "client/lord.h"

extern ImageTheme ImageTheme;

/** add comments here */
Player::Player( Game * game, GenericMap * theMap )
	: GenericPlayer( theMap )
{
	_game = game;
	
}

void Player::unSelectLord()
{
	if( _selectedLord )
		((Lord *)_selectedLord)->showPath( false );	
	
	GenericPlayer::unSelectLord();
}

/*!

*/

void Player::setSelectedLord( GenericLord * lord )
{
	GenericPlayer::setSelectedLord( lord );
	((Lord *)lord)->showPath();
	_game->setState( Game::MS_LORD );
}

void Player::setSelectedBase( GenericBase * base )
{
	GenericPlayer::setSelectedBase( base );
	_game->setState( Game::MS_BASE );
}

QPixmap * Player::getLordPixmapByNum( int num )
{
	return ImageTheme.getLordPixmap( getLord( num )->getId() );
}

QPixmap * Player::getLordPixmapById( int id )
{
	return ImageTheme.getLordPixmap( id );
}

QPixmap * Player::getSelectedLordPixmap()
{
	if( _selectedLord == 0 ) {
		logEE( "No selected Lord !");
		return 0;
	} else {
		return ImageTheme.getLordPixmap( _selectedLord->getId() );
	}
}

QPixmap * Player::getLordSmallPixmapByNum( int num )
{
	return ImageTheme.getLordSmallPixmap( getLord( num )->getId() );
}

QPixmap * Player::getLordSmallPixmapById( int id )
{
	return ImageTheme.getLordSmallPixmap( id );
}


QPixmap * Player::getBaseSmallPixmap( int num )
{
	return ImageTheme.getBaseSmallPixmap( num );
}

