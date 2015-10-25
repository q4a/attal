/****************************************************************
**
** Attal : Lords of Doom
**
** chatWidget.cpp
** Widget for chat between players
**
** Version : $Id: chatWidget.cpp,v 1.3 2002/08/06 20:11:43 audoux Exp $
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
#include <qlayout.h>
// application specific include files
#include "common/log.h"


ChatWidget::ChatWidget( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );

	_chat = new QMultiLineEdit( this );
	_chat->setReadOnly( true );
	layout->addWidget( _chat, 1 );

	layout->addSpacing( 1 );

	_edit = new QLineEdit( this );
	_edit->setMinimumHeight( _edit->sizeHint().height() );
	layout->addWidget( _edit );

	layout->activate();

	connect( _edit, SIGNAL( returnPressed() ), SLOT( slot_message() ) );

	setMinimumHeight( 130 );
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

void ChatWidget::newMessage( QString msg )
{
	_chat->append( msg );
	_chat->setCursorPosition( _chat->numLines(), 0 );
}


