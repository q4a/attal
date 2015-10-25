/****************************************************************
**
** Attal : Lords of Doom
**
** screen.cpp
** main widget of the editor
**
** Version : $Id: screen.cpp,v 1.9 2001/03/05 17:51:29 audoux Exp $
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
#include <qtextstream.h>
#include <qinputdialog.h>
// application specific includes
#include "editor/chooseTiles.h"
#include "client/map.h"
#include "client/mapView.h"
#include "client/cell.h"
#include "common/log.h"

/** add comments here */
Screen::Screen( QWidget * parent, char * name )
	: QWidget( parent, name )
{	
	_currentCell = 0;
	_info = 0;
	
	QHBoxLayout * lay = new QHBoxLayout( this );
	choose = new ChooseTiles( this );
	
	_map = new Map( this );	
	MapView * view = new MapView( _map, this );
	
	lay->addWidget( view, 1 );
	lay->addSpacing( 3 );
	lay->addWidget( choose );
	lay->activate();
	
	connect( view, SIGNAL( sig_mouseMoved  (Cell *) ), SLOT( slot_mouseMoved  (Cell *) ) );
	connect( view, SIGNAL( sig_mousePressed(Cell *) ), SLOT( slot_mousePressed(Cell *) ) );
}

void Screen::slot_mousePressed( Cell * c )
{
	c->setType( (GenericCell::TypeCell)choose->getTile() );
}

void Screen::slot_mouseMoved( Cell * c )
{
	if( _currentCell != c ) {
		// XXX: _currentCell->setBorder( false );
		_currentCell = c;
		// XXX: _currentCell->setBorder( true );
	}
}

void Screen::displayInfo()
{
	if( !_info )
		_info = new ScenarioInfo( this );
	_info->exec();
}

bool Screen::load( const QString filename )
{
	return _map->load( filename );
}

void Screen::save( const QString filename )
{
	QFile f( filename );	

	if (! f.open(IO_WriteOnly) ) {
		logEE("Could not open file %s for writing\n", filename.latin1() );
		return;
	}
	
	QTextStream ts( &f );	

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE scenario>" << endl;
	ts << "<scenario>" << endl;
	ts << "<name>" << _info->getName() << "</name>" << endl;
	ts << "<theme>" << _info->getTheme() << "</theme>" << endl;
	ts << "<description>" << endl;
	ts << _info->getDescription() << endl;
	ts << "</description>" << endl;
	
	_map->save( &ts );
	
	for( int i = 0; i < _info->getNbPlayer(); i++ ) {
		ts << "<player>" << endl;
		ts << "</player>" << endl;
	}
	
	ts << "</scenario>" << endl;
	
	f.close();
}

void Screen::newMap()
{
	if( !_info )
		_info = new ScenarioInfo( this );
	_info->clear();
	
	if( _info->exec() ) {
		_map->newMap( _info->getMapHeight(), _info->getMapWidth() );
	}
}
