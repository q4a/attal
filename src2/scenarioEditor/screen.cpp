/****************************************************************
**
** Attal : Lords of Doom
**
** screen.cpp
** main widget of the editor
**
** Version : $Id: screen.cpp,v 1.75 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo - Forest Darling
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

#include "screen.h"

 
// generic include files
// include files for QT
#include <QCursor>
#include <QFile>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QSignalMapper>
#include <QTextStream>
#include <QVBoxLayout>
#include <QRectF>

// application specific includes
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/parser.h"

#include "libClient/event.h"
#include "libClient/map.h"
#include "libClient/mapView.h"
#include "libClient/miniMap.h"
#include "libClient/cell.h"
#include "libClient/graphicalArtefact.h"

#include "scenarioEditor/displayQuests.h"
#include "scenarioEditor/itemSelector.h"
#include "scenarioEditor/propertiesBase.h"
#include "scenarioEditor/propertiesBuilding.h"
#include "scenarioEditor/propertiesCreature.h"
#include "scenarioEditor/propertiesEvent.h"
#include "scenarioEditor/propertiesLord.h"
#include "scenarioEditor/propertiesCreature.h"
#include "scenarioEditor/propertiesPlayers.h"
#include "scenarioEditor/scenarioInfo.h"

extern Map * theMap;

Screen::Screen( QWidget * parent, char * name )
	: QWidget( parent ), GraphicalGameData()
{
	setWindowTitle( QString ( name ));
	_leftPressed = false;
	_currentCell = 0;
	_changed = false;

	theMap = new Map( this );
	_map = theMap;
	_view = new MapView( theMap, this );

	_selector = new ItemSelector( this, this );

	_miniMap = new MiniMap( theMap, this );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->addWidget( _selector, 1 );
	layV1->addWidget( _miniMap );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addLayout( layV1 );
	layout->addWidget( _view, 1 );
	layout->activate();

	reinit();

	_menu = new QMenu( this );
	
	QSignalMapper * sigmap = new QSignalMapper( this );

	addAction( tr( "&Delete" ), M_DELETE, sigmap );
	addAction( tr( "&Properties" ), M_PROPERTIES, sigmap );

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_popup( int ) ) );
	
	connect( _view, SIGNAL( sig_mouseMoved  ( GenericCell *) ), SLOT( slot_mouseMoved  ( GenericCell *) ) );
	connect( _view, SIGNAL( sig_mouseLeftPressed( GenericCell *) ), SLOT( slot_mouseLeftPressed( GenericCell *) ) );
	connect( _view, SIGNAL( sig_mouseRightPressed( GenericCell *) ), SLOT( slot_mouseRightPressed( GenericCell *) ) );
	connect( _view, SIGNAL( sig_mouseReleased() ), SLOT( slot_mouseReleased() ) );
	
	connect( _view, SIGNAL( sig_viewportResized( double, double ) ), _miniMap, SLOT( slot_mapviewResized( double, double ) ) );
	connect( _view, SIGNAL( sig_viewportScrolled( double, double ) ), _miniMap, SLOT( slot_mapviewScrolled( double, double ) ) );
	connect( this, SIGNAL( sig_enableGame( bool ) ), _view, SLOT( slot_enableGame( bool ) ) );
	
	connect( _miniMap , SIGNAL( sig_mouseReleasedMinimap( double, double ) ), _view, SLOT( slot_Center( double, double ) ) );
	connect( theMap , SIGNAL( sig_cellChanged( int, int )), _miniMap, SLOT( redrawCell( int , int ) ) );
}

Screen::~Screen()
{
	clearMap();
}


void Screen::slot_mouseLeftPressed( GenericCell * c )
{
	_selector->handleLeftClick( c );
	cellChanged( c );
	_leftPressed = true;
}

void Screen::slot_mouseRightPressed( GenericCell * c )
{
	_currentCell = c;
	_menu->popup( QCursor::pos() );
}

void Screen::slot_mouseReleased()
{
	_leftPressed = false;
}

void Screen::slot_mouseMoved( GenericCell * c )
{
	if( _currentCell != c ) {
		updateStatusBar( c );
		if( c->getLord() ) {
			setCursor( Qt::WaitCursor );
		} else if( c->getBase() ) {
			setCursor( Qt::WaitCursor );
		} else if( c->getBuilding() ) {
			setCursor( Qt::WaitCursor );
		} else if( c->getEvent() ) {
			setCursor( Qt::WaitCursor );
		} else if( c->getCreature() ) {
			setCursor( Qt::WaitCursor );
		} else if( c->getDecorationGroup() ) {
			setCursor( Qt::PointingHandCursor );
		} else if( c->getCoeff() < 0  || !c->isStoppable() || !c->isFree() ) {
			setCursor( Qt::ForbiddenCursor );
	//	} else if( c->getTransition() ) {
	//		setCursor( Qt::upArrowCursor );
		} else {
			setCursor( Qt::ArrowCursor );
		}
		// XXX: _currentCell->setBorder( false );
		_currentCell = c;
		if( _leftPressed ) {
			_selector->handleLeftClick( c );
			cellChanged( c );
		}
		// XXX: _currentCell->setBorder( true );
	}
}



void Screen::slot_popup( int num )
{
	switch( num ) {
		case M_DELETE:
			handleDelete();
			break;
		case M_PROPERTIES:
			handleProperties();
			break;
	}

}

void Screen::slot_centerMinimap( qreal x, qreal y )
{
	_view->centerOn(x, y);
}

void Screen::displayMiniMap( bool state )
{
	if( _miniMap ) {
		if( state ) {
			_miniMap->show();
			emit sig_redrawMap( theMap );
		} else {
			_miniMap->hide();
		}
	}
}

void Screen::handleDelete()
{
	if( _currentCell->getLord() ) {
		GenericLord * lord = _currentCell->getLord();
		if(lord->getOwner()){
			lord->getOwner()->removeLord(lord);
		}
		lord->setCell( NULL );
	} else if( _currentCell->getCreature() ) {
		GenericMapCreature * creature = _currentCell->getCreature();
		removeMapCreature(creature);
		_currentCell->setCreature( NULL );
	} else if( _currentCell->getBuilding() ) {
		GenericBuilding * build = _currentCell->getBuilding();
		removeBuilding( build );
		_currentCell->setBuilding( NULL );
	} else if( _currentCell->getBase() ) {
		GenericBase * base = _currentCell->getBase();
		removeBase(  base );
		_currentCell->setBase( NULL );
	} else if( _currentCell->getEvent() ) {
		Event * event = (Event *)( _currentCell->getEvent() );
		removeEvent( event );
		_currentCell->setEvent( NULL );
	}

	_currentCell->setDecoration( 0, 0 );
	_currentCell->setTransition( 0 );
	_currentCell->setTransitionCellType( 0 );

	cellChanged( _currentCell );
}

void Screen::handleProperties()
{
	if( _currentCell->getLord() ) {
		handlePropertiesLord();
	} else if( _currentCell->getBase() ) {
		handlePropertiesBase();
	} else if( _currentCell->getBuilding() ) {
		handlePropertiesBuilding();
	} else if (_currentCell->getCreature() ) {
		handlePropertiesCreature();
	} else if( _currentCell->getEvent() ) {
		handlePropertiesEvent();
	}
}

void Screen::handlePropertiesLord()
{
	/// XXX: to improve (member for PropeertiesLord... ?? )
	PropertiesLord * propLord = new PropertiesLord( & _players );

	//* Here we'll receive info about all lords and give this info to dialog (for ids...)*/
	int numLords = 0;
	uint* ids = 0;

	for( uint i = 0; i < theMap->getHeight(); i++ ) {
		for( uint j = 0; j < theMap->getWidth(); j++ ) {
			if( theMap->at( i, j )->getLord() ) {
				GenericLord* lord = theMap->at( i, j )->getLord();
				if( lord != _currentCell->getLord() ) {
					numLords++;
				}
			}
		}
	}

	if (numLords > 0) {
		ids = new uint [numLords];
		numLords = 0;
		for( uint i = 0; i < theMap->getHeight(); i++ ) {
			for( uint j = 0; j < theMap->getWidth(); j++ ) {
				if (theMap->at( i, j )->getLord ()) {
					GenericLord* lord = theMap->at( i, j )->getLord ();
					if (lord != _currentCell->getLord ()) {
						ids[numLords] = lord->getId ();
						numLords++;
					}
				}
			}
		}
	}

	propLord->init( _currentCell->getLord(), ids, numLords );
	propLord->exec();

	if (ids) {
		delete[] ids;
	}
	delete propLord;
}

void Screen::handlePropertiesBase()
{
	PropertiesBase * propBase = new PropertiesBase( & _players );
	propBase->init( _currentCell->getBase() );
	propBase->exec();
	
	delete propBase;
}

void Screen::handlePropertiesBuilding()
{
	PropertiesBuilding propBuilding( & _players );
	propBuilding.init( _currentCell->getBuilding() );
	propBuilding.exec();
}

void Screen::handlePropertiesCreature()
{
	PropertiesCreature propCreature;
        propCreature.init( _currentCell->getCreature() );
        propCreature.exec();
}

void Screen::handlePropertiesEvent()
{
	PropertiesEvent propEvent( _currentCell->getEvent() );
	propEvent.exec();
}

void Screen::updateStatusBar( GenericCell * c )
{
	if( c ) {
		QString msg;

		msg = tr( "(Cell %1x%2)" ).arg( c->getRow()).arg( c->getCol() );
		emit sig_status( msg );
	}
}

void Screen::cellChanged( GenericCell * c )
{
	//int width = DataTheme.tiles.getWidth();
	//int height = DataTheme.tiles.getHeight();
	
	TRACE("cellChanged row %d, col %d",c->getRow(), c->getCol() );

	_changed = true;

	emit sig_cellChanged( c->getRow(), c->getCol() );

	theMap->updateMap();
	/*
	// TODO the graphicalMap/Map class should handle cell to rect updating
	// HACK large drawing for all cases
	for ( int row = std::max( 0, c->getRow() - 1 ); row < std::min( c->getRow() + 2, (int)_map->getHeight() ); row++ ) {
		for ( int col = std::max( 0, c->getCol() - 1); col < std::min( c->getCol() + 2, (int)_map->getWidth() ); col++ ) {
			theMap->updateMapRect( QRectF( col * width, row * height, width, height) );
		}
	}*/
}

void Screen::displayInfo()
{
	ScenarioInfo info( this );

	info.setNbPlayer( getNbPlayer() );
	info.setName( getScenarioName() );
	info.setTheme( getScenarioTheme() );
	info.setDescription( getScenarioDescription() );
	info.setDay(getDateByType( 0 ) );
	info.setWeek(getDateByType( 2 ) );
	info.setMonth(getDateByType( 3 ) );
	info.setYear(getDateByType( 4 ) );
	if(theMap) {
		info.setMapWidth( theMap->getWidth() );
		info.setMapHeight( theMap->getHeight() );
	}
	if( info.exec() ) {
		setNbPlayer( info.getNbPlayer() );
		setScenarioName( info.getName() );
		setScenarioTheme( info.getTheme() );
		setScenarioDescription( info.getDescription() );
		setDateByType( 0, info.getDay() > 0 ? info.getDay() : 1  );
		setDateByType( 1, 1 );
		setDateByType( 2, info.getWeek()  );
		setDateByType( 3, info.getMonth()  );
		setDateByType( 4, info.getYear() );

		adjustPlayers();
	}
}

bool Screen::loadScenario( const QString filename )
{
	reinit();

	if ( !load( filename ) ) {
		return false;
	}

	_map->computeStoppable();
	emit sig_redrawMap( theMap );
	emit sig_enableGame( true );
	//HACK to workaround some widget problems
	layout()->update();
	return true;
}

void Screen::save( QString filename )
{
	if(!filename.contains(".scn") && !filename.contains(".gam")){
		filename.append(".scn");
	}
	QFile f( filename );

	if (! f.open(QIODevice::WriteOnly) ) {
		logEE("Could not open file %s for writing\n", qPrintable(filename) );
		return;
	}

	_changed = false; 

	QTextStream ts( &f );

	GraphicalGameData::save( &ts );

	f.close();
}

bool Screen::newScenario()
{
	bool ret = false;

	ScenarioInfo info( this );

	if( info.exec() ) {
		ret = true;

		reinit();
		theMap->newUnknownMap( info.getMapHeight(), info.getMapWidth() );
		_players.clear();
		setScenarioName( info.getName() );
		setScenarioTheme( info.getTheme() );
		setScenarioDescription( info.getDescription() );
		setNbPlayer( info.getNbPlayer() );
		setDateByType( 0, info.getDay() > 0 ? info.getDay() : 1  );
		setDateByType( 1, 1 );
		setDateByType( 2, info.getWeek() > 0 ? info.getWeek() : 1 );
		setDateByType( 3, info.getMonth()  > 0 ? info.getMonth() : 1);
		setDateByType( 4, info.getYear() > 0 ? info.getYear() : 1044);
		for( uint i = 0; i < info.getNbPlayer(); i++ ) {
			GenericPlayer * player = new GenericPlayer( theMap );
			player->setNum( i );
			player->setName( QString("Player %1").arg(i) );
			_players.append( player );
		}

		emit sig_redrawMap( theMap );
		emit sig_enableGame( true );
		//HACK to workaround some widget problems
		_miniMap->slot_mapviewResized( _view->width() / theMap->width(), _view->height() / theMap->height() );
		layout()->update();
	}
	
	return ret;
}

void Screen::clearMap()
{
	TRACE("Screen::clearMap");

	if( theMap ) {
		int height = theMap->getHeight();
		int width = theMap->getWidth();
		theMap->clearData();
		reinit();
		theMap->newUnknownMap( height, width );
		
		emit sig_redrawMap( theMap );
		emit sig_enableGame( false );
		
		_map->computeStoppable();
	}
}

void Screen::fillMap( uint tile, uint divers )
{
	if( theMap ) {
		for( uint i = 0; i < theMap->getHeight(); i++ ) {
			for( uint j = 0; j < theMap->getWidth(); j++ ) {
				theMap->at( i, j )->setType( tile );
				if( ( divers == 0 ) && ( tile != 0 ) ) {
					theMap->at( i, j )->setRandomDiversification();
				} else {
					theMap->at( i, j )->setDiversification( divers - 1 );
				}
			}
		}
	}	
	theMap->updateMap();
	_miniMap->redrawMap( theMap );
}

void Screen::displayPlayersProperties ()
{
	PropertiesPlayers propertiesPlayerDialog (this, &_players);

	propertiesPlayerDialog.exec ();
}

void Screen::manageQuests()
{
	DisplayQuests * quests = new DisplayQuests( this );
	quests->reinit( _quests );
	quests->exec();
	delete quests;
}

void Screen::gridMap()
{
	if(theMap) {
		theMap->activateGrid();
	}
}

QAction * Screen::addAction( const QString & label, POPUP_ITEM id, QSignalMapper * sigmap )
{
	QAction * action;

	action = new QAction( label, this );
	_actions.insert( id, action );
	_menu->addAction( action );
	sigmap->setMapping( action, id );
	connect( action, SIGNAL( triggered() ), sigmap, SLOT( map() ) );

	return action;
}
