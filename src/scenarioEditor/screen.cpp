/****************************************************************
**
** Attal : Lords of Doom
**
** screen.cpp
** main widget of the editor
**
** Version : $Id: screen.cpp,v 1.27 2002/12/15 12:09:56 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qlayout.h>
#include <qlabel.h>
#include <qfile.h>
#include <qpopupmenu.h>
#include <qtextstream.h>
#include <qinputdialog.h>
#include <qcursor.h>
// application specific includes
#include "scenarioEditor/scenarioInfo.h"
#include "scenarioEditor/itemSelector.h"
#include "scenarioEditor/propertiesItem.h"
#include "scenarioEditor/propertiesLord.h"
#include "client/map.h"
#include "client/mapView.h"
#include "client/cell.h"
#include "client/graphicalArtefact.h"
#include "common/genericPlayer.h"
#include "common/log.h"
#include "server/parser.h"

extern Map * theMap;

/** add comments here */
Screen::Screen( QWidget * parent, char * name )
	: QWidget( parent, name ), GraphicalGameData()
{	
	_leftPressed = false;
	_currentCell = 0;
		
	QHBoxLayout * lay = new QHBoxLayout( this );
	
	_selector = new ItemSelector( this, this );

	theMap = new Map( this );	
	_map = theMap;
	MapView * view = new MapView( theMap, this );

	lay->addWidget( _selector );
	lay->addWidget( view, 1 );
	lay->activate();

	_menu = new QPopupMenu( this );
	_menu->insertItem( "&Delete", M_DELETE );
	_menu->insertItem( "&Properties", M_PROPERTIES );
	connect( _menu, SIGNAL( activated( int ) ), SLOT( slot_popup( int ) ) );
	
	connect( view, SIGNAL( sig_mouseMoved  (Cell *) ), SLOT( slot_mouseMoved  (Cell *) ) );
	connect( view, SIGNAL( sig_mouseLeftPressed(Cell *) ), SLOT( slot_mouseLeftPressed(Cell *) ) );
	connect( view, SIGNAL( sig_mouseRightPressed(Cell *) ), SLOT( slot_mouseRightPressed(Cell *) ) );
	connect( view, SIGNAL( sig_mouseReleased() ), SLOT( slot_mouseReleased() ) );
}

void Screen::slot_mouseLeftPressed( Cell * c )
{
	_selector->handleLeftClick( c );
	_leftPressed = true;
}

void Screen::slot_mouseRightPressed( Cell * c )
{
	_currentCell = c;
	_menu->popup( QCursor::pos() );
}

void Screen::slot_mouseReleased()
{
	_leftPressed = false;
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

void Screen::handleDelete()
{
	if( _currentCell->getLord() ) {
		GenericLord * lord = _currentCell->getLord();
		delete lord;
		_currentCell->setLord( 0 );
	} else if( _currentCell->getBuilding() ) {
		GenericBuilding * build = _currentCell->getBuilding();
		delete build;
		_currentCell->setBuilding( 0 );
	} else if( _currentCell->getBase() ) {
		GenericBase * base = _currentCell->getBase();
		delete base;
		_currentCell->setBase( 0 );
	} else if( _currentCell->getArtefact() ) {
		Artefact * artefact = (Artefact *)( _currentCell->getArtefact() );
		delete artefact;
		_currentCell->setArtefact( 0 );
	} else if( _currentCell->getDecoration() ) {
		_currentCell->setDecoration( 0 );
	} else if( _currentCell->getTransition() ) {
		_currentCell->setTransition( 0 );
		_currentCell->setTransitionCellType( 0 );
	}
}

void Screen::handleProperties()
{
	if( _currentCell->getLord() ) {
		/// XXX: to improve (member for PropeertiesLord... ?? )
		PropertiesLord * propLord = new PropertiesLord( & _players );
		propLord->init( _currentCell->getLord() );
		propLord->exec();
		delete propLord;
		return;
	} else if( _currentCell->getBase() ) {
		PropertiesBase * propBase = new PropertiesBase();
		propBase->init( _currentCell->getBase() );
		propBase->exec();
		delete propBase;
		return;
	}
}

void Screen::slot_mouseMoved( Cell * c )
{
	if( _currentCell != c ) {
		// XXX: _currentCell->setBorder( false );
		_currentCell = c;
		if( _leftPressed ) {
			_selector->handleLeftClick( c );
		}
		// XXX: _currentCell->setBorder( true );
	}
}

void Screen::displayInfo()
{
	ScenarioInfo info( this );

	info.setNbPlayer( getNbPlayer() );
	info.setName( getScenarioName() );
	info.setTheme( getScenarioTheme() );
	info.setDescription( getScenarioDescription() );
	if( info.exec() ) {
		// XXX:get back the infos...
	}
}

bool Screen::load( const QString filename )
{
	reinit();

	ScenarioParser handler( (GameData*)this );
	QFile file( filename );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", filename.latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

void Screen::save( const QString filename )
{
	QFile f( filename );

	if (! f.open(IO_WriteOnly) ) {
		logEE("Could not open file %s for writing\n", filename.latin1() );
		return;
	}

	QTextStream ts( &f );

	GraphicalGameData::save( &ts );

	f.close();
}

void Screen::newScenario()
{
	ScenarioInfo info( this );

	if( info.exec() ) {
		reinit();
		theMap->newMap( info.getMapHeight(), info.getMapWidth() );
		_players.clear();
		setScenarioName( info.getName() );
		setScenarioTheme( info.getTheme() );
		setScenarioDescription( info.getDescription() );
		setNbPlayer( info.getNbPlayer() );
		for( uint i = 0; i < info.getNbPlayer(); i++ ) {
			GenericPlayer * player = new GenericPlayer( theMap );
			player->setNum( i );
			_players.append( player );
		}
	}
}

void Screen::clearMap()
{
	if( theMap ) {
		for( int i = 0; i < theMap->getHeight(); i++ ) {
			for( int j = 0; j < theMap->getWidth(); j++ ) {
				if( theMap->at( i, j )->getLord() ) {
					delete theMap->at( i, j )->getLord();
					theMap->at( i, j )->setLord( 0 );
				}
				if( theMap->at( i, j )->getBuilding() ) {
					delete theMap->at( i, j )->getBuilding();
					theMap->at( i, j )->setBuilding( 0 );
				}
				theMap->at( i, j )->setDecoration( 0 );
				theMap->at( i, j )->setTransitionCellType( 0 );
				theMap->at( i, j )->setTransition( 0 );
			}
		}
		_players.clear();
		theMap->newMap( theMap->getHeight(), theMap->getWidth() );
	}
}

void Screen::fillMap( int t )
{
	if( theMap ) {
		for( int i = 0; i < theMap->getHeight(); i++ ) {
			for( int j = 0; j < theMap->getWidth(); j++ ) {
				theMap->at( i, j )->setType( t );
			}
		}
	}
}
