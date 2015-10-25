/****************************************************************
**
** Attal : Lords of Doom
**
** main.cpp
** main file for ia
**
** Version : $Id: main.cpp,v 1.4 2002/11/07 19:45:19 audoux Exp $
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

// include files for QT
#include <qapplication.h>
#include <qstring.h>
#include <qwidget.h>
#include <qdir.h>
// application specific includes
#include "conf.h"
#include "ia/aiInterface.h"

QString DATA_PATH;
QString THEME( THEME_DEFAULT );

void useTheme()
{
	QDir folder;

	folder.setPath( "./themes/" + THEME );
	if( ! folder.exists() ) {
		ialogC( "Theme %s not found", THEME.latin1() );
		THEME = THEME_DEFAULT;
		folder.setPath( "./themes/" + THEME );
	}
	QString THEME_PATH = folder.absPath() + "/";

	DATA_PATH = THEME_PATH + DATA_DEFAULT_PATH;

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

	AiInterface game;
	app.setMainWidget( &game );
        game.show();

	return app.exec();
}
