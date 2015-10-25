/****************************************************************
**
** Attal : Lords of Doom
**
** insideAction.cpp
** Dialogs for actions of inside buildings
**
** Version : $Id: insideAction.cpp,v 1.9 2002/09/20 23:56:29 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/05/2002
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

#include "insideAction.h"

// generic include files
// include files for QT
#include <qlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qscrollview.h>
#include <qsignalmapper.h>
// application specific include files
#include "client/displayCreature.h"
#include "client/gui.h"
#include "common/attalSocket.h"
#include "common/dataTheme.h"
#include "common/genericBase.h"
#include "common/genericInsideBuilding.h"
#include "common/genericPlayer.h"

extern QString IMAGE_PATH;
extern DataTheme DataTheme;

//
// ----- InsideActionAllBuildings -----
//

InsideActionAllBuildings::InsideActionAllBuildings( GenericBase * base, QWidget * parent, const char * name )
	:QDialog( parent, name, true )
{
	_base = base;
	_socket = 0;
	_player = 0;

	QVBoxLayout * layout = new QVBoxLayout( this );

	QScrollView * scroll = new QScrollView( this );
	scroll->setVScrollBarMode( QScrollView::AlwaysOn );
	BuildingsView * buildings = new BuildingsView( base, scroll->viewport() );
	scroll->addChild( buildings );
	layout->addWidget( scroll, 1 );
	//layout->addStretch( 1 );

	QHBoxLayout * layH = new QHBoxLayout();
	layH->addStretch( 1 );
	QPushButton * pbOk = createButtonOk( this );
	layH->addWidget( pbOk );
	layH->addStretch( 1 );
	layout->addSpacing( 5 );
	layout->addLayout( layH );

	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( buildings, SIGNAL( sig_buy( int ) ), SLOT( slot_buy( int ) ) );
	setFixedSize( 800, 600 );
}

void InsideActionAllBuildings::slot_buy( int building )
{
	// XXX: check if 'buy' is possible...
	if( _player && _socket ) {
		if( _player->canBuy( DataTheme.bases.at( _base->getRace() )->getBuildingModel( building ) ) ) {
			_socket->requestBuilding( _base, building );
		} else {
			QMessageBox::warning( this, "Can't buy", "Not enough ressources to buy this building" );
		}
	} else {
		logEE( "Player or socket not initialized for InsideActionAllBuildings" );
	}
}

//
// ----- BuildingsView -----
//

BuildingsView::BuildingsView( GenericBase * base, QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );
	GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
	QVBoxLayout * layout = new QVBoxLayout( this );

	_panels = new BuildingPanel * [baseModel->getBuildingCount()];
	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		_panels[i] = 0;
		if( ! base->isForbidden( i ) ) {
			InsideBuildingModel * building = baseModel->getBuildingModel( i );
			_panels[i] = new BuildingPanel( building, this );
			layout->addWidget( _panels[i] );
			sigmap->setMapping( _panels[i], i );
			connect( _panels[i], SIGNAL( sig_buy() ), sigmap, SLOT( map() ) );
			if( base->hasBuildingType( i ) ) {
				_panels[i]->setBuyable( false );
			} else {
				_panels[i]->setBuyable( true );
			}
		}
	}

	layout->activate();
	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_buy( int ) ) );
}

void BuildingsView::buy( int building )
{
	if( _panels[building] ) {
		_panels[building]->setBuyable( false );
	}
}

//
// ----- BuildingPanel -----
//

BuildingPanel::BuildingPanel( InsideBuildingModel * model, QWidget * parent, const char * name )
	:QFrame( parent, name )
{
	setFrameStyle( Box | Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );

	// XXX: add pic of building first...
	QLabel * pb = new QLabel( this );
	pb->setPixmap( IMAGE_PATH + "building/buildingPic.png" );
	pb->setFixedSize( 100, 80 );
	layout->addWidget( pb );

	QVBoxLayout * layV = new QVBoxLayout();
	layV->setMargin( 5 );
	layV->setSpacing( 5 );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	QLabel * thename = new QLabel( this );
	thename->setText( "Name: " + model->getName() );
	FIXEDSIZE( thename );
	layH1->addWidget( thename );
	layH1->addStretch( 1 );
	layV->addLayout( layH1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	QLabel * desc = new QLabel( this );
	desc->setText( "Description: " + model->getDescription() );
	FIXEDSIZE( desc );
	layH2->addWidget( desc );
	layH2->addStretch( 1 );
	layV->addLayout( layH2 );

	layV->addStretch( 1 );
	layout->addLayout( layV );
	layout->addStretch( 1 );

	_button = new QPushButton( this );
	_button->setText( "Buy" );
	FIXEDSIZE( _button );
	_button->hide();
	layout->addWidget( _button );
	layout->addSpacing( 5 );

	layout->activate();
	connect( _button, SIGNAL( clicked() ), SIGNAL( sig_buy() ) );
}

void BuildingPanel::setBuyable( bool state )
{
	if( state ) {
		_button->show();
	} else {
		_button->hide();
	}
}

//
// ----- InsideActionSomeCreature -----
//
/*
InsideActionSomeCreature::InsideActionSomeCreature( int race, int level, QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	QGridLayout * layout = new QGridLayout( this );
	DisplayCreature * cre = new DisplayCreature( this );
	cre->setCreature( race, level );
	layout->addWidget( cre, 0, 0 );
	layout->setRowStretch( 0, 1 );
	layout->setColStretch( 0, 1 );
	layout->activate();
}*/

//
// ----- DisplayCreatureBase -----
//

DisplayCreatureBase::DisplayCreatureBase( QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	QVBoxLayout * lay = new QVBoxLayout( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	QHBoxLayout * layH2 = new QHBoxLayout();
	QHBoxLayout * layH3 = new QHBoxLayout();
	QHBoxLayout * layH4 = new QHBoxLayout();

	QSignalMapper * sigmap = new QSignalMapper( this );
	for( int i = 0; i < 7; i++ ) {
		_buy[i] = new DisplayCreature( this );
		//_buy[i]->setCreature( i );
		sigmap->setMapping( _buy[i], i );
		connect( _buy[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}

	layH1->addWidget( _buy[0], 1 );
	layH1->addSpacing( 5 );
	layH1->addWidget( _buy[1], 1 );

	layH2->addWidget( _buy[2], 1 );
	layH2->addSpacing( 5 );
	layH2->addWidget( _buy[3], 1 );

	layH3->addWidget( _buy[4], 1 );
	layH3->addSpacing( 5 );
	layH3->addWidget( _buy[5], 1 );

	layH4->addStretch( 1 );
	layH4->addWidget( _buy[6], 2 );
	layH4->addStretch( 1 );

	lay->addLayout( layH1, 1 );
	lay->addSpacing( 5 );
	lay->addLayout( layH2, 1 );
	lay->addSpacing( 5 );
	lay->addLayout( layH3, 1 );
	lay->addSpacing( 5 );
	lay->addLayout( layH4, 1 );

	lay->activate();

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_buyUnit( int ) ) );
}

void DisplayCreatureBase::slot_buyUnit( int num )
{
	BuyCreature es( this );
	es.exec();
}
