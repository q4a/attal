/****************************************************************
**
** Attal : Lords of Doom
**
** interface.h
** Manages the editor
**
** Version : $Id: interface.h,v 1.6 2001/03/05 17:51:29 audoux Exp $
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

 
#ifndef INTERFACE_H
#define INTERFACE_H
 
// include files for QT
#include <qmainwindow.h>
#include <qstring.h>
// application specific includes

class Screen;

/*              ------------------------------
 *                         Interface
 *              ------------------------------ */


/** comment for the class */
class Interface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	Interface();
		
private:
	enum MENU_FIL {
		FILE_NEW,
		FILE_OPEN,
		FILE_SAVE,
		FILE_QUIT
	};
	
	enum MENU_SCEN {
		SCEN_INFO
	};
	
	enum MENU_HELP {
		HELP_INFO
	};
	
	/** Define menus */
	void initMenuBar();
	
	/** Define statusBar */
	void initStatusBar();
		
public slots:
	/** Slot managing 'File' menu */
	void slot_menuFileActivated( int num );
	
	/** Slot managing 'Scenario' menu */
	void slot_menuScenActivated( int num );
	
	/** Slot managing 'Help' menu */
	void slot_menuHelpActivated( int num );
	
	/** Slot managing the status bar */
	void slot_status( QString text );
	
private:
	Screen * _screen;
};
 
#endif // INTERFACE_H   
