/****************************************************************
**
** Attal : Lords of Doom
**
** sectionLord.cpp
** section for specifying lords
**
** Version : $Id: sectionLord.cpp,v 1.21 2009/11/11 00:21:57 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

#include "sectionLord.h"
 
// generic include files
#include <algorithm>
// include files for QT
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/unit.h"
#include "libCommon/log.h"

#include "libClient/gui.h"
#include "libClient/askCost.h"

extern QString IMAGE_PATH;

SectionLord::SectionLord( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	setTitle( tr( "Lord" ) );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_bigPix = new AskPixmap( true, "", tr( "Photo: " ), _mainWidget );
	layout->addWidget( _bigPix );

	_smallPix = new AskPixmap( true, "", tr( "Icon: " ), _mainWidget );
	layout->addWidget( _smallPix );

	_name = new AskString( tr( "Name : " ), _mainWidget );
	layout->addWidget( _name );

	_category = new AskCombo( tr( "Category : " ), _mainWidget );
	layout->addWidget( _category );

	_attack = new AskInt( tr( "Attack : " ), _mainWidget );

	_defense = new AskInt( tr( "Defense : " ), _mainWidget );

	_movemax = new AskInt( tr( "MaxMoves : " ), _mainWidget );
	_movemax->setMaxValue( 9000 );

	_vision = new AskInt( tr( "Vision : " ), _mainWidget );

	QWidget * widg = new QWidget(this);
	
	QGridLayout * glayout = new QGridLayout( widg );
	glayout->setColumnStretch( 0, 1 );
	glayout->setColumnStretch( 1, 1 );
	glayout->addWidget( _attack, 0, 0,Qt::AlignLeft );
	glayout->addWidget( _defense, 0, 1 ,Qt::AlignLeft );
	glayout->addWidget( _movemax, 1, 0 ,Qt::AlignLeft );
	glayout->addWidget( _vision, 1, 1 ,Qt::AlignLeft );
	glayout->setRowStretch( 2, 1 );
	glayout->setMargin( 0 );
	widg->setFixedWidth( 450 );
	
	layout->addWidget( widg );
	
	_cost = new AskCost( tr( "Cost: " ), _mainWidget );
	layout->addWidget( _cost );

	_troops = new DisplayTroops( _mainWidget );
	layout->addWidget( _troops );

	layout->addStretch( 1 );
	layout->activate();

	if( DataTheme.lords.count() > 1 ) {
		_idLord = 1;
	} else {
		_idLord = 0;
	}
	updateData();
	selectFirst();
}

void SectionLord::updateData()
{
	LordCategoryModel * category;
	QString str;
	_category->clear();

	uint nbCategory = DataTheme.lordCategories.count();
	for( uint i = 0; i < nbCategory; i++ ) {
		category = DataTheme.lordCategories.at( i );
		str = tr( " (Race: " ) + DataTheme.creatures.getRace( category->getRace() )->getName() + ")";
		_category->insertItem( category->getName() + str );
	}

	_troops->updateData();
	init();
}

void SectionLord::clear()
{
	_name->setValue( "" );
	_category->setCurrentItem( 0 );
	_attack->setValue( 1 );
	_defense->setValue( 1 );
	_movemax->setValue( 16 );
	_vision->setValue( 5 );
	_troops->clear();
	_bigPix->setValue( "" );
	_smallPix->setValue( "" );
}

void SectionLord::init()
{
	GenericLordModel * lord = DataTheme.lords.at( _idLord );

	if( _idLord > 0 ) {
		_name->setValue( lord->getName() );
		_category->setCurrentItem( DataTheme.getLordCategory( lord->getCategory() ) );
		_attack->setValue( lord->getBaseCharac( ATTACK ) );
		_defense->setValue( lord->getBaseCharac( DEFENSE ) );
		_movemax->setValue( lord->getBaseCharac( MAXMOVE ) );
		_vision->setValue( lord->getBaseCharac( VISION ) );
		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			_cost->setValue( i, lord->getCost( i ) );
		}
		QString dest; 
		dest.sprintf( "lords/lord_%03d.png", _idLord );
		_bigPix->setDestination( IMAGE_PATH + dest );
		dest.sprintf( "lords/smallLord_%03d.png", _idLord );
		_smallPix->setDestination( IMAGE_PATH + dest );
		_troops->init( lord );
	} else {
		clear();
	}
}

void SectionLord::selectFirst()
{
	save();
	if( DataTheme.lords.count() > 1 ) {
		_idLord = 1;
		init();
	} else {
		_idLord = 0;
	}
	init();
}

void SectionLord::selectPrevious()
{
	save();
	if( _idLord > 1 ) {
		_idLord--;
	}
	init();
}

void SectionLord::selectNext()
{
	save();
	if( _idLord < DataTheme.lords.count() - 1 ) {
		_idLord++;
	}
	init();
}

void SectionLord::selectLast()
{
	save();
	_idLord = DataTheme.lords.count() - 1;
	init();
}

void SectionLord::selectNew()
{
	uchar numLords = DataTheme.lords.count();
	if( numLords < GenericLord::MAX_LORDS-1 ) {
		save();
		_idLord = DataTheme.lords.count();
		GenericLordModel * lord = new GenericLordModel();
		DataTheme.lords.append( lord );
		init();
	}
}

void SectionLord::selectDel()
{
	if( _idLord > 0 ) {
		clear();
		delete DataTheme.lords.takeAt( _idLord );
		_idLord = std::min( _idLord, DataTheme.lords.count() - 1 );
		init();
	} else {
		///XXX: warning, can't delete lord (list empty)
	}
}

void SectionLord::save()
{
	GenericLordModel * lord = DataTheme.lords.at( _idLord );

	if( _idLord > 0 ) {
		lord->setName( _name->getValue() );
		lord->setCategory( DataTheme.lordCategories.at( _category->currentItem() ) );
		lord->setBaseCharac( ATTACK, _attack->getValue() );
		lord->setBaseCharac( DEFENSE, _defense->getValue() );
		lord->setBaseCharac( MAXMOVE, _movemax->getValue() );
		lord->setBaseCharac( VISION, _vision->getValue() );
		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			lord->setCost( i, _cost->getValue( i ) );
		}
		_bigPix->save();
		_smallPix->save();
		_troops->save( lord );
	}
}


//
// ----- DisplayTroops -----
//

DisplayTroops::DisplayTroops( QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	int i;

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	QHBoxLayout * lay2 = new QHBoxLayout();
	lay2->setMargin( 5 );
	lay2->setSpacing( 5 );

	for( i = 0; i < 4; i++ ) {
		_troop[i] = new EditTroop( i, this );
		lay2->addWidget( _troop[i] );
	}
	lay2->addStretch( 1 );
	layout->addLayout( lay2 );
	
	QHBoxLayout * lay3 = new QHBoxLayout();
	lay3->setMargin( 5 );
	lay3->setSpacing( 5 );
	
	for( i = 4; i < MAX_UNIT; i++ ) {
		_troop[i] = new EditTroop( i, this );
		lay3->addWidget( _troop[i] );
	}
	lay3->addStretch( 1 );
	layout->addLayout( lay3 );
		
	layout->addStretch( 1 );
	layout->activate();
}

void DisplayTroops::clear()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->clear();
	}
}

void DisplayTroops::init( GenericLordModel * lord )
{
	if( lord ) {
		for( int i = 0; i < MAX_UNIT; i++ ) {
			_troop[i]->init( lord->getUnit( i ) );
		}
	} else {
		clear();
	}
}

void DisplayTroops::save( GenericLordModel * lord )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->save( i, lord );
	}	
}

void DisplayTroops::updateData()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->updateData();
	}	
}

//
// ----- EditTroop -----
//

EditTroop::EditTroop( int num, QWidget * parent, const char * /* name*/ )
	: QFrame( parent )
{
	_unit = 0;
	setFrameStyle( QFrame::Panel | QFrame::Raised );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	QLabel * label = new QLabel( this );
	label->setText(  "Unit " + QString::number( num )  );
	FIXEDSIZE( label );
	layout->addWidget( label );

	_race = new AskCombo( tr( "Race : " ), this );
	_race->setCurrentItem( 0 );
	layout->addWidget( _race );

	_name = new AskCombo( tr( "Name : " ), this );
	layout->addWidget( _name );

	_number = new AskInt( tr( "Number : " ), this );
	layout->addWidget( _number );
	
	layout->addStretch( 1 );
	layout->activate();
	
	updateData();
	
	connect( _race, SIGNAL( sig_activated( int ) ), SLOT( slot_raceChanged( int ) ) );
}

void EditTroop::slot_raceChanged( int race )
{
	_name->clear();
	for( int i = 0; i < DataTheme.creatures.getRace( race )->count(); i++ ) {
		_name->insertItem( DataTheme.creatures.getRace( race )->at( i )->getName() );
	}	
}

void EditTroop::updateData()
{	
	int race = _race->currentItem();

	_race->clear();
	for( uint i = 0; i < DataTheme.countRace(); i++ ) {
		_race->insertItem( DataTheme.creatures.getRace( i )->getName() );
	}
	if( _unit ) {
		_race->setCurrentItem( _unit->getRace() );
	} else {
		_race->setCurrentItem( race );
	}

	_name->clear();
	if(_race->currentItem() >= 0) {
		for( int i = 0; i < DataTheme.creatures.getRace( _race->currentItem() )->count(); i++ ) {
			_name->insertItem( DataTheme.creatures.getRace( _race->currentItem() )->at( i )->getName() );
		}
	}
}

void EditTroop::clear()
{
	_race->setCurrentItem( 0 );
	_name->setCurrentItem( 0 );
	_number->setValue( 0 );
}

void EditTroop::init( GenericFightUnit * unit )
{
	_unit = unit;
	if( unit ) {
		_race->setCurrentItem( unit->getRace() );
		slot_raceChanged( unit->getRace() );
		_name->setCurrentItem( unit->getLevel() );
		_number->setValue( unit->getNumber() );
	} else {
		clear();
	}
}

void EditTroop::save( int i, GenericLordModel * lord )
{
	GenericFightUnit * unit = lord->getUnit( i );
	
	if( _number->getValue() > 0 ) {
		if( ! unit ) {
			unit = new GenericFightUnit();
		}
		unit->setCreature( _race->currentItem(), _name->currentItem() );
		unit->setNumber( _number->getValue() );
		lord->setUnit( i, unit );
	} else {
		if( unit ) {
			delete unit;
		}
		lord->setUnit( i, 0 );
	}
}
