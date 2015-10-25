/****************************************************************
**
** Attal : Lords of Doom
**
** unit.h
** manages unit (army)
**
** Version : $Id: unit.h,v 1.20 2002/12/17 12:53:25 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2000
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

 
#ifndef UNIT_H
#define UNIT_H
 
 
// generic include files
// include files for QT
#include <qlist.h>
#include <qstring.h>
// application specific includes

class Creature;
class GenericFightCell;

/*              ------------------------------
 *                         Unit
 *              ------------------------------ */

/** comment for the class */
class GenericFightUnit
{

public:
	/** Constructor */
	GenericFightUnit();
	
	/** Set race of unit */
	void setCreature( QString name );
	
	void setCreature( int race, int level );

	void setCreature( Creature * creature );
	
	Creature * getCreature() { return _creature; }
	
	/** Return race of unit */
	int getRace() { return _race; }

	/** Return race of unit */
	int getLevel() { return _level; }
	
	/** Set nb of creatures */
	void setNumber( int nb ) { _number = nb; }	
	
	/** return nb of creatures */
	int getNumber() { return _number; }

	/** Add nb of creatures */
	void addNumber( int nb ) { _number += nb; }
	
	/** Set move points */
	void setMove( int nb ) { _move = nb; }
	
	/** Return move points */
	int getMove() { return _move; }

	void setMaxMove();
	
	/** Set health */
	void setHealth( int nb ) { _health = nb; }
	
	/** Return health */
	int getHealth() { return _health; }

	/** Display unit */
	void display();	
	
	/** Calculate the damages made by the attack */
	int hit( long int nb );
	
	/** Go to fight cell */
	void goTo( GenericFightCell * cell );
	
	/** Return cell of the unit */
	GenericFightCell * getCell() {
		return _currentFightCell;
	}
	
	
	
	
	/** Return attack points of creature */	
	int getAttack();
	
	/** Return defense points of creature */
	int getDefense();

	/** Return dist attack points of creature */
	int getDistAttack();
	
	/** Tell if dist attack is allowed for creature  */
	bool isDistAttack();
	
	/** Return max health points of creature */
	int getMaxHealth();
	
	/** Return max move points of creature */
	int getMaxMove();
	
	/** Return morale of creature */
	int getMorale();
	
	/** Return luck of creature */
	int getLuck();

	/** Return min damages points of creature */
	int getMinDamages();
		
	/** Return max damages points of creature */
	int getMaxDamages();

	/** Return mantainment cost of creature */
	int getMantCost(int ress);
	
	
protected:
	int _number, _move, _health;
        int _race, _level;
	GenericFightCell * _currentFightCell;
	Creature * _creature;

};
 
#endif // UNIT_H   
