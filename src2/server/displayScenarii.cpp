/****************************************************************
**
** Attal : Lords of Doom
**
** displayScenarii.cpp
** Smart display of scenarii
**
** Version : $Id: displayScenarii.cpp,v 1.7 2006/07/26 11:30:43 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2004
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

#include "displayScenarii.h"
 
// generic include files
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLayout>
#include <QPoint>
#include <QPushButton>
#include <QString>

// application specific include files
#include "libCommon/log.h"

#include "libServer/scenarioDescription.h"

extern QString SCENARIO_PATH;

//
// ----- DisplayScenarioDescription -----
//
DisplayScenarioDescription::DisplayScenarioDescription( QWidget * parent )
: QWidget( parent )
{
	_labName = new QLabel( this );
	_labName->setFixedSize( _labName->sizeHint() );
	
	_labDescription = new QLabel( this );
	_labDescription->setFixedSize( _labDescription->sizeHint() );
	
	_labPlayers = new QLabel( this );
	_labPlayers->setFixedSize( _labPlayers->sizeHint() );
	
	_labSize = new QLabel( this );
	_labSize->setFixedSize( _labSize->sizeHint() );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _labName );
	layout->addWidget( _labSize );
	layout->addWidget( _labPlayers );
	layout->addWidget( _labDescription );
	layout->addStretch( 1 );
	layout->activate();
	
	clear();
	
	setFixedWidth( 300 );
}
	
void DisplayScenarioDescription::init( ScenarioDescription * desc )
{
	if( desc ) {
		_labName->setText( tr( "Name: %1" ).arg( desc->getName() ) );
		_labName->setFixedSize( _labName->sizeHint() );
				
		_labPlayers->setText( tr( "Player(s): %1" ).arg( desc->getNbPlayers() ) );
		_labPlayers->setFixedSize( _labPlayers->sizeHint() );
		
		_labSize->setText( tr( "Size: %1x%2" ).arg( desc->getWidth() ).arg( desc->getHeight() ) );
		_labSize->setFixedSize( _labSize->sizeHint() );
		
		_labDescription->setText( tr( "Description: %1" ).arg( desc->getDescription() ) );
		_labDescription->setFixedSize( _labDescription->sizeHint() );
	}
}
	
void DisplayScenarioDescription::clear()
{
	_labName->setText( tr( "Name: " ) );
	_labName->setFixedSize( _labName->sizeHint() );
				
	_labPlayers->setText( tr( "Player(s): " ) );
	_labPlayers->setFixedSize( _labPlayers->sizeHint() );
		
	_labSize->setText( tr( "Size: " ) );
	_labSize->setFixedSize( _labSize->sizeHint() );
		
	_labDescription->setText( tr( "Description: " ) );
	_labDescription->setFixedSize( _labDescription->sizeHint() );
}

//
// ----- ScenarioListViewItem -----
//

ScenarioListViewItem::ScenarioListViewItem( QTreeWidget * list, ScenarioDescription * desc )
: QTreeWidgetItem( list )
{
	_desc = desc;
	QString temp;
	
	temp.sprintf( "%dx%d", _desc->getWidth(), _desc->getHeight() );
	setText( 0, temp );
	
	setText( 1, QString::number( _desc->getNbPlayers() ) );
	
	setText( 2, _desc->getName() );
}

ScenarioListViewItem::~ScenarioListViewItem()
{
	if( _desc ) {
		delete _desc;
	}
}

ScenarioDescription * ScenarioListViewItem::getScenario()
{
	return _desc;
}

//
// ----- DisplayScenarii -----
//

DisplayScenarii::DisplayScenarii( QWidget * parent )
: QWidget( parent )
{
	_currentScen = 0;
	
	_list = new QTreeWidget( this );

	_list->setSelectionBehavior( QAbstractItemView::SelectRows );
	_list->setColumnCount( 3 );
	QStringList labels;
	labels << tr( "Size" );
	labels << tr( "Players" ); 
	labels << tr( "Name" );
	_list->setHeaderLabels( labels );
		
	_description = new DisplayScenarioDescription( this );
	_description->setMinimumWidth( 250 );
	
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( _list, 1 );
	layout->addWidget( _description );
	layout->activate();
	
	init();
	
	connect( _list, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), SLOT( slot_clicked( QTreeWidgetItem * ) ) );
	connect( _list, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), SIGNAL( sig_dbleClicked() ) );
	
	setMinimumWidth( 600 );
}

void DisplayScenarii::slot_clicked( QTreeWidgetItem * item )
{
	ScenarioListViewItem * scenarioItem = ( ScenarioListViewItem * ) item;
	_currentScen = scenarioItem->getScenario();
	_description->init( scenarioItem->getScenario() );
}


void DisplayScenarii::init()
{
	QDir scenarioDir( SCENARIO_PATH );
        scenarioDir.setFilter( QDir::Files | QDir::Hidden );
        scenarioDir.setSorting( QDir::Size | QDir::Reversed );
	scenarioDir.setNameFilters( QStringList( "*.scn" ) );

        const QList<QFileInfo> fileList = scenarioDir.entryInfoList();

	ScenarioDescription * desc;
	
	uint nbItems = (uint)fileList.count();
	for( uint i = 0; i < nbItems; ++i ) {
		desc = new ScenarioDescription();
		desc->load( fileList[ i ].absoluteFilePath() );
		new ScenarioListViewItem( _list, desc );
		
	}
	/* was
        while ( (fileInfo = it.value() ) != 0 ) {
		desc = new ScenarioDescription();
		desc->load( fileInfo->absFilePath() );
		new ScenarioListViewItem( _list, desc );
		++it;
        }*/
}
	
void DisplayScenarii::clear()
{
	_list->clear();
}


//
// ----- DisplayScenariiDialog -----
//

DisplayScenariiDialog::DisplayScenariiDialog( QWidget * parent )
: QDialog( parent )
{
	_display = new DisplayScenarii( this );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addWidget( _display, 1 );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	pbOk->setFixedSize( pbOk->sizeHint() );	

	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setText( tr( "Cancel" ) );
	pbCancel->setFixedSize( pbCancel->sizeHint() );
		
	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addStretch( 1 );
	layH2->addWidget( pbOk );
	layH2->addStretch( 1 );
	layH2->addWidget( pbCancel );
	layH2->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1, 1 );
	layout->addLayout( layH2 );
	layout->activate();
		
	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCancel, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( _display, SIGNAL( sig_dbleClicked() ), SLOT( accept() ) );
}

QString DisplayScenariiDialog::getFileName()
{
	QString ret = "";
	
	ret = _display->getCurrentScenario()->getFileName();
	
	return ret;
}



