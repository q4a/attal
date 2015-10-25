/****************************************************************
**
** Attal : Lords of Doom
**
** interfaceDuel.h
** Manages the game of The Duel
**
** Version : $Id: interfaceDuel.h,v 1.2 2005/10/30 22:20:56 audoux Exp $
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
#include <QMainwindow>
#include <QApplication>
#include <QString>

// application specific includes
#include "common/attalSocket.h"
#include "client/player.h"
#include "client/gui.h"

class Fight;

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
	/** Slot for managing 'File' menu */
	void slot_menuFileActivated( int num );
	
	/** Slot for managing 'Game' menu */
	void slot_menuGameActivated( int num );
	
	/** Slot for writing on the StatusBar */
	void slot_status( QString text );
	
	/** Slot for reading socket */
	void slot_readSocket();
	
private:
	enum MENU_FIL {
		FIL_NET,
		FIL_MSG,
		FIL_QUIT
	};
	
	enum MENU_GAM {
		GAM_INFO,
		GAM_LORD,
		GAM_NEXT,
		GAM_TURN,
		GAM_POWER
	};
	
	/** define menus */
	void initMenuBar();
	
	/** define statusBar */
	void initStatusBar();

	Fight * _fight;
	
	AttalSocket * _socket;

};
 
#endif // INTERFACEDUEL_H   
