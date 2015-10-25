/****************************************************************
**
** Attal : Lords of Doom
**
** interface.cpp
** Manages the editor
**
** Version : $Id: interface.cpp,v 1.10 2001/03/05 17:51:29 audoux Exp $
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


#include "interface.h"
 
// include files for QT
#include <qapplication.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <qcanvas.h>
// application specific include files
#include "client/gui.h"
#include "editor/screen.h"

QCanvasPixmapArray * CellPixmapArray;
QCanvasPixmapArray * LordPixmapArray;
QCanvasPixmapArray * BuildingPixmapArray;

extern QString IMAGE_PATH;

extern QString DATA_PATH;

/** add comments here */
Interface::Interface()
{
	setCaption( "Scenario editor for 'Attal - Lords of Doom'" );
	initMenuBar();
	initStatusBar();
	initPixmap();
	_screen = new Screen( this );
	setCentralWidget( _screen );
	
	resize( 600, 600 );
}


void Interface::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}
	
/*!
  define menus
*/

void Interface::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "New", FILE_NEW );
	menuFile->insertItem( "Open", FILE_OPEN );
	menuFile->insertItem( "Save", FILE_SAVE );	
	menuFile->insertSeparator();
	menuFile->insertItem( "Quit", FILE_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );	

	QPopupMenu * menuScen = new QPopupMenu();
	CHECK_PTR( menuScen );
	menuScen->insertItem( "Scenario Information", SCEN_INFO );
	connect( menuScen, SIGNAL( activated( int ) ), this, SLOT( slot_menuScenActivated( int ) ) );
	
	QPopupMenu * menuHelp = new QPopupMenu();
	CHECK_PTR( menuHelp );
	menuHelp->insertItem( "Help", HELP_INFO );
	connect( menuHelp, SIGNAL( activated( int ) ), this, SLOT( slot_menuHelpActivated( int ) ) );	
		
	menuBar()->insertItem( "&File", menuFile );
	menuBar()->insertItem( "&Scenario", menuScen );
	menuBar()->insertItem( "&Help", menuHelp );
}

void Interface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FILE_NEW:
		_screen->newMap();
		break;
	case FILE_OPEN: {
		QString filename;
		filename = QFileDialog::getOpenFileName( "", "*.map", this );
		if (!filename.isNull()) {
			_screen->load( filename );
		}
		break;
	}
	case FILE_SAVE:{
		QString filename;
		filename = QFileDialog::getSaveFileName( "", "*.map", this );
		if (!filename.isNull()) {
			_screen->save( filename );
		}
		break;
	}
	case FILE_QUIT:
		qApp->quit();
		break;
	}
}

void Interface::slot_menuScenActivated( int num )
{
	switch( num ) {
	case SCEN_INFO:
		_screen->displayInfo();
		break;
	}
}

void Interface::slot_menuHelpActivated( int num )
{
	switch( num ) {
	case HELP_INFO:
		break;
	}
}

void Interface::slot_status( QString text )
{
	statusBar()->message( text, 0 );	
}
