/****************************************************************
**
** Attal : Lords of Doom
**
** gameData.cpp
** Manage data of a whole game
**
** Version : $Id: gameData.cpp,v 1.44 2014/06/22 19:27:31 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QTextStream>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/calendar.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericChest.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericMap.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/parser.h"
#include "libCommon/quest.h"
#include "libCommon/questManager.h"

extern QString VERSION;

GameData::GameData()
	:_nbPlayer( 0 ), _map( 0 ), _currPlayNum( 0 )
{
	try {
		_quests = new QuestManager();
		_calendar = new Calendar();
	} catch( std::bad_alloc &) {
		logEE("Memory error, game closed");
		return;
	}
	_nbArtefact = 0;
	_nbBonus = 0;
	_nbChest = 0;
	_player = 0;

	init();
}

GameData::~GameData()
{
	TRACE("GameData::~GameData");

	clearLists();

	if( _quests ) {
		delete _quests;
	}
	if( _calendar ) {
		delete _calendar;
	}
	if( _map ) {
		delete _map;
	}
	
	if( _player ) {
		delete _player;
	}
	
	TRACE("End of GameData::~GameData");
	
}

void GameData::clearLists()
{
	TRACE("GameData::clearLists");

	while( ! _bases.isEmpty() ) {
		delete _bases.takeFirst();
	}
	while( ! _buildings.isEmpty() ) {
		delete _buildings.takeFirst();
	}
	while( ! _lords.isEmpty() ) {
		delete _lords.takeFirst();
	}
	while( ! _events.isEmpty() ) {
		delete _events.takeFirst();
	}
	while( ! _creatures.isEmpty() ) {
		delete _creatures.takeFirst();
	}
}

void GameData::init()
{
	_nbPlayer = 0;
	_currPlayNum = 0;
	_nbArtefact = 0;
	_nbBonus = 0;
	_nbChest = 0;
	
	initLords();
}

void GameData::reinit()
{
	TRACE("GameData::reinit");

	if( _map ) {
		_map->clear();
	}
	
	_calendar->reinit();
	
	clearLists();
	
	_quests->clear();
 
 init();	
	
}

void GameData::initLords()
{
	TRACE("GameData::initLords");

	uint nbLords = DataTheme.lords.count();
	for( uint i = 0; i < nbLords; i++ ) {
		GenericLord * lord = new GenericLord( i );
		_lords.append( lord );
	}
}

bool GameData::load( const QString & filename )
{
	TRACE("bool GameData::load( const QString & filename %s)", qPrintable( filename ) );

	for( int i = 0; i < _players.count(); i++ ) {
		_players.at( i )->cleanData();
	}

	//reinit already in scenario parser 
	ScenarioParser handler( (GameData*)this );
	QFile file( filename );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	
	if( !ok ) {	
		logEE( "Parse Error (%s) : %s", qPrintable( filename ), qPrintable( handler.errorProtocol()) );
	}

	return ok;

}

void GameData::save( QTextStream * ts )
{
	*ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE scenario>" << endl;
	*ts << "<scenario nbPlayer=\"" << getNbPlayer() << "\">" << endl;
	*ts << "\t<attalversion>"  << VERSION << "</attalversion>" << endl;
	*ts << "\t<name>" << getScenarioName() << "</name>" << endl;
	*ts << "\t<theme>" << getScenarioTheme() << "</theme>" << endl;
	*ts << "\t<currentplayer>" << getCurrentPlayerNum() << "</currentplayer>" << endl;
	*ts << "\t<description>" << endl;
	*ts << "\t\t" << getScenarioDescription() << endl;
	*ts << "\t</description>" << endl;
	
	_calendar->save( ts, 1 );

	_map->save( ts );

	for( int i = 0; i < _events.count(); i++ ) {
		_events.at( i )->save( ts, 1 );
	}

	for( int i = 0; i < _bases.count(); i++ ) {
		_bases.at( i )->save( ts, 1 );
	}

	for( int i = 0; i < _buildings.count(); i++ ) {
		_buildings.at( i )->save( ts, 1 );
	}

	for( int i = 0; i < _lords.count(); i++ ) {
		_lords.at( i )->save( ts, 1 );
	}

	for( int i = 0; i < _creatures.count(); i++ ) {
		_creatures.at( i )->save( ts, 1 );
	}

	for( uint i = 0; i < getNbPlayer(); i++ ) {
		_players.at( i )->save( ts, 1 );
	}

	_quests->save( ts, 1 );

	*ts << "</scenario>" << endl;
}

void GameData::resetOwnership( GenericPlayer * player )
{
	for( int i = 0; i < _bases.count(); i++ ) {
		if(_bases.at( i )->getOwner() == player ) {
			_bases.at( i )->setOwner(0);
		}
	}

	for( int i = 0; i < _buildings.count(); i++ ) {
		if( _buildings.at( i )->getOwner() == player ) {
			_buildings.at( i )->setOwner(0);
		}
	}

	for( int i = 0; i < _lords.count(); i++ ) {
		if(_lords.at( i )->getOwner() == player ) {
			_lords.at( i )->setOwner(0);
		}
	}
}

GenericCell * GameData::getCell( uint row, uint col )
{
	GenericCell * cell;
	try {
		cell = _map->at( row, col );
		return cell;
	} catch ( const char * err ) {
		logEE("%s",err);
		return NULL;
	}
}

uint GameData::getBaseNumber()
{
	return _bases.count();
}

uint GameData::getBuildingNumber()
{
	return _buildings.count();
}

GenericBase * GameData::getBase( uint num )
{
	if( (int)num < _bases.count() ) {
		return _bases.at( num );
	}	
	
	return NULL;
}

void GameData::loadMap( QTextStream * stream, int width, int height )
{
	if( _map ) {
		delete _map;
	}
	_map = new GenericMap();
	_map->load( stream, width, height );
}

GenericBase * GameData::getNewBase( uchar race ) 
{
	GenericBase * base = new GenericBase( race );
	_bases.append( base );
	base->setOwner( NULL );
	return base;
}

GenericBase * GameData::getNewBase(	uchar race, int row,	int col, uchar id,	int nb, const QList<uchar> & forbidlist)
{
	GenericBase * base = NULL;

	try {
		base = _map->at( row, col )->getBase();


		if( base ) {
			removeBase( base );
		}

		base = getNewBase( race );
		base->setPosition( _map->at( row, col ));
		_map->computeStoppable( base );
		base->setId( id );
		base->setState( GenericBase::BASE_UNIT_BOUGHT, false );

		for( int i = 0; i < nb; i++ ) {
			base->addForbiddenBuilding( forbidlist.at(i) );
		}

		return base;

	} catch ( const char * err ) {
		logEE("%s", err);
		return NULL;
	}

}


void GameData::removeBase( GenericBase * base )
{
	if( base ) {
		if(base->getOwner()){
			base->getOwner()->removeBase(base);
		}
		_bases.removeAll( base );
		delete base;
	}
}

void GameData::setPlayerNumber( const uint & nb )
{
	//XXX very ugly function
	while( ! _players.isEmpty() ) {
		delete _players.takeFirst();
	}
	for( uint i = 0; i < nb; i++ ) {
		_players.append( new GenericPlayer( 0 ) );
	}
}

void GameData::setBaseId( GenericBase * base ) 
{
	int id = 0;

	for( int i = 0; i < _bases.count(); i++ ) {
		if(_bases.at( i )->getId() == id && _bases.at( i ) != base ) {
			id++;
			i = 0;
		}
	}
	base->setId( id );
}

void GameData::setBuildingId( GenericBuilding * build ) 
{
	int id = 0;

	for( int i = 0; i < _buildings.count(); i++ ) {
		if(_buildings.at( i )->getId() == id && _buildings.at( i ) != build ) {
			id++;
			i = 0;
		}
	}

	build->setId( id );
}

void GameData::setBase2Player( int idBase, int player ) 
{
	TRACE("GameData::setBase2Player idBase %d, player %d", idBase, player );

	if(  _players.count() > player  )  {
		GenericBase * base = getBaseById( idBase );
		
		if( base->getOwner() ) {
			base->getOwner()->removeBase( base );
		}

		if( ( _players.at( player ) ) && ( base )  ) {
			base->setOwner( _players.at( player ) );
			_players.at( player )->addBase( base );
		}
	}
}
	
void GameData::setBuilding2Player( int idBuild, int player )
{
	TRACE("GameData::setBuilding2Player idBuilding %d, player %d", idBuild, player );

	if( ( _players.count() > player  )) {
		GenericBuilding * build = getBuildingById( idBuild );
		
		if( build->getOwner() ) {
			build->getOwner()->removeBuilding( build );
		}

		if( _players.at( player ) ) {
			build->setOwner( _players.at( player ) );
			_players.at( player )->addBuilding( build );		
		}
	}
}

void GameData::setDateByType( int type, int value )
{
	_calendar->setDateByType(type,value);
}

void GameData::setInitialDateByType( int type, int value )
{
	_calendar->setInitialDateByType(type,value);
}

void GameData::setTurn( uint turn )
{
	_calendar->setTurn( turn );
}

uint GameData::getTurn()
{
	return _calendar->getTurn();
}

uint GameData::getDateByType( int type )
{
	return _calendar->getDateByType(type);
}

GenericLord * GameData::getLord( int idLord )
{
	return _lords.at( idLord );
}

uint GameData::getLordNumber()
{
	return _lords.count();
}

GenericPlayer * GameData::getPlayer( int num )
{
	if( ( num >= 0 ) && (num < _players.count() ) ) {
		return _players.at( num );
	} else {
		return NULL;
	}
}

uint GameData::getPlayerNumber()
{
	return _players.count();
}

GenericArtefact * GameData::getArtefactById( int id )
{
	GenericArtefact * ret = NULL;
	for( int i = 0; i < _events.count(); i++ ) {
		if( ( _events.at( i )->getType() == GenericEvent::EventArtefact ) &&
		  _events.at( i )->getArtefact()->getId() == id ) {
			ret = _events.at( i )->getArtefact();
		}
	}
	return ret;
}

GenericBase * GameData::getBaseById( int id )
{
	GenericBase * ret = NULL;
	for( int i = 0; i < _bases.count(); i++ ) {
		 if( _bases.at( i )->getId() == id ) {
			ret = _bases.at( i );
			break;
		}
	}
	
	return ret;
}

GenericBuilding * GameData::getBuildingById( int id )
{	
	GenericBuilding * ret = NULL;

	for( int i = 0; i < _buildings.count(); i++ ) {
		 if( _buildings.at( i )->getId() == id ) {
			ret = _buildings.at( i );
			break;
		}
	}
	return ret;	
}

bool GameData::setLord2Player( int idLord, int player )
{
	bool ret = false;
	if( ( _players.count() > player  ) && ( _lords.count() > idLord ) ) {
		if( ( _players.value( player ) ) && ( _lords.at( idLord ) ) ) {
			_lords.at( idLord )->setOwner( _players.at( player ) );
			_players.at( player )->addLord( _lords.at( idLord ) );		
			ret = true;
		} else {
			logEE("Error value %d, lords %d", _players.value( player ), _lords.at( idLord ) );
		}
	} else {
		logEE("Error out of range count %d, idLord %d", player , idLord );
	}
	return ret;
}

GenericEvent * GameData::getNewArtefact( int id )
{
	GenericEvent * ret = new GenericEvent();
	GenericArtefact * artefact = new GenericArtefact();
	artefact->setId( id );
	ret->setArtefact( artefact );
	_events.append( ret );
	return ret;
}

GenericEvent * GameData::getNewBonus()
{
	GenericEvent * ret = new GenericEvent();
	GenericBonus * bonus = new GenericBonus();
	ret->setBonus( bonus );
	_events.append( ret );
	return ret;

}

GenericEvent * GameData::getNewChest()
{
	GenericEvent * ret = new GenericEvent();
	GenericChest * chest = new GenericChest();
	ret->setChest( chest );
	_events.append( ret );
	return ret;

}

GenericBuilding * GameData::getNewBuilding( uchar type )
{
	GenericBuilding * ret = new GenericBuilding( type );
	_buildings.append( ret );
	return ret;
}

GenericBuilding * GameData::getNewBuilding( uchar type, int id, int row, int col)
{

	TRACE("GenericBuilding * GameData::getNewBuilding( uchar type %d, int id %d, int row %d, int col %d )",type, id, row, col );

	GenericBuilding * building = NULL;

	try {
		building = _map->at( row, col )->getBuilding();
	} catch ( const char * err ) {
		logEE("%s", err);
		return NULL;
	}

	if( building ) {
		removeBuilding( building );
	}

	building = getNewBuilding( type );
	/* keep id, position in that order **/
	building->setId( id );
	building->setPosition( _map->at( row, col ) );
	_map->computeStoppable( building );

	return building;	
}

void GameData::removeBuilding( GenericBuilding * build )
{
	if( build ) {
		if(build->getOwner()){
			build->getOwner()->removeBuilding( build );
		}
		_buildings.removeAll( build );
		delete build;
	}
}

Quest * GameData::getNewQuest()
{
	Quest * ret = new Quest();
	_quests->appendSecondaryQuest( ret );
	return ret;
}

Quest * GameData::getMainQuest()
{
	Quest * ret = _quests->getMainQuest();

	if( ! ret ) {
		ret = new Quest();
		_quests->setMainQuest( ret );
	}

	return ret;
}

GenericMapCreature * GameData::getNewMapCreature()
{
	GenericMapCreature * ret = new GenericMapCreature();
	_creatures.append( ret );
	return ret;
}

GenericMapCreature * GameData::getNewMapCreature( int row, int col, uchar race, uchar level, int nb, bool /* looking */)
{
	GenericCell * cell;
	try {
		cell = _map->at( row, col );
	} catch ( const char * err ) {
		logEE("%s",err);
		return NULL;
	}

	GenericMapCreature * creature = getNewMapCreature(); 
	creature->setCreature( race, level );
	creature->setCategoryNumber( nb );
	creature->setCell( cell );
	cell->setCreature( creature );
	return creature;

}

void GameData::removeMapCreature( GenericMapCreature * creature )
{
	if( creature ) {
		_creatures.removeAll( creature );
		delete creature;
	}
}

void GameData::removeMapCreature( int row, int col )
{
	try{
		GenericCell * cell = _map->at( row, col );
		GenericMapCreature * crea = cell->getCreature();
		if( crea ) {
			removeMapCreature( crea );
			cell->setCreature( NULL );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}

}

void GameData::removeEvent( int row, int col )
{
	try{
		GenericEvent * event = _map->at( row, col )->getEvent();
		if( event ) {
			removeEvent( event );
			_map->at( row, col )->setEvent( NULL );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void GameData::removeEvent( GenericEvent * event )
{
	if( event ) {
		_events.removeAll( event );
		delete event;
	}
}

GenericLord * GameData::modifLordNew( int row, int col, uchar id ) 
{
	GenericLord * lord = _lords.at( id );
	lord->setCell( _map->at( row, col ) );
	lord->setOwner( _player );

	_player->addLord( lord );
	return lord;
}
	
GenericLord * GameData::modifLordVisit( int row, int col, uchar id, uchar num, uchar present )
{	
	TRACE("GameData::socketModifLordVisit() num %d, row %d, col %d, id %d, present %d", num, row, col, id , present );

	GenericLord * lord = _lords.at( id );
	if( present == 1 ) {
		setLord2Player( id, num );
		lord->setCell( _map->at( row, col ) );
	} else {
		if( lord->getCell() ) {
			lord->getCell()->setLord(NULL);
		}
	}
	return lord;
}

void GameData::modifLordUnit(uchar id , uchar pos, uchar race, uchar level, int nb, uchar move, int health)
{

	TRACE("GameData::updateLordUnit id  %d, pos %d , race %d, level %d, nb %d, move %d, health %d", id,pos, race, level, nb, move, health );	
	
	if(_lords.at( id )) {
		_lords.at( id )->updateUnit( pos ,race, level,nb, move, health);
	}

}
	
void GameData::modifLordCharac( char id , char charac, int number ) 
{

	TRACE("GameData::socketModifLordCharac idLord %d, charac %s, nb %d", id, qPrintable(getCharacName( (LordCharac) charac)), number  );

	GenericLord * lord = _lords.at( id );
	if( lord ) {
		lord->setBaseCharac( (LordCharac) charac, number );
	} else {
		logEE("GameData::socketModifLordCharac error lord not exist idLord %d", id);
	}
}

void GameData::exchangeArtefactLord( uchar idLord1, uchar idLord2, int item)
{

	GenericLord * lord1 = NULL;
	GenericLord * lord2 = NULL;
	
	if( idLord1 ) {
		lord1 = _lords.at(  idLord1 );
	}
	if( idLord2 ) {
		lord2 = _lords.at(  idLord2 );
	}
	
	if( lord1 && lord2 ) {
		ArtefactManager * manag1 = lord1->getArtefactManager();
		ArtefactManager * manag2 = lord2->getArtefactManager();
		
		GenericLordArtefact * artefact = manag1->getArtefact( item );
		
		manag1->removeArtefact( item );
		manag2->addArtefact( artefact );
		
	}
}

void GameData::modifBuildingOwner( int row, int col,  int playNum )
{
	try {
		GenericBuilding * build = _map->at( row, col )->getBuilding();

		if(  playNum == _player->getNum() ) {
			if( build ) {
				_player->addBuilding( build );
				build->setOwner( _player );
			} else {
				logEE("no build");
			}
		} else {
			if( build ) {
				if(build->getOwner() == _player){
					_player->removeBuilding( build );
				}
				if( playNum == GenericPlayer::NO_PLAYER ) {
					build->setOwner( 0 );
				} else {
					build->setOwner( getPlayer(playNum) );
				}
			}
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}

}


/* SocketModifBase */

void GameData::modifBaseOwner( int row, int col,  int playNum )
{
	
	TRACE("GameData::modifBaseOwner row %d, col  %d, player num %d", row,col,playNum);

	try {
		GenericBase * base = _map->at( row, col )->getBase();
		if( base ) {
			if( playNum == _player->getNum() ) {
				_player->addBase( base );
				base->setOwner( _player );
			} else {
				/// XXX: improve management of base of other player
				if( base->getOwner() == _player ) {
					_player->removeBase( base );
				}
				base->setOwner( getPlayer( playNum ) );
			}
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void GameData::modifBaseUnit( int row, int col, uchar pos, uchar race , uchar level, int number )
{
	try {
		GenericBase * base = _map->at( row, col )->getBase();
		if( base ) {
			base->addUnit( pos, race, level, number );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void GameData::modifBaseProduction( int row, int col, uchar race , uchar level, int number )
{
	try {
		GenericBase * base = _map->at( row, col )->getBase();
		if( base ) {
			TRACE("base");
			Creature * creature = DataTheme.creatures.at( race, level );
			base->setCreatureProduction( creature, number );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}

}

void GameData::modifBaseBuilding( int row, int col, uchar level,	bool create )
{
	TRACE("GameData::modifBaseBuilding row %d, col %d, level %d, create %d",row,col,level,create);

	try{
		GenericBase * base =	_map->at( row, col )->getBase();
		if( base ) {
			base->updateBuilding( level, create );
		} else {
			logEE( "Base not found" );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}
	
void GameData::modifBaseState( int row, int col, int type,	bool state ) 
{
	TRACE("GameData::modifBaseState row %d, col %d, type %d, state %d",row,col,type,state);

	try{
		if( _map->at( row, col )->getBase() ) {
			GenericBase * base = (GenericBase *)_map->at( row, col )->getBase();
			base->setState( (GenericBase::BaseState) type, state );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}

}

void GameData::modifBaseName( int row, int col, const QString & name ) 
{
	
	TRACE("GameData::modifBaseName row %d, col %d, name %s",row,col,qPrintable(name));

	try {
		GenericBase * base = _map->at( row, col )->getBase();
		if( base ) {
			base->setName( name );
		}
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}
