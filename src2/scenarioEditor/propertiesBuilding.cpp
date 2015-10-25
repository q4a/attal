/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesBuilding.cpp
** Defines properties box for buildings on map
**
** Version : $Id: propertiesBuilding.cpp,v 1.20 2009/11/11 00:22:58 lusum Exp $
**
** Author(s) : Sardi Carlo - Pascal Audoux
**
** Date : 22/04/2004
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

#include "propertiesBuilding.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>

// application specific include files
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/dataTheme.h"
#include "libCommon/action.h"

#include "libClient/gui.h"

//
// ----- propertiesBuilding -----
//

PropertiesBuilding::PropertiesBuilding( QList<GenericPlayer *> * players, QWidget * parent, const char * /* name */ )
	: QDialog( parent, Qt::Dialog )
{
        setWindowTitle( tr( "building properties" ) );
	_building = 0;

	QLabel * ownerLabel = new QLabel( this );
	ownerLabel->setText( tr( "Owner:" ) );

	_name = new QLabel( this );
	
	_resBuil = new AskCost( tr( "Res Buil: " ), this );
	
	_ownerList = new QComboBox( this );
	refresh( players );

	_listBuilding = new QListWidget( this );

	QPushButton * pb = new QPushButton( this );
	pb->setText( tr( "Ok" ) );
	FIXEDSIZE( pb );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setSpacing( 5 );
	layH1->addWidget( ownerLabel );
	layH1->addWidget( _ownerList, 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setSpacing( 5 );
	layH2->addWidget( _listBuilding, 1 );

	QHBoxLayout * layH = new QHBoxLayout();
	layH->setMargin( 5 );
	layH->addStretch( 1 );
	layH->addWidget( pb );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addStretch( 1 );
	layout->addWidget( _name );
	layout->addWidget( _resBuil );
	layout->addWidget( _listBuilding );
	layout->addLayout( layH );
	layout->activate();

	connect( pb, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( _ownerList, SIGNAL( activated( int ) ), SLOT( slot_owner( int ) ) );
}

void PropertiesBuilding::refresh( QList<GenericPlayer *> * players )
{
	_players = players;
	_ownerList->clear();
	_ownerList->addItem( tr( "None" ) );
	
	for( int i = 0; i < players->count(); i++ ) {
		GenericPlayer * player = players->at( i );
		if( player ) {
			_ownerList->addItem( player->getName() );
		}
	}
}


void PropertiesBuilding::slot_accept()
{
	int nbRes = DataTheme.resources.count();

	for( int i = 0; i < nbRes; i++ ) {
			_building->getResourceList()->setValue( i, _resBuil->getValue( i ) );
	}

	accept();
}

void PropertiesBuilding::slot_owner( int num )
{
	if( _building->getOwner() ) {
		_building->getOwner()->removeBuilding( _building );
	}
	if( num == 0 ) {
		_building->setOwner( 0 );
	} else {
		_building->setOwner( _players->at( num-1 ) );
		_players->at( num-1 )->addBuilding( _building );
	}
}

void PropertiesBuilding::init( GenericBuilding * building )
{
	_building = building;
	_listBuilding->clear();
	_name->setText( _building->getName() );
	
	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
		_resBuil->setValue( i , _building->getResourceList()->getValue( i ) );
	}

	for( int type = Action::DATE; type< Action::LAST; type++ ){
		QList<Action *>  action = _building->getActionList((Action::ActionType) type);
		for( int i = 0; i < action.count(); i++ ) {
			_listBuilding->addItem( action.at(i)->getDescription( true ));
		}
	}

	if( _building->getOwner() ) {
		_ownerList->setCurrentIndex( _building->getOwner()->getNum()+1 );
	} else {
		_ownerList->setCurrentIndex( 0 );
	}
}




