/****************************************************************
**
** Attal : Lords of Doom
**
** gameInfo.cpp
** Widget giving info during the game
**
** Version : $Id: gameInfo.cpp,v 1.24 2008/06/09 11:27:46 lusum Exp $
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
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/calendar.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericLord.h"
#include "libCommon/unit.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

//
// ----- GameInfo -----
//

GameInfo::GameInfo( Calendar * calendar, QWidget * parent, const char * /* name*/ )
	:QWidget( parent )
{
	_calendar = calendar;
	_nothing = 0;
	_waitPlayer = 0;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	//layout->addStretch( 1 );
	_main = new QWidget( this );
	layout->addWidget( _main );
	_isPlay = new QLabel( this );
	_isPlay->setText( "Not your turn" );
	layout->addWidget( _isPlay );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	
	_dateM = new QLabel( this );
	layH1->addWidget( _dateM );
	layH1->addStretch( 1 );
	/*
	_dateW = new QLabel( this );
	layH1->addWidget( _dateW );
	layH1->addStretch( 1 );
		
	_dateD = new QLabel( this );
	layH1->addWidget( _dateD );
	layH1->addStretch( 1 );	
	*/
	layout->addLayout( layH1 );
	
	layout->activate();
//	setFixedSize( 200, 200 );
	
	nothing();
	
	connect( _calendar, SIGNAL( sig_changed() ), SLOT( slot_dateChanged() ) ); 
}

void GameInfo::nothing()
{
	if( ! _nothing ) {
		_nothing = new QLabel( this );
		_nothing->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( MISC_LOGO ) ) );
		FIXEDSIZE( _nothing );
	}
	if( _waitPlayer ) {
		_waitPlayer->hide();
	}
	_main = _nothing;
	_nothing->show();
}

void GameInfo::setStatePlayer( bool state )
{
	if( _isPlay ) {
		if( state ) {
			_isPlay->setText( "Your turn" );
		} else {
			_isPlay->setText( "Not your turn" );
		}
	}
}

void GameInfo::slot_dateChanged()
{
	_dateM->setText(_calendar->getDate());
	//_dateM->setText( "Month : " + QString::number( _calendar->getMonth() ) );
	//FIXEDSIZE( _dateM );

	//_dateW->setText( "Week : " + QString::number( _calendar->getWeek() ) );
	//FIXEDSIZE( _dateW );	
	
	//_dateD->setText( "Day : " + QString::number( _calendar->getDay() ) );
	//FIXEDSIZE( _dateD );	
}

void GameInfo::waitPlayer( uint num )
{
	if( ! _waitPlayer ) {
		_waitPlayer = new QLabel( this );
	}
	if( _nothing ) {
		_nothing->hide();
	}
	//_waitPlayer->setBackgroundColor( color );
	_waitPlayer->setPixmap( * ImageTheme.getFlag( num ) );
	FIXEDSIZE( _waitPlayer );
	_waitPlayer->show();
	_main = _waitPlayer;
}

//
// ----- LordInfo -----
//

LordInfo::LordInfo( QWidget * parent, const char * /* name */ )
: QWidget( parent )
{
	_photo = new Icon( this );
	_photo->setFixedSize( 50, 60 );
	_name = new QLabel( this );
	_level = new QLabel( this );
	
	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->addWidget( _name );
	layV1->addWidget( _level );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addWidget( _photo );
	layH1->addLayout( layV1, 1 );
	QVBoxLayout * layV[MAX_UNIT];
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setSpacing( 5 );
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		layV[i] = new QVBoxLayout();
		_units[ i ] = new Icon( this );
		_numUnit[i] = new QLabel( this );
		_numUnit[i]->setFixedSize( 60, 20 );
		_numUnit[i]->setAlignment( Qt::AlignCenter );
		layV[i]->addWidget( _units[i] );
		layV[i]->addWidget( _numUnit[i] );
		layH2->addLayout( layV[i] );
	}
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addLayout( layH1 );
	layout->addSpacing( 3 );
	layout->addLayout( layH2 );
	//layout->addStretch( 1 );
	layout->activate();
}

void LordInfo::init( GenericLord * lord )
{
	QString temp;
	
	_name->setText( "Lord " + lord->getName() );
	FIXEDSIZE( _name );
	
	temp.sprintf( "Level %d", lord->getCharac( LEVEL ) );
	_level->setText( temp );
	FIXEDSIZE( _level );
	
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		GenericFightUnit * unit = lord->getUnit( i );
		if( unit ) {
			_units[ i ]->setPixmap( ImageTheme.getPhotoCreature( unit ) );
			_numUnit[i]->setText( QString::number( unit->getNumber() ) );
		} else {
			_numUnit[i]->clear();
			_units[i]->clear();
		}
	}
	
	if( ImageTheme.getLordPixmap( lord->getId() ) ) {
		_photo->setPixmap( * ImageTheme.getLordPixmap( lord->getId() ) );
	}
}

//
// ----- BaseInfo -----
//

BaseInfo::BaseInfo( QWidget * parent, const char * /* name */)
: QWidget( parent )
{
	_name = new QLabel( this );
	_info = new QLabel( this );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 3 );
	layout->addWidget( _name );
	layout->addWidget( _info ); 
	//layout->addStretch( 1 );
	layout->activate();  
}

void BaseInfo::init( GenericBase * base )
{
	_name->setText( base->getName() );
	FIXEDSIZE( _name );
	
	QString text;
	text.sprintf( "%s ", qPrintable( base->getModelName() ) );
	_info->setText( text );
	FIXEDSIZE( _info );
}

//
// ----- InfoStack -----
//

InfoStack::InfoStack( QWidget * parent )
: QStackedWidget( parent )
{
	_info = 0;
	_lord = 0;
	_base = 0;
	_current = 0;
}
	
void InfoStack::init( QWidget * game, QWidget * lord, QWidget * base )
{
	_info = game;
	_lord = lord;
	_base = base;
	addWidget( _info );
	addWidget( _lord );
	addWidget( _base );

	raiseInfo();
}

void InfoStack::raiseLord()
{
	_current = _lord;
	setCurrentWidget( _lord );
}
	
void InfoStack::raiseBase()
{
	_current = _base;
	setCurrentWidget( _base );
}

void InfoStack::raiseInfo()
{
	_current = _info;
	setCurrentWidget( _info );
}
	
void InfoStack::mousePressEvent( QMouseEvent * e )
{
	if( e->button() == Qt::LeftButton ) {
		if( currentWidget() == _current ) {
			setCurrentWidget( _info );
		} else {
			setCurrentWidget( _current );
		}
	}
}




