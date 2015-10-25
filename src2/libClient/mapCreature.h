/****************************************************************
**
** Attal : Lords of Doom
**
** mapCreature.h
** Manages creatures on map
**
** Version : $Id: mapCreature.h,v 1.4 2006/10/02 17:54:08 lusum Exp $
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
// application specific include files
#include "libCommon/genericMapCreature.h"
#include "libClient/attalSprite.h"

/*              ------------------------------
 *                         MapCreature
 *              ------------------------------ */


/** comment for the class */
class MapCreature : public AttalSprite, public GenericMapCreature
{

public:
	/** Constructor */
	MapCreature( QGraphicsScene * map );

	virtual ~MapCreature() {}

	virtual void setCell( GenericCell * cell );

	virtual void setCreature( int race, int level );

	virtual void setLookingRight( bool state );
};

#endif // MAPCREATURE_H
