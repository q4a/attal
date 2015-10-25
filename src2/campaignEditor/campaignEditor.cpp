/****************************************************************
**
** Attal : Lords of Doom
**
** campaignEditor.cpp
** main file for the campaign editor
**
** Version : $Id: campaignEditor.cpp,v 1.15 2007/06/01 18:10:45 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 20/08/2004
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
#include <QIcon>
#include <QLocale>
#include <QString>
#include <QTextCodec>
#include <QTranslator>

// application specific includes
#include "conf.h"

#include "libCommon/attalCommon.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

#include "campaignEditor/campaignInterface.h"

extern QString IMAGE_PATH;
extern QString CAMPAIGN_PATH;
extern QString SCENARIO_PATH;
extern QString THEME;
extern QString THEME_PATH;

int main( int argc, char **argv )
{
	QDir trfolder;
	trfolder = translationsDir(); 

	QApplication app( argc, argv );
	QTranslator translator( 0 );
	translator.load( QString("campaign_editor_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibcomm( 0 );
	translatorlibcomm.load( QString("attal_libcommon_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );
	QTranslator translatorlibclient( 0 );
	translatorlibclient.load( QString("attal_libclient_") + QLocale::system().name().section('_', 0,0 ), trfolder.absolutePath() );

	app.installTranslator( &translator );
	app.installTranslator( &translatorlibcomm );
	app.installTranslator( &translatorlibclient );
	
	if (parseArgs( argc, argv, AttalCommon::ATTALCAMPAIGNEDITOR) ) {
		return 1;
	}

	useTheme();
	app.setWindowIcon(QIcon( IMAGE_PATH + "misc/ico_attal-campaign-editor.png"));
	if (app.windowIcon().isNull())
		qDebug("Window icon is null!");
	
	CampaignInterface edit;
	//app.setMainWidget( &edit );
        edit.show();

	return app.exec();	
}
