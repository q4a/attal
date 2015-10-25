/****************************************************************
**
** Attal : Lords of Doom
**
** insideBuilding.h
** display building on the inside view
**
** Version : $Id: insideBuilding.h,v 1.6 2002/09/20 23:56:29 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/11/2001
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

 
#ifndef INSIDEBUILDING_H
#define INSIDEBUILDING_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>
// application specific include files


class GenericInsideBuilding;

/*              ------------------------------
 *                         InsideBuilding
 *              ------------------------------ */



/** comment for the class */
class InsideBuilding : public QCanvasSprite
{

public:
	/** Constructor */
	InsideBuilding( int type, GenericInsideBuilding * build, QCanvas * );

	void select();

	void deselect();

	virtual void move( int x, int y );

	GenericInsideBuilding * getBuilding() { return _building; }

	/** Return RTTI */
	int rtti() const { return RTTI; };

	static const int RTTI;

protected:
	GenericInsideBuilding * _building;
};
 
#endif // INSIDEBUILDING_H   
