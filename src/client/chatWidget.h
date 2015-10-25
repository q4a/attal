/****************************************************************
**
** Attal : Lords of Doom
**
** chatWidget.h
** Widget for chat between players
**
** Version : $Id: chatWidget.h,v 1.2 2001/04/19 22:26:51 audoux Exp $
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
#include <qmultilinedit.h>
#include <qlineedit.h>
// application specific include files


/*              ------------------------------
 *                         ChatWidget
 *              ------------------------------ */



/** comment for the class */
class ChatWidget : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ChatWidget( QWidget * parent = 0, const char * name = 0 );

	/** Clear chat log */
	void clear();

	/** A new message to display */
	void newMessage( QString );

public slots:
	/** A new message to send */
	void slot_message();

signals:
	void sig_message( QString msg );

private:
	QLineEdit * _edit;
	QMultiLineEdit * _chat;
};

#endif // CHATWIDGET_H
