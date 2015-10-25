/****************************************************************
**
** Attal : Lords of Doom
**
** serverInterface.h
** interface for the server
**
** Version : $Id: serverInterface.h,v 1.39 2013/11/22 12:22:25 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QCloseEvent>
#include <QDialog>
#include <QEvent>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QString>
#include <QTreeWidget>
#include <QVector>


// application specific includes
#include "libServer/attalServer.h"

#include "libAi/analyst.h"

#include "libServer/engine.h"
#include "libServer/loadGame.h"

class QAction;
class QLineEdit;
class QListWidget;
class QSignalMapper;

class ServerWidget;
class ConfigConnection;

/*              ------------------------------
 *                      ServerInterface
 *              ------------------------------ */


/** comment for the class */
class ServerInterface : public QMainWindow, public LoadGame
{
	Q_OBJECT
public:
	/** Constructor */
	ServerInterface();
	
	~ServerInterface();

	bool initServer();	

public slots:
	/** Slot for managing 'actions' */
	void slot_action( int num );

	/** Slot managing the status bar */
	void slot_status( QString text );

	/** Slot for ending game */
	void slot_stop();

	/** Slot for loading scenario */
	void slot_load( QString );

	/** Slot for saving game */
	void slot_save();
	
	void slot_result( int, bool);
	
	void slot_endGame( int );
	
	void slot_endConnection( QString name );

	void slot_started();
	
	void slot_banned( QString );
	
	void slot_ready();
	
	bool fillWithExternalAI(QString filename);

	void adjustWidgetLoading( StatusWidget type );

protected:
	void closeEvent(QCloseEvent* ce);
	
	virtual void changeEvent ( QEvent * e );

private:
	enum MENU_ACTIONS {
		ACTION_LOADSCENARIO,
		ACTION_LOADCAMPAIGN,
		ACTION_CONTCAMPAIGN,
		ACTION_LOADGAME,
		ACTION_SAVE,
		ACTION_END,
		ACTION_QUIT,
		
		ACTION_FILLAI,
		ACTION_FILL_EXTERNAL_AI,
		ACTION_ADDAI,
		ACTION_AIDBG,
		ACTION_HIDE,
		
		NB_ACTIONS
	};
		
	/** define actions */
	void initActions();

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	/** Add new action */
	QAction * addAction( const QString & label, MENU_ACTIONS id, QSignalMapper * sigmap );
	
	void addAI(bool hide);
	
	void addInternalAI();
	
	void disconnectAI();
	
	bool killAI();

	ServerWidget * _widget;
	
	uint _actScen, _nbScen;

	QVector<QAction *> _actions;
	QList<QProcess *> _proclist;
	QList<Analyst *> _aiList;
	bool _hide;
};

#endif // SERVERINTERFACE_H
