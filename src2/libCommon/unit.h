/****************************************************************
**
** Attal : Lords of Doom
**
** unit.h
** manages unit (army)
**
** Version : $Id: unit.h,v 1.22 2009/11/04 22:26:58 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QString>
// application specific includes

class Creature;
class GenericFightCell;
class GenericFightMap;

/*              ------------------------------
 *                         Unit
 *              ------------------------------ */

class GenericFightUnit
{

public:
	/** Constructor */
	GenericFightUnit();
	
	GenericFightUnit( Creature * creature );

	GenericFightUnit( uint race, uint level );

	GenericFightUnit( QString name );

	/** Destructor */	
	virtual ~GenericFightUnit();

	/** Set the fight map for unit */
	void setFightMap ( GenericFightMap* map );

	GenericFightMap* getFightMap () { return _map; }

	/** Set race of unit */
	void setCreature( QString name );

	void setCreature( uint race, uint level );

	void setCreature( Creature * creature );

	Creature * getCreature() { return _creature; }

	/** Return race of unit */
	uint getRace() { return _race; }

	/** Return race of unit */
	uint getLevel() { return _level; }

	/** Set nb of creatures */
	void setNumber( long int nb ) { _number = nb; }

	/** return nb of creatures */
	long int getNumber() { return _number; }

	/** Add nb of creatures */
	void addNumber( long int nb ) { _number += nb; }

	/** Set move points */
	void setMove( long int nb ) { _move = nb; }

	/** Return move points */
	long int getMove() { return _move; }

	void setMaxMove();

	/** Set health */
	void setHealth( long int nb ) { _health = nb; }

	/** Return health */
	long int getHealth() { return _health; }
	
	/** Set experience */
	void setExperience( long int nb ) { _experience = nb; }

	/** Return health */
	long int getExperience() { return _experience; }

	/** Display unit */
	void display();

	QString getUnitDescription();

	/** Calculate the damages made by the attack */
	int hit( long int nb );
	
	bool isAlive () { return (_number > 0); }

	/** Go to fight cell */
	void goTo( GenericFightCell * cell );

	/** Get the second cell of a 2 size unit */
	GenericFightCell * getNeighbour( GenericFightCell * cell );
	
	/** Return cell of the unit */
	GenericFightCell * getCell() {
		return _currentFightCell;
	}
	
	/** Return second cell of the unit (if exist)*/
	GenericFightCell * getNeibCell() {
		return _currentNeibCell;
	}
	
	/** buggy, don't use */
	void setCell(GenericFightCell * cell) {
		_currentFightCell = cell;
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

	/** Return maintenance cost of creature */
	int getMantCost(int ress);

	/** Is this unit looking to right (important for 2-cells units)*/
	bool isLookingToRight ();
	
	void setLookingToRight ( bool mirror );
	
	bool canAttack () {return _hasAttack;}
	
	void finishAttack () { _hasAttack = false;}
	
	void activateAttack () { _hasAttack = true;}


protected:
	long int _number, _move, _health, _experience;
	uint _race, _level;
	GenericFightCell * _currentFightCell, *  _currentNeibCell;
	Creature * _creature;
	GenericFightMap* _map;
	bool _lookingToRight;
	bool _hasAttack;

private:
	void checkAndSetDefaultCharac( Creature * creature );

};
 
#endif // UNIT_H   
