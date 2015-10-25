/****************************************************************
**
** Attal : Lords of Doom
**
** aiInterface.h
** interface for the ai-client
**
** Version : $Id: aiInterface.h,v 1.1 2002/11/07 19:45:19 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/02/2001
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


#ifndef AIINTERFACE_H
#define AIINTERFACE_H

// generic include files
#include <stdio.h>
// include files for QT
#include <qmainwindow.h>
#include <qstring.h>
// application specific include files

class QMultiLineEdit;
class QPopupMenu;
class AttalSocket;
class Analyst;

enum IALogLevel {
	LOG_VERBOSE,
	LOG_NORMAL,
	LOG_CRITICAL,
	LOG_QUIET
};


/*              ------------------------------
 *                         Interface
 *              ------------------------------ */


/** comment for the class */
class AiInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	AiInterface();

public slots:
	/** Slot managing 'File' menu */
	void slot_menuFileActivated( int num );

	/** Slot managing 'Log' sub-menu */
	void slot_menuLogActivated( int num );

	/** Slot for reading sockets */
	void slot_readSocket();

	void slot_log( QString msg );

private:
	enum MENU_FIL {
		FIL_CON,
		FIL_DIS,
		FIL_QUIT
	};

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	QPopupMenu * _menuLog;
	QMultiLineEdit * _edit;
	AttalSocket * _socket;
	Analyst * _analyst;
};

/** Log function for the IA */
#ifdef WIN32
void ialog( char *fmt, ... );
#define ialogV ialog
#define ialogN ialog
#define ialogC ialog

#else
void ialog( IALogLevel level, char *fmt, ... );
#define ialogV( format, args... ) ialog( LOG_VERBOSE, format, ## args )
#define ialogN( format, args... ) ialog( LOG_NORMAL, format, ## args )
#define ialogC( format, args... ) ialog( LOG_CRITICAL, format, ## args )
#endif // WIN32

#endif // INTERFACE_H
