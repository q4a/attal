/****************************************************************
**
** Attal : Lords of Doom
**
** fightUnit.h
** Manage Units
**
** Version : $Id: fightUnit.h,v 1.7 2002/03/06 22:17:43 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/12/2000
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


#ifndef FIGHTUNIT_H
#define FIGHTUNIT_H


// generic include files
// include files for QT
// application specific include files
#include "common/unit.h"
#include "client/graphicalFightUnit.h"

class FightCell;

/*              ------------------------------
 *                         FightUnit
 *              ------------------------------ */


/** comment for the class */
class FightUnit : public GraphicalFightUnit, public GenericFightUnit
{

public:
	/** Constructor */
	FightUnit( QCanvas * canvas );

	FightUnit( QCanvas * canvas, GenericFightUnit * unit, bool mirror );

	/** Hit unit by 'nb' hitpoints */
	int hit( int nb );

	/** unit go to cell */
	void goTo( FightCell * cell );
};

#endif // FIGHTUNIT_H
