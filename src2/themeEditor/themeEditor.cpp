/****************************************************************
**
** Attal : Lords of Doom
**
** themeEditor.cpp
** main file for the theme editor
**
** Version : $Id: themeEditor.cpp,v 1.20 2007/06/01 18:10:45 lusum Exp $
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
#include <QApplication>
#include <QIcon>
#include <QDir>
#include <QLocale>
#include <QString>
#include <QTextCodec>
#include <QTranslator>

// application specific includes
#include "conf.h"

#include "libCommon/attalCommon.h"
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

#include "themeEditor/themeInterface.h"

extern QString IMAGE_PATH;
extern QString THEME;
extern QString THEME_PATH;

int main( int argc, char **argv )
{
	QDir trfolder;
	trfolder = translationsDir(); 

	QApplication app( argc, argv );
	QTranslator translator( 0 );
	translator.load( QString("theme_editor_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibcomm( 0 );
	translatorlibcomm.load( QString("attal_libcommon_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibclient( 0 );
	translatorlibclient.load( QString("attal_libclient_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );

	app.installTranslator( &translator );
	app.installTranslator( &translatorlibcomm );
	app.installTranslator( &translatorlibclient );
	
	if (parseArgs( argc, argv, AttalCommon::ATTALTHEMEEDITOR) ) {
		return 1;
	}
	
	useTheme();
	app.setWindowIcon(QIcon( IMAGE_PATH + "misc/ico_attal-theme-editor.png"));
	if (app.windowIcon().isNull())
		qDebug("Window icon is null!");


	ThemeInterface edit;
	//app.setMainWidget( &edit );
	edit.show();

	return app.exec();	
}
