/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInterface.h
** Manages the scenario editor
**
** Version : $Id: scenarioInterface.h,v 1.4 2002/10/31 22:59:15 audoux Exp $
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

 
#ifndef SCENARIOINTERFACE_H
#define SCENARIOINTERFACE_H
 
// include files for QT
#include <qmainwindow.h>
#include <qstring.h>
#include <qdialog.h>
// application specific includes

class Screen;
class QComboBox;

/*              ------------------------------
 *                         ScenarioInterface
 *              ------------------------------ */


/** comment for the class */
class ScenarioInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ScenarioInterface();
		
private:
	enum MENU_FIL {
		FILE_NEW,
		FILE_OPEN,
		FILE_SAVE,
		FILE_SAVEAS,
		FILE_QUIT
	};
	
	enum MENU_SCEN {
		SCEN_INFO
	};
	
	enum MENU_TOOLS {
		TOOL_CLEAR,
		TOOL_FILL
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

	/** Slot managing 'Tools' menu */
	void slot_menuToolsActivated( int num );
	
	/** Slot managing 'Help' menu */
	void slot_menuHelpActivated( int num );
	
	/** Slot managing the status bar */
	void slot_status( QString text );
	
private:
	void handleFill();
	
	Screen * _screen;
	QString _filename;
};

/** Dialog asking to choose a tile for filling the map with */
class FillDialog : public QDialog
{
public:
	/** Constructor */
	FillDialog( QWidget * parent = 0, const char * name = 0 );

	/** \return Returns the num of the tile choosen */
	int getTileNumber();
	
private:
	QComboBox * _combo;
};

#endif // SCENARIOINTERFACE_H   
