/****************************************************************
**
** Attal : Lords of Doom
**
** lord.h
** 
**
** Version : $Id: lord.h,v 1.9 2008/06/26 23:31:15 lusum Exp $
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
// application specific includes
#include "libCommon/genericLord.h"
#include "libCommon/genericCell.h"
#include "libClient/graphicalLord.h"
#include "libClient/map.h"


/*              ------------------------------
 *                         Lord
 *              ------------------------------ */

/** comment for the class */
class Lord : public GraphicalLord, public GenericLord
{
 
public:
	/** Constructor */
	Lord( Map * map, int id );
	
	/** Destructor */
	~Lord( );
	
	/** Set position of a lord */
	void setCell( GenericCell * cell );

	virtual void highlight( bool state);
	
	/** Set the lord visible on the map (or not) */
	void setVisible( bool state );
	
	virtual void setOwner( GenericPlayer * player );

};
 
#endif // LORD_H   
