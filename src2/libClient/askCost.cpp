/****************************************************************
**
** Attal : Lords of Doom
**
** askCost.cpp
** Widget for asking cost
**
** Version : $Id: askCost.cpp,v 1.1 2008/01/07 15:06:12 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 25/08/2003
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

#include "askCost.h"

// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"

//
// ----- AskCost -----
//

AskCost::AskCost( const QString & text, QWidget * parent, const char * /*name*/ )
: QWidget( parent )
{
	_text = new QLabel( this );
	_text->setText( text );
	FIXEDSIZE( _text );

	_cost = new QLabel( this );
	_cost->setText( "0" );
	FIXEDSIZE( _cost );

	QPushButton * pbChange = new QPushButton( this );
	pbChange->setText( tr( "Change" ) );
	FIXEDSIZE( pbChange );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _text );
	layout->addWidget( _cost );
	layout->addWidget( pbChange );
	layout->addStretch( 1 );
	layout->activate();

	connect( pbChange, SIGNAL( clicked() ), SLOT( slot_change() ) );
}

void AskCost::setText( const QString & text )
{
	_text->setText( text );
	FIXEDSIZE( _text );
}

void AskCost::slot_change()
{
	EditCost cost( _resources, this );
	_resources = cost.getResources();

	if( cost.exec() ) {
		_resources = cost.getResources();
	}
	updateCost();
}

void AskCost::updateCost()
{
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();
	QString text = "";
	for( uint i = 0; i < nbRes; ++i ) {
		if( _resources[ themeResources.getRessource( i ) ] != 0 ) {
			if( text != "" ) {
				text += " + ";
			}
			text += QString::number( _resources[ themeResources.getRessource( i ) ] )
				+ " " + themeResources.getRessource( i );
		}
	}
	if( text == "" ) {
		text = "0";
	}
	_cost->setText( text );
	FIXEDSIZE( _cost );
}

void AskCost::setValue( uint num, uint value )
{
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();
	if( num < nbRes ) {
		_resources[ themeResources.getRessource( num ) ] = value;
	}
	updateCost();
}

uint AskCost::getValue( uint num )
{
	uint ret = 0;
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();
	if( num < nbRes ) {
		ret = _resources[ themeResources.getRessource( num ) ];
	}

	return ret;
}

//
// ----- EditCost -----
//

EditCost::EditCost( const QMap<QString, int> & resources, QWidget * parent, const char * /*name*/ )
: QDialog( parent, Qt::Dialog )
{
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QGridLayout * grid = new QGridLayout();
	QLabel * label;
	QSpinBox * box;
	for( uint i = 0; i < nbRes; i++ ) {
		label = new QLabel( themeResources.getRessource( i ), this );
		FIXEDSIZE( label );
		grid->addWidget( label, i, 0 );
		box = new QSpinBox( this );
		box->setMinimum( 0 );
		box->setMaximum( 1000000 );
		box->setValue( resources[ themeResources.getRessource( i ) ] );
		FIXEDSIZE( box );
		grid->addWidget( box, i, 1 );
		_boxes.append( box );
	}

	QHBoxLayout * layH = new QHBoxLayout();
	layH->addStretch( 1 );
	layH->addWidget( pbOk );
	layH->addStretch( 1 );
	layH->addWidget( pbCan );
	layH->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( grid, 1 );
	layout->addLayout( layH );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
}

QMap<QString, int> EditCost::getResources()
{
	QMap<QString, int> ret;
	ResourceList & themeResources = DataTheme.resources;
	uint nb = _boxes.count();
	for( uint i = 0; i < nb; ++i ) {
		ret.insert( themeResources.getRessource( i ), _boxes[ i ]->value() );
	}

	return ret;
}

