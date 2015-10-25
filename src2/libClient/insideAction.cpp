/****************************************************************
**
** Attal : Lords of Doom
**
** insideAction.cpp
** Dialogs for actions of inside buildings
**
** Version : $Id: insideAction.cpp,v 1.52 2008/12/31 18:22:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo 
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
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSignalMapper>
#include <QTimer>
#include <QVBoxLayout>

// application specific include files
#include "libClient/attalButton.h"
#include "libClient/displayCreature.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/genericRessources.h"

//
// ----- InsideActionAllBuildings -----
//

InsideActionAllBuildings::InsideActionAllBuildings( QWidget * parent, GenericBase * base, GenericPlayer * player, bool ok )
	: QDialog( parent, Qt::Dialog )
{
	_base = base;
	_player = player;

	setWindowTitle( tr( "Buildings" ) );

	_buildings = new BuildingsView( this, base, player );

	_scroll = new QScrollArea( this );
	_scroll->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	_scroll->setWidget( _buildings );
	_scroll->setMinimumWidth( _buildings->width() + 40 );
	_scroll->setWidgetResizable( true );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _scroll, 1 );
	layout->addSpacing( 5 );
	
	if( ok ) {
		
		AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );
		
		layout->addWidget( pbOk );
		layout->setAlignment( pbOk, Qt::AlignHCenter );
		connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	}

	updateView();
	
	layout->activate();

	connect( _buildings, SIGNAL( sig_buy( int ) ), SLOT( slot_buy( int ) ) );
	connect( _buildings, SIGNAL( sig_sell( int ) ), SLOT( slot_sell( int ) ) );
	connect( parent, SIGNAL( sig_castle() ), _buildings , SLOT( updateView() ) );
	connect( this, SIGNAL( sig_requestBuilding( int, bool ) ), parent , SLOT( slot_requestBuilding( int, bool) ) );
}

void InsideActionAllBuildings::slot_buy( int building )
{
	// XXX: check if 'buy' is possible...
	if( _player ) {
		if( _player->canBuy( DataTheme.bases.at( _base->getRace() )->getBuildingModel( building ) ) ) {
			emit sig_requestBuilding( building, true );
		} else {
			QMessageBox::warning( this, tr("Can't buy"), tr("Not enough ressources to buy this building") );
		}
	} else {
		logEE( "Player not initialized for InsideActionAllBuildings" );
	}
}

void InsideActionAllBuildings::slot_sell( int building )
{	
	if( _player ) {	
		QMessageBox msb( tr( "Building" ), tr( "Are you sure to sell this building?" ), QMessageBox::Warning, QMessageBox::Yes | 			QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
		if( msb.exec() == QMessageBox::Yes){
			emit sig_requestBuilding( building, false );
		}
	}
}

void InsideActionAllBuildings::updateView()
{
	_buildings->updateView();
}
	
//
// ----- BuildingsView -----
//

BuildingsView::BuildingsView( QWidget * parent, GenericBase * base, GenericPlayer * player )
	:QWidget( parent )
{
	_base = base;
	_player = player;
	QSignalMapper * sigmap = new QSignalMapper( this );
	QSignalMapper * sigmap2 = new QSignalMapper( this );
	GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
	_layout = new QVBoxLayout( this );

	_panels = new BuildingPanel * [baseModel->getBuildingCount()];
	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		InsideBuildingModel * building = baseModel->getBuildingModel( i );
		_panels[i] = new BuildingPanel( building, this );
		_layout->addWidget( _panels[i] );
		if( !building->getAction() || building->getAction()->getType() == INSIDE_NONE || _base->isForbidden( i ) ) {
				_panels[ i ]->setVisible(false);
			} else {
				_panels[ i ]->setVisible(true);
			}
		sigmap->setMapping( _panels[i], i );
		sigmap2->setMapping( _panels[i], i );
		connect( _panels[i], SIGNAL( sig_buy() ), sigmap, SLOT( map() ) );
		connect( _panels[i], SIGNAL( sig_sell() ), sigmap2, SLOT( map() ) );
	}

	updateView();

	_layout->activate();
	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_buy( int ) ) );
	connect( sigmap2, SIGNAL( mapped( int ) ), SIGNAL( sig_sell( int ) ) );
}

BuildingsView::~BuildingsView()
{
	GenericBaseModel * baseModel = DataTheme.bases.at( _base->getRace() );
	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		delete _panels[i];
	}
	delete [] _panels;
}

void BuildingsView::buy( int building )
{
	if( _panels[building] ) {
		_panels[building]->setBuyable( false );
		_panels[building]->setSellable( true );
	}
}

void BuildingsView::updateView()
{
	TRACE("BuildingsView::updateView");
	
	GenericBaseModel * baseModel = DataTheme.bases.at( _base->getRace() );

	_layout->setEnabled( false );

	// list the bought buildings first
	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		if( !_base->hasBuildingType( i ) ) {
			_layout->addWidget( _panels[i] );
		}
	}
	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		if( _base->hasBuildingType( i ) ) {
			_layout->addWidget( _panels[i] );
		}
	}

	for( uint i = 0; i < baseModel->getBuildingCount(); i++ ) {
		InsideBuildingModel * building = baseModel->getBuildingModel( i );
		if( _base->hasBuildingType( i ) ) {
			_panels[i]->setBuyable( false ); 
			_panels[i]->setSellable( true );
			if( _base->getState( GenericBase::BASE_SELL ) ) {
				_panels[i]->disable( false );
			} else {
				_panels[i]->disable( true );
			}
		} else {
			if( _player ) {
				if (_player->canBuy( building ) && _base->canBuildBuilding( building )  && _base->getState( GenericBase::BASE_BUY ) ) {
					_panels[i]->disable( false );
				} else {
					_panels[i]->disable( true );
				}
			} else {
				_panels[i]->disable( false );
			}
			_panels[i]->setBuyable( true );
			_panels[i]->setSellable( false );
		} 
	}
	
	_layout->setEnabled( true );
}

//
// ----- BuildingPanel -----
//

BuildingPanel::BuildingPanel( InsideBuildingModel * model, QWidget * parent )
	: QFrame( parent )
{
	_model = model;
	_buyEnabled = true;
	
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	// XXX: add pic of building first...
	QLabel * smallPic = new QLabel( this );
	smallPic->setPixmap( ImageTheme.getInsideBuildingView( _model->getRace() , _model->getLevel()) );
	smallPic->setFixedSize( 100, 80 );

	QLabel * thename = new QLabel( this );
	thename->setText( tr("Name: ") + model->getName() );
	FIXEDSIZE( thename );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	layH1->addWidget( thename );
	layH1->addStretch( 1 );
	
	QLabel * desc = new QLabel( this );
	desc->setText( model->getBuildDescActions() );
	FIXEDSIZE( desc );
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	layH2->addWidget( desc );
	layH2->addStretch( 1 );
		
	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->setSpacing( 5 );
	layV1->addLayout( layH1 );
	layV1->addLayout( layH2 );
	layV1->addStretch( 1 );
	
	QPushButton * pbInfo = new QPushButton( this );
	pbInfo->setText( tr( "Info" ) );
	FIXEDSIZE( pbInfo );
	
	_pbBuySell = new QPushButton( this );
	updateBuySell();
	
	QVBoxLayout * layV2 = new QVBoxLayout();
	layV2->setMargin( 5 );
	layV2->setSpacing( 5 );
	layV2->addStretch( 1 );
	layV2->addWidget( pbInfo );
	layV2->addStretch( 1 );
	layV2->addWidget( _pbBuySell );
	layV2->addStretch( 1 );
	
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( smallPic );
	layout->addLayout( layV1 );
	layout->addStretch( 1 );
	layout->addLayout( layV2 );
	layout->activate();
	
	connect( pbInfo, SIGNAL( clicked() ), SLOT( slot_info() ) );
	connect( _pbBuySell, SIGNAL( clicked() ), SLOT( slot_buysell() ) );
}

void BuildingPanel::slot_info()
{
	QString text = "";

	text = _model->getBuildDescActions();
	QMessageBox::information( this, tr( "Building informations" ), text, tr( "Ok" ) );
}

void BuildingPanel::slot_buysell()
{
	if( _buyEnabled ) {
		emit sig_buy();
	} else {
		emit sig_sell();
	}
}

void BuildingPanel::disable( bool state )
{
	_pbBuySell->setDisabled( state );
}

void BuildingPanel::setBuyable( bool state )
{
	_buyEnabled = state;
	updateBuySell();
}

void BuildingPanel::setSellable( bool state )
{
	_buyEnabled = ! state;
	updateBuySell();
}

void BuildingPanel::updateBuySell()
{
	if( _buyEnabled ) {
		_pbBuySell->setText( tr( "Buy" ) );
	} else {
		_pbBuySell->setText( tr( "Sell" ) );
	}
	FIXEDSIZE( _pbBuySell );
}

//
// ----- InsideActionSomeCreature -----
//
/*
InsideActionSomeCreature::InsideActionSomeCreature( int race, int level, QWidget * parent, const char * name )
	: QDialog( parent, Qt::Dialog )
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

DisplayCreatureBase::DisplayCreatureBase( QWidget * parent, GenericBase * base , GenericPlayer * player )
	: QDialog( parent, Qt::Dialog )
{

	TRACE("DisplayCreatureBase::DisplayCreatureBase( QWidget * parent %p, GenericBase * base %p , GenericPlayer * player %p)", parent, base, player   );

	_player = player;
	_base = base;

	setWindowTitle( tr("Creatures") );

	QGridLayout * layout = new QGridLayout( this );
	layout->setSizeConstraint( QLayout::SetFixedSize );
	layout->setSpacing( 10 );

	QSignalMapper * sigmap = new QSignalMapper( this );
	for( int i = 0; i < DataTheme.creatures.getRace(_base->getRace())->count();i++ ) {
		_buy[i] = new DisplayCreature( this );
		_buy[i]->setCreature( _base->getRace(), i , base);

		layout->addWidget( _buy[i], i / 3, i % 3 );

		sigmap->setMapping( _buy[i], i );
		connect( _buy[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}
	slot_reinit();

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_buyUnit( int ) ) );
	connect( parent, SIGNAL( sig_castle() ), SLOT( slot_reinit() ) );
	connect( this, SIGNAL( sig_baseUnitBuy( Creature * , int  ) ), parent, SLOT( slot_baseUnitBuy( Creature * , int  ) ) );
}

void DisplayCreatureBase::slot_buyUnit( int num )
{
	BuyCreature es( this , _player, _base, _buy[num]->getCreature() );
	es.exec();
	QTimer::singleShot( 0, this, SLOT( slot_reinit() ) ); // HACK due to the event loop
}

void DisplayCreatureBase::slot_baseUnitBuy( Creature * creature, int num )
{
	emit sig_baseUnitBuy( creature, num );
}

void DisplayCreatureBase::slot_reinit()
{
	for( int i = 0; i < DataTheme.creatures.getRace(_base->getRace())->count();i++ ) {
		_buy[i]->setCreature( _base->getRace(), i , _base);
	}
	emit sig_castle();
}
