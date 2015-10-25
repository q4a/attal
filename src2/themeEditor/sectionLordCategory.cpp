/****************************************************************
**
** Attal : Lords of Doom
**
** sectionLordCategory.cpp
** section for editing lord categories
**
** Version : $Id: sectionLordCategory.cpp,v 1.7 2009/09/11 13:00:05 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/05/2004
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

#include "sectionLordCategory.h"
 
// generic include files
#include <algorithm>

// include files for QT
#include <QLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

SectionLordCategory::SectionLordCategory( QWidget * parent, const char * name )
: GenericSection( parent, name )
{
	setTitle( tr( "Lord Categories" ) );
	
	_num = 0;
	_name = new AskString( tr( "Name: " ), _mainWidget );
	_description = new AskString( tr( "Description: " ), _mainWidget );
	_race = new AskCombo( tr( "Race: " ), _mainWidget );
	_attack = new AskInt( tr( "Evolution of Attack skill (weight): " ), _mainWidget );
	_defense = new AskInt( tr( "Evolution of Defense skill (weight): " ), _mainWidget );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _description );
	layout->addWidget( _race );
	layout->addWidget( _attack );
	layout->addWidget( _defense );
	layout->addStretch( 1 );
	layout->activate();

	updateData();
	selectFirst();
}

void SectionLordCategory::updateData()
{
	_race->clear();
	uint nbRace = DataTheme.countRace();

	for( uint i = 0; i < nbRace; i++ ) {
		_race->insertItem( DataTheme.creatures.getRace( i )->getName() );
	}
	init();
}

void SectionLordCategory::clear()
{
	_name->setValue( QString::number( _num + 1 ) );
	_description->setValue( "" );
	_race->setCurrentItem( 0 );
	_attack->setValue( 1 );
	_defense->setValue( 1 );
}

void SectionLordCategory::init()
{
	if( DataTheme.lordCategories.count() > 0 ) {
		LordCategoryModel * category = DataTheme.lordCategories.at( _num );
		_name->setValue( category->getName() );
		_description->setValue( category->getDescription() );
		_race->setCurrentItem( category->getRace() );
		_attack->setValue( category->getEvolution( ATTACK ) );
		_defense->setValue( category->getEvolution( DEFENSE ) );
	}
}

void SectionLordCategory::save()
{
	if( DataTheme.lordCategories.count() > 0 ) {
		LordCategoryModel * category = DataTheme.lordCategories.at( _num );
		category->setName( _name->getValue() );
		category->setDescription( _description->getValue() );
		category->setRace( _race->currentItem() );
		category->setEvolution( ATTACK, _attack->getValue() );
		category->setEvolution( DEFENSE, _defense->getValue() );
		emit sig_changed();
	}
}
void SectionLordCategory::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionLordCategory::selectPrevious()
{
	save();
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionLordCategory::selectNext()
{
	save();
	_num = std::min( (int)DataTheme.lordCategories.count() - 1, _num + 1 );
	init();
}

void SectionLordCategory::selectLast()
{
	save();
	_num = DataTheme.lordCategories.count() - 1;
	init();
}

void SectionLordCategory::selectNew()
{
	save();
	LordCategoryModel * category = new LordCategoryModel();
	_num = DataTheme.lordCategories.count();
	DataTheme.lordCategories.append( category );
	clear();
}

void SectionLordCategory::selectDel()
{
	if( DataTheme.lordCategories.count() > 1 ) {
		DataTheme.lordCategories.remove( _num );
		_num = std::min( _num, (int)DataTheme.lordCategories.count() - 1 );
		init();
	}
}




