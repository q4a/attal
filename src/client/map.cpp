/****************************************************************
**
** Attal : Lords of Doom
**
** map.cpp
** Manage the global view
**
** Version : $Id: map.cpp,v 1.23 2002/07/16 20:04:28 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

#include "map.h"

 
// generic include files
#include <stdlib.h>

// include files for QT
#include <qstring.h>
#include <qstack.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qcanvas.h>
#include <qlist.h>

// application specific includes
#include "common/log.h"
#include "common/pathFinder.h"
#include "client/cell.h"
#include "client/lord.h"

/** add comments here */
Map::Map( QObject * parent , const char * name )
  : GraphicalMap( parent , name ),
    GenericMap()
{
	
}

Map::~Map()
{
	delete _path;

	// XXX: delete [][] 
	for( int i=0 ; i < _height ; i++) {
		delete [] _theCells[i];
		_theCells[i] = 0;
	}
	delete [] _theCells;
	_theCells = 0;
}

void Map::clearMap()
{
	if( _theCells != 0 ) {
		for( int i = 0 ; i < _height ; i++ ) {
			for( int j = 0; j < _width; j++ )
				delete (Cell*)_theCells[i][j];
			delete [] _theCells[i];
		}
		delete [] _theCells;	
	}
	_theCells = 0;
	
	_height = 0;
	_width = 0;
	_path->clear();
}

void Map::newMap( int h, int w )
{
	clearMap();
	_height = h;
	_width = w;
	_theCells = new GenericCell **[_height];
	int i;

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	for( i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			Cell * tempo;
			tempo = new Cell( i, j, this );
			tempo->show();
			// XXX: bof
			tempo->setType( 0 );
			_theCells[i][j] = (GenericCell *)tempo;
		}
	}
	_path->newMap( _height, _width, (GenericMap*)this );
	autoSize();
}

void Map::newUnknownMap( int h, int w )
{
	clearMap();
	_height = h;
	_width = w;
	_theCells = new GenericCell **[_height];
	int i;

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	for( i = 0; i < _height; i++ ) {
		for( int j=0; j < _width; j++ ) {
			Cell * tempo;
			tempo = new Cell( i, j, this );
			tempo->show();
			tempo->setType( 0 );
			_theCells[i][j] = (GenericCell *)tempo;
		}
	}

	_path->newMap( _height, _width, (GenericMap*)this );
	autoSize();
}

void Map::changeCell( int i,
		      int j,
		      int typ,
		      int transition,
		      int typtra,
		      int decoration )
{
	GenericMap::changeCell( i, j, typ, transition, typtra, decoration );

	if( _theCells[i][j]->getType() == 0 ) {
		/*((Cell*)_theCells[i][j])->setType( typ );
		((Cell*)_theCells[i][j])->setTransition( transition );
		((Cell*)_theCells[i][j])->setTransitionCellType( typtra );
		((Cell*)_theCells[i][j])->setDecoration( decoration );*/
		((Cell*)_theCells[i][j])->show();
	}/* else {
		logEE( "Cell type should be unknown" );
	}*/

}

bool Map::load( QTextStream * ts, int width, int heigth )
{
	_width = width;
	_height = heigth;
	int i, j;
	
	_theCells = (GenericCell ***) ( new Cell **[_height] );

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = (GenericCell**) ( new Cell *[_width] );
	}
	int val;
	
	// Loading base cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				Cell * tempo;
				tempo = new Cell( i, j, this );
				tempo->show();
				*ts >> val;
				tempo->setType( val );
				_theCells[i][j] = (GenericCell*)tempo;
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}
	
	// Loading transition type
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setTransition( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	// Loading transition cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setTransitionCellType( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	// Loading decoration of the ground
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setDecoration( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	
	_path = new PathFinder( _width, _height, this );
	autoSize();
	return true;	
}
