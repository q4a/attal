/****************************************************************
**
** Attal : Lords of Doom
**
** sectionCreature.cpp
** section for specifying creatures
**
** Version : $Id: sectionCreature.cpp,v 1.24 2009/11/04 22:34:29 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

#include "sectionCreature.h"
 
// generic include files
#include <algorithm>
// include files for QT
#include <QLayout>
#include <QList>
#include <QVBoxLayout>
// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/unit.h"

#include "libClient/askCost.h"

extern QString IMAGE_PATH;

SectionCreature::SectionCreature( QWidget * parent, const char * name )
	: Section( parent, name )
{
	_race = 0;
	_creature = 0;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_selectRace = new SelectionWidget( this );
	_selectRace->setTitle( tr( "Race" ) );
	layout->addWidget( _selectRace );

	_nameRace = new AskString( tr( "Name: " ), this );
	layout->addWidget( _nameRace );
	
	_selectCreature = new SelectionWidget( this );
	_selectCreature->setTitle( tr( " Creature " ) );
	layout->addWidget( _selectCreature );
	
	_editCreature = new EditCreature( this );
	layout->addWidget( _editCreature );
		
	layout->addStretch( 1 );
	
	layout->activate();
	
	connect( _selectRace, SIGNAL( sig_first() ), SLOT( slot_firstRace() ) );
	connect( _selectRace, SIGNAL( sig_previous() ), SLOT( slot_previousRace() ) );
	connect( _selectRace, SIGNAL( sig_next() ), SLOT( slot_nextRace() ) );
	connect( _selectRace, SIGNAL( sig_last() ), SLOT( slot_lastRace() ) );
	connect( _selectRace, SIGNAL( sig_new() ), SLOT( slot_newRace() ) );
	connect( _selectRace, SIGNAL( sig_del() ), SLOT( slot_delRace() ) );
	
	connect( _selectCreature, SIGNAL( sig_first() ), SLOT( slot_firstCreature() ) );
	connect( _selectCreature, SIGNAL( sig_previous() ), SLOT( slot_previousCreature() ) );
	connect( _selectCreature, SIGNAL( sig_next() ), SLOT( slot_nextCreature() ) );
	connect( _selectCreature, SIGNAL( sig_last() ), SLOT( slot_lastCreature() ) );
	connect( _selectCreature, SIGNAL( sig_new() ), SLOT( slot_newCreature() ) );
	connect( _selectCreature, SIGNAL( sig_del() ), SLOT( slot_delCreature() ) );
	
	init();
}

void SectionCreature::save()
{
	DataTheme.creatures.getRace( _race )->setName( _nameRace->getValue() );
	_editCreature->save( _race, _creature );
	emit sig_changed();
}

void SectionCreature::init()
{
	QString text = QString (tr(" Race : %1/%2").arg( _race + 1 ).arg(DataTheme.creatures.countRace() ));
	_selectRace->setTitle( text );

	if( _race < DataTheme.creatures.countRace() ) {
		text = QString (tr(" Creature : %1/%2").arg( _creature + 1 ).arg(DataTheme.creatures.getRace( _race )->count() ));
	} else {
		text = QString (tr(" Creature : %1/??").arg( _creature + 1) );
	}
	_selectCreature->setTitle( text );
	_nameRace->setValue( DataTheme.creatures.getRace( _race )->getName() );
	_editCreature->init( _race, _creature );
}

void SectionCreature::slot_firstRace()
{
	if( _race > 0 ) {
		save();
		_race = 0;
		_creature = 0;
		init();
	}
}

void SectionCreature::slot_previousRace()
{
	if( _race > 0 ) {
		save();
		_race--;
		_creature = 0;
		init();
	}	
}

void SectionCreature::slot_nextRace()
{
	if( _race < DataTheme.creatures.countRace() - 1 ) {
		save();
		_race++;
		_creature = 0;
		init();	
	}
}

void SectionCreature::slot_lastRace()
{
	if( _race < DataTheme.creatures.countRace() - 1 ) {
		save();
		_race = DataTheme.creatures.countRace() - 1;
		_creature = 0;
		init();	
	}	
}

void SectionCreature::slot_newRace()
{
	Race * race = new Race();
	Creature * creature = new Creature();
	race->append( creature );
	DataTheme.creatures.QList<Race *>::append( race );
	slot_lastRace();
}

void SectionCreature::slot_delRace()
{
	if( DataTheme.creatures.countRace() > 1 ) {
		save();
		removeRace();
		_creature = 0;
		init();
	}
}

void SectionCreature::removeRace()
{
	delete DataTheme.creatures.takeAt( _race );
	int newRace = std::min( _race, DataTheme.creatures.countRace() - 1 );
	int newCreature = std::min( (int) _creature, DataTheme.creatures.getRace( newRace )->count() - 1 );

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		GenericLordModel * lord = DataTheme.lords.at( i );

		for( uint j = 0; j < MAX_UNIT; j++ ) {
			GenericFightUnit * unit = lord->getUnit( j );
			if( ( unit ) && ( unit->getRace() == _race ) ) {
				unit->setCreature( newRace, newCreature );
			}
		}
	}
	
	_race = newRace;
}

void SectionCreature::slot_firstCreature()
{
	save();
	_creature = 0;
	init();
}

void SectionCreature::slot_previousCreature()
{
	save();
	_creature = (uint)std::max( 0, (int)_creature-1 );
	init();	
}

void SectionCreature::slot_nextCreature()
{
	save();
	_creature = std::min( DataTheme.creatures.getRace( _race )->count() - 1, (int)_creature + 1 );
	init();	
}

void SectionCreature::slot_lastCreature()
{
	save();
	_creature = DataTheme.creatures.getRace( _race )->count() - 1;
	init();	
}

void SectionCreature::slot_newCreature()
{
	Creature * creature = new Creature();
	DataTheme.creatures.getRace( _race )->append( creature );
	slot_lastCreature();
}

void SectionCreature::slot_delCreature()
{
	if( DataTheme.creatures.getRace( _race )->count() > 1 ) {
		save();
		removeCreature();
		init();
	}	
}

void SectionCreature::removeCreature()
{
	delete DataTheme.creatures.getRace( _race )->takeAt( _creature );
	int newCreature = std::min( (int)_creature, DataTheme.creatures.getRace( _race )->count() - 1 );

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		GenericLordModel * lord = DataTheme.lords.at( i );

		for( uint j = 0; j < MAX_UNIT; j++ ) {
			GenericFightUnit * unit = lord->getUnit( j );
			if( ( unit ) && ( unit->getLevel() == _creature ) ) {
				unit->setCreature( _race, newCreature );
			}
		}
	}	
	
	_creature = newCreature;
}

/*************************************************************************/

EditCreature::EditCreature( QWidget * parent, const char * /*name*/ )
	: QWidget( parent )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( tr( "Name: " ), this );
	layout->addWidget( _name );

	_ptAttack = new AskInt( tr( "Attack: " ), this );
	_ptDefense = new AskInt( tr( "Defense: " ), this );
	_maxHealth = new AskInt( tr( "Max Health: " ), this );
	_maxMove = new AskInt( tr( "Max Move: " ), this );
	_distAttack = new AskInt( tr( "Dist Attack: " ), this );
	_minDamages = new AskInt( tr( "Min Damages: " ), this );
	_maxDamages = new AskInt( tr( "Max Damages: " ), this );
	_morale = new AskInt( tr( "Morale: " ), this );
	_luck = new AskInt( tr( "Luck: " ), this );
	_size = new AskInt( tr( "Size: " ), this );
	
	QWidget * widg = new QWidget(this);
	
	QGridLayout * glayout = new QGridLayout( widg );
	glayout->setColumnStretch( 0, 1 );
	glayout->setColumnStretch( 1, 1 );
	glayout->addWidget( _ptAttack, 0, 0,Qt::AlignLeft );
	glayout->addWidget( _ptDefense, 0, 1 ,Qt::AlignLeft );
	glayout->addWidget( _maxHealth, 1, 0 ,Qt::AlignLeft );
	glayout->addWidget( _maxMove, 1, 1 ,Qt::AlignLeft );
	glayout->addWidget( _distAttack, 2, 0 ,Qt::AlignLeft );
	glayout->addWidget( _minDamages, 3, 0 ,Qt::AlignLeft );
	glayout->addWidget( _maxDamages, 3, 1 ,Qt::AlignLeft );
	glayout->addWidget( _morale, 4, 0 ,Qt::AlignLeft );
	glayout->addWidget( _luck, 4, 1 ,Qt::AlignLeft );
	glayout->addWidget( _size, 5, 0 ,Qt::AlignLeft );
	glayout->setRowStretch( 6, 1 );
	glayout->setMargin( 0 );
	widg->setFixedWidth( 450 );
	
	layout->addWidget( widg );

	_cost = new AskCost( tr( "Cost: " ), this );
	layout->addWidget( _cost );

	_maintCost = new AskCost( tr( "Maintenance cost: " ), this );
	layout->addWidget( _maintCost );
	
	_XOffset = new AskInt( tr( "X Offset: " ), this );
	_XOffset->setMinValue( -255 );
	
	_XOffsetMirr = new AskInt( tr( "X Offset mirror: " ), this );
	_XOffsetMirr->setMinValue( -255 );
	
	_YOffset = new AskInt( tr( "Y Offset: " ), this );
	_YOffset->setMinValue( -255 );
	
	QWidget * widg2 = new QWidget(this);
	
	QGridLayout * glayout2 = new QGridLayout( widg2 );
	glayout2->setColumnStretch( 0, 1 );
	glayout2->setColumnStretch( 1, 1 );
	glayout2->addWidget( _XOffset, 0, 0,Qt::AlignLeft );
	glayout2->addWidget( _XOffsetMirr, 0, 1 ,Qt::AlignLeft );
	glayout2->addWidget( _YOffset, 1, 0 ,Qt::AlignLeft );

	glayout2->setRowStretch( 2, 1 );
	glayout2->setMargin( 0 );
	widg2->setFixedWidth( 450 );
	
	layout->addWidget( widg2 );
	
	_numFrames = new AskInt( tr( "Num. Frames: " ), this );
	_numFrames ->setMinValue( 0 );
	layout->addWidget( _numFrames );
	
	_creatureAnimation = new CreatureAnimation( this );
	layout->addWidget( _creatureAnimation );

	_pix = new AskPixmap( true, "", tr( "Picture: " ), this );
	layout->addWidget( _pix );
	
	layout->addStretch( 1 );
	layout->activate();
}

void EditCreature::save( int race, int creature )
{
	Creature * unit = DataTheme.creatures.getRace( race )->at( creature );
	
	unit->setName( _name->getValue() );
	unit->setAttack( _ptAttack->getValue() );
	unit->setDefense( _ptDefense->getValue() );
	unit->setMaxHealth( _maxHealth->getValue() );
	unit->setMaxMove( _maxMove->getValue() );
	unit->setDistAttack( _distAttack->getValue() );
	unit->setMinDamages( _minDamages->getValue() );
	unit->setMaxDamages( _maxDamages->getValue() );
	unit->setMorale( _morale->getValue() );
	unit->setSize( _size->getValue() );
	unit->setLuck( _luck->getValue() );
	unit->setXOffset( _XOffset->getValue() );
	unit->setXOffsetMirror( _XOffsetMirr->getValue() );
	unit->setYOffset( _YOffset->getValue() );
	unit->setNumFrames( _numFrames->getValue() );
	uint nbRes = DataTheme.resources.count();
	for( uint i = 0; i < nbRes; i++ ) {
		unit->setCost( i, _cost->getValue( i ) );
		unit->setMantCost( i, _maintCost->getValue( i ) );
	}
	_creatureAnimation->save();

	_pix->save();
}

void EditCreature::init( int race, int creature )
{
	Creature * unit = DataTheme.creatures.getRace( race )->at( creature );

	_name->setValue( unit->getName() );
	_ptAttack->setValue( unit->getAttack() );
	_ptDefense->setValue( unit->getDefense() );
	_maxHealth->setValue( unit->getMaxHealth() );
	_maxMove->setValue( unit->getMaxMove() );
	_distAttack->setValue( unit->getDistAttack() );
	_minDamages->setValue( unit->getMinDamages() );
	_maxDamages->setValue( unit->getMaxDamages() );
	_morale->setValue( unit->getMorale() );
	_size->setValue( unit->getSize() );
	_luck->setValue( unit->getLuck() );
	_XOffset->setValue(unit->getXOffset());
	_XOffsetMirr->setValue(unit->getXOffsetMirror());
	_YOffset->setValue(unit->getYOffset());	
	_numFrames->setValue(unit->getNumFrames());	
	_creatureAnimation->init( unit );
	uint nbRes = DataTheme.resources.count();
	for( uint i = 0; i < nbRes; i++ ) {
		_cost->setValue( i, unit->getCost( i ) );
		_maintCost->setValue( i, unit->getMantCost( i ) );
	}
	QString name;
	name = QString( IMAGE_PATH + "units/race_" + QString::number( race ) + "/level_" + QString::number( creature ) + "/creature.png" );
	_pix->setDestination( name );
}
