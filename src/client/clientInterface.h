/****************************************************************
**
** Attal : Lords of Doom
**
** clientInterface.h
** Manages the whole game
**
** Version : $Id: clientInterface.h,v 1.1 2002/11/08 12:34:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 17/08/2000
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


#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

// include files for QT
#include <qmainwindow.h>
#include <qapplication.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qdialog.h>
// application specific includes
#include "common/genericBuilding.h"
#include "client/player.h"
#include "client/game.h"
#include "client/gui.h"

class QPopupMenu;

class AttalSocket;
class DisplayBase;
class Fight;
class GainLevel;
class GenericLord;
class ConfigConnection;

/*              ------------------------------
 *                      ClientInterface
 *              ------------------------------ */


/** comment for the class */
class ClientInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ClientInterface();

	/** Destructor */
	~ClientInterface();

public slots:
	/** Slot for managing 'File' menu */
	void slot_menuFileActivated( int num );

	/** Slot for managing 'Game' menu */
	void slot_menuGameActivated( int num );

	/** Slot for writing in the StatusBar */
	void slot_status( QString text );

	/** Slot for reading the socket */
	void slot_readSocket();

	/** Slot for displaying the base */
	void slot_base( GenericBase * base );

	/** Slot for quitting the base */
	void slot_map();

	/** Slot for beginning fight */
	void slot_fight( GenericLord * lord, CLASS_FIGHTER cla );

protected:
	void closeEvent( QCloseEvent* );

private:
	enum MENU_FIL {
		FIL_CON,
		FIL_DIS,
		FIL_QUIT
	};

	enum MENU_GAM {
		GAM_MINIMAP
	};

	enum StateGame {
		SG_MAP,
		SG_FIGHT,
		SG_BASE
	};

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	/** Manage socket SO_FIGHT */
	void socketFight();

	/** Manage socket QR */
	void socketQR();

	void showFightResults( char result );

	void updateUnits();

	void quitting();

	Game * _game;
	Fight * _fight;
	DisplayBase * _base;
	AttalSocket * _socket;
	GainLevel * _level;
	ConfigConnection * _config;
	bool _mini;
	QPopupMenu * _menuGame;

	StateGame _state;
};

/** Dialog for configuring connection to server */
class ConfigConnection : public QDialog
{
public:
	/** Constructor */
	ConfigConnection( QWidget * parent = 0, const char * name = 0 );

	/** Setting hostname */
	void setHost( QString host ) { _host->setText( host ); }

	/** Getting hostname */
	QString getHost() { return _host->text(); }

	int getPort() { return _port->text().toInt(); }

	/** Setting port number */
	void setPort( int port ) { _port->setText( QString::number( port ) ); }

	/** Getting player's name */
	QString getPlayerName() { return _name->text(); }

	/** Setting player's name */
	void setPlayerName( QString name ) { _name->setText( name ); }

private:
	QLineEdit * _host, * _port, * _name;
};

#endif // CLIENTINTERFACE_H
