/****************************************************************
**
** Attal : Lords of Doom
**
** gameConrol.cpp
** Widget on the right, controlling the game
**
** Version : $Id: gameControl.cpp,v 1.22 2002/11/09 21:10:09 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/09/2000
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

#include "gameControl.h"

 
// generic include files
// include files for QT
#include <qsignalmapper.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
// application specific includes
#include "conf.h"

extern QString IMAGE_PATH;

/** add comments here */
GameControl::GameControl( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * lay = new QHBoxLayout( this );
	
	_cen = new CentralControl( this );
	_scrL = new ScrollLord( this );
	_scrB = new ScrollBase( this );
	lay->addWidget( _scrL );
	lay->addStretch( 1 );
	lay->addWidget( _cen );
	lay->addStretch( 1 );	
	lay->addWidget( _scrB );
	lay->activate();
	
	setFixedSize( 200, 210 );
	
	connect( _cen, SIGNAL( sig_endTurn() ), SIGNAL( sig_endTurn() ) );
	connect( _scrL, SIGNAL( sig_lord() ), SIGNAL( sig_lord() ) );
	connect( _scrL, SIGNAL( sig_lordSelected() ), SLOT( slot_lordSelected() ) );
	connect( _scrB, SIGNAL( sig_base() ), SIGNAL( sig_base() ) );
	connect( _scrB, SIGNAL( sig_baseSelected() ), SLOT( slot_baseSelected() ) );
}

void GameControl::setPlayer( Player * player )
{
	_player = player;
	_cen->setPlayer( player );
	_scrL->setPlayer( player );
	_scrB->setPlayer( player );
}

void GameControl::reinit()
{
	_scrL->reinit();
	_scrB->reinit();	
}

void GameControl::selectFirst()
{
	if( ! _player->selectFirstLord() )
		_player->selectFirstBase();
	
	reinit();
}

void GameControl::disableGame()
{
	_scrL->setEnabled( false );
	_scrB->setEnabled( false );
	_cen->disableGame();
}

void GameControl::enableGame()
{
	_scrL->enableGame();
	_scrB->enableGame();
	_cen->enableGame();	
}

void GameControl::slot_baseSelected()
{
	_scrL->deselect();
}

void GameControl::slot_lordSelected()
{
	_scrB->deselect();
}

//
// ----- CentralControl -----
//

CentralControl::CentralControl( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_player = 0;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	layout->addStretch( 1 );
	
	QWidget * w1 = new QWidget( this );
	w1->setFixedSize( 100, 40 );
	QPushButton * pb11 = new QPushButton( w1 );
	pb11->setFixedSize( 50, 40 );
	QPushButton * pb12 = new QPushButton( w1 );
	pb12->setFixedSize( 50, 40 );
	pb12->move( 50, 0 );
	layout->addWidget( w1 );
	
	QWidget * w2 = new QWidget( this );
	w2->setFixedSize( 100, 40 );
	QPushButton * pb21 = new QPushButton( w2 );
	pb21->setFixedSize( 50, 40 );
	QPushButton * pb22 = new QPushButton( w2 );
	pb22->setFixedSize( 50, 40 );
	pb22->move( 50, 0 );
	layout->addWidget( w2 );
	
	QWidget * w3 = new QWidget( this );
	w3->setFixedSize( 100, 40 );
	_pbQuit = new QPushButton( w3 );
	_pbQuit->setFixedSize( 50, 40 );
	_pbQuit->setText( "Quit" );
	_pbQuit->move( 50, 0 );
	layout->addWidget( w3 );
	
	QWidget * w4 = new QWidget( this );
	w4->setFixedSize( 100, 40 );
	_pbNext = new QPushButton( w4 );
	_pbNext->setFixedSize( 100, 40 );	
	_pbNext->setText( "Next Lord" );
	layout->addWidget( w4 );
	
	QWidget * w5 = new QWidget( this );
	w5->setFixedSize( 100, 40 );
	_pbTurn = new QPushButton( w5 );
	_pbTurn->setFixedSize( 100, 40 );	
	_pbTurn->setText( "End Turn" );
	layout->addWidget( w5 );	
		
	layout->addStretch( 1 );
	
	layout->activate();
	setFixedSize( 100, 210 );

	connect( _pbQuit, SIGNAL( clicked() ), qApp, SLOT( quit() ) );
	connect( _pbNext, SIGNAL( clicked() ), SLOT( slot_nextLord() ) );
	connect( _pbTurn, SIGNAL( clicked() ), SIGNAL( sig_endTurn() ) );
	
	disableGame();
}

void CentralControl::disableGame()
{
	_pbTurn->setEnabled( false );
	_pbNext->setEnabled( false );
}

void CentralControl::enableGame()
{
	_pbTurn->setEnabled( true );
	_pbNext->setEnabled( true );
}

//
// ----- ScrollList -----
//

ScrollList::ScrollList( QWidget * parent, const char * name )
	: QWidget( parent, name ),
	  _current( 0 ),
	  _selected( -1 )
{
	QLabel * lab1 = new QLabel( this );
	lab1->setPixmap( QPixmap( IMAGE_PATH + "misc/arrow_up.png" ) );
	lab1->setFixedSize( 50, 5 );
	
	QLabel * lab2 = new QLabel( this );
	lab2->setPixmap( QPixmap( IMAGE_PATH + "misc/arrow_down.png" ) );
	lab2->setFixedSize( 50, 5 );
	lab2->move( 0, 205 );
	
	QSignalMapper * sigmap = new QSignalMapper( this );
	
	for( int i = 0; i < 5; i++ ) {
	        _buttons[i] = new QPushButton( this );
		_buttons[i]->setFixedSize( 50, 40 );
		_buttons[i]->move( 0, (i*40) + 5 );
		sigmap->setMapping( _buttons[i], i );		
		connect( _buttons[i], SIGNAL( clicked() ), sigmap, SLOT( map() ) );
	}
	
	setFixedSize( 50, 210 );
	connect( sigmap, SIGNAL( mapped( int ) ), this, SLOT( slot_clicked( int ) ) );
}

void ScrollList::enableGame()
{
	setEnabled( true );
}

void ScrollList::slot_up()
{
	if( _current > 0 ) {
		_current--;
		reinit();
	}
}

void ScrollList::slot_down()
{
	if( _current + 4 < _player->numLord() - 1 ) {
		_current++;
		reinit();
	}	
}

void ScrollList::select( int num )
{
	deselect();
	/// XXX: mettre cadre
	_selected = num + _current;
	_buttons[num]->setBackgroundColor( yellow );
}

void ScrollList::deselect()
{
	if( _selected != -1 ) {
		_selected = -1;
		for( int i = 0; i < 5; i++ ) {
			_buttons[i]->setBackgroundColor( blue );
		}
	}
}

//
// ----- ScrollLord -----
//

ScrollLord::ScrollLord( QWidget * parent, const char * name )
	: ScrollList( parent, name )
{
	_lordList.setAutoDelete( false );
}

void ScrollLord::reinit()
{
	_lordList.clear();
	uint i;

	for( i = 0; i < _player->numLord(); i++ ) {
		GenericLord * lord = _player->getLord( i );
		if( lord && lord->isVisible() ) {
			_lordList.append( lord );
		}
	}

	_selected = -1;
	for( i = 0; i < 5; i++ ) {
		if( _current + i < _lordList.count() ) {
			_buttons[i]->setEnabled( true );
			_buttons[i]->setPixmap( *_player->getLordSmallPixmapById( _lordList.at( _current + i )->getId() ) );
			if( _player->getSelectedLord() && ( _lordList.at( _current + i ) == _player->getSelectedLord() ) ) {
				_buttons[i]->setBackgroundColor( yellow );
				_selected = _current + i;
			}
		} else {
			_buttons[i]->setBackgroundColor( blue );
			_buttons[i]->setEnabled( false );
		}
	}
}

void ScrollLord::select( int num )
{
	if(_player) {
		_player->unSelectLord();
		_player->unSelectBase();

		ScrollList::select( num );

		if( _selected < (int)_lordList.count() ) {
			_player->setSelectedLord( _lordList.at( _selected ) );
			emit sig_lordSelected();
		} else {
			deselect();
		}
	}
}

void ScrollLord::deselect()
{
	ScrollList::deselect();
	_player->unSelectLord();
}

void ScrollLord::slot_clicked( int num )
{
	if( _selected == int( num + _current ) ) {
		emit sig_lord();
	} else {
		select( num );
	}
}

//
// ----- ScrollBase -----
//

ScrollBase::ScrollBase( QWidget * parent, const char * name )
	: ScrollList( parent, name )
{

}

void ScrollBase::reinit()
{
	_selected = -1;
	for( uint i = 0; i < 5; i++ ) {
		if( _current + i < _player->numBase() ) {
			_buttons[i]->setEnabled( true );
			_buttons[i]->setPixmap( *_player->getBaseSmallPixmap( _current+i ) );
			if( _player->getBase( _current + i ) == _player->getSelectedBase() ) {
				_buttons[i]->setBackgroundColor( yellow );
				_selected = _current + i;
			}
		} else {
			_buttons[i]->setBackgroundColor( blue );
			_buttons[i]->setEnabled( false );
		}
	}	
}

void ScrollBase::select( int num )
{
	_player->unSelectLord();
	_player->unSelectBase();
	
	ScrollList::select( num );

	if( _selected < (int)_player->numBase() ) {
		_player->setSelectedBase( _player->getBase( _selected ) );
		emit sig_baseSelected();
	} else {
		deselect();
	}
}

void ScrollBase::deselect()
{
	ScrollList::deselect();
	_player->unSelectBase();
}

void ScrollBase::slot_clicked( int num )
{
	if( _selected == int( num + _current ) ) {
		emit sig_base();
	} else {
		select( num );
	}	
}
