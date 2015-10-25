/****************************************************************
**
** Attal : Lords of Doom
**
** chatWidget.h
** Widget for chat between players
**
** Version : $Id: chatWidget.h,v 1.5 2007/08/26 14:12:36 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 13/03/2001
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



#ifndef CHATWIDGET_H
#define CHATWIDGET_H


// generic include files
// include files for QT
#include <QDialog>
#include <QLineEdit>
#include <QPoint>
#include <QTextEdit>

// application specific include files


class QMenu;

class ChatLineEdit;

/*              ------------------------------
 *                         ChatWidget
 *              ------------------------------ */

/// XXX: Don't works perfeectly

class ChatWidget : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ChatWidget( QWidget * parent = 0, const char * name = 0 );

	/** Clear chat log */
	void clear();

public slots:
	/** A new message to send */
	void slot_message();
	
	/** A new message to display */
	void slot_displayMessage( QString );

signals:
	void sig_message( QString msg );

private:
	QLineEdit * _edit;
	ChatLineEdit * _chat;
};

/*              ------------------------------
 *                         ChatDialog
 *              ------------------------------ */

class ChatDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	ChatDialog();

	/** Clear chat log */
	void clear();

public slots:
	/** A new message to send */
	void slot_message();
	
signals:
	void sig_message( QString msg );

private:
	QLineEdit * _edit;
};

/** */
class ChatLineEdit : public QTextEdit
{
public:
	/** constructor */
	ChatLineEdit( QWidget * parent = 0, const char * name = 0 );

protected:
	QMenu * createPopupMenu( const QPoint & pos );
};

#endif // CHATWIDGET_H
