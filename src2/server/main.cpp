/****************************************************************
**
** Attal : Lords of Doom
**
** attal.cpp
** main file
**
** Version : $Id: main.cpp,v 1.23 2007/06/01 18:10:45 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include <QApplication>
#include <QDir>
#include <QLocale>
#include <QTranslator>
#include <QWidget>

// application specific include
#include "conf.h"

#include "libCommon/attalCommon.h"

#include "server/serverInterface.h"


extern QString IMAGE_PATH;
extern QString THEME;
extern QString THEME_PATH;

extern QString PORT;
extern bool FAST;

int main( int argc, char **argv )
{
	QDir trfolder;
	trfolder = translationsDir(); 

	QApplication app( argc, argv );
	QTranslator translator( 0 );
	translator.load( QString("attal_server_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibs( 0 );
	translatorlibs.load( QString("attal_libserver_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibai( 0 );
	translatorlibai.load( QString("attal_libai_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );

	app.installTranslator( &translator );
	app.installTranslator( &translatorlibs );
	app.installTranslator( &translatorlibai );
	
	if (parseArgs( argc, argv, AttalCommon::ATTALSERVER ) ) {
		return 1;
	}

	useTheme();
	app.setWindowIcon(QIcon( IMAGE_PATH + "misc/ico_attal-server.png"));
	if (app.windowIcon().isNull())
		qDebug("Window icon is null!");

	ServerInterface game;
	//app.setMainWidget( &game );

	game.show();
	if(!game.initServer()) {
		exit(0);
	}


	return app.exec();	
}
