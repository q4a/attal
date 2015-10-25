/****************************************************************
**
** Attal : Lords of Doom
**
** clientInterface.cpp
** Manages the whole game
**
** Version : $Id: clientInterface.cpp,v 1.4 2003/02/13 20:09:13 lusum Exp $
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


#include "clientInterface.h"

// include files for QT
#include <qcanvas.h>
#include <qapplication.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
// application specific includes
#include "conf.h"
#include "common/attalSocket.h"
#include "common/log.h"
#include "common/skill.h"
#include "common/unit.h"
#include "common/dataTheme.h"
#include "client/attalStyle.h"
#include "client/building.h"
#include "client/gainLevel.h"
#include "client/game.h"
#include "client/fightResult.h"
#include "client/attalStyle.h"
#include "client/displayLord.h"
#include "client/displayBase.h"
#include "client/fight.h"
#include "client/gui.h"
#include "client/imageTheme.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;


ClientInterface::ClientInterface()
{
	_socket = 0;
	_fight = 0;
	_level = 0;
	_base = 0;
	_config = 0;
	_state = SG_MAP;
	_mini = true;

	qApp->setStyle( new AttalStyle( DATA_PATH + "style.dat" ) );

	setCaption( "Attal - Lords of Doom" );
	initMenuBar();
	initStatusBar();
	DataTheme.init();
	ImageTheme.init();

	_game = new Game( this );
	setCentralWidget( _game );

	connect( _game, SIGNAL( sig_base( GenericBase * ) ), SLOT( slot_base( GenericBase * ) ) );
	connect( _game, SIGNAL( sig_fight( GenericLord *, CLASS_FIGHTER ) ), SLOT( slot_fight( GenericLord *, CLASS_FIGHTER ) ) );
}

ClientInterface::~ClientInterface()
{
	if( _socket ) {
		delete _socket;
	}
}

void ClientInterface::closeEvent( QCloseEvent* event )
{
	quitting();
	event->accept();
}

void ClientInterface::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "Connect to server", FIL_CON );
	menuFile->insertItem( "Disconnect", FIL_DIS );
	menuFile->insertItem( "Quit", FIL_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );

	_menuGame = new QPopupMenu();
	CHECK_PTR( _menuGame );
	_menuGame->insertItem( "Mini map", GAM_MINIMAP );
	_menuGame->setItemChecked( GAM_MINIMAP, true );
	connect( _menuGame, SIGNAL( activated( int ) ), this, SLOT( slot_menuGameActivated( int ) ) );

	menuBar()->insertItem( "&File", menuFile );
	menuBar()->insertItem( "&Game", _menuGame );
}

void ClientInterface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FIL_CON:
		if( !_socket ) {
			if( _config == 0 ) {
				_config = new ConfigConnection( this );
			}
			_config->setHost( "localhost" );
			_config->setPort( ATTAL_PORT );
			_config->setPlayerName( _game->getPlayer()->getName() );
			if( _config->exec() ) {
				_socket = new AttalSocket();
				_game->setSocket( _socket );
				_game->setPlayerName( _config->getPlayerName() );
				_socket->connectToHost( _config->getHost(), _config->getPort() );
				connect( _socket, SIGNAL( readyRead() ), SLOT( slot_readSocket() ) );
			}
		} else {
			QMessageBox::critical( this, "Can't connect", "You're already connected to a server. Please disconnect first." );
		}
		break;
	case FIL_DIS: {
			QMessageBox msb( "Match","Do you want abandon the match?", QMessageBox::Warning, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
			if ( msb.exec() == QMessageBox::Yes ) {
				_game->endGame();
				if( _socket ) {
					_game->setSocket( 0 );
					_socket->close();
					delete _socket;
					_socket = 0;
				}
			}
		}
		break;
	case FIL_QUIT:
		quitting();
		break;
	}
}

void ClientInterface::quitting()
{
	_game->endGame();
	if( _socket ) {
		_game->setSocket( 0 );
		_socket->close();
		delete _socket;
		_socket = 0;
	}
	qApp->quit();
}

void ClientInterface::slot_menuGameActivated( int num )
{
	switch( num ) {
	case GAM_MINIMAP:
		if( _game ) {
			_mini = ! _mini;
			_game->displayMiniMap( _mini );
			_menuGame->setItemChecked( GAM_MINIMAP, _mini );
		}
		break;
	default:
		break;
	}
}

void ClientInterface::slot_status( QString text )
{
	statusBar()->message( text, 0 );
}

void ClientInterface::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}

void ClientInterface::slot_base( GenericBase * base )
{
	_state = SG_BASE;
	if( !_base ) {
		_base = new DisplayBase( this );
		_base->setPlayer( _game->getPlayer() );
		_base->setSocket( _socket );
		_base->setGame( _game );
		_base->show();
		connect( _base, SIGNAL( sig_quit() ), SLOT( slot_map() ) );
	} else {
		_base->show();
	}
	_base->setBase( base );
	setCentralWidget( _base );
	_game->hide();
}

void ClientInterface::slot_map()
{
	switch( _state ) {
	case SG_FIGHT:
		_fight->hide();
		break;
	case SG_BASE:
		_base->hide();
		break;
	}
	_state = SG_MAP;
	setCentralWidget( _game );
	_game->show();
}

void ClientInterface::slot_fight( GenericLord * lord, CLASS_FIGHTER cla )
{
	if ( _fight == 0 ) {
		_fight = new Fight( this );
		_fight->setGame( _game );
		_fight->show();
		connect( _fight, SIGNAL( sig_quit() ), SLOT( slot_map() ) );
	} else {
		_fight->reinit();
		_fight->show();
	}
	_state = SG_FIGHT;
	setCentralWidget( _fight );
	_game->hide();
	_fight->setSocket( _socket );
	_fight->setLord( lord, cla );
}


void ClientInterface::slot_readSocket()
{
	_socket->readData();
	switch( _state ) {
	case SG_MAP:
		_game->handleSocket();
		break;
	case SG_FIGHT:
		_fight->handleSocket();
		break;
	case SG_BASE:
		_base->handleSocket();
		break;
	}

	if( _socket->bytesAvailable() > 0 ) {
		slot_readSocket();
	}
}



//
// ----- ConfigConnection -----
//

ConfigConnection::ConfigConnection( QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	setCaption( "Connection to server" );
	QVBoxLayout * layout = new QVBoxLayout( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	QLabel * labHost = new QLabel( "Host : ", this );
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
	QLabel * labPort = new QLabel( "Port : ", this );
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
	QLabel * labName = new QLabel( "Name : ", this );
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
	pbOk->setText( "Connect" );
	FIXEDSIZE( pbOk );
	layH3->addStretch( 1 );
	layH3->addWidget( pbOk );
	layH3->addStretch( 1 );
	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( "Cancel" );
	FIXEDSIZE( pbCan );
	layH3->addWidget( pbCan );
	layH3->addStretch( 1 );
	layout->addLayout( layH3, 2 );

	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );

	setFixedSize( 250, 150 );
}
