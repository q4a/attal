/****************************************************************
**
** Attal : Lords of Doom
**
** attal.cpp
** main file
**
** Version : $Id: main.cpp,v 1.6 2002/11/07 22:08:53 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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


// include files for QT
#include <qwidget.h>
#include <qmultilinedit.h>
#include <qapplication.h>
#include <qcanvas.h>
#include <qdir.h>
// application specific include
#include "conf.h"
#include "server/serverInterface.h"


QString DATA_PATH;
QString SCENAR_PATH;
QString THEME( THEME_DEFAULT );

void useTheme()
{
	QDir folder;
		
	folder.setPath( "./themes/" + THEME );
	if( ! folder.exists() ) {
		logEE( "Theme %s not found", THEME.latin1() );
		THEME = THEME_DEFAULT;
		folder.setPath( "./themes/" + THEME );
	}
	QString THEME_PATH = folder.absPath() + "/";
	
	DATA_PATH = THEME_PATH + DATA_DEFAULT_PATH;
	SCENAR_PATH = THEME_PATH + "scenarii/";
	
	/// XXX: needs more tests of existence
}
int main( int argc, char **argv )
{
	QApplication app( argc, argv );

	for( int i = 1; i < argc; i++ ) {
		if ( strcmp( argv[i], "-theme" ) == 0 ) {
			i++;
			if( i < argc )
				THEME = QString( argv[i] );
		}	
	}
	useTheme();

	ServerInterface game;
	app.setMainWidget( &game );
        game.show();

	return app.exec();	
}
