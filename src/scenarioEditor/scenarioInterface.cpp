/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInterface.cpp
** Manages the scenario editor
**
** Version : $Id: scenarioInterface.cpp,v 1.4 2002/10/31 22:59:15 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/08/2000
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


#include "scenarioInterface.h"
 
// include files for QT
#include <qapplication.h>
#include <qpushbutton.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <qcanvas.h>
#include <qcombobox.h>
#include <qlayout.h>
// application specific include files
#include "client/gui.h"
#include "client/imageTheme.h"
#include "common/dataTheme.h"
#include "scenarioEditor/screen.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;

extern QString IMAGE_PATH;
extern QString DATA_PATH;

/** add comments here */
ScenarioInterface::ScenarioInterface()
{
	_filename = "";

	setCaption( "Scenario editor for 'Attal - Lords of Doom'" );
	initMenuBar();
	initStatusBar();
	DataTheme.init();
	ImageTheme.init();
	_screen = new Screen( this );
	setCentralWidget( _screen );

	resize( 600, 600 );
}


void ScenarioInterface::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}

/*!
  define menus
*/

void ScenarioInterface::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	menuFile->insertItem( "New", FILE_NEW );
	menuFile->insertItem( "Open", FILE_OPEN );
	menuFile->insertItem( "Save", FILE_SAVE );
	menuFile->insertItem( "Save as", FILE_SAVEAS );
	menuFile->insertSeparator();
	menuFile->insertItem( "Quit", FILE_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );

	QPopupMenu * menuScen = new QPopupMenu();
	menuScen->insertItem( "Scenario Information", SCEN_INFO );
	connect( menuScen, SIGNAL( activated( int ) ), this, SLOT( slot_menuScenActivated( int ) ) );

	QPopupMenu * menuTools = new QPopupMenu();
	menuTools->insertItem( "Clear screen", TOOL_CLEAR );
	menuTools->insertItem( "Fill screen", TOOL_FILL );
	connect( menuTools, SIGNAL( activated( int ) ), this, SLOT( slot_menuToolsActivated( int ) ) );

	QPopupMenu * menuHelp = new QPopupMenu();
	menuHelp->insertItem( "Help", HELP_INFO );
	connect( menuHelp, SIGNAL( activated( int ) ), this, SLOT( slot_menuHelpActivated( int ) ) );

	menuBar()->insertItem( "&File", menuFile );
	menuBar()->insertItem( "&Scenario", menuScen );
	menuBar()->insertItem( "&Tools", menuTools );
	menuBar()->insertItem( "&Help", menuHelp );
}

void ScenarioInterface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FILE_NEW:
		_screen->newScenario();
		_filename = "";
		break;
	case FILE_OPEN: {
		QString filename = QFileDialog::getOpenFileName( "", "*.scn", this );
		if( !filename.isEmpty() ) {
			if( _screen->load( filename ) ) {
				_filename = filename;
			}
		}
		break;
	}
	case FILE_SAVE:{
		if( _filename.isEmpty() ) {
			slot_menuFileActivated( FILE_SAVEAS );
		} else {
			_screen->save( _filename );
		}
		break;
	}
	case FILE_SAVEAS:{
		_filename = QFileDialog::getSaveFileName( "", "*.scn", this );
		if( !_filename.isEmpty() ) {
			_screen->save( _filename );
		}
		break;
	}
	case FILE_QUIT:
		qApp->quit();
		break;
	}
}

void ScenarioInterface::slot_menuScenActivated( int num )
{
	switch( num ) {
	case SCEN_INFO:
		_screen->displayInfo();
		break;
	}
}

void ScenarioInterface::slot_menuToolsActivated( int num )
{
	switch( num ) {
	case TOOL_CLEAR:
		_screen->clearMap();
		break;
	case TOOL_FILL: 
		handleFill();
		break;
	}
}

void ScenarioInterface::slot_menuHelpActivated( int num )
{
	switch( num ) {
	case HELP_INFO:
		break;
	}
}

void ScenarioInterface::slot_status( QString text )
{
	statusBar()->message( text, 0 );	
}

void ScenarioInterface::handleFill()
{
	FillDialog dialog( this );
	if( dialog.exec() ) {
		_screen->fillMap( dialog.getTileNumber() );
	}
}

//
// ----- FillDialog -----
//

FillDialog::FillDialog( QWidget * parent, const char * name )
	:QDialog( parent, name, true )
{
	setCaption( "Choose tile" );
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	
	_combo = new QComboBox( this );
	for( uint i = 0; i < DataTheme.tiles.count(); i++ ) {
		QString name = QString::number( i ) + " - " + DataTheme.tiles.at( i )->getName();
		_combo->insertItem( name, i );
	}
	FIXEDSIZE( _combo );
	layout->addWidget( _combo );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->addStretch( 1 );
	
	QPushButton * pb1 = new QPushButton( this );
	pb1->setText( "Ok" );
	FIXEDSIZE( pb1 );
	layH1->addWidget( pb1 );
	layH1->addStretch( 1 );
	
	QPushButton * pb2 = new QPushButton( this );
	pb2->setText( "Cancel" );
	FIXEDSIZE( pb2 );
	layH1->addWidget( pb2 );
	layH1->addStretch( 1 );
	
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	
	layout->activate();
	
	//setFixedSize();
	connect( pb1, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pb2, SIGNAL( clicked() ), SLOT( reject() ) );
}

int FillDialog::getTileNumber()
{
	
	return _combo->currentItem();
}
