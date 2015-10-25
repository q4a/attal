/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBuilding.cpp
** section specifying buildings
**
** Version : $Id: sectionBuilding.cpp,v 1.14 2009/11/19 22:14:10 lusum Exp $
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
#include <algorithm>

// include files for QT
#include <QLayout>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBuilding.h"

#include "libClient/gui.h"

#include "themeEditor/mapDispositionEditor.h"

extern QString IMAGE_PATH;

SectionBuilding::SectionBuilding( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	_num = 0;

	setTitle( tr( "Building" ) );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	layout->addWidget( _name );
	
	_vision = new AskInt( tr( "Vision: " ), _mainWidget );
	_vision->setMaxValue(1000);
	layout->addWidget( _vision );

	_description = new AskString( tr( "Description: " ), _mainWidget );
	layout->addWidget( _description );

	_frame = new AskInt( tr( "Number Frames: " ), _mainWidget );
	layout->addWidget( _frame );

	_animFreq = new AskInt( tr( "Animation Freq.: " ), _mainWidget );
	layout->addWidget( _animFreq );

	_resBuild = new AskCost( tr( "Res Build: " ), _mainWidget );
	layout->addWidget( _resBuild );

	_item = new AskPixmap( true, "", tr( "Picture: " ), _mainWidget );
	layout->addWidget( _item );

	_buildingAction = new BuildingAction( _mainWidget );
	layout->addWidget( _buildingAction );

	_dispo = new MapDispositionEditor( _mainWidget );
	layout->addWidget( _dispo );

	layout->addStretch( 1 );
	layout->activate();
	init();
}

void SectionBuilding::init()
{
	if( (int)DataTheme.buildings.count() > _num ) {
		GenericBuildingModel * building = DataTheme.buildings.at( _num );
		_name->setValue( building->getName() );
		_vision->setValue( building->getVision() );
		_description->setValue( building->getDescription() );
	 	_frame->setValue(building->getNbFrame());
	 	_animFreq->setValue(building->getAnimFreq());
		QString dest;
		dest = IMAGE_PATH + QString( "building/building_" ) + QString::number( _num ) + QString( "_0" )  + QString( ".png" );
		_item->setDestination( dest );
		_buildingAction->init( building->getActionList() );
		_dispo->init( building, dest );

		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			_resBuild->setValue( i , building->getResourceList()->getValue( i ) );
		}


	}
}

void SectionBuilding::clear()
{
	_name->setValue( "" );
	_vision->setValue( 3 );
	_description->setValue( "" );
	_item->setValue( "" );
	_frame->setValue( 1 );
	_animFreq->setValue( 1 );
}

void SectionBuilding::save()
{
	if( (int)DataTheme.buildings.count() > _num ) {
		GenericBuildingModel * building = DataTheme.buildings.at( _num );
		building->setName( _name->getValue() );
		building->setVision( _vision->getValue() );
		building->setDescription( _description->getValue() );
		building->setNbFrame( _frame->getValue() );
		building->setAnimFreq( _animFreq->getValue() );
		_item->save();
		
		building->clearActions();
		QList<Action *>   alist;
		alist = _buildingAction->save();
		Action * action;
		if( alist.count() ) {
			for( int i = 0; i < alist.count(); i++ ) {
				action = alist.at( i );
				if( action ) {
					building->addAction( action );
				}
			}
		}

		uint nbRes = DataTheme.resources.count();
		for( uint i = 0; i < nbRes; i++ ) {
			building->getResourceList()->setValue( i, _resBuild->getValue( i ) );
		}
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
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionBuilding::selectNext()
{
	save();
	_num = std::min( int(DataTheme.buildings.count() - 1), _num + 1 );
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
		delete DataTheme.buildings.takeAt( _num );
		_num = std::min( _num, int(DataTheme.buildings.count() - 1) );
		init();
	}
}
