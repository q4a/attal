/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInterface.h
** Manages the scenario editor
**
** Version : $Id: scenarioInterface.h,v 1.18 2008/09/21 19:26:07 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/08/2000
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

 
#ifndef SCENARIOINTERFACE_H
#define SCENARIOINTERFACE_H
 
// include files for QT
#include <QAction>
#include <QDialog>
#include <QMainWindow>
#include <QString>
#include <QVector>

// application specific includes
#include "libCommon/displayHelp.h"

class QComboBox;
class QSignalMapper;

class Screen;

/*              ------------------------------
 *                         ScenarioInterface
 *              ------------------------------ */


class ScenarioInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	ScenarioInterface();
	
	/** Destructor */
	virtual ~ScenarioInterface();
		
public slots:
	/** Slot managing the actions */
	void slot_action( int num );
	
	/** Slot managing the status bar */
	void slot_status( const QString & text );

protected:
	virtual void changeEvent ( QEvent * e );
	
	virtual void closeEvent( QCloseEvent * event );

private:
	enum MENU_ACTION {
		ACTION_NEW,
		ACTION_OPEN,
		ACTION_SAVE,
		ACTION_SAVEAS,
		ACTION_QUIT,
		ACTION_INFO,
		ACTION_PLAYERS,
		ACTION_QUESTS,
		ACTION_CLEAR,
		ACTION_FILL,
		ACTION_MINIMAP,
		ACTION_GRID,
		ACTION_HELP,
		ACTION_ABOUT,

		NB_ACTIONS
	};

	void actionNew();
	void actionOpen();
	void actionSave();
	void actionSaveAs();
	void actionQuit();
	void actionInfo();
	void actionPlayers();
	void actionQuests();
	void actionClear();
	void actionFill();
	void actionMinimap();
	void actionGrid();
	void actionHelp();
	void actionAbout();

	/** Defines actions */
	void initActions();

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	void checkChanged();

	void activateMenuForScenario( bool state = true );

	void addAction( const QString & text, const QString & key,
			MENU_ACTION num, QSignalMapper * sigmap );

	Screen * _screen;
	QString _filename;
	DisplayHelp * _help;
	QVector<QAction *> _actions;
};

/** Dialog asking to choose a tile for filling the map with */
class FillDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	FillDialog( QWidget * parent = 0, const char * name = 0 );

	/** \return Returns the num of the tile choosen */
	uint getTileNumber();

	/** \return Returns the diversification of the tile (0 means 'random') */
	uint getTileDiversification();

public slots:
	void slot_updateDiversification( int tile );
	
private:
	QComboBox * _comboTile, * _comboDiversification;
};

#endif // SCENARIOINTERFACE_H   
