/****************************************************************
**
** Attal : Lords of Doom
**
** serverInterface.cpp
** interface for the server
**
** Version : $Id: serverInterface.cpp,v 1.93 2013/11/22 12:22:25 lusum Exp $
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


#include "serverInterface.h"

// generic include files
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/// XXX: usefull ?
#ifndef WIN32
#include <sys/types.h>
#include <sys/wait.h>
#endif

#include <signal.h>
// include files for QT
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>
#include <QStringList>
#include <QTimer>

// application specific includes
#include "conf.h"
#include "libCommon/campaign.h"
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

#include "serverWidgets.h"
#include "displayScenarii.h"

extern QString DATA_PATH;
extern QString SCENARIO_PATH;
extern QString CAMPAIGN_PATH;
extern QString SAVE_PATH;
extern QString THEME;
extern QString PORT;
extern bool FAST;

#define CLIENT

ServerInterface::ServerInterface()
	:QMainWindow()
{
	initActions();
	initMenuBar();
	initStatusBar();
	if( ! DataTheme.init() ) {
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}

	_widget = new ServerWidget( this );
	setCentralWidget( _widget );

	connect( _widget, SIGNAL( sig_stop() ), SLOT( slot_stop() ) );
	/* emitted when starting game in ServerWidgets */
	connect( _widget, SIGNAL( sig_load( QString ) ), SLOT( slot_load( QString ) ) );
	connect( _widget, SIGNAL( sig_save() ), SLOT( slot_save() ) );
	connect( _widget, SIGNAL( sig_banned( QString ) ), SLOT( slot_banned( QString ) ) );

	_hide = false;
	_local = false;

	setMinimumSize( 350, 200 );
	srand( time( NULL ) );
	connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( slot_stop() ) );
	//connect( qApp, SIGNAL( destroyed() ), this, SLOT( slot_stop() ) );
}

ServerInterface::~ServerInterface()
{
	killAI();
	DataTheme.clear();

	endEngine();
	endServer();
}

void ServerInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch (e->type()){
	case QEvent::WindowTitleChange:
	{
		setWindowTitle( tr( "Attal - Lords of Doom (Server)" ) );
	}
	break;
	default:
	QWidget::changeEvent ( e );
	break;
	}
}

void ServerInterface::closeEvent( QCloseEvent* ce )
{
	ce->accept();
}

bool ServerInterface::initServer()
{
	bool ret = false;

	ConfigConnection config;
	config.setHost( "localhost" );
	config.setPort( PORT.toInt() );

	if(FAST){
		config.accept();
	} else { 
		config.exec();
	}

	if( config.result() == QDialog::Accepted ) {
		_server = new AttalServer( config.getPort() );

		if( _server->isListening() ) {
			
			connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), this, SLOT( slot_ready() ) );
			connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), _widget, SLOT( slot_newPlayer( AttalPlayerSocket * ) ) );
			connect( _server, SIGNAL( sig_endConnection( QString ) ), this , SLOT( slot_endConnection( QString ) ) );
			connect( _server, SIGNAL( sig_endConnection( QString ) ), _widget, SLOT( slot_endConnection( QString ) ) );
			connect( _server, SIGNAL( sig_result( int, bool) ), this, SLOT( slot_result(int, bool)) );
			connect( _server, SIGNAL( sig_endGame( int ) ), this, SLOT( slot_endGame( int )) );
			connect( _server, SIGNAL( sig_stop() ), this, SLOT( slot_stop()) );

			ret = true;

			newEngine();

		} else {
			endServer();
			if( QMessageBox::warning( this,
						tr( "Server error" ),
						tr( "Could not listen for sockets." ),
						tr( "Try again" ),
						tr( "Quit" ),
						0, 0, 1 )  == 0 ) {
				ret = initServer();
			} else {
				ret = false;
				qApp->quit();
			}
		}
	} else {
		ret = false;
		qApp->quit();
	}

	return ret;
}

void ServerInterface::initStatusBar()
{
	statusBar()->showMessage( tr( "Status Bar" ), 0 );
}

QAction * ServerInterface::addAction( const QString & label, MENU_ACTIONS id, QSignalMapper * sigmap )
{
	QAction * action;

	action = new QAction( label, this );
	_actions.insert( id, action );
	sigmap->setMapping( action, id );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );

	return action;
}

void ServerInterface::initActions()
{
	_actions.resize( NB_ACTIONS );
	
	QAction * action;
	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "Load scenario" ), ACTION_LOADSCENARIO, sigmap );
	addAction( tr( "Load campaign" ), ACTION_LOADCAMPAIGN, sigmap );
	addAction( tr( "Continue campaign" ), ACTION_CONTCAMPAIGN, sigmap );
	addAction( tr( "Load game" ), ACTION_LOADGAME, sigmap );
	addAction( tr( "Save game" ), ACTION_SAVE, sigmap );
	addAction( tr( "End game" ), ACTION_END, sigmap );
	addAction( tr( "Quit" ), ACTION_QUIT, sigmap );
	addAction( tr( "Fill with AI" ), ACTION_FILLAI, sigmap );
	addAction( tr( "Fill with External AI" ), ACTION_FILL_EXTERNAL_AI, sigmap );
	addAction( tr( "Add external AI player" ), ACTION_ADDAI, sigmap );
	action = addAction( tr( "Debug AI" ), ACTION_AIDBG, sigmap );
	action->setCheckable( true );
	action->setChecked( _aiDbg );
	
	action = addAction( tr( "Hide AI" ), ACTION_HIDE, sigmap );
	action->setCheckable( true );
	action->setChecked( _hide );
			
	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_action( int ) ) );
}

void ServerInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	QMenu * menuGame = menuBar()->addMenu( tr( "&Game" ) );

	menuFile->addAction( _actions[ ACTION_LOADSCENARIO ] );
	menuFile->addAction( _actions[ ACTION_LOADCAMPAIGN ] );
	menuFile->insertSeparator(_actions[ ACTION_LOADCAMPAIGN ]);
	menuFile->addAction( _actions[ ACTION_CONTCAMPAIGN ] );
	menuFile->addAction( _actions[ ACTION_LOADGAME ] );
	menuFile->addAction( _actions[ ACTION_SAVE ] );
	menuFile->addAction( _actions[ ACTION_END ] );
	menuFile->addAction( _actions[ ACTION_QUIT ] );

	menuGame->addAction( _actions[ ACTION_FILLAI ] );
	menuGame->addAction( _actions[ ACTION_FILL_EXTERNAL_AI ] );
	menuGame->addAction( _actions[ ACTION_ADDAI ] );
	menuGame->addAction( _actions[ ACTION_AIDBG ] );
	menuGame->addAction( _actions[ ACTION_HIDE ] );
	menuGame->addAction( _actions[ ACTION_HIDE ] );

}

void ServerInterface::slot_action( int num )
{
	switch( num ) {
	case ACTION_LOADSCENARIO:{
		DisplayScenariiDialog * scen = new DisplayScenariiDialog( this );
		if( scen->exec() ) {
			slot_load( scen->getFileName() );
		}
		} break;
	case ACTION_LOADCAMPAIGN:{
		QString filename;
		filename = QFileDialog::getOpenFileName( this, tr( "Load campaign" ), CAMPAIGN_PATH, "*.cmp" );
		loadCampaign( filename );
		} break;
	case ACTION_CONTCAMPAIGN:{
		QString filename;
		filename = QFileDialog::getOpenFileName( this, tr( "Load campaign" ), SAVE_PATH, "*.cms" );
		loadCampaign( filename );
		} break;
	case ACTION_LOADGAME:{
		QString filename;
		filename = QFileDialog::getOpenFileName( this, tr( "Load game" ), SAVE_PATH, "*.scn *.gam" );
		slot_load( filename );
		} break;
	case ACTION_SAVE:
		slot_save();
		break;
	case ACTION_END:
		slot_stop();
		break;
	case ACTION_QUIT:
		qApp->quit();
		break;
	case ACTION_FILLAI:{
		QString filename = _widget->getFilename();
		fillWithAI(filename);
	  } break;
	case ACTION_FILL_EXTERNAL_AI:{
		QString filename = _widget->getFilename();
		fillWithExternalAI(filename);
	  } break;
	case ACTION_ADDAI:
		addAI(false);
		break;
	case ACTION_AIDBG:{
		_aiDbg = !_aiDbg;
	  _actions[ ACTION_AIDBG ]->setChecked( _aiDbg );
		}
		break;
	case ACTION_HIDE:{
			_hide = !_hide;
			_actions[ ACTION_HIDE ]->setChecked( _hide );
		}
		break;
	}
}

bool ServerInterface::killAI()
{
	int count;
	Analyst * ai;
		
	count = _aiList.count();
	TRACE("ServerInterface::killAI count %d",count);

	if( _aiList.count() > 0 ) {
		for( int i = 0; i < count; i++ ) {
				ai = _aiList.takeFirst();
				//ai->disconnectAi();
				ai->exit(0);
				ai->wait();
				delete ai;
		}
		return true;	
	}

	count = _proclist.count();
	if( _proclist.count() > 0 && !_aiDbg) {
		for( int i = 0; i < count; i++ ) {
			_proclist.at(0)->terminate();
			_proclist.at(0)->waitForFinished();
			//QTimer::singleShot( 5000, _proclist.at(0), SLOT( kill() ) );
			delete _proclist.takeFirst();
		}
		return true;	
	}
	return false;
}

void ServerInterface::disconnectAI()
{	
	TRACE("ServerInterface::disconnectAI");

	Analyst * ai;

	int count = _aiList.count();
	if( _aiList.count() > 0 ) {
		for( int i = 0; i < count; i++ ) {
				ai = _aiList.at(i);
				ai->disconnectAi();
		}
	}

}

bool ServerInterface::fillWithExternalAI(QString filename)
{
	//logDD("filename %s", qPrintable( filename ) );

	int neededPlay = neededAiPlayers( filename );
	if ( neededPlay ) {
		QTimer::singleShot( (neededPlay+2)* 1000, this, SLOT( slot_ready() ) );
		for( int i = 0; i < neededPlay; i++ ) {
			_readyIn = false;
			//logDD("ai %d",i);
			addAI(_hide);	
			while( !_readyIn ){
				qApp->processEvents();
			}
		}	
		return true;
	}
	return false;
}

void ServerInterface::addInternalAI()
{

 TRACE("ServerInterface::addInternalAI");

	QStringList arglist;
	Analyst * ai = new Analyst();
	ai->connectAi( "localhost", _server->serverPort() );
	_aiList.append( ai );
	ai->start();
}
	
void ServerInterface::addAI(bool hide)
{
	TRACE("Add ai, hide %d", hide );
	QProcess * proc = new QProcess(this);
	QStringList arglist;


	connect(proc, SIGNAL(started()), SLOT(slot_started())); 
	_proclist.append( proc );
  
	if( hide ) {
		arglist.append( "--hide" );
	}
	/*do not put space in arguments to append "--port", not "--port "*/
	arglist.append("--port");
 	arglist.append(QString::number(_server->serverPort()));
	arglist.append("--fast");
	/* uncomment to debug */
//	for( int i = 0 ; i < arglist.length(); i++ )
//		TRACE("%s", qPrintable( arglist.at(i) ) );


	_readyIn = false;
#ifdef WIN32
	proc->start( "attal-ai.exe", arglist );
#else
	QFile file("./attal-ai");

	if (file.exists()) {
		proc->start( "./attal-ai", arglist );
	} else {
		proc->start( "attal-ai", arglist );
	}
#endif
}

void ServerInterface::slot_started()
{
	qApp->processEvents();
}

void ServerInterface::slot_banned( QString name)
{
	_server->closeConnectionPlayer( name, AttalServer::SK_KICKED );
}

void ServerInterface::slot_ready()
{
	TRACE("ServerInterface::slot_ready");

	_readyIn =  true;
}

void ServerInterface::slot_endConnection( QString /*name*/ )
{
	TRACE("ServerInterface::slot_endConnection");
}

void ServerInterface::slot_status( QString text )
{
	statusBar()->showMessage( text, 0 );
}

void ServerInterface::slot_stop()
{

  TRACE("ServerInterface::slot_stop");
/*
	if( _engine ) {
		_engine->endGameExternal();
	}
	*/
	endGame();
}

void ServerInterface::slot_load( QString filename )
{
	load( filename );
}

void ServerInterface::slot_save()
{
	
	TRACE("ServerInterface::slot_save");

	save();
}

void ServerInterface::slot_result( int /* id */, bool result )
{
	TRACE("ServerInterface::slot_result( bool result %d )", result );
	
	_winner = result;
	
}

void ServerInterface::slot_endGame( int /* id */ )
{
	TRACE("ServerInterface::slot_endGame");
	
	handleCheckEndGame();

	TRACE("ServerInterface::slot_endGame ( finish )");
}

void ServerInterface::adjustWidgetLoading( StatusWidget type )
{
	
	TRACE("ServerInterface::adjustWidgetLoading(  StatusWidget type  %d)", type);
	
	switch( type ) {
		case W_ENGINE:
			_widget->setGameLoaded( true );
			break;
		case W_NORMAL:
			_widget->setGameLoaded( false );
			break;
		default:
			break;
	}
}
