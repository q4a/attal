/****************************************************************
**
** Attal : Lords of Doom
**
** interfaceDuel.cpp
** interface for the server of The Duel
**
** Version : $Id: interfaceDuel.cpp,v 1.4 2005/12/28 23:01:57 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/12/2000
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


#include "interfaceDuel.h"
 
// include files for QT
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QPopupMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineedit>
// application specific includes
#include "conf.h"
#include "common/log.h"


extern QString DATA_PATH;
extern QString PORT;

/** add comments here */
InterfaceDuel::InterfaceDuel()
{
	_server = new AttalServer( PORT.toInt() );	
	setWindowTitle("Attal - Lords of Doom (Server)");
	initMenuBar();
	initStatusBar();

	_widget = new ServerWidget( this );
	setCentralWidget( _widget );
}

void InterfaceDuel::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}
	
/*!
  define menus
*/

void InterfaceDuel::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "Load Game", FIL_LOAD );
	menuFile->insertItem( "Save Game", FIL_SAVE );
	menuFile->insertItem( "Start Game", FIL_START );
	menuFile->insertItem( "End Game", FIL_END );	
	menuFile->insertItem( "Quit", FIL_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );	
	
	QPopupMenu * menuGame = new QPopupMenu();
	CHECK_PTR( menuGame );
	menuGame->insertItem( "Fight", GAM_FIG );
	menuGame->insertItem( "End Fight", GAM_END );
	connect( menuGame, SIGNAL( activated( int ) ), this, SLOT( slot_menuGameActivated( int ) ) );
	
	menuBar()->insertItem( "&File", menuFile );
	menuBar()->insertItem( "&Game", menuGame );
}

void InterfaceDuel::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FIL_LOAD:{
		QString filename;
		filename = QFileDialog::getOpenFileName( "", "*.map", this );
		if (!filename.isNull()) {

		}
		break;
	}
	case FIL_SAVE:{
		QString filename;
		filename = QFileDialog::getSaveFileName( "", "*.map", this );
		if (!filename.isNull()) {

		}
		break;
	}
	case FIL_START:
		if( (uint) _widget->getNbPlayer() == _server->getNbSocket() ) {

		} else {
			QMessageBox::critical( this, "Unable to start game", "There is not enough players connected.", 0, 1 );
		}
		break;
	case FIL_END:
		_server->endGame();
		break;
	case FIL_QUIT:
		qApp->quit();
	}
}

void InterfaceDuel::slot_menuGameActivated( int num )
{
	switch( num ) {
	case GAM_FIG:

		break;
	case GAM_END:

		break;
	}
}


void InterfaceDuel::slot_status( QString text )
{
	statusBar()->message( text, 0 );	
}

ServerWidget::ServerWidget(  QWidget * parent , const char * name )
	: QWidget( parent, name )
{
	QLabel labPlayer( this );
	labPlayer.setText( "Nb of players : " );
	labPlayer.setFixedSize( labPlayer.sizeHint() );
	labPlayer.move( 10, 10 );
	
	_editLine = new QLineEdit( this );
	_editLine->setFixedSize( _editLine->sizeHint() );
	_editLine->move( 20 + labPlayer.size().width(), 10 );
	_editLine->setText( "1" );
}


/*!

*/

int ServerWidget::getNbPlayer()
{
	return( _editLine->text().toInt() );
}
