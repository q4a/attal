/****************************************************************
**
** Attal : Lords of Doom
**
** chatWidget.cpp
** Widget for chat between players
**
** Version : $Id: chatWidget.cpp,v 1.10 2008/04/20 14:24:42 lusum Exp $
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


#include "chatWidget.h"

// generic include files
// include files for QT
#include <QLayout>
#include <QMenu>

// application specific include files
#include "libCommon/log.h"

//
// ----- ChatWidget -----
//

ChatWidget::ChatWidget( QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	_chat = new ChatLineEdit( this );
	_chat->setReadOnly( true );

	_edit = new QLineEdit( this );
	_edit->setMinimumHeight( _edit->sizeHint().height() );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _chat );
	layout->addSpacing( 1 );
	layout->addWidget( _edit );
	layout->activate();

	connect( _edit, SIGNAL( returnPressed() ), SLOT( slot_message() ) );
	connect( parent , SIGNAL( sig_newMessage( QString ) ), SLOT( slot_displayMessage( QString ) ) );

	setMinimumHeight( 40 );
}

void ChatWidget::clear()
{
	_edit->clear();
	_chat->clear();
}

void ChatWidget::slot_message()
{
	emit sig_message( _edit->text() );
	_edit->clear();
}

void ChatWidget::slot_displayMessage( QString msg )
{
	_chat->append( msg );
	/// XXX: usefull ? _chat->setCursorPosition( _chat->numLines(), 0 );
}

//
// ----- ChatLineEdit -----
//

ChatLineEdit::ChatLineEdit( QWidget * parent, const char * /* name */)
: QTextEdit( parent )
{
}

QMenu * ChatLineEdit::createPopupMenu( const QPoint & /* pos */ )
{
	QMenu * ret = createStandardContextMenu();

	ret->addAction( "Clear", this, SLOT( clear() ) );

	return ret;
}

//
// ----- ChatDialog -----
//

ChatDialog::ChatDialog()
	: QDialog( 0, Qt::Dialog )
{
	_edit = new QLineEdit( this );
	_edit->setMinimumHeight( _edit->sizeHint().height() );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _edit );
	layout->activate();

	connect( _edit, SIGNAL( returnPressed() ), SLOT( slot_message() ) );

	setMinimumHeight( 20 );
}

void ChatDialog::clear()
{
	_edit->clear();
}

void ChatDialog::slot_message()
{
	emit sig_message( _edit->text() );
	_edit->clear();
}

