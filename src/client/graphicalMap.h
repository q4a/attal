/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalMap.h
** this class draws a map of cells
**
** Version : $Id: graphicalMap.h,v 1.3 2002/03/04 21:09:43 audoux Exp $
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

 
#ifndef GRAPHICALMAP_H
#define GRAPHICALMAP_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>
// application specific includes
 
 
/*              ------------------------------
 *                         GraphicalMap
 *              ------------------------------ */



/** this class draws a map of cells */
class GraphicalMap : public QCanvas
{ 
public:
	/** Constructor */
	GraphicalMap( QObject * parent = 0, const char * name = 0 );

	virtual ~GraphicalMap();
};
 
#endif // GRAPHICALMAP_H   
