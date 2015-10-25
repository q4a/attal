/****************************************************************
**
** Attal : Lords of Doom
**
** building.h
** manages buildings...
**
** Version : $Id: building.h,v 1.17 2002/10/20 16:15:49 audoux Exp $
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

 
#ifndef BUILDING_H
#define BUILDING_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "common/genericBuilding.h"
#include "common/genericBase.h"
#include "client/graphicalBuilding.h"
#include "client/cell.h"
 
/*              ------------------------------
 *                         Building
 *              ------------------------------ */

class Map;

/** Manage base in client side (on the map) */
class Base : public GraphicalBase, public GenericBase
{
public:
	/** Construtor */
	Base( Map * map );

	/** Initialises the building position */
	void setPosition( GenericCell * cell );

	/** Initialises the building position */
	void setPosition( int row, int col );

	/** The lord enters in the base */
	void enter( GenericLord * lord );

	/** Sets the race of the base */
	void setRace( int race );

protected:
	Map * _map;
};

/** Manage building in client side (on the map) */
class Building : public GraphicalBuilding, public GenericBuilding
{
public:
	/** Constructor */
	Building( QCanvas * canvas );

	/** initialises the building position */
	void setPosition( GenericCell * cell );

	bool load( QTextStream * ) { return true; }
	void enter( GenericLord * ) {}
	void out( GenericLord * ) {}

	/** Sets type of building */
	void setType( int type );
};

#endif // BUILDING_H
