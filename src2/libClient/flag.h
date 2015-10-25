/****************************************************************
**
** Attal : Lords of Doom
**
** flag.h
** manage display of flags on map
**
** Version : $Id: flag.h,v 1.4 2006/10/02 17:49:31 lusum Exp $
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

 
#ifndef FLAG_H
#define FLAG_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "libClient/attalSprite.h"
 

class GenericPlayer;
 
/*              ------------------------------
 *                         Flag
 *              ------------------------------ */

class Flag : public AttalSprite
{
public:
	/** Constructor */
	Flag( QGraphicsScene * canvas );
	
	/** Destructor */
	virtual ~Flag();
	
	void setFlagType( uint type );
	
	void setOwner( GenericPlayer * player );
	
protected:
	uint _type, _teamId;
	
};
 
#endif // FLAG_H   
