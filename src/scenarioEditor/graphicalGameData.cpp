/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalGameData.cpp
** Manage data (graphically) of a whole game
**
** Version : $Id: graphicalGameData.cpp,v 1.8 2002/11/01 23:10:29 audoux Exp $
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
// application specific include files
#include "common/genericPlayer.h"
#include "common/genericBase.h"
#include "common/artefact.h"
#include "common/dataTheme.h"
#include "common/log.h"
#include "client/building.h"
#include "client/graphicalArtefact.h"
#include "client/map.h"
#include "client/lord.h"

extern Map * theMap;
extern DataTheme DataTheme;

GraphicalGameData::GraphicalGameData()
	:GameData()
{
	reinit();
}

void GraphicalGameData::reinit()
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
		Lord * lord = new Lord( theMap );
		lord->setId( i );
		lord->setVisible( false );
		_lords.append( (GenericLord*)lord );
	}
}

void GraphicalGameData::loadMap( QTextStream * stream, int width, int height )
{
	_map = (GenericMap *)theMap;
	theMap->clearMap();
	theMap->load( stream, width, height );
}	

void GraphicalGameData::addPlayer()
{
	_players.append( new GenericPlayer( _map ) );
}

GenericBase * GraphicalGameData::getNewBase()
{
	Base * base = new Base( theMap );
	_bases.append( (GenericBase*)base );
	return (GenericBase*)base;
}

GenericArtefact * GraphicalGameData::getNewArtefact()
{
       GenericArtefact * ret = (GenericArtefact *)( new Artefact( theMap ) );
       ret->setId( _artefacts.count() );
       _artefacts.append( ret );
       return ret;
}

GenericBuilding * GraphicalGameData::getNewBuilding()
{
	GenericBuilding * ret = (GenericBuilding *)( new Building( theMap ) );
	ret->setId( _buildings.count() );
	_buildings.append( ret );
	return ret;
}



