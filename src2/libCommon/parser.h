/****************************************************************
**
** Attal : Lords of Doom
**
** parser.h
** Parser for scenarii and saved games.
**
** Version : $Id: parser.h,v 1.10 2008/09/07 16:08:58 lusum Exp $
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

 
#ifndef PARSER_H
#define PARSER_H

 
// generic include files
// include files for QT
#include <QStack>
#include <QTextStream>
#include <QList>
#include <QXmlDefaultHandler>
// application specific include files
#include "libCommon/genericLord.h"
#include "libCommon/quest.h"

enum ScenarioState {
	StateInit,
	StateDocument,
	StateDocumentOther,
	StateAttalVersion,
	StateName,
	StateTheme,
	StateCurrentPlayer,
	StateDescription,
	StateCalendar,
	StateCalendarDate,
	StateCalendarStart,
	StateCalendarStartDate,
	StateCalendarTurn,
	StateMap,
	StateMapOther,
	StateWidth,
	StateHeight,
	StateBase,
	StateCreature,
	StateCreatureCol,
	StateCreatureRow,
	StateCreatureRace,
	StateCreatureLevel,
	StateCreatureStack,
	StateCreatureBehaviour,
	StateCreatureGrowth,
	StateCreatureGrowthParam,
	StateCreatureFlee,
	StateCreatureLooking,
	StateCreatureResource,
	StateBuilding,
	StateBuildingId,
	StateBuildingCol,
	StateBuildingRow,
	StateBuildingResource,
	StateLord,
	StateEvent,
	StateQuest,
	StatePlayer,
	StatePlayerName,
	StatePlayerVision,
	StatePlayerRuled,
	StatePlayerTeam,
	StatePlayerBase,
	StatePlayerBuilding,
	StatePlayerLord,
	StatePlayerResource,
	StateFinish
};
 
/*              ------------------------------
 *                         ScenarioParser
 *              ------------------------------ */

class GameData;
class Calendar;
class GenericArtefact;
class GenericBase;
class GenericBonus;
class GenericBuilding;
class GenericChest;
class GenericEvent;
class GenericFightUnit;
class GenericLord;
class GenericMapCreature;

/** comment for the class */
class ScenarioParser : public QXmlDefaultHandler
{
public:
	/** Constructor */
	ScenarioParser( GameData * data );

	/** Destructor */
	virtual ~ScenarioParser();
	
	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();
	
	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );
	
	bool startElementLord( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );
	
	bool startElementBase( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	bool startElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	bool startElementEvent( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );

	bool endElementLord( const QString& namespaceURI, const QString& localName, const QString& qName );
	
	bool endElementBase( const QString& namespaceURI, const QString& localName, const QString& qName );

	bool endElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName );

	bool endElementEvent( const QString& namespaceURI, const QString& localName, const QString& qName );
	
	/** Define what to do of characters */
	bool characters( const QString& ch );
	
	bool charactersLord( const QString& ch );
	
	bool charactersBase( const QString& ch );

	bool charactersQuest( const QString& ch );

	bool charactersEvent( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );

protected:
	void manageCondition( const QString & type );

	void manageConditionParam( const QString & param );

private:
	GameData * _data;
 	QTextStream * _tempStream;
	QString _errorProt;
	Calendar * _calendar;
	GenericBase * _base;
	GenericBuilding * _building;
	GenericLord * _lord;
	GenericMapCreature * _creature;
	Quest * _quest;
	GenericFightUnit * _unit;
	GenericEvent * _event;
	GenericArtefact * _artefact;
	GenericLordArtefact * _lordArtefact;
	QuestCondition * _currentCondition;
	QStack<QuestConditionComposite *> _conditionStack;
	QList<uint> _params;
	LordCharac _charac;
	int _height, _width;
	int _id, _row, _col, _pos, _baseType, _baseId, _buildId;
	int _number, _move, _health;
	int _behaviour;
	bool _flee, _looking;
	int _race, _level, _res, _bstates;
	int _machine;
	QString _type, _baseName;
	int _player;
	bool _posOk;
	uint _nbParam;
	
	enum LordState {
		LordStateInit,
		LordStateCol,
		LordStateRow,
		LordStateCharac,
		LordStateUnit,
		LordStateUnitRace,
		LordStateUnitLevel,
		LordStateUnitNumber,
		LordStateUnitMove,
		LordStateUnitHealth,
		LordStateArtefact,
		LordStateArtefactPosition,
		LordStateMachine
	};

	enum BaseState {
		BaseStateInit,
		BaseStateId,
		BaseStateType,
		BaseStateName,
		BaseStateStates,
		BaseStateResource,
		BaseStateBuilding,
		BaseStateForbidden,
		BaseStateCol,
		BaseStateRow,
		BaseStateUnit,
		BaseStateUnitRace,
		BaseStateUnitLevel,
		BaseStateUnitNumber,
		BaseStateUnitMove,
		BaseStateUnitHealth,
		BaseStateProduction,
		BaseStateProductionUnit,
		BaseStateProductionUnitRace,
		BaseStateProductionUnitLevel,
		BaseStateProductionUnitNumber
	};

	enum QuestState {
		QuestInit,
		QuestQuest,
		QuestName,
		QuestConfirmation,
		QuestDescription,
		QuestStart,
		QuestFail,
		QuestSuccess,
		QuestStartEndMessage,
		QuestStartAgree,
		QuestStartDisagree,
		QuestStartEndArg,
		QuestDefCondition,
		QuestConditionParam,
		QuestAction,
		QuestMessage
	};

	enum EventState {
		EventInit,
		EventRow,
		EventCol,
		EventArtefact,
		EventBonus,
		EventBonusParam,
		EventChest,
		EventChestParam
	};

	ScenarioState _state;
	LordState _lordState;
	BaseState _baseState;
	QuestState _questState, _oldQuestState;
	EventState _eventState;



};

#endif // PARSER_H
