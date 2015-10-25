/****************************************************************
**
** Attal : Lords of Doom
**
** clientWidgets.cpp
** Various dialog for client
**
** Version : $Id: clientWidgets.cpp,v 1.8 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 26/07/2006
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

#include "clientWidgets.h"
 
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
#include "libCommon/campaign.h"

#include "libClient/gui.h"

#include "libServer/scenarioDescription.h"

extern QString SCENARIO_PATH;
extern QString CAMPAIGN_PATH;

//
// ----- ConfigConnection -----
//

ConfigConnection::ConfigConnection( QWidget * parent )
	: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Connection to server" ) );
	QVBoxLayout * layout = new QVBoxLayout( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	QLabel * labHost = new QLabel( tr( "Host : " ), this );
	FIXEDSIZE( labHost );
	layH1->addWidget( labHost );
	layH1->addSpacing( 5 );
	_host = new QLineEdit( this );
	_host->setFixedSize( 160, 20 );
	layH1->addWidget( _host );
	layH1->addStretch( 1 );
	layout->addLayout( layH1, 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	QLabel * labPort = new QLabel( tr( "Port : " ), this );
	FIXEDSIZE( labPort );
	layH2->addWidget( labPort );
	layH2->addSpacing( 5 );
	_port = new QLineEdit( this );
	_port->setFixedSize( 80, 20 );
	layH2->addWidget( _port );
	layH2->addStretch( 1 );
	layout->addLayout( layH2, 1 );

	QHBoxLayout * layH4 = new QHBoxLayout();
	layH4->addSpacing( 5 );
	QLabel * labName = new QLabel( tr( "Name : " ), this );
	FIXEDSIZE( labName );
	layH4->addWidget( labName );
	layH4->addSpacing( 5 );
	_name = new QLineEdit( this );
	_name->setFixedSize( 80, 20 );
	layH4->addWidget( _name );
	layH4->addStretch( 1 );
	layout->addLayout( layH4, 1 );

	QHBoxLayout * layH3 = new QHBoxLayout();
	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Connect" ) );
	FIXEDSIZE( pbOk );
	layH3->addStretch( 1 );
	layH3->addWidget( pbOk );
	layH3->addStretch( 1 );
	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );
	layH3->addWidget( pbCan );
	layH3->addStretch( 1 );
	layout->addLayout( layH3, 2 );

	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );

	setFixedSize( 250, 150 );
}

//
// ----- StartGameDialog -----
//

StartGameDialog::StartGameDialog( QWidget * parent )
	: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Start Game" ) );
	QVBoxLayout * layout = new QVBoxLayout( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	QPushButton * labScen = new QPushButton( this );
	labScen->setText( tr( "Start Scenario " ) );
	labScen->setFixedSize( 150,30);
	layH1->addWidget( labScen );
	layH1->addSpacing( 5 );
	layout->addLayout( layH1, 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	QPushButton * labCamp = new QPushButton( this );
	labCamp->setText( tr( "Start Campaign " ) );
	labCamp->setFixedSize( 150,30);
	layH2->addWidget( labCamp );
	layH2->addSpacing( 5 );
	layout->addLayout( layH2, 1 );

	QHBoxLayout * layH4 = new QHBoxLayout();
	layH4->addSpacing( 5 );
	QPushButton * labLoadScen = new QPushButton( this );
	labLoadScen->setText( tr( "Load Scenario " ) );
	labLoadScen->setFixedSize( 150,30);
	layH4->addWidget( labLoadScen );
	layH4->addSpacing( 5 );
	layout->addLayout( layH4, 1 );

	QHBoxLayout * layH5 = new QHBoxLayout();
	layH5->addSpacing( 5 );
	QPushButton * labLoadCamp = new QPushButton( this );
	labLoadCamp->setText( tr( "Load Campaign " ) );
	labLoadCamp->setFixedSize( 150,30);
	layH5->addWidget( labLoadCamp );
	layH5->addSpacing( 5 );
	layout->addLayout( layH5, 1 );


	layout->activate();

	connect( labScen, SIGNAL( clicked() ), SIGNAL( sig_newScen() ) );
	connect( labCamp, SIGNAL( clicked() ), SIGNAL( sig_newCamp() ) );
	connect( labLoadScen, SIGNAL( clicked() ), SIGNAL( sig_loadScen() ) );
	connect( labLoadCamp, SIGNAL( clicked() ), SIGNAL( sig_loadCamp() ) );

	setFixedSize( 250, 150 );
}

StartGameDialog::~StartGameDialog()
{
}

void StartGameDialog::closeEvent(QCloseEvent *event)
{
	emit sig_dialogClosed();
	event->accept();
}

//
// ----- DisplayScenarioDescription -----
//
DisplayScenarioDescription::DisplayScenarioDescription( QWidget * parent )
: QWidget( parent )
{
	_labName = new QLabel( this );
	_labName->setFixedSize( _labName->sizeHint() );
	
	_labDescription = new QLabel( this );
	_labDescription->setWordWrap( true );
	_labDescription->setFixedWidth( 250 );
	
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
				
		_labPlayers->setText( tr( "Opponent(s): %1" ).arg( desc->getNbPlayers()-1 ) );
		_labPlayers->setFixedSize( _labPlayers->sizeHint() );
		
		_labSize->setText( tr( "Size: %1x%2" ).arg( desc->getWidth() ).arg( desc->getHeight() ) );
		_labSize->setFixedSize( _labSize->sizeHint() );
		
		_labDescription->setText( tr( "Description: %1" ).arg( desc->getDescription() ) );
		//_labDescription->setFixedSize( _labDescription->sizeHint() );
	}
}
	
void DisplayScenarioDescription::clear()
{
	_labName->setText( tr( "Name: " ) );
	_labName->setFixedSize( _labName->sizeHint() );
				
	_labPlayers->setText( tr( "Opponent(s): " ) );
	_labPlayers->setFixedSize( _labPlayers->sizeHint() );
		
	_labSize->setText( tr( "Size: " ) );
	_labSize->setFixedSize( _labSize->sizeHint() );
		
	_labDescription->setText( tr( "Description: " ) );
	//_labDescription->setFixedSize( _labDescription->sizeHint() );
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
	
	//setText( 1, QString::number( _desc->getNbPlayers() ) );
	
	setText( 1, _desc->getName() );
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
	_list->setRootIsDecorated( false );
	_list->setSelectionBehavior( QAbstractItemView::SelectRows );
	_list->setColumnCount( 2 );
	QStringList labels;
	labels << tr( "Num scenarios" );
	//labels << tr( "Players" ); 
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
	emit sig_scenarioSelected();
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

DisplayScenariiDialog::DisplayScenariiDialog( const QString & title, QWidget * parent )
: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( title );
	_display = new DisplayScenarii( this );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addWidget( _display, 1 );

	QPushButton * pbOk = _pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	pbOk->setFixedSize( pbOk->sizeHint() );
	pbOk->setEnabled(false);

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
	connect( _display, SIGNAL( sig_scenarioSelected() ), SLOT ( slot_scenarioSelected() ) );
	connect( _display, SIGNAL( sig_dbleClicked() ), SLOT( accept() ) );
}

void DisplayScenariiDialog::slot_scenarioSelected()
{
	_pbOk->setEnabled(true);
}

QString DisplayScenariiDialog::getFileName()
{
	QString ret = "";
	
	ret = _display->getCurrentScenario()->getFileName();
	
	return ret;
}

//
// ----- CampaignListViewItem -----
//

CampaignListViewItem::CampaignListViewItem( QTreeWidget * list, Campaign * desc )
: QTreeWidgetItem( list )
{
	_desc = desc;
	QString temp;
	
	temp.sprintf( "%d", _desc->getScenarioNumber() );
	setText( 0, temp );
	
	setText( 1, _desc->getName() );
}

CampaignListViewItem::~CampaignListViewItem()
{
	if( _desc ) {
		delete _desc;
	}
}

Campaign * CampaignListViewItem::getCampaign()
{
	return _desc;
}

//
// ----- DisplayCampaign -----
//

DisplayCampaign::DisplayCampaign( QWidget * parent )
: QWidget( parent )
{
	_currentCamp = NULL;
	
	_list = new QTreeWidget( this );
	_list->setRootIsDecorated( false );
	_list->setSelectionBehavior( QAbstractItemView::SelectRows );
	_list->setColumnCount( 2 );
	QStringList labels;
	labels << tr( "Num scenarios" );
	//labels << tr( "Players" ); 
	labels << tr( "Name" );
	_list->setHeaderLabels( labels );
		
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( _list, 1 );
	layout->activate();
	
	init();
	
	connect( _list, SIGNAL( itemClicked( QTreeWidgetItem *, int ) ), SLOT( slot_clicked( QTreeWidgetItem * ) ) );
	connect( _list, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), SIGNAL( sig_dbleClicked() ) );
	
	setMinimumWidth( 600 );
}

void DisplayCampaign::slot_clicked( QTreeWidgetItem * item )
{
	CampaignListViewItem * scenarioItem = ( CampaignListViewItem * ) item;
	_currentCamp = scenarioItem->getCampaign();
	emit sig_campaignSelected();
}


void DisplayCampaign::init()
{
	QDir scenarioDir( CAMPAIGN_PATH );
        scenarioDir.setFilter( QDir::Files | QDir::Hidden );
        scenarioDir.setSorting( QDir::Size | QDir::Reversed );
	scenarioDir.setNameFilters( QStringList( "*.cmp" ) );

        const QList<QFileInfo> fileList = scenarioDir.entryInfoList();

	Campaign * desc;
	
	uint nbItems = (uint)fileList.count();
	for( uint i = 0; i < nbItems; ++i ) {
		desc = new Campaign();
		desc->load( fileList[ i ].absoluteFilePath() );
		new CampaignListViewItem( _list, desc );
	}
}
	
void DisplayCampaign::clear()
{
	_list->clear();
}


//
// ----- DisplayCampaignDialog -----
//

DisplayCampaignDialog::DisplayCampaignDialog( const QString & title, QWidget * parent )
: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( title );
	_displayC = new DisplayCampaign( this );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addWidget( _displayC, 1 );

	QPushButton * pbOk = _pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	pbOk->setFixedSize( pbOk->sizeHint() );
	pbOk->setEnabled(false);

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
	connect( _displayC, SIGNAL( sig_campaignSelected() ), SLOT ( slot_campaignSelected() ) );
	connect( _displayC, SIGNAL( sig_dbleClicked() ), SLOT( accept() ) );
}

void DisplayCampaignDialog::slot_campaignSelected()
{
	_pbOk->setEnabled(true);
}

QString DisplayCampaignDialog::getFileName()
{
	QString ret = "";
	
	ret = _displayC->getCurrentCampaign()->getFileName();
	
	return ret;
}


