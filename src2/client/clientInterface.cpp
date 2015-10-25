/****************************************************************
**
** Attal : Lords of Doom
**
** clientInterface.cpp
** Manages the whole game
**
** Version : $Id: clientInterface.cpp,v 1.188 2013/11/22 12:22:25 lusum Exp $
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
*	GNU General Public License for more details.
**
****************************************************************/


#include "clientInterface.h"

// generic include files
#include <limits>
// include files for QT
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>

#include <QFileDialog>
#include <QLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMenu>
#include <QObject>
#include <QProcess>
#include <QProgressDialog>
#include <QPushButton>
#include <QSignalMapper>
#include <QStatusBar>
#include <QTimer>


// application specific includes
#include "conf.h"
#include "client/clientWidgets.h"
#include "libCommon/attalSettings.h"
#include "libCommon/attalSocket.h"
#include "libCommon/calendar.h"
#include "libCommon/campaign.h"
#include "libCommon/log.h"
#include "libCommon/skill.h"
#include "libCommon/unit.h"
#include "libCommon/dataTheme.h"

#include "libClient/aboutDialog.h"
#include "libClient/attalStyle.h"
#include "libClient/building.h"
#include "libClient/chatWidget.h"
#include "libClient/displayLord.h"
#include "libClient/displayBase.h"
#include "libClient/game.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/optionsDialog.h"
#include "libClient/widget.h"

#include "libFight/fight.h"
#include "libFight/fightResult.h"

#include "libServer/attalServer.h"
#include "libServer/engine.h"

#include "libAi/analyst.h"

extern QString DATA_PATH;
extern QString THEME;
extern QString SCENARIO_PATH;
extern QString CAMPAIGN_PATH;
extern QString SAVE_PATH;
extern QString PORT;

//
// ----- ClientInterface -----
//

ClientInterface::ClientInterface()
{
			
	setWindowTitle( tr( "Attal - Lords of Doom" ) );
	
	_socket = NULL;
	_fight = NULL;
	_base = NULL;
	_state = SG_MAP;
	_mini = true;
	_full = true;
	_proc = NULL;
	_msg = NULL;
	_statusBar  = NULL;
	_progress = NULL;
	_progressNum = 0;
	_local = true;

	qApp->setStyle( new AttalStyle( DATA_PATH + "style.dat" ) );

	initActions();
	initMenuBar();
	_centralWindow = new QStackedWidget(this);
	setCentralWidget( _centralWindow );
	//loadMenu(true);
	initProgressBar();

	connect( & ImageTheme, SIGNAL( sig_loadStep() ), SLOT( slot_loadThemeStep() ) );
	
	if( DataTheme.init() && ImageTheme.init() ) {
		ImageTheme.enableSound( AttalSettings::getInstance()->getStrategyModeSettings().isSoundOn );
		ImageTheme.enableMusic( AttalSettings::getInstance()->getStrategyModeSettings().isMusicOn );
		_socket = new AttalSocket();
		_game = new Game( _centralWindow, _socket );
		_game->setObjectName( QString("AttalGame") );
		_numWidg[0] = _centralWindow->addWidget( _game );
		_centralWindow->setCurrentWidget( _game );
		initStatusBar();
		resizeWidget();

		connect( this, SIGNAL( sig_gameInfo() ), _game, SLOT( slot_gameInfo() ) );
		connect( this, SIGNAL( sig_animations() ), _game , SLOT( updateAnimations() ) );
		connect( this, SIGNAL( sig_dispositions() ), _game , SLOT( updateDispositionMode() ) );
		connect( _game, SIGNAL( sig_statusMsg( const QString & ) ), SLOT( slot_status( const QString & ) ) );
		connect( _game, SIGNAL( sig_base( GenericBase * ) ), SLOT( slot_base( GenericBase * ) ) );
		connect( _game, SIGNAL( sig_fight( GenericLord *, CLASS_FIGHTER ) ), SLOT( slot_fight( GenericLord *, CLASS_FIGHTER ) ) );
		connect( _game, SIGNAL( sig_endGame() ), SLOT( slot_endGame() ) );
		//connect( _game, SIGNAL( sig_endTurn() ), SLOT( slot_endTurn() ) );
		connect( _game, SIGNAL( sig_beginTurn() ), SLOT( slot_beginTurn() ) );
		connect( _game, SIGNAL( sig_result( bool ) ), SLOT( slot_result( bool ) ) );
		connect( _game, SIGNAL( sig_statusBar() ), SLOT( slot_resourceBar() ) );
		connect( this, SIGNAL( sig_map()), menuBar() , SLOT( show() ) );
		connect( this, SIGNAL( sig_map()) , _statusBar , SLOT( show() ) );
		connect( this, SIGNAL( sig_fightDisplay()) , menuBar() , SLOT( hide() ) );
		connect( this, SIGNAL( sig_baseDisplay() ), _statusBar , SLOT( hide() ) );
		connect( this, SIGNAL( sig_widgChanged(int)) , _centralWindow , SLOT( setCurrentIndex(int)) );
		
		ChatWidget * chat = this->findChild<ChatWidget *>("AttalGameChat");
		if( chat ) {
			connect( this, SIGNAL( sig_newMessage( QString ) ), chat, SLOT( slot_displayMessage( QString ) ) );
		}

		connect( _socket, SIGNAL( readyRead() ), SLOT( slot_readSocket() ) );
		connect( _socket, SIGNAL( hostFound() ), SLOT( slot_hostfound() ) );
		connect( _socket, SIGNAL( disconnected() ), SLOT( slot_connectionClosed() ) );
		connect( _socket, SIGNAL( error( QAbstractSocket::SocketError ) ), SLOT( slot_error( QAbstractSocket::SocketError ) ) );

		_startDialog = new StartGameDialog(this);

		connect( _startDialog, SIGNAL( sig_newScen() ), this, SLOT( slot_newScen() ) );
		connect( _startDialog, SIGNAL( sig_loadScen() ), this, SLOT( slot_loadScen() ) );
		connect( _startDialog, SIGNAL( sig_newCamp() ), this, SLOT( slot_newCamp() ) );
		connect( _startDialog, SIGNAL( sig_loadCamp() ), this, SLOT( slot_loadCamp() ) );
		connect( _startDialog, SIGNAL( sig_dialogClosed() ), this, SLOT( slot_dialogClosed() ) );
	} else {
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}
	
	loadMenu(false);

	//XXX here insert main menu
}

ClientInterface::~ClientInterface()
{
	TRACE("_socket %p, _game %p", _socket, _game );

	if( _socket ) {
		disconnectClient();
		endGame();
	}

	delete _game;

	if( _fight ) {
		delete _fight;
	}
	if( _base ) {
		delete _base;
	}
	if( _statusBar ) {
		delete _statusBar;
	}
	
	if( _progress ) {
		delete _progress;
	}

	delete _socket;
	//free before ImageTheme, after DataTheme (clear of ImageTheme depend on DataTheme data)
	ImageTheme.clear();
	DataTheme.clear();
}

void ClientInterface::reinit()
{
	TRACE("ClientInterface::reinit");

	_game->reinit();
	
	//static int count = 0;
	_winner = true;
	_state = SG_MAP;
	_game->setVisible( true );
	/* reinit resourceBar */
	_resourceBar->reinit();
	
	if( _fight ) {
		_fight->hide();
		delete _fight;
		_fight = NULL;
	}
	if( _base ) {
		_base->hide();
		delete _base;
		_base = NULL;
	}
	adjustWidgetLoading(  W_NORMAL );
}

void ClientInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch( e->type() ) {
		case QEvent::WindowTitleChange:
			setWindowTitle( tr( "Attal - Lords of Doom" ) );
		break;
		default:
			QWidget::changeEvent ( e );
		break;
	}
}

void ClientInterface::closeEvent( QCloseEvent * event )
{
	TRACE("ClientInterface::closeEvent( QCloseEvent * event %p)", event );
	if( actionQuit() ) {
		event->accept();
	} else {
		event->ignore();
	}
}

QAction * ClientInterface::addAction( const QString & label, const QKeySequence & shortcut, MENU_ACTIONS id, QSignalMapper * sigmap )
{
	QAction * action;

	if( !label.isEmpty() ) {
		action = new QAction( label, this );
	} else {
		action = new QAction( this );
	}
	if( shortcut ) {
		action->setShortcut( shortcut );
	}
	_actions.insert( id, action );
	sigmap->setMapping( action, id );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );

	return action;
}

void ClientInterface::initActions()
{
	_actions.resize( NB_ACTIONS );
	
	QAction * action;
	_sigmap = new QSignalMapper( this );

	/* Menu File */
	addAction( tr( "&Connect to server" ), QKeySequence( tr( "CTRL+C" ) ), ACTION_CONNECT, _sigmap );
	addAction( tr( "&Start game" ), QKeySequence( tr( "CTRL+N" ) ), ACTION_START, _sigmap );

	action = addAction( tr( "&End game" ), QKeySequence( tr( "CTRL+E" ) ), ACTION_END, _sigmap );
	action->setDisabled( true );

	action = addAction( tr( "&Save game" ), QKeySequence( tr( "CTRL+W" ) ), ACTION_SAVE, _sigmap );
	action->setDisabled( true );

	addAction( tr( "Fast &Server" ), QKeySequence( tr( "CTRL+S" ) ), ACTION_FASTCONNECT, _sigmap );

	action = addAction( tr( "&Disconnect" ), QKeySequence( tr( "CTRL+D" ) ), ACTION_DISCONNECT, _sigmap );
	action->setDisabled( true );

  /* Menu Game */
	addAction( tr( "&Mini map" ), QKeySequence( tr( "CTRL+M" ) ), ACTION_MINIMAP, _sigmap );
	addAction( tr( "&Fullscreen" ), QKeySequence( tr( "CTRL+F" ) ), ACTION_FULL, _sigmap );
	addAction( tr("&Popup message (base mode)"), QKeySequence( tr( "CTRL+T" ) ), ACTION_CHAT, _sigmap );
	addAction( tr( "&Options" ), QKeySequence( tr( "CTRL+O" ) ), ACTION_OPTIONS, _sigmap );

	action = addAction( tr( "&Scenario Informations" ), 0 , ACTION_INFO,_sigmap );

	/* Right panel */
	addAction( tr( "Next &Lord" ), QKeySequence( tr( "CTRL+L" ) ), ACTION_NEXTLORD, _sigmap );
	addAction( tr( "Next &Base" ), QKeySequence( tr( "CTRL+B" ) ), ACTION_NEXTBASE, _sigmap );
	addAction( tr( "&Help" ), QKeySequence( tr( "Key_F1" ) ), ACTION_HELP, _sigmap );
	addAction( tr( "&About" ), QKeySequence( tr( "CTRL+A" ) ), ACTION_ABOUT, _sigmap );
	addAction( tr( "&Quit" ), QKeySequence( tr( "ESC" ) ), ACTION_QUIT, _sigmap );

	/* Menu Screen */
	action = addAction( tr( "Free size" ), QKeySequence( "" ), ACTION_FREESIZE, _sigmap );
	action->setChecked( true );

	addAction( tr( "800x600" ), QKeySequence( "" ), ACTION_800, _sigmap );
	addAction( tr( "1024x768" ), QKeySequence( "" ), ACTION_1024, _sigmap );
	addAction( tr( "1280x1024" ), QKeySequence( "" ), ACTION_1280, _sigmap );
	/// XXX: we have to use QActionGroup for switch between size of screen

	addAction( tr( "Test Style" ), QKeySequence( "CTRL+T" ), ACTION_TESTSTYLE, _sigmap );

	connect( _sigmap, SIGNAL( mapped( int ) ), SLOT( slot_action( int ) ) );
}

void ClientInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	QMenu * menuGame = menuBar()->addMenu( tr( "&Game" ) );
	QMenu * menuScreen = menuBar()->addMenu( tr( "&Screen" ) );
	QMenu * menuHelp = menuBar()->addMenu( tr( "&Help" ) );

	menuFile->addAction( _actions[ ACTION_START ] );
	menuFile->addAction( _actions[ ACTION_END ] );
	menuFile->addAction( _actions[ ACTION_SAVE ] );
	menuFile->addAction( _actions[ ACTION_CONNECT ] );
	menuFile->addAction( _actions[ ACTION_FASTCONNECT ] );
	menuFile->addAction( _actions[ ACTION_DISCONNECT ] );
	menuFile->addAction( _actions[ ACTION_QUIT ] );

	menuGame->addAction( _actions[ ACTION_MINIMAP ] );
	menuGame->addAction( _actions[ ACTION_FULL ] );
	menuGame->addAction( _actions[ ACTION_CHAT ] );
	//_centralWindow->addAction( _actions[ ACTION_CHAT ] );
	menuGame->addAction( _actions[ ACTION_OPTIONS ] );
	menuGame->addAction( _actions[ ACTION_INFO ] );
	
	menuScreen->addAction( _actions[ ACTION_FREESIZE ] );
	menuScreen->addAction( _actions[ ACTION_800 ] );
	menuScreen->addAction( _actions[ ACTION_1024 ] );
	menuScreen->addAction( _actions[ ACTION_1280 ] );
#ifdef TEST
	menuScreen->addAction( _actions[ ACTION_TESTSTYLE ] );
#endif
	
	menuHelp->addAction( _actions[ ACTION_HELP ] );
	menuHelp->addAction( _actions[ ACTION_ABOUT ] );

}

void ClientInterface::slot_action( int num )
{
	switch( num ) {
	case ACTION_START:
		actionStart();
		break;
	case ACTION_END:
		actionEnd();
		break;
	case ACTION_SAVE:
		save();
		break;
	case ACTION_CONNECT:
		actionConnect(false);
		break;
	case ACTION_FASTCONNECT:
		actionFastConnect();
		break;
	case ACTION_INFO:
		actionInfo();
		break;
	case ACTION_DISCONNECT:
		actionDisconnect();
		break;
	case ACTION_MINIMAP:
		actionMinimap();
		break;
	case ACTION_FULL:
		actionFullScreen();
		break;
	case ACTION_CHAT:
		actionPopupChat();
		break;
	case ACTION_OPTIONS:
		actionOptions();
		break;
	case ACTION_NEXTLORD:
		actionNextLord();
		break;
	case ACTION_NEXTBASE:
		actionNextBase();
		break;
	case ACTION_HELP:
		actionHelp();
		break;
	case ACTION_ABOUT:
		actionAbout();
		break;
	case ACTION_QUIT:
		actionQuit();
		break;
	case ACTION_FREESIZE:
		actionResize( SIZEMODE_FREE );
		break;
	case ACTION_800:
		actionResize( SIZEMODE_800 );
		break;
	case ACTION_1024:
		actionResize( SIZEMODE_1024 );
		break;
	case ACTION_1280:
		actionResize( SIZEMODE_1280 );
		break;
	case ACTION_TESTSTYLE:
		actionTestStyle();
		break;
	}
}

void ClientInterface::slot_result( bool result )
{
	TRACE("ClientInterface::slot_result( bool result %d)",result);
	_winner = result;
}

void ClientInterface::slot_endConnection( QString /*name*/ )
{
	TRACE("ClientInterface::slot_endConnection");
}

void ClientInterface::slot_endGame()
{
	TRACE("ClientInterface::slot_endGame");
	
	handleCheckEndGame();
}

void ClientInterface::slot_dialogClosed()
{

	TRACE("ClientInterface::slot_dialogClosed");

	disconnectClient();
	endGame();
}

void ClientInterface::actionFastConnect()
{

	TRACE("ClientInterface::actionFastConnect");

	_proc = new QProcess(this);
	if( ! _proc ) {
		actionQuit();
	}
	QStringList arglist;

	arglist.append( "--fast" );

#ifdef WIN32
	_proc->start( "attal-server.exe", arglist );
#else
	QFile file("./attal-server");

	if (file.exists()) {
		_proc->start( "./attal-server", arglist );
	} else {
		_proc->start( "attal-server", arglist );
	}
#endif
	while( !_proc->waitForStarted()) {}
	QTimer::singleShot( 2000, this, SLOT( slot_clientConnect() ) );
}

void ClientInterface::slot_clientConnect()
{
	actionConnect( true );
}

void ClientInterface::actionConnect(bool fast)
{
	if( _socket->state() == QAbstractSocket::UnconnectedState  ) {
		ConfigConnection * config = new ConfigConnection( this );
		config->setHost( "localhost" );
		config->setPort( PORT.toInt() );
		config->setPlayerName( _game->getGamePlayer()->getConnectionName() );
		if(fast){
			config->accept();
		} else { 
			config->exec();
		}
		if( config->result() == QDialog::Accepted ) {
			_game->setPlayerName( config->getPlayerName() );
			adjustWidgetLoading(  W_CLIENT );
			_socket->connectToHost( config->getHost(), config->getPort() );
		}
		delete config;
	} else {
		QMessageBox::critical( this, tr( "Can't connect" ),
			tr( "You're already connected to a server. Please disconnect first." ) );
	}
}

void ClientInterface::slot_hostfound()
{
	if( !_server ) {
		emit sig_newMessage(QString (tr("Host found , connecting... ")));
	}
}

void ClientInterface::slot_error( QAbstractSocket::SocketError error )
{
	if( !_server ) {
		logEE("Cannot connect to server");
		switch( error ) {
			case QAbstractSocket::ConnectionRefusedError: 
				{
					logEE( "Connection Refused" );
					emit sig_newMessage( QString( tr("Connection Refused") ) );
					if( _server ) {
						return;
					}
				}
				break;
			case QAbstractSocket::HostNotFoundError:
				logEE( "Host not found" );
				emit sig_newMessage( QString( "Host not found" ) );
				break;
			case QAbstractSocket::RemoteHostClosedError:  {
																											logEE("The remote host closed the connection.");	
																										}
																										break;
			case QAbstractSocket::SocketAccessError:   logEE("The socket operation failed because the application lacked the required privileges.");	break;
			case QAbstractSocket::SocketResourceError:   logEE("The local system ran out of resources (e.g., too many sockets).");	break;
			case QAbstractSocket::SocketTimeoutError:    logEE("The socket operation timed out.");	break;
			case QAbstractSocket::DatagramTooLargeError:    logEE("The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");	break;
			case QAbstractSocket::NetworkError:    logEE("An error occurred with the network (e.g., the network cable was accidentally plugged out).");	break;
			case QAbstractSocket::AddressInUseError:    logEE("The address specified to QUdpSocket::bind() is already in use and was set to be exclusive.");	break;
			case QAbstractSocket::SocketAddressNotAvailableError:    logEE("The address specified to QUdpSocket::bind() does not belong to the host.");	break;
			case QAbstractSocket::UnsupportedSocketOperationError:   logEE("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");	break;
			default: {
								 logEE( "Other socket error occured" );
								 emit sig_newMessage( QString( "Other socket error occured" ) );
								 return;
							 }
																															 break;
		}
	}

	disconnectClient();
	endGame();
}

void ClientInterface::slot_connectionClosed()
{
	TRACE( "Server disconnect" );
	if( !_server  ) {
		emit sig_newMessage(QString ("Server Disconnect "));
	}
	killServer();
}

void ClientInterface::killServer()
{

	TRACE( "_proc %p", _proc );

	if(_proc) {
		_proc->close();
		//QTimer::singleShot( 5000, _proc, SLOT( kill() ) );
		_proc->terminate();
		_proc->waitForFinished();
		_proc = NULL;
	}
}

void ClientInterface::actionStart()
{
	TRACE("ClientInterface::actionStart");

	_server = new AttalServer( PORT.toInt(), QHostAddress::LocalHost );
	
	if( _server && _server->isListening() ) {

		connect( _server, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), this, SLOT( slot_ready() ) );
		connect( _server, SIGNAL( sig_endConnection( QString ) ), this , SLOT( slot_endConnection( QString ) ) );

		newEngine();

	} else {
		endServer();
	}

	actionConnect( true );
		
	_startDialog->setVisible( true );

}

void ClientInterface::actionEnd()
{
	TRACE("ClientInterface::actionEnd");
	
	disconnectClient();
	endGame();
}

void ClientInterface::slot_newScen()
{
	DisplayScenariiDialog scen( tr( "Choose new scenario" ) );
	if( scen.exec() ) {
		_startDialog->hide();
		loadSingle( scen.getFileName() );
	}
}

void ClientInterface::slot_newCamp()
{
	QString filename;

	DisplayCampaignDialog camp( tr( "Choose new campaign" ) );
	if( camp.exec() ) {
		_startDialog->hide();
		loadCampaign( camp.getFileName() );
	}
}

void ClientInterface::slot_loadScen()
{
	QString filename;
	filename = QFileDialog::getOpenFileName( this, tr( "Load game" ), SAVE_PATH, "*.scn *.gam" );
	if ( !filename.isEmpty() ) {
		_startDialog->hide();
		loadSingle( filename );
	}
}

void ClientInterface::slot_loadCamp()
{
	QString filename;
	filename = QFileDialog::getOpenFileName( this, tr( "Load campaign" ), SAVE_PATH, "*.cms" );
	if ( !filename.isEmpty() ) {
		_startDialog->hide();
		loadCampaign( filename );
	}
}

void ClientInterface::slot_endTurn()
{
}

void ClientInterface::slot_beginTurn()
{
	autosave();
}

void ClientInterface::slot_ready()
{

	TRACE("ClientInterface::slot_ready");

	_readyIn =  true;
}

void ClientInterface::addInternalAI()
{
	TRACE("ClientInterface::addInternalAI");

	Analyst * ai = new Analyst();
	ai->start();
	ai->connectAi( "localhost", _server->serverPort() );
	_aiList.append( ai );
}

bool ClientInterface::killAI()
{
	int count;
	Analyst * ai;
		
	count = _aiList.count();
	TRACE("bool ClientInterface::killAI count %d",count);
	
	if( _aiList.count() > 0 ) {
		for( int i = 0; i < count; i++ ) {
				ai = _aiList.takeFirst();
				ai->disconnectAi();
				ai->exit(0);
				ai->wait();
				delete ai;
		}
		return true;	
	}

	return false;
}
				
void ClientInterface::actionDisconnect()
{
	TRACE("ClientInterface::actionDisconnect");

	QMessageBox msb( tr( "Match" ), tr( "Do you want abandon the match ?" ), QMessageBox::Warning, QMessageBox::Yes | 			QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
	if( msb.exec() != QMessageBox::Yes){
		return;
	}

	disconnect();
}

void ClientInterface::disconnectAI()
{	

	TRACE("ClientInterface::disconnectAI");

	Analyst * ai;

	int count = _aiList.count();
	if( _aiList.count() > 0 ) {
		for( int i = 0; i < count; i++ ) {
				TRACE("ClientInterface::disconnectAI i %d", i );
				ai = _aiList.at(i);
				ai->disconnectAi();
		}
	}

}

void ClientInterface::disconnectClient()
{
	TRACE("ClientInterface::disconnectClient");
	
	if( !_server ) {
		emit sig_newMessage(QString ("Disconnect from server"));
	}

	updateScreen();
	
	_socket->abort();
	
}

void ClientInterface::actionMinimap()
{
	if( _game ) {
		_mini = ! _mini;
		_game->displayMiniMap( _mini );
		_actions[ ACTION_MINIMAP ]->setChecked( _mini );
		_actions[ ACTION_FULL ]->setEnabled( _mini );
	}
}

void ClientInterface::actionPopupChat()
{
	
	if( _base ) {
		ChatDialog chatDialog;
		connect( &chatDialog, SIGNAL(sig_message( QString )), _base, SLOT(slot_message( QString )) );
		chatDialog.exec();
	}

}

void ClientInterface::actionFullScreen()
{
	if( _game ) {
		_full = ! _full;
		_game->displayFullScreen( _full );
		_actions[ ACTION_FULL ]->setChecked( _full );
		_actions[ ACTION_MINIMAP ]->setEnabled( _full );
	}
}

void ClientInterface::actionNextLord()
{
	if( _game ) {
		_game->nextLord();
	}
}

void ClientInterface::actionNextBase()
{
	if( _game ) {
		_game->nextBase();
	}
}

void ClientInterface::actionInfo()
{
	emit sig_gameInfo();
}

void ClientInterface::actionHelp()
{
	DisplayHelp help;
	help.resize( 800, 600 );
	help.exec();
}

void ClientInterface::actionAbout()
{
	AboutDialog dialog;
	dialog.exec();
}


bool ClientInterface::actionQuit()
{
	TRACE("ClientInterface::actionQuit");

	bool ret = true;
	QMessageBox msb( tr("Are you sure ?"), tr("Do you really want to quit?"), QMessageBox::Warning, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
	if ( msb.exec() == QMessageBox::Yes ) {
		AttalSettings::getInstance()->save();
		
		qApp->quit();
	} else {
		ret = false;
	}	

	return ret;
}

void ClientInterface::actionResize( SizeMode mode )
{
	_actions[ ACTION_FREESIZE ]->setChecked( false );
	_actions[ ACTION_800 ]->setChecked( false );
	_actions[ ACTION_1024 ]->setChecked( false );
	_actions[ ACTION_1280 ]->setChecked( false );
	
	switch( mode ) {
	case SIZEMODE_FREE:
		_actions[ ACTION_FREESIZE ]->setChecked( true );
		setMinimumSize( 0, 0 );
		setMaximumSize( 10000, 10000 );
		AttalSettings::getInstance()->setDispositionMode( AttalSettings::DM_FULL );
		break;
	case SIZEMODE_800:
		_actions[ ACTION_800 ]->setChecked( true );
		setFixedSize( 800, 560 );
		AttalSettings::getInstance()->setDispositionMode( AttalSettings::DM_VERYCOMPACT );
		break;
	case SIZEMODE_1024:
		_actions[ ACTION_1024 ]->setChecked( true );
		setFixedSize( 1024, 728 );
		AttalSettings::getInstance()->setDispositionMode( AttalSettings::DM_COMPACT );
		break;
	case SIZEMODE_1280:
		_actions[ ACTION_1280 ]->setChecked( true );
		setFixedSize( 1280, 964);
		AttalSettings::getInstance()->setDispositionMode( AttalSettings::DM_FULL );
		break;
	}
	emit sig_dispositions();
}

void ClientInterface::actionTestStyle()
{
#ifdef TEST
	TestAttalStyle test;
	test.exec();
#endif
}

void ClientInterface::slot_status( const QString & text )
{
	_statusBar->showMessage( text, 1000 );
}

void ClientInterface::initStatusBar()
{
	_statusBar = statusBar();

	_resourceBar = new RessourceBar( this );
	_resourceBar->setPlayer( _game->getGamePlayer() );
	_cal = new QLabel( this );
	_statusBar->addWidget( _resourceBar, 1 );
	_statusBar->addWidget( _cal );
	//_statusBar->showMessage( tr( "Status Bar" ), 0 );
}

void ClientInterface::loadMenu( bool show )
{
	//XXX : don't display text, but a good start
	if( !_msg ) {
		_msg = new AttalMessage(this, false);
		_msg->setWindowTitle( tr("Loading Data...") );
		_msg->addText( tr("Loading Data...") );
		_msg->resize( 300, 70 );
		_msg->setModal(false);
	}
	if( show ) {
		_msg->setVisible( true );
	} else { 
		_msg->setVisible( false );
	}
}

void ClientInterface::initProgressBar()
{
	if( ! _progress ) {
		_progress = new QProgressDialog( tr( "Loading data..." ), tr( "Cancel" ), 0, ImageTheme.getMaxLoadStep(), this );
		_progress->setLabelText( tr( "Loading data..." ) );
	}
	//_progress->setWindowModality(Qt::WindowModal);
}

void ClientInterface::slot_loadThemeStep()
{
	if( _progress ) {
		_progress->setValue( ++_progressNum );
		qApp->processEvents();
	}
}

void ClientInterface::slot_base( GenericBase * base )
{

	TRACE("ClientInterface::slot_base( GenericBase * %p	)",base);

	_state = SG_BASE;
	
	if( !_base ) {
		TRACE("ClientInterface::slot_base( new base )");
		_base = new DisplayBase( _centralWindow, base, _socket );
		_numWidg[1] = _centralWindow->addWidget( _base );
		connect( _base, SIGNAL( sig_quit() ), SLOT( slot_map() ) );
		connect( this, SIGNAL( sig_baseDisplay() ), _base , SLOT( show() ) );
		connect( this, SIGNAL( sig_dispositions() ), _base , SLOT( updateDispositionMode() ) );
	} else if( _base->getBase() != base )  {
		TRACE("ClientInterface::slot_base( reinit base )");
		_base->reinitBase( base );
	} else { 
		TRACE("ClientInterface::slot_base( same base )");
		//reinit base ( right panel )
		_base->reinit();
	}

	
	emit sig_widgChanged( _numWidg[1] );
	emit sig_baseDisplay();

	ImageTheme.playMusicBase( base->getRace() );
}

void ClientInterface::slot_map()
{
	TRACE("void ClientInterface::slot_map()");

	updateScreen();
	ImageTheme.playMusicMap();
}

void ClientInterface::updateScreen()
{
	TRACE("void ClientInterface::updateScreen()");
	
	_state = SG_MAP;
	emit sig_map();
	emit sig_widgChanged( _numWidg[0] );
}

void ClientInterface::slot_fight( GenericLord * lord, CLASS_FIGHTER cla )
{
	TRACE("void ClientInterface::slot_fight( GenericBase * base )");

	if ( !_fight ) {
		_fight = new Fight( _centralWindow, _game, _socket );
		_numWidg[2] = _centralWindow->addWidget( _fight );
		connect( _fight, SIGNAL( sig_quit() ), SLOT( slot_map() ) );
		connect( _fight, SIGNAL( sig_statusMsg( const QString & ) ), SLOT( slot_status( const QString & ) ) );
	} else {
		_fight->reinit();
	}
	_state = SG_FIGHT;
	_fight->setLord( lord, cla );

	emit sig_fightDisplay();
	emit sig_widgChanged( _numWidg[2] );
	ImageTheme.playMusicFight();
}


void ClientInterface::slot_readSocket()
{
	_socket->readData();
	//TRACE( "Socket received %d | %d | %d", _socket->getCla1(), _socket->getCla2(), _socket->getCla3() );
	switch( _state ) {
	case SG_MAP:
	case SG_BASE:
		_game->handleSocket();
		break;
	case SG_FIGHT:
		_fight->handleSocket();
		break;
	default:
		logEE("slot_readSocket, state error %d", _state );
		break;
	}

	if( _socket->bytesAvailable() > 0 ) {
		 slot_readSocket();
	}
}

void ClientInterface::slot_resourceBar()
{
	_resourceBar->reinit();
	_cal->setText( _game->getCalendar()->getDate());
}

void ClientInterface::actionOptions()
{
	OptionsDialog * options = new OptionsDialog( this );
	
	connect( options, SIGNAL( sig_animation( int ) ), this , SLOT( actionOptionsAnimations( int ) )	);

	connect( options, SIGNAL( sig_dispositions() ), this, SIGNAL( sig_dispositions() )	);
	
	options->exec();
	
	delete options;
}

void ClientInterface::actionOptionsAnimations( int state )
{

	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();

  switch( state ) {
		case Qt::Checked:
			{
				settings.isAnimationEnabled = true;
			}
			break;
		case Qt::Unchecked:
			{
				settings.isAnimationEnabled = false;
			}
			break;
		default:
			break;
	}
	AttalSettings::getInstance()->setStrategyModeSettings( settings );
	emit sig_animations();
}

void ClientInterface::adjustWidgetLoading(  StatusWidget type )
{
	TRACE("ClientInterface::adjustWidgetLoading(  StatusWidget type  %d)", type);

	switch( type ) {
		case W_ENGINE:
			_actions[ ACTION_START ]->setEnabled( false );
			_actions[ ACTION_END ]->setEnabled( true );
			_actions[ ACTION_SAVE ]->setEnabled( true );
			_actions[ ACTION_CONNECT ]->setEnabled( false );
			_actions[ ACTION_DISCONNECT ]->setEnabled( false );
			_actions[ ACTION_FASTCONNECT ]->setEnabled(false);
			break;
		case W_NORMAL:
			_actions[ ACTION_START ]->setEnabled( true );
			_actions[ ACTION_END ]->setEnabled( false );
			_actions[ ACTION_SAVE ]->setEnabled( false );
			_actions[ ACTION_CONNECT ]->setEnabled( true );
			_actions[ ACTION_DISCONNECT ]->setEnabled( false );
			_actions[ ACTION_FASTCONNECT ]->setEnabled( true );
			break;
		case W_CLIENT:
			_actions[ ACTION_START ]->setEnabled( false );
			_actions[ ACTION_END ]->setEnabled( false );
			_actions[ ACTION_SAVE ]->setEnabled( false );
			_actions[ ACTION_CONNECT ]->setEnabled( false );
			_actions[ ACTION_DISCONNECT ]->setEnabled( true );
			_actions[ ACTION_FASTCONNECT ]->setEnabled(false);
			break;
		default:
			break;
	}
}

void ClientInterface::resizeWidget()
{
	switch( QApplication::desktop()->width() ) {
		case 1280:
			actionResize(SIZEMODE_1280);
			break;
		case 1024:
			actionResize(SIZEMODE_1024);
			break;
		case 800:
			actionResize(SIZEMODE_800);
			break;
		default:
			actionResize( SIZEMODE_FREE	);
			break;
	}
}
