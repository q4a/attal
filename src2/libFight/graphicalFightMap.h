/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightMap.h
** this class draws a fightMap of cells
**
** Version : $Id: graphicalFightMap.h,v 1.5 2006/10/02 22:16:09 lusum Exp $
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

 
#ifndef GRAPHICALFIGHTMAP_H
#define GRAPHICALFIGHTMAP_H
 
 
// generic include files


// include files for QT
#include <QGraphicsScene>

// application specific includes
#include "libCommon/log.h" 
 
/*              ------------------------------
 *                         GraphicalFightMap
 *              ------------------------------ */



/** this class draws a fightMap of cells */
class GraphicalFightMap : public QGraphicsScene
{ 
public:
	/** Constructor */
	GraphicalFightMap( QObject * parent = 0 ) 
		: QGraphicsScene( parent ) {};
	
};
 
#endif // GRAPHICALFIGHTMAP_H   
