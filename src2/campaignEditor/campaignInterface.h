/****************************************************************
**
** Attal : Lords of Doom
**
** campaignInterface.h
** Manages the campaign editor
**
** Version : $Id: campaignInterface.h,v 1.4 2005/12/26 12:18:26 lusum Exp $
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

 
#ifndef CAMPAIGNINTERFACE_H
#define CAMPAIGNINTERFACE_H
 
// include files for QT
#include <QMainWindow>


#include <QDialog>
#include <QString>
#include <QVector>
// application specific includes

class QAction;
class QComboBox;
class QSignalMapper;

class MainCampaignScreen;

/*              ------------------------------
 *                         CampaignInterface
 *              ------------------------------ */


class CampaignInterface : public QMainWindow
{
	Q_OBJECT
public:
	/** Constructor */
	CampaignInterface();
	
	~CampaignInterface();
		
public slots:
	/** Slot managing the actions */
	void slot_action( int num );
	
	/** Slot managing the status bar */
	void slot_status( const QString & text );
	
protected:
	virtual void changeEvent ( QEvent * e );

private:
	enum MENU_ACTION {
		ACTION_NEW,
		ACTION_OPEN,
		ACTION_SAVE,
		ACTION_SAVEAS,
		ACTION_QUIT,
		ACTION_HELP,

		NB_ACTIONS
	};

	void actionNew();
	void actionOpen();
	void actionSave();
	void actionSaveAs();
	void actionQuit();
	void actionHelp();

	/** Defines actions */
	void initActions();

	/** Define menus */
	void initMenuBar();

	/** Define statusBar */
	void initStatusBar();

	void addAction( const QString & label, const QString & key, MENU_ACTION num, QSignalMapper * sigmap );

	QString _filename;
	MainCampaignScreen * _screen;
	QVector<QAction *> _actions;
};

#endif // CAMPAIGNINTERFACE_H   
