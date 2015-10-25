/****************************************************************
**
** Attal : Lords of Doom
**
** serverInterface.h
** interface for the server
**
** Version : $Id: serverInterface.h,v 1.2 2002/11/09 20:16:37 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 01/11/2000
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


#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

// include files for QT
#include <qmainwindow.h>
#include <qstring.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
// application specific includes
#include "server/attalServer.h"
#include "server/engine.h"

class QVButtonGroup;
class QHButtonGroup;
class ServerWidget;
class QLineEdit;
class QListBox;
class QListView;

/*              ------------------------------
 *                      ServerInterface
 *              ------------------------------ */


/** comment for the class */
class ServerInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ServerInterface();

public slots:
	/** Slot managing 'File' menu */
	void slot_menuFileActivated( int num );

	/** Slot managing the status bar */
	void slot_status( QString text );

	/** Slot for ending game */
	void slot_stop();

	/** Slot for loading scenario */
	void slot_load( QString );

	/** Slot for saving game */
	void slot_save();

private:
	enum MENU_FIL {
		FIL_LOAD,
		FIL_SAVE,
		FIL_END,
		FIL_QUIT
	};

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	bool init();

	AttalServer * _server;
	Engine * _engine;
	ServerWidget * _widget;
};

/** radio button allowing to choose a file on the disk */
class ChooseFileRadioButton : public QRadioButton
{
	Q_OBJECT
public:
	/** Constructor */
	ChooseFileRadioButton( QWidget * parent = 0, const char * name = 0 );

	/** Return file in the lineedit */
	QString getText() { return _edit->text(); }

public slots:
	/** Slot for choosing file */
	void slot_choose();

	/** Toggle radio button */
	void slot_toggle( bool );

private:
	QLineEdit * _edit;
	QPushButton * _choose;
};

/** Main widget for the server */
class ServerWidget : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ServerWidget( QWidget * parent = 0, const char * name = 0 );

	/** Toggle buttons state as game started or not */
	void setGameLoaded( bool b );

public slots:
	/** Slot for handling buttons */
	void slot_choice( int );

	/** Slot for handling new players */
	void slot_newPlayer( AttalPlayerSocket * player );

	/** slot for handling disconnected players */
	void slot_endConnection( QString name );

signals:
	/** Signal for buttopn 'End' */
	void sig_stop();

	/** Signal for button 'Load' */
	void sig_load( QString );

	/** Signal for button 'Save' */
	void sig_save();

private:
	QVButtonGroup * _group;
	QHButtonGroup * _groupBottom;
	ChooseFileRadioButton * _radio3;
	QListView * _playerList;
	bool _loaded;
};


#endif // INTERFACE_H
