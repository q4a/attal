/****************************************************************
**
** Attal : Lords of Doom
**
** sectionMachine.cpp
** section for specifying war machines
**
** Version : $Id: sectionMachine.cpp,v 1.8 2006/11/03 18:28:59 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/11/2002
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

#include "sectionMachine.h"

// generic include files
#include <algorithm>

// include files for QT
#include <QLayout>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/warMachine.h"

#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;

SectionMachine::SectionMachine( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	_num = 0;

	setTitle( tr( "War Machine" ) );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	layout->addWidget( _name );

	_type = new AskInt( tr( "Type: " ), _mainWidget );
	_type->setMinValue( 0 );
	_type->setMaxValue( 3 );
	layout->addWidget( _type );

	_param1 = new AskInt( tr( "Param (1): " ), _mainWidget );
	_param1->setMinValue( 0 );
	_param1->setMaxValue( 3 );
	layout->addWidget( _param1 );

	_params = new AskIntList( _mainWidget );
	_params->setLabel( tr( "Params: " ) );
	//_params->setMinValue( 0 );
	//_param1->setMaxValue( 3 );
	layout->addWidget( _params );

	_icon = new AskPixmap( true, "", tr( "Icon: " ), _mainWidget );
	layout->addWidget( _icon );

	_item = new AskPixmap( true, "", tr( "Item: " ), _mainWidget );
	layout->addWidget( _item );

	layout->addStretch( 1 );
	layout->activate();

	init();
}

void SectionMachine::clear()
{
	_name->setValue( "" );
	_type->setValue( 0 );
	_param1->setValue( 0 );
}

void SectionMachine::init()
{
	if( (int)DataTheme.machines.count() > _num ) {
		WarMachine * machine = DataTheme.machines.at( _num );
		_name->setValue( machine->getName() );
		_type->setValue( machine->getType() );
		_param1->setValue( machine->getParam( 0 ) );
		QString dest;
		dest = IMAGE_PATH + QString( "machines/machineIcon_" ) + QString::number( _num ) + QString( ".png" );
		_icon->setDestination( dest );
		dest = IMAGE_PATH + QString( "machines/machineItem_" ) + QString::number( _num ) + QString( ".png" );
		_item->setDestination( dest );
	}
}

void SectionMachine::save()
{
	if( (int)DataTheme.machines.count() > _num ) {
		WarMachine * machine = DataTheme.machines.at( _num );
		machine->setName( _name->getValue() );
		machine->setType( (WarMachine::MachineType)_type->getValue() );
		machine->addParam0( _param1->getValue() );
		_icon->save();
		_item->save();
	}
}

void SectionMachine::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionMachine::selectPrevious()
{
	save();
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionMachine::selectNext()
{
	save();
	_num = std::min( int(DataTheme.artefacts.count() - 1), _num + 1 );
	init();
}

void SectionMachine::selectLast()
{
	save();
	_num = DataTheme.artefacts.count() - 1;
	init();
}

void SectionMachine::selectNew()
{
	save();
	WarMachine * machine = new WarMachine();
	_num = DataTheme.machines.count();
	DataTheme.machines.append( machine );
	clear();
}

void SectionMachine::selectDel()
{
	if( DataTheme.machines.count() > 0 ) {
		delete DataTheme.machines.takeAt( _num );
		_num = std::min( _num, DataTheme.machines.count() - 1 );
		init();
	}
}


