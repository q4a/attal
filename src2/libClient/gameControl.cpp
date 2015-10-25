/****************************************************************
**
** Attal : Lords of Doom
**
** gameConrol.cpp
** Widget on the right, controlling the game
**
** Version : $Id: gameControl.cpp,v 1.34 2009/09/05 17:51:21 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QApplication>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>

// application specific includes
#include "conf.h"

#include "libCommon/attalSettings.h"

#include "libClient/attalButton.h"
#include "libClient/imageTheme.h"

GameControl::GameControl( QWidget * parent, Player * player )
	: QWidget( parent )
{
	_player = player;
	_cen = new CentralControl( this, player );

	QHBoxLayout * lay = new QHBoxLayout( this );
	lay->addWidget( _cen );
	lay->addStretch( 1 );
	lay->activate();

	connect( _cen, SIGNAL( sig_endTurn() ), SIGNAL( sig_endTurn() ) );
	connect( _cen, SIGNAL( sig_nextLord() ), SIGNAL( sig_nextLord() ) );
	connect( parent, SIGNAL( sig_enableGame( bool ) ), SIGNAL( sig_enableGame(bool) ) );
	connect( _cen, SIGNAL( sig_gotoLord() ), SIGNAL( sig_gotoLord() ) );
}

void GameControl::reinit()
{
}

void GameControl::selectFirst()
{
	if( ! _player->selectFirstLord() ) {
		_player->selectFirstBase();
	}

	reinit();
}

void GameControl::disableGame()
{
	_cen->disableGame();
}

void GameControl::enableGame()
{
	_cen->enableGame();
}

void GameControl::slot_baseSelected()
{
}

void GameControl::slot_lordSelected()
{
}

//
// ----- CentralControl -----
//

CentralControl::CentralControl( QWidget * parent, Player * player )
	:QWidget( parent )
{
	_player = player;
	
	_pbNext = new QPushButton( this );
	_pbNext->setFixedSize( 70, 25 );
	_pbNext->setText( tr("Next Lord") );
	
	QPushButton * pbGoto = new QPushButton( this );
	pbGoto->setFixedSize( 70, 25 );
	pbGoto->setText( tr("Goto") );

	_pbTurn = new QPushButton( this );
	_pbTurn->setFixedSize( 70, 25 );
	_pbTurn->setText( tr("End Turn") );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->setMargin( 3 );
	layout->setSpacing( 5 );

	layout->addWidget( _pbNext );
	layout->addWidget( pbGoto );
	layout->addWidget( _pbTurn );

	layout->activate();

	connect( _pbNext, SIGNAL( clicked() ), parent,  SIGNAL( sig_nextLord() ) );
	connect( _pbTurn, SIGNAL( clicked() ), SIGNAL( sig_endTurn() ) );
	connect( pbGoto, SIGNAL( clicked() ), SIGNAL( sig_gotoLord() ) );

	setEnabled( false );
	//disableGame();
	connect( parent, SIGNAL( sig_enableGame( bool ) ), SLOT( setEnabled(bool) ) );
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

ScrollList::ScrollList( bool horizontal, uint nbItem, QWidget * parent, Player * player )
	: QWidget( parent ),
	  _player( player ),
	  _current( 0 ),
	  _selected( -1 ),
	  _horizontal( horizontal ),
	  _nbItem( nbItem )
{
	QPushButton * pbDown = new QPushButton( this );
	QPushButton * pbUp = new QPushButton( this );
	QBoxLayout * layout;
	
	if( _horizontal ) {
		pbUp->setIcon( QIcon(QPixmap(  *ImageTheme.getWidgetPixmap( ARROW_LEFT ) ) ));
		pbUp->setFixedSize( 9, 50 );
		//pbUp->move( 265, 0 );
		pbDown->setIcon( QIcon(QPixmap(  *ImageTheme.getWidgetPixmap( ARROW_RIGHT) )));
		pbDown->setFixedSize( 9, 50 );
		//pbDown->move( 0, 0 );
		layout = new QHBoxLayout( this );
		setFixedSize( (_nbItem * 55) + 23, 60 );
	} else {
		pbUp->setIcon( QIcon(QPixmap(  *ImageTheme.getWidgetPixmap( ARROW_UP ) )));
		pbUp->setFixedSize( 50, 9 );
		//pbUp->move( 0, 0 );
		pbDown->setIcon( QIcon(QPixmap(  *ImageTheme.getWidgetPixmap( ARROW_DOWN ) )));
		pbDown->setFixedSize( 50, 9 );
		//pbDown->move( 0, 265 );
		layout = new QVBoxLayout( this );
		setFixedSize( 60, (_nbItem * 55) + 23 );
	}

	layout->addSpacing( 5 );
	layout->addWidget( pbUp );
	
	QSignalMapper * sigmap = new QSignalMapper( this );
	_buttons = new AttalButton * [ _nbItem ];
	
	for( uint i = 0; i < _nbItem; i++ ) {
	        _buttons[i] = new AttalButton( this );
		_buttons[i]->setFixedSize( 50, 40 );
		/*if( _horizontal ) {
			_buttons[i]->move(  (i*50) + 10, 5 );
		} else {
			_buttons[i]->move(  0, 10 + (i*50) );
		}*/
		layout->addWidget( _buttons[ i ] );
		sigmap->setMapping( _buttons[ i ], i );
		connect( _buttons[i], SIGNAL( clicked() ), sigmap, SLOT( map() ) );
	}
	
	layout->addWidget( pbDown );
	layout->addSpacing( 5 );

	setEnabled( false );

	connect( parent, SIGNAL( sig_enableGame( bool ) ), SLOT( setEnabled( bool ) ) );
	connect( sigmap, SIGNAL( mapped( int ) ), this, SLOT( slot_clicked( int ) ) );
	connect( pbDown, SIGNAL( clicked() ), SLOT( slot_down() ) );
	connect( pbUp, SIGNAL( clicked() ), SLOT( slot_up() ) );
}

ScrollList::~ScrollList( )
{
	for( uint i = 0; i < _nbItem; i++ ) {
		if( _buttons[ i ]) {
			_buttons[i] = new AttalButton( this );
		}
	}
	delete _buttons;
}

void ScrollList::enableGame()
{
	setEnabled( true );
}

void ScrollList::slot_reinit()
{
	reinit();
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
	if( ( _current + _nbItem - 1 ) < getListCount() ) {
		_current++;
		reinit();
	}	
}

void ScrollList::select( int num )
{
	deselect();
	/// XXX: mettre cadre
	_selected = num + _current;
	_buttons[num]->setBackgroundColor( Qt::yellow );
}

void ScrollList::deselect()
{
	if( _selected != -1 ) {
		_selected = -1;
		for( uint i = 0; i < _nbItem; i++ ) {
			_buttons[i]->setBackgroundColor( Qt::blue );
		}
	}
}

void ScrollList::repaintButtons(uint num)
{
	uint i;
	uint len = 23;
	for( i = 0; i < num; i++ ) {
		_buttons[i]->show();
		len += 55;
	}
	if( num < _nbItem ) {
		for( i = num; i < _nbItem; i++ ) {
			_buttons[i]->hide();
			len += 5;
		}
	}

	if( _horizontal ) {
		setFixedSize( len, 60 );
	} else {
		setFixedSize( 60, len );
	}
}

//
// ----- ScrollLord -----
//

ScrollLord::ScrollLord( bool horizontal, uint nbItem, QWidget * parent, Player * player )
	: ScrollList( horizontal, nbItem, parent, player )
{
	connect( parent, SIGNAL( sig_lordReinit() ), SLOT( slot_reinit() ) );
}

void ScrollLord::reinit()
{
	if( _player ) {
		_lordList.clear();
		uint i;
	
		for( i = 0; i < _player->numLord(); i++ ) {
			GenericLord * lord = _player->getLord( i );
			if( lord && lord->isVisible() ) {
				_lordList.append( lord );
			}
		}
	
		_selected = -1;
		for( i = 0; i < _nbItem; i++ ) {
			if( (int)(_current + i) < _lordList.count() ) {
				_buttons[i]->setEnabled( true );
				_buttons[i]->setLord( _lordList.at( _current + i )->getId() );
				if( _player->getSelectedLord() && ( _lordList.at( _current + i ) == _player->getSelectedLord() ) ) {
					_buttons[i]->setBackgroundColor( Qt::yellow );
					_selected = _current + i;
				}
			} else {
				_buttons[i]->setBackgroundColor( Qt::blue );
				_buttons[i]->setEnabled( false );
				/** memory leak? i don't know, if true is little and correct a big graphic bug (if qt will have a function to
					unselect icon will use that function **/
				_buttons[i]->setIcon( QIcon() );
			}
		}
	}
}

void ScrollLord::select( int num )
{
	if( _player ) {
		emit sig_unSelectLord();
		_player->unSelectBase();

		ScrollList::select( num );

		if( _selected < (int)_lordList.count() ) {
			emit sig_lordSelected( _lordList.at( _selected ) );
		} else {
			deselect();
		}
	}
}

void ScrollLord::deselect()
{
	ScrollList::deselect();
	emit sig_unSelectLord();
}

void ScrollLord::slot_clicked( int num )
{
	if( _selected == int( num + _current ) ) {
		emit sig_lord();
	} else {
		select( num );
	}
}

uint ScrollLord::getListCount()
{
	return _lordList.count();
}

//
// ----- ScrollBase -----
//

ScrollBase::ScrollBase( bool horizontal, uint nbItem, QWidget * parent, Player * player )
	: ScrollList( horizontal, nbItem, parent, player )
{
	connect( parent, SIGNAL( sig_baseReinit() ), SLOT( slot_reinit() ) );
}

void ScrollBase::reinit()
{
	if( _player ) {
		_selected = -1;
		for( uint i = 0; i < _nbItem; i++ ) {
			if( _current + i < _player->numBase() ) {
				_buttons[i]->setEnabled( true );
				_buttons[i]->setBase( _player->getBase( _current+i )->getRace() );
				if( _player->getBase( _current + i ) == _player->getSelectedBase() ) {
					_buttons[i]->setBackgroundColor( Qt::yellow );
					_selected = _current + i;
				}
			} else {
				_buttons[i]->setBackgroundColor( Qt::blue );
				_buttons[i]->setEnabled( false );
				_buttons[i]->setIcon( QIcon() );
			}	
		}
	}	
}

void ScrollBase::select( int num )
{
	if( _player ) {
		emit sig_unSelectLord();
		_player->unSelectBase();
		
		ScrollList::select( num );
	
		if( _selected < (int)_player->numBase() ) {
			_player->setSelectedBase( _player->getBase( _selected ) );
			emit sig_baseSelected();
		} else {
			deselect();
		}
	}
}

void ScrollBase::deselect()
{
	ScrollList::deselect();
	if( _player ) {
		_player->unSelectBase();
	}
}

void ScrollBase::slot_clicked( int num )
{
	if( _selected == int( num + _current ) ) {
		emit sig_base();
	} else {
		select( num );
	}	
}

uint ScrollBase::getListCount()
{
	if( _player ) {
		return _player->numBase();
	} else {
		return 0;
	}
}

