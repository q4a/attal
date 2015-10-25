/****************************************************************
**
** Attal : Lords of Doom
**
** aiInterface.cpp
** interface for the ia-client
**
** Version : $Id: aiInterface.cpp,v 1.40 2013/11/22 12:22:25 lusum Exp $
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


#include "aiInterface.h"

// generic include files
#include <assert.h>
#include <stdarg.h>
// include files for QT
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QSignalMapper>
#include <QStatusBar>
#include <QMessageBox>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTextCursor>
#include <QVBoxLayout>

// application specific includes
#include "conf.h"

#include "libCommon/log.h"
#include "libCommon/attalSocket.h"
#include "libCommon/dataTheme.h"

#include "libAi/analyst.h"

extern QString DATA_PATH;
extern QString THEME;
extern QString PORT;
extern bool FAST;

QTextEdit * screen;


AiInterface::AiInterface()
{
	aifLog.setAiLogLevel(LOG_NORMAL);
	if( DataTheme.init() ) {

		_analyst = new Analyst();
		_analyst->start();

		initActions();
		initMenuBar();
		initStatusBar();

		_edit = new QTextEdit( this );
		_edit->setReadOnly( true );
		screen = _edit;
		setCentralWidget( _edit );
		_config=0;

		connect( _analyst, SIGNAL( sig_disconnected()),SLOT(slot_disconnect()));
		connect( _analyst, SIGNAL( sig_connected()),SLOT(slot_connect()));

		connect( &aifLog, SIGNAL( sig_print( QString ) ), SLOT( slot_log( QString ) ) );
		connect( &aifLog, SIGNAL( sig_printAi( QString ) ), SLOT( slot_logAi( QString ) ) );

		if(FAST)
			_analyst->connectAi("localhost",PORT.toInt());
	} else {	
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}
}

AiInterface::~AiInterface()
{
	if(_config){
		delete _config;
	}

	DataTheme.clear();
}
	
void AiInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch (e->type()){
	case QEvent::WindowTitleChange:
	{
		setWindowTitle( tr( "Attal: IA client" ) );
	}
	break;
	default:
	QWidget::changeEvent ( e );
	break;
	}
}

void AiInterface::slot_log( QString msg )
{
	screen->append( msg );
}

void AiInterface::slot_logAi( QString result)
{
	if( ( !screen ) )
		return;

	screen->append( result );
	screen->textCursor().atEnd();
}

void AiInterface::initStatusBar()
{
	statusBar()->showMessage( tr( "Log level : Normal" ), 0 );
}

QAction * AiInterface::addAction( const QString & label, const QKeySequence & shortcut, MENU_ACTIONS id, QSignalMapper * sigmap )
{
	QAction * action = 0;

	action = new QAction( label, this );
	action->setShortcut( shortcut );
	_actions.insert( id, action );
	sigmap->setMapping( action, id );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );

	return action;
}

void AiInterface::initActions()
{
	_actions.resize( NB_ACTIONS );

	QAction * action;
	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "&Connect to server" ), QKeySequence( tr( "CTRL+C" ) ), ACTION_CONNECT, sigmap );
	action = addAction( tr( "&Disconnect" ), QKeySequence( tr( "CTRL+D" ) ), ACTION_DISCONNECT, sigmap );
	action->setEnabled( false );

	QActionGroup * groupLog = new QActionGroup( this );
	action = addAction( tr( "Verbose" ), QKeySequence( tr( "" ) ), ACTION_LOGVERBOSE, sigmap );
	action->setCheckable( true );
	groupLog->addAction( action );
	action = addAction( tr( "Normal" ), QKeySequence( tr( "" ) ), ACTION_LOGNORMAL, sigmap );
	action->setCheckable( true );
	groupLog->addAction( action );
	action->setChecked( true );
	action = addAction( tr( "Critical" ), QKeySequence( tr( "" ) ), ACTION_LOGCRITICAL, sigmap );
	action->setCheckable( true );
	groupLog->addAction( action );
	action = addAction( tr( "Quiet" ), QKeySequence( tr( "" ) ), ACTION_LOGQUIET, sigmap );
	action->setCheckable( true );	
	groupLog->addAction( action );
	action = addAction( tr( "&Quit" ), QKeySequence( tr( "CTRL+Q" ) ), ACTION_QUIT, sigmap );

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_action( int ) ) );
}
/*!
  define menus
*/

void AiInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	
	menuFile->addAction( _actions[ ACTION_CONNECT ] );
	menuFile->addAction( _actions[ ACTION_DISCONNECT ] );
	_menuLog = menuFile->addMenu( tr( "Log level" ) );
	menuFile->addAction( _actions[ ACTION_QUIT ] );
	
	_menuLog->addAction( _actions[ ACTION_LOGVERBOSE ] );
	_menuLog->addAction( _actions[ ACTION_LOGNORMAL ] );
	_menuLog->addAction( _actions[ ACTION_LOGCRITICAL ] );
	_menuLog->addAction( _actions[ ACTION_LOGQUIET ] );

}

void AiInterface::slot_action( int num )
{
	switch( num ) {
	case ACTION_CONNECT:
		actionConnect(false);
		break;
	case ACTION_DISCONNECT:
		actionDisconnect();
		break;
	case ACTION_LOGVERBOSE:
		statusBar()->showMessage( tr( "Log level : Verbose" ), 0 );
		aifLog.setAiLogLevel(LOG_VERBOSE);
		break;
	case ACTION_LOGNORMAL:
		statusBar()->showMessage( tr( "Log level : Normal" ), 0 );
		aifLog.setAiLogLevel(LOG_NORMAL);
		break;
	case ACTION_LOGCRITICAL:
		statusBar()->showMessage( tr( "Log level : Critical" ), 0 );
		aifLog.setAiLogLevel(LOG_CRITICAL);
		break;
	case ACTION_LOGQUIET:
		statusBar()->showMessage( tr( "Log level : Quiet" ), 0 );
		aifLog.setAiLogLevel(LOG_QUIET);
		break;
	case ACTION_QUIT:
		actionQuit();
		break;
	}
}

void AiInterface::actionConnect(bool fast)
{
	TRACE("actionConnect, fast %d", fast);
	
	if( _analyst->getSocketState() == QAbstractSocket::UnconnectedState  ) {
		if( _config == 0 ) {
			_config = new ConfigConnection( this );
		}
		_config->setHost( "localhost" );
		_config->setPort( PORT.toInt() );
		_config->setPlayerName( "IA" );
		_config->exec();

		if( _config->result() == QDialog::Accepted || fast) {
			_analyst->connectAi(_config->getHost(), _config->getPort() );
		}
	} else {
		QMessageBox::critical( this, tr( "Can't connect" ), tr( "You're already connected to a server. Please disconnect first." ) );
	}

}

void AiInterface::slot_connect()
{
	_actions[ ACTION_CONNECT ]->setEnabled( false );
	_actions[ ACTION_DISCONNECT ]->setEnabled( true );
}

void AiInterface::slot_disconnect()
{
	_actions[ ACTION_CONNECT ]->setEnabled( true );
	_actions[ ACTION_DISCONNECT ]->setEnabled( false );
}

void AiInterface::actionDisconnect()
{
	_analyst->disconnectAi(); 
}

void AiInterface::actionQuit()
{
	qApp->quit();
}

//
// ----- ConfigConnection -----
//

ConfigConnection::ConfigConnection( QWidget * parent, const char * /* name */ )
	: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Connection to server" ) );
	QVBoxLayout * layout = new QVBoxLayout( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	QLabel * labHost = new QLabel( tr( "Host : " ), this );
	FIXEDSIZE( labHost );
	layH1->addWidget( labHost );
	layH1->addSpacing( 5 );
	_host = new QLineEdit( this );
	_host->setFixedSize( 160, 20 );
	layH1->addWidget( _host );
	layH1->addStretch( 1 );
	layout->addLayout( layH1, 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	QLabel * labPort = new QLabel( tr( "Port : " ), this );
	FIXEDSIZE( labPort );
	layH2->addWidget( labPort );
	layH2->addSpacing( 5 );
	_port = new QLineEdit( this );
	_port->setFixedSize( 80, 20 );
	layH2->addWidget( _port );
	layH2->addStretch( 1 );
	layout->addLayout( layH2, 1 );

	QHBoxLayout * layH4 = new QHBoxLayout();
	layH4->addSpacing( 5 );
	QLabel * labName = new QLabel( tr( "Name : " ), this );
	FIXEDSIZE( labName );
	layH4->addWidget( labName );
	layH4->addSpacing( 5 );
	_name = new QLineEdit( this );
	_name->setFixedSize( 80, 20 );
	layH4->addWidget( _name );
	layH4->addStretch( 1 );
	layout->addLayout( layH4, 1 );

	QHBoxLayout * layH3 = new QHBoxLayout();
	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Connect" ) );
	FIXEDSIZE( pbOk );
	layH3->addStretch( 1 );
	layH3->addWidget( pbOk );
	layH3->addStretch( 1 );
	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );
	layH3->addWidget( pbCan );
	layH3->addStretch( 1 );
	layout->addLayout( layH3, 2 );

	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );

	setFixedSize( 250, 150 );
}
