/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalPath.h
** Visible path on the map
**
** Version : $Id: graphicalPath.h,v 1.7 2002/10/22 18:48:23 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/01/2001
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

 
#ifndef GRAPHICALPATH_H
#define GRAPHICALPATH_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>
#include <qlist.h>
#include <qstack.h>
// application specific include files
#include "client/map.h"
#include "client/cell.h"

class GraphicalPathCell;
 
/*              ------------------------------
 *                         GraphicalPath
 *              ------------------------------ */



/** comment for the class */
class GraphicalPath : public QList<GraphicalPathCell>
{
public:
	/** Constructor */
	GraphicalPath( Map * map );
			
	/** Init the pathes for the 'cell' */
	void initPath( GenericCell * cell );
	
private:
	Map * _map;
};

/** Sprite for displatying the path on the map */
class GraphicalPathCell : public QCanvasSprite
{
public:
	/** Constructor */
	GraphicalPathCell( QCanvas * canvas );
	
	/** Put it on the map */
	void setPosition( Cell * cell );
	
	/** Set as the end of path */
	void computeFrame( int prevRow, int prevCol, int nextRow, int nextCol );
	
	/** Get the row of the cell */
	int getRow() { return _row; }
	
	/** Get the col of the cell */
	int getCol() { return _col; }
	
private:
	int _row, _col;
};

/** Computes good arrow for the path */
class ArrowNumber
{
public:
	/** Constrcutor */
	ArrowNumber();

	/** \return Returns the frame number for the good arrow */
	int getFrame( int prev, int next );

private:
	int _arrow[8][8];
};

#endif // GRAPHICALPATH_H
