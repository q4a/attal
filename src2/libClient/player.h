/****************************************************************
**
** Attal : Lords of Doom
**
** player.h
** manage player
**
** Version : $Id: player.h,v 1.11 2009/09/05 17:51:21 lusum Exp $
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
#include <QPixmap>
// application specific include files
#include "libCommon/genericPlayer.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericMap.h"
 
class QPixmap;
 
/*              ------------------------------
 *                         Player
 *              ------------------------------ */



/** comment for the class */
class Player : public GenericPlayer
{
 
public:
	/** Constructor */
	Player( GenericMap * theMap = 0 );
	
	/** Select a lord */
	virtual void setSelectedLord( GenericLord * lord );
	
	/** Unselect the lord (if any) */
	virtual void unSelectLord();
	
private:
	void selectLord( GenericLord * lord );

};
 
#endif // PLAYER_H   
