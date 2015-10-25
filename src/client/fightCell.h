/****************************************************************
**
** Attal : Lords of Doom
**
** fightCell.h
**
** Version : $Id: fightCell.h,v 1.9 2001/06/16 14:25:38 audoux Exp $
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

 
#ifndef FIGHTCELL_H
#define FIGHTCELL_H
 
 
// generic include files
// include files for QT
#include <qcanvas.h>

// application specific includes
#include "common/genericFightCell.h"
#include "client/graphicalFightCell.h"
 
/*              ------------------------------
 *                         FightCell
 *              ------------------------------ */



/** comment for the class */
class FightCell : public GraphicalFightCell , public GenericFightCell
{
public:
	/** Constructor */
	FightCell( int row, int col, QCanvas * canvas )
		: GraphicalFightCell( row, col, canvas ),
		  GenericFightCell( row, col ) {};
	
	/** Display cell info in the log */
	void displayCell();
};

 
#endif // FIGHTCELL_H   
