/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalGameData.h
** Manage data (graphically) of a whole game
**
** Version : $Id: graphicalGameData.h,v 1.7 2002/10/23 22:19:50 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/01/2002
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

 
#ifndef GRAPHICALGAMEDATA_H
#define GRAPHICALGAMEDATA_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "server/gameData.h"
 
/*              ------------------------------
 *                         GraphicalGameData
 *              ------------------------------ */

class GenericBase;
class GenericBuilding;
class GenericArtefact;

/** comment for the class */
class GraphicalGameData : public GameData
{
 
public:
	/** Constructor */
	GraphicalGameData();

	/** Reinits graphical game data */
	void reinit();

	/** loads map */
	virtual void loadMap( QTextStream * stream, int width, int height );

	/** Adds a player to the data */
	virtual void addPlayer();

	/** Creates a new base */
	virtual GenericBase * getNewBase();

	/** Creates a new artefact */
	virtual GenericArtefact * getNewArtefact();

	/** Creates a new building */
	virtual GenericBuilding * getNewBuilding();
};
 
#endif // GRAPHICALGAMEDATA_H   
