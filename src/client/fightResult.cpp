/****************************************************************
**
** Attal : Lords of Doom
**
** fightResult.cpp
** Displaying the result of a fight
**
** Version : $Id: fightResult.cpp,v 1.10 2003/01/06 21:52:07 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/12/2000
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

#include "fightResult.h"


// generic include files
// include files for QT
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qframe.h>
#include <qfont.h>
// application specific include file
#include "client/fight.h"
#include "client/gui.h"
#include "client/widget.h"

extern QString IMAGE_PATH;

//
// ----- FightResult -----
//

FightResult::FightResult( Fight * fight, char result, QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	QVBoxLayout * layout = new QVBoxLayout( this );

	layout->addSpacing( 5 );
	_mainResult = new MainResult( fight->getAttack(), fight->getDefense(), this );
	layout->addWidget( _mainResult );

	layout->addStretch( 1 );

	QLabel * labCasu = new QLabel( this );
	labCasu->setFont( QFont( "Helvetica", 20, QFont::Bold ) );
	labCasu->setAlignment( AlignCenter );
	labCasu->setText( "Battlefield Casualties" );
	layout->addWidget( labCasu );

	layout->addStretch( 1 );

	QLabel * labAttack = new QLabel( this );
	labAttack->setFont( QFont( "Helvetica", 16, QFont::Bold ) );
	labAttack->setAlignment( AlignCenter );
	labAttack->setText( "Attack" );
	layout->addWidget( labAttack );

	_attackList = new CasualtiesList( this );

	for( uint i = 0; i < fight->getCasualtiesNumber( FIGHTER_ATTACK ); i++ ) {
		GenericFightUnit * unit = fight->getCasualtiesUnit( FIGHTER_ATTACK, i );
		if( unit ) {
			_attackList->addCasualty( unit );
		}
	}
	layout->addWidget( _attackList );

	layout->addStretch( 1 );

	QLabel * labDefense = new QLabel( this );
	labDefense->setFont( QFont( "Helvetica", 16, QFont::Bold ) );
	labDefense->setAlignment( AlignCenter );
	labDefense->setText( "Defense" );
	layout->addWidget( labDefense );

	_defenseList = new CasualtiesList( this );
	for( uint j = 0; j < fight->getCasualtiesNumber( FIGHTER_DEFENSE ); j++ ) {
		GenericFightUnit * unit = fight->getCasualtiesUnit( FIGHTER_DEFENSE, j );
		if( unit ) {
			_defenseList->addCasualty( unit );
		}
	}
	layout->addWidget( _defenseList );

	layout->addStretch( 1 );

	QHBoxLayout * lay1 = new QHBoxLayout();
	QPushButton * pbOk = createButtonOk( this );
	lay1->addStretch( 6 );
	lay1->addWidget( pbOk );
	lay1->addStretch( 1 );
	layout->addLayout( lay1 );
	layout->addSpacing( 15 );

	layout->activate();
	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );

	hide();
}

//
// ----- MainResult -----
//

MainResult::MainResult( GenericLord * attack, GenericLord * defense, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );
	QString fichier;

	_photo1 = new Icon( this );
	fichier.sprintf( "lords/lord_%03d.png", attack->getId() );
	_photo1->setPixmap( QPixmap( IMAGE_PATH + fichier ) );
	layout->addWidget( _photo1 );
	layout->addStretch( 1 );

	_photo2 = new Icon( this );
	fichier.sprintf( "lords/lord_%03d.png", defense->getId() );
	_photo2->setPixmap( QPixmap( IMAGE_PATH + fichier ) );
	layout->addWidget( _photo2 );

	layout->activate();
}

//
// ----- CasualtiesList -----
//

CasualtiesList::CasualtiesList( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_present = new QFrame( this );
	_present->setFrameStyle( QFrame::Box | QFrame::Raised );
	_present->setLineWidth( 1 );
	_present->setMidLineWidth( 1 );
	_present->setFixedHeight( 44 );

	_none = new QLabel( _present );
	_none->setText( "None" );
	_none->setFixedSize( _none->sizeHint() );

	_layPresent = new QHBoxLayout( _present );
	_layPresent->addStretch( 1 );
	_layPresent->addWidget( _none );
	_layPresent->addStretch( 1 );
	_layPresent->activate();

	_numbers = new QFrame( this );
	_numbers->setFixedHeight( 10 );

	_layNumbers = new QHBoxLayout( _numbers );
	_layNumbers->addStretch( 1 );
	_layNumbers->activate();

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _present, 1 );
	layout->addWidget( _numbers, 1 );
	layout->activate();

	setFixedHeight( 54 );
}

void CasualtiesList::addCasualty( GenericFightUnit * unit )
{
	if( unit ) {
		_none->hide();
		QString f;
		f.sprintf( "units/face_%i_%i.png", unit->getRace(), unit->getLevel() );
		Icon * photo = new Icon( _present, "photo" );
		photo->setPixmap( QPixmap( IMAGE_PATH + f ) );
		_layPresent->addWidget( photo );
		_layPresent->addStretch( 2 );
	}
}


