/****************************************************************
**
** Attal : Lords of Doom
**
** sectionCreature.cpp
** section for specifying creatures
**
** Version : $Id: sectionCreature.cpp,v 1.4 2003/04/11 10:11:45 audoux Exp $
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
// include files for QT
#include <qlayout.h>
#include <qlist.h>
// application specific include files
#include "common/log.h"
#include "common/dataTheme.h"
#include "common/genericLord.h"
#include "common/unit.h"

extern DataTheme DataTheme;
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
	_selectRace->setTitle( "Race" );
	layout->addWidget( _selectRace );
	
	_nameRace = new AskString( "Name : ", this );
	layout->addWidget( _nameRace );
	
	_selectCreature = new SelectionWidget( this );
	_selectCreature->setTitle( " Creature " );
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
	QString text;
	text.sprintf( " Race : %d/%d",  ( _race + 1 ), DataTheme.creatures.countRace() );
	_selectRace->setTitle( text );

	if( _race < DataTheme.creatures.countRace() ) {
		text.sprintf( " Creature : %d/%d", ( _creature + 1 ), DataTheme.creatures.getRace( _race )->count() );
	} else {
		text.sprintf( " Creature : %d/??", _creature + 1 );
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
	DataTheme.creatures.QList<Race>::append( race );
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
	DataTheme.creatures.remove( _race );
	int newRace = QMIN( _race, DataTheme.creatures.countRace() - 1 );
	int newCreature = QMIN( _creature, DataTheme.creatures.getRace( newRace )->count() - 1 );

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		GenericLordModel * lord = DataTheme.lords.at( i );
		
		for( int j = 0; j < MAX_UNIT; j++ ) {
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
	_creature = QMAX( 0, _creature-1 );
	init();	
}

void SectionCreature::slot_nextCreature()
{
	save();
	_creature = QMIN( DataTheme.creatures.getRace( _race )->count() - 1, _creature + 1 );
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
	DataTheme.creatures.getRace( _race )->remove( _creature );
	int newCreature = QMIN( _creature, DataTheme.creatures.getRace( _race )->count() - 1 );

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		GenericLordModel * lord = DataTheme.lords.at( i );
		
		for( int j = 0; j < MAX_UNIT; j++ ) {
			GenericFightUnit * unit = lord->getUnit( j );
			if( ( unit ) && ( unit->getLevel() == _creature ) ) {
				unit->setCreature( _race, newCreature );
			}
		}
	}	
	
	_creature = newCreature;
}

/*************************************************************************/

EditCreature::EditCreature( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( "Name : ", this );
	layout->addWidget( _name );
	
	_ptAttack = new AskInt( "Attack : ", this );
	layout->addWidget( _ptAttack );
	
	_ptDefense = new AskInt( "Defense : ", this );
	layout->addWidget( _ptDefense );
	
	_maxHealth = new AskInt( "Max Health : ", this );
	layout->addWidget( _maxHealth );
	
	_maxMove = new AskInt( "Max Move : ", this );
	layout->addWidget( _maxMove );
	
	_distAttack = new AskInt( "Dist Attack : ", this );
	layout->addWidget( _distAttack );
	
	_minDamages = new AskInt( "Min Damages : ", this );
	layout->addWidget( _minDamages );
	
	_maxDamages = new AskInt( "Max Damages : ", this );
	layout->addWidget( _maxDamages );
	
	_morale = new AskInt( "Morale : ", this );
	layout->addWidget( _morale );
	
	_luck = new AskInt( "Luck : ", this );
	layout->addWidget( _luck );
	
	_pix = new AskPixmap( true, "", "Picture: ", this );
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
	unit->setLuck( _luck->getValue() );
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
	_luck->setValue( unit->getLuck() );
	QString name;
	name.sprintf( "%sunits/cre_%d_%d.png", IMAGE_PATH.latin1(), race, creature );
	_pix->setDestination( name );
}
