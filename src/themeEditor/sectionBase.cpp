/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBase.cpp
** section specifying bases
**
** Version : $Id: sectionBase.cpp,v 1.4 2003/04/11 10:11:44 audoux Exp $
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

#include "sectionBase.h"
 
// generic include files
// include files for QT
#include <qlayout.h>
// application specific include files
#include "common/dataTheme.h"
#include "common/log.h"

extern DataTheme DataTheme;
extern QString IMAGE_PATH;

SectionBase::SectionBase( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	setTitle( " Base " );
	_idBase = 0;

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_name = new AskString( "Name : ", _mainWidget );
	layout->addWidget( _name );

	_population = new AskInt( "Population : ", _mainWidget );
	layout->addWidget( _population );

	_inside = new SectionInsideBuilding( _mainWidget );
	layout->addWidget( _inside, 1 );

	layout->activate();

	updateData();
	init();
}

void SectionBase::updateData()
{
	_population->setEnabled( DataTheme.isPopulation() );
}

void SectionBase::init()
{
	if( _idBase < DataTheme.bases.count() ) {
		QString text;
		text.sprintf( " Base : %d/%d",  ( _idBase + 1 ), DataTheme.bases.count() );
		setTitle( text );

		GenericBaseModel * base = DataTheme.bases.at( _idBase );
		_name->setValue( base->getModelName() );
		_population->setValue( base->getPopulation() );
		_inside->setBase( _idBase );
		_inside->init();
	} else {
		clear();
	}
}

void SectionBase::clear()
{
	_idBase = 0;
	_name->setValue( "" );
	_population->setValue( 0 );
	_inside->setBase( 0 );
	_inside->clear();
}

void SectionBase::selectFirst()
{
	save();
	if( _idBase > 0 ) {
		_idBase = 0;
		init();
	}
}

void SectionBase::selectPrevious()
{
	save();
	if( _idBase > 0 ) {
		_idBase--;
	}
	init();
}

void SectionBase::selectNext()
{
	save();
	if( _idBase < DataTheme.bases.count() - 1 ) {
		_idBase++;
	}
	init();
}

void SectionBase::selectLast()
{
	save();
	_idBase = DataTheme.bases.count() - 1;
	init();
}

void SectionBase::selectNew()
{
	save();
	_idBase = DataTheme.bases.count();
	GenericBaseModel * base = new GenericBaseModel();
	DataTheme.bases.append( base );
	init();
}

void SectionBase::selectDel()
{
	clear();
	DataTheme.bases.remove( _idBase );
	_idBase = QMIN( _idBase, DataTheme.bases.count() - 1 );
	init();
}

void SectionBase::save()
{
	if( _idBase < DataTheme.bases.count() ) {
		GenericBaseModel * base = DataTheme.bases.at( _idBase );
		base->setModelName( _name->getValue() );
		base->setPopulation( _population->getValue() );
		_inside->save();
	}
}

//
// ----- SectionInsideBuilding -----
//

SectionInsideBuilding::SectionInsideBuilding( QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_select = new SelectionWidget( this );
	_select->setTitle( " Building " );

	_name = new AskString( "Name : ", this );
	_description = new AskString( "Description : ", this );
	_x = new AskInt( "x : ", this );
	_x->setMaxValue( 1000 );
	_y = new AskInt( "y : ", this );
	_y->setMaxValue( 1000 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _select );
	layout->addWidget( _name );
	layout->addWidget( _description );
	layout->addWidget( _x );
	layout->addWidget( _y );


	layout->addStretch( 1 );
	layout->activate();

	connect( _select, SIGNAL( sig_first() ), SLOT( selectFirst() ) );
	connect( _select, SIGNAL( sig_previous() ), SLOT( selectPrevious() ) );
	connect( _select, SIGNAL( sig_next() ), SLOT( selectNext() ) );
	connect( _select, SIGNAL( sig_last() ), SLOT( selectLast() ) );
	connect( _select, SIGNAL( sig_new() ), SLOT( selectNew() ) );
	connect( _select, SIGNAL( sig_del() ), SLOT( selectDel() ) );

	init();
}

void SectionInsideBuilding::setBase( int base )
{
	if( base != _idBase ) {
		save();
		clear();
		_idBase = base;
		_idBuild = 0;
		init();

	}
}
void SectionInsideBuilding::selectFirst()
{
	save();
	if( _idBuild > 0 ) {
		_idBuild = 0;
		init();
	}
}

void SectionInsideBuilding::selectPrevious()
{
	save();
	if( _idBuild > 0 ) {
		_idBuild--;
	}
	init();
}

void SectionInsideBuilding::selectNext()
{
	save();
	if( ( _idBase < DataTheme.bases.count() )
		&& ( _idBuild < DataTheme.bases.at( _idBase )->getBuildingCount() - 1 ) ) {
		_idBuild++;
	}
	init();
}

void SectionInsideBuilding::selectLast()
{
	save();
	if( _idBase < DataTheme.bases.count() ) {
		_idBuild = DataTheme.bases.at( _idBase )->getBuildingCount() - 1;
	}
	init();
}

void SectionInsideBuilding::selectNew()
{
	save();
	if( _idBase < DataTheme.bases.count() ) {
		_idBuild = DataTheme.bases.at( _idBase )->getBuildingCount();
		InsideBuildingModel * build = new InsideBuildingModel();
		DataTheme.bases.at( _idBase )->addBuilding( build );
		/// XXX: setRace/setLevel
	}
	init();
}

void SectionInsideBuilding::selectDel()
{
	clear();
	if( _idBase < DataTheme.bases.count() ) {
		DataTheme.bases.at( _idBase )->removeBuilding( _idBuild );
		_idBuild = QMIN( _idBase, DataTheme.bases.at( _idBase )->getBuildingCount() - 1 );
	}
	init();
}

void SectionInsideBuilding::save()
{
	if( ( _idBase < DataTheme.bases.count() )
	  && ( _idBuild < DataTheme.bases.at( _idBase )->getBuildingCount() ) ) {
		InsideBuildingModel * build = DataTheme.bases.at( _idBase )->getBuildingModel( _idBuild );
		build->setName( _name->getValue() );
		build->setDescription( _description->getValue() );
		build->setX( _x->getValue() );
		build->setY( _y->getValue() );
	}
}

void SectionInsideBuilding::init()
{
	if( ( _idBase < DataTheme.bases.count() )
	  && ( _idBuild < DataTheme.bases.at( _idBase )->getBuildingCount() ) ) {
		QString text;
		text.sprintf( " Building : %d/%d", ( _idBuild + 1 ), DataTheme.bases.at( _idBase )->getBuildingCount() );
		_select->setTitle( text );

		InsideBuildingModel * build = DataTheme.bases.at( _idBase )->getBuildingModel( _idBuild );
		_name->setValue( build->getName() );
		_description->setValue( build->getDescription() );
		_x->setValue( build->getX() );
		_y->setValue( build->getY() );
	}
}

void SectionInsideBuilding::clear()
{
	_idBuild = 0;
	_name->setValue( "" );
	_description->setValue( "" );
	_x->setValue( 0 );
	_y->setValue( 0 );
}

