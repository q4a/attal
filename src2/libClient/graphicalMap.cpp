/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalMap.cpp
** this class draws a map of cells
**
** Version : $Id: graphicalMap.cpp,v 1.8 2008/04/10 20:11:58 lusum Exp $
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
#include <QColor>
#include <QString>
#include <QPixmap>
// application specific includes
#include "conf.h"
#include "libClient/imageTheme.h"

/** this class draws a map of cells */
GraphicalMap::GraphicalMap( QObject * parent , const char * /* name */ )
	: QGraphicsScene(  parent  )
{
	_shroudImage = QPixmap( *ImageTheme.getShroudPixmap() );

	//setAdvancePeriod( 10 ); 
}

GraphicalMap::~GraphicalMap()
{

}

