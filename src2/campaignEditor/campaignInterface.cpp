/****************************************************************
**
** Attal : Lords of Doom
**
** campaignInterface.cpp
** Manages the campaign editor
**
** Version : $Id: campaignInterface.cpp,v 1.11 2006/11/03 18:28:55 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 20/08/2004
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


#include "campaignInterface.h"
 
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

#include "libClient/gui.h"
#include "libClient/imageTheme.h"

#include "campaignEditor/mainCampaignScreen.h"

extern QString IMAGE_PATH;
extern QString DATA_PATH;
extern QString CAMPAIGN_PATH;
extern QString THEME;

//
// ----- CampaignInterface -----
//

CampaignInterface::CampaignInterface()
{
	_actions.resize( NB_ACTIONS );

	initActions();
	initMenuBar();
	initStatusBar();
	if( DataTheme.init() && ImageTheme.init() ) {
		_screen = new MainCampaignScreen( this );
		setCentralWidget( _screen );
		resize( 600, 600 );
	} else {
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}
}

CampaignInterface::~CampaignInterface()
{
	//free before ImageTheme, after DataTheme (clear of ImageTheme depend on DataTheme data)
	ImageTheme.clear();
	DataTheme.clear();
}

void CampaignInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch (e->type()){
	case QEvent::WindowTitleChange:
	{
		setWindowTitle( "Campaign editor for 'Attal - Lords of Doom'" );
	}
	break;
	default:
	QWidget::changeEvent ( e );
	break;
	}
}



void CampaignInterface::addAction( const QString & label, const QString & key, MENU_ACTION num, QSignalMapper * sigmap )
{
	QAction * action = new QAction( label, this );
	action->setShortcut( QKeySequence( key ) );
	_actions.insert( num, action );
	sigmap->setMapping( action, num );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );
}

void CampaignInterface::initActions()
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "&New" ), tr( "CTRL+N" ), ACTION_NEW, sigmap );
	addAction( tr( "&Open" ), tr( "CTRL+O" ), ACTION_OPEN, sigmap );
	addAction( tr( "&Save" ), tr( "CTRL+S" ), ACTION_SAVE, sigmap );
	addAction( tr( "&Save as" ), "", ACTION_SAVEAS, sigmap );
	addAction( tr( "&Quit" ), tr( "CTRL+Q" ), ACTION_QUIT, sigmap );
	addAction( tr( "&Help" ), tr( "F1" ), ACTION_HELP, sigmap );

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_action( int ) ) );
}

void CampaignInterface::initStatusBar()
{
	statusBar()->showMessage( "Status Bar", 0 );
}

void CampaignInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	QMenu * menuHelp = menuBar()->addMenu( tr( "&Help" ) );

	menuFile->addAction( _actions[ ACTION_NEW ] );
	menuFile->addAction( _actions[ ACTION_OPEN ] );
	menuFile->addAction( _actions[ ACTION_SAVE ] );
	menuFile->addAction( _actions[ ACTION_SAVEAS ] );
	menuFile->addSeparator();
	menuFile->addAction( _actions[ ACTION_QUIT ] );

	menuHelp->addAction( _actions[ ACTION_HELP ] );

}

void CampaignInterface::slot_action( int num )
{
	switch( num ) {
	case ACTION_NEW:
		actionNew();
		break;
	case ACTION_OPEN:
		actionOpen();
		break;
	case ACTION_SAVE:
		actionSave();
		break;
	case ACTION_SAVEAS:
		actionSaveAs();
		break;
	case ACTION_QUIT:
		actionQuit();
		break;
	case ACTION_HELP:
		actionHelp();
		break;
	}
}

void CampaignInterface::actionNew()
{
	_filename = "";
	_screen->newCampaign();
}

void CampaignInterface::actionOpen()
{
	QString filename = QFileDialog::getOpenFileName( this, "Open campaign", CAMPAIGN_PATH, "*.cmp" );
	if( ! filename.isEmpty() ) {
		_filename=  filename;
		_screen->load( filename );
	}
}

void CampaignInterface::actionSave()
{
	if( _filename.isEmpty() ) {
		slot_action( ACTION_SAVEAS );
	} else {
		_screen->save( _filename );
	}
}

void CampaignInterface::actionSaveAs()
{
	_filename = QFileDialog::getSaveFileName( this, "Save campaign as", "", "*.cmp" );
	if( ! _filename.isEmpty() ) {
		_screen->save( _filename );
	}
}

void CampaignInterface::actionQuit()
{
	qApp->quit();
}

void CampaignInterface::actionHelp()
{
}

void CampaignInterface::slot_status( const QString & text )
{
	statusBar()->showMessage( text, 0 );
}

