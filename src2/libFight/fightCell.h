/****************************************************************
**
** Attal : Lords of Doom
**
** fightCell.h
**
** Version : $Id: fightCell.h,v 1.4 2006/09/25 19:44:22 lusum Exp $
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
#include <QGraphicsScene>

// application specific includes
#include "libCommon/genericFightCell.h"
#include "libFight/graphicalFightCell.h"
 
/*              ------------------------------
 *                         FightCell
 *              ------------------------------ */



/** comment for the class */
class FightCell : public GraphicalFightCell , public GenericFightCell
{
public:
	/** Constructor */
	FightCell( int row, int col, QGraphicsScene * canvas, bool horizontalDraw = false )
		: GraphicalFightCell( row, col, canvas, horizontalDraw ),
		  GenericFightCell( row, col ) {};
	
	/** Display cell info in the log */
	void displayCell();
	
	enum { 
		Type = QGraphicsItem::UserType + 44
	};

	static const int RTTI;
	
	int type() const;

};

 
#endif // FIGHTCELL_H   
