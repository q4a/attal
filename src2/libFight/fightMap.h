/****************************************************************
**
** Attal : Lords of Doom
**
** fightMap.h
** Manage the map of the fight
**
** Version : $Id: fightMap.h,v 1.4 2005/10/30 22:21:00 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 07/12/2000
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

 
#ifndef FIGHTMAP_H
#define FIGHTMAP_H
 
 
// generic include files
// include files for QT
// application specific includes
#include "libCommon/attalCommon.h"
#include "libCommon/genericFightMap.h"
#include "libFight/graphicalFightMap.h"
#include "libFight/fightCell.h"

/*              ------------------------------
 *                         FightMap
 *              ------------------------------ */



/** A Map that contains FightCells */
class FightMap : public GraphicalFightMap, public GenericFightMap
{
public:
	/** Constructor */
	FightMap( QObject * parent =0 , const char * name =0);

	/// XXX: Does we need a destructor for the FightCells ??
	/** Destructor */
	~FightMap();
	
	/** Clear the map */
	void clearFightMap();
	
	/** Change the cell type */
	void changeFightCell( int i, int j, AttalCommon::TypeFightCell typ );

 	/** Reinit a new fight map */
	void newFightMap( int h, int w, bool horizontalDraw = false );	
	
	/** Reinit a new unknown fight map */
	void newUnknownFightMap( int h, int w );

	/** Return the FightCell at (i,j) */
	FightCell * at( int i, int j ) {
		return (FightCell*)GenericFightMap::at( i, j );
	}

	/** Clears the path on the figth map */
	void clearPath();

	/** Compute the state of the cells (and the color) before unit mvt */
	void initPath( GenericFightUnit * unit );

	/** Show all cells*/
	void showCells ();

	/** Hide all cells*/
	void hideCells ();
};

#endif // FIGHTMAP_H
