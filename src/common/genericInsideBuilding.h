/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.h
** describes building inside bases
**
** Version : $Id: genericInsideBuilding.h,v 1.7 2002/12/17 12:53:25 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/01/2002
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

 
#ifndef GENERICINSIDEBUILDING_H
#define GENERICINSIDEBUILDING_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
// application specific include files
#include "common/define.h"

class InsideAction;
 
/**              ------------------------------
 *                    GenericInsideBuilding
 **              ------------------------------ */

class InsideBuildingModel
{
public:
	/** Constructor */
	InsideBuildingModel();

	int getRace() { return _race; }

	void setRace( int race ) { _race = race; }

	int getLevel() { return _level; }

	void setLevel( int level ) { _level = level; }

	QString getName() { return _name; }
	void setName( QString name ) { _name = name; }

	QString getDescription() { return _description; }
	void setDescription( QString description ) { _description = description; }

	int getX() { return _x; }
	void setX( int x ) { _x = x; }

	int getY() { return _y; }
	void setY( int y ) { _y = y; }

	int getReplacement() { return _replacement; }
	void setReplacement( int replace ) { _replacement = replace; }

	void addRequirement( int );
	int getRequirementNumber() { return _require.count(); }
	int getRequirement( int num );

	InsideAction * getAction() { return _action; }
	void setAction( InsideAction * action ) { _action = action; }

	int getCost( int ress );
	void setCost( int ress, int cost );
	
	int getMantCost( int ress );
	void setMantCost( int ress, int cost );

	
private:
	QString _name, _description;
	int _race, _level;
	int _x, _y;
	int _replacement;
	QList<int> _require;
	InsideAction * _action;
	int _cost[MAX_RESS];
	int _mcost[MAX_RESS];
};

/** comment for the class */
class GenericInsideBuilding
{
 
public:
	/** Constructor */
	GenericInsideBuilding();

	/** Destructor */
	virtual ~GenericInsideBuilding();

	int getRace() { return _race; }

	void setRace( int race ) { _race = race; }

	int getLevel() { return _level; }

	void setLevel( int level ) { _level = level; }
	
	int getX();

	int getY();
	
	int getMantCost( int ress );

protected:
	int _race, _level;

};

#endif // GENERICINSIDEBUILDING_H
