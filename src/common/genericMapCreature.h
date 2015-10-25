/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapCreature.h
** Manages creatures on map
**
** Version : $Id: genericMapCreature.h,v 1.5 2003/02/03 21:59:02 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/09/2002
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


#ifndef GENERICMAPCREATURE_H
#define GENERICMAPCREATURE_H


// generic include files
// include files for QT
#include <qlist.h>
// application specific include files
#include "common/creature.h"

class GenericCell;

/**              ------------------------------
 *                      GenericMapCreature
 **              ------------------------------ */

class GenericMapCreature
{
public:
	/** Constructor */
	GenericMapCreature();

	virtual ~GenericMapCreature() {}

	/** \return Return the creature associated to this creature on map */
	Creature * getCreature() {
		return _creature;
	}

	/** Sets the creature associated to this creature on map */
	void setCreature( int race, int level );

	/** \return Returns the race of the creature associated */
	int getRace();

	/** \return Returns the level of the creature associated */
	int getLevel();

	/** \return Returns the category of size of this unit */
	int getCategoryNumber();

	/** Sets a category of size for this unit */
	void setCategoryNumber( int categ ) {}

	/** \return Returns the number of stacks of this unit */
	int getStackNumber() {
		return _stacks.count();
	}

	/** Add a stack to this creature */
	void addStack( int number );

	//void addStack( Creature * creature, int number );

	/** \return Returns the number of creature in the stack 'num' */
	int getStack( int num );

	/** \return Returns the cell of the map where this creature is */
	GenericCell * getCell() {
		return _cell;
	}

	/** Sets the cell where this creature is */
	void setCell( GenericCell * cell ) {
		_cell = cell;
	}


protected:
	QList<int> _stacks;
	Creature * _creature;
	GenericCell * _cell;
};

#endif // GENERICMAPCREATURE_H
