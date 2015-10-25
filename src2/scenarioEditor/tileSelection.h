/****************************************************************
**
** Attal : Lords of Doom
**
** tileSelection.h
** Manages selection of tiles
**
** Version : $Id: tileSelection.h,v 1.7 2008/09/01 22:55:32 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/01/2004
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

 
#ifndef TILESELECTION_H
#define TILESELECTION_H
 
 
// generic include files
// include files for QT
#include <QButtonGroup>
#include <QList>
#include <QWidget>
// application specific include files
#include "scenarioEditor/itemSelection.h"
#include "scenarioEditor/tilePresentation.h"

class QComboBox;
class QPushButton;



/*              ------------------------------
 *                       TileSelection
 *              ------------------------------ */

/** Selection of tiles */
class TileSelection : public ItemSelection
{
	Q_OBJECT
public:
	/** Constructor */
	TileSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );

protected:
	void tileSquare( uint row, uint col );
	QButtonGroup * _sizeG;
	QButtonGroup * _eff;
	QList<TilePresentation *> * _tiles;
};

#endif // TILESELECTION_H
