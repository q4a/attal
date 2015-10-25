/****************************************************************
**
** Attal : Lords of Doom
**
** attalButton.cpp
** Class for managing buttons in attal
**
** Version : $Id: attalButton.cpp,v 1.7 2007/04/30 12:01:01 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 30/10/2005
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

#include "attalButton.h"
 
// generic include files
// include files for QT
#include <QPixmap>
#include <QSize>
#include <QString>
// application specific include files
#include "libCommon/log.h"

#include "libClient/imageTheme.h"

AttalButton::AttalButton( QWidget * parent, ButtonType type )
: QPushButton( parent )
{
	_type = type;

	switch( _type ) {
	case BT_NONE:
		break;
	case BT_LORD:
		createButtonLord();
		break;
	case BT_BASE:
		createButtonBase();
		break;
	case BT_OK:
		createButtonOk();
		break;
	case BT_CANCEL:
		createButtonCancel();
		break;
	case BT_PREV:
		createButtonPrevious();
		break;
	case BT_NEXT:
		createButtonNext();
		break;
	}
}

void AttalButton::setLord( uint id )
{
	if( _type != BT_LORD ) {
		_type = BT_LORD;
		createButtonLord();
	} 

	setIcon( * ImageTheme.getLordSmallPixmap( id ) );
}

void AttalButton::setBase( uint race )
{
	if( _type != BT_BASE ) {
		_type = BT_BASE;
		createButtonBase();
	} 

	setIcon( * ImageTheme.getBaseSmallPixmap( race ) );
}


void AttalButton::createButtonLord()
{
	setFixedSize( 52, 42 );
	setIconSize( QSize( 50, 40 ) );
}

void AttalButton::createButtonBase()
{
	setFixedSize( 52, 42 );
	setIconSize( QSize( 50, 40 ) );
}


void AttalButton::createButtonOk()
{
	setPixmap( * ImageTheme.getOkPixmap() );
}

void AttalButton::createButtonCancel()
{
	setPixmap( * ImageTheme.getCancelPixmap() );
}

void AttalButton::createButtonPrevious()
{
	setFixedSize( 52, 42 );
	setText( "<<" );
}

void AttalButton::createButtonNext()
{
	setFixedSize( 52, 42 );
	setText( ">>" );
}

void AttalButton::setPixmap( const QPixmap & pixmap )
{
	QSize size = pixmap.size();
	
	setFixedSize( size.width() + 2, size.height() + 2 );
	setIconSize( size );
	setIcon( pixmap );
}

void AttalButton::setBackgroundColor( const QColor & color )
{
	QPalette palette;
	palette.setColor(backgroundRole(), color);
	setPalette(palette);
}
