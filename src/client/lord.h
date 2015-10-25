/****************************************************************
**
** Attal : Lords of Doom
**
** lord.h
** 
**
** Version : $Id: lord.h,v 1.12 2002/11/18 17:21:12 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 25/08/2000
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

 
#ifndef LORD_H
#define LORD_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>
// application specific includes
#include "common/genericLord.h"
#include "common/genericCell.h"
#include "client/graphicalLord.h"
#include "client/graphicalPath.h"
#include "client/map.h"

class AttalSocket;

/*              ------------------------------
 *                         Lord
 *              ------------------------------ */

/** comment for the class */
class Lord : public GraphicalLord, public GenericLord
{
 
public:
	/** Constructor */
	Lord( Map * map );
	
	/** Move the lord at the given cell */
	void moveTo( GenericCell * cell );

	/** Intializes the lord */
	void setPosition( GenericCell * cell );
	
	/** Tell to show (or clear) the path */
	void showPath( bool state = true );
	
	/** Set the destination of the lord */
	void setDestination( GenericCell * cell ) { _dest = cell; }
	
	/** Get the destination of the lord */
	GenericCell * getDestination() { return _dest; }
	
	/** Compute the path */
	void computePath( GenericCell * cell );

	void cleanPath();
	
	/** Follow the path computed */
	void followPath( AttalSocket * socket );

	/** Set the lord visible on the map (or not) */
	void setVisible( bool state );
	
private:
	GenericMap * _map;
	GraphicalPath * _showPath;
	GenericCell * _dest;
};
 
#endif // LORD_H   
