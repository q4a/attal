/****************************************************************
**
** Attal : Lords of Doom
**
** parser.cpp
** Parser for scenarii and saved games.
**
** Version : $Id: parser.cpp,v 1.34 2002/12/22 19:53:25 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qbuffer.h>
#include <qcstring.h>
// application specific include files
#include "common/log.h"
#include "common/unit.h"
#include "common/dataTheme.h"
#include "common/genericInsideBuilding.h"
#include "common/genericBuilding.h"
#include "common/genericEvent.h"
#include "common/genericPlayer.h"
#include "common/genericLord.h"
#include "common/genericMap.h"
#include "common/genericMapCreature.h"
#include "common/artefact.h"
#include "common/quest.h"
#include "server/gameData.h"

extern DataTheme DataTheme;

ScenarioParser::ScenarioParser( GameData * data )
{
	_data = data;
}

bool ScenarioParser::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_player = 0;
	_state = StateInit;
	return true;
}

bool ScenarioParser::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	bool ret = true;

	if( qName == "scenario" && _state == StateInit ) {
		_state = StateDocument;
		_data->reinit();
		_data->setNbPlayer( atts.value( "nbPlayer" ).toInt() );
	} else if( qName == "name" && _state == StateDocument ) {
		_state = StateName;
	} else if( qName == "theme" && _state == StateDocument ) {
		_state = StateTheme;
	} else if( qName == "description" && _state == StateDocument ) {
		_state = StateDescription;
	} else if( qName == "map" && _state == StateDocument ) {
		_state = StateMap;
		_buffer = new QByteArray();
		_tempStream = new QTextStream( *_buffer, IO_WriteOnly );
	} else if( qName == "width" && _state == StateMap ) {
		_state = StateWidth;
	} else if ( qName == "height" && _state == StateMap ) {
		_state = StateHeight;
	} else if( qName == "base" && _state == StateDocument ) {
		_state = StateBase;
		_row = 0;
		_col = 0;
		_baseType = 0;
		_base = _data->getNewBase();
	} else if( qName == "type" && _state == StateBase ) {
		_state = StateBaseType;
	} else if( qName == "col" && _state == StateBase ) {
		_state = StateBaseCol;
	} else if( qName == "row" && _state == StateBase ) {
		_state = StateBaseRow;		
	} else if( qName == "building" && _state == StateBase ) {
		_state = StateBaseBuilding;
	} else if( qName == "forbidden" && _state == StateBase ) {
		_state = StateBaseForbidden;
	} else if( qName == "building" && _state == StateDocument ) {
		_state = StateBuilding;
		_building = _data->getNewBuilding();
		_building->setType( atts.value( "type" ).toInt() );
		_row = 0;
		_col = 0;
	} else if( qName == "col" && _state == StateBuilding ) {
		_state = StateBuildingCol;
	} else if( qName == "row" && _state == StateBuilding ) {
		_state = StateBuildingRow;
	} else if( qName == "artefact" && _state == StateDocument ) {
		_state = StateArtefact;
		_artefact = _data->getNewArtefact();
		_row = 0;
		_col = 0;
		_artefact->setType( atts.value( "type" ).toInt() );
	} else if( qName == "col" && _state == StateArtefact ) {
		_state = StateArtefactCol;
	} else if( qName == "row" && _state == StateArtefact ) {
		_state = StateArtefactRow;
	} else if( qName == "creature" && _state == StateDocument ) {
		_state = StateCreature;
		_creature = new GenericMapCreature();
		_row = 0;
		_col = 0;
		_race = 0;
		_level = 0;
		_number = 0;
	} else if( qName == "col" && _state == StateCreature ) {
		_state = StateCreatureCol;
	} else if( qName == "row" && _state == StateCreature ) {
		_state = StateCreatureRow;
	} else if( qName == "race" && _state == StateCreature ) {
		_state = StateCreatureRace;
	} else if( qName == "level" && _state == StateCreature ) {
		_state = StateCreatureLevel;
	} else if( qName == "number" && _state == StateCreature ) {
		_state = StateCreatureNumber;
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
	} else if( qName == "name" && _state == StatePlayer ) {
		_state = StatePlayerName;
	} else if( qName == "base" && _state == StatePlayer ) {
		_state = StatePlayerBase;
	} else if( qName == "building" && _state == StatePlayer ) {
		_state = StatePlayerBuilding;
	} else if( qName == "lord" && _state == StatePlayer ) {
		_state = StatePlayerLord;
	} else if( qName == "resource" && _state == StatePlayer ) {
		_state = StatePlayerResource;
		_res = atts.value( "type" ).toInt();
	} else if( qName == "quest" && _state == StateDocument ) {
		_state = StateQuest;
		_questState = QuestInit;
	} else if( qName == "event" && _state == StateDocument ) {
		_state = StateEvent;
		_event = new GenericEvent();
		_row = 0;
		_col = 0;
		_event->setType( (GenericEvent::EventType)atts.value( "type" ).toInt() );
	} else if( qName == "param" && _state == StateEvent ) {
		_state = StateEventParam;
	} else if( qName == "col" && _state == StateEvent ) {
		_state = StateEventCol;
	} else if( qName == "row" && _state == StateEvent ) {
		_state = StateEventRow;
	} else if( _state == StateQuest ) {
		ret = startElementQuest( namespaceURI, localName, qName, atts );
	} else {
		// error
		return false;
	}
	return ret;
}

bool ScenarioParser::startElementLord( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "col" && _lordState == LordStateInit ) {
		_lordState = LordStateCol;
	} else if( qName == "row" && _lordState == LordStateInit ) {
		_lordState = LordStateRow;
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
		_artefact = _data->getNewArtefact();
		_artefact->setType( atts.value( "type" ).toInt() );
	} else if( qName == "position" && _lordState == LordStateArtefact ) {
		_lordState = LordStateArtefactPosition;
	} else if( qName == "machine" && _lordState == LordStateInit ) {
		_lordState = LordStateMachine;
	} else {
		return false;
	}
	return true;
}

bool ScenarioParser::startElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "start" && _questState == QuestInit ) {
		_questState = QuestStart;
	} else if( qName == "message" && ( _questState == QuestStart || _questState == QuestEnd ) ) {
		_oldQuestState = _questState;
		_questState = QuestStartEndMessage;
	} else if( qName == "agree" && _questState == QuestStart ) {
		_questState = QuestStartAgree;
	} else if( qName == "disagree" && _questState == QuestStart ) {
		_questState = QuestStartDisagree;
	} else if( qName == "arg" && ( _questState == QuestStart || _questState == QuestEnd ) ) {
		_oldQuestState = _questState;
		_questState = QuestStartEndArg;
	} else if( qName == "condition" && ( _questState == QuestStart || _questState == QuestEnd ) ) {
		_oldQuestState = _questState;
		_questState = QuestStartEndCondition;
	} else if( qName == "end" && _questState == QuestInit ) {
		_questState = QuestEnd;
	} else if( qName == "message" && _questState == QuestInit ) {
		_questState = QuestMessage;
	} else {
		return false;
	}
	return true;
}


bool ScenarioParser::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	bool ret = true;

	switch ( _state ) {
	case StateName:
		_state = StateDocument;
		break;
	case StateTheme:
		_state = StateDocument;
		break;
	case StateDescription:
		_state = StateDocument;
		break;
	case StateMap:
		delete _tempStream;
		_tempStream = new QTextStream( *_buffer, IO_ReadOnly );
		_data->loadMap( _tempStream, _width, _height );
		//		_data->_map = new GenericMap();
		//		_data->_map->load( _tempStream, _width, _height );
		delete _tempStream;
		delete _buffer;
		_state = StateDocument;
		break;
	case StateWidth:
		_state = StateMap;
		break;
	case StateHeight:
		_state = StateMap;
		break;
	case StateBase:
		_state = StateDocument;
		_base->setRace( _baseType );
		_base->setPosition( _data->getCell( _row, _col ) );
		_base->setId( _data->getBaseNumber() );
		break;
	case StateBaseType:
		_state = StateBase;
		break;
	case StateBaseCol:
		_state = StateBase;
		break;
	case StateBaseRow:
		_state = StateBase;
		break;
	case StateBaseBuilding:
		{
			GenericInsideBuilding * building = new GenericInsideBuilding();
			building->setRace( _base->getRace() );
			building->setLevel( _id );
			_base->addBuilding( building );
			_state = StateBase;
		}
		break;
	case StateBaseForbidden:
		_state = StateBase;
		break;
	case StateBuilding:
		_building->setPosition( _data->getCell( _row, _col ) );
		_state = StateDocument;
		break;
	case StateBuildingCol:
		_state = StateBuilding;
		break;
	case StateBuildingRow:
		_state = StateBuilding;
		break;
	case StateArtefact:
		_artefact->setCell( _data->getCell( _row, _col ) );
		_data->getCell( _row, _col )->setArtefact( _artefact );
		_state = StateDocument;
		break;
	case StateArtefactCol:
		_state = StateArtefact;
		break;
	case StateArtefactRow:
		_state = StateArtefact;
		break;
	case StateCreature:
		_creature->setCreature( _race, _level );
		_creature->setCell( _data->getCell( _row, _col ) );
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
	case StateCreatureNumber:
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
		_event->setCell( _data->getCell( _row, _col ) );
		_data->getCell( _row, _col )->setEvent( _event );
		_state = StateDocument;
		break;
	case StateEventParam:
		_state = StateEvent;
		break;
	case StateEventRow:
	case StateEventCol:
		_state = StateEvent;
		break;
	default:
		break;
    }
    return ret;
}

bool ScenarioParser::endElementLord( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _lordState ) {
	case LordStateInit:
		_state = StateDocument;
		if( _posOk ) {
			_lord->setPosition( _data->getCell( _row, _col ) );
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
		_lord->addArtefact( _artefact );
		_artefact->setLord( _lord );
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

bool ScenarioParser::endElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _questState ) {
	case QuestInit:
		_state = StateDocument;
		break;
	case QuestStart:
		_questState = QuestInit;
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
	case QuestStartEndCondition:
		_questState = _oldQuestState;
		break;
	case QuestEnd:
		_questState = QuestInit;
		break;
	case QuestMessage:
		_questState = QuestInit;
		break;
	default:
		break;
	}
	return true;
}

bool ScenarioParser::characters( const QString& ch )
{
	bool ret = true;
	QString ch_simplified = ch.simplifyWhiteSpace();

	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_data->setScenarioName( ch_simplified );
		break;
	case StateTheme:
		_data->setScenarioTheme( ch_simplified );
		break;
	case StateDescription:
		_data->setScenarioDescription( ch_simplified );
		break;
	case StateMap:
		*_tempStream << ch_simplified;
		break;
	case StateWidth:
		_width = ch_simplified.toInt();
		break;
	case StateHeight:
		_height = ch_simplified.toInt();
		break;
	case StateBase:
		break;
	case StateBuilding:
		break;
	case StateBuildingCol:
		_col = ch_simplified.toInt();
		break;
	case StateBuildingRow:
		_row = ch_simplified.toInt();
		break;
	case StateArtefactCol:
		_col = ch_simplified.toInt();
		break;
	case StateArtefactRow:
		_row = ch_simplified.toInt();
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
	case StateCreatureNumber:
		_number = ch_simplified.toInt();
		break;
	case StateBaseType:
		_baseType = ch_simplified.toInt();
		break;
	case StateBaseCol:
		_col = ch_simplified.toInt();
		break;
	case StateBaseRow:
		_row = ch_simplified.toInt();
		break;
	case StateBaseBuilding:
		_id = ch_simplified.toInt();
		break;
	case StateBaseForbidden:
		_base->addForbiddenBuilding( ch_simplified.toInt() );
		break;
	case StatePlayerName:
		break;
	case StatePlayerBase:
		_data->setBase2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerBuilding:
		_data->setBuilding2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerLord:
		_data->setLord2Player( ch_simplified.toInt(), _player );
		break;
	case StatePlayerResource:
		if( _data->getPlayer( _player ) ) {
			_data->getPlayer( _player )->setResource( _res, ch_simplified.toInt() );
		}
		break;
	case StateLord:
		ret = charactersLord( ch_simplified );
		break;
	case StateQuest:
		ret = charactersQuest( ch_simplified );
		break;
	case StateEventParam:
		_event->addParam( ch_simplified.toInt() );
		break;
	case StateEventCol:
		_col = ch_simplified.toInt();
		break;
	case StateEventRow:
		_row = ch_simplified.toInt();
		break;
	default:
		ret = false;
	}
	return ret;
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
		_artefact->setPosition( ch.toInt() );
		break;
	case LordStateMachine:
		_machine = ch.toInt();
		break;
	default:
		ret = false;
	}
	return ret;
}

bool ScenarioParser::charactersQuest( const QString& ch )
{
	bool ret = true;

	switch( _questState ) {
	case QuestStartEndMessage:
		break;
	case QuestStartAgree:
		break;
	case QuestStartDisagree:
		break;
	case QuestStartEndArg:
		break;
	case QuestStartEndCondition:
		break;
	case QuestMessage:
		break;
	default:
		ret = false;
	}
	return ret;
}

bool ScenarioParser::fatalError( const QXmlParseException& exception )
{
     logEE( "state %d, lordState %d", _state, _lordState );

    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

