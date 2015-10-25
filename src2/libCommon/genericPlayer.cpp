/****************************************************************
**
** Attal : Lords of Doom
**
** genericPlayer.cpp
** manage a player
**
** Version : $Id: genericPlayer.cpp,v 1.62 2009/11/19 22:14:10 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/generalOptions.h"
#include "libCommon/genericMap.h"
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/priceMarket.h"

//
// ----- GenericPlayer -----
//

GenericPlayer::GenericPlayer( GenericMap * theMap )
	:  _num( 0 ),
	_alive( true ),
	_isRuledByAi( false ),
	_selectedLord( NULL),
	_selectedBase( NULL)
{
	_ress = new GenericResourceList();

	_teamId = 0;
	
	_map = theMap;
	_vision = 0;
	_theight = 0;
	_price = new PriceMarket();
	_isTeamSet = false;
	setName( QString( "New player" ) );
	setConnectionName( QString( "New player" ) );
}

GenericPlayer& GenericPlayer::operator = (GenericPlayer& other)
{
	for( int i = 0; i < DataTheme.resources.count(); i++ )
	{
		_ress->setValue(i, other._ress->getValue(i));
	}

	_num = other._num;
	_teamId = other._teamId;
	_name = other._name;
	_alive = other._alive;
	_isRuledByAi = other._isRuledByAi;
	_theLords = other._theLords;
	_theBase = other._theBase;
	_theBuild = other._theBuild;

	_selectedLord = other._selectedLord;
	_selectedBase = other._selectedBase;
	_map = other._map;

	return *this;
}

GenericPlayer::~GenericPlayer()
{
	clearVision();

 if(_price){
	 delete _price;
 }
 delete _ress;
}

bool GenericPlayer::loadVision( QTextStream * ts , int width, int height)
{
	int val;
	int i,j;
	clearVision();
	_theight = height;

	_vision = new int * [ height ];
	for( int i = 0; i < _theight; i++ ) {
		_vision[i] = new int [ width ];
		for( int j = 0; j < width; j++ ) {
			_vision[i][j] = 0;
		}
	}

	for( i = 0; i < _theight; i++ ) {
		for( j = 0; j < width; j++ ) {
			if( ! ts->atEnd() ) {
			  *ts >> val;
			  _vision[i][j] =  val;
			} else {
				logEE("Stream too short");
				for( i = 0; i < _theight; ++i ) {
					delete [] _vision[i];
				}
				delete [] _vision;
				_vision = 0;
				_theight = 0;
				return false;
			}
		}
	}
	return true;
}

void GenericPlayer::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	(*ts) << "<player>" << endl;

	indentation( ts, indent );
	(*ts) << "\t<ruledbyai>" << _isRuledByAi << "</ruledbyai>" << endl;
	
	indentation( ts, indent );
	(*ts) << "\t<name>" << getName().toLatin1() << "</name>" << endl;
	
	indentation( ts, indent );
	(*ts) << "\t<team>" << getTeamId() << "</team>" << endl;
		
	if( _vision ) {
		indentation( ts, indent );
		(*ts) << "\t<vision>";
		(*ts) << endl;
		uint height = _map->getHeight();
		uint width = _map->getWidth();
		for( uint i = 0; i < height; i++ ) {
			(*ts) << "\t\t";
			for( uint j = 0; j < width; j++ ) {
				(*ts) << (int)_vision[i][j] << " ";
			}
			(*ts) << endl;
		}
		(*ts) << flush;
		indentation( ts, indent+1 );
		(*ts) << "</vision>" << endl;
	}

	for( uint j = 0; j < numBase(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<base>";
		(*ts) << getBase( j )->getId();
		(*ts) << "</base>" << endl;
	}

	for( uint j = 0; j < numBuilding(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<building>";
		(*ts) << getBuilding( j )->getId();
		(*ts) << "</building>" << endl;
	}

	for( uint j = 0; j < numLord(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<lord>";
		(*ts) << getLord( j )->getId();
		(*ts) << "</lord>" << endl;
	}

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<resource type=\"" << j << "\">";
		(*ts) << _ress->getValue( j );
		(*ts) << "</resource>" << endl;
	}

	indentation( ts, indent );
	(*ts) << "</player>" << endl;
	*ts << flush;
}

void GenericPlayer::setNum( int num )
{ 
	_num = num; 
	if( ! _isTeamSet ) {
		_teamId = num;
	}
}

void GenericPlayer::newTurn()
{
	TRACE("GenericPlayer::newTurn");

	uint i;
	for( i = 0; i < numLord(); i++ ) {
		getLord( i )->newTurn();
	}

	for( i = 0; i < numBase(); i++ ) {
		_theBase.at( i )->setState( GenericBase::BASE_BUY, true );
		_theBase.at( i )->setState( GenericBase::BASE_SELL, true );
	}
}

void GenericPlayer::newWeek()
{
	for( uint i = 0; i < numBase(); i++ ) {
		_theBase.at( i )->initCreatureProduction();
	}
}

QString GenericPlayer::getAutoDescription()
{

	QString msg;
	msg = QObject::tr( "Player" ) + " " + getName(); 
	msg += "\n";
	msg += QObject::tr( "Connection name" ) + " " + getConnectionName() + "\n"; 
	for( int i = 0; i < _theBase.count() ; i++ ) {
		msg += "\n";
		msg += _theBase.at( i )->getBaseDescription();

	}

	int buildnum[DataTheme.buildings.count()];
	
	for( int i = 0; i < DataTheme.buildings.count() ; i++ ) {
		buildnum[i] = 0;
	}

	for( int i = 0; i < _theBuild.count() ; i++ ) {
		int type = _theBuild.at(i)->getType();
		buildnum[type]++;
	}

	msg += "\n";
	for( int i = 0; i < DataTheme.buildings.count() ; i++ ) {
		if( buildnum[ i ] > 0 ) {
			msg += QString::number( buildnum[i] ) + " ";
			msg += DataTheme.buildings.at(i)->getBuildAutoDescription() + "\n";
		}
	}
	return msg;

}

void GenericPlayer::addLord( GenericLord * lord )
{

	TRACE("GenericPlayer::addLord( id %d )", lord->getId() );

	if( ! hasLord( lord ) ) {
		_theLords.append( lord );
	}
}

void GenericPlayer::addBase( GenericBase * base )
{
	if( ! hasBase( base ) ) {
		_theBase.append( base );
	}
}


void GenericPlayer::addBuilding( GenericBuilding * b )
{
	if( ! hasBuilding( b ) ) {
		_theBuild.append( b );
	}
}

bool GenericPlayer::shouldEnd()
{
	for( uint i = 0; i < numLord(); i++ ) {
		if( ( ! _theLords.at( i )->isSleeping() ) && canMove( _theLords.at( i ) ) )
			return false;
	}
	
	return true;
}

void GenericPlayer::endTurn()
{
	// Resources, thanks to lords
	for( uint i = 0; i < numLord(); i++ ) {
		for( int j = 0; j < DataTheme.resources.count(); j++ ) {
			_ress->increaseValue( j, _theLords.at( i )->collectRessource( j ) );
		}
				
		_theLords.at(i)->endTurn();
	}
	
	// Resources, thanks to base
	for( uint i = 0; i < numBase(); i++ ) {
		for( int j = 0; j < DataTheme.resources.count(); j++ ) {
			_ress->increaseValue( j, _theBase.at( i )->collectRessource( j ));
		}
	}
	
	// Resources, thanks to mines
	//for( uint i = 0; i < numBuilding(); i++ ) {
		//for( int j = 0; j < DataTheme.resources.count(); j++ ) { 
		//	_ress[j] += _theBuild.at( i )->collectRessource( j );
		//}
	//}
}

bool GenericPlayer::selectFirstLord()
{
	for( uint i = 0; i < numLord(); i++ ) {
		if( _theLords.at( i )->isActive() ) {
			//_selectedLord = _theLords.at( i );
			setSelectedLord( _theLords.at( i ) );
			return true;
		}
	}
	return false;
}

GenericLord * GenericPlayer::nextLord()
{
	TRACE("GenericPlayer::nextLord");
	TRACE("SelectedLord %p, numLord %d", _selectedLord, numLord());

	if ( _selectedLord == 0 ) {
		if ( numLord() > 0 ) {
			_selectedLord = _theLords.at( 0 );
			return _theLords.at( 0 );
		} else
			return 0;
	} else {
		int num = _theLords.indexOf( _selectedLord );
		if ( num >= 0 ) {
			if ( (uint)num == numLord() - 1 ) {
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
	
	logEE("getLordById lord not found id %d numLord %d", id, numLord() );
	printLords();
	return 0;
}

void GenericPlayer::removeLord( int num )
{
	TRACE("GenericPlayer::removeLord( num %d )", num );
	if( _selectedLord == _theLords.at( num ) ) {
		_selectedLord = 0;
	}
	_theLords.removeAt( num );
}

void GenericPlayer::removeLord( GenericLord * lord )
{
	TRACE("GenericPlayer::removeLord( id %d )", lord->getId() );
	
	_theLords.removeAll( lord );
	if( _selectedLord == lord ) {
		_selectedLord = 0;
	}
}

GenericBase * GenericPlayer::nextBase()
{
	if ( _selectedBase == 0 ) {
		if ( numBase() > 0 ) {
			_selectedBase = _theBase.at( 0 );
			return _theBase.at( 0 );
		} else
			return 0;
	} else {
		int num = _theBase.indexOf( _selectedBase );
		if ( num >= 0 ) {
			if ( (uint)num == numBase() - 1 ) {
				_selectedBase = _theBase.at( 0 );
				return _theBase.at( 0 );
			} else {
				_selectedBase = _theBase.at( num+1 );
				return _theBase.at( num+1 );	
			}
		} else {
			if ( numBase() > 0 ) {
				_selectedBase = _theBase.at( 0 );
				return _theBase.at( 0 );
			} else {
				return 0;
			}
		}
	}
}


void GenericPlayer::removeBase( GenericBase * base )
{
	_theBase.removeAll( base );
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
	uint tmp, i;

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

	_alive = true;
	
	clearVision();
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

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( _ress->getValue(i) < (int) building->getCost( i ) ) {
			ret = false;
		}
	}

	return ret;
}

bool GenericPlayer::canBuy( Creature * creature, int number )
{
	bool ret = true;

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( _ress->getValue(i) < (int)(creature->getCost( i ) * number ) ) {
			ret = false;
		}
	}

	return ret;
}

bool GenericPlayer::canBuy( GenericLordModel * lord )
{
	bool ret = true;

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( _ress->getValue( i ) < (int)(lord->getCost( i ) ) ) {
			ret = false;
		}
	}

	return ret;
}

void GenericPlayer::buy( Creature * creature, int number )
{
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		ResourceModel * model = DataTheme.resources.get( i );
		if( !model->isPreservable() ){
			_ress->decreaseValue( i, creature->getCost( i ) * number );
		}
	}
}

uint GenericPlayer::computeBuyCreatureMax( Creature * creature )
{
	int ret = -1;

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( creature->getCost( i ) >  0 ) {
			int temp = _ress->getValue( i ) / creature->getCost( i );
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

void GenericPlayer::clearVision()
{
	if( _vision && _theight){
		for( int i = 0; i < _theight; ++i ) {
			delete [] _vision[i];
		}
		delete [] _vision;
		_vision = 0;
		_theight = 0;
	}
}

void GenericPlayer::initMapVision()
{
	if(!_vision){
		_theight = _map->getHeight();
		int width = _map->getWidth();
		_vision = new int * [ _theight ];
		for( int i = 0; i < _theight; i++ ) {
			_vision[i] = new int [ width ];
			for( int j = 0; j < width; j++ ) {
				_vision[i][j] = 0;
			}
		}
	}
}

QList<GenericCell *> GenericPlayer::removeCellVision( GenericCell * cell, int vision )
{
	QList<GenericCell *> ret;
	GenericCell * tmp = NULL;
	int i,j;
	
	GeneralOptions::VISION_MANAGEMENT visionType = DataTheme.general.getVisionManagement();
	QList<GenericCell *> list = _map->giveRadiusCell( cell , vision );

	if( visionType == GeneralOptions::VISION_REAL ) {
		for (int k = 0; k < list.size(); ++k) {
			tmp = list.at( k );
			i = tmp->getRow();
			j = tmp->getCol();
			if( _vision[i][j] > 0 ) {
				_vision[i][j]--;
				if( _vision[i][j] == 0 ) {
					ret.append( tmp );
				}
			}
		}
	}

	return ret;
}

QList<GenericCell *> GenericPlayer::addCellVision( GenericCell * cell, int vision )
{
	QList<GenericCell *> ret;
	GenericCell * tmp = NULL;
	int i, j;

	GeneralOptions::VISION_MANAGEMENT visionType = DataTheme.general.getVisionManagement();
	QList<GenericCell *> list = _map->giveRadiusCell( cell , vision );

	for (int k = 0; k < list.size(); ++k) {
		tmp = list.at( k );
		i = tmp->getRow();
		j = tmp->getCol();
		_vision[i][j]++;
		if( _vision[i][j] == 1 ) {
			ret.append( tmp );
		} else if( visionType == GeneralOptions::VISION_ONCE ) {
			_vision[i][j] = 2;
		}
	}

	return ret;
}

QList<GenericCell *> GenericPlayer::removeLordVision( GenericLord * lord )
{
	int vision = lord->getCharac( VISION );
	GenericCell * cell = lord->getCell();

	return removeCellVision( cell, vision );
}

QList<GenericCell *> GenericPlayer::addLordVision( GenericLord * lord )
{
	int vision = lord->getCharac( VISION );
	GenericCell * cell = lord->getCell();
	return addCellVision( cell, vision );
}

QList<GenericCell *> GenericPlayer::removeBaseVision( GenericBase * base )
{
	int vision = base->getVision();
	GenericCell * cell = base->getCell();

	return removeCellVision( cell, vision );
}

QList<GenericCell *> GenericPlayer::addBaseVision( GenericBase * base )
{
	int vision = base->getVision();
	GenericCell * cell = base->getCell();

	return addCellVision( cell, vision );
}

QList<GenericCell *> GenericPlayer::addBuildingVision( GenericBuilding * building )
{
	int vision = building->getVision();
	GenericCell * cell = building->getCell();

	return addCellVision( cell, vision );
}

QList<GenericCell *> GenericPlayer::removeBuildingVision( GenericBuilding * building )
{
	int vision = building->getVision();
	GenericCell * cell = building->getCell();

	return removeCellVision( cell, vision );
}

bool GenericPlayer::canSee( uint row, uint col )
{
	bool ret = false;

	if( _map->inMap( row, col ) ) {
		ret = ( _vision[row][col] > 0 );
	}

	return ret;
}

bool GenericPlayer::canSee( GenericCell * cell )
{
	return canSee( cell->getRow(), cell->getCol() );
}

bool GenericPlayer::canMove( GenericLord * lord )
{
	int movePt = lord->getCharac( MOVE );
	GenericCell * start = lord->getCell();
	QList<GenericCell *> list = _map->giveRadiusCell( lord->getCell() , 1 );
	
	for (int i = 0; i < list.size(); ++i) {
		if( list.at(i) == start ) {
			continue;
		}
		int cost = PathFinder::computeCostMvt( start, list.at(i) );
		//logDD("cost %d , movePt %d,i %d , row %d ,col %d", cost, movePt, i, list.at(i)->getRow(), list.at(i)->getCol());
		if ( cost !=0 && cost < movePt ) {
			return true;
		}
	}

	return false;
}

void GenericPlayer::printLords()
{
	logDD("lord count %d", _theLords.count() );	
	for(  int i = 0; i< _theLords.count() ; i++ ) {
		logDD("lords id %i", _theLords.at( i )->getId() );
	}
}
