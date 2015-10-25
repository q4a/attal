/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesCreature.cpp
** Allow selection of specific creature panels
**
** Author(s) : Nickolay Semenov
**
** Date : 18/06/2003
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

#include "propertiesCreature.h"

// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericMapCreature.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"

PropertiesCreature::PropertiesCreature (QWidget * parent, const char * name)
    :QDialog (parent, Qt::Dialog)
{
	setWindowTitle( QString ( name));
	_creaturePix = new QLabel( this );
	
	_resCrea= new AskCost( tr( "Res Crea: " ), this );

	_creatureBehaviour = new AskCombo( tr( "Behaviour of creatures: " ), this );
	_creatureBehaviour->insertItem( GenericMapCreature::getBehaviourString( GenericMapCreature::Obedient ) );
	_creatureBehaviour->insertItem( GenericMapCreature::getBehaviourString( GenericMapCreature::Friendly ) );
	_creatureBehaviour->insertItem( GenericMapCreature::getBehaviourString( GenericMapCreature::Neutral ) );
	_creatureBehaviour->insertItem( GenericMapCreature::getBehaviourString( GenericMapCreature::Aggressive ) );
	_creatureBehaviour->insertItem( GenericMapCreature::getBehaviourString( GenericMapCreature::Hostile ) );

	_creatureFlee = new AskBool( tr( "Never flee: " ), this );
	_lookingRight = new AskBool( tr( "Looking at right side: " ), this );
	
	_creatureGrowth = new AskCombo( tr( "Growth of creatures: " ), this );
	_creatureGrowth->insertItem( GenericMapCreature::getGrowthString( GenericMapCreature::Stable ) );
	_creatureGrowth->insertItem( GenericMapCreature::getGrowthString( GenericMapCreature::FixedPercentage ) );
	_creatureGrowth->insertItem( GenericMapCreature::getGrowthString( GenericMapCreature::VariablePercentage ) );
	
	_growthParam0 = new AskInt( tr( "Growth param 0: " ), this );
	_growthParam0->setMinValue( 0 ); 
	_growthParam0->setMaxValue( 1000 );
	_growthParam0->setEnabled( false );
	_growthParam1 = new AskInt( tr( "Growth param 1: " ), this );
	_growthParam1->setMinValue( 0 );
	_growthParam1->setMaxValue( 1000 );
	_growthParam0->setEnabled( false );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setMargin( 5 );
	layV1->addWidget( _creaturePix, 1 );
	layV1->addSpacing( 10 );
	layV1->addWidget( _creatureBehaviour, 1 );
	layV1->addWidget( _creatureFlee, 1 );
	layV1->addWidget( _lookingRight, 1 );
	layV1->addWidget( _creatureGrowth, 1 );
	layV1->addWidget( _growthParam0, 1 );
	layV1->addWidget( _growthParam1, 1 );

	QVBoxLayout * layV2 = new QVBoxLayout();
	layV2->setMargin( 5 );
	layV2->setSpacing( 5 );
	layV2->addStretch( 1 );
	AskInt * stack;
	QString label;
	_creatureStacks.resize( MAX_UNIT );
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		label = QString (tr( "Number of creatures in stack %1: " ).arg(i));
		stack = new AskInt( label, this );
		stack->setMinValue( 0 );
		stack->setMaxValue( 10000 );
		layV2->addWidget( stack );
		layV2->addStretch( 1 );
		if( _creatureStacks[ i] ) {
		 delete _creatureStacks[ i];
		}
		_creatureStacks[ i] = stack ;
	}

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setSpacing( 5 );
	layH1->setMargin( 5 );
	layH1->addLayout( layV1, 1 );
	layH1->addLayout( layV2, 1 );

	QPushButton * butOk = new QPushButton( this );
	butOk->setText( tr( "Ok" ) );
	FIXEDSIZE( butOk );

	QPushButton * butCan = new QPushButton( this );
	butCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( butCan );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setMargin( 5 );
	layH2->setSpacing( 5 );
	layH2->addStretch( 1 );
	layH2->addWidget( butOk );
	layH2->addStretch( 1 );
	layH2->addWidget( butCan );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1, 1 );
	layout->addWidget( _resCrea );
	layout->addLayout( layH2 );
	layout->activate();

	connect( butOk, SIGNAL( clicked() ), this, SLOT( slot_accept() ) );
	connect( butCan, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( _creatureGrowth, SIGNAL( sig_activated( int ) ), SLOT( slot_growthMode( int ) ) );
}

void PropertiesCreature::init( GenericMapCreature * creature )
{
	_creature = creature;
	_creaturePix->setPixmap( ImageTheme.getMapCreature( creature ) );
	_creatureBehaviour->setCurrentItem( (int)creature->getBehaviour() );
	_creatureFlee->setValue( ! creature->isFleeing() );
	_lookingRight->setValue( creature->isLookingRight() );
	_creatureGrowth->setCurrentItem( (int)creature->getGrowthMode() );
	_growthParam0->setValue( creature->getGrowthParam( 0 ) );
	_growthParam1->setValue( creature->getGrowthParam( 1 ) );
	slot_growthMode( _creatureGrowth->currentItem() );
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		_creatureStacks[i]->setValue( creature->getStack( i ) );
	}
	
	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
		_resCrea->setValue( i , _creature->getResourceList()->getValue( i ) );
	}

}

void PropertiesCreature::slot_accept()
{

	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
			_creature->getResourceList()->setValue( i, _resCrea->getValue( i ) );
	}

	_creature->setGrowthMode( (GenericMapCreature::GrowthMode)_creatureGrowth->currentItem() );
	_creature->setGrowthParam( 0, _growthParam0->getValue() );
	_creature->setGrowthParam( 1, _growthParam1->getValue() );
	_creature->setBehaviour( (GenericMapCreature::CreatureBehaviour)_creatureBehaviour->currentItem() );
	_creature->setFleeing( ! _creatureFlee->getValue() );
	_creature->setLookingRight( _lookingRight->getValue() );
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		_creature->setStack( i, _creatureStacks[i]->getValue() );
	}
	accept();
}

void PropertiesCreature::slot_growthMode( int mode )
{
	if( mode == 0 ) {
		_growthParam0->setEnabled( false );
		_growthParam1->setEnabled( false );
	} else if( mode == 1 ) {
		_growthParam0->setEnabled( true );
		_growthParam1->setEnabled( false ); 
	} else {
		_growthParam0->setEnabled( true );
		_growthParam1->setEnabled( true );
	}	
}


