/****************************************************************
**
** Attal : Lords of Doom
**
** clientWidgets.h
** Various dialog for client
**
** Version : $Id: clientWidgets.h,v 1.5 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 26/07/2006
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

#ifndef CLIENTWIDGETS_H
#define CLIENTWIDGETS_H

// generic include files
// include files for QT
#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QTreeWidget>
#include <QWidget>
#include <QPushButton>

// application specific include files

class QLabel;

class ScenarioDescription;
class Campaign;

/** Dialog for configuring connection to server */
class ConfigConnection : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	ConfigConnection( QWidget * parent = 0 );

	/** Setting hostname */
	void setHost( QString host ) { _host->setText( host ); }

	/** Getting hostname */
	QString getHost() { return _host->text(); }

	int getPort() { return _port->text().toInt(); }

	/** Setting port number */
	void setPort( int port ) { _port->setText( QString::number( port ) ); }

	/** Getting player's name */
	QString getPlayerName() { return _name->text(); }

	/** Setting player's name */
	void setPlayerName( QString name ) { _name->setText( name ); }
	
	void accept() { QDialog::accept(); }

private:
	QLineEdit * _host, * _port, * _name;
};

/** Dialog for start the game */
class StartGameDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	StartGameDialog( QWidget * parent = 0 );
	
	/** Distructor */
	~StartGameDialog();

	void closeEvent(QCloseEvent *event);

	void accept() { QDialog::accept(); }

signals: 
	
	void sig_newScen();
	
	void sig_newCamp();
	
	void sig_loadScen();
	
	void sig_loadCamp();
	
	void sig_dialogClosed();

};

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
	void sig_scenarioSelected();
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
	DisplayScenariiDialog( const QString & title = "", QWidget * parent = 0 );
	
	QString getFileName();

public slots:
	void slot_scenarioSelected();

protected:
	QPushButton * _pbOk;
	DisplayScenarii * _display;
};
 
class CampaignListViewItem : public QTreeWidgetItem
{
public:
	/** Constructor */
	CampaignListViewItem( QTreeWidget * list, Campaign * desc );
	
	/** Destructor */
	virtual ~CampaignListViewItem();

	Campaign * getCampaign();
	
protected:
	Campaign * _desc;
};
 
/*              ------------------------------
 *                      DisplayCampaign
 *              ------------------------------ */

class DisplayCampaign : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayCampaign( QWidget * parent = 0 );
	
	void init();
	
	void clear();
	
	Campaign * getCurrentCampaign() { return _currentCamp; }
	
public slots:
	void slot_clicked( QTreeWidgetItem * item );
	
signals:
	void sig_campaignSelected();
	void sig_dbleClicked();
	
protected:
	Campaign * _currentCamp;
	QTreeWidget * _list;
};


/*              ------------------------------
 *                  DisplayCampaignDialog
 *              ------------------------------ */

class DisplayCampaignDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayCampaignDialog( const QString & title = "", QWidget * parent = 0 );
	
	QString getFileName();

public slots:
	void slot_campaignSelected();

protected:
	QPushButton * _pbOk;
	DisplayCampaign * _displayC;
};
 
#endif // CLIENTWIDGETS_H
