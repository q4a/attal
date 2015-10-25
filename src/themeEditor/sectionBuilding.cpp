/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBuilding.cpp
** section specifying buildings
**
** Version : $Id: sectionBuilding.cpp,v 1.3 2002/04/20 18:42:10 audoux Exp $
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

#include "sectionBuilding.h"
 
// generic include files
// include files for QT
#include <qlayout.h>
// application specific include files
#include "common/log.h"
#include "common/dataTheme.h"
#include "common/genericBuilding.h"

extern DataTheme DataTheme;
extern QString IMAGE_PATH;

SectionBuilding::SectionBuilding( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	
	setTitle( "Building" );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( "Name: ", _mainWidget );
	layout->addWidget( _name );

	_description = new AskString( "Description: ", _mainWidget );
	layout->addWidget( _description );

	_type = new AskInt( "Type: ", _mainWidget );
	layout->addWidget( _type );

	_item = new AskPixmap( true, "", "Picture: ", _mainWidget );
	layout->addWidget( _item );

	layout->addStretch( 1 );
	layout->activate();
	init();
}

void SectionBuilding::init()
{
	if( (int)DataTheme.buildings.count() > _num ) {
		GenericBuildingModel * building = DataTheme.buildings.at( _num );
		_name->setValue( building->getName() );
		_description->setValue( building->getDescription() );
		_type->setValue( building->getType() );
		QString dest;
		dest = IMAGE_PATH + QString( "building/building_" ) + QString::number( _num ) + QString( ".png" );
		_item->setDestination( dest );
	}
}

void SectionBuilding::clear()
{
	_name->setValue( "" );
	_description->setValue( "" );
	_type->setValue( 0 );
	_item->setValue( "" );
}

void SectionBuilding::save()
{
	if( (int)DataTheme.buildings.count() > _num ) {
		GenericBuildingModel * building = DataTheme.buildings.at( _num );
		building->setName( _name->getValue() );
		building->setDescription( _description->getValue() );
		building->setType( _type->getValue() );
		_item->save();
	}
}

void SectionBuilding::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionBuilding::selectPrevious()
{
	save();
	_num = QMAX( 0, _num - 1 );
	init();
}

void SectionBuilding::selectNext()
{
	save();
	_num = QMIN( int(DataTheme.buildings.count() - 1), _num + 1 );
	init();
}

void SectionBuilding::selectLast()
{
	save();
	_num = DataTheme.buildings.count() - 1;
	init();
}

void SectionBuilding::selectNew()
{
	save();
	GenericBuildingModel * building = new GenericBuildingModel();
	_num = DataTheme.buildings.count();
	DataTheme.buildings.append( building );
	clear();
}

void SectionBuilding::selectDel()
{
	if( DataTheme.buildings.count() > 0 ) {
		DataTheme.buildings.remove( _num );
		_num = QMIN( _num, int(DataTheme.buildings.count() - 1) );
		init();
	}
}
