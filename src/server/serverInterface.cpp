/****************************************************************
**
** Attal : Lords of Doom
**
** serverInterface.cpp
** interface for the server
**
** Version : $Id: serverInterface.cpp,v 1.3 2003/02/10 21:52:06 audoux Exp $
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


#include "serverInterface.h"

#include <stdlib.h>
// include files for QT
#include <qapplication.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qvbuttongroup.h>
#include <qhbuttongroup.h>
#include <qlistview.h>
// application specific includes
#include "conf.h"
#include "common/log.h"
#include "common/dataTheme.h"
#include "client/gui.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;
extern QString SCENAR_PATH;

#define CLIENT

/** add comments here */
ServerInterface::ServerInterface()
	:QMainWindow()
{
	setCaption( "Attal - Lords of Doom (Server)" );
	initMenuBar();
	initStatusBar();
	DataTheme.init();

	_widget = new ServerWidget( this );
	setCentralWidget( _widget );

	connect( _widget, SIGNAL( sig_stop() ), SLOT( slot_stop() ) );
	connect( _widget, SIGNAL( sig_load( QString ) ), SLOT( slot_load( QString ) ) );
	connect( _widget, SIGNAL( sig_save() ), SLOT( slot_save() ) );

	setMinimumSize( 350, 200 );

	if( !init() ) {
		logDD( "quit" );
		qApp->quit();
	}
	connect( _server, SIGNAL( sig_endConnection( QString ) ), _widget, SLOT( slot_endConnection( QString ) ) );
}

bool ServerInterface::init()
{
	bool ret = false;
	_server = new AttalServer( ATTAL_PORT );
	if( _server->ok() ) {
		ret = true;
		_engine = new Engine( _server );
		_server->setEngine( _engine );
		connect( _engine, SIGNAL( sig_newPlayer( AttalPlayerSocket * ) ), _widget, SLOT( slot_newPlayer( AttalPlayerSocket * ) ) );
	} else {
		delete _server;
		_server = 0;
		if( QMessageBox::warning( this,
				"Server error",
				"Could not listen for sockets.",
				"Try again",
				"Quit",
				0, 0, 1 )  == 0 ) {
			ret = init();
		} else {
			exit( 0 );
		}
	}

	return ret;
}

void ServerInterface::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}

/*!
  define menus
*/

void ServerInterface::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "Load Game", FIL_LOAD );
	menuFile->insertItem( "Save Game", FIL_SAVE );
	menuFile->insertItem( "End Game", FIL_END );
	menuFile->insertItem( "Quit", FIL_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );

	menuBar()->insertItem( "&File", menuFile );
}

void ServerInterface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FIL_LOAD:{
		QString filename;
		filename = QFileDialog::getOpenFileName( "", "*.scn", this );
		slot_load( filename );
		break;
	}
	case FIL_SAVE:
		slot_save();
		break;
	case FIL_END:
		slot_stop();
		break;
	case FIL_QUIT:
		qApp->quit();
	}
}

void ServerInterface::slot_status( QString text )
{
	statusBar()->message( text, 0 );
}

void ServerInterface::slot_stop()
{
	_engine->endGame();
	_widget->setGameLoaded( false );
}

void ServerInterface::slot_load( QString filename )
{
	if( ( _server->getNbSocket() > 0 ) && ( !filename.isNull() ) ) {
		if( _engine->loadGame( filename ) ) {
			_engine->startGame();
			_widget->setGameLoaded( true );
		}
	}
}

void ServerInterface::slot_save()
{
	QString filename;
	filename = QFileDialog::getSaveFileName( "", "*.gam", this );
	if (!filename.isNull()) {
		_engine->saveGame( filename );
	}
}

//
// ----- ChooseFileRadioButton -----
//

ChooseFileRadioButton::ChooseFileRadioButton( QWidget * parent, const char * name )
	:QRadioButton( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addSpacing( 20 );
	
	_edit = new QLineEdit( this );
	layout->addWidget( _edit, 1 );
	layout->addSpacing( 10 );
	
	_choose = new QPushButton( this );
	_choose->setText( "Choose" );
	FIXEDSIZE( _choose );
	layout->addWidget( _choose );
	
	layout->activate();
	
	connect( _choose, SIGNAL( clicked() ), SLOT( slot_choose() ) );
	connect( this, SIGNAL( toggled( bool ) ), SLOT( slot_toggle( bool ) ) );
	setFixedHeight( _choose->size().height() + 4 );
}

void ChooseFileRadioButton::slot_choose()
{
	QString filename = QFileDialog::getOpenFileName( "", "*.scn", this ); 
	if( ! filename.isNull() ) {
		_edit->setText( filename );
	}
}

void ChooseFileRadioButton::slot_toggle( bool st )
{
	_choose->setEnabled( st );
	_edit->setEnabled( st );
}

//
// ----- ServerWidget -----
//

ServerWidget::ServerWidget( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_group = new QVButtonGroup( this );

	QRadioButton * radio1 = new QRadioButton( _group );
	radio1->setText( "Demo 1 player" );
	radio1->setChecked( true );

	QRadioButton * radio2 = new QRadioButton( _group );
	radio2->setText( "Demo 2 players" );

	_radio3 = new ChooseFileRadioButton( _group );
	_radio3->slot_toggle( false );

	_playerList = new QListView( this );
	_playerList->addColumn( "Name" );
	_playerList->addColumn( "Address" );
	_playerList->setMinimumHeight( 50 );

	_groupBottom = new QHButtonGroup( this );

	QPushButton * butStart = new QPushButton( _groupBottom );
	butStart->setText( "Start" );
	FIXEDSIZE( butStart );

	QPushButton * butStop = new QPushButton( _groupBottom );
	butStop->setText( "Save" );
	FIXEDSIZE( butStop );

	QPushButton * butSave = new QPushButton( _groupBottom );
	butSave->setText( "End" );
	FIXEDSIZE( butSave );

	_groupBottom->find( 0 )->setEnabled( true );
	_groupBottom->find( 1 )->setEnabled( false );
	_groupBottom->find( 2 )->setEnabled( false );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _group );
	layout->addSpacing( 5 );
	layout->addWidget( _playerList, 1 );
	layout->addSpacing( 5 );
	layout->addWidget( _groupBottom );
	layout->activate();

	connect( _groupBottom, SIGNAL( clicked( int ) ), SLOT( slot_choice( int ) ) );
}

void ServerWidget::slot_newPlayer( AttalPlayerSocket * player )
{
	_playerList->insertItem( new QListViewItem( _playerList, player->getPlayer()->getName(), player->address().toString() )  );
}

void ServerWidget::slot_endConnection( QString name )
{
	bool found = false;
	QListViewItem * item = _playerList->firstChild();

	do {
		if( item ) {
			if( item->text( 0 ) == name ) {
				found = true;
				_playerList->takeItem( item );
			} else {
				item = item->nextSibling();
			}
		} else {
			found = true;
		}
	} while( !found );
}

void ServerWidget::setGameLoaded( bool b )
{
	_loaded = b;
	if( _loaded ) {
		_groupBottom->find( 0 )->setEnabled( false );
		_groupBottom->find( 1 )->setEnabled( true );
		_groupBottom->find( 2 )->setEnabled( true );
		_group->setEnabled( false );
	} else {
		_groupBottom->find( 0 )->setEnabled( true );
		_groupBottom->find( 1 )->setEnabled( false );
		_groupBottom->find( 2 )->setEnabled( false );
		_group->setEnabled( true );
	}
}

void ServerWidget::slot_choice( int choice )
{
	switch( choice ) {
	case 0: {
		QString filename;
		switch( _group->id( _group->selected() ) ) {
		case 0:
			filename = SCENAR_PATH + "demo_1player.scn";
			break;
		case 1:
			filename = SCENAR_PATH + "demo_2players.scn";
			break;
		case 2:
			filename = _radio3->getText();
			break;
		}
		emit sig_load( filename );

		break;
	}
	case 1:
		emit sig_save();
		break;
	case 2:
		emit sig_stop();
		break;
	}
}

