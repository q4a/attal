/****************************************************************
**
** Attal : Lords of Doom
**
** duel.cpp
** main file for duel server
**
** Version : $Id: duel.cpp,v 1.4 2006/02/28 14:20:01 lusum Exp $
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
#include <QWidget>
#include <QMultiLineEdit>
#include <QApplication>
#include <Q3Canvas>
#include <QDir>
// application specific include
#include "conf.h"
#include "libServer/interfaceDuel.h"


QString DATA_PATH;

QString THEME( THEME_DEFAULT );

void useTheme()
{
	QDir folder;
		
	folder.setPath( "./themes/" + THEME );
	if( ! folder.exists() ) {
		folder.setPath( "/usr/local/share/games/attal/themes/" + THEME );
		if( ! folder.exists() ) {
			folder.setPath( "/usr/share/games/attal/themes/" + THEME );
		}
	}
	if( ! folder.exists() ) {
		folder.setPath( "./themes/" + THEME_DEFAULT );
		logEE( "Theme %s not found", THEME.toLatin1() );
		THEME = THEME_DEFAULT;
	}
	QString THEME_PATH = folder.absPath() + "/";
	
	DATA_PATH = THEME_PATH + DATA_DEFAULT_PATH;
		
	/// XXX: need more tests of existence
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

	InterfaceDuel game;
	app.setMainWidget( &game );
        game.show();

	return app.exec();	
}
