/****************************************************************
**
** Attal : Lords of Doom
**
** genericFightCell.h
**  generic class for managing (not graphical) fightCells
**
** Version : $Id: genericFightCell.h,v 1.8 2003/01/01 17:59:39 lusum Exp $
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

class GenericFightUnit;

/*              ------------------------------
 *                         GenericFightCell
 *              ------------------------------ */

enum TypeFightCell {
	UNKNOWN_TYPE,
	NORMAL,
	OBSTACLE
};
	
enum TypeAccess {
	UNKNOWN_ACCESS,
	NONE,
	NEAR,
	NEAR_OCCUPIED,
	FAR,
	FAR_OCCUPIED
};

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
		_coeff =  (typ == OBSTACLE ? -1:1);
	}
	
	/** return type of the fight cell */
	int getType() { return _type; }	

	void setAccess( TypeAccess typ ) { _access = typ; }
	
	TypeAccess getAccess() { return _access; }
	
	int getDist() { return _dist; }
	
	void setDist( int dist ) { _dist = dist; }
	
	/** Return row */
	int getRow() { return _row; }
	
	/** Return col */
	int getCol()
		{ return _col; }
	
	void setRow( int row ) { _row=row; }
	
	void setCol( int col )  { _col=col; }
	
	/** Tell which unit is on it */
	void setUnit( GenericFightUnit * unit ) { _unit = unit; }
	
	/** Return the unit on the cell */
	GenericFightUnit * getUnit() { return _unit; }
	
protected:
	int _coeff;
	int _type;
	int _dist;
	int _row, _col;
	GenericFightUnit * _unit;
	TypeAccess _access;
}
;
 
#endif // GENERICFIGHTCELL_H   
