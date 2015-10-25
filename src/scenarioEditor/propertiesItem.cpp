/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesItem.cpp
** Defines properties box for items on map
**
** Version : $Id: propertiesItem.cpp,v 1.7 2003/05/09 08:04:58 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/12/2001
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

#include "propertiesItem.h"

// generic include files
// include files for QT
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlistview.h>
// application specific include files
#include "common/genericInsideBuilding.h"
#include "common/genericLord.h"
#include "common/dataTheme.h"
#include "client/gui.h"

extern DataTheme DataTheme;


//
// ----- PropertiesBase -----
//

PropertiesBase::PropertiesBase( QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
        setCaption( "Base properties" );
	_base = 0;

	_listBuilding = new QListView( this );
	_listBuilding->addColumn( "Buildings" );
	_listBuilding->setSorting( -1 );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	layH1->addWidget( _listBuilding, 1 );
	layH1->addSpacing( 5 );

	QPushButton * pb = new QPushButton( this );
	pb->setText( "Ok" );
	FIXEDSIZE( pb );

	QHBoxLayout * layH = new QHBoxLayout();
	layH->setMargin( 5 );
	layH->addStretch( 1 );
	layH->addWidget( pb );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->addLayout( layH1, 1 );
	layout->addSpacing( 5 );
	layout->addLayout( layH );
	layout->activate();

	connect( pb, SIGNAL( clicked() ), SLOT( slot_accept() ) );
}

void PropertiesBase::slot_accept()
{
	QListViewItemIterator ite( _listBuilding );
	int i =0;
	for( ite.current(); ite.current(); ite++ ) {
		if( ( (QCheckListItem*) ite.current() )->isOn() ) {
			if( ! _base->hasBuildingType( i ) ) {
				GenericInsideBuilding * building = new GenericInsideBuilding();
				building->setRace( _base->getRace() );
				building->setLevel( i );
				_base->addBuilding( building );
			}
		} else {
			if( _base->hasBuildingType( i ) ) {
				_base->removeBuildingType( i );
			}
		}
		i++;
	}
	accept();
}

void PropertiesBase::init( GenericBase * base )
{
	_base = base;
	_listBuilding->clear();

	for( int i = DataTheme.bases.at( _base->getRace() )->getBuildingCount() - 1; i >= 0; i-- ) {
		InsideBuildingModel * building = DataTheme.bases.at( _base->getRace() )->getBuildingModel( i );
		new QCheckListItem( _listBuilding, building->getName(), QCheckListItem::CheckBox );
	}

 	for( int j = 0; j < (int)_base->getBuildingCount(); j++ ) {
		GenericInsideBuilding * building = _base->getBuilding( j );
		QListViewItemIterator ite( _listBuilding );
		for( int k = 0; k < building->getLevel(); k++ ) {
			ite++;
		}
		( (QCheckListItem*)ite.current() )->setOn( true );
	}
}




