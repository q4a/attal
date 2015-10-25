/****************************************************************
**
** Attal : Lords of Doom
**
** tavernManager.h
** Manages tavern (available lords)
**
** Version : $Id: tavernManager.h,v 1.1 2006/07/13 14:10:32 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/02/2005
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

 
#ifndef TAVERNMANAGER_H
#define TAVERNMANAGER_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericLord.h"

class GameData;
class GenericBase;
 
 
/*              ------------------------------
 *                       TavernManager
 *              ------------------------------ */


class TavernManager
{
 
public:
	/** Constructor */
	TavernManager( GameData * data );
	
	void reinit();
	
	uint getTavernLordNumber( GenericBase * base );
	
	GenericLord * getTavernLord( GenericBase * base, uint numLord );
	
	void newDay();
	
private:
	GameData * _data;
};
 
#endif // TAVERNMANAGER_H   
