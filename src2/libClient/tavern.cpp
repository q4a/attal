/****************************************************************
**
** Attal : Lords of Doom
**
** tavern.cpp
** Manage tavern building
**
** Version : $Id: tavern.cpp,v 1.22 2009/09/11 18:01:57 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 04/02/2005
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

#include "tavern.h"
 
// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>

#include <QScrollArea>
// application specific include files
#include "libCommon/attalSocket.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

//
// ----- Tavern -----
//

Tavern::Tavern( QWidget * parent )
: QDialog( parent, Qt::Dialog )
{
	QLabel * labIntro = new QLabel( this );
	labIntro->setWordWrap( true );	
	labIntro->setText( tr("Some lords are visiting your base and are available in lord's building. Here is the list:") );
	
	
	AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );
	
	QHBoxLayout * layHOk = new QHBoxLayout();
	layHOk->addStretch( 1 );
	layHOk->addWidget( pbOk );
	layHOk->addStretch( 1 );
	
	_scroll = new QScrollArea( this );
	
	_tview = new TavernView( this );

	_scroll->setWidget( _tview );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( labIntro );
	layout->addWidget( _scroll, 1 );
	layout->addLayout( layHOk );
	layout->activate();
	
	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( parent, SIGNAL( sig_castle() ), SLOT( restart() ) );
	connect( parent, SIGNAL( sig_tavernInfo( char ) ),_tview,  SLOT( handleTavernInfo( char ) ) );
	connect( parent , SIGNAL( sig_tavernLord( int ,bool  ) ), _tview , SLOT( handleTavernLord( int , bool  ) ) );
	connect( this, SIGNAL( sig_buyLord( uchar ) ), parent, SLOT( slot_buyLord( uchar ) ) );
	connect( this, SIGNAL( sig_updateLordInfo() ), parent, SLOT( slot_updateLordInfo() ) );
	connect( this, SIGNAL( sig_askLord( int ) ), parent, SLOT( slot_askLord( int ) ) );
	connect( _tview, SIGNAL( sig_updateLordInfo() ),SIGNAL( sig_updateLordInfo() ) );
	connect( _tview, SIGNAL( sig_buyLord( uchar ) ),SIGNAL( sig_buyLord( uchar ) ) );
	connect( _tview, SIGNAL( sig_askLord( int ) ),SIGNAL( sig_askLord( int ) ) );
	
	resize( 440, 300 );
}

Tavern::~Tavern()
{
}

void Tavern::restart()
{
	//emit sig_updateLordInfo();
}

//
// ----- TavernView -----
//

TavernView::TavernView( QWidget * /* parent */ )
{
	
	_signalMapper = NULL;
	_signalMapper = new QSignalMapper( this );
	_layout = new QVBoxLayout( this );

	_layout->activate();
}

TavernView::~TavernView()
{
	reinit(); 

	if( _signalMapper ) {
		delete _signalMapper;
		_signalMapper = NULL;
	}

}

void TavernView::reinit()
{
	while( ! _lords.isEmpty() ) {
		TavernLord * widget  = _lords.takeFirst();
		_layout->removeWidget(  widget  );
		disconnect( widget, 0, 0, 0);
		delete widget;
		_signalMapper->removeMappings( widget );
	}
}

void TavernView::handleTavernLord( int idLord, bool enabled )
{

	_layout->setEnabled( false );
	_nbLordRead++;
	/// XXX: reads lord info and displays it
	TavernLord * widget = new TavernLord( this );
	widget->setEnabled( enabled ) ;
	widget->init( idLord );
	_signalMapper->setMapping( widget, _lords.count() );
	_lords.append( widget );
	_layout->addWidget( widget );
	connect( widget, SIGNAL( sig_buy() ), _signalMapper, SLOT( map() ) );
	if( _nbLordRead >= _nbLord ) {
		//QDialog::exec();
	}
	resize( 440, 100 * _lords.count() ); //workaround
	_layout->setEnabled( true );

}

void TavernView::handleTavernInfo( char nbLord )
{
	_nbLord = nbLord;
	_nbLordRead = 0;

	reinit();
	
	connect( _signalMapper, SIGNAL( mapped( int ) ), SLOT( slot_buy( int ) ) );
	if( _nbLordRead >= _nbLord ) {
	} else {
		emit sig_askLord( _nbLordRead );
	}
}

void TavernView::slot_buy(  int numLord )
{
	if( _lords.value( numLord ) ) {
		emit sig_buyLord(  _lords.at(numLord)->getLord()->getId() );
	}
}

//
// ----- TavernLord -----
//

TavernLord::TavernLord( QWidget * parent )
: QFrame( parent )
{
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	_lord = 0;

	_enabled = true;

	_photo = new Icon( this );
	
	_info = new QLabel( this );
	_price = new QLabel( this );
	
	_pbBuy = new QPushButton( this );
	_pbBuy->setText( tr( "Buy" ) );
	FIXEDSIZE( _pbBuy );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->setSpacing( 5 );
	layV1->addWidget( _info );
	layV1->addWidget( _price );
	layV1->addStretch( 1 );
	
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _photo );
	layout->addLayout( layV1,1 );
	layout->addWidget( _pbBuy );
	layout->activate();
	
	resize( 400, 80 );
	
	connect( _pbBuy, SIGNAL( clicked() ), this, SIGNAL( sig_buy() ) );
}

TavernLord::~TavernLord()
{
	reinit();
}

void TavernLord::reinit()
{
	if(_lord){
		delete _lord;
		_lord = NULL;
	}
}

void TavernLord::init( int idLord )
{
	_pbBuy->setEnabled( _enabled );
	QString temp;
	reinit();
	
	GenericLord * lord = new GenericLord( idLord );

	_lord = lord;

	if( ImageTheme.getLordPixmap( lord->getId() ) ) {
		_photo->setPixmap( * ImageTheme.getLordPixmap( lord->getId() ) );
	}

	temp.sprintf( "Lord %s (%s)", qPrintable(lord->getName()), qPrintable( lord->getCategoryName() ) );
	_info->setText( temp );
	FIXEDSIZE( _info );
	GenericLordModel * lordModel = DataTheme.lords.at(_lord->getId());
	
	_price->setText( lordModel->getLordDescription() );
	FIXEDSIZE( _price );
}



