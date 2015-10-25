/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalMap.h
** this class draws a map of cells
**
** Version : $Id: graphicalMap.h,v 1.4 2006/10/16 18:35:14 fdarling Exp $
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
#include <QGraphicsScene>
#include <QPixmap>
// application specific includes
 
 
/*              ------------------------------
 *                         GraphicalMap
 *              ------------------------------ */



/** this class draws a map of cells */
class GraphicalMap : public QGraphicsScene
{ 
public:
	/** Constructor */
	GraphicalMap( QObject * parent = 0, const char * name = 0 );

	virtual ~GraphicalMap();
protected:
	QPixmap _shroudImage;
};
 
#endif // GRAPHICALMAP_H   
