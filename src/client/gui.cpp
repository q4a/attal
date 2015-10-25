/****************************************************************
**
** Attal : Lords of Doom
**
** gui.cpp
** macros and usefull function for GUI
**
** Version : $Id: gui.cpp,v 1.12 2002/05/04 21:16:04 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/09/2000
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


#include "gui.h"

// include files for QT
#include <qcanvas.h>
#include <qstring.h>
#include <qdir.h>
#include <qpushbutton.h>

// application specific include
#include "common/log.h"
#include "conf.h"
#include "client/imageTheme.h"

extern ImageTheme ImageTheme;

extern QString DATA_PATH;
extern QString IMAGE_PATH;
extern QString THEME;

void useTheme()
{
	QDir folder;

	folder.setPath( "./themes/" + THEME );
	if( ! folder.exists() ) {
		folder.setPath( "./themes/" + THEME_DEFAULT );
		logEE( "Theme %s not found", THEME.latin1() );
		THEME = THEME_DEFAULT;
	}
	QString THEME_PATH = folder.absPath() + "/";

	IMAGE_PATH = THEME_PATH + IMAGE_DEFAULT_PATH;
	DATA_PATH = THEME_PATH + DATA_DEFAULT_PATH;

	/// XXX: need more tests of existence
}

QPushButton * createButtonOk( QWidget * parent )
{
	QPushButton * ret = new QPushButton( parent, "Button Yes" );
	ret->setFixedSize( 50, 40 );
	ret->setPixmap( QPixmap( IMAGE_PATH + "misc/yes.png" ) );

	return ret;
}







