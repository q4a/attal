/****************************************************************
**
** Attal : Lords of Doom
**
** screen.h
** main widget of the editor
**
** Version : $Id: screen.h,v 1.10 2002/04/28 19:38:20 audoux Exp $
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
#include <qlist.h>
// application specific include files
#include "scenarioEditor/graphicalGameData.h"

class QPopupMenu;
class Cell;
class ItemSelector;
 
/*              ------------------------------
 *                         Screen
 *              ------------------------------ */

enum POPUP_ITEM {
	M_DELETE,
	M_PROPERTIES
};

/** comment for the class */
class Screen : public QWidget, public GraphicalGameData
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
	
	/** Create new scenario */
	void newScenario();
	
	/** clear the current map */
	void clearMap();
	
	/** fill the current map with the tile t */
	void fillMap( int t );
	
public slots:
	/** Slot when mouse button pressed */
	void slot_mouseLeftPressed( Cell * c );
	void slot_mouseRightPressed( Cell * c );
	
	/** Slot when mouse button is released */
	void slot_mouseReleased();
	
	/** Handle popup menu */
	void slot_popup( int );
	
	/** Slot when mouse moved */
	void slot_mouseMoved( Cell * c );

	
private:
	void handleDelete();
	void handleProperties();
	
	bool _leftPressed;
	ItemSelector * _selector;
	Cell * _currentCell;
	
	QPopupMenu * _menu;
};
 
#endif // SCREEN_H   
