/****************************************************************
**
** Attal : Lords of Doom
**
** market.cpp
** display market place
**
** Version : $Id: market.cpp,v 1.27 2008/07/10 23:26:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 14/01/2001
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

#include "market.h"
 
// generic include files
// include files for QT
#include <QLayout>
#include <QPushButton>
#include <QPixmap>
#include <QSignalMapper>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"
#include "libCommon/attalSocket.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"
#include "libClient/widget.h"

Market::Market( QWidget * parent, GenericPlayer * player  )
	: QDialog( parent, Qt::Dialog )
{
	_player = player;
	_prices = _player->getPriceMarket();
	
	setWindowTitle( tr("Marketplace") );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
		     
	QHBoxLayout * layH1 = new QHBoxLayout();
	_own = new DisplayResources( this );
	_own->setTitle( tr("Kingdom resources") );
	layH1->addWidget( _own, 1 );

	_other = new DisplayResources( this );
	_other->setTitle( tr("Available for trade") );
	layH1->addWidget( _other, 1 );
	
	layout->addLayout( layH1, 1 );
	
	_exchange = new ExchangeResources( this , player );
	layout->addWidget( _exchange );
	
	layout->setMargin( 2 );
	layout->activate();
	
	connect( _exchange, SIGNAL( sig_quit() ), SLOT( accept() ) );
	connect( _own, SIGNAL( sig_resource( int ) ), SLOT( slot_ownResource( int ) ) );
	connect( _other, SIGNAL( sig_resource( int ) ), SLOT( slot_otherResource( int ) ) );
	connect( parent, SIGNAL( sig_castle() ), SLOT( slot_reinit() ) );
	connect( _exchange, SIGNAL( sig_buy( int , int , int ) ), parent ,SLOT( slot_market( int, int ,int) ) );
}

void Market::slot_reinit()
{
	if(_player) {
		_own->setResources(_player);
	}
}

void Market::slot_ownResource( int num )
{
	_exchange->slot_ownResource( num );
	if(_player)
		_own->setResources(_player);
	_other->setPrices( _prices, num );
}

void Market::slot_otherResource( int num )
{
	_exchange->slot_otherResource( num );
}

/*!  Constructs an empty

*/

DisplayResources::DisplayResources( QWidget * parent, const char * /* name */)
	: QWidget( parent )
{
	uint nbResources = DataTheme.resources.count();

	QVBoxLayout * layout = new QVBoxLayout( this );
	_title = new Sentence( this );

	layout->addWidget( _title );
	layout->addStretch( 1 );

	_resources = new ResourceIcon * [ nbResources ];

	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < nbResources; i+=3 ) {
		if( nbResources-i == 1 ) {
			_resources[i] = new ResourceIcon( this );
			_resources[i]->setResource( i );
			layout->addWidget( _resources[i] );
			sigmap->setMapping( _resources[i], i );
			connect( _resources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
		} else if( nbResources-i == 2 ) {
			QHBoxLayout * layH = new QHBoxLayout();
			_resources[i] = new ResourceIcon( this );
			_resources[i]->setResource( i );
			layH->addStretch( 1 );
			layH->addWidget( _resources[i] );
			layH->addStretch( 1 );
			sigmap->setMapping( _resources[i], i );
			connect( _resources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			_resources[i+1] = new ResourceIcon( this );
			_resources[i+1]->setResource( i+1 );
			layH->addWidget( _resources[i+1] );
			layH->addStretch( 1 );
			sigmap->setMapping( _resources[i+1], i+1 );
			connect( _resources[i+1], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			layout->addLayout( layH );
		} else {
			QHBoxLayout * layH = new QHBoxLayout();

			_resources[i] = new ResourceIcon( this );
			_resources[i]->setResource( i );
			layH->addWidget( _resources[i] );
			sigmap->setMapping( _resources[i], i );
			connect( _resources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );

			_resources[i+1] = new ResourceIcon( this );
			_resources[i+1]->setResource( i+1 );
			layH->addWidget( _resources[i+1] );
			sigmap->setMapping( _resources[i+1], i+1 );
			connect( _resources[i+1], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );

			_resources[i+2] = new ResourceIcon( this );
			_resources[i+2]->setResource( i+2 );
			layH->addWidget( _resources[i+2] );
			sigmap->setMapping( _resources[i+2], i+2 );
			connect( _resources[i+2], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );

			layout->addLayout( layH );
		}
	}


	layout->activate();

	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_resource( int ) ) );
}

DisplayResources::~DisplayResources()
{
	uint nbResources = DataTheme.resources.count();
	for( uint i = 0; i < nbResources; i++ ) {
			delete _resources[i];
	}
	
	delete [] _resources;
}

void DisplayResources::setResources( GenericPlayer * player )
{
	uint nbResources = DataTheme.resources.count();
	for( uint i = 0; i < nbResources; i++ ) {
		_resources[i]->setValue( QString::number( player->getResourceList()->getValue( i ) ) );
	}
}

void DisplayResources::setPrices( PriceMarket * prices, uchar resource )
{
	QString text;
	ResourceModel * model = DataTheme.resources.get( resource );
	uint nbResources = DataTheme.resources.count();
	for( uint i = 0; i < nbResources; i++ ) {
		ResourceModel * model2 = DataTheme.resources.get( i );
		if( resource == i || model->isPreservable() || model2->isPreservable() ) {
			text = QString("n/a");
		} else {
			text = QString::number( prices->getResourceInResourceFloat(  i, resource ) , 'g', 3 );
		}
		_resources[i]->setValue( text );
	}
}

void DisplayResources::setTitle( QString title )
{
	_title->setText( title );
}

/*!  Constructs an empty

*/

ExchangeResources::ExchangeResources( QWidget * parent, GenericPlayer * player )
	: QWidget( parent )
{

	_player = player;
	_prices = _player->getPriceMarket();
	_resource = 0;
	_other = 0;

	setFixedHeight( 200 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	
	layH1->addStretch( 2 );
	_icoLeft = new ResourceIcon( this );
	layH1->addWidget( _icoLeft );
	layH1->addSpacing( 20 );
	
	_scroll = new QScrollBar( Qt::Horizontal, this );
	layH1->addWidget( _scroll, 2 );
	layH1->addSpacing( 20 );
	
	_icoRight = new ResourceIcon( this );
	layH1->addWidget( _icoRight );
	layH1->addStretch( 2 );
	
	layout->addLayout( layH1 );
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	
	layH2->addStretch( 1 );
	
	_butAll = new QPushButton( this );
	_butAll->setText( tr("All") );
	_butAll->setFixedSize( 50, 40 );
	layH2->addWidget( _butAll );
	layH2->addSpacing( 50 );
	
	_butBuy = new QPushButton( this );
	_butBuy->setText( tr("Buy") );
	_butBuy->setFixedSize( 50, 40 );
	layH2->addWidget( _butBuy );
	
	layH2->addStretch( 1 );

	AttalButton * butOk = new AttalButton( this, AttalButton::BT_OK );
	layH2->addWidget( butOk );
	
	layout->addLayout( layH2 );
	
	layout->activate();
	
	connect( butOk, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
	connect( _butAll, SIGNAL( clicked() ), SLOT( slot_all() ) );
	connect( _butBuy, SIGNAL( clicked() ), SLOT( slot_buy() ) );
	connect( _scroll, SIGNAL( valueChanged( int ) ), SLOT( setValue( int ) ) );
	
	clear();
}

void ExchangeResources::slot_all()
{
	int rvalue = _player->getResourceList()->getValue( _resource );
	setValue( rvalue );
}

void ExchangeResources::slot_buy()
{
	emit sig_buy(_resource, _other, _value);
	_scroll->setMaximum( 99 );
	clear();
}

void ExchangeResources::slot_ownResource( int num )
{
	ResourceModel * model = DataTheme.resources.get( num );
	if( model->isPreservable() ){
		return;	
	}

	_icoLeft->setResource( num );
	_isLeft = true;
	_resource = num;
	setValue( 0 );
	_icoRight->setValue( QString::number( 0 ) );
	if( _isRight ) {
		int rvalue = _player->getResourceList()->getValue( _resource );
		_scroll->setMaximum( rvalue );
		_scroll->setEnabled( true );
		_butBuy->setEnabled( true );
		_butAll->setEnabled( true );
	}
}

void ExchangeResources::slot_otherResource( int num )
{
	ResourceModel * model = DataTheme.resources.get( num );
	if( model->isPreservable() ){
		return;	
	}

	_other= num;
	_icoRight->setResource( num );
	_isRight = true;
	_icoRight->setValue( QString::number( 0 ) );
	_icoLeft->setValue( QString::number( 0 ) );
	if( _isLeft ) {
		int rvalue = _player->getResourceList()->getValue( _resource );
		_scroll->setMaximum( rvalue );
		_scroll->setEnabled( true );
		_butBuy->setEnabled( true );
		_butAll->setEnabled( true );
	}
}

void ExchangeResources::clear()
{
	_icoRight->clear();
	_icoLeft->clear();
	_scroll->setEnabled( false );
	_butBuy->setEnabled( false );
	_butAll->setEnabled( false );
	_isRight = false;
	_isLeft = false;
}

void ExchangeResources::setValue( int val )
{
	if( _player->getResourceList()->getValue( _resource ) < val ) {
		val = _player->getResourceList()->getValue( _resource );
	}
	_value=val;
	_scroll->setValue( val );
	float fcost = _prices->getResourceInResourceFloat(  _resource , _other );
	_icoLeft->setValue( QString::number( _value ) );
	_icoRight->setValue( QString::number( (int) ( _value * fcost )) );
}
