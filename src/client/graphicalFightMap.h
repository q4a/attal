/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightMap.h
** this class draws a fightMap of cells
**
** Version : $Id: graphicalFightMap.h,v 1.3 2001/03/05 16:43:48 audoux Exp $
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
#include <qcanvas.h>

// application specific includes
#include "common/log.h" 
 
/*              ------------------------------
 *                         GraphicalFightMap
 *              ------------------------------ */



/** this class draws a fightMap of cells */
class GraphicalFightMap : public QCanvas
{ 
public:
	/** Constructor */
	GraphicalFightMap( QObject * parent = 0, const char * name = 0 ) 
		: QCanvas( parent , name) {};
	
	/** Destructor */
	~GraphicalFightMap() {
		logDD("Destruction de GraphicalFightMap");
	}
};
 
#endif // GRAPHICALFIGHTMAP_H   
