/****************************************************************
**
** Attal : Lords of Doom
**
** fightMap.cpp
** Manage the global view
**
** Version : $Id: fightMap.cpp,v 1.15 2002/07/25 22:18:25 audoux Exp $
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
#include <qcanvas.h>
#include <qlist.h>

// application specific includes
#include "common/log.h"
#include "common/pathFinder.h"
#include "common/genericFightCell.h"
#include "client/fightCell.h"
#include "client/graphicalFightUnit.h"

/** add comments here */
FightMap::FightMap( QObject * parent , const char * name )
  : GraphicalFightMap( parent , name ),
    GenericFightMap()
{
	newFightMap( 9, 15 );
}


FightMap::~FightMap()
{
	// XXX: delete [][]
	// XXX to complete
	logDD("Delete FightMap");
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

void FightMap::newFightMap( int h, int w )
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
			tempo = new FightCell( i, j, this );
			tempo->setType( NORMAL );
			_genericFightCells[i][j] = tempo;
			tempo->show();
		}
	}
}

void FightMap::changeFightCell( int i, int j, TypeFightCell typ )
{
	if( _genericFightCells[i][j]->getType() == UNKNOWN_TYPE ) {
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
			case UNKNOWN_ACCESS:
				( (FightCell*)_genericFightCells[i][j] )->setColor( gray );
				break;
			case NONE:
				( (FightCell*)_genericFightCells[i][j] )->setColor( red );
				break;
			case NEAR:
			case NEAR_OCCUPIED:
				( (FightCell*)_genericFightCells[i][j] )->setColor( blue );
				break;
			case FAR:
			case FAR_OCCUPIED:
				( (FightCell*)_genericFightCells[i][j] )->setColor( green );
				break;
			default:
				logEE( "Should not happen %d", ( (FightCell*)_genericFightCells[i][j] )->getAccess() );
				break;
			}
		}
	}

	setAllChanged();
	update();
}

void FightMap::clearPath()
{
	GenericFightMap::clearPath();

	for( int i = 0; i < _height; i++ ) {
		for( int j = 0; j < _width; j++ ) {
			( (FightCell*)_genericFightCells[i][j] )->setColor( gray );
		}
	}

	setAllChanged();
	update();
}
