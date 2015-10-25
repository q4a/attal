/****************************************************************
**
** Attal : Lords of Doom
**
** chest.h
** Displays chests on map
**
** Version : $Id: chest.h,v 1.3 2006/10/02 17:49:31 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/08/2003
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

 
#ifndef CHEST_H
#define CHEST_H

// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericChest.h"
#include "libClient/attalSprite.h"

class GenericCell;


/*              ------------------------------
 *                         Chest
 *              ------------------------------ */

class Chest : public AttalSprite, public GenericChest
{

public:
	/** Constructor */
	Chest( QGraphicsScene * canvas );
	
	enum { 
		Type = QGraphicsItem::UserType + 1009
	};

	void setCell( GenericCell * cell );

	void setupChest();

	static const int RTTI;

	int type() const;
};

#endif // CHEST_H
