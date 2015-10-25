/****************************************************************
**
** Attal : Lords of Doom
**
** market.cpp
** display market place
**
** Version : $Id: market.cpp,v 1.6 2002/05/13 21:43:53 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qlayout.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qsignalmapper.h>
#include <qscrollbar.h>
// application specific include files
#include "common/log.h"
#include "client/gui.h"
#include "client/widget.h"

extern QString IMAGE_PATH;

Market::Market( int nb, QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	_prices = new PriceMarket();
	
	setCaption( "Marketplace" );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
		     
	QHBoxLayout * layH1 = new QHBoxLayout();
	_own = new DisplayRessources( nb, this );
	_own->setTitle( "Kingdom ressources" );
	layH1->addWidget( _own, 1 );
	
	_other = new DisplayRessources( nb, this );
	_other->setTitle( "Available for trade" );
	layH1->addWidget( _other, 1 );
	
	layout->addLayout( layH1, 1 );
	
	_exchange = new ExchangeRessources( this );
	layout->addWidget( _exchange );
	
	layout->setMargin( 2 );
	layout->activate();
	
	connect( _exchange, SIGNAL( sig_quit() ), SLOT( accept() ) );
	connect( _own, SIGNAL( sig_ressource( int ) ), SLOT( slot_ownRessource( int ) ) );
	connect( _other, SIGNAL( sig_ressource( int ) ), SLOT( slot_otherRessource( int ) ) );
}

void Market::setPrices( PriceMarket * prices )
{
	_prices = prices;
	_exchange->setPrices( prices );
}

void Market::slot_ownRessource( int num )
{
	_exchange->slot_ownRessource( num );
	_other->setPrices( _prices, num );
}

void Market::slot_otherRessource( int num )
{
	_exchange->slot_otherRessource( num );
}

/*!  Constructs an empty

*/

DisplayRessources::DisplayRessources( int nb, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_nbRessources = nb;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	_title = new Sentence( this );
	
	layout->addWidget( _title );
	layout->addStretch( 1 );
	
	_ressources = new RessourceIcon * [ nb ];

	QSignalMapper * sigmap = new QSignalMapper( this );
	
	for( int i = 0; i < nb; i+=3 ) {
		if( nb-i == 1 ) {
			_ressources[i] = new RessourceIcon( this );
			_ressources[i]->setRessource( i );
			layout->addWidget( _ressources[i] );
			sigmap->setMapping( _ressources[i], i );
			connect( _ressources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
		} else if( nb-i == 2 ) {
			QHBoxLayout * layH = new QHBoxLayout();
			_ressources[i] = new RessourceIcon( this );
			_ressources[i]->setRessource( i );
			layH->addStretch( 1 );
			layH->addWidget( _ressources[i] );
			layH->addStretch( 1 );
			sigmap->setMapping( _ressources[i], i );
			connect( _ressources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			_ressources[i+1] = new RessourceIcon( this );
			_ressources[i+1]->setRessource( i+1 );
			layH->addWidget( _ressources[i+1] );
			layH->addStretch( 1 );
			sigmap->setMapping( _ressources[i+1], i+1 );
			connect( _ressources[i+1], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			layout->addLayout( layH );
		} else {
			QHBoxLayout * layH = new QHBoxLayout();
			
			_ressources[i] = new RessourceIcon( this );
			_ressources[i]->setRessource( i );
			layH->addWidget( _ressources[i] );
			sigmap->setMapping( _ressources[i], i );
			connect( _ressources[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			
			_ressources[i+1] = new RessourceIcon( this );
			_ressources[i+1]->setRessource( i+1 );
			layH->addWidget( _ressources[i+1] );
			sigmap->setMapping( _ressources[i+1], i+1 );
			connect( _ressources[i+1], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			
			_ressources[i+2] = new RessourceIcon( this );
			_ressources[i+2]->setRessource( i+2 );
			layH->addWidget( _ressources[i+2] );
			sigmap->setMapping( _ressources[i+2], i+2 );
			connect( _ressources[i+2], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
			
			layout->addLayout( layH );
		}
	}
	

	layout->activate();
	
	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_ressource( int ) ) );
}

void DisplayRessources::setRessources( GenericPlayer * player )
{
	for( int i = 0; i < _nbRessources; i++ ) {
		_ressources[i]->setValue( QString::number( player->getResource( i ) ) );
	}
}

void DisplayRessources::setPrices( PriceMarket * prices, char ressource )
{
	for( int i = 0; i < _nbRessources; i++ ) {
		if( ressource == i )
			_ressources[i]->setValue( "n/a" );
		else
			_ressources[i]->setValue( QString::number( prices->getRessourceInRessource( i, ressource ) ) );
	}	
}

void DisplayRessources::setTitle( QString title )
{
	_title->setText( title );
}

/*!  Constructs an empty

*/

ExchangeRessources::ExchangeRessources( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_prices = new PriceMarket();
	setFixedHeight( 200 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	
	layH1->addStretch( 2 );
	_icoLeft = new RessourceIcon( this );
	layH1->addWidget( _icoLeft );
	layH1->addSpacing( 25 );
	
	_scroll = new QScrollBar( QScrollBar::Horizontal, this );
	layH1->addWidget( _scroll, 1 );
	layH1->addSpacing( 25 );
	
	_icoRight = new RessourceIcon( this );
	layH1->addWidget( _icoRight );
	layH1->addStretch( 2 );
	
	layout->addLayout( layH1 );
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	
	layH2->addStretch( 1 );
	
	_butAll = new QPushButton( this );
	_butAll->setText( "All" );
	_butAll->setFixedSize( 50, 40 );
	layH2->addWidget( _butAll );
	layH2->addSpacing( 50 );
	
	_butBuy = new QPushButton( this );
	_butBuy->setText( "Buy" );
	_butBuy->setFixedSize( 50, 40 );
	layH2->addWidget( _butBuy );
	
	layH2->addStretch( 1 );

	QPushButton * butOk = createButtonOk( this );
	layH2->addWidget( butOk );
	
	layout->addLayout( layH2 );
	
	layout->activate();
	
	connect( butOk, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
	connect( _butAll, SIGNAL( clicked() ), SLOT( slot_all() ) );
	connect( _butBuy, SIGNAL( clicked() ), SLOT( slot_buy() ) );
	connect( _scroll, SIGNAL( valueChanged( int ) ), SLOT( slot_value( int ) ) );
	
	clear();
}

void ExchangeRessources::slot_all()
{
	
}

void ExchangeRessources::slot_buy()
{
	
}

void ExchangeRessources::slot_ownRessource( int num )
{
	_icoLeft->setRessource( num );
	_isLeft = true;
	_ressource = num;
	setValue( 0 );
	if( _isRight ) {
		_cost = _prices->getRessourceInRessource( num, _ressource );
		_icoRight->setValue( QString::number( 0 ) );
		_scroll->setEnabled( true );
		_butBuy->setEnabled( true );
		_butAll->setEnabled( true );
	}
}

void ExchangeRessources::slot_otherRessource( int num )
{
	_icoRight->setRessource( num );
	_isRight = true;
	_icoRight->setValue( QString::number( 0 ) );
	if( _isLeft ) {
		_cost = _prices->getRessourceInRessource( num, _ressource );
		_icoRight->setValue( QString::number( 0 ) );
		_scroll->setEnabled( true );
		_butBuy->setEnabled( true );
		_butAll->setEnabled( true );
	}
}

void ExchangeRessources::clear()
{
	_icoRight->clear();
	_icoLeft->clear();
	_scroll->setEnabled( false );
	_butBuy->setEnabled( false );
	_butAll->setEnabled( false );
	_isRight = false;
	_isLeft = false;
}

void ExchangeRessources::setValue( int val )
{
	_scroll->setValue( val );
	_icoLeft->setValue( QString::number( val * _cost ) );
	_icoRight->setValue( QString::number( val ) );
}
