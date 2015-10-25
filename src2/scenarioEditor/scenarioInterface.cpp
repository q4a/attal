/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInterface.cpp
** Manages the scenario editor
**
** Version : $Id: scenarioInterface.cpp,v 1.35 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 27/08/2000
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


#include "scenarioInterface.h"
 
// include files for QT
#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QMenu>
#include <QPushButton>
#include <QSignalMapper>
#include <QStatusBar>

// application specific include files
#include "conf.h"

#include "libCommon/dataTheme.h"

#include "libClient/aboutDialog.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

#include "scenarioEditor/screen.h"

extern QString IMAGE_PATH;
extern QString DATA_PATH;
extern QString THEME;

//
// ----- ScenarioInterface -----
//

ScenarioInterface::ScenarioInterface()
{
	_filename = "";
	_actions.resize( NB_ACTIONS );
	_help = NULL;

	initActions();
	initMenuBar();
	initStatusBar();
	activateMenuForScenario( false );
	if( DataTheme.init() && ImageTheme.init() ) {
		_screen = new Screen( this );
		setCentralWidget( _screen );
		resize( 600, 600 );
		connect( _screen, SIGNAL( sig_status( const QString & ) ), this, SLOT( slot_status( const QString & ) ) );
	} else {
		/// XXX: we could manage this better (later :) )
		QMessageBox::critical( this, tr( "Can't load theme" ), tr( "Theme " ) + THEME + tr( " has not been loaded successfully" ) );
	}
}

ScenarioInterface::~ScenarioInterface()
{
	delete _screen;
	//free before ImageTheme, after DataTheme (clear of ImageTheme depend on DataTheme data)
	ImageTheme.clear();
	DataTheme.clear();
}

void ScenarioInterface::changeEvent ( QEvent * e )
{
	//used to replace setCaption
	switch (e->type()){
	case QEvent::WindowTitleChange:
	{
		setWindowTitle( "Scenario editor for 'Attal - Lords of Doom'" );
	}
	break;
	default:
	QWidget::changeEvent ( e );
	break;
	}
}

void ScenarioInterface::closeEvent( QCloseEvent * /* event */ )
{
	checkChanged();
}

void ScenarioInterface::addAction( const QString & text,
					const QString & key,
					MENU_ACTION num,
					QSignalMapper * sigmap )
{
	QAction * action = new QAction( text, this );
	action->setShortcut( QKeySequence( key ) );
	_actions.insert( num, action );
	sigmap->setMapping( action, num );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );
}

void ScenarioInterface::initActions()
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "&New" ), tr( "CTRL+N" ), ACTION_NEW, sigmap );
	addAction( tr( "&Open" ), tr( "CTRL+O" ), ACTION_OPEN, sigmap );
	addAction( tr( "&Save" ), tr( "CTRL+S" ), ACTION_SAVE, sigmap );
	addAction( tr( "&Save as" ), "", ACTION_SAVEAS, sigmap );
	addAction( tr( "&Quit" ), tr( "CTRL+Q" ), ACTION_QUIT, sigmap );
	addAction( tr( "&Scenario information" ), tr( "CTRL+I" ), ACTION_INFO, sigmap );
	addAction( tr( "&Players properties" ), tr( "CTRL+P" ),	ACTION_PLAYERS, sigmap );
	addAction( tr( "Quests management" ), "", ACTION_QUESTS, sigmap );
	addAction( tr( "&Clear screen" ), tr( "CTRL+C" ), ACTION_CLEAR, sigmap );
	addAction( tr( "&Fill screen" ), tr( "CTRL+F" ), ACTION_FILL, sigmap );
	addAction( tr( "&Mini map" ), tr( "CTRL+M" ), ACTION_MINIMAP, sigmap );
	_actions[ ACTION_MINIMAP ]->setCheckable( true );
	_actions[ ACTION_MINIMAP ]->setChecked( true );

	addAction( tr( "&Grid" ), tr( "CTRL+G" ), ACTION_GRID, sigmap );
	_actions[ ACTION_GRID ]->setCheckable( true );
	_actions[ ACTION_GRID ]->setChecked( false );
	addAction( tr( "&Help" ), tr( "F1" ), ACTION_HELP, sigmap );
	addAction( tr( "&About" ), tr( "CTRL+A" ), ACTION_ABOUT, sigmap );

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_action( int ) ) );
}

void ScenarioInterface::initStatusBar()
{
	statusBar()->showMessage( "Status Bar", 0 );
}

/*!
  define menus
*/

void ScenarioInterface::initMenuBar()
{
	QMenu * menuFile = menuBar()->addMenu( tr( "&File" ) );
	QMenu * menuScen = menuBar()->addMenu( tr( "&Scenario" ) );
	QMenu * menuTools = menuBar()->addMenu( tr( "&Tools" ) );
	QMenu * menuHelp = menuBar()->addMenu( tr( "&Help" ) );

	menuFile->addAction( _actions[ ACTION_NEW ] );
	menuFile->addAction( _actions[ ACTION_OPEN ] );
	menuFile->addAction( _actions[ ACTION_SAVE ] );
	menuFile->addAction( _actions[ ACTION_SAVEAS ] );
	menuFile->addSeparator();
	menuFile->addAction( _actions[ ACTION_QUIT ] );

	menuScen->addAction( _actions[ ACTION_INFO ] );
	menuScen->addAction( _actions[ ACTION_PLAYERS ] );
	menuScen->addAction( _actions[ ACTION_QUESTS ] );

	menuTools->addAction( _actions[ ACTION_CLEAR ] );
	menuTools->addAction( _actions[ ACTION_FILL ] );
	menuTools->addAction( _actions[ ACTION_MINIMAP ] );
	menuTools->addAction( _actions[ ACTION_GRID ] );

	menuHelp->addAction( _actions[ ACTION_HELP ] );
	menuHelp->addAction( _actions[ ACTION_ABOUT ] );

}

void ScenarioInterface::slot_action( int num )
{
	switch( num ) {
	case ACTION_NEW:
		actionNew();
		break;
	case ACTION_OPEN:
		actionOpen();
		break;
	case ACTION_SAVE:
		actionSave();
		break;
	case ACTION_SAVEAS:
		actionSaveAs();
		break;
	case ACTION_QUIT:
		actionQuit();
		break;
	case ACTION_INFO:
		actionInfo();
		break;
	case ACTION_PLAYERS:
		actionPlayers();
		break;
	case ACTION_QUESTS:
		actionQuests();
		break;
	case ACTION_CLEAR:
		actionClear();
		break;
	case ACTION_FILL:
		actionFill();
		break;
	case ACTION_MINIMAP:
		actionMinimap();
		break;
	case ACTION_GRID:
		actionGrid();
		break;
	case ACTION_HELP:
		actionHelp();
		break;
	case ACTION_ABOUT:
		actionAbout();
		break;
	}
}

void ScenarioInterface::activateMenuForScenario( bool state )
{
	_actions[ ACTION_SAVE ]->setEnabled( state );
	_actions[ ACTION_SAVEAS ]->setEnabled( state );
	_actions[ ACTION_INFO ]->setEnabled( state );
	_actions[ ACTION_PLAYERS ]->setEnabled( state );
	_actions[ ACTION_QUESTS ]->setEnabled( state );
	_actions[ ACTION_CLEAR ]->setEnabled( state );
	_actions[ ACTION_FILL ]->setEnabled( state );
}

void ScenarioInterface::actionNew()
{
	checkChanged();
	if( _screen->newScenario() ) {
		_actions[ ACTION_GRID ]->setChecked( false );
		_filename = "";
		activateMenuForScenario( true );
	}
}

void ScenarioInterface::actionOpen()
{
	checkChanged();
	QString filename = QFileDialog::getOpenFileName( this, tr( "Open scenario" ), "", "*.scn *.gam" );
	if( !filename.isEmpty() ) {
		_actions[ ACTION_GRID ]->setChecked( false );
		if( _screen->loadScenario( filename ) ) {
			_filename = filename;
			activateMenuForScenario( true );
		}
	}
}

void ScenarioInterface::actionSave()
{
	if( _filename.isEmpty() ) {
		slot_action( ACTION_SAVEAS );
	} else {
		_screen->save( _filename );
	}
}

void ScenarioInterface::actionSaveAs()
{
	_filename = QFileDialog::getSaveFileName( this, tr( "Save scenario as" ), "", "*.scn" );
	if( !_filename.isEmpty() ) {
		_screen->save( _filename );
	}
}

void ScenarioInterface::actionQuit()
{
	checkChanged();
	qApp->quit();
}

void ScenarioInterface::checkChanged()
{	
	if( _screen->isChanged() ) {
		QMessageBox msb( tr( "Save?" ), tr( "Scenario changed?\nDo you want to save it?" ), QMessageBox::Warning, QMessageBox::Yes | 			QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
		if( msb.exec() == QMessageBox::Yes){
			actionSave();
		}
		_screen->setChanged( false );
	}
}

void ScenarioInterface::actionInfo()
{
	_screen->displayInfo();
}

void ScenarioInterface::actionPlayers()
{
	_screen->displayPlayersProperties();
}

void ScenarioInterface::actionQuests()
{
	_screen->manageQuests();
}

void ScenarioInterface::actionClear()
{
	_screen->clearMap();
}

void ScenarioInterface::actionFill()
{
	FillDialog dialog( this );
	if( dialog.exec() ) {
		_screen->fillMap( dialog.getTileNumber(), dialog.getTileDiversification() );
	}
}

void ScenarioInterface::actionMinimap()
{
	_screen->displayMiniMap( _actions[ ACTION_MINIMAP ]->isChecked() );
}

void ScenarioInterface::actionGrid()
{
	_screen->gridMap();
}

void ScenarioInterface::actionHelp()
{	
	if( ! _help ) {
		_help = new DisplayHelp( this );
	}
	_help->resize( 800, 600 );
	_help->show();

}

void ScenarioInterface::actionAbout()
{
	AboutDialog dialog( this );
	dialog.exec();
}

void ScenarioInterface::slot_status( const QString & text )
{
	statusBar()->showMessage( text, 0 );
}

//
// ----- FillDialog -----
//

FillDialog::FillDialog( QWidget * parent, const char * /* name */ )
	:QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Choose tile" ) );

	_comboTile = new QComboBox( this );
	for( int i = 0; i < DataTheme.tiles.count(); i++ ) {
		QString name = QString::number( i ) + " - " + DataTheme.tiles.at( i )->getName();
		_comboTile->addItem( name, i );
	}
	FIXEDSIZE( _comboTile );

	_comboDiversification = new QComboBox( this );
	slot_updateDiversification( 0 );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addWidget( _comboTile );
	layH1->addWidget( _comboDiversification );
	layH1->addStretch( 1 );

	QPushButton * pb1 = new QPushButton( this );
	pb1->setText( tr( "Ok" ) );
	FIXEDSIZE( pb1 );

	QPushButton * pb2 = new QPushButton( this );
	pb2->setText( tr( "Cancel" ) );
	FIXEDSIZE( pb2 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setMargin( 5 );
	layH2->addStretch( 1 );
	layH2->addWidget( pb1 );
	layH2->addStretch( 1 );
	layH2->addWidget( pb2 );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addStretch( 1 );
	layout->addLayout( layH2 );
	layout->activate();

	connect( pb1, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pb2, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( _comboTile, SIGNAL( activated( int ) ), SLOT( slot_updateDiversification( int ) ) );
}

void FillDialog::slot_updateDiversification( int tile )
{
	QString text;
	CellModel * cell;
	cell = DataTheme.tiles.at( tile );

	_comboDiversification->clear();
	_comboDiversification->addItem( "Random" );
	uint nb = cell->getDiversificationNumber();
	for( uint i = 0; i < nb; i++ ) {
		text =  tr( "%1 (weight=%2)" ).arg(i+1).arg( cell->getDiversification( i ));
		_comboDiversification->addItem( text );
	}
	_comboDiversification->setCurrentIndex( 0 );
	FIXEDSIZE( _comboDiversification );
}

uint FillDialog::getTileNumber()
{
	return _comboTile->currentIndex();
}

uint FillDialog::getTileDiversification()
{
	return _comboDiversification->currentIndex();
}
