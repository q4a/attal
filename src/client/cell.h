/****************************************************************
**
** Attal : Lords of Doom
**
** cell.h
**
** Version : $Id: cell.h,v 1.13 2002/10/20 08:54:26 audoux Exp $
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
#include <qcanvas.h>
// application specific includes
#include "common/genericCell.h"
#include "client/graphicalCell.h"

/*              ------------------------------
 *                         Cell
 *              ------------------------------ */


/** comment for the class */
class Cell : public GraphicalCell , public GenericCell
{
public:
	/** Construtor */
	Cell( int row, int col, QCanvas * canvas );
	
	/** Destructor */
	~Cell();
	
	/** Display the cell (row and col) (debugging) */
	void displayCell() const;
	
	/** Set type of cell */
	void setType( const int type );

	/** Set type of transition */
	void setTransitionCellType( int type );

	/** Add a decoration to the cell */
	void setDecoration( int decoration );

	/** \return Returns itself. Reimplemented for internal purpose */
	virtual Cell * getParent() { return this; }
	
private:
	Transition * _transitionCell;
	Decoration * _decorationCell;
	QCanvas * _canvas;
};
 

#endif // CELL_H   
