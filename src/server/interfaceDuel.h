/****************************************************************
**
** Attal : Lords of Doom
**
** interfaceDuel.h
** interface for the server of The Duel
**
** Version : $Id: interfaceDuel.h,v 1.2 2001/03/05 16:35:03 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/12/2000
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

 
#ifndef INTERFACEDUEL_H
#define INTERFACEDUEL_H
 
// include files for QT
#include <qmainwindow.h>
#include <qstring.h>

// application specific includes
#include "server/attalServer.h"

class ServerWidget;
class QLineEdit;

/*              ------------------------------
 *                         Interface
 *              ------------------------------ */


/** comment for the class */
class InterfaceDuel : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	InterfaceDuel();
	
public slots:
	/** Slot managing 'File' menu */
	void slot_menuFileActivated( int num );
	
	/** Slot managing 'Game' menu */
	void slot_menuGameActivated( int num );
	
	/** Slot managing the status bar */
	void slot_status( QString text );
			
private:
	enum MENU_FIL {
		FIL_LOAD,
		FIL_SAVE,
		FIL_START,
		FIL_END,
		FIL_QUIT
	};
	
	enum MENU_GAM {
		GAM_FIG,
		GAM_END
	};
		
	
	/** Define menus */
	void initMenuBar();
	
	/** Define statusBar */
	void initStatusBar();

	AttalServer * _server;
	ServerWidget * _widget;
};
 
class ServerWidget : public QWidget
{
public:
	/** Constructor */
	ServerWidget( QWidget * parent = 0, const char * name = 0 );
	
	/** Return nb of player written in the lineEdit */
	int getNbPlayer();
	
private:
	QLineEdit * _editLine;
};


#endif // INTERFACEDUEL_H   
