/****************************************************************
**
** Attal : Lords of Doom
**
** serverWidgets.h
** Various dialog for server
**
** Version : $Id: serverWidgets.h,v 1.1 2006/10/11 15:42:49 lusum Exp $
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

#ifndef SERVERWIDGETS_H
#define SERVERWIDGETS_H

// generic include files
// include files for QT
#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTreeWidget>
#include <QWidget>

// application specific include files
#include "libServer/attalServer.h"

class QButtonGroup;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;

/** radio button allowing to choose a file on the disk */
class ChooseFileWidget : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ChooseFileWidget( QWidget * parent = 0, const char * name = 0 );

	/** Return file in the lineedit */
	QString getText() { return _edit->text(); }

public slots:
	/** Slot for choosing file */
	void slot_choose();

	/** Toggle radio button */
	void slot_toggle( bool );

private:
	QLineEdit * _edit;
	QPushButton * _choose;
};

/** Main widget for the server */
class ServerWidget : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ServerWidget( QWidget * parent = 0, const char * name = 0 );

	/** Toggle buttons state as game started or not */
	void setGameLoaded( bool b );

	/** returns filename to load */
	QString getFilename();

public slots:
	/** Slot for start button */
	void slot_start();

	/** Slot for handling new players */
	void slot_newPlayer( AttalPlayerSocket * player );

	/** slot for handling disconnected players */
	void slot_endConnection( QString name );

	void slot_clicked(QTreeWidgetItem * item, int column);

signals:
	/** Signal for buttopn 'End' */
	void sig_stop();

	/** Signal for button 'Load' */
	void sig_load( QString );

	/** Signal for button 'Save' */
	void sig_save();

	void sig_banned( QString name);

private:

	QButtonGroup * _group;
	QGroupBox * _groupWidget;
	QPushButton * _butStart, * _butStop, * _butSave;
	QRadioButton * _radio1, * _radio2, * _radio3;
	ChooseFileWidget * _choose;

	QTreeWidget * _playerList;
	bool _loaded;
};

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

	void accept() { QDialog::accept(); }

private:
	QLineEdit * _host, * _port;
};

#define FIXEDSIZE( wid ) (wid)->setFixedSize( (wid)->sizeHint() )

#endif // SERVERWIDGETS_H
