/****************************************************************
**
** Attal : Lords of Doom
**
** gameData.cpp
** Manage data of a whole game
**
** Version : $Id: gameData.cpp,v 1.12 2002/09/26 21:43:52 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "gameData.h"

// generic include files
// include files for QT
#include <qtextstream.h>
// application specific include files
#include "common/genericMap.h"
#include "common/genericLord.h"
#include "common/genericPlayer.h"
#include "common/genericBase.h"
#include "common/genericBuilding.h"
#include "common/artefact.h"
#include "common/dataTheme.h"
#include "common/quest.h"
#include "server/parser.h"
#include "server/questManager.h"

extern DataTheme DataTheme;

/** add comments here */
GameData::GameData()
	:_nbPlayer( 0 ), _map( 0 )
{
	_players.setAutoDelete( true );
	_bases.setAutoDelete( true );
	_buildings.setAutoDelete( true );
	_lords.setAutoDelete( true );
	_artefacts.setAutoDelete( true );
	_quests = new QuestManager();

	reinit();
}

GameData::~GameData()
{
	if( _quests ) {
		delete _quests;
	}
}

void GameData::save( QTextStream * ts )
{
	uint i;

	*ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE scenario>" << endl;
	*ts << "<scenario nbPlayer=\"" << getNbPlayer() << "\">" << endl;
	*ts << "\t<name>" << getScenarioName() << "</name>" << endl;
	*ts << "\t<theme>" << getScenarioTheme() << "</theme>" << endl;
	*ts << "\t<description>" << endl;
	*ts << "\t\t" << getScenarioDescription() << endl;
	*ts << "\t</description>" << endl;

	_map->save( ts );

	for( i = 0; i < _artefacts.count(); i++ ) {
		if( _artefacts.at( i )->isOnMap() ) {
			_artefacts.at( i )->save( ts, 1 );
		}
	}

	for( i = 0; i < _bases.count(); i++ ) {
		_bases.at( i )->save( ts, 1 );
	}

	for( i = 0; i < _buildings.count(); i++ ) {
		_buildings.at( i )->save( ts, 1 );
	}

	for( i = 0; i < _lords.count(); i++ ) {
		_lords.at( i )->save( ts, 1 );
	}

	for( i = 0; i < getNbPlayer(); i++ ) {
		_players.at( i )->save( ts, 1 );
	}

	*ts << "</scenario>" << endl;
}

void GameData::reinit()
{
	if( _map ) {
		_map->clear();
	}
	_bases.clear();
	_buildings.clear();
	_nbPlayer = 0;
	_lords.clear();
	_artefacts.clear();
	for( uint i = 0; i < DataTheme.lords.count(); i++ ) {
		GenericLord * lord = new GenericLord();
		lord->setId( i );
		_lords.append( lord );
	}
}

GenericCell * GameData::getCell( int row, int col )
{
	return _map->at( row, col );
}

uint GameData::getBaseNumber()
{
	return _bases.count();
}

void GameData::loadMap( QTextStream * stream, int width, int height )
{
	if( _map ) {
		delete _map;
	}
	_map = new GenericMap();
	_map->load( stream, width, height );
}

GenericBase * GameData::getNewBase() 
{
	GenericBase * base = new GenericBase();
	_bases.append( base );
	return base;
}
	
void GameData::setBase2Player( int idBase, int player ) 
{
	if( ( _players.at( player ) ) && ( _bases.at( idBase ) ) ) {
		_bases.at( idBase )->setOwner( _players.at( player ) );
		_players.at( player )->addBase( _bases.at( idBase ) );
	}
}
	
void GameData::setBuilding2Player( int idBuild, int player )
{
	if( ( _players.at( player ) ) && ( _buildings.at( idBuild ) ) ) {
		_buildings.at( idBuild )->setOwner( _players.at( player ) );
		_players.at( player )->addBuilding( _buildings.at( idBuild ) );		
	}
}

GenericLord * GameData::getLord( int idLord )
{
	return _lords.at( idLord );
}

GenericPlayer * GameData::getPlayer( int num )
{
	return _players.at( num );
}

void GameData::setLord2Player( int idLord, int player )
{
	if( ( _players.at( player ) ) && ( _lords.at( idLord ) ) ) {
		_lords.at( idLord )->setOwner( _players.at( player ) );
		_players.at( player )->addLord( _lords.at( idLord ) );		
	}
}

GenericArtefact * GameData::getNewArtefact()
{
	GenericArtefact * ret = new GenericArtefact();
	ret->setId( _artefacts.count() );
	_artefacts.append( ret );
	return ret;
}

GenericBuilding * GameData::getNewBuilding()
{
	GenericBuilding * ret = new GenericBuilding();
	ret->setId( _buildings.count() );
	_buildings.append( ret );
	return ret;
}

Quest * GameData::getNewQuest()
{
	Quest * ret = new Quest();
	_quests->append( ret );
	return ret;
}

