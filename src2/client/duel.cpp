/****************************************************************
**
** Attal : Lords of Doom
**
** duel.cpp
** main file
**
** Version : $Id: duel.cpp,v 1.2 2005/10/30 22:20:56 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
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
#include <QApplication>
#include <Q3Canvas>
#include <QString>

// application specific includes
#include "client/interfaceDuel.h"
#include "conf.h"

QString IMAGE_PATH( IMAGE_DEFAULT_PATH );

QString DATA_PATH( DATA_DEFAULT_PATH );

QString THEME( THEME_DEFAULT );

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
