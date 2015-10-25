/****************************************************************
**
** Attal : Lords of Doom
**
** player.h
** manage player
**
** Version : $Id: player.h,v 1.10 2002/08/10 21:57:49 audoux Exp $
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

 
#ifndef PLAYER_H
#define PLAYER_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "common/genericPlayer.h"
#include "common/genericLord.h"
#include "common/genericMap.h"
 
class Game;
class QPixmap;
 
/*              ------------------------------
 *                         Player
 *              ------------------------------ */



/** comment for the class */
class Player : public GenericPlayer
{
 
public:
	/** Constructor */
	Player( Game * game, GenericMap * theMap = 0 );
	
	/** Select a lord */
	void setSelectedLord( GenericLord * lord );
	
	/** Select a base */
	void setSelectedBase( GenericBase * base );

	/** Unselect the lord (if any) */
	void unSelectLord();
	
	/** Return pixmap of the num-th lord */
	QPixmap * getLordPixmapByNum( int num );
	
	/** Return pixmap of the lord 'id' */
	QPixmap * getLordPixmapById( int id );
	
	/** Return the pixmap of the selected lord */
	QPixmap * getSelectedLordPixmap();
	
	/** Return small pixmap of the num-th lord */
	QPixmap * getLordSmallPixmapByNum( int num );

	/** Return small pixmap of the lord 'id' */
	QPixmap * getLordSmallPixmapById( int id );

	/** Return small pixmap of the num-th base */
	QPixmap * getBaseSmallPixmap( int num );
	
private:
	Game * _game;
};
 
#endif // PLAYER_H   
