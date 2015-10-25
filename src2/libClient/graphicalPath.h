/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalPath.h
** Visible path on the map
**
** Version : $Id: graphicalPath.h,v 1.10 2008/03/23 18:57:13 lusum Exp $
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
// application specific include files
#include "libClient/attalSprite.h"
#include "libClient/map.h"
#include "libCommon/genericCell.h"

class GraphicalPathCell;
class NumPathCell;
 
/*              ------------------------------
 *                         GraphicalPath
 *              ------------------------------ */



/** comment for the class */
class GraphicalPath : public QList<GraphicalPathCell *>
{
public:
	/** Constructor */
	GraphicalPath( Map * map );
	
	~GraphicalPath();
			
	/** Init the pathes for the 'cell' */
	void initPath( GenericCell * cell );

	/** Compute the path between cell start and cell dest */
	void computePath( GenericCell * start, GenericCell * dest, int movePt, int maxMove=0  );

	/** Give the path between cell start and cell dest */
	QList<GenericCell*> followPath( GenericCell * start, int movePt );
	
	void clearPath();
	
	void clearNum();
	
	void removeFirstCell();
	
private:
	Map * _map;
	NumPathCell * _numCell;
};

/** Sprite for displatying the path on the map */
class GraphicalPathCell : public AttalSprite
{
public:
	/** Constructor */
	GraphicalPathCell( QGraphicsScene * canvas );

	~GraphicalPathCell();
	
	/** Put it on the map */
	void setPosition( GenericCell * cell );
	
	/** Set as the end of path */
	void computeFrame( int prevRow, int prevCol, int nextRow, int nextCol );
	
	/** Get the row of the cell */
	int getRow() { return _row; }
	
	/** Get the col of the cell */
	int getCol() { return _col; }
	
	void setFar( bool state );
	
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

class NumPathCell : public QGraphicsTextItem
{
public:
	/** Constructor */
	NumPathCell( QGraphicsScene * canvas );

	/** Put it on the map */
	void setPosition( GenericCell * cell );
	
	/** Get the row of the cell */
	int getRow() { return _row; }
	
	/** Get the col of the cell */
	int getCol() { return _col; }
	
	void setNumber( int num );
	
private:
	int _row, _col;
};

#endif // GRAPHICALPATH_H
