/****************************************************************
**
** Attal : Lords of Doom
**
** fightMap.cpp
** Manage the global view
**
** Version : $Id: fightMap.cpp,v 1.15 2006/10/02 22:16:09 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 07/12/2000
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

#include "fightMap.h"

// generic include files
#include <stdlib.h>
// include files for QT
// application specific includes
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/genericFightCell.h"

#include "libFight/fightCell.h"
#include "libFight/fightUnit.h"


FightMap::FightMap( QObject * parent , const char * /* name */)
  : GraphicalFightMap( parent ),
    GenericFightMap()
{
	newFightMap( 9, 15 );
}


FightMap::~FightMap()
{
	// XXX: delete [][]
	// XXX to complete
}

void FightMap::clearFightMap()
{
	if( _genericFightCells != NULL ) {
		for( int i = 0 ; i < _height ; i++ ) {
			for( int j = 0; j < _width; j++ ){
				if( _genericFightCells[i][j] )
				    delete (FightCell*)_genericFightCells[i][j];
			}
			if( _genericFightCells[i] )
			    delete [] _genericFightCells[i];
		}

		if( _genericFightCells )
			delete [] _genericFightCells;
	}

	_genericFightCells = NULL;
	_height = 0;
	_width = 0;
}

void FightMap::newFightMap( int h, int w, bool horizontalDraw )
{
	_height = h;
	_width = w;
	_genericFightCells = new GenericFightCell **[_height];
	int i;

	for( i = 0; i < _height; i++ ) {
		_genericFightCells[i] = new GenericFightCell *[_width];
	}

	for( i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			FightCell * tempo;
			tempo = new FightCell( i, j, this, true /*false*/ );
			tempo->setType( AttalCommon::NORMAL );
			_genericFightCells[i][j] = tempo;
			tempo->show ();
		}
	}

	_horizontalDraw = horizontalDraw;
}

void FightMap::changeFightCell( int i, int j, AttalCommon::TypeFightCell typ )
{
	if( _genericFightCells[i][j]->getType() == AttalCommon::UNKNOWN_TYPE ) {
		( (FightCell*)_genericFightCells[i][j] )->setType( typ );
		( (FightCell*)_genericFightCells[i][j] )->show();
	} else {
		logEE( "FightCell type should be unknown" );
	}
}

void FightMap::initPath( GenericFightUnit * unit )
{
	GenericFightMap::initPath( unit );

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			switch( ( (FightCell*)_genericFightCells[i][j] )->getAccess() ) {
			case AttalCommon::UNKNOWN_ACCESS:
				( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::gray );
				break;
			case AttalCommon::NONE:
				( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::red );
				break;
			case AttalCommon::NEAR_FREE:
				( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::blue );
				break;
			case AttalCommon::NEAR_OCCUPIED:
				( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::yellow );
				break;
			case AttalCommon::FAR_FREE:
			case AttalCommon::FAR_OCCUPIED:
				( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::green );
				break;
			default:
				logEE( "Should not happen %d", ( (FightCell*)_genericFightCells[i][j] )->getAccess() );
				break;
			}
		}
	}

	update();
}

void FightMap::clearPath()
{
	GenericFightMap::clearPath();

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			( (FightCell*)_genericFightCells[i][j] )->setColor( Qt::gray );
		}
	}

	update();
}

void FightMap::showCells ()
{
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			( (FightCell*)_genericFightCells[i][j] )->setEnabled( true );
		}
	}

	update ();
}

void FightMap::hideCells ()
{
	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			( (FightCell*)_genericFightCells[i][j] )->setEnabled( false );
		}
	}

	update ();
}
