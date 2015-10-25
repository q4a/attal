/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalMap.cpp
** this class draws a map of cells
**
** Version : $Id: graphicalMap.cpp,v 1.7 2002/03/04 21:09:43 audoux Exp $
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

#include "graphicalMap.h"
 
// generic include files

// include files for QT
#include <qcolor.h>
#include <qcanvas.h>
#include <qstring.h>
// application specific includes
#include "client/cell.h"
#include "conf.h"

extern QString IMAGE_PATH;

/** this class draws a map of cells */
GraphicalMap::GraphicalMap( QObject * parent , const char * name )
	: QCanvas( parent , name )
{
	setBackgroundPixmap( QPixmap( IMAGE_PATH + "tiles/unknown.png" ) );

	setAdvancePeriod( 10 ); 
}

GraphicalMap::~GraphicalMap()
{

}

