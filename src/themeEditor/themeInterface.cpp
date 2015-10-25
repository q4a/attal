/****************************************************************
**
** Attal : Lords of Doom
**
** mapInterface.cpp
** Manages the editor
**
** Version : $Id: themeInterface.cpp,v 1.2 2002/10/16 19:51:43 audoux Exp $
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
#include <qapplication.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qfiledialog.h>
#include <qcanvas.h>
// application specific include files
#include "client/gui.h"
#include "client/imageTheme.h"
#include "common/dataTheme.h"
#include "themeEditor/sectionSelector.h"

QCanvasPixmapArray * CellPixmapArray;
QCanvasPixmapArray * LordPixmapArray;
QCanvasPixmapArray * BuildingPixmapArray;

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;
extern QString IMAGE_PATH;
extern QString DATA_PATH;

/** add comments here */
ThemeInterface::ThemeInterface()
{
	setCaption( "Theme editor for 'Attal - Lords of Doom'" );
	initMenuBar();
	initStatusBar();
	DataTheme.init();
	ImageTheme.init();
	
	_selector = new SectionSelector( this );
	setCentralWidget( _selector );
	
	resize( 600, 600 );
}


void ThemeInterface::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}
	
void ThemeInterface::initMenuBar()
{
	_menuFile = new QPopupMenu();

	_menuFile->insertItem( "New theme", FILE_NEW );
	_menuFile->insertItem( "Open theme", FILE_OPEN );
	_menuFile->insertItem( "Save theme", FILE_SAVE );	
	_menuFile->insertSeparator();
	_menuFile->insertItem( "Quit", FILE_QUIT );
	_menuFile->setItemEnabled( FILE_NEW, false );
	_menuFile->setItemEnabled( FILE_OPEN, false );
	connect( _menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );

	QPopupMenu * menuHelp = new QPopupMenu();
	CHECK_PTR( menuHelp );
	menuHelp->insertItem( "Help", HELP_INFO );
	connect( menuHelp, SIGNAL( activated( int ) ), this, SLOT( slot_menuHelpActivated( int ) ) );

	menuBar()->insertItem( "&File", _menuFile );
	menuBar()->insertItem( "&Help", menuHelp );
}

void ThemeInterface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FILE_NEW:
		_menuFile->setItemEnabled( FILE_SAVE, true );
		break;
	case FILE_OPEN: {
		QString filename;
		filename = QFileDialog::getOpenFileName( "", "*.scn", this );
		if (!filename.isNull()) {

		}
		_menuFile->setItemEnabled( FILE_SAVE, true );
		break;
	}
	case FILE_SAVE: {
		_selector->save();
		DataTheme.save();
		break;
	}
	case FILE_QUIT:
		qApp->quit();
		break;
	}
}

void ThemeInterface::slot_menuHelpActivated( int num )
{
	switch( num ) {
	case HELP_INFO:
		break;
	}
}

void ThemeInterface::slot_status( QString text )
{
	statusBar()->message( text, 0 );	
}
