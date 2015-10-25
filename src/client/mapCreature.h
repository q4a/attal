/****************************************************************
**
** Attal : Lords of Doom
**
** mapCreature.h
** Manages creatures on map
**
** Version : $Id: mapCreature.h,v 1.3 2002/09/14 14:17:38 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/09/2002
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


#ifndef MAPCREATURE_H
#define MAPCREATURE_H


// generic include files
// include files for QT
#include <qcanvas.h>
// application specific include files
#include "common/genericMapCreature.h"

/*              ------------------------------
 *                         MapCreature
 *              ------------------------------ */


/** comment for the class */
class MapCreature : public QCanvasSprite, public GenericMapCreature
{

public:
	/** Constructor */
	MapCreature( QCanvas * map );

	virtual ~MapCreature() {}

	virtual void setCell( GenericCell * cell );

	virtual void setCreature( int race, int level );
};

#endif // MAPCREATURE_H
