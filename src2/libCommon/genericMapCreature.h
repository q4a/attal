/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapCreature.h
** Manages creatures on map
**
** Version : $Id: genericMapCreature.h,v 1.20 2007/12/24 22:22:05 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QVector>
#include <QTextStream>
// application specific include files
#include "libCommon/define.h"
#include "libCommon/creature.h"
#include "libCommon/log.h"
#include "libCommon/genericRessources.h"

class GenericCell;


/**              ------------------------------
 *                      GenericMapCreature
 **              ------------------------------ */

class GenericMapCreature
{
public:
	/** Creature's behavior */
	enum CreatureBehaviour
	{
		Obedient = 0,
	    	Friendly,
    		Neutral,
    		Aggressive,
		Hostile
	};
	
	/** Creature growth mode */
	enum GrowthMode
	{
		Stable,
		FixedPercentage,
		VariablePercentage
	};

	/** Constructor */
	GenericMapCreature();

	virtual ~GenericMapCreature(); 

	/** Save lord */
	void save( QTextStream *, int indent = 0 );

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

	void setBehaviour ( CreatureBehaviour behaviour ) {
		_behaviour = behaviour;
	}

	CreatureBehaviour getBehaviour () {
		return _behaviour;
	}

	QString getBehaviourString();

	static QString getBehaviourString( CreatureBehaviour behaviour );
	
	static QString getGrowthString( GrowthMode growth );

	bool isFleeing() {
		return _flee;
	}

	void setFleeing( bool state ) {
		_flee = state;
	}

	bool isEstimated() { return _estimated; }

	//unsigned int getMinEstimatedNumber();

	//unsigned int getMaxEstimatedNumber();

	/** \return Returns the category of size of this unit */
	unsigned int getCategoryNumber();

	unsigned int getCreatureNumber();

	void setCategoryNumber( unsigned int nb ) {
		_categoryNumber = nb;
	}

	/** Set the value of the 'numStack' stack of creatures */
	void setStack( uint numStack, uint value );

	/** \return Returns the number of creature in the stack 'num' */
	uint getStack( unsigned int num );

	/** \return Returns the cell of the map where this creature is */
	GenericCell * getCell() const {
		return _cell;
	}

	/** Sets the cell where this creature is */
	virtual void setCell( GenericCell * cell ) {
		_cell = cell;
	}

	uint computeForceIndicator( bool real );

	bool isLookingRight() { return _lookingRight; }

	virtual void setLookingRight( bool state ) { _lookingRight = state; }
	
	void setGrowthMode( GrowthMode growth ) { _growth = growth; }
	
	GrowthMode getGrowthMode() { return _growth; }
	
	void setGrowthParam( uint param, uint value );
	
	uint getGrowthParam( uint param );
	
	void grow();

	GenericResourceList * getResourceList() { return _ress; }

protected:
	QVector<unsigned int *> _stacks;
	Creature * _creature;
	GenericCell * _cell;
	unsigned int _categoryNumber;
	CreatureBehaviour _behaviour;
	GrowthMode _growth;
	uint _growthParam0, _growthParam1;
	bool _flee;
	bool _lookingRight;
	bool _estimated;
	GenericResourceList * _ress;
};

#endif // GENERICMAPCREATURE_H
