/****************************************************************
**
** Attal : Lords of Doom
**
** chooseTiles.h
** Widget for choosing tiles (for the edition)
**
** Version : $Id: chooseTiles.h,v 1.4 2001/03/05 17:51:29 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/08/2000
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

 
#ifndef CHOOSETILES_H
#define CHOOSETILES_H
 
 
// generic include files
// include files for QT
#include <qtabwidget.h>
// application specific includes
 

class QButtonGroup;

/*              ------------------------------
 *                         ChooseTiles
 *              ------------------------------ */



/** comment for the class */
class ChooseTiles : public QTabWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ChooseTiles( QWidget * parent );
	
	/** Return tile choosen */
	int getTile() { return _tile; }
	
public slots:
	/** Slot when a tile is selected */
	void slot_tileSelected( int num );
	
signals:
	/** Signal when tile is selected */
	void tileSelected();
	
private:
	int _tile;
	QButtonGroup * _panelGround;
	QButtonGroup * _panelBuilding;
};

 
#endif // CHOOSETILES_H   
