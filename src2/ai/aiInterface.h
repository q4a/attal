/****************************************************************
**
** Attal : Lords of Doom
**
** aiInterface.h
** interface for the ai-client
**
** Version : $Id: aiInterface.h,v 1.18 2013/11/19 21:40:59 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/02/2001
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


#ifndef AIINTERFACE_H
#define AIINTERFACE_H

// generic include files
#include <stdio.h>
// include files for QT
#include <QAbstractSocket>
#include <QDialog>
#include <QKeySequence>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QString>
#include <QVector>

// application specific include files
#include "libCommon/log.h"

class QAction;
class QTextEdit;
class QSignalMapper;

class Analyst;
class AttalSocket;
class ConfigConnection;


/*              ------------------------------
 *                         Interface
 *              ------------------------------ */


/** comment for the class */
class AiInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	AiInterface();

	/** Destructor */
	virtual ~AiInterface();

public slots:
	/** Slot managing 'File' menu */
	void slot_action( int num );

	void slot_log( QString msg );
	
	void slot_logAi( QString msg );
	
protected:
	virtual void changeEvent ( QEvent * e );

private slots:
	void slot_disconnect();
	
	void slot_connect();

private:
	enum MENU_ACTIONS {
		ACTION_CONNECT=0,
		ACTION_DISCONNECT,
		ACTION_LOGVERBOSE,
		ACTION_LOGNORMAL,
		ACTION_LOGCRITICAL,
		ACTION_LOGQUIET,
		ACTION_QUIT,

		NB_ACTIONS
	};

	/** Defines menus */
	void initMenuBar();

	/** Defines statusBar */
	void initStatusBar();

	/** Defines actions */
	void initActions();

	/** Add a new action */
	QAction * addAction( const QString & label, const QKeySequence & shortcut, MENU_ACTIONS id, QSignalMapper * sigmap );


	void actionConnect(bool fast);

	void actionDisconnect();

	void actionQuit();

	QMenu * _menuLog;
	QTextEdit * _edit;
	Analyst * _analyst;
	QVector<QAction *> _actions;
	ConfigConnection * _config;
	IALogLevel iaLogLevel;
};

/** Dialog for configuring connection to server */
class ConfigConnection : public QDialog
{
	Q_OBJECT
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
	
	void accept() { QDialog::accept(); }

private:
	QLineEdit * _host, * _port, * _name;
};

#define FIXEDSIZE( wid ) (wid)->setFixedSize( (wid)->sizeHint() )



#endif // INTERFACE_H
