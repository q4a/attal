/****************************************************************
**
** Attal : Lords of Doom
**
** cell.h
**
** Version : $Id: cell.h,v 1.9 2006/11/11 19:38:01 fdarling Exp $
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

 
#ifndef CELL_H
#define CELL_H
 
// generic include files
// include files for QT
// application specific includes
#include "libCommon/genericCell.h"
#include "libClient/graphicalCell.h"

/*              ------------------------------
 *                         Cell
 *              ------------------------------ */


/** comment for the class */
class Cell : public GraphicalCell , public GenericCell
{
public:
	/** Construtor */
	Cell( int row, int col, QGraphicsScene * canvas );
	
	/** Destructor */
	~Cell();
	
	/** Add a decoration to the cell */
	void setDecoration( uint group, uint item );

private:
	Decoration * _decorationCell;
};
 

#endif // CELL_H   
