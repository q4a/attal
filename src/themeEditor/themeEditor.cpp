/****************************************************************
**
** Attal : Lords of Doom
**
** themeEditor.cpp
** main file for the theme editor
**
** Version : $Id: themeEditor.cpp,v 1.2 2001/09/08 13:32:02 audoux Exp $
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



// include files for QT
#include <qapplication.h>
#include <qstring.h>

// application specific includes
#include "themeEditor/themeInterface.h"
#include "common/log.h"
#include "common/dataTheme.h"
#include "client/gui.h"
#include "conf.h"

QString IMAGE_PATH( IMAGE_DEFAULT_PATH );
QString DATA_PATH( DATA_DEFAULT_PATH );
QString THEME( THEME_DEFAULT );

extern DataTheme DataTheme;

int main( int argc, char **argv )
{
	QApplication app( argc, argv );
	
	for( int i = 1; i < argc; i++ ) {
		if( ( strcmp( argv[i], "-theme" ) == 0 ) 
		    || ( strcmp( argv[i], "--theme" ) == 0 ) ) {
			i++;
			if( i < argc ) {
				THEME = QString( argv[i] );
			}
		}	
	}
	useTheme();
	
	ThemeInterface edit;
	app.setMainWidget( &edit );
        edit.show();

	return app.exec();	
}
