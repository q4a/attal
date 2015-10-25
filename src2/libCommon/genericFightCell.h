/****************************************************************
**
** Attal : Lords of Doom
**
** genericFightCell.h
**  generic class for managing (not graphical) fightCells
**
** Version : $Id: genericFightCell.h,v 1.5 2008/06/08 22:17:34 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

 
#ifndef GENERICFIGHTCELL_H
#define GENERICFIGHTCELL_H
 
 
// generic include files
// include files for QT
// application specific includes
#include "libCommon/attalCommon.h"

class GenericFightUnit;

/*              ------------------------------
 *                         GenericFightCell
 *              ------------------------------ */


/** generic class for managing (not graphical) fightCells */
class GenericFightCell
{
 
public:
	/**  Constructor */
	GenericFightCell( int row, int col );

	/** Destructor */
	virtual ~GenericFightCell();

	/** Set type of the fight cell */
	void setType( int typ ) { 
		_type = typ;
		_coeff =  (typ == AttalCommon::OBSTACLE ? -1:1);
	}
	
	/** return type of the fight cell */
	inline int getType() { return _type; }	

	void setAccess( AttalCommon::FightCellAccess typ ) { _access = typ; }
	
	AttalCommon::FightCellAccess getAccess() { return _access; }
	
	int getDist() { return _dist; }
	
	void setDist( int dist ) { _dist = dist; }
	
	/** Return row */
	int getRow() { return _row; }
	
	/** Return col */
	int getCol()
		{ return _col; }
	
	/** Tell which unit is on it */
	void setUnit( GenericFightUnit * unit ) { _unit = unit; }
	
	/** Return the unit on the cell */
	GenericFightUnit * getUnit() { return _unit; }

	/** Tell which unit is on it */
	void setHeadUnit( GenericFightUnit * unit ) { _headUnit = unit; }
	
	/** Return the unit on the cell */
	GenericFightUnit * getHeadUnit() { return _headUnit; }
	
	
protected:
	int _coeff;
	int _type;
	int _dist;
	const int _row, _col;
	GenericFightUnit * _unit;
	GenericFightUnit * _headUnit;
	AttalCommon::FightCellAccess _access;
};
 
#endif // GENERICFIGHTCELL_H   
