/****************************************************************
**
** Attal : Lords of Doom
**
** themeInterface.h
** Manages the theme editor
**
** Version : $Id: themeInterface.h,v 1.1 2001/06/16 15:10:53 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

 
#ifndef THEMEINTERFACE_H
#define THEMEINTERFACE_H
 
// include files for QT
#include <qmainwindow.h>
#include <qstring.h>
// application specific include

class SectionSelector;
class QPopupMenu;

/*              ------------------------------
 *                         ThemeInterface
 *              ------------------------------ */


/** comment for the class */
class ThemeInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ThemeInterface();
		
private:
	enum MENU_FIL {
		FILE_NEW,
		FILE_OPEN,
		FILE_SAVE,
		FILE_QUIT
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
	
	/** Slot managing 'Help' menu */
	void slot_menuHelpActivated( int num );
	
	/** Slot managing the status bar */
	void slot_status( QString text );
	
private:
	SectionSelector * _selector;
	QPopupMenu * _menuFile;
};
 
#endif // THEMEINTERFACE_H   
