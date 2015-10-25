/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesEvent.cpp
** Defines properties box for events on map
**
** Version : $Id: propertiesEvent.cpp,v 1.9 2006/11/03 18:28:59 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 24/04/2004
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

#include "propertiesEvent.h"

// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericChest.h"
#include "libCommon/genericEvent.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

PropertiesEvent::PropertiesEvent( GenericEvent * event, QWidget * parent, const char * /* name */ )
: QDialog( parent, Qt::Dialog )
{
	_event = event;

	createContentView();

	QPushButton * pbCondition = new QPushButton( this );
	pbCondition->setText( tr( "Condition" ) );
	FIXEDSIZE( pbCondition );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbCondition );
	layH1->addStretch( 1 );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addStretch( 1 );
	layH2->addWidget( pbCan );
	layH2->addStretch( 1 );
	layH2->addWidget( pbOk );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _content, 1 );
	layout->addLayout( layH1 );
	layout->addLayout( layH2 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
}

void PropertiesEvent::slot_accept()
{
	_content->save();
	accept();
}

void PropertiesEvent::createContentView()
{
	_content = 0;

	if( _event ) {
		switch( _event->getType() ) {
		case GenericEvent::EventArtefact:
			_content = (PropertiesEventContent *)( new PropertiesArtefact( _event->getArtefact(), this ) );
			break;
		case GenericEvent::EventBonus:
			_content = (PropertiesEventContent *)( new PropertiesBonus( _event->getBonus(), this ) );
			break;
		case GenericEvent::EventChest:
			_content = (PropertiesEventContent *)( new PropertiesChest( _event->getChest(), this ) );
			break;
		default:
			logEE( "Should not happen" );
		}
	}
}

//
//----- PropertiesArtefact -----
//

PropertiesArtefact::PropertiesArtefact( GenericArtefact * artefact, QWidget * parent, const char * name )
: PropertiesEventContent( parent, name )
{
	_artefact = artefact;
}
//
//----- PropertiesBonus -----
//

PropertiesBonus::PropertiesBonus( GenericBonus * bonus, QWidget * parent, const char * /* name*/ )
: PropertiesEventContent( parent )
{
	_bonus = bonus;

	switch( _bonus->getType() ) {
	case GenericBonus::BonusResource:
		displayResource();
		break;
	case GenericBonus::BonusPrimSkill:
		displayPrimSkill();
		break;
	case GenericBonus::BonusSkill:
		displaySkill();
		break;
	case GenericBonus::BonusSpell:
		displaySpell();
		break;
	}
}

void PropertiesBonus::displayResource()
{
	QLabel * labResource = new QLabel( this );
	labResource->setText( tr( "Resource: " ) + DataTheme.resources.getRessource( _bonus->getParam( 0 ) ) );
	FIXEDSIZE( labResource );

	_fixed = new AskInt( tr( "Fixed value: " ), this );
	_fixed->setMinValue( 0 );
	_fixed->setMaxValue( 10000 );
	_fixed->setValue( _bonus->getParam( 2 ) );
	FIXEDSIZE( _fixed );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( labResource );
	layout->addWidget( _fixed );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesBonus::displayPrimSkill()
{
	_primSkill = new AskCombo( tr( "Primary skill: " ), this );
	_primSkill->insertItem( tr( "Attack" ) );
	_primSkill->insertItem( tr( "Defense" ) );
	_primSkill->insertItem( tr( "Power" ) );
	_primSkill->insertItem( tr( "Knowledge" ) );
	_primSkill->insertItem( tr( "Morale" ) );
	_primSkill->insertItem( tr( "Luck" ) );
	_primSkill->insertItem( tr( "Charisma" ) );
	_primSkill->setCurrentItem( _bonus->getParam( 0 ) );

	_value = new AskInt( tr( "Add value: " ), this );
	_value->setValue( _bonus->getParam( 1 ) );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _primSkill );
	layout->addWidget( _value );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesBonus::displaySkill()
{
	QLabel * lab = new QLabel( this );
	lab->setText( tr( "Bonus for secondary skills are not yet implemented" ) );
	FIXEDSIZE( lab );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( lab );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesBonus::displaySpell()
{
	QLabel * lab = new QLabel( this );
	lab->setText( tr( "Bonus for spells are not yet implemented" ) );
	FIXEDSIZE( lab );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( lab );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesBonus::save()
{
	switch( _bonus->getType() ) {
	case GenericBonus::BonusResource: {
		uint resource = _bonus->getParam( 0 );
		_bonus->clearParams();
		_bonus->addParam( resource );
		_bonus->addParam( 0 );
		_bonus->addParam( _fixed->getValue() );
		} break;
	case GenericBonus::BonusPrimSkill:
		_bonus->clearParams();
		_bonus->addParam( _primSkill->currentItem() );
		_bonus->addParam( _value->getValue() );
		break;
	case GenericBonus::BonusSkill:
		break;
	case GenericBonus::BonusSpell:
		break;
	}
}

//
//----- PropertiesChest -----
//

PropertiesChest::PropertiesChest( GenericChest * chest, QWidget * parent, const char * /*name*/ )
: PropertiesEventContent( parent )
{
	_chest = chest;
}




