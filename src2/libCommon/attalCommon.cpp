/****************************************************************
**
** Attal : Lords of Doom
**
** attalCommon.cpp
** Manage common data for everyone
**
** Version : $Id: attalCommon.cpp,v 1.28 2010/01/10 13:54:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/07/2005
**
** Licence :
**  This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
****************************************************************/

#include "attalCommon.h"

// generic include files
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
// include files for QT
#include <QDir>
#include <QLocale>
#include <QString>
// application specific include files
#include "conf.h"
#include "libCommon/log.h"

using namespace std;

QString IMAGE_PATH( IMAGE_DEFAULT_PATH );

QString MUSIC_PATH( MUSIC_DEFAULT_PATH );

QString SOUND_PATH( SOUND_DEFAULT_PATH );

QString DATA_PATH( DATA_DEFAULT_PATH );

QString CAMPAIGN_PATH( CAMPAIGN_DEFAULT_PATH );

QString SCENARIO_PATH( SCENARIO_DEFAULT_PATH );

QString CONFIG_PATH( CONFIG_DEFAULT_PATH );

QString THEME( THEME_DEFAULT );

QString SAVE_PATH;

QString THEME_CONFIG_PATH;

QString VERSION(ATTAL_VERSION);

QString THEME_PATH;

QString PORT = "1717";
bool FAST = false;
bool HIDE = false;

#ifdef WITH_SOUND
bool ATT_SOUND = true;
#endif

using namespace AttalCommon;

int parseArgs(int argc, char** argv, AttalProgramType prog )
{

#ifdef WITH_SOUND
	if( prog !=  ATTALCLIENT /* && prog != ATTALTHEMEEDITOR */ ) {
		ATT_SOUND = false;
	}
#endif

	for( int i = 1; i < argc; i++ ) {
		if( ( strcmp( argv[i], "-theme" ) == 0 ) || ( strcmp( argv[i], "--theme" ) == 0 ) ) {
			i++;
			if( i < argc )
				THEME = QString( argv[i] );
		} else if(  (strcmp( argv[i], "-port" ) == 0 ) || ( strcmp( argv[i], "--port" ) == 0) )  {
			i++;
			if( i < argc ){
				PORT = QString( argv[i] );
			}
		} else if( ( strcmp( argv[i], "-d" ) == 0 )  ) {	
			int level;
			i++;
			if( i < argc ){
				level = atoi(argv[i]) ;
				setLogLevel((LogLevel) level );
			}
		} else if(  strcmp( argv[i], "--hide" ) == 0  ) {
			HIDE = true;
		} else if( ( strcmp( argv[i], "--themepath" ) == 0 )  ) {	
			i++;
			if( i < argc )
				THEME_PATH = QString( argv[i] );
		} else if( ( strcmp( argv[i], "-fast" ) == 0 ) || ( strcmp( argv[i], "--fast" ) == 0 ) ) {		
			FAST = true;
		} else if(  strcmp( argv[i], "--nosound")  == 0  ) {
#ifdef WITH_SOUND
			ATT_SOUND = false;
#endif
		} else if( ( strcmp( argv[i], "-help" ) == 0 ) || ( strcmp( argv[i], "--help" ) == 0 ) ) {
			switch( prog ) {
				case ATTALSERVER:
					cout << "Usage: attal-server [OPTION]"  << endl;
					cout << "Start the server "  << endl;
					cout << endl;
					break;
				case ATTALCLIENT:
					cout << "Usage: attal-client [OPTION]"  << endl;
					cout << "Start client "  << endl;
					cout << endl;
					break;
				case ATTALCAMPAIGNEDITOR:
					cout << "Usage: campaignEditor [OPTION]"  << endl;
					cout << "Start campaignEditor "  << endl;
					cout << endl;
					break;
				case ATTALSCENARIOEDITOR:
					cout << "Usage: scenarioEditor [OPTION]"  << endl;
					cout << "Start scenarioEditor "  << endl;
					cout << endl;
					break;
				case ATTALTHEMEEDITOR:
					cout << "Usage: themeEditor [OPTION]"  << endl;
					cout << "Start themeEditor "  << endl;
					cout << endl;
					break;
				case ATTALAI:
					cout << "Usage: attal-ai [OPTION]"  << endl;
					cout << "Start ai robot "  << endl;
					cout << endl;
					cout << "  --hide                      Hide ai (remember to kill)"  << endl;
					break;
			}
			switch( prog ) {
				case ATTALSERVER:
				case ATTALAI:
					cout << "  -port   --port [PORT]       Used to use a particular port"  << endl;
					cout << "  -fast   --fast              Fast start use: localhost:1717"  << endl;
					break;
				case ATTALCLIENT:
				case ATTALTHEMEEDITOR:
#ifdef WITH_SOUND
					cout << "  --nosound                   Disable totally sound (ex: for hardware problems)"  << endl;
#endif
					break;
				default:
					break;
			};

			cout << "  -theme  --theme [NAME]      Load a particular theme (Medieval by default)"  << endl;
			cout << "  --themepath [NAME]          Get the path of theme (local directory or hardcoded path will be the default)"  << endl;
			cout << "  -d [LEVEL]                  Level of debug messages (0-5) (default 4)"  << endl;
			cout << "  --version -v                Prints attal's version and exits."  << endl;
			cout << "  -help   --help              This screen"  << endl;
			return 1; 
		} else if( ( strcmp( argv[i], "--version" ) == 0 ) || ( strcmp( argv[i], "-v" ) == 0)  ) {	
			cout << "Attal Lords of Doom " << qPrintable(VERSION) << endl;
#ifdef __GNUC__
			cout << "Compiler:" << "\tGCC " <<  __GNUC__ << "." << __GNUC_MINOR__ << "." <<  __GNUC_PATCHLEVEL__  << "\n";
#endif
			return 1;
		}
	}
	return 0;

}

void useTheme()
{
	QDir folder;
	
	folder = setThemePath( THEME_PATH );

	if( ! folder.exists() ) {
		QString envpath = getenv("ATTAL_THEME_PATH");
		TRACE("EnvPath %s", qPrintable(envpath));
		folder = setThemePath( envpath );
	}

	if( ! folder.exists() ) {
		folder = setThemePath( "./themes" );
	}

#ifndef WIN32
	QString qConfigPath;
	qConfigPath = ATT_THEMES_DIR;
	if( ! folder.exists() ) {
		folder = setThemePath( qConfigPath );
	}
#endif

	if( ! folder.exists() ) {
		logEE( "Theme %s not found", qPrintable(THEME) );
		THEME = THEME_DEFAULT;
	}
	THEME_PATH = folder.absolutePath() + "/";

	TRACE("ThemePath %s", qPrintable(THEME_PATH));

	IMAGE_PATH = THEME_PATH + IMAGE_DEFAULT_PATH;
	SOUND_PATH = THEME_PATH + SOUND_DEFAULT_PATH;
	MUSIC_PATH = THEME_PATH + MUSIC_DEFAULT_PATH;
	DATA_PATH = THEME_PATH + DATA_DEFAULT_PATH;
	CAMPAIGN_PATH = THEME_PATH + CAMPAIGN_DEFAULT_PATH;
	
	TRACE("Campaign Path %s", qPrintable(CAMPAIGN_PATH));
	SCENARIO_PATH = THEME_PATH + SCENARIO_DEFAULT_PATH;

	QDir tmpdir = QDir::home();
	if( tmpdir.isRoot() ) {
		CONFIG_PATH = QDir::homePath() + CONFIG_DEFAULT_PATH; 
	} else {
		CONFIG_PATH = QDir::homePath() + QDir::separator() + CONFIG_DEFAULT_PATH; 
	}
	folder.setPath(CONFIG_PATH);
	if( ! folder.exists() ) {
		if(!folder.mkdir(CONFIG_PATH)) {
			logEE( "Cannot create config dir %s", qPrintable(CONFIG_PATH) );
		}
	}
	THEME_CONFIG_PATH = CONFIG_PATH + THEME;
	folder.setPath(THEME_CONFIG_PATH);
	if( ! folder.exists() ) {
		if(!folder.mkdir(THEME_CONFIG_PATH)) {
			logEE( "Cannot create theme config dir %s", qPrintable(THEME_CONFIG_PATH) );
		}
	}
	SAVE_PATH =  THEME_CONFIG_PATH +  "/saves/";

	TRACE("ThemeConfigPath %s, savepath %s", qPrintable(THEME_CONFIG_PATH), qPrintable(SAVE_PATH));

	folder.setPath(SAVE_PATH);
	if( ! folder.exists() ) {
		if(!folder.mkdir(SAVE_PATH)) {
			logEE( "Cannot create save dir %s", qPrintable(SAVE_PATH) );
		}
	}
	/// XXX: need more tests of existence
}

QDir translationsDir()
{

    QDir trfolder;
    trfolder.setPath( "./i18n/" +  QLocale::system().name() .section('_', 0,0 ) );
#ifndef WIN32
		QString qConfigPath;
		qConfigPath = ATT_TRANSL_DIR;
		//logDD("attalCommon %s"qPrintable(qConfigPath));
    if( ! trfolder.exists() ) {
			trfolder.setPath( qConfigPath  );
    }
#endif
    return trfolder;

}


QDir setThemePath( QString path )
{
	QDir tmpfolder;

	tmpfolder.setPath( path + QDir::separator() + THEME );

	return tmpfolder;

}

