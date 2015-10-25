/****************************************************************
**
** Attal : Lords of Doom
**
** themeInterface.h
** Manages the theme editor
**
** Version : $Id: themeInterface.h,v 1.8 2008/03/01 18:13:20 lusum Exp $
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

 
#ifndef THEMEINTERFACE_H
#define THEMEINTERFACE_H
 
// include files for QT
#include <QMainWindow>
#include <QString>
#include <QVector>
// application specific include
#include "libCommon/displayHelp.h"

class QAction;
class QSignalMapper;

class SectionSelector;


/*              ------------------------------
 *                         ThemeInterface
 *              ------------------------------ */


class ThemeInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ThemeInterface();
	
	/** Destructor */
	~ThemeInterface();
	
protected:
	virtual void changeEvent ( QEvent * e );
		
private:
	enum MENU_ACTIONS {
		ACTION_NEW,
		ACTION_OPEN,
		ACTION_SAVE,
		ACTION_QUIT,
		ACTION_HELP,
		ACTION_ABOUT,

		NB_ACTIONS
	};

	/** Defines actions */
	void initActions();

	/** Defines menus */
	void initMenuBar();

	/** Defines statusBar */
	void initStatusBar();
	
	void actionHelp();

	/** Add new action */
	void addAction( const QString & text, const QString & key, MENU_ACTIONS num, QSignalMapper * sigmap );

public slots:
	/** Slot managing actions */
	void slot_actions( int num );

	/** Slot managing the status bar */
	void slot_status( QString text );
	
private:
	SectionSelector * _selector;
	DisplayHelp * _help;
	QVector<QAction *> _actions;
};
 
#endif // THEMEINTERFACE_H   
