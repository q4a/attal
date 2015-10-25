/****************************************************************
**
** Attal : Lords of Doom
**
** askDiversification.cpp
** Displays and asks diversification of tiles

**
** Version : $Id: askDiversification.cpp,v 1.11 2007/05/03 13:46:54 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 12/01/2004
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

#include "askDiversification.h"
 
// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;

//
// ----- AskDiversification -----
//


AskDiversification::AskDiversification( const QString & destination, QWidget * parent, const char * /*name*/ )
: QDialog( parent, Qt::Dialog )
{
	_pixmap = new AskPixmap( true, destination, "Tile: ", this );
	_weight = new AskInt( "Weight: ", this );

	QPushButton * pbOk = new QPushButton( "Ok", this );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( "Cancel", this );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _pixmap );
	layout->addWidget( _weight );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
}

uint AskDiversification::getWeight()
{
	return _weight->getValue();
}


void AskDiversification::setWeight( uint weight )
{
	_weight->setValue( weight );
}


QString AskDiversification::getPixmapPath()
{
	return _pixmap->getValue();
}


void AskDiversification::setPixmapPath( const QString & path )
{
	_pixmap->setValue( path );
}

void AskDiversification::slot_accept()
{
	_pixmap->save();
	accept();
}

//
// ----- AskDiversificationList -----
//

AskDiversificationList::AskDiversificationList( QWidget * parent, const char * /*name*/ )
:QWidget( parent )
{
	_cell = 0;
	_list = new QListWidget( this );

	_pbNew = new QPushButton( tr("New"), this );
	FIXEDSIZE( _pbNew );

	_pbDel = new QPushButton( tr("Del"), this );
	FIXEDSIZE( _pbDel );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->setSpacing( 5 );
	layV1->addWidget( _pbNew );
	layV1->addWidget( _pbDel );
	layV1->addStretch( 1 );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _list, 1 );
	layout->addLayout( layV1 );
	layout->activate();

	connect( _pbNew, SIGNAL( clicked() ), SLOT( slot_new() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
}

void AskDiversificationList::slot_new()
{
	AskDiversification dialog( IMAGE_PATH + "/tiles/new.png", this );
	if( dialog.exec() ) {
		/* This code works but is commented cause bugs in ImageThemes.cells ( don't load new picture )
		 *
		_cell->addDiversification( dialog.getWeight() );
		setValue( _cell );
		*/
	}
}

void AskDiversificationList::slot_del()
{
	if( _list->count() > 1 ) {
		_cell->removeDiversification( _list->currentRow() );
		setValue( _cell );
	}
}

void AskDiversificationList::setValue( CellModel * cell )
{
	uint nb, val, index = 0;
	QString text;

	_cell = cell;
	_list->clear();
	nb = _cell->getDiversificationNumber();
	for( int j = 0; j < DataTheme.tiles.count(); j++ ) {
		if( DataTheme.tiles.at(j) == cell ) {
			index = j;
			break;
		}
	}
	for( uint i = 0; i < nb; i++ ) {
		val = _cell->getDiversification( i );
		text.sprintf( "(weight = %d)", val );
		QListWidgetItem * item = new QListWidgetItem( _list );
		item->setIcon( ImageTheme.cells[index]->at( i ) );
		item->setText( text );
	}

}

void AskDiversificationList::save()
{

}

void AskDiversificationList::clear()
{
	_list->clear();
}

