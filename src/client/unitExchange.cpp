/****************************************************************
**
** Attal : Lords of Doom
**
** unitExchange.cpp
** dialog for creature exchange between units
**
** Version : $Id: unitExchange.cpp,v 1.8 2002/05/04 21:16:04 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/01/2001
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

#include "unitExchange.h"

// generic include files
// include files for QT
#include <qlayout.h>
#include <qscrollbar.h>
#include <qpushbutton.h>
// application specific include files
#include "common/unit.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/widget.h"

extern QString IMAGE_PATH;
extern ImageTheme ImageTheme;

/*!  Constructs an empty

*/

UnitExchange::UnitExchange( QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addStretch( 1 );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	_icoLeft = new Icon( this );
	layH1->addWidget( _icoLeft );
	layH1->addStretch( 2 );
	_icoRight = new Icon( this );
	layH1->addWidget( _icoRight );
	layH1->addStretch( 1 );
	layout->addLayout( layH1 );
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addStretch( 1 );
	_leftLCD = new QLCDNumber( this );
	layH2->addWidget( _leftLCD );
	layH2->addStretch( 2 );
	_rightLCD = new QLCDNumber( this );
	layH2->addWidget( _rightLCD );
	layH2->addStretch( 1 );
	layout->addLayout( layH2 );
	
	QHBoxLayout * layH3 = new QHBoxLayout();
	layH3->addStretch( 1 );
	_scroll = new QScrollBar( QScrollBar::Horizontal, this );
	layH3->addWidget( _scroll, 2 );
	layH3->addStretch( 1 );
	layout->addLayout( layH3 );

	layout->addSpacing( 5 );
	layout->addStretch( 1 );

	QHBoxLayout * layH4 = new QHBoxLayout();
	layH4->addStretch( 1 );
	QPushButton * pbOk = createButtonOk( this );
	layH4->addWidget( pbOk );
	layH4->addStretch( 1 );
	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setFixedSize( 50, 40 );
	pbCancel->setPixmap( IMAGE_PATH + "misc/no.png" );
	layH4->addWidget( pbCancel );
	layH4->addStretch( 1 );
	layout->addLayout( layH4 );

	layout->addSpacing( 5 );

	layout->activate();
	connect( _scroll, SIGNAL( valueChanged( int ) ), SLOT( slot_barScrolled( int ) ) );
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_ok() ) );
	connect( pbCancel, SIGNAL( clicked() ), SLOT( slot_cancel() ) );
	
	setFixedSize( 190, 220 );
}

void UnitExchange::setMin( int val )
{
	_scroll->setMinValue( val );
}

void UnitExchange::setMax( int val )
{
	_scroll->setMaxValue( val );
}

void UnitExchange::setUnits( GenericFightUnit * uniLeft, GenericFightUnit * uniRight )
{
	if( uniLeft == 0 ) {
		return;
	}
	int valLeft, valRight;
	if( uniRight == 0 ) {
		valRight = 0;
	} else {
		valRight = uniRight->getNumber();
	}
	valLeft = uniLeft->getNumber();
	
	 
	_sum = valLeft + valRight;
	_scroll->setValue( valRight );
	slot_barScrolled( valRight );
	_icoRight->setPixmap( QPixmap( ImageTheme.getPhotoCreature( uniLeft ) ) );
	_icoLeft->setPixmap( QPixmap( ImageTheme.getPhotoCreature( uniLeft ) ) );
}

void UnitExchange::slot_barScrolled( int val )
{
	_rightLCD->display( val );
	_leftLCD->display( _sum - val );
}
