/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.h
** describes building inside bases
**
** Version : $Id: genericInsideBuilding.h,v 1.10 2007/10/24 22:40:38 lusum Exp $
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
#include <QCoreApplication>
#include <QList>
#include <QString>

// application specific include files
#include "libCommon/define.h"

class QTextStream;

class InsideAction;
 
/**              ------------------------------
 *                    InsideBuildingModel
 **              ------------------------------ */

class InsideBuildingModel
{
	
	Q_DECLARE_TR_FUNCTIONS(InsideBuildingModel)

public:
	/** Constructor */
	InsideBuildingModel();
	
	virtual ~InsideBuildingModel();

	void save( QTextStream * ts, int indent );

	uint getRace() { return _race; }

	void setRace( uint race ) { _race = race; }

	uint getLevel() { return _level; }

	void setLevel( uint level ) { _level = level; }

	QString getName() { return _name; }
	void setName( const QString & name ) { _name = name; }

	QString getDescription() { return _description; }

	void setDescription( const QString & description ) { _description = description; }

	QString getBuildDescActions();

	int getX() { return _x; }
	void setX( int x ) { _x = x; }

	int getY() { return _y; }
	void setY( int y ) { _y = y; }

	int getReplacement() { return _replacement; }
	void setReplacement( int replace ) { _replacement = replace; }

	void clearRequirements() { _require.clear(); }
	void addRequirement( int );
	int getRequirementNumber() { return _require.count(); }
	int getRequirement( int num );

	InsideAction * getAction() { return _action; }
	void setAction( InsideAction * action );

	uint getCost( uint ress );
	void setCost( uint ress, uint cost );

	uint getMantCost( uint ress );
	void setMantCost( uint ress, uint cost );


private:
	QString _name, _description;
	uint _race, _level;
	int _x, _y;
	int _replacement;
	QList<int> _require;
	InsideAction * _action;
	uint _cost[ MAX_RESS ];
	uint _mcost[ MAX_RESS ];
};

/**              ------------------------------
 *                    GenericInsideBuilding
 **              ------------------------------ */

class GenericInsideBuilding
{
 
public:
	/** Constructor */
	GenericInsideBuilding();

	/** Destructor */
	virtual ~GenericInsideBuilding();

	uint getRace() { return _race; }

	void setRace( uint race ) { _race = race; }

	uint getLevel() { return _level; }

	void setLevel( uint level ) { _level = level; }

	int getX();

	int getY();

	uint getMantCost( uint ress );

protected:
	uint _race, _level;

};

#endif // GENERICINSIDEBUILDING_H
