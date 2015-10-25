/****************************************************************
**
** Attal : Lords of Doom
**
** attal.cpp
** main file
**
** Version : $Id: main.cpp,v 1.22 2008/03/18 23:00:22 lusum Exp $
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
#include <QIcon>
#include <QString>
#include <QTextCodec>
#include <QTranslator>

// application specific includes
#include "libCommon/attalCommon.h"

#include "client/clientInterface.h"

extern QString THEME;

extern QString THEME_PATH;

extern QString IMAGE_PATH;

extern QString PORT;

int main( int argc, char **argv )
{
	QApplication app( argc, argv );
	QDir trfolder;
	trfolder = translationsDir(); 
	
	QTranslator translator( 0 );
	QString trstring("attal_client_" +  QLocale::system().name().section('_', 0,0 ) );
	if (!translator.load( trstring, trfolder.absolutePath() ) ) {
		logEE("failed to load translator %s", qPrintable( trfolder.absolutePath() ));
	}
	QTranslator translatorlibs( 0 );
	translatorlibs.load( QString("attal_libserver_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibcomm( 0 );
	translatorlibcomm.load( QString("attal_libcommon_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibclient( 0 );
	translatorlibclient.load( QString("attal_libclient_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibai( 0 );
	translatorlibai.load( QString("attal_libai_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibfight( 0 );
	translatorlibfight.load( QString("attal_libfight_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );

	app.installTranslator( &translator );
	app.installTranslator( &translatorlibs );
	app.installTranslator( &translatorlibcomm );
	app.installTranslator( &translatorlibclient );
	app.installTranslator( &translatorlibfight );
	app.installTranslator( &translatorlibai );

	if (parseArgs( argc, argv, AttalCommon::ATTALCLIENT ) ) {
		return 1;
	}

	useTheme();
	app.setWindowIcon(QIcon( IMAGE_PATH + "misc/ico_attal-client.png"));
	if (app.windowIcon().isNull())
		qDebug("Window icon is null!");

	ClientInterface game;
	game.showMaximized();
	//app.setMainWidget( &game );
	game.show();

	return app.exec();
}
