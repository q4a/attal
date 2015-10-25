/****************************************************************
**
** Attal : Lords of Doom
**
** bonus.h
** Displays bonus on map
**
** Version : $Id: bonus.h,v 1.5 2006/10/02 17:49:30 lusum Exp $
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


#ifndef BONUS_H
#define BONUS_H
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericBonus.h"
#include "libClient/attalSprite.h"

class GenericCell;

/*              ------------------------------
 *                         Bonus
 *              ------------------------------ */

class Bonus : public AttalSprite, public GenericBonus
{

public:
	/** Constructor */
	Bonus( QGraphicsScene * canvas );
	
	enum { 
		Type = QGraphicsItem::UserType + 1008
	};

	void setCell( GenericCell * cell );

	void setupBonus();

	static const int RTTI;

	int type() const;
};

#endif // BONUS_H
