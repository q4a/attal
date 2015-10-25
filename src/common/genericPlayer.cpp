/****************************************************************
**
** Attal : Lords of Doom
**
** genericPlayer.cpp
** manage a player
**
** Version : $Id: genericPlayer.cpp,v 1.36 2003/05/09 12:46:04 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/08/2000
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

#include "genericPlayer.h"

// generic include files
#include <math.h>
// include files for QT
#include <qtextstream.h>
// application specific include files
#include "common/log.h"
#include "common/dataTheme.h"
#include "common/genericMap.h"
#include "common/pathFinder.h"

extern DataTheme DataTheme;

int computeDist( GenericCell * cell1, GenericCell * cell2 )
{
	int ret = 0;
	int row1 = cell1->getRow();
	int col1 = cell1->getCol();
	int row2 = cell2->getRow();
	int col2 = cell2->getCol();

	ret = int( sqrt( ( (col1-col2)*(col1-col2) ) + ( (row1-row2)*(row1-row2) ) ) );

	return ret;
}

//
// ----- GenericPlayer -----
//

GenericPlayer::GenericPlayer( GenericMap * theMap )
	:  _num( 0 ),
	_alive( true ),
	_selectedLord( NULL )

{
	for( int i = 0; i < MAX_RESS; i++ ) {
		_ress[i] = 0;
	}
	_theLords.setAutoDelete( false );
	_theBuild.setAutoDelete( false );
	_theBase.setAutoDelete( false );
	_map = theMap;
	_vision = 0;
	setName( QString("New player") );
}

GenericPlayer::~GenericPlayer()
{

}

void GenericPlayer::save( QTextStream * ts, int indent )
{
	uint j;

	indentation( ts, indent );
	(*ts) << "<player>" << endl;

	indentation( ts, indent );
	(*ts) << "\t<name>" << getName().latin1() << "</name>" << endl;

	for( j = 0; j < numBase(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<base>";
		(*ts) << getBase( j )->getId();
		(*ts) << "</base>" << endl;
	}

	for( j = 0; j < numBuilding(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<building>";
		(*ts) << getBuilding( j )->getId();
		(*ts) << "</building>" << endl;
	}

	for( j = 0; j < numLord(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<lord>";
		(*ts) << getLord( j )->getId();
		(*ts) << "</lord>" << endl;
	}

	for( j = 0; j < MAX_RESS; j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<resource type=\"" << j << "\">";
		(*ts) << getResource( j );
		(*ts) << "</resource>" << endl;
	}

	indentation( ts, indent );
	(*ts) << "</player>" << endl;
}

void GenericPlayer::newTurn()
{
	uint i;
	// XXX: do 'lord->newTurn()' ??
	for( i = 0; i < numLord(); i++ ) {
		getLord( i )->setBaseCharac( MOVE, getLord( i )->getCharac( MAXMOVE ) );
	}
	for( i = 0; i < numBase(); i++ ) {
		_theBase.at( i )->canBuild( true );
	}
}

void GenericPlayer::newWeek()
{
	for( uint i = 0; i < numBase(); i++ ) {
		_theBase.at( i )->initCreatureProduction();
	}
}

void GenericPlayer::addLord( GenericLord * lord )
{
       if( ! hasLord( lord ) ) {
              _theLords.append( lord );
       }
}
void GenericPlayer::setResource( uint num, uint val )
{
	if( num < MAX_RESS ) {
		_ress[num] = val;
	}
}

void GenericPlayer::decreaseResource( uint num, uint val )
{
	if( num < MAX_RESS ) {
		if( val <= _ress[num] ) {
			_ress[num] = _ress[num] - val;
		} else {
			_ress[num] = 0;
		}
	}
}

void GenericPlayer::increaseResource( uint num, uint val )
{
	if( num < MAX_RESS ) {
		_ress[num] = _ress[num] + val;
	}
}

uint GenericPlayer::getResource( uint num )
{
	if( num < MAX_RESS ) {
		return _ress[num];
	} else {
		logEE( "Resource number too high : %d", num );
		return 0;
	}
}

bool GenericPlayer::shouldEnd()
{
	// XXX: to improve; don't compare with 0 but with min of coeff of neighbour cells 
	for( uint i = 0; i < numLord(); i++ ) {
		if( ( ! _theLords.at( i )->isSleeping() ) && ( _theLords.at( i )->getCharac( MOVE ) != 0 ) )
			return false;
	}
	
	return true;
}

void GenericPlayer::endTurn()
{
	uint i;
	int j;
	// New ressources
	
	// thanks to lords
	for( i = 0; i < numLord(); i++ ) {
		for( j = 0; j < MAX_RESS; j++ ) {
			_ress[j] += _theLords.at( i )->collectRessource( j );
		}
				
		_theLords.at(i)->endTurn();
	}
	
	// thanks to base
	for( i = 0; i < numBase(); i++ ) {
		for( j = 0; j < MAX_RESS; j++ ) {
			_ress[j] += _theBase.at( i )->collectRessource( j );
		}
	}
	
	// thanks to mines
	for( i = 0; i < numBuilding(); i++ ) {
		for( j = 0; j < MAX_RESS; j++ ) { 
		//	_ress[j] += _theBuild.at( i )->collectRessource( j );
		}
	}
}

bool GenericPlayer::selectFirstLord()
{
	for( uint i = 0; i < numLord(); i++ ) {
		if( _theLords.at( i )->isActive() ) {
			_selectedLord = _theLords.at( i );
			return true;
		}
	}
	return false;
}

GenericLord * GenericPlayer::nextLord()
{
	if ( _selectedLord == 0 ) {
		if ( numLord() > 0 ) {
			_selectedLord = _theLords.at( 0 );
			return _theLords.at( 0 );
		} else
			return 0;
	} else {
		uint num = _theLords.findRef( _selectedLord );
		if ( num >= 0 ) {
			if ( num == numLord() - 1 ) {
				_selectedLord = _theLords.at( 0 );
				return _theLords.at( 0 );
			} else {
				_selectedLord = _theLords.at( num+1 );
				return _theLords.at( num+1 );	
			}
		} else {
			if ( numLord() > 0 ) {
				_selectedLord = _theLords.at( 0 );
				return _theLords.at( 0 );
			} else {
				return 0;
			}
		}
	}
}

GenericLord * GenericPlayer::getLordById( int id )
{
    	for( uint i = 0; i < numLord(); i++ ) {
		if( _theLords.at( i )->getId() == id ) {
			return _theLords.at( i );
		}
	}
	return 0;
}

void GenericPlayer::removeLord( int num )
{
	_theLords.remove( num );
	if( _selectedLord == _theLords.at( num ) ) {
		_selectedLord = 0;
	}
}

void GenericPlayer::removeLord( GenericLord * lord )
{
	_theLords.remove( lord );
	if( _selectedLord == lord ) {
		_selectedLord = 0;
	}
}

void GenericPlayer::removeBase( GenericBase * base )
{
	_theBase.remove( base );
	if( _selectedBase ) {
		_selectedBase = 0;
	}
}

GenericBase * GenericPlayer::getBaseById( int id )
{
   	for( uint i = 0; i < numBase(); i++ ) {
		if( _theBase.at( i )->getId() == id ) {
			return _theBase.at( i );
		}
	}
	return 0;
}

void GenericPlayer::cleanData()
{
	int tmp;
	uint i;

	tmp = numLord();
   	for( i = 0; i < tmp; i++ ) {
		removeLord( 0 );
	}

	tmp = numBase();
   	for( i = 0; i < tmp; i++ ) {
		removeBase( 0 );
	}

	tmp = numBuilding();
   	for( i = 0; i < tmp; i++ ) {
		removeBuilding( 0 );
	}
}

bool GenericPlayer::hasLost()
{
	bool ret = false;

	if( _theLords.count() == 0 ) {
		ret = true;
		_alive = false;
	}

	return ret;
}

bool GenericPlayer::canBuy( InsideBuildingModel * building )
{
	bool ret = true;

	for( uint i = 0; i < MAX_RESS; i++ ) {
		if( _ress[i] < building->getCost( i ) ) {
			ret = false;
		}
	}

	return ret;
}

bool GenericPlayer::canBuy( Creature * creature, int number )
{
	bool ret = true;

	for( uint i = 0; i < MAX_RESS; i++ ) {
		if( _ress[i] < (creature->getCost( i ) * number ) ) {
			ret = false;
		}
	}

	return ret;
}

void GenericPlayer::buy( Creature * creature, int number )
{
	for( uint i = 0; i < MAX_RESS; i++ ) {
		_ress[i] = _ress[i] - (creature->getCost( i ) * number );
	}
}

uint GenericPlayer::computeBuyCreatureMax( Creature * creature )
{
	int ret = -1;

	for( int i = 0; i < MAX_RESS; i++ ) {
		if( creature->getCost( i ) >  0 ) {
			int temp = _ress[i] / creature->getCost( i );
			if( ( ret == -1 ) || ( temp < ret ) ) {
				ret = temp;
			}
		}
	}

	if( ret == -1 ) {
		ret = 0;
	}

	return (uint)ret;
}

void GenericPlayer::initMapVision()
{
	/// XXX: delete vision before if needed!!

	int height = _map->getHeight();
	int width = _map->getWidth();
	_vision = new int * [ height ];
	for( int i = 0; i < height; i++ ) {
		_vision[i] = new int [ width ];
		for( int j = 0; j < width; j++ ) {
			_vision[i][j] = 0;
		}
	}
}

QList<GenericCell> GenericPlayer::removeCellVision( GenericCell * cell, int vision )
{
	QList<GenericCell> ret;
	int height = _map->getHeight();
	int width = _map->getWidth();
	FOW_TYPE fow = DataTheme.getFogOfWar();

	if( fow == FOW_REAL ) {
		for( int i = 0; i < height; i++ ) {
			for( int j = 0; j < width; j++ ) {
				if( computeDist( cell, _map->at( i, j ) ) <= vision ) {
					if( _vision[i][j] > 0 ) {
						_vision[i][j]--;
						if( _vision[i][j] == 0 ) {
							ret.append( _map->at( i, j ) );
						}
					}
				}
			}
		}
	}

	return ret;
}

QList<GenericCell> GenericPlayer::addCellVision( GenericCell * cell, int vision )
{
	QList<GenericCell> ret;
	int height = _map->getHeight();
	int width = _map->getWidth();
	FOW_TYPE fow = DataTheme.getFogOfWar();

	for( int i = 0; i < height; i++ ) {
		for( int j = 0; j < width; j++ ) {
			if( computeDist( cell, _map->at( i, j ) ) <= vision ) {
				_vision[i][j]++;
				if( _vision[i][j] == 1 ) {
					ret.append( _map->at( i, j ) );
				} else if( fow == FOW_ONCE ) {
					_vision[i][j] = 2;
				}
			}
		}
	}

	return ret;
}

QList<GenericCell> GenericPlayer::removeLordVision( GenericLord * lord )
{
	int vision = lord->getCharac( VISION );
	GenericCell * cell = lord->getCell();

	return removeCellVision( cell, vision );
}

QList<GenericCell> GenericPlayer::addLordVision( GenericLord * lord )
{
	int vision = lord->getCharac( VISION );
	GenericCell * cell = lord->getCell();
	return addCellVision( cell, vision );
}

QList<GenericCell> GenericPlayer::removeBaseVision( GenericBase * base )
{
	int vision = base->getVision();
	GenericCell * cell = base->getCell();

	return removeCellVision( cell, vision );
}

QList<GenericCell> GenericPlayer::addBaseVision( GenericBase * base )
{
	int vision = base->getVision();
	GenericCell * cell = base->getCell();

	return addCellVision( cell, vision );
}

QList<GenericCell> GenericPlayer::addBuildingVision( GenericBuilding * building )
{
	int vision = 3; // XXX: to change!!!
	GenericCell * cell = building->getCell();

	return addCellVision( cell, vision );
}

QList<GenericCell> GenericPlayer::removeBuildingVision( GenericBuilding * building )
{
	int vision = 3; // XXX: to change!!!
	GenericCell * cell = building->getCell();

	return removeCellVision( cell, vision );
}

bool GenericPlayer::canSee( int row, int col )
{
	bool ret = false;

	if( ( row >= 0 ) && ( row < _map->getHeight() ) && ( col >= 0 ) && ( col < _map->getWidth() ) ) {
		ret = ( _vision[row][col] > 0 );
	}

	return ret;
}

bool GenericPlayer::canSee( GenericCell * cell )
{
	return canSee( cell->getRow(), cell->getCol() );
}



