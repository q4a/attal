/****************************************************************
**
** Attal : Lords of Doom
**
** screen.h
** main widget of the editor
**
** Version : $Id: screen.h,v 1.6 2001/03/05 17:51:29 audoux Exp $
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

 
#ifndef SCREEN_H
#define SCREEN_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
#include <qstring.h>
// application specific includes
#include "editor/scenarioInfo.h"

class Cell;
class Map;
class ChooseTiles;
 
/*              ------------------------------
 *                         Screen
 *              ------------------------------ */



/** comment for the class */
class Screen : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	Screen( QWidget * parent = 0, char * name = 0 );
	
	/** Display Info Scenario dialog */
	void displayInfo();
	
	/** Load scenario */
	bool load( const QString filename );
	
	/** Save scenario */
	void save( const QString filename );
	
	/** Create new map */
	void newMap();
	
public slots:
	/** Slot when mouse button pressed */
	void slot_mousePressed( Cell * c );
	
	/** Slot when mouse moved */
	void slot_mouseMoved( Cell * c );

	
private:
	ScenarioInfo * _info;
	Cell * _currentCell;
	ChooseTiles * choose;
	Map * _map;
};
 
#endif // SCREEN_H   
