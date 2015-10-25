/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalGameData.cpp
** Manage data (graphically) of a whole game
**
** Version : $Id: graphicalGameData.cpp,v 1.18 2008/06/26 23:31:15 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/01/2002
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

#include "graphicalGameData.h"
 
// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/questManager.h"

#include "libClient/attalSprite.h"
#include "libClient/bonus.h"
#include "libClient/building.h"
#include "libClient/chest.h"
#include "libClient/event.h"
#include "libClient/graphicalArtefact.h"
#include "libClient/lord.h"
#include "libClient/map.h"
#include "libClient/mapCreature.h"



Map * theMap = NULL;

GraphicalGameData::GraphicalGameData()
	:GameData()
{
}

GraphicalGameData::~GraphicalGameData()
{
	_animations.clear();
}

void GraphicalGameData::reinit()
{
	_animations.clear();
	
	while( ! _players.isEmpty() ) {
		delete _players.takeFirst();
	}

	GameData::reinit();
}

void GraphicalGameData::initLords()
{
	TRACE("GraphicalGameData::initLords");

	for( int i = 0; i < DataTheme.lords.count(); i++ ) {
		Lord * lord = new Lord( theMap, i );
		lord->setVisible( false );
		_lords.append( (GenericLord*)lord );
	}
}

void GraphicalGameData::loadMap( QTextStream * stream, int width, int height )
{
	_map = (GenericMap *)theMap;
	theMap->clear();
	theMap->load( stream, width, height );
}	

void GraphicalGameData::addPlayer()
{
	_players.append( new GenericPlayer( _map ) );
}

GenericBase * GraphicalGameData::getNewBase( uchar race )
{
	Base * base = new Base( theMap, race );
	_bases.append( (GenericBase*)base );
	base->setOwner( NULL );
	return (GenericBase*)base;
}
	
GenericBase * GraphicalGameData::getNewBase(	uchar race, int row,	int col, uchar id,	int nb,	const QList<uchar>  & forbidlist)
{
	GenericBase * base = GameData::getNewBase(	race, row, col, id, nb, forbidlist);
	dynamic_cast<Base*>(base)->show();
	return base;
}

GenericEvent * GraphicalGameData::getNewArtefact( int id )
{
	TRACE("GraphicalGameData::getNewArtefact");

	GenericEvent * ret = (GenericEvent*) new Event();
	GenericArtefact * artefact = (GenericArtefact*) new Artefact( theMap );
	if( id != -1 ) {
		artefact->setId( id );
	} else {
		artefact->setId( _nbArtefact );
		_nbArtefact++;
	}
	ret->setArtefact( artefact );
	_events.append( ret );
	return ret;
}

GenericEvent * GraphicalGameData::getNewBonus()
{
	TRACE("GraphicalGameData::getNewBonus");

	GenericEvent * ret = (GenericEvent*) new Event();
	GenericBonus * bonus = (GenericBonus*) new Bonus( theMap );
	_nbBonus++;
	ret->setBonus( bonus );
	_events.append( ret );
	return ret;
}

GenericEvent * GraphicalGameData::getNewChest()
{
	TRACE("GraphicalGameData::getNewChest");

	GenericEvent * ret = (GenericEvent*) new Event();
	GenericChest * chest = (GenericChest*) new Chest( theMap );
	_nbBonus++;
	ret->setChest( chest );
	_events.append( ret );
	return ret;
}

GenericBuilding * GraphicalGameData::getNewBuilding( uchar type )
{
	GenericBuilding * ret = (GenericBuilding *)( new Building( theMap, type ) );
	_buildings.append( ret );
	return ret;
}
	
GenericBuilding * GraphicalGameData::getNewBuilding( uchar type, int id, int row, int col )
{
	
	TRACE("GenericBuilding * GraphicalGameData::getNewBuilding( uchar type %d, int id %d, int row %d, int col %d )",type, id, row, col );

	Building * building = dynamic_cast<Building *> (GameData::getNewBuilding(	type, id, row, col ));
	if( building ) {
		appendAnimation(  building );
		building->show();
	}

	return building;
}

void GraphicalGameData::removeBuilding( GenericBuilding * build )
{
	removeAnimation( dynamic_cast<Building*>(build) );
	GameData::removeBuilding( build );
}

GenericMapCreature * GraphicalGameData::getNewMapCreature()
{
	GenericMapCreature * ret = (GenericMapCreature *)( new MapCreature( theMap ) );
	_creatures.append( ret );
	return ret;
}

GenericMapCreature * GraphicalGameData::getNewMapCreature( int row, int col, uchar race, uchar level, int nb, bool looking)
{
	MapCreature * creature  = dynamic_cast<MapCreature *>(GameData::getNewMapCreature( row, col, race, level, nb, looking));
	if( creature ) {
		creature->setLookingRight( looking );
	}
	return creature;
}

void GraphicalGameData::adjustPlayers()
{
	uint oldNbPlayer = _players.count();
	uint i;
	GenericPlayer * player;
	
	TRACE("GraphicalGameData::Adjust old %d, new %d ",oldNbPlayer, getNbPlayer());

	if( oldNbPlayer > getNbPlayer() ) {
		for( i = getNbPlayer(); i < oldNbPlayer; i++ ) {
			player =  _players.takeLast();
			resetOwnership( player );
			delete player;
		}
	} else if ( oldNbPlayer < getNbPlayer() ) {
		for( i = oldNbPlayer; i < getNbPlayer(); i++ ) {
		  player = new GenericPlayer( theMap );
			player->setNum( i );
			player->setName( QString("Player %1").arg(i) );
			_players.append( player );
		}
	}
}

void GraphicalGameData::appendAnimation( AttalSprite * item)
{
	if( item->animated() ) {
		_animations.append( item );
	}
}

void GraphicalGameData::removeAnimation( AttalSprite * item)
{
	_animations.removeAll( item );
}

void GraphicalGameData::advanceAnimations()
{
	int i;
	int count;

	count = _animations.count();

	for( i = 0 ; i < count ; i++ ) {
		_animations.at( i )->advance(1);
	}

}
