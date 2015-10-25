/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalGameData.h
** Manage data (graphically) of a whole game
**
** Version : $Id: graphicalGameData.h,v 1.14 2008/06/21 17:29:00 lusum Exp $
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
#include <QTextStream>
// application specific include files
#include "libCommon/gameData.h"
 
/*              ------------------------------
 *                         GraphicalGameData
 *              ------------------------------ */

class GenericBase;
class GenericBuilding;
class GenericEvent;
class AttalSprite;

/** comment for the class */
class GraphicalGameData : public GameData
{
 
public:
	/** Constructor */
	GraphicalGameData();

	~GraphicalGameData();

	/** Reinits graphical game data */
	virtual void reinit();
	
	void initLords();
	
	/** Adds a player to the data */
	virtual void addPlayer();

	/** loads map */
	virtual void loadMap( QTextStream * stream, int width, int height );

	/** Creates a new base */
	virtual GenericBase * getNewBase( uchar race = 0 );
	
	virtual GenericBase * getNewBase(	uchar race, int row,	int col, uchar id, int nb, const QList<uchar> & forbidlist);

	/** Creates a new event */
	virtual GenericEvent * getNewArtefact( int id = -1 );

	virtual GenericEvent * getNewBonus();

	virtual GenericEvent * getNewChest();

	/** Creates a new building */
	virtual GenericBuilding * getNewBuilding( uchar type );

	virtual GenericBuilding * getNewBuilding( uchar type, int id, int row, int col );
	
	virtual void removeBuilding( GenericBuilding * building );

	/** Creates a new map creature */
	virtual GenericMapCreature * getNewMapCreature();
	
	virtual GenericMapCreature * getNewMapCreature( int row, int col, uchar race, uchar level, int nb, bool looking);
	
	 void adjustPlayers();

	 void appendAnimation( AttalSprite * item);

	 void removeAnimation( AttalSprite * item);

	 void advanceAnimations();

protected:
	QList<AttalSprite *> _animations;

};
 
#endif // GRAPHICALGAMEDATA_H   
