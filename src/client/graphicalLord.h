/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.h
** draw a hero on the map
**
** Version : $Id: graphicalLord.h,v 1.10 2002/03/04 21:15:14 audoux Exp $
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

#ifndef GRAPHICALLORD_H
#define GRAPHICALLORD_H
 
// generic include files
// include files for QT
#include <qcanvas.h>
#include <qcolor.h>
// application specific includes
#include "client/cell.h"

/*              ------------------------------
 *                        GraphicalLord
 *              ------------------------------ */

/** build the graphical part of the Lord */
class GraphicalLord : public QCanvasSprite
{
public:
	/** Constructor */
	GraphicalLord( QCanvas * canvas );

	virtual ~GraphicalLord();

	/** Set the Lord on the cell */
	virtual void setPosition( Cell *cell );
	
	/** Move the lord at the given cell */
	virtual void goTo( GenericCell *cell );
	
	/** Advance animation */
	void advance( int stage );
	
	/** Set color to the lord */
	void setColor( QColor color );
	
	/** Return RTTI */
	int rtti() const { return RTTI; };
	
	static const int RTTI;
	
protected:
	QCanvasPixmap* imageAdvanced() const;
	
};

#endif // GRAPHICALLORD_H

