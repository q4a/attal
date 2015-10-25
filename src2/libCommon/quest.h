/****************************************************************
**
** Attal : Lords of Doom
**
** quest.h
** Manages quests in Attal
**
** Version : $Id: quest.h,v 1.16 2007/04/06 12:55:06 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 02/04/2002
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


#ifndef QUEST_H
#define QUEST_H


// generic include files
// include files for QT
#include <QList>
#include <QObject>
#include <QString>

// application specific include files
#include "libCommon/define.h"

class QTextStream;

class GenericLord;
class GenericPlayer;
class QuestCondition;


/*              ------------------------------
 *                         Quest
 *              ------------------------------ */


class Quest : public QObject
{
	Q_OBJECT
public:
	/** Constructor */
	Quest();
	
	~Quest();

	enum QuestType {
		ONLY_ONE,
		ONE_PERPLAYER,
		ONE_PERLORD,
		ANYONE
	};

	QuestType getType() {
		return _type;
	}

	static QString getTypeName( QuestType type );

	void setType( QuestType  type ) {
		_type = type;
	}

	const QString & getName() {
		return _name;
	}

	void setName( const QString & name ) {
		_name = name;
	}

	const QString & getDescription() {
		return _description;
	}

	void setDescription( const QString & description ) {
		_description = description;
	}

	/** for saving quest in scenario file */
	void save( QTextStream * ts, int indent );

	QuestCondition * getStartCondition() { return _startCondition; }

	void setStartCondition( QuestCondition * condition ) { _startCondition = condition; }

	QuestCondition * getFailCondition() { return _failCondition; }

	void setFailCondition( QuestCondition * condition ) { _failCondition = condition; }

	QuestCondition * getSuccessCondition() { return _successCondition; }

	void setSuccessCondition( QuestCondition * condition ) { _successCondition = condition; }

	void setConfirmationQuestion( const QString & confirmation ) { _confirmationQuestion = confirmation; }

	const QString & getConfirmationQuestion() { return _confirmationQuestion; }

	void setAcceptMessage( const QString & msg ) { _acceptMsg = msg; }

	const QString & getAcceptMessage() { return _acceptMsg; }

	void setRejectMessage( const QString & msg ) { _rejectMsg = msg; }

	const QString & getRejectMessage() { return _rejectMsg; }

	bool isConfirmationNeeded() { return _needConfirmation; }

	void setConfirmationNeeded( bool state ) { _needConfirmation = state; }

protected:
	QuestType _type;
	QString _name;
	QString _description;
	bool _needConfirmation;
	QString _confirmationQuestion;
	QString _acceptMsg, _rejectMsg;
	QuestCondition * _startCondition;
	QuestCondition * _failCondition;
	QuestCondition * _successCondition;
};

class QuestData
{
public:
	/** Constructor */
	QuestData();

	GenericLord * getLord() {
		return _lord;
	}

	void setLord( GenericLord * lord ) {
		_lord = lord;
	}

	GenericPlayer * getPlayer() {
		return _player;
	}

	void setPlayer( GenericPlayer * player ) {
		_player = player;
	}
	
	void setCurrentAbsoluteDate( uint date ) {
		_currentDate = date;
	}
	
	void setCurrentTurn( uint turn ) {
		_currentTurn = turn;
	}
	
	uint getCurrentAbsoluteDate() { return _currentDate; }
	
	uint getCurrentTurn() { return _currentTurn; }
	
protected:
	GenericLord * _lord;
	GenericPlayer * _player;
	uint _currentDate, _currentTurn;
};

class QuestCondition : public QObject
{
public:
	enum ConditionType {
		COMPOSITE,
		DATE,
		LORD,
		PLAYER
	};

	enum CheckType {
		INFERIOR,
		INFERIOR_EQUAL,
		EQUAL,
		SUPERIOR_EQUAL,
		SUPERIOR
	};

	/** Constructor */
	QuestCondition() {}

	/** Destructor */
	virtual ~QuestCondition() = 0;

	/** for saving quest condition in scenario file */
	virtual void save( QTextStream * ts, int indent ) = 0;

	virtual void clear();

	virtual bool check( QuestData * data ) = 0;

	virtual QString getLabel() = 0;

	ConditionType getType() {
		return _type;
	}

protected:
	ConditionType _type;
};

class QuestConditionComposite : public QuestCondition
{
	Q_OBJECT
public:
	enum CompositeType {
		AND,
		OR
	};

	/** Constructor */
	QuestConditionComposite( CompositeType type = AND );

	virtual ~QuestConditionComposite();

	virtual void clear();

	CompositeType getCompositeType() {
		return _compositeType;
	}

	void setCompositeType( CompositeType type ) {
		_compositeType = type;
	}

	void addCondition( QuestCondition * condition );

	uint getConditionNumber();

	QuestCondition * getCondition( uint nb );

	void delCondition( uint nb );

	void delCondition( QuestCondition * condition );

	/** for saving quest condition in scenario file */
	void save( QTextStream * ts, int indent );

	bool check( QuestData * data );

	QString getLabel();

protected:
	CompositeType _compositeType;
	QList<QuestCondition *> _list;
};

class QuestConditionDate : public QuestCondition
{
	Q_OBJECT
public:
	enum ConditionCategory {
		DATE,
		DELAY
	};

	/** Constructor */
	QuestConditionDate();

	/** for saving quest condition in scenario file */
	void save( QTextStream * ts, int indent );

	bool check( QuestData * data );

	QString getLabel();

	void setConditionDate( uint day, uint week, uint month, uint year );
	
	void setConditionDelay( uint nbDay );

	ConditionCategory getCategory() { return _category; }

	uint getDay() { return _day; }

	uint getWeek() { return _week; }

	uint getMonth() { return _month; }
	
	uint getYear() { return _year; }

	uint getDelay() { return _nbDay; }

protected:
	ConditionCategory _category;
	uint _day, _week, _month, _year;
	uint _nbDay;
};

class QuestConditionLord : public QuestCondition
{
	Q_OBJECT
public:
	enum ConditionCategory {
		PRIMARY_SKILL,
		SECONDARY_SKILL,
		ARTEFACT,
		SPELL,
		NB_CREATURES
	};

	/** Constructor */
	QuestConditionLord();

	/** for saving quest condition in scenario file */
	void save( QTextStream * ts, int indent );

	bool check( QuestData * data );

	QString getLabel();

	void setPrimarySkillCondition( LordCharac charac, CheckType check, uint value );

	LordCharac getLordCharac() { return _charac; }

	CheckType getCheckType() { return _check; }

	uint getValue() { return _value; }
	
	void setNumberCondition( CheckType check, uint value );

	void setSecondarySkillCondition( uint secondarySkill, bool present );

	uint getSecondarySkill() { return _secondary; }

	bool isSecondarySkillPresent() { return _present; }

	void setArtefactCondition( uint artefact, bool present );

	uint getArtefact() { return _artefact; }

	bool isArtefactPresent() { return _present; }

	void setSpellCondition( uint spell, bool present );

	uint getSpell() { return _spell; }

	bool isSpellPresent() { return _present; }

	ConditionCategory getConditionType() { return _category; }

protected:
	void savePrimary( QTextStream * ts, int indent );
	void saveArtefact( QTextStream * ts, int indent );
	void saveNumber( QTextStream * ts, int indent );

	bool checkPrimary( QuestData * data );

	ConditionCategory _category;
	LordCharac _charac;
	CheckType _check;
	bool _present;
	uint _value, _secondary, _artefact, _spell;
};

class QuestConditionPlayer : public QuestCondition
{
	Q_OBJECT
public:
	enum ConditionCategory {
		NB_LORD,
		NB_BASE,
		BASENAME
	};

	/** Constructor */
	QuestConditionPlayer();

	/** for saving quest condition in scenario file */
	void save( QTextStream * ts, int indent );

	bool check( QuestData * data );

	QString getLabel();

	void setNbLordCondition( CheckType check, uint value );
	
	void setNbBaseCondition( CheckType check, uint value );
	
	void setBasenameCondition( const QString & name );
	
	uint getValue() { return _value; }
	
	CheckType getCheckType() { return _check; }
	
	const QString & getName() { return _name; }
	
	ConditionCategory getConditionType() { return _category; }

protected:
	ConditionCategory _category;
	CheckType _check;
	uint _value;
	QString _name;
};



#endif // QUEST_H
