/****************************************************************
**
** Attal : Lords of Doom
**
** fightResult.cpp
** Displaying the result of a fight
**
** Version : $Id: fightResult.cpp,v 1.12 2008/04/10 21:33:32 lusum Exp $
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
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

// application specific include file
#include "libClient/attalButton.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

#include "libFight/casualtiesReport.h"
#include "libFight/fight.h"
#include "libFight/lordReport.h"

//
// ----- FightResult -----
//

FightResult::FightResult( Fight * fight, char /*result*/, QWidget * parent, const char * /* name */ )
	: QDialog( parent, Qt::Dialog )
{
	/// XXX: use result ?
	QVBoxLayout * layout = new QVBoxLayout( this );

	layout->addSpacing( 5 );
	_mainResult = new MainResult( fight->getAttack(), fight->getDefense(), this );
	layout->addWidget( _mainResult );

	layout->addStretch( 1 );

	QLabel * labCasu = new QLabel( this );
	labCasu->setFont( QFont( "Helvetica", 20, QFont::Bold ) );
	labCasu->setAlignment( Qt::AlignCenter );
	labCasu->setText( "Battlefield Casualties" );
	layout->addWidget( labCasu );

	layout->addStretch( 1 );

	QLabel * labAttack = new QLabel( this );
	labAttack->setFont( QFont( "Helvetica", 16, QFont::Bold ) );
	labAttack->setAlignment( Qt::AlignCenter );
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
	labDefense->setAlignment( Qt::AlignCenter );
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
	AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );
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

MainResult::MainResult( GenericLord * attack, GenericLord * defense, QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	_photo1 = new Icon( this );
	_photo1->setPixmap( * ImageTheme.getLordPixmap( attack->getId() )  );
	layout->addWidget( _photo1 );
	layout->addStretch( 1 );

	if( defense ) {
		_photo2 = new Icon( this );
		_photo2->setPixmap(* ImageTheme.getLordPixmap( attack->getId() ) );
		layout->addWidget( _photo2 );
	}

	layout->activate();
}

//
// ----- CasualtiesList -----
//

CasualtiesList::CasualtiesList( QWidget * parent, const char * /* name */)
	: QWidget( parent )
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
		Icon * photo = new Icon( _present, "photo" );
		photo->setPixmap( ImageTheme.getPhotoCreature( unit ) );
		_layPresent->addWidget( photo );
		_layPresent->addStretch( 2 );
	}
}

//
// ----- FightResultWizard -----
//

FightResultWizard::FightResultWizard( Fight * fight )
: QDialog( fight, Qt::Dialog )
{
	setWindowTitle(tr("FightResultWizard"));
	FightResultStatus result = fight->getFightResultStatus();

	QLabel * labResult = new QLabel( this );
	labResult->setFont( QFont( "Helvetica", 20, QFont::Bold ) );
	labResult->setAlignment( Qt::AlignCenter );
	
	if( ( fight->isAttack() && result.hasAttackWin() ) || ( ( ! fight->isAttack() ) && result.hasDefenseWin() ) ) {
		_lordReport = 0;
		/// XXX TODO when other gain will be managed more clearly _lordReport = new LordReport;
		labResult->setText( tr( "Congratulations, you win!" ) );
	} else {
		_lordReport = 0;
		if( result.hasFled() ) {
			labResult->setText( tr( "Humm, you have fled!" ) );
		} else if( result.hasSurrendered() ) {
			labResult->setText( tr( "Pff, you have surrendered!" ) );
		} else {
			labResult->setText( tr( "Sorry, you lose!" ) );
		}
	}
	if( fight->isAttack() ) {
		_playerCasualties = new CasualtiesReport( FIGHTER_ATTACK, fight );
		_opponentCasualties = new CasualtiesReport( FIGHTER_DEFENSE, fight );
	} else {
		_playerCasualties = new CasualtiesReport( FIGHTER_DEFENSE, fight );
		_opponentCasualties = new CasualtiesReport( FIGHTER_ATTACK, fight );
	}
	
	_stack = new QStackedWidget( this );
	if( _lordReport ) {
		_stack->addWidget( _lordReport );
	}
	_stack->addWidget( _playerCasualties );
	_stack->addWidget( _opponentCasualties );

	_pbPrev = new AttalButton( this, AttalButton::BT_PREV );
	_pbNext = new AttalButton( this, AttalButton::BT_NEXT );
	_pbOk = new AttalButton( this, AttalButton::BT_OK );

	FIXEDSIZE( labResult );

	QHBoxLayout * layHButtons = new QHBoxLayout();
	layHButtons->setMargin( 5 );
	layHButtons->setSpacing( 5 );
	layHButtons->addStretch( 1 );
	layHButtons->addWidget( _pbPrev );
	layHButtons->addStretch( 1 );
	layHButtons->addWidget( _pbNext );
	layHButtons->addStretch( 1 );
	layHButtons->addWidget( _pbOk );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addSpacing( 5 );
	layout->addWidget( labResult );
	layout->addWidget( _stack, 1 );
	layout->addLayout( layHButtons );
	layout->activate();

	connect( _pbPrev, SIGNAL( clicked() ), SLOT( slot_previous() ) );
	connect( _pbNext, SIGNAL( clicked() ), SLOT( slot_next() ) );
	connect( _pbOk, SIGNAL( clicked() ), SLOT( accept() ) );

	_stack->setCurrentIndex( 0 );
	checkArrows();
}


void FightResultWizard::slot_previous()
{
	int index = _stack->currentIndex();
	if( index > 0 ) {
		_stack->setCurrentIndex( index - 1 );
	}
	checkArrows();
}

void FightResultWizard::slot_next()
{
	if( _stack->currentIndex() < ( _stack->count() - 1 ) ) {
		_stack->setCurrentIndex( _stack->currentIndex() + 1 );
	}
	checkArrows();
}

void FightResultWizard::checkArrows()
{
	int index = _stack->currentIndex();

	if( index == 0 ) {
		_pbPrev->setEnabled( false );
	} else {
		_pbPrev->setEnabled( true );
	}

	if( index == ( _stack->count() - 1 ) ) {
		_pbNext->setEnabled( false );
	} else {
		_pbNext->setEnabled( true );
	}
}








