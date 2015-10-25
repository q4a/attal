/****************************************************************
**
** Attal : Lords of Doom
**
** displayCreature.cpp
** widget for displaying, buying cretaures
**
** Version : $Id: displayCreature.cpp,v 1.35 2008/06/30 23:15:58 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 01/01/2001
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

#include "displayCreature.h"
 
// generic include files
#include <algorithm>
// include files for QT
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QGridLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

extern QString DATA_PATH;

//
// ----- DisplayCreature -----
//

DisplayCreature::DisplayCreature( QWidget * parent, const char * /* name */)
	: QFrame( parent )
{
	_creature = NULL;

	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	QVBoxLayout * info = new QVBoxLayout();
	info->addWidget( _name = new QLabel( this ) );
	info->addWidget( _picture = new QLabel( this ) );
	info->addWidget( _available = new QLabel( this ) );

	QGridLayout * stats = new QGridLayout();
	stats->addWidget( new QLabel( tr("Attack"), this ), 0, 0 );
	stats->addWidget( new QLabel( tr("Defense"), this ), 1, 0 );
	stats->addWidget( new QLabel( tr("Damage"), this ), 2, 0 );
	stats->addWidget( new QLabel( tr("Health"), this ), 3, 0 );
	stats->addWidget( new QLabel( tr("Speed"), this ), 4, 0 );
	stats->addWidget( new QLabel( tr("Growth"), this ), 5, 0 );
	stats->addWidget( _attack = new QLabel( this ), 0, 1, Qt::AlignRight );
	stats->addWidget( _defense = new QLabel( this ), 1, 1, Qt::AlignRight );
	stats->addWidget( _damage = new QLabel( this ), 2, 1, Qt::AlignRight );
	stats->addWidget( _health = new QLabel( this ), 3, 1, Qt::AlignRight );
	stats->addWidget( _speed = new QLabel( this ), 4, 1, Qt::AlignRight );
	stats->addWidget( _growth = new QLabel( this ), 5, 1, Qt::AlignRight );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addLayout( info );
	layout->addLayout( stats );
}

void DisplayCreature::setCreature( int race, int level , GenericBase * base)
{
	_creature = DataTheme.creatures.at( race , level );

	_name->setText( _creature->getName() );
	_picture->setPixmap( ImageTheme.getPhotoCreature( race, level ) );
	_picture->setFixedSize( 60, 60 );
	/// XXX: _building->setText( temp->getBuilding() );
	if(!base) {
		_available->setText( "Available : ??" );
	} else {
		QString tmp = "";
		tmp = tr("no building");
		if( base->canProduceCreature( _creature ) ) { 
			int prod = base->getCreatureProduction( _creature );
			if( prod > 0 ) {
				tmp = QString::number( prod );
			} else {
				tmp = tr("next week");
			}
		}

		QString ava = tr("Available : ") + tmp;
		_available->setText( ava );
	}

	_attack->setText( QString::number( _creature->getAttack() ) );
	_defense->setText( QString::number( _creature->getDefense() ) );
	_damage->setText( QString::number( _creature->getMinDamages() ) + QString("-") + QString::number( _creature->getMaxDamages() ) );
	_health->setText( QString::number( _creature->getMaxHealth() ) );
	_speed->setText( QString::number( _creature->getMaxMove() ) );

	/// XXX: to change...
	_growth->setText( QString::number( 4 ) );
}

void DisplayCreature::mousePressEvent( QMouseEvent * /*event*/ )
{
	emit sig_clicked();
}

//
// ----- BuyCreature -----
//

BuyCreature::BuyCreature( QWidget * parent, GenericPlayer * player, GenericBase * base, Creature * creature  )
	:QDialog( parent, Qt::Dialog )
{
	_max = 0;

	_labTitle = new QLabel( this );

	_ico = new Icon( this );

	_bar = new QScrollBar( Qt::Horizontal, this );
	_labAvail = new QLabel( QString::number( 0 ), this );
	_labBought = new QLabel( QString::number( 0 ), this );
	_individualCost = new CreatureCost( this );
	_boughtCost = new CreatureCost( this );

	_butAll = new QPushButton( this );
	_butAll->setText( tr( "All" ) );

	_butBuy = new QPushButton( this );
	_butBuy->setText( tr( "Buy" ) );

	QPushButton * butCan = new QPushButton( this );
	butCan->setText( tr( "Quit" ) );

	QGridLayout * layout = new QGridLayout( this );
	layout->setSizeConstraint( QLayout::SetFixedSize );
	layout->addWidget( _individualCost, 0, 0, 3, 1, Qt::AlignBottom | Qt::AlignRight );
	layout->addWidget( _labTitle, 0, 1, 1, 3, Qt::AlignHCenter | Qt::AlignBottom );
	_boughtCost->setMinimumWidth( 100 );
	layout->addWidget( _boughtCost, 0, 4, 3, 1, Qt::AlignBottom | Qt::AlignLeft );

	layout->addWidget( _ico, 1, 1, 1, 3, Qt::AlignHCenter );

	layout->addWidget( _labAvail, 2, 1, Qt::AlignRight );
	_bar->setMinimumWidth( 120 );
	layout->addWidget( _bar, 2, 2 );
	layout->setColumnMinimumWidth( 2, 150 );
	layout->setColumnStretch( 2, 1 );
	layout->addWidget( _labBought , 2, 3, Qt::AlignLeft );

	layout->addWidget( _butAll, 3, 0, 1, 2 );
	layout->addWidget( _butBuy, 3, 2 );
	layout->addWidget( butCan, 3, 3, 1, 2 );

	init( player, base, creature );

	connect( _bar, SIGNAL( valueChanged( int ) ), SLOT( slot_newValue( int ) ) );
	connect( parent, SIGNAL( sig_castle() ), SLOT( reinit() ) );
	connect( this, SIGNAL( sig_baseUnitBuy( Creature * , int  ) ), parent, SLOT( slot_baseUnitBuy( Creature * , int  ) ) );

	connect( butCan, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( _butBuy, SIGNAL( clicked() ), SLOT( slot_buy() ) );
	connect( _butAll, SIGNAL( clicked() ), SLOT( slot_all() ) );
}

void BuyCreature::init( GenericPlayer * player, GenericBase * base, Creature * creature )
{
	_player = player;
	_base = base;
	_creature = creature;
	/// XXX: todo
	int race = creature->getRace();
	int level = creature->getLevel();
	//Creature * creature = DataTheme.creatures.at( race, level );

	setWindowTitle( tr("Recruit ") + creature->getName() );
	_labTitle->setText( creature->getName() );
	_ico->setPixmap( ImageTheme.getPhotoCreature( race, level ) );
	/// XXX: use prodBase when init of base will be done correctly...
	int prodBase = base->getCreatureProduction( creature );
	int prodPlayer = player->computeBuyCreatureMax( creature );
	//logDD(" creature %p, prodBase%d, prodPlayer %d", creature, prodBase, prodPlayer );
	
	_max = std::min( prodBase, prodPlayer );
	_individualCost->init( creature );
	_individualCost->setNumber( 1 );
	_boughtCost->init( creature );
	_boughtCost->setNumber( 0 );
	_bar->setMaximum( _max );
	_labAvail->setText( QString::number( _max ) ); // why doesn't prodBase work?
	_labBought->setText( QString::number( 0 ) );
	
	_butAll->setEnabled( _max );
	_butBuy->setEnabled( false );
}

void BuyCreature::reinit()
{
	int prodBase = _base->getCreatureProduction( _creature );
	int prodPlayer = _player->computeBuyCreatureMax( _creature );
	//logDD(" creature %p, prodBase%d, prodPlayer %d", creature, prodBase, prodPlayer );

	_max = std::min( prodBase, prodPlayer );
	_boughtCost->setNumber( 0 );
	_bar->setMaximum( _max );
	_bar->setSliderPosition( 0 );
	_labAvail->setText( QString::number( _max ) ); // why doesn't prodBase work?
	_labBought->setText( QString::number( 0 ) );
	
	_butAll->setEnabled( _max );
}

void BuyCreature::slot_newValue( int value ) {
	_boughtCost->setNumber( value );
	_labAvail->setText( QString::number( _max - value ) );
	_labBought->setText( QString::number( value ) );
	
	_butBuy->setEnabled( value );
}

void BuyCreature::slot_all()
{ 
	if( _base->canAddGarrison( _creature ) ) {
		if( _player->canBuy( _creature, _max ) ) {
			emit sig_baseUnitBuy( _creature, _max );
		}
	} else {
		QMessageBox::warning( this, tr("No room left"), tr("No room left for a new unit") );
	}
}

void BuyCreature::slot_buy()
{
	if( _base->canAddGarrison( _creature ) ) {
		if( _bar->value() > 0 ) {
			if( _player->canBuy( _creature, _bar->value() ) ) {
				emit sig_baseUnitBuy( _creature, _bar->value() );
			}
		}
	} else {
		QMessageBox::warning( this, tr("No room left"), tr("No room left for a new unit") );
	}
}

//
// ----- CreatureCost -----
//

CreatureCost::CreatureCost( QWidget * parent, const char * /* name */)
: QFrame( parent )
{
	_nb = 0;
	_creature = 0;

	QVBoxLayout * layout = new QVBoxLayout( this );

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		_res[i] = new ResourceLabel( this );
		_res[i]->setResource( i );
		layout->addWidget( _res[i] );
		_res[i]->hide();
	}

	layout->addStretch( 1 );
	layout->activate();
}

void CreatureCost::init( Creature * creature )
{
	_nb = 0;
	_creature = creature;
	reinit();
}

void CreatureCost::setNumber( unsigned int nb )
{
	_nb = nb;
	reinit();
}

void CreatureCost::reinit()
{
	if( _creature ) {
		for( int i = 0; i < DataTheme.resources.count(); i++)  {
			if( _creature->getCost( i ) > 0 ) {
				_res[i]->setNumber( _creature->getCost( i ) * _nb );
				_res[i]->show();
				_res[i]->setFixedHeight( 30 );
			} else {
				_res[i]->hide();
			}
		}
	}
}

