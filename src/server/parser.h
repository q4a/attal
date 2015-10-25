/****************************************************************
**
** Attal : Lords of Doom
**
** parser.h
** Parser for scenarii and saved games.
**
** Version : $Id: parser.h,v 1.21 2002/12/21 21:06:47 audoux Exp $
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

 
#ifndef PARSER_H
#define PARSER_H
 
 
// generic include files
// include files for QT
#include <qxml.h>
#include <qtextstream.h>
// application specific include files
 
 
/*              ------------------------------
 *                         ScenarioParser
 *              ------------------------------ */

class GameData;
class GenericBase;
class GenericEvent;
class GenericLord;
class GenericBuilding;
class GenericFightUnit;
class GenericArtefact;
class GenericMapCreature;
class Quest;

/** comment for the class */
class ScenarioParser : public QXmlDefaultHandler
{
public:
	/** Constructor */
	ScenarioParser( GameData * data );
	
	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();
	
	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );
	
	bool startElementLord( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	bool startElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );
	
	bool endElementLord( const QString& namespaceURI, const QString& localName, const QString& qName );

	bool endElementQuest( const QString& namespaceURI, const QString& localName, const QString& qName );
	
	/** Define what to do of characters */
	bool characters( const QString& ch );
	
	bool charactersLord( const QString& ch );

	bool charactersQuest( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );

private:
	GameData * _data;
 	QTextStream * _tempStream;
	QByteArray * _buffer;
	QString _errorProt;
	GenericBase * _base;
	GenericBuilding * _building;
	GenericLord * _lord;
	GenericMapCreature * _creature;
	Quest * _quest;
	GenericFightUnit * _unit;
	GenericArtefact * _artefact;
	GenericEvent * _event;
	int _height, _width;
	int _id, _row, _col, _pos, _baseType;
	int _number, _move, _health;
	int _race, _level, _res;
	int _machine;
	QString _type;
	int _player;
	bool _posOk;

	enum State {
		StateInit,
		StateDocument,
		StateName,
		StateTheme,
		StateDescription,
		StateMap,
		StateWidth,
		StateHeight,
		StateBase,
		StateBaseType,
		StateBaseCol,
		StateBaseRow,
		StateBaseBuilding,
		StateBaseForbidden,
		StateArtefact,
		StateArtefactCol,
		StateArtefactRow,
		StateCreature,
		StateCreatureCol,
		StateCreatureRow,
		StateCreatureRace,
		StateCreatureLevel,
		StateCreatureNumber,
		StateBuilding,
		StateBuildingCol,
		StateBuildingRow,
		StateLord,
		StateEvent,
		StateEventParam,
		StateEventRow,
		StateEventCol,
		StateQuest,
		StatePlayer,
		StatePlayerName,
		StatePlayerBase,
		StatePlayerBuilding,
		StatePlayerLord,
		StatePlayerResource
	};

	enum LordState {
		LordStateInit,
		LordStateCol,
		LordStateRow,
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

	enum QuestState {
		QuestInit,
		QuestStart,
		QuestEnd,
		QuestStartEndMessage,
		QuestStartAgree,
		QuestStartDisagree,
		QuestStartEndArg,
		QuestStartEndCondition,
		QuestAction,
		QuestMessage
	};

	State _state;
	LordState _lordState;
	QuestState _questState, _oldQuestState;
	
	
	
};
 
#endif // PARSER_H   
