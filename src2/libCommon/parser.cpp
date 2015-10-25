/****************************************************************
**
** Attal : Lords of Doom
**
** parser.cpp
** Parser for scenarii and saved games.
**
** Version : $Id: parser.cpp,v 1.17 2009/09/11 16:44:09 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 04/02/2001
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

#include "parser.h"

// generic include files
// include files for QT
#include <QBuffer>
#include <QTextStream>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/dataTheme.h"
#include "libCommon/gameData.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericChest.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/genericMap.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"

ScenarioParser::ScenarioParser( GameData * data )
{
	_data = data;
	_tempStream = 0;
}

ScenarioParser::~ScenarioParser()
{
	if( _tempStream ) {
		delete _tempStream;
	}
}

bool ScenarioParser::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_player = 0;
	_state = StateInit;
	//_conditionStack.setAutoDelete( false );
	_currentCondition = 0;
	return true;
}

bool ScenarioParser::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	bool ret = true;

	if( qName == "scenario" && _state == StateInit ) {
		_state = StateDocument;
		_data->reinit();
		_data->setNbPlayer( atts.value( "nbPlayer" ).toInt() );
	} else if( qName == "attalversion" && _state == StateDocument ) {
		_state = StateAttalVersion;
	} else if( qName == "name" && _state == StateDocument ) {
		_state = StateName;
	} else if( qName == "theme" && _state == StateDocument ) {
		_state = StateTheme;
	} else if( qName == "description" && _state == StateDocument ) {
		_state = StateDescription;
	} else if( qName == "currentplayer" && _state == StateDocument ) {
		_state = StateCurrentPlayer;
	} else if( qName == "calendar" && _state == StateDocument ) {
		_state = StateCalendar;
	} else if( qName == "value" && _state == StateCalendar ) {
		_state = StateCalendarDate;
		_number = atts.value( "type" ).toInt();
	} else if( qName == "startcalendar" && _state == StateCalendar ) {
		_state = StateCalendarStart;
	} else if( qName == "value" && _state == StateCalendarStart ) {
		_state = StateCalendarStartDate;
		_number = atts.value( "type" ).toInt();
	} else if( qName == "turn" && _state == StateCalendar ) {
		_state = StateCalendarTurn;
	} else if( qName == "map" && _state == StateDocument ) {
		_state = StateMap;
	} else if( qName == "width" && _state == StateMap ) {
		_state = StateWidth;
	} else if ( qName == "height" && _state == StateMap ) {
		_state = StateHeight;
	} else if( qName == "base" && _state == StateDocument ) {
		_state = StateBase;
		_baseState = BaseStateInit;
		_row = 0;
		_col = 0;
		_baseId = -1;
		_baseType = 0;
		_baseName = "";
		_base = _data->getNewBase();
	} else if( _state == StateBase ) {
		ret = startElementBase( namespaceURI, localName, qName, atts );
	} else if( qName == "building" && _state == StateDocument ) {
		_state = StateBuilding;
		_building = _data->getNewBuilding( atts.value( "type" ).toInt() );
		_buildId = -1;
		_row = 0;
		_col = 0;
	} else if( qName == "id" && _state == StateBuilding ) {
		_state = StateBuildingId;
	} else if( qName == "col" && _state == StateBuilding ) {
		_state = StateBuildingCol;
	} else if( qName == "row" && _state == StateBuilding ) {
		_state = StateBuildingRow;
	} else if( qName == "resource" && _state == StateBuilding ) {
		_state = StateBuildingResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "creature" && _state == StateDocument ) {
		_state = StateCreature;
		_creature = _data->getNewMapCreature();
		_row = 0;
		_col = 0;
		_race = 0;
		_level = 0;
		_number = 0;
		_behaviour = 2;
		_flee = false;
		_looking = false;
	} else if( qName == "col" && _state == StateCreature ) {
		_state = StateCreatureCol;
	} else if( qName == "row" && _state == StateCreature ) {
		_state = StateCreatureRow;
	} else if( qName == "race" && _state == StateCreature ) {
		_state = StateCreatureRace;
	} else if( qName == "level" && _state == StateCreature ) {
		_state = StateCreatureLevel;
	} else if( qName == "stack" && _state == StateCreature ) {
		_state = StateCreatureStack;
		_number = atts.value( "num" ).toInt();
	} else if( qName == "behaviour" && _state == StateCreature ) {
		_state = StateCreatureBehaviour;
	} else if( qName == "growth" && _state == StateCreature ) {
		_state = StateCreatureGrowth;
		_creature->setGrowthMode( (GenericMapCreature::GrowthMode)atts.value( "type" ).toInt() );
		_nbParam = 0;
	} else if( qName == "param" && _state == StateCreatureGrowth ) {
		_state = StateCreatureGrowthParam;
	} else if( qName == "flee" && _state == StateCreature ) {
		_state = StateCreatureFlee;
	} else if( qName == "lookingRight" && _state == StateCreature ) {
		_state = StateCreatureLooking;
	} else if( qName == "resource" && _state == StateCreature ) {
		_state = StateCreatureResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "lord" && _state == StateDocument ) {
		_state = StateLord;
		_lordState = LordStateInit;
		_row = 0;
		_col = 0;
		_posOk = false;
		_id = atts.value( "id" ).toInt();
		_lord = _data->getLord( _id );
		_lord->clearUnits();
	} else if( _state == StateLord ) {
		ret = startElementLord( namespaceURI, localName, qName, atts );
	} else if( qName == "player" && _state == StateDocument ) {
		_state = StatePlayer;
		_data->addPlayer();
		if(_data->getPlayer( _player )){
				_data->getPlayer( _player )->setNum( _player );
		}
	} else if( qName == "name" && _state == StatePlayer ) {
		_state = StatePlayerName;
	} else if( qName == "ruledbyai" && _state == StatePlayer ) {
		_state = StatePlayerRuled;
	} else if( qName == "team" && _state == StatePlayer ) {
		_state = StatePlayerTeam;
	} else if( qName == "vision" && _state == StatePlayer ) {
		_state = StatePlayerVision;
	} else if( qName == "base" && _state == StatePlayer ) {
		_state = StatePlayerBase;
	} else if( qName == "building" && _state == StatePlayer ) {
		_state = StatePlayerBuilding;
	} else if( qName == "lord" && _state == StatePlayer ) {
		_state = StatePlayerLord;
	} else if( qName == "resource" && _state == StatePlayer ) {
		_state = StatePlayerResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "quests" && _state == StateDocument ) {
		_state = StateQuest;
		_questState = QuestInit;
	} else if( qName == "event" && _state == StateDocument ) {
		_state = StateEvent;
		_eventState = EventInit;
		_row = 0;
		_col = 0;
		GenericEvent::EventType type = (GenericEvent::EventType)atts.value( "type" ).toInt();
		switch( type ) {
		case GenericEvent::EventArtefact:
			_event = _data->getNewArtefact();
			break;
		case GenericEvent::EventBonus:
			_event = _data->getNewBonus();
			break;
		case GenericEvent::EventChest:
			_event = _data->getNewChest();
			break;
		default:
			ret = false;
		}
	} else if( _state == StateEvent ) {
		ret = startElementEvent( namespaceURI, localName, qName, atts );
	} else if( _state == StateQuest ) {
		ret = startElementQuest( namespaceURI, localName, qName, atts );
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return ret;
}

bool ScenarioParser::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	bool ret = true;

	switch ( _state ) {
	case StateAttalVersion:
		_state = StateDocument;
		break;
	case StateName:
		_state = StateDocument;
		break;
	case StateTheme:
		_state = StateDocument;
		break;
	case StateDescription:
		_state = StateDocument;
		break;
	case StateCurrentPlayer:
		_state = StateDocument;
		break;
	case StateCalendar:
		_state = StateDocument;
		break;
	case StateCalendarDate:
		_state = StateCalendar;
		break;
	case StateCalendarStart:
		_state = StateCalendar;
		break;
	case StateCalendarStartDate:
		_state = StateCalendarStart;
		break;
	case StateCalendarTurn:
		_state = StateCalendar;
		break;
	case StateMap:
		_data->loadMap( _tempStream, _width, _height );
		_state = StateDocument;
		break;
	case StateWidth:
		_state = StateMap;
		break;
	case StateHeight:
		_state = StateMap;
		break;
	case StateBase:
		ret = endElementBase( namespaceURI, localName, qName );
		break;
	case StateBuilding:
		_building->setPosition( _data->getCell( _row, _col ) );
		if( _buildId < 0 ) {
			_building->setId( _data->getBuildingNumber()-1 );
		}
		_state = StateDocument;
		break;
	case StateBuildingId:
		_building->setId( _buildId );
		_state = StateBuilding;
		break;
	case StateBuildingCol:
		_state = StateBuilding;
		break;
	case StateBuildingRow:
		_state = StateBuilding;
		break;
	case StateBuildingResource:
		_state = StateBuilding;
		break;
	case StateCreature:
		_creature->setCreature( _race, _level );
		_creature->setCell( _data->getCell( _row, _col ) );
		_creature->setBehaviour( (GenericMapCreature::CreatureBehaviour)_behaviour );
		_creature->setFleeing( _flee );
		_creature->setLookingRight( _looking );
		_data->getCell( _row, _col )->setCreature( _creature );
		_state = StateDocument;
		break;
	case StateCreatureCol:
		_state = StateCreature;
		break;
	case StateCreatureRow:
		_state = StateCreature;
		break;
	case StateCreatureRace:
		_state = StateCreature;
		break;
	case StateCreatureLevel:
		_state = StateCreature;
		break;
	case StateCreatureStack:
		_state = StateCreature;
		break;
	case StateCreatureBehaviour:
		_state = StateCreature;
		break;
	case StateCreatureGrowth:
		_state = StateCreature;
		break;
	case StateCreatureGrowthParam:
		_state = StateCreatureGrowth;
		break;
	case StateCreatureFlee:
		_state = StateCreature;
		break;
	case StateCreatureLooking:
		_state = StateCreature;
		break;
	case StateCreatureResource:
		_state = StateCreature;
		break;
	case StateLord:
		ret = endElementLord( namespaceURI, localName, qName );
		break;
	case StatePlayer:
		_player++;
		_state = StateDocument;
		break;
	case StatePlayerName:
		_state = StatePlayer;
		break;
	case StatePlayerRuled:
		_state = StatePlayer;
		break;
	case StatePlayerTeam:
		_state = StatePlayer;
		break;
	case StatePlayerVision:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->loadVision( _tempStream , _width, _height );
		}
		_state = StatePlayer;
		break;
	case StatePlayerBase:
		_state = StatePlayer;
		break;
	case StatePlayerBuilding:
		_state = StatePlayer;
		break;
	case StatePlayerLord:
		_state = StatePlayer;
		break;
	case StatePlayerResource:
		_state = StatePlayer;
		break;
	case StateQuest:
		ret = endElementQuest( namespaceURI, localName, qName );
		break;
	case StateEvent:
		ret = endElementEvent( namespaceURI, localName, qName );
		break;
	default:
		break;
    }
    return ret;
}

bool ScenarioParser::characters( const QString& ch )
{
	bool ret = true;
	QString ch_simplified = ch.simplified();

	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateAttalVersion:
		_data->setAttalVersion( ch_simplified );
		break;
	case StateName:
		_data->setScenarioName( ch_simplified );
		break;
	case StateTheme:
		_data->setScenarioTheme( ch_simplified );
		break;
	case StateDescription:
		_data->setScenarioDescription( ch_simplified );
		break;
	case StateCurrentPlayer:
		_data->setCurrentPlayerNum( ch_simplified.toInt() );
		break;
	case StateCalendar:
		break;
	case StateCalendarDate:
		_data->setDateByType( _number, ch_simplified.toInt() );
		break;
	case StateCalendarStart:
		break;
	case StateCalendarStartDate:
		_data->setInitialDateByType( _number, ch_simplified.toInt() );
		break;
	case StateCalendarTurn:
		_data->setTurn( ch_simplified.toInt() );
		break;
	case StateMap:
		delete _tempStream;
		_tempStream = new QTextStream( ch_simplified.toLatin1() );
		break;
	case StateWidth:
		_width = ch_simplified.toInt();
		break;
	case StateHeight:
		_height = ch_simplified.toInt();
		break;
	case StateBase:
		ret = charactersBase( ch_simplified );
		break;
	case StateBuilding:
		break;
	case StateBuildingId:
		_buildId = ch_simplified.toInt();
		break;
	case StateBuildingCol:
		_col = ch_simplified.toInt();
		break;
	case StateBuildingRow:
		_row = ch_simplified.toInt();
		break;
	case StateBuildingResource:
		_building->getResourceList()->setValue( _res, ch_simplified.toInt() );
		break;
	case StateCreatureCol:
		_col = ch_simplified.toInt();
		break;
	case StateCreatureRow:
		_row = ch_simplified.toInt();
		break;
	case StateCreatureRace:
		_race = ch_simplified.toInt();
		break;
	case StateCreatureLevel:
		_level = ch_simplified.toInt();
		break;
	case StateCreatureStack:
		_creature->setStack( _number, ch_simplified.toInt() );
		break;
	case StateCreatureBehaviour:
		_behaviour = ch_simplified.toInt();
		break;
	case StateCreatureGrowth:
		_creature->setGrowthMode( (GenericMapCreature::GrowthMode)ch_simplified.toInt() );
		break;
	case StateCreatureGrowthParam:
		_creature->setGrowthParam( _nbParam, ch_simplified.toInt() );
		_nbParam++;
		break;
	case StateCreatureFlee:
		_flee = (bool) ch_simplified.toInt();
		break;
	case StateCreatureLooking:
		_looking = (bool) ch_simplified.toInt();
		break;
	case StateCreatureResource:
		_creature->getResourceList()->setValue( _res, ch_simplified.toInt() );
		break;
	case StatePlayerName:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->setName( ch_simplified );
		}
		break;
	case StatePlayerRuled:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->setRuledByAi( ch_simplified.toInt() );
		}
		break;
	case StatePlayerTeam:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->setTeam( ch_simplified.toInt() );
		}
		break;
	case StatePlayerBase:
		_data->setBase2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerVision:
		delete _tempStream;
		_tempStream = new QTextStream( ch_simplified.toLatin1() );
		break;
	case StatePlayerBuilding:
		_data->setBuilding2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerLord:
		_data->setLord2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerResource:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->getResourceList()->setValue( _res, ch_simplified.toInt() );
		}
		break;
	case StateLord:
		ret = charactersLord( ch_simplified );
		break;
	case StateQuest:
		ret = charactersQuest( ch_simplified );
		break;
	case StateEvent:
		ret = charactersEvent( ch_simplified );
		break;
	default:
		ret = false;
	}
	return ret;
}

// Parsing of Lords

bool ScenarioParser::startElementLord( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "col" && _lordState == LordStateInit ) {
		_lordState = LordStateCol;
	} else if( qName == "row" && _lordState == LordStateInit ) {
		_lordState = LordStateRow;
	} else if( qName == "characteristic" && _lordState == LordStateInit ) {
		_lordState = LordStateCharac;
		_charac = detectCharac( atts.value( "type" ) );
	} else if( qName == "unit" && _lordState == LordStateInit ) {
		_lordState = LordStateUnit;
		_pos = atts.value( "pos" ).toInt();
		_unit = new GenericFightUnit();
		_race = 0;
		_level = 0;
	} else if( qName == "race" && _lordState == LordStateUnit ) {
		_lordState = LordStateUnitRace;
	} else if( qName == "level" && _lordState == LordStateUnit ) {
		_lordState = LordStateUnitLevel;
	} else if( qName == "number" && _lordState == LordStateUnit ) {
		_lordState = LordStateUnitNumber;
	} else if( qName == "move" && _lordState == LordStateUnit ) {
		_lordState = LordStateUnitMove;
	} else if( qName == "health" && _lordState == LordStateUnit ) {
		_lordState = LordStateUnitHealth;
	} else if( qName == "artefact" && _lordState == LordStateInit ) {
		_lordState = LordStateArtefact;
		_pos = 0;
		_lordArtefact = new GenericLordArtefact();
		_lordArtefact->setType( atts.value( "type" ).toInt() );
	} else if( qName == "position" && _lordState == LordStateArtefact ) {
		_lordState = LordStateArtefactPosition;
	} else if( qName == "machine" && _lordState == LordStateInit ) {
		_lordState = LordStateMachine;
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return true;
}

bool ScenarioParser::endElementLord( const QString &, const QString &, const QString & )
{
	switch ( _lordState ) {
	case LordStateInit:
		_state = StateDocument;
		if( _posOk ) {
			_lord->setCell( _data->getCell( _row, _col ) );
		}
		break;
	case LordStateCol:
		_lordState = LordStateInit;
		_posOk = true;
		break;
	case LordStateRow:
		_lordState = LordStateInit;
		_posOk = true;
		break;
	case LordStateCharac:
		_lordState = LordStateInit;
		break;
	case LordStateUnit:
		_unit->setCreature( _race, _level );
		_lord->setUnit( _pos, _unit );
		_lordState = LordStateInit;
		break;
	case LordStateUnitRace:
		//_unit->setCreature( _type );
		_lordState = LordStateUnit;
		break;
	case LordStateUnitLevel:
		_lordState = LordStateUnit;
		break;
	case LordStateUnitNumber:
		_unit->setNumber( _number );
		_lordState = LordStateUnit;
		break;
	case LordStateUnitMove:
		_unit->setMove( _move );
		_lordState = LordStateUnit;
		break;
	case LordStateUnitHealth:
		_unit->setHealth( _health );
		_lordState = LordStateUnit;
		break;
	case LordStateArtefact:
		_lord->getArtefactManager()->addArtefact( _lordArtefact );
		_lordArtefact->setLord( _lord );
		_lordState = LordStateInit;
		break;
	case LordStateArtefactPosition:
		_lordState = LordStateArtefact;
		break;
	case LordStateMachine:
		_lordState = LordStateInit;
		_lord->addMachine( _machine );
		break;
	default:
		break;
	}
	return true;
}

bool ScenarioParser::charactersLord( const QString& ch )
{
	bool ret = true;

	switch( _lordState ) {
	case LordStateCol:
		_col = ch.toInt();
		break;
	case LordStateRow:
		_row = ch.toInt();
		break;
	case LordStateCharac:
		_lord->setBaseCharac( _charac, ch.toInt() );
		break;
	case LordStateUnitRace:
		_race = ch.toInt();
		break;
	case LordStateUnitLevel:
		_level = ch.toInt();
		break;
	case LordStateUnitNumber:
		_number = ch.toInt();
		break;
	case LordStateUnitMove:
		_move = ch.toInt();
		break;
	case LordStateUnitHealth:
		_health = ch.toInt();
		break;
	case LordStateArtefactPosition:
		_lordArtefact->setPosition( ch.toInt() );
		break;
	case LordStateMachine:
		_machine = ch.toInt();
		break;
	default:
		ret = false;
	}
	return ret;
}

// Parsing of Bases

bool ScenarioParser::startElementBase( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "type" && _baseState == BaseStateInit ) {
		_baseState = BaseStateType;
	} else if( qName == "name" && _baseState == BaseStateInit ) {
		_baseState = BaseStateName;
	} else if( qName == "id" && _baseState == BaseStateInit ) {
		_baseState = BaseStateId;
	} else if( qName == "col" && _baseState == BaseStateInit ) {
		_baseState = BaseStateCol;
	} else if( qName == "row" && _baseState == BaseStateInit ) {
		_baseState = BaseStateRow;		
	} else if( qName == "basestates" && _baseState == BaseStateInit ) {
		_baseState = BaseStateStates;
		_bstates = atts.value( "type" ).toInt();
	} else if( qName == "resource" && _baseState == BaseStateInit ) {
		_baseState = BaseStateResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "building" && _baseState == BaseStateInit ) {
		_baseState = BaseStateBuilding;
	} else if( qName == "forbidden" && _baseState == BaseStateInit ) {
		_baseState = BaseStateForbidden;
	} else if( qName == "unit" && _baseState == BaseStateInit ) {
		_baseState = BaseStateUnit;
		_pos = atts.value( "pos" ).toInt();
		_unit = new GenericFightUnit();
		_race = 0;
		_level = 0;
	} else if( qName == "race" && _baseState == BaseStateUnit ) {
		_baseState = BaseStateUnitRace;
	} else if( qName == "level" && _baseState == BaseStateUnit ) {
		_baseState = BaseStateUnitLevel;
	} else if( qName == "number" && _baseState == BaseStateUnit ) {
		_baseState = BaseStateUnitNumber;
	} else if( qName == "move" && _baseState == BaseStateUnit ) {
		_baseState = BaseStateUnitMove;
	} else if( qName == "health" && _baseState == BaseStateUnit ) {
		_baseState = BaseStateUnitHealth;
	} else if( qName == "available" && _baseState == BaseStateInit ) {
		_baseState = BaseStateProduction;
	} else if( qName == "unit" && _baseState == BaseStateProduction ) {
		_baseState = BaseStateProductionUnit;
		_race = 0;
		_level = 0;
		_number = 0;
	} else if( qName == "race" && _baseState == BaseStateProductionUnit ) {
		_baseState = BaseStateProductionUnitRace;
	} else if( qName == "level" && _baseState == BaseStateProductionUnit ) {
		_baseState = BaseStateProductionUnitLevel;
	} else if( qName == "number" && _baseState == BaseStateProductionUnit ) {
		_baseState = BaseStateProductionUnitNumber;
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return true;
}

bool ScenarioParser::endElementBase( const QString &, const QString &, const QString & )
{
	switch ( _baseState ) {	
	case BaseStateInit:
		_state = StateDocument;
		_base->setName( _baseName );
		_base->setPosition( _data->getCell( _row, _col ) );
		//used for compatibility with old scenarios
		if( _baseId < 0 ) {
			_base->setId( _data->getBaseNumber()-1 );
		}
		break;
	case BaseStateId:
		_base->setId( _baseId );
		_baseState = BaseStateInit;
		break;
	case BaseStateType:
		_baseState = BaseStateInit;
		break;
	case BaseStateName:
		_baseState = BaseStateInit;
		break;
	case BaseStateCol:
		_baseState = BaseStateInit;
		break;
	case BaseStateRow:
		_baseState = BaseStateInit;
		break;
	case BaseStateBuilding:
		{
			GenericInsideBuilding * building = new GenericInsideBuilding();
			building->setRace( _baseType );
			building->setLevel( _id );
			_base->addBuilding( building );
			_baseState = BaseStateInit;
		}
		break;
	case BaseStateForbidden:
		_baseState = BaseStateInit;
		break;
	case BaseStateUnit:
		_unit->setCreature( _race, _level );
		_base->setUnit( _pos, _unit );
		_baseState = BaseStateInit;
		break;
	case BaseStateUnitRace:
		//_unit->setCreature( _type );
		_baseState = BaseStateUnit;
		break;
	case BaseStateUnitLevel:
		_baseState = BaseStateUnit;
		break;
	case BaseStateUnitNumber:
		_unit->setNumber( _number );
		_baseState = BaseStateUnit;
		break;
	case BaseStateUnitMove:
		_unit->setMove( _move );
		_baseState = BaseStateUnit;
		break;
	case BaseStateUnitHealth:
		_unit->setHealth( _health );
		_baseState = BaseStateUnit;
		break;
	case BaseStateProductionUnitRace:
		_baseState = BaseStateProductionUnit;
		break;
	case BaseStateProductionUnitLevel:
		_baseState = BaseStateProductionUnit;
		break;
	case BaseStateProductionUnitNumber:
		_baseState = BaseStateProductionUnit;
		break;
	case BaseStateProductionUnit:
		_base->addCreature( _race, _level, _number );
		_baseState = BaseStateProduction;
		break;
	case BaseStateProduction:
		_baseState = BaseStateInit;
		break;
	case BaseStateResource:
		_baseState = BaseStateInit;
		break;
	case BaseStateStates:
		_baseState = BaseStateInit;
		break;
	default:
		break;
	}
	return true;
}

bool ScenarioParser::charactersBase( const QString& ch )
{
	bool ret = true;

	switch( _baseState ) {
	case BaseStateType:
		_baseType = ch.toInt();
		//XXX: workaround to race set in costructor
		_base->setRace( _baseType );
		break;
	case BaseStateId:
		_baseId = ch.toInt();
		break;
	case BaseStateName:
		_baseName = ch;
		break;
	case BaseStateCol:
		_col = ch.toInt();
		break;
	case BaseStateRow:
		_row = ch.toInt();
		break;
	case BaseStateBuilding:
		_id = ch.toInt();
		break;
	case BaseStateForbidden:
		_base->addForbiddenBuilding( ch.toInt() );
		break;
	case BaseStateUnitRace:
		_race = ch.toInt();
		break;
	case BaseStateUnitLevel:
		_level = ch.toInt();
		break;
	case BaseStateUnitNumber:
		_number = ch.toInt();
		break;
	case BaseStateUnitMove:
		_move = ch.toInt();
		break;
	case BaseStateUnitHealth:
		_health = ch.toInt();
		break;
	case BaseStateProductionUnitRace:
		_race = ch.toInt();
		break;
	case BaseStateProductionUnitLevel:
		_level = ch.toInt();
		break;
	case BaseStateProductionUnitNumber:
		_number = ch.toInt();
		break;
	case BaseStateStates:
		_base->setState( (GenericBase::BaseState) _bstates,  (bool) ch.toInt() );
		break;
	case BaseStateResource:
		_base->getResourceList()->setValue( _res, ch.toInt() );
		break;
	default:
		ret = false;
	}
	return ret;
}

// Parsing of Events

bool ScenarioParser::startElementEvent( const QString &, const QString &, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "row" && _eventState == EventInit ) {
		_eventState = EventRow;
	} else if( qName == "col" && _eventState == EventInit ) {
		_eventState = EventCol;
	} else if( qName == "artefact" && _eventState == EventInit ) {
		_eventState = EventArtefact;
		_event->getArtefact()->setType( atts.value( "type" ).toInt() );
	} else if( qName == "bonus" && _eventState == EventInit ) {
		_eventState = EventBonus;
		_event->getBonus()->setType( (GenericBonus::BonusType) atts.value( "type" ).toInt() );
	} else if( qName == "param" && _eventState == EventBonus ) {
		_eventState = EventBonusParam;
	} else if( qName == "chest" && _eventState == EventInit ) {
		_eventState = EventChest;
	} else if( qName == "param" && _eventState == EventChest ) {
		_eventState = EventChestParam;
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return true;
}

bool ScenarioParser::endElementEvent( const QString &, const QString &, const QString & )
{
	switch ( _eventState ) {
	case EventInit:
		_event->setCell( _data->getCell( _row, _col ) );
		_data->getCell( _row, _col )->setEvent( _event );
		_state = StateDocument;
		break;
	case EventRow:
		_eventState = EventInit;
		break;
	case EventCol:
		_eventState = EventInit;
		break;
	case EventArtefact:
		_eventState = EventInit;
		break;
	case EventBonus:
		_event->getBonus()->setupBonus();
		_eventState = EventInit;
		break;
	case EventBonusParam:
		_eventState = EventBonus;
		break;
	case EventChest:
		_eventState = EventInit;
		break;
	case EventChestParam:
		_eventState = EventChest;
		break;
	default:
		break;
	}
	return true;
}

bool ScenarioParser::charactersEvent( const QString& ch )
{
	bool ret = true;

	switch( _eventState ) {
	case EventRow:
		_row = ch.toInt();
		break;
	case EventCol:
		_col = ch.toInt();
		break;
	case EventBonusParam:
		_event->getBonus()->addParam( ch.toInt() );
		break;
	case EventChestParam:
		_event->getChest()->addParam( ch.toInt() );
		break;
	default:
		ret = false;
	}
	return ret;
}

// Parsing of Quests

bool ScenarioParser::startElementQuest( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "quest" && _questState == QuestInit ) {
		_questState = QuestQuest;
		if( atts.value( "type" ) == "main" ) {
			_quest = _data->getMainQuest();
		} else {
			_quest = _data->getNewQuest();
		}
	} else if( qName == "name" && _questState == QuestQuest ) {
		_questState = QuestName;
	} else if( qName == "description" && _questState == QuestQuest ) {
		_questState = QuestDescription;
	} else if( qName == "confirmation" && _questState == QuestQuest ) {
		_questState = QuestConfirmation;
	} else if( qName == "start" && _questState == QuestQuest ) {
		_questState = QuestStart;
	} else if( qName == "message" && ( _questState == QuestStart || _questState == QuestFail || _questState == QuestSuccess ) ) {
		_oldQuestState = _questState;
		_questState = QuestStartEndMessage;
	} else if( qName == "agree" && _questState == QuestStart ) {
		_questState = QuestStartAgree;
	} else if( qName == "disagree" && _questState == QuestStart ) {
		_questState = QuestStartDisagree;
	} else if( qName == "arg" && ( _questState == QuestStart || _questState == QuestFail || _questState == QuestSuccess ) ) {
		_oldQuestState = _questState;
		_questState = QuestStartEndArg;
	} else if( qName == "condition" && ( _questState == QuestStart || _questState == QuestFail || _questState == QuestSuccess ) ) {
		_oldQuestState = _questState;
		_questState = QuestDefCondition;
		_conditionStack.clear();
		_currentCondition = 0;
		manageCondition( atts.value( "type" ) );
	} else if( qName == "condition" && _questState == QuestDefCondition ) {
		manageCondition( atts.value( "type" ) );
	} else if( qName == "param" && _questState == QuestDefCondition ) {
		_questState = QuestConditionParam;
	} else if( qName == "fail" && _questState == QuestQuest ) {
		_questState = QuestFail;
	} else if( qName == "success" && _questState == QuestQuest ) {
		_questState = QuestSuccess;
	} else if( qName == "message" && _questState == QuestQuest ) {
		_questState = QuestMessage;
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return true;
}

void ScenarioParser::manageCondition( const QString & type )
{
	_params.clear();
	if( _currentCondition ) {
		_conditionStack.push( (QuestConditionComposite *)_currentCondition );
	}

	if( type == "composite" ) {
		_currentCondition = new QuestConditionComposite();
	} else if( type == "lord" ) {
		_currentCondition = new QuestConditionLord();
	} else if( type == "date" ) {
		_currentCondition = new QuestConditionDate();
	} else if( type == "player" ) {
		_currentCondition = new QuestConditionPlayer();
	}

	if( _conditionStack.isEmpty() ) {
		switch( _oldQuestState ) {
		case QuestStart:
			_quest->setStartCondition( _currentCondition );
			break;
		case QuestFail:
			_quest->setFailCondition( _currentCondition );
			break;
		case QuestSuccess:
			_quest->setSuccessCondition( _currentCondition );
			break;
		default:
			logEE( "Should not happen" );
		}
	} else {
		_conditionStack.top()->addCondition( _currentCondition );
	}
}

bool ScenarioParser::endElementQuest( const QString &, const QString &, const QString & )
{
	switch ( _questState ) {
	case QuestInit:
		_state = StateDocument;
		break;
	case QuestQuest:
		_questState = QuestInit;
		break;
	case QuestName:
		_questState = QuestQuest;
		break;
	case QuestDescription:
		_questState = QuestQuest;
		break;
	case QuestConfirmation:
		_questState = QuestQuest;
		break;
	case QuestStart:
		_questState = QuestQuest;
		break;
	case QuestStartEndMessage:
		_questState = _oldQuestState;
		break;
	case QuestStartAgree:
		_questState = QuestStart;
		break;
	case QuestStartDisagree:
		_questState = QuestStart;
		break;
	case QuestStartEndArg:
		_questState = _oldQuestState;
		break;
	case QuestDefCondition:
		if( _conditionStack.isEmpty() ) {
			_questState = _oldQuestState;
		} else {
			_currentCondition = _conditionStack.pop();
		}
		break;
	case QuestConditionParam:
		_questState = QuestDefCondition;
		break;
	case QuestFail:
		_questState = QuestQuest;
		break;
	case QuestSuccess:
		_questState = QuestQuest;
		break;
	case QuestMessage:
		_questState = QuestQuest;
		break;
	default:
		break;
	}
	return true;
}

bool ScenarioParser::charactersQuest( const QString& ch )
{
	bool ret = true;

	switch( _questState ) {
	case QuestName:
		_quest->setName( ch );
		break;
	case QuestDescription:
		_quest->setDescription( ch );
		break;
	case QuestConfirmation:
		_quest->setConfirmationNeeded( true );
		_quest->setConfirmationQuestion( ch );
		break;
	case QuestStartEndMessage:
		break;
	case QuestStartAgree:
		break;
	case QuestStartDisagree:
		break;
	case QuestStartEndArg:
		break;
	case QuestDefCondition:
		break;
	case QuestConditionParam:
		manageConditionParam( ch );
		break;
	case QuestQuest:
		break;
	case QuestFail:
		break;
	case QuestSuccess:
		break;
	case QuestMessage:
		break;
	default:
		ret = false;
	}
	return ret;
}

void ScenarioParser::manageConditionParam( const QString & param )
{
	switch( _currentCondition->getType() ) {
	case QuestCondition::COMPOSITE:
		if( param == "OR" ) {
			( ( QuestConditionComposite * ) _currentCondition )->setCompositeType( QuestConditionComposite::OR );
		} else {
			( ( QuestConditionComposite * ) _currentCondition )->setCompositeType( QuestConditionComposite::AND );
		}
		break;
	case QuestCondition::DATE:
		_params.append( param.toUInt() );
		switch( (QuestConditionDate::ConditionCategory) _params[ 0 ] ) {
			case QuestConditionDate::DATE:
				if(  _params.count() == 5 ) {
					( ( QuestConditionDate * ) _currentCondition )->setConditionDate( _params[1], _params[2], _params[3], _params[4] );
				}
				break;
			case QuestConditionDate::DELAY:
				if(  _params.count() == 2 )  {
					( ( QuestConditionDate * ) _currentCondition )->setConditionDelay( _params[ 1 ] );
				}
				break;
			default:
				break;
		}
		break;
	case QuestCondition::LORD:
		_params.append( param.toUInt() );
			switch( (QuestConditionLord::ConditionCategory) _params[ 0 ] ) {
			case QuestConditionLord::ARTEFACT:
				if(  _params.count() == 3 ) {
					( ( QuestConditionLord * ) _currentCondition )->setArtefactCondition( _params[ 1 ], (bool)_params[ 2 ] );
				}
				break;
			case QuestConditionLord::PRIMARY_SKILL:
				if(  _params.count() == 4 )  {
					( ( QuestConditionLord * ) _currentCondition )->setPrimarySkillCondition( (LordCharac)_params[1], (QuestCondition::CheckType)_params[2], _params[3] );
				}
				break;
			case QuestConditionLord::NB_CREATURES:
			 	if(  _params.count() == 3  ) {
					( ( QuestConditionLord * ) _currentCondition )->setNumberCondition( (QuestCondition::CheckType)_params[1], _params[2] );
				}
				break;
			default:
				break;
			}
		break;
	case QuestCondition::PLAYER:
		if( _params.count() == 0 ) {
			_params.append( param.toUInt() );
		} else {
			switch( (QuestConditionPlayer::ConditionCategory) _params[ 0 ] ) {
			case QuestConditionPlayer::NB_LORD:
				if( _params.count() == 1 ) {
					_params.append( param.toUInt() );
				} else {
					( ( QuestConditionPlayer * ) _currentCondition )->setNbBaseCondition( (QuestCondition::CheckType)_params[ 1 ], param.toUInt() );
				}
				break;
			case QuestConditionPlayer::NB_BASE:
				if( _params.count() == 1 ) {
					_params.append( param.toUInt() );
				} else {
					( ( QuestConditionPlayer * ) _currentCondition )->setNbBaseCondition( (QuestCondition::CheckType)_params[ 1 ], param.toUInt() );
				}
				break;
			case QuestConditionPlayer::BASENAME:
				( ( QuestConditionPlayer * ) _currentCondition )->setBasenameCondition( param );
				break;
			}
		}
		break;
	}
}

bool ScenarioParser::fatalError( const QXmlParseException& exception )
{
     logEE( "state %d, lordState %d, baseState %d, questState %d, eventState %d", _state, _lordState, _baseState, _questState, _eventState );

    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

