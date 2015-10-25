/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.h
** draw a hero on the map
**
** Version : $Id: graphicalBuilding.h,v 1.9 2002/10/22 18:48:23 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2000
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

#ifndef GRAPHICALBUILDING_H
#define GRAPHICALBUILDING_H
 
// generic include files
// include files for QT
#include <qcanvas.h>

// application specific includes
#include "client/cell.h"

/** Build the graphical part of the Lord */
class GraphicalBuilding : public QCanvasSprite
{
public:
	/** Constructor */
	GraphicalBuilding( QCanvas * canvas );
	GraphicalBuilding( QCanvasPixmapArray * array, QCanvas * canvas );

	/** Set the Lord on the cell */
	virtual void setPosition( Cell *cell, int offsetRow=0, int offsetCol=0 );
	
	/** Advance animation */
	void advance( int stage );
	
	/** return RTTI */
	int rtti() const { return RTTI; };
	
	static const int RTTI;

	void setType( int type );

	void setAnimation( int nbFrame, int freq );

protected:
	QCanvasPixmap* imageAdvanced() const;
	int _frame, _nbFrame, _freq, _clock;

};

/** Graphical aspect of the base on the map */
class GraphicalBase : public GraphicalBuilding
{
public:
	/** Constructor */
	GraphicalBase( QCanvas * canvas );

	/** Sets the race of the base */
	void setRace( int );
};

#endif // GRAPHICALBUILDING_H
