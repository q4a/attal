/****************************************************************
**
** Attal : Lords of Doom
**
** gameInfo.cpp
** Widget giving info during the game
**
** Version : $Id: gameInfo.cpp,v 1.5 2001/10/29 16:30:57 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/01/2001
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

#include "gameInfo.h"
 
// generic include files
// include files for QT
#include <qlabel.h>
#include <qpixmap.h>
#include <qstring.h>
// application specific include files
#include "common/calendar.h"
#include "client/gui.h"

extern QString IMAGE_PATH;

/** add comments here */
GameInfo::GameInfo( Calendar * calendar, QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_calendar = calendar;
	_nothing = 0;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	layout->addStretch( 1 );
	_main = new QWidget( this );
	layout->addWidget( _main );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	
	_dateM = new QLabel( this );
	layH1->addWidget( _dateM );
	layH1->addStretch( 1 );
	
	_dateW = new QLabel( this );
	layH1->addWidget( _dateW );
	layH1->addStretch( 1 );
		
	_dateD = new QLabel( this );
	layH1->addWidget( _dateD );
	layH1->addStretch( 1 );	
	
	layout->addLayout( layH1 );
	
	layout->activate();
	setFixedSize( 200, 200 );
	
	nothing();
	
	connect( _calendar, SIGNAL( sig_changed() ), SLOT( slot_dateChanged() ) ); 
}

void GameInfo::nothing()
{
	if( ! _nothing ) {
		_nothing = new QLabel( this );
		_nothing->setPixmap( QPixmap( IMAGE_PATH + "misc/logo.png" ) );
		FIXEDSIZE( _nothing );
	}
	_main = _nothing;
	
}

void GameInfo::slot_dateChanged()
{
	_dateM->setText( "Month : " + QString::number( _calendar->getMonth() ) );
	FIXEDSIZE( _dateM );
	
	_dateW->setText( "Week : " + QString::number( _calendar->getWeek() ) );
	FIXEDSIZE( _dateW );	
	
	_dateD->setText( "Day : " + QString::number( _calendar->getDay() ) );
	FIXEDSIZE( _dateD );	
}

void GameInfo::waitPlayer( QColor color )
{
	if( ! _waitPlayer )
		_waitPlayer = new QWidget( this );
	_waitPlayer->setBackgroundColor( color );
	_main = _waitPlayer;
}
