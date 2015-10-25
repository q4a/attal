/****************************************************************
**
** Attal : Lords of Doom
**
** genericBonus.h
** Class for managing bonus on map
**
** Version : $Id: genericBonus.h,v 1.7 2007/10/15 21:49:56 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2003
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


#ifndef GENERICBONUS_H
#define GENERICBONUS_H
 
 
// generic include files
// include files for QT
#include <QList>

// application specific include files


//class GenericCell;
class QTextStream;

// For BonusResource, first param is resource type,
// then 2nd param is numberType:
// 0 - Fixed -> next param is number of resource
// 1 - Various -> next param describes how many various
// 	values, then the couples (value-ponderation)
// 2 - Alea ->next params are, minValue (included),
//	maxValue(included) and step value
// For PrimSkill, first value is the primSkill
// (0-Attack, 1-Defense, 2-Power, 3-Knowledge, 4-Morale, 5-Luck)
// then the number 'added'

/*              ------------------------------
 *                         GenericBonus
 *              ------------------------------ */

class GenericBonus
{

public:
	enum BonusType {
		BonusResource = 0,
		BonusPrimSkill,
		BonusSkill,
		BonusSpell
	};

	/** Constructor */
	GenericBonus();

	/** Destructor */
	virtual ~GenericBonus();

	BonusType getType() { return _type; }

	void setType( BonusType type ) { _type = type; }

	void addParam( uint value );

	uint getParamNumber();

	uint getParam( uint num );

	void clearParams();

	/** Save building */
	void save( QTextStream * ts, int indent = 0 );

	QString getBonusDescription();

	virtual void setupBonus() {}

protected:
	BonusType _type;
	
	QList<uint> _params;
};

#endif // GENERICBONUS_H
