/****************************************************************
**
** Attal : Lords of Doom
**
** displayCreature.cpp
** widget for displaying, buying cretaures
**
** Version : $Id: displayCreature.cpp,v 1.12 2002/09/08 14:07:31 audoux Exp $
**
** Author(s) : Pascal Audoux
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
// include files for QT
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qscrollbar.h>
// application specific include files
#include "common/attalSocket.h"
#include "common/dataTheme.h"
#include "common/genericPlayer.h"
#include "common/log.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/widget.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;
extern DataTheme DataTheme;
extern ImageTheme ImageTheme;

//
// ----- DisplayCreature -----
//

DisplayCreature::DisplayCreature( QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_creature = 0;
	setFrameStyle( Box | Raised );
	setLineWidth( 1 );
	setMidLineWidth( 1 );

	QHBoxLayout * layout = new QHBoxLayout( this );

	_creaturePres = new CreaturePresentation( this );
	layout->addWidget( _creaturePres, 3 );
	layout->addSpacing( 2 );

	_creatureInfo = new CreatureInfo( this );
	layout->addWidget( _creatureInfo, 1 );

	layout->activate();
}

void DisplayCreature::setCreature( int race, int level )
{
	_creaturePres->setCreature( race, level );
	_creatureInfo->setCreature( race, level );
}

void DisplayCreature::mousePressEvent( QMouseEvent * event )
{
	emit sig_clicked();
}

//
// ----- CreaturePresentation -----
//

CreaturePresentation::CreaturePresentation( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	QVBoxLayout * layV1 = new QVBoxLayout();
	_name = new QLabel( this );
	layV1->addWidget( _name );
	layV1->addStretch( 1 );

	_picture = new QLabel( this );
	_picture->setFixedSize( 60, 60 );

	layV1->addStretch( 1 );

	_building = new QLabel( this );
	layV1->addWidget( _building );

	_available = new QLabel( this );
	layV1->addWidget( _available );

	layout->addLayout( layV1, 1 );

	layout->addStretch( 1 );

	layout->activate();
}

void CreaturePresentation::setCreature( int race, int level )
{
	Creature * crea = DataTheme.creatures.at( race, level );

	_name->setText( crea->getName() );
	QString pic = IMAGE_PATH + "units/face_";
	pic += QString::number( race ) + "_" + QString::number( level ) + ".png";
	_picture->setPixmap( pic );
        /// XXX: _building->setText( temp->getBuilding() );

	/// XXX: to change
	_available->setText( "Available : ??" );
}

//
// ----- CreatureInfo -----
//

CreatureInfo::CreatureInfo( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );

	_attack = new InfoFrame( INF_ATTACK, this );
	layout->addWidget( _attack );

	_defense = new InfoFrame( INF_DEFENSE, this );
	layout->addWidget( _defense );

	_damage = new InfoFrame( INF_DAMAGE, this );
	layout->addWidget( _damage );

	_health = new InfoFrame( INF_HEALTH, this );
	layout->addWidget( _health );

	_speed = new InfoFrame( INF_SPEED, this );
	layout->addWidget( _speed );

	_growth = new InfoFrame( INF_GROWTH, this );
	layout->addWidget( _growth );

	layout->addStretch( 1 );

	layout->activate();
}

void CreatureInfo::setCreature( int race, int level )
{
	Creature * crea = DataTheme.creatures.at( race, level );
	_attack->setValue( crea->getAttack() );
	_defense->setValue( crea->getDefense() );
	_damage->setValue( crea->getMinDamages(), crea->getMaxDamages() );
	_health->setValue( crea->getMaxHealth() );
	_speed->setValue( crea->getMaxMove() );

	/// XXX: to change...
	_growth->setValue( 4 );
}

//
// ----- InfoFrame -----
//

InfoFrame::InfoFrame( TypeInfo info, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	QLabel * ico = new QLabel( this );
	ico->setFixedSize( 25, 25 );
	layout->addWidget( ico );
	
	QLabel * theType = new QLabel( this );
	switch( info ) {
	case INF_ATTACK:
		theType->setText( "Attack" );
		break;
	case INF_DEFENSE:
		theType->setText( "Defense" );
		break;
	case INF_ARROW:
		theType->setText( "Arrow" );
		break;
	case INF_DAMAGE:
		theType->setText( "Damage" );
		break;
	case INF_HEALTH:
		theType->setText( "Health" );
		break;
	case INF_SPEED:
		theType->setText( "Speed" );
		break;
	case INF_GROWTH:
		theType->setText( "Growth" );
		break;
	}
	
	layout->addWidget( theType );
	layout->addStretch( 1 );
	
	_value = new QLabel( this );
	layout->addWidget( _value );
	
	layout->activate();
	
	setFixedSize( 150, 25 );
}

void InfoFrame::setValue( int val )
{
	_value->setText( QString::number( val ) );
	FIXEDSIZE( _value );
}

void InfoFrame::setValue( int val1, int val2 )
{
	QString temp;
	if( val1 != val2 )
		temp.sprintf( "%d-%d", val1, val2 ); 
	else
		temp.sprintf( "%d", val1 );
	_value->setText( temp );
	FIXEDSIZE( _value );
}

//
// ----- BuyCreature -----
//

BuyCreature::BuyCreature( QWidget * parent, const char * name )
	:QDialog( parent, name, true )
{
	_socket = 0;
	_max = 0;
	_base = 0;
	_player = 0;

	QLabel * title = new QLabel( this );
	title->setText( "Recruit creature" );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	_ico = new Icon( this );
	layH1->addWidget( _ico );
	layH1->addStretch( 1 );

	_cost = new ComputeCost( this );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addStretch( 2 );
	QPushButton * butAll = new QPushButton( this );
	butAll->setText( "All" );
	FIXEDSIZE( butAll );
	layH2->addWidget( butAll );
	layH2->addStretch( 1 );
	QPushButton * butBuy = new QPushButton( this );
	butBuy->setText( "Buy" );
	FIXEDSIZE( butBuy );
	layH2->addWidget( butBuy );
	layH2->addStretch( 1 );
	QPushButton * butCan = new QPushButton( this );
	butCan->setText( "Cancel" );
	FIXEDSIZE( butCan );
	layH2->addWidget( butCan );
	layH2->addStretch( 2 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( title );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->addStretch( 1 );
	layout->addWidget( _cost );
	layout->addSpacing( 10 );

	layout->addLayout( layH2, 1 );
	layout->activate();

	connect( butCan, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( butBuy, SIGNAL( clicked() ), SLOT( slot_buy() ) );
	connect( butAll, SIGNAL( clicked() ), SLOT( slot_all() ) );
}

void BuyCreature::init( GenericPlayer * player, GenericBase * base, Creature * creature )
{
	_player = player;
	_base = base;
	_creature = creature;
	/// XXX: todo
	int race = 0; //creature->getRace();
	int level = 0; //creature->getLevel();
	//Creature * creature = DataTheme.creatures.at( race, level );

	setCaption( "Recruit " + creature->getName() );
	QString pic = IMAGE_PATH + "units/face_";
	pic += QString::number( race ) + "_" + QString::number( level ) + ".png";
	_ico->setPixmap( pic );
	_cost->init( creature, base->getCreatureProduction( creature ) );
}

void BuyCreature::slot_all()
{

}

void BuyCreature::slot_buy()
{
	if( _base->canAddGarrison( _creature ) ) {
		if( _cost->getNumber() > 0 ) {
			if( _player->canBuy( _creature, _cost->getNumber() ) ) {
				_socket->sendBaseUnit( _base, _creature, _cost->getNumber() );
				_base->buyCreature( _creature, _cost->getNumber() );
			}
		}
	} else {
		QMessageBox::warning( this, "No room left", "No room left for a new unit" );
	}
}

//
// ----- ComputeCost -----
//

ComputeCost::ComputeCost( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_max = 0;

	QVBoxLayout * layV1 = new QVBoxLayout();
	_bar = new QScrollBar( this );
	_bar->setOrientation( Qt::Horizontal );
	layV1->addWidget( _bar );

	_unitaryCost = new CreatureCost( this );
	_unitaryCost->setNumber( 1 );
	_totalCost = new CreatureCost( this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addStretch( 1 );
	layout->addWidget( _unitaryCost );
	layout->addStretch( 1 );
	layout->addLayout( layV1 );
	layout->addStretch( 1 );
	layout->addWidget( _totalCost );
	layout->addStretch( 1 );

	layout->activate();

	connect( _bar, SIGNAL( valueChanged( int ) ), SLOT( slot_newValue( int ) ) );
}

int ComputeCost::getNumber()
{
	return _bar->value();
}

void ComputeCost::init( Creature * creature, int max )
{
	_max = max;
	_bar->setMaxValue( max );
	_unitaryCost->init( creature );
	_unitaryCost->setNumber( 1 );
	_totalCost->init( creature );
}

void ComputeCost::slot_newValue( int value )
{
	_totalCost->setNumber( value );
}

//
// ----- CreatureCost -----
//

CreatureCost::CreatureCost( QWidget * parent, const char * name )
: QFrame( parent, name )
{
	_nb = 0;
	_creature = 0;

	QVBoxLayout * layout = new QVBoxLayout( this );

	for( int i = 0; i < MAX_RESS; i++ ) {
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
		for( int i = 0; i < MAX_RESS; i++)  {
			if( _creature->getCost( i ) > 0 ) {
				_res[i]->setNumber( _creature->getCost( i ) * _nb );
				_res[i]->show();
				_res[i]->setFixedHeight( 20 );
			} else {
				_res[i]->hide();
			}
		}
	}
}

