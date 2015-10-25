/****************************************************************
**
** Attal : Lords of Doom
**
** mapInterface.cpp
** Manages the editor
**
** Version : $Id: themeInterface.cpp,v 1.19 2008/03/01 18:13:17 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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


#include "themeInterface.h"
 
// include files for QT
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMenu>
#include <QSignalMapper>
#include <QStatusBar>

// application specific include files
#include "conf.h"

#include "libCommon/dataTheme.h"

#include "libClient/aboutDialog.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

#include "themeEditor/sectionSelector.h"

extern QString IMAGE_PATH;
extern QString DATA_PATH;
extern QString THEME;

ThemeInterface::ThemeInterface()
{
	_help =NULL;
	initActions();
	initMenuBar();
	initStatusBar();

	if( DataTheme.init() && ImageTheme.init() ) {
		_selector = new SectionSelector( this );
		setCentralWidget( _selector );
	} else {
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}

	resize( 600, 600 );
}

ThemeInterface::~ThemeInterface()
{
	//free before ImageTheme, after DataTheme (clear of ImageTheme depend on DataTheme data)
	ImageTheme.clear();
	DataTheme.clear();
}

void ThemeInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch (e->type()){
	case QEvent::WindowTitleChange:
	{
		setWindowTitle( tr( "Theme editor for 'Attal - Lords of Doom'" ) );
	}
	break;
	default:
	QWidget::changeEvent ( e );
	break;
	}
}



void ThemeInterface::initStatusBar()
{
	statusBar()->showMessage( tr( "Status Bar" ), 0 );
}

void ThemeInterface::addAction( const QString & text, const QString & key, MENU_ACTIONS num, QSignalMapper * sigmap )
{
	QAction * action = new QAction( text, this );
	action->setShortcut( QKeySequence( key ) );
	_actions.insert( num, action );
	sigmap->setMapping( action, num );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );
}

void ThemeInterface::initActions()
{
	_actions.resize( NB_ACTIONS );

	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "&New theme" ), tr( "CTRL+N" ), ACTION_NEW, sigmap );
	addAction( tr( "&Open theme" ), tr( "CTRL+O" ), ACTION_OPEN, sigmap );
	addAction( tr( "&Save theme" ), tr( "CTRL+S" ), ACTION_SAVE, sigmap );
	addAction( tr( "&Quit" ), tr( "CTRL+Q" ), ACTION_QUIT, sigmap );
	addAction( tr( "&Help" ), tr( "F1" ), ACTION_HELP, sigmap );
	addAction( tr( "&About" ), tr( "CTRL+A" ), ACTION_ABOUT, sigmap );

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_actions( int ) ) );

	_actions[ ACTION_NEW ]->setEnabled( false );
	_actions[ ACTION_OPEN ]->setEnabled( false );
}

void ThemeInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	QMenu * menuHelp = menuBar()->addMenu( tr( "&Help" ) );

	menuFile->addAction( _actions[ ACTION_NEW ] );
	menuFile->addAction( _actions[ ACTION_OPEN ] );
	menuFile->addAction( _actions[ ACTION_SAVE ] );
	menuFile->addSeparator();
	menuFile->addAction( _actions[ ACTION_QUIT ] );

	menuHelp->addAction( _actions[ ACTION_HELP ] );
	menuHelp->addAction( _actions[ ACTION_ABOUT ] );

}

void ThemeInterface::slot_actions( int num )
{
	switch( num ) {
	case ACTION_NEW:
		_actions[ ACTION_SAVE ]->setEnabled( true );
		break;
	case ACTION_OPEN: {
		QString filename;
		filename = QFileDialog::getOpenFileName( this, tr( "Open theme" ), "", "*.scn" );
		if (!filename.isNull()) {

		}
		_actions[ ACTION_SAVE ]->setEnabled( true );

		break;
	}
	case ACTION_SAVE: {
		_selector->saveAll();
		DataTheme.save();
		break;
	}
	case ACTION_QUIT:
		qApp->quit();
		break;
	case ACTION_HELP:
		actionHelp();
		break;
	case ACTION_ABOUT: {
			AboutDialog dialog( this );
			dialog.exec();
		} break;
	}
}

void ThemeInterface::actionHelp()
{	
	if( ! _help ) {
		_help = new DisplayHelp( this );
	}
	_help->resize( 800, 600 );
	_help->show();

}



void ThemeInterface::slot_status( QString text )
{
	statusBar()->showMessage( text, 0 );	
}
