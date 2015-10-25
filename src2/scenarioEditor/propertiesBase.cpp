/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesItem.cpp
** Defines properties box for bases on map
**
** Version : $Id: propertiesBase.cpp,v 1.27 2009/11/11 00:22:58 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

#include "propertiesBase.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>

#include <QTreeWidget>

// application specific include files
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/dataTheme.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"

//
// ----- PropertiesBase -----
//

PropertiesBase::PropertiesBase( QList<GenericPlayer *> * players, QWidget  * parent, const char * /* name */ )
	:QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Base properties" ) );
	_players = players;
	_base = 0;

	_general = new GeneralPropertiesBase( players, 0 );
	_units = new PropertiesBaseUnits( 0 );

	_tabWidget = new QTabWidget( this );

	_tabWidget->addTab( _general, tr( "General" ) );
	_tabWidget->addTab( _units, tr( "Units" ) );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCancel );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCancel );
	layH1->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _tabWidget, 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( pbCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

void PropertiesBase::accept()
{
	_general->accept();
	_units->accept();
	QDialog::accept();
}

void PropertiesBase::init( GenericBase * base )
{
	_base = base;
	_general->init( base );
	_units->init( base );
}

//
// ----- GeneralPropertiesBase -----
//

GeneralPropertiesBase::GeneralPropertiesBase( QList<GenericPlayer *> * players, QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	_base = 0;

	QLabel * ownerLabel = new QLabel( this );
	ownerLabel->setText( tr( "Owner:" ) );

	_ownerList = new QComboBox( this );
	refresh( players );
	
	_resBase = new AskCost( tr( "Res Base: " ), this );

	_listBuilding = new QTreeWidget( this );
	_listBuilding->setColumnCount( 1 );
	QStringList labels;
	labels << tr( "Buildings" );
	labels << tr( "Description " );
	labels << tr( "Actions " );
	_listBuilding->setHeaderLabels( labels );
	_listBuilding->setMinimumHeight( 50 );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setSpacing( 5 );
	layH1->addWidget( ownerLabel );
	layH1->addWidget( _ownerList, 1 );
	
	_name = new AskString( tr( "Name: " ), this );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setSpacing( 5 );
	layH2->addWidget( _listBuilding, 1 );

	QHBoxLayout * layH = new QHBoxLayout();
	layH->setMargin( 5 );
	layH->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addWidget( _name );
	layout->addWidget( _resBase );
	layout->addLayout( layH2, 1 );
	layout->addLayout( layH );
	layout->activate();
}

void GeneralPropertiesBase::refresh( QList<GenericPlayer *> * players )
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


void GeneralPropertiesBase::accept()
{
	setOwner();
	QTreeWidgetItem * item;
	int i = 0;
		
	int nbRes = DataTheme.resources.count();

	for( i = 0; i < nbRes; i++ ) {
			_base->getResourceList()->setValue( i, _resBase->getValue( i ) );
	}

	while( i < _listBuilding->topLevelItemCount() ) {
		item = _listBuilding->topLevelItem( i );

		if( item ) {
			if( item->checkState(0) == Qt::Checked ) {
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
	}

	_base->setName( _name->getValue() );
}

void GeneralPropertiesBase::setOwner()
{
	if( _base->getOwner() ) {
		_base->getOwner()->removeBase( _base );
	}

	int num = _ownerList->currentIndex() -1 ;

	if( num < 0 ) {
		_base->setOwner( 0 );
	} else {
		_base->setOwner( _players->at( num ) );
		_players->at( num )->addBase( _base );
	}

}

void GeneralPropertiesBase::init( GenericBase * base )
{
	_base = base;
	_listBuilding->clear();
	int m =0;

	QTreeWidgetItem * item;

	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
		_resBase->setValue( i , _base->getResourceList()->getValue( i ) );
	}

	for( uint i = 0; i <= DataTheme.bases.at( _base->getRace() )->getBuildingCount() - 1; i++ ) {
		InsideBuildingModel * building = DataTheme.bases.at( _base->getRace() )->getBuildingModel( i );
		QStringList strlist;
		strlist << building->getName();
		strlist << building->getDescription();
		strlist << building->getBuildDescActions();
		item = new QTreeWidgetItem( _listBuilding, strlist  );
		item->setCheckState(0, Qt::Unchecked );
	}

	for( int j = 0; j < (int)_base->getBuildingCount(); j++ ) {
		GenericInsideBuilding * building = _base->getBuilding( j );
		m = 0;
		for( uint k = 0; k < building->getLevel(); k++ ) {
			m++;
		}

		item = _listBuilding->topLevelItem( m );

		if( item ) {
			item->setCheckState(0, Qt::Checked );
		}

	}
	if( _base->getOwner() ) {
		_ownerList->setCurrentIndex( _base->getOwner()->getNum()+1 );
	} else {
		_ownerList->setCurrentIndex( 0 );
	}


	_name->setValue( _base->getName() );
	
}

//
// ----- PropertiesBaseUnits -----
//

PropertiesBaseUnits::PropertiesBaseUnits( QWidget * parent, const char * /* name */)
: QWidget( parent )
{
	_base = 0;

	QHBoxLayout * layH1 = new QHBoxLayout();
	QHBoxLayout * layH2 = new QHBoxLayout();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i] = new EditTroop( i, this );
		if( i <= (MAX_UNIT / 2 ) ) {
			layH1->addWidget( _troop[i] );
		} else {
			layH2->addWidget( _troop[i] );
		}
	}
	layH1->addStretch( 1 );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addLayout( layH2 );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesBaseUnits::init( GenericBase * base )
{
	_base = base;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->init (_base->getUnit (i));
	}
}

void PropertiesBaseUnits::accept()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->save( i, _base );
	}
}

bool PropertiesBaseUnits::hasNoneUnits ()
{
	bool hasUnits = false;

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if (_troop[i]->hasUnits ())
		{
			hasUnits = true;
		}
	}

	return (!hasUnits);
}



