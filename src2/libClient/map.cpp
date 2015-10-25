/****************************************************************
**
** Attal : Lords of Doom
**
** map.cpp
** Manage the global view
**
** Version : $Id: map.cpp,v 1.38 2008/07/02 08:22:50 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include <QFile>
#include <QString>
#include <QTextStream>
// application specific includes
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/dataTheme.h"

#include "libClient/cell.h"
#include "libClient/lord.h"
#include "libClient/building.h"
#include "libClient/graphicalPath.h"
#include "libClient/mapCreature.h"
#include "libClient/event.h"
#include "libClient/imageTheme.h"

//
// ----- Map -----
//

Map::Map( QObject * parent , const char * name )
  : GraphicalMap( parent , name ),
    GenericMap()
{
	_isGrid = false;
	_showPath = new GraphicalPath( this );
}

Map::~Map()
{
	clear();

	if(_path) {
		delete _path;
		_path = NULL;
	}
	
	if( _showPath ) {
		_showPath->clearPath();
		delete _showPath;
	}

}

void Map::clear()
{
	TRACE("Map::clear");
	
	_showPath->clearPath();

	if( _theCells != NULL ) {
		for( uint i = 0 ; i < _height ; i++ ) {
			for( uint j = 0; j < _width; j++ ) {
				delete (Cell*)_theCells[i][j];
			}
			delete [] _theCells[i];
		}
		delete [] _theCells;	
	}
	_theCells = NULL;
	
	_height = 0;
	_width = 0;

	if(_path) {
		_path->clear();
	}
	_isGrid = false;

	updateMap();
}

void Map::newUnknownMap( int h, int w )
{
	newMapType( h, w, 0 );
}

void Map::newMapType( int h, int w, int typ )
{
	clear();
	_height = h;
	_width = w;
	_theCells = new GenericCell **[_height];
	uint i;

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	for( i = 0; i < _height; i++ ) {
		for( uint j = 0; j < _width; j++ ) {
			_theCells[i][j] = (GenericCell *)( new Cell( i, j, this ) );
			((Cell*)_theCells[i][j])->setType( typ );
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
		      uint decorationGroup, 
		      uint decorationItem ,
		      uchar diversification)
{
	GenericMap::changeCell( i, j, typ, transition, typtra, decorationGroup, decorationItem, diversification );
	((Cell*)at( i, j ))->setDiversification( diversification );

	//use signal changed instead directly update
	int width = DataTheme.tiles.getWidth();
	int height = DataTheme.tiles.getHeight();
	
	emit sig_cellChanged( i , j );
	updateMapRect(QRectF( j * width, i * height, width, height ));

}

bool Map::load( QTextStream * ts, int width, int heigth )
{
	_width = width;
	_height = heigth;
	uint i, j;
	
	_theCells = (GenericCell ***) ( new Cell **[_height] );

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = (GenericCell**) ( new Cell *[_width] );
	}
	int val;
	uint item;
	
	// Loading base cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j] = (GenericCell *)( new Cell( i, j, this ) );
			((Cell*)_theCells[i][j])->setType( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}

	// Loading diversification cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setDiversification( val );
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
				if( val ) {
					*ts >> item;	
					_theCells[i][j]->setDecoration( val, item );
				}
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	
	_path = new PathFinder( _width, _height, this );
	updateMap();
	autoSize();
	return true;	
}

void Map::activateGrid()
{
	// TODO implement a boolean toggle for drawBackground to draw a grid //((Cell *) _theCells[i][j])->activateGrid(!_isGrid);
	_isGrid = !_isGrid;
	updateMap();
}

void Map::drawBackground( QPainter * painter , const QRectF & rect )
{
	//mantain optimized 
	int tileh = DataTheme.tiles.getHeight();
	int tilew = DataTheme.tiles.getWidth();
	int iwidth = (int)_width;
	int iheight = (int)_height;
	int rowinit =std::max(0, (int)(rect.y() / tileh ));
	int colinit  =std::max(0, (int)(rect.x() / tilew ));
	int rowend = std::min(iheight, (int)(rect.bottom() / tileh ) + 1);
	int colend = std::min(iwidth, (int)(rect.right() / tilew ) + 1);


	for (int row = rowinit; row < rowend; row++) {
		for (int col = colinit; col < colend; col++) {
			GenericCell * cell = at( row,col );
			int type = cell->getType();
			if (row < iheight && col < iwidth && type ) { // possibly unnecessary inside-of-map tests
				painter->drawPixmap(col * tilew, row * tileh, tilew , tileh , (*ImageTheme.cells[type])[cell->getDiversification()]);
				if ( cell->getTransitionCellType())
					painter->drawPixmap(col * tilew, row * tileh, tilew, tileh,
					(*ImageTheme.transition[cell->getTransitionCellType() - 1])[std::max(0, cell->getTransition() - 1)]); // HACK std::max used to workaround a bug
				/*if (at(row, col)->getDecorationGroup())
					painter->drawPixmap(col * DataTheme.tiles.getWidth(), row * DataTheme.tiles.getHeight(), DataTheme.tiles.getWidth(), DataTheme.tiles.getHeight(),
					(*ImageTheme.decoration[at(row, col)->getDecorationGroup() - 1])[at(row, col)->getDecorationItem()]);*/
			}
		}
	}
}

void Map::drawForeground( QPainter * painter , const QRectF & rect )
{
	painter->setPen(Qt::darkRed);
	int tileh, tilew;
	if (getHeight() && getWidth()) {

		tileh = (int)height() / _height;
		tilew = (int)width() / _width;
	} else {
		tileh = _shroudImage.height();
		tilew = _shroudImage.width();
	}

	int iwidth = (int)_width;
	int iheight = (int)_height;
	int rowinit =std::max(0, (int)(rect.y() / tileh ));
	int colinit  =std::max(0, (int)(rect.x() / tilew ));
	int rowend = std::min(iheight, (int)(rect.bottom() / tileh ) + 1);
	int colend = std::min(iwidth, (int)(rect.right() / tilew ) + 1);


	for (int row = rowinit; row < rowend; row++) {
		for (int col = colinit; col < colend; col++) {
			if (row < iheight && col < iwidth && !at(row, col)->getType() ) { // possibly unnecessary inside-of-map tests
				painter->drawPixmap(col * tilew, row * tileh, tileh, tilew, _shroudImage);
			}
			if (_isGrid) {
				painter->drawRect(col * tilew, row * tileh, tileh, tilew);
			}
		}
	}
	
}

void Map::clearPath()
{
	_showPath->clearPath();
}

void Map::updateMap()
{
	//TRACE("update map");
	updateMapRect(sceneRect());
}

void Map::updateMapRect( QRectF rect )
{
	//TRACE("update map rect");
	QList<QRectF> rectlist;
	rectlist.append( rect );
	emit changed( rectlist );
}
