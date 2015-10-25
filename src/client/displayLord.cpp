/****************************************************************
**
** Attal : Lords of Doom
**
** displayLord.cpp
** show infos about Lord
**
** Version : $Id: displayLord.cpp,v 1.40 2003/05/08 20:19:31 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 24/08/2000
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

#include "displayLord.h"

 
// generic include files

// include files for QT
#include <qpushbutton.h>
#include <qstatusbar.h>
#include <qsignalmapper.h>
#include <qlayout.h>
#include <qmultilinedit.h>
#include <qframe.h>

// application specific includes
#include "conf.h"
#include "common/log.h"
#include "common/dataTheme.h"
#include "common/genericLord.h"
#include "common/specifications.h"

#include "client/displayTechnics.h"
#include "client/graphicalArtefact.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/lord.h"
#include "client/player.h"
#include "client/unitExchange.h"
#include "client/widget.h"

extern QString IMAGE_PATH;
extern DataTheme DataTheme;
extern ImageTheme ImageTheme;

//
// ----- DisplayLord -----
//

DisplayLord::DisplayLord( Player * player, QWidget * parent, char * name )
//	:QDialog( parent, name, true, WStyle_Customize | WStyle_NoBorder ),
	:QDialog( parent, name, true ),
	 _player( player )
{
	QVBoxLayout * layout1 = new QVBoxLayout( this );
	layout1->addSpacing( 5 );

	QHBoxLayout * layout2 = new QHBoxLayout();
	layout2->addSpacing( 5 );

	QVBoxLayout * layout3 = new QVBoxLayout();

	QHBoxLayout * layout4 = new QHBoxLayout();
	_dispGeneral = new DisplayGeneral( player, this );
	layout4->addWidget( _dispGeneral, 1 );
	layout4->addSpacing( 5 );
	QVBoxLayout * layout5 = new QVBoxLayout();
	_dispArtefact = new DisplayArtefact( player, this );
	layout5->addWidget( _dispArtefact, 1 );
	layout5->addSpacing( 50 + 5 );

	layout4->addLayout( layout5, 1 );

	layout3->addLayout( layout4, 1 );
	_dispUnit = new DisplayUnit( player, this );
	layout3->addWidget( _dispUnit );

	layout2->addLayout( layout3, 1 );

	layout2->addSpacing( 5 );

	_dispList = new DisplayListPanel( player, this );


	layout2->addWidget( _dispList );

	layout1->addLayout( layout2, 1 );
	layout1->addSpacing( 5 );
	QStatusBar * bar = new QStatusBar( this );
	bar->message( "Info Lord" );
	bar->setFixedHeight( bar->sizeHint().height() );
	bar->setSizeGripEnabled( false );
	layout1->add( bar );
	
	layout1->activate();
	
	setFixedSize( 800, 600 );
	connect( _dispList, SIGNAL( sig_quit() ), SLOT( accept() ) );
	connect( _dispList, SIGNAL( sig_lord( int ) ), SLOT( slot_lord( int ) ) );
}

DisplayLord::~DisplayLord()
{

}

void DisplayLord::slot_lord( int num )
{
	_player->setSelectedLord( _player->getLord( num ) );
	reupdate();
}

void DisplayLord::show()
{
	reinit();
	QDialog::show();
}

void DisplayLord::reinit()
{
	_dispList->reinit();
	_dispUnit->reinit();
	_dispArtefact->reinit();
	_dispGeneral->reinit();
}

void DisplayLord::reupdate()
{
	_dispList->reupdate();
	_dispUnit->reupdate();
	_dispArtefact->reupdate();
	_dispGeneral->reupdate();	
}

//
// ----- DisplayGeneral -----
//

DisplayGeneral::DisplayGeneral( Player * player, QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_player = player;
	
	_photo = new Icon( this );
	_photo->move( 10, 10 );
	
	_title = new QLabel( this );
	_title->setAlignment( AlignCenter );
	_title->move( 90, 20 );
	
	Icon * ic1 = new Icon( this );
	ic1->move( 10, 140 );
	ic1->setPixmap( QPixmap( IMAGE_PATH + "misc/ico_attack.png" ) );
	Icon * ic2 = new Icon( this );
	ic2->move( 80, 140 );
	ic2->setPixmap( QPixmap( IMAGE_PATH + "misc/ico_defense.png" ) );
	Icon * ic3 = new Icon( this );
	ic3->move( 150, 140 );
	ic3->setPixmap( QPixmap( IMAGE_PATH + "misc/ico_power.png" ) );
	Icon * ic4 = new Icon( this );
	ic4->move( 220, 140 );
	ic4->setPixmap( QPixmap( IMAGE_PATH + "misc/ico_knowledge.png" ) );
	
	QLabel * titre[4];
	for( int i = 0; i < 4; i++ ) {
		titre[i] = new QLabel( this );
		titre[i]->setAlignment( AlignCenter );
		titre[i]->setFixedSize( 60, 20 );
		titre[i]->move( 10 + (i*70), 120 );
	}
	titre[0]->setText( "Attack" );
	titre[1]->setText( "Defense" );
	titre[2]->setText( "Power" );
	titre[3]->setText( "Knowledge" );
	
	_labAttack = new QLabel( this );
	_labAttack->setAlignment( AlignCenter );
	_labAttack->setFixedSize( 60, 20 );
	_labAttack->move( 10, 200 );
	
	_labDefense = new QLabel( this );
	_labDefense->setAlignment( AlignCenter );
	_labDefense->setFixedSize( 60, 20 );
	_labDefense->move( 80, 200 );
	
	_labPower = new QLabel( this );
	_labPower->setAlignment( AlignCenter );
	_labPower->setFixedSize( 60, 20 );
	_labPower->move( 150, 200 );
	
	_labKnowledge = new QLabel( this );
	_labKnowledge->setAlignment( AlignCenter );
	_labKnowledge->setFixedSize( 60, 20 );
	_labKnowledge->move( 220, 200 );		
	
	_specialty = new InfoLabel( this );
	_specialty->move( 10, 240 );
	_specialty->setText( "Specialty\nNone" );
	
	_experience = new InfoLabel( this );
	_experience->move( 10, 300 );
	
	_spellPoints = new InfoLabel( this );
	_spellPoints->move( 160, 300 );

	reinit();
}

void DisplayGeneral::reinit()
{
	Lord * aLord = (Lord*)_player->getSelectedLord();
	_photo->setPixmap( *_player->getSelectedLordPixmap() );
	
	QString title;
	title.sprintf( "Lord %s\nLevel %d of %s", aLord->getName().latin1(),
		       aLord->getLevel(), aLord->getCategoryName().latin1() );
	_title->setText( title );	
	FIXEDSIZE( _title );
	
	_labAttack->setText( QString::number( aLord->getCharac( ATTACK ) ) );
	_labDefense->setText( QString::number( aLord->getCharac( DEFENSE ) ) );
	_labPower->setText( QString::number( aLord->getCharac( POWER ) ) );
	_labKnowledge->setText( QString::number( aLord->getCharac( KNOWLEDGE ) ) );

	_specialty->setText( "Specialty\nNone" );
	QString temp;
	temp.sprintf( "Experience\n%d", aLord->getCharac( EXPERIENCE ) );
	_experience->setText( temp );
	temp.sprintf( "Spell Points\n%d/%d", aLord->getCharac( TECHNICPOINT ),
		       aLord->getCharac( MAXTECHNICPOINT ) );
	_spellPoints->setText( temp );
}

//
// ----- DisplayArtefact -----
//

DisplayArtefact::DisplayArtefact( Player * player, QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_player = player;
	_index = 0;
	_used = 0;
	uint i;

	setBackgroundPixmap( IMAGE_PATH + "misc/artefactLord.png" );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->addStretch( 1 );

	QHBoxLayout * layH1 = new QHBoxLayout();

	QPushButton * pb1 = new QPushButton( this );
	pb1->setText( "<" );
	FIXEDSIZE( pb1 );
	layH1->addWidget( pb1 );

	/// WIN: was "_used = new (IconArtefact *) [DataTheme.specifications.getArtefactNumber()];"
	_used = new IconArtefact * [DataTheme.specifications.getArtefactNumber()];
	SpecificationArtefact * temp;

	for( i = 0; i < DataTheme.specifications.getArtefactNumber(); i++ ) {
		temp = DataTheme.specifications.getArtefact( i );
		_used[i] = new IconArtefact( this );
		_used[i]->move( temp->getX(), temp->getY() );
	}

	layH1->addStretch( 1 );

	for( i = 0; i < 3; i++ ) {
		_ico[i] = new IconArtefact( this );
		_ico[i]->clear();
		layH1->addWidget( _ico[i] );
	}

	layH1->addStretch( 1 );
	QPushButton * pb2 = new QPushButton( this );
	pb2->setText( ">" );
	FIXEDSIZE( pb2 );
	layH1->addWidget( pb2 );

	layout->addLayout( layH1 );

	layout->activate();

	connect( pb1, SIGNAL( clicked() ), SLOT( slot_prevArtefact() ) );
	connect( pb2, SIGNAL( clicked() ), SLOT( slot_nextArtefact() ) );
}

DisplayArtefact::~DisplayArtefact()
{
	if( _used ) {
		delete [] _used;
	}
}

void DisplayArtefact::reinit()
{
	GenericLord * lord = _player->getSelectedLord();

	for( int i = 0; i < 3; i++ ) {
		if( ( _index + i ) < lord->numArtefact() ) {
			_ico[i]->setType( lord->getArtefact( _index + i )->getType() );
		} else {
			_ico[i]->clear();
		}
	}
}

void DisplayArtefact::slot_nextArtefact()
{
	if( _index + 3 < _player->getSelectedLord()->numArtefact() ) {
		_index++;
		reinit();
	}
}

void DisplayArtefact::slot_prevArtefact()
{
	if( _index > 0 ) {
		_index--;
		reinit();
	}
}

//
// ----- DisplayUnit -----
//

DisplayUnit::DisplayUnit( Player * player, QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_unit = -1;
	_exch = false;
	_player = player;
	_unitExchange = 0;
	int i;
	
	QSignalMapper * sigmap = new QSignalMapper( this );
	
	for( i = 0; i < MAX_UNIT; i++ ) {
		_buttonUnit[i] = new Icon( this );
		_buttonUnit[i]->move( 30 + (i*60), 10 );
		_numUnit[i] = new QLabel( this );
		_numUnit[i]->setFixedSize( 60, 20 );
		_numUnit[i]->setAlignment( AlignCenter );
		_numUnit[i]->move( 30 + (i*60), 70 );
		sigmap->setMapping( _buttonUnit[i], i );
		connect( _buttonUnit[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}	
			
	for( i = 0; i < 4; i++ ) {
		_dispo[i] = new QPushButton( this );
		_dispo[i]->setFixedSize( 50, 40 );
	}
	_dispo[0]->move( 480, 5 );
	_dispo[1]->move( 480, 55 );
	_dispo[2]->move( 560, 5 );
	_dispo[3]->move( 560, 55 );
			
	_butExchange = _dispo[2];
	_butExchange->setEnabled( false );
	_butExchange->setFixedSize( 50, 40 );
	_butExchange->setPixmap( IMAGE_PATH + "misc/exchange.png" );
	
	setFixedSize( 640, 100 );
	reinit();
	
	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_unitClicked( int ) ) );
	connect( _butExchange, SIGNAL( clicked() ), SLOT( slot_exchange() ) );
	connect( _dispo[3], SIGNAL( clicked() ), SLOT( slot_technic() ) );
}

void DisplayUnit::reinit()
{
	GenericLord * lord = _player->getSelectedLord();
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( lord->getUnit( i ) ) {
			_buttonUnit[i]->setPixmap( QPixmap( ImageTheme.getPhotoCreature( lord->getUnit( i ) ) ) );
			_buttonUnit[i]->setPopupText( lord->getUnit( i )->getCreature()->getName() );
			_numUnit[i]->setText( QString::number( lord->getUnit( i )->getNumber() ) );
		} else {
			_buttonUnit[i]->clear();
			_numUnit[i]->clear();
		}
	}
}

void DisplayUnit::slot_exchange()
{
	_exch = true;
}

void DisplayUnit::slot_technic()
{
	DisplayTechnics dialog;
	dialog.exec();
}

void DisplayUnit::quitExchange()
{
	deselectUnit();
	_exch = false;
}

void DisplayUnit::selectUnit( int num )
{
	_unit = num;
	_butExchange->setEnabled( true );
}

void DisplayUnit::deselectUnit()
{
	_unit = -1;
	_butExchange->setEnabled( false );
}

void DisplayUnit::exchangeUnit( int num )
{	
	GenericLord * lord = _player->getSelectedLord();

	if( _unit == num ) {
		/// XXX: something to do ?
	} else {
		if( lord->getUnit( num ) ) {
			if( lord->getUnit( _unit )->getCreature() == lord->getUnit( num )->getCreature() ) {
				if( ! _unitExchange ) {
					_unitExchange = new UnitExchange( this );
				}
				_unitExchange->setMin( 0 );
				_unitExchange->setMax( lord->getUnit( _unit )->getNumber() + lord->getUnit( num )->getNumber() );
				_unitExchange->setUnits( lord->getUnit( _unit ), lord->getUnit( num ) );
				//_unitExchange->setCreature( lord->getUnit( _unit )->getCreature() );
				if( _unitExchange->exec() ) {
					if( _unitExchange->getLeftValue() != 0 )
						lord->getUnit( _unit )->setNumber( _unitExchange->getLeftValue() );
					else {
						delete lord->getUnit( _unit );
						lord->setUnit( _unit, 0 );
					}
					if( _unitExchange->getRightValue() != 0 )
						lord->getUnit( num )->setNumber( _unitExchange->getRightValue() );
					else {
						delete lord->getUnit( num );
						lord->setUnit( num, 0 );
					}
				}
			}
		} else {
			if( ! _unitExchange ) {
				_unitExchange = new UnitExchange( this );
			}
			_unitExchange->setMin( 0 );
			_unitExchange->setMax( lord->getUnit( _unit )->getNumber() );
			_unitExchange->setUnits( lord->getUnit( _unit ), 0 );
			//_unitExchange->setCreature( lord->getUnit( _unit )->getCreature() );
			if( _unitExchange->exec() ) {
				if( _unitExchange->getRightValue() != 0 ) {
					GenericFightUnit * tempUnit = new GenericFightUnit();
					tempUnit->setCreature( lord->getUnit( _unit )->getRace(), lord->getUnit( _unit )->getLevel() );
					lord->setUnit( num, tempUnit );
					lord->getUnit( num )->setNumber( _unitExchange->getRightValue() );
				} 
				if( _unitExchange->getLeftValue() != 0 ) {
					lord->getUnit( _unit )->setNumber( _unitExchange->getLeftValue() );
				} else {
					delete lord->getUnit( _unit );
					lord->setUnit( _unit, 0 );
				}
			}
		}	
	}
	quitExchange();	
}

void DisplayUnit::slot_unitClicked( int num )
{
	GenericLord * lord = _player->getSelectedLord();

	if( _exch ) {
		exchangeUnit( num );
	} else {
		if( _unit == num ) {
			/// XXX: show unit charact
			return;
		}
	
		if( _unit == -1 ) {
			if( lord->getUnit( num ) ) {
				selectUnit( num );
			}
		} else {
			if( lord->getUnit( num ) ) {
				if( lord->getUnit( _unit )->getCreature() == lord->getUnit( num )->getCreature() ) {
					lord->getUnit( num )->addNumber( lord->getUnit( _unit )->getNumber() );
					delete lord->getUnit( _unit );
					lord->setUnit( _unit, 0 );
					deselectUnit();
				} else {
					GenericFightUnit * tempUnit = (lord->getUnit( _unit ));
					lord->setUnit( _unit, lord->getUnit( num ) );
					lord->setUnit( num, tempUnit );				
					deselectUnit();
				}
			} else {
				lord->setUnit( num, lord->getUnit( _unit ) );
				lord->setUnit( _unit, 0 );
				deselectUnit();
			}
		}
	}
	reinit();
}

//
// ----- DisplayListPanel -----
//

DisplayListPanel::DisplayListPanel( Player * player, QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_player = player;
	
  	setFrameStyle( QFrame::Panel | QFrame::Sunken );
	setLineWidth( 1 );
	setFixedWidth( 60 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );

	QLabel * emblem = new QLabel( this );
	emblem->setFixedSize( 58, 58 );
	emblem->setPixmap( * ImageTheme.getFlag( 0 ) );
	
	layout->addWidget( emblem );
	layout->addSpacing( 5 );
	
	_listLord = new DisplayListLord( _player, this );
	layout->addWidget( _listLord );
	
	layout->addStretch( 1 );
	layout->addSpacing( 5 );

	QPushButton * butOk = createButtonOk( this );

	layout->addWidget( butOk );
	layout->addSpacing( 5 );

	layout->activate();

	connect( butOk, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
	connect( _listLord, SIGNAL( sig_lord( int ) ), SIGNAL( sig_lord( int ) ) );
}

void DisplayListPanel::reinit()
{
	_listLord->reinit();
	update();
}

void DisplayListPanel::reupdate()
{
	_listLord->reupdate();
	update();
}

/************************************************************************************/


DisplayListLord::DisplayListLord( Player * player, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_player = player;
	_sigmap = new QSignalMapper( this );
	_listBut.setAutoDelete( true );
	setFixedWidth( 55 );
	connect( _sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_lord( int ) ) );
}

void DisplayListLord::reupdate()
{
	/// XXX: add border to selected unit
}

void DisplayListLord::reinit()
{
	_listBut.clear();
	
	for( uint i = 0; i < _player->numLord(); i++ ) {
		QPushButton * but = new QPushButton( this );
		but->setFixedSize( 50, 40 );
		but->move( 5, 5 + 50 * i );
		but->setPixmap( *_player->getLordSmallPixmapByNum( i ) );
		_sigmap->setMapping( but, i );
		connect( but, SIGNAL( clicked() ), _sigmap, SLOT( map() ) );
		_listBut.append( but );
	}
	
	setMinimumHeight( _player->numLord() * 50 );
}



