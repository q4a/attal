/****************************************************************
**
** Attal : Lords of Doom
**
** displayScenarii.h
** Smart display of scenarii
**
** Version : $Id: displayScenarii.h,v 1.6 2006/02/05 21:48:36 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2004
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

 
#ifndef DISPLAYSCENARII_H
#define DISPLAYSCENARII_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QTreeWidget>
#include <QWidget>

// application specific include files

class QLabel;

class ScenarioDescription;

/*              ------------------------------
 *                DisplayScenarioDescription
 *              ------------------------------ */

class DisplayScenarioDescription : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayScenarioDescription( QWidget * parent = 0 );
	
	void init( ScenarioDescription * desc );
	
	void clear();
protected:
	QLabel * _labName;
	QLabel * _labDescription;
	QLabel * _labPlayers;
	QLabel * _labSize;
};
 
class ScenarioListViewItem : public QTreeWidgetItem
{
public:
	/** Constructor */
	ScenarioListViewItem( QTreeWidget * list, ScenarioDescription * desc );
	
	/** Destructor */
	virtual ~ScenarioListViewItem();

	ScenarioDescription * getScenario();
	
protected:
	ScenarioDescription * _desc;
};
 
/*              ------------------------------
 *                      DisplayScenarii
 *              ------------------------------ */

class DisplayScenarii : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayScenarii( QWidget * parent = 0 );
	
	void init();
	
	void clear();
	
	ScenarioDescription * getCurrentScenario() { return _currentScen; }
	
public slots:
	void slot_clicked( QTreeWidgetItem * item );
	
signals:
	void sig_dbleClicked();
	
protected:
	DisplayScenarioDescription * _description;
	ScenarioDescription * _currentScen;
	QTreeWidget * _list;
};


/*              ------------------------------
 *                  DisplayScenariiDialog
 *              ------------------------------ */

class DisplayScenariiDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayScenariiDialog( QWidget * parent = 0 );
	
	QString getFileName();
		
protected:
	DisplayScenarii * _display;
};
 
#endif // DISPLAYSCENARII_H   
