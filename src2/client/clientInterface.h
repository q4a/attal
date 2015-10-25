/****************************************************************
**
** Attal : Lords of Doom
**
** clientInterface.h
** Manages the whole game
**
** Version : $Id: clientInterface.h,v 1.80 2013/11/20 10:46:52 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QAbstractSocket>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QKeySequence>
#include <QString>
#include <QVector>
#include <QStackedWidget>

// application specific includes
#include "libCommon/displayHelp.h"
#include "libCommon/genericBuilding.h"
#include "libClient/game.h"
#include "libClient/gui.h"
#include "libClient/ressourceBar.h"
#include "libServer/loadGame.h"



class QProcess;

class QAction;
class QProgressDialog;
class QSignalMapper;

class AttalSocket;
class DisplayBase;
class Fight;
class GenericLord;
class ConfigConnection;
class StartGameDialog;
class AttalMessage;
class AttalServer;
class Engine;
class Analyst;

/*              ------------------------------
 *                      ClientInterface
 *              ------------------------------ */


class ClientInterface : public QMainWindow,  public LoadGame
{
	Q_OBJECT
public:
	/** Constructor */
	ClientInterface();

	/** Destructor */
	~ClientInterface();

public slots:
	/** Slot for managing 'actions' */
	void slot_action( int num );

	/** Slot for writing in the StatusBar */
	void slot_status( const QString & text );
	
	/** Slot for reading the socket */
	void slot_readSocket();
	
	/** when connection is closed by server **/
	void slot_connectionClosed();
	
	void slot_endGame();
	
	void slot_hostfound();
	
	/** manage connection's errors **/
	void slot_error( QAbstractSocket::SocketError error );

	/** Slot for displaying the base */
	void slot_base( GenericBase * base );

	/** Slot for quitting the base */
	void slot_map();

	/** Slot for beginning fight */
	void slot_fight( GenericLord * lord, CLASS_FIGHTER cla );
	
	void slot_clientConnect();

	void slot_quit() { actionQuit(); }
	
	void slot_newScen();
	
	void slot_newCamp();
	
	void slot_loadScen();
	
	void slot_loadCamp();
	
	void slot_dialogClosed();

	void slot_ready();
	
	void slot_endConnection( QString name );

	void slot_endTurn();
	
	void slot_beginTurn();

	void slot_result(bool result);
	
	void slot_resourceBar();

	void slot_loadThemeStep();
	
	void actionOptionsAnimations(int state);

signals:
	
	void sig_newMessage( const QString & );

	void sig_map();
	
	void sig_baseDisplay();
	
	void sig_fightDisplay();
	
	void sig_widgChanged( int );
	
	void sig_animations();
	
	void sig_dispositions();
	
	void sig_gameInfo();

protected:
	void closeEvent( QCloseEvent* );
	
	void updateScreen();

	void resizeWidget();
	
	virtual void changeEvent ( QEvent * e );

private:
	enum MENU_ACTIONS {
		ACTION_CONNECT = 0,
		ACTION_START,
		ACTION_END,
		ACTION_SAVE,
		ACTION_FASTCONNECT ,
		ACTION_DISCONNECT,
		ACTION_MINIMAP,
		ACTION_FULL,
		ACTION_CHAT,
		ACTION_OPTIONS,
		ACTION_INFO,
		ACTION_ENDTURN,
		ACTION_NEXTLORD,
		ACTION_NEXTBASE,
		ACTION_HELP,
		ACTION_ABOUT,
		ACTION_QUIT,
		
		ACTION_FREESIZE,
		ACTION_800,
		ACTION_1024,
		ACTION_1280,

		ACTION_TESTSTYLE,

		NB_ACTIONS
	};

	enum StateGame {
		SG_MAP,
		SG_FIGHT,
		SG_BASE
	};

	enum SizeMode {
		SIZEMODE_FREE,
		SIZEMODE_800,
		SIZEMODE_1024,
		SIZEMODE_1280
	};

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	void initActions();

	void initProgressBar();
	
	void loadMenu( bool show );

	/** Add a new action */
	QAction * addAction( const QString & label, const QKeySequence & shortcut, MENU_ACTIONS id, QSignalMapper * sigmap );

	/** Manage socket SO_FIGHT */
	void socketFight();

	/** Manage socket QR */
	void socketQR();

	void showFightResults( char result );

	void updateUnits();

	void actionConnect(bool fast);

	void actionStart();

	void actionEnd();
	
	void actionFastConnect();

	void actionDisconnect();

	void actionMinimap();
	
	void actionFullScreen();
		
	void actionPopupChat();
	
	void actionOptions();
	
	void actionInfo();
	
	void actionNextLord();
	
	void actionNextBase();
	
	void actionHelp();

	void actionAbout();

	bool actionQuit();
	
	void actionResize( SizeMode mode );

	void actionTestStyle();
	
	void killServer();
	
	void addInternalAI();
	
	bool killAI();
				
	void adjustWidgetLoading(  StatusWidget type );

	void disconnectClient();
	
	void disconnectAI();
	
	void reinit();

	Game * _game;
	Fight * _fight;
	DisplayBase * _base;
	AttalSocket * _socket;
	StartGameDialog * _startDialog;
	bool _mini, _inLoad;
	bool _full;
	QProcess * _proc;
	QVector<QAction *> _actions;
	QList<Analyst *> _aiList;
	QStackedWidget * _centralWindow;
	AttalMessage * _msg;
	QProgressDialog * _progress;
	int _progressNum;
	QStatusBar * _statusBar;
	RessourceBar * _resourceBar;
	QLabel * _cal;
	int _numWidg[3];
	QSignalMapper * _sigmap;

	StateGame _state;
};

#endif // CLIENTINTERFACE_H
