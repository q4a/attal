/****************************************************************
**
** Attal : FightUnits of Doom
**
** graphicalFightUnit.h
** draw a hero on the map
**
** Version : $Id: graphicalFightUnit.h,v 1.10 2002/09/01 17:04:30 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2000
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

#ifndef GRAPHICALFIGHTUNIT_H
#define GRAPHICALFIGHTUNIT_H
 
// generic include files
// include files for QT
#include <qcanvas.h>
// application specific includes
#include "client/graphicalFightCell.h"

/** build the graphical part of the FightUnit */
class GraphicalFightUnit : public QCanvasSprite
{
public:
	/** Constructor */
	GraphicalFightUnit( QCanvas * canvas );

	/** Destructor */
	virtual ~GraphicalFightUnit();

	/** Set the FightUnit on the cell */
	void setPosition( GraphicalFightCell * cell );

	/** Move the lord at the given cell */
	void goTo( GraphicalFightCell * cell );

	/** Advance animation */
	void advance( int stage );

	/** Highlight (or not) the unit if active (or not) */
	void setActive( bool state = true ) {}

	/** Draw destroyed unit (or not) if destroyed (or not) */
	void setDestroyed( bool state );

	/** Return RTTI */
	int rtti() const { return RTTI; };

	static const int RTTI;

protected:
	QCanvasPixmap* imageAdvanced() const;

//private:
//	QCanvasRectangle * _numBox;

};

#endif // GRAPHICALFIGHTUNIT_H
