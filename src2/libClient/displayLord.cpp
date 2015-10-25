/****************************************************************
**
** Attal : Lords of Doom
**
** displayLord.cpp
** show infos about Lord
**
** Version : $Id: displayLord.cpp,v 1.43 2009/10/28 11:54:47 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 24/08/2000
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

#include "displayLord.h"

 
// generic include files

// include files for QT
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>

// application specific includes
#include "conf.h"

#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/attalSocket.h"

#include "libClient/attalButton.h"
#include "libClient/graphicalArtefact.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/lord.h"
#include "libClient/player.h"
#include "libClient/unitExchange.h"
#include "libClient/widget.h"

//
// ----- DisplayLordTab -----
//

DisplayLordTab::DisplayLordTab( Player * player, QWidget * parent )
: 	QDialog( parent ),
	_player( player )
{
	QWidget * panel = new QWidget( this );
	panel->setFixedHeight( 80 );

	_photo = new Icon( panel );
	_photo->move( 10, 10 );
	
	_title = new QLabel( panel );
	_title->setAlignment( Qt::AlignCenter );
	_title->move( 90, 20 );
		
	_tabGeneral = new DisplayLordTabGeneral( player );
	_tabUnits = new DisplayLordTabUnits( player );
	_tabArtefacts = new DisplayLordTabArtefacts( player );

	_tabs = new QTabWidget( this );
	_tabs->addTab( _tabGeneral, tr( "General" ) );
	_tabs->addTab( _tabUnits, tr( "Units" ) );
	_tabs->addTab( _tabArtefacts, tr( "Artefacts" ) );

	_listPanel = new DisplayListPanel( player, this );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->addWidget( panel );
	layV1->addWidget( _tabs, 1 );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layV1, 1 );
	layout->addWidget( _listPanel );
	layout->activate();

	connect( _listPanel, SIGNAL( sig_lord( int ) ), SLOT( slot_lord( int ) ) );
	connect( _listPanel, SIGNAL( sig_quit() ), SLOT( accept() ) );
	connect( parent, SIGNAL( sig_updateWidget() ), SLOT( reupdate() ) );
	connect( _tabUnits, SIGNAL( sig_sendExchangeUnitSplit(GenericLord * ,int, int, int, int) ), parent , SLOT( slot_sendExchangeUnitSplit(GenericLord * ,int, int, int, int) ) );
	connect( _tabUnits, SIGNAL( sig_sendExchangeUnit( GenericLord * , int , GenericLord * , int ) ), parent , SLOT ( slot_sendExchangeUnit(  GenericLord * , int , GenericLord * , int ))); 
	connect( _tabUnits, SIGNAL( sig_sendLordUnit( GenericLord * , int, int ) ), parent, SLOT( slot_sendLordUnit( GenericLord *, int, int ) ) );
}


void DisplayLordTab::exec()
{
	reinit();
	QDialog::exec();
}

void DisplayLordTab::reinit()
{
	Lord * aLord = ( Lord * )_player->getSelectedLord();

	if( aLord ) {
		_photo->setPixmap( *ImageTheme.getLordPixmap( aLord->getId() ) );
		QString title;
		title.sprintf( "Lord %s\nLevel %d of %s", qPrintable( aLord->getName() ),
				aLord->getCharac( LEVEL ), qPrintable( aLord->getCategoryName() ) );
		_title->setText( title );	
		FIXEDSIZE( _title );
	}

	_tabGeneral->reinit();
	_tabUnits->reinit();
	_tabArtefacts->reinit();
	_listPanel->reinit();
}

void DisplayLordTab::reupdate()
{
	_tabGeneral->reinit();
	_tabUnits->reinit();
	_tabArtefacts->reinit();

	_listPanel->reupdate();
}


void DisplayLordTab::slot_lord( int num )
{
	_player->setSelectedLord( _player->getLord( num ) );
	reinit();
}

//
// ----- DisplayLordTabGeneral -----
//

DisplayLordTabGeneral::DisplayLordTabGeneral( GenericPlayer * player, QWidget * parent )
: QWidget( parent )
{
	_player = player;
	
	_experience = new InfoWidget( this );
	_experience->setIcon( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_EXPERIENCE ) ));
	_luck = new InfoWidget( this );
	_luck->setIcon( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_LUCK ) ));
	_attack = new InfoWidget( this );
	_attack->setIcon( QPixmap( *ImageTheme.getWidgetPixmap( ICO_ATTACK ) ) );
	_defense = new InfoWidget( this );
	_defense->setIcon( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_DEFENSE ) ) );
	_move = new InfoWidget( this );
	_move->setIcon( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_MOVE ) ));
	_vision = new InfoWidget( this );
	_vision->setIcon( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_DEFENSE ) ) );

	QGridLayout * layout = new QGridLayout( this );
	layout->setColumnStretch( 0, 1 );
	layout->setColumnStretch( 1, 1 );
	layout->addWidget( _experience, 0, 0 );
	layout->addWidget( _luck, 0, 1 );
	layout->addWidget( _attack, 1, 0 );
	layout->addWidget( _defense, 1, 1 );
	layout->addWidget( _move, 2, 0 );
	layout->addWidget( _vision, 2, 1 );
	layout->setRowStretch( 3, 1 );
	layout->activate();	
}

QString DisplayLordTabGeneral::textCharac( GenericLord * lord, LordCharac charac )
{
	return getCharacName( charac ) + ": " + QString::number( lord->getCharac( charac ) );
}

void DisplayLordTabGeneral::reinit()
{
	GenericLord * lord = _player->getSelectedLord();

	if( lord ) {
		//GenericLordModel * model = DataTheme.lords.at( lord->getId() );
	
		int nextLevel = DataTheme.lordExperience.getLevel( lord->getCharac( LEVEL )  + 1 );
		_experience->setText( textCharac( lord, EXPERIENCE ) + "\n( next " + QString::number(nextLevel) + " )" );
		_luck->setText( textCharac( lord, LUCK ) );
		_attack->setText( textCharac( lord, ATTACK ) );
		_defense->setText( textCharac( lord, DEFENSE ) );
		_move->setText( textCharac( lord, MOVE ) + " / " + QString::number( lord->getCharac( MAXMOVE ) ) );
		_vision->setText( textCharac( lord, VISION ) );
	}
}

//
// -----  DisplayLordTabUnits -----
//

DisplayLordTabUnits::DisplayLordTabUnits( GenericPlayer * player, QWidget * parent )
: QWidget( parent )
{
	_player = player;
	_select = -1;
	_exchange = false;
	_unitExchange = 0;

	_butExchange = new AttalButton( this );
	_butExchange->setEnabled( false );
	_butExchange->setText( tr( "Split unit" ) );

	QSignalMapper * sigmap = new QSignalMapper( this );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _butExchange );

	for( uint i = 0; i < MAX_UNIT; i++ ) {
		InfoWidget * unit = new InfoWidget( this );
		_units.append( unit );
		layout->addWidget( unit );
		sigmap->setMapping( unit, i );
		connect( unit, SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}

	layout->addStretch();
	layout->activate();

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_unitClicked( int ) ) );
	connect( _butExchange, SIGNAL( clicked() ), SLOT( slot_exchange() ) );
}

void DisplayLordTabUnits::reinit()
{
	GenericLord * lord = _player->getSelectedLord();
	GenericFightUnit * unit;
	
	if( lord ) {
		for( uint i = 0; i < MAX_UNIT; i++ ) {
			unit = lord->getUnit( i );
			if( unit ) {
				_units.at( i )->setUnit( unit );
			} else {
				_units.at( i )->clear();
			}
		}
	}
}

void DisplayLordTabUnits::slot_exchange()
{
	_exchange = true;
	_butExchange->setText( tr( "Now click on a free cell or a unit of same type" ) );
}

void DisplayLordTabUnits::quitExchange()
{
	deselectUnit();
	_exchange = false;
	_butExchange->setText( tr( "Split unit" ) );
}

void DisplayLordTabUnits::selectUnit( int num )
{
	_select = num;
	_butExchange->setEnabled( true );
}

void DisplayLordTabUnits::deselectUnit()
{
	_select = -1;
	_butExchange->setEnabled( false );
}

void DisplayLordTabUnits::exchangeUnit( int num )
{	
	GenericLord * lord = _player->getSelectedLord();

	if( _select == num ) {
		/// nothing to do
	} else if (lord) {
		if( lord->getUnit( num ) ) {
			if( lord->getUnit( _select )->getCreature() == lord->getUnit( num )->getCreature() ) {
				if( ! _unitExchange ) {
					_unitExchange = new UnitExchange( this );
				}
				_unitExchange->setMin( 0 );
				_unitExchange->setMax( lord->getUnit( _select )->getNumber() + lord->getUnit( num )->getNumber() );
				_unitExchange->setUnits( lord->getUnit( _select ), lord->getUnit( num ) );
				if( _unitExchange->exec() ) {
					emit sig_sendExchangeUnitSplit( lord, _select, _unitExchange->getLeftValue() , num, _unitExchange->getRightValue() );
				}
			}
		} else {
			if( ! _unitExchange ) {
				_unitExchange = new UnitExchange( this );
			}
			_unitExchange->setMin( 0 );
			_unitExchange->setMax( lord->getUnit( _select )->getNumber() );
			_unitExchange->setUnits( lord->getUnit( _select ), 0 );
			if( _unitExchange->exec() ) {
				emit sig_sendExchangeUnitSplit( lord, _select, _unitExchange->getLeftValue() , num, _unitExchange->getRightValue() );
			}
		}	
	}
	quitExchange();	
}

void DisplayLordTabUnits::slot_unitClicked( int num )
{
	TRACE("DisplayLordTabUnits::slot_unitClicked( %d )", num);

	GenericLord * lord = _player->getSelectedLord();

	if( lord ) {
		if( _exchange ) {
			exchangeUnit( num );
		} else {
			if( _select == num ) {
				QMessageBox msb( tr("Unit"), tr("Do you want destroy this unit ?"), QMessageBox::Warning, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
				if ( msb.exec() == QMessageBox::Yes ) {
					emit sig_sendLordUnit( lord, num , 0 );
				}
				reinit();
				/// XXX: show unit charact
				deselectUnit();
				return;
			}

			if( _select == -1 ) {
				if( lord->getUnit( num ) ) {
					selectUnit( num );
				}
			} else {
				emit sig_sendExchangeUnit( lord, _select, lord, num );
				deselectUnit();
			}
		}
		reinit();
	}
}

//
// ----- DisplayListPanel -----
//

DisplayListPanel::DisplayListPanel( GenericPlayer * player, QWidget * parent, const char * /* name */ )
	: QFrame( parent )
{
	_player = player;
	
  	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setLineWidth( 1 );
	setFixedWidth( 60 );

	QLabel * emblem = new QLabel( this );
	emblem->setFixedSize( 58, 58 );
	emblem->setPixmap( * ImageTheme.getFlag( player->getTeamId() ) ); 
	
	_listLord = new DisplayListLord( _player, this );

	AttalButton * butOk = new AttalButton( this, AttalButton::BT_OK );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setAlignment( Qt::AlignHCenter );
	layout->addWidget( emblem );
	layout->addSpacing( 5 );
	layout->addWidget( _listLord, 1 );
	//layout->addStretch( 1 );
	layout->addSpacing( 5 );
	layout->addWidget( butOk );
	layout->addSpacing( 5 );
	layout->setAlignment( butOk, Qt::AlignHCenter );
	layout->activate();

	connect( butOk, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
	connect( _listLord, SIGNAL( sig_lord( int ) ), SIGNAL( sig_lord( int ) ) );
}

void DisplayListPanel::reinit()
{
	_listLord->reinit();
	update();
}

void DisplayListPanel::reupdate()
{
	_listLord->reupdate();
	update();
}

//
// ----- DisplayListLord -----
//

DisplayListLord::DisplayListLord( GenericPlayer * player, QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	_player = player;
	_sigmap = new QSignalMapper( this );
	setFixedWidth( 55 );
	connect( _sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_lord( int ) ) );
}

DisplayListLord::~DisplayListLord()
{
	while( ! _listBut.isEmpty() ) {
		delete _listBut.takeFirst();
	}
}

void DisplayListLord::reupdate()
{
	/// XXX: add border to selected unit
}

void DisplayListLord::reinit()
{
	AttalButton * button;

	while( ! _listBut.isEmpty() ) {
		button = _listBut.takeFirst();
		_sigmap->removeMappings( button );
		delete button;
	}
	
	for( uint i = 0; i < _player->numLord(); i++ ) {
		button = new AttalButton( this, AttalButton::BT_LORD );
		button->move( 3, 5 + (50 * i) );
		button->setLord( _player->getLord( i )->getId() );
		button->show();
		_sigmap->setMapping( button, i );
		connect( button, SIGNAL( clicked() ), _sigmap, SLOT( map() ) );
		_listBut.append( button );
	}
	
	setMinimumHeight( _player->numLord() * 50 );
}

