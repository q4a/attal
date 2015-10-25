/****************************************************************
**
** Attal : Lords of Doom
**
** creature.h
** Manages creatures
**
** Version : $Id: creature.h,v 1.5 2003/04/24 20:39:45 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/09/2002
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


#ifndef CREATURE_H
#define CREATURE_H

// generic include files
// include files for QT
#include <qlist.h>
#include <qstring.h>
#include <qxml.h>
// application specific include files
#include "common/define.h"

class CreatureHandler;
class GenericFightUnit;

/** Manage a creature */
class Creature
{
public:
	/** Constructor */
	Creature();

	/** \return Returns the race of the creature */
	int getRace() { return _race; }

	/** Sets the race of teh creature */
	void setRace( int race ) { _race = race; }

	/** \return Returns the level of the creature */
	int getLevel() { return _level; }

	/** Sets the level of the creature */
	void setLevel( int level ) { _level = level; }

	/** Set name of the creature */
	void setName( QString name ) { _name = name; }

	/** Return name of the creature */
	QString getName() { return _name; }

	/** Set attack points */
	void setAttack( int nb ) { _ptAttack = nb; }

	/** Return attack points */
	int getAttack() { return _ptAttack; }

	/** Set defense points */
	void setDefense( int nb ) { _ptDefense = nb; }

	/** Return defense points */
	int getDefense() { return _ptDefense; }

	/** Set dist attack points */
	void setDistAttack( int nb ) { _distAttack = nb; }

	/** Return dist attack points */
	int getDistAttack() { return _distAttack; }

	/** Tell if dist attack is allowed */
	bool isDistAttack() { return (_distAttack > 0 ); }

	/** Set max health points */
	void setMaxHealth( int nb ) { _maxHealth = nb; }

	/** Return max health points */
	int getMaxHealth() { return _maxHealth; }

	/** Set max move points */
	void setMaxMove( int nb ) { _maxMove = nb; }

	/** Return max move points */
	int getMaxMove() { return _maxMove; }

	/** Set morale */
	void setMorale( int nb ) { _morale = nb; }

	/** Return morale */
	int getMorale() { return _morale; }

	/** Set luck */
	void setLuck( int nb ) { _luck = nb; }

	/** Return luck */
	int getLuck() { return _luck; }

	/** Set min damages points */
	void setMinDamages( int nb ) { _minDamages = nb; }

	/** Return min damages points */
	int getMinDamages() { return _minDamages; }

	/** Set max damages points */
	void setMaxDamages( int nb ) { _maxDamages = nb; }

	/** Return max damages points */
	int getMaxDamages() { return _maxDamages; }

	int getCost( int res );

	void setCost( int res, uint cost );

	int getMantCost( int res );

	void setMantCost( int res, uint cost );

	void save( QTextStream & ts, int indent );

protected:
	QString _name, _building;
	int _ptAttack, _ptDefense;
	int _maxHealth, _maxMove;
	int _distAttack;
	int _minDamages, _maxDamages;
	int _morale, _luck;
	int _race, _level;
	uint _cost[MAX_RESS];
	uint _mcost[MAX_RESS];
	/// XXX: manage spells...

};

/** Manage a stack of creature */
class CreatureStack
{
public:
	/** Constructor */
	CreatureStack( Creature * creature, int nb );

	/** \return Returns the creature of this stack */
	Creature * getCreature() {
		return _creature;
	}

	/** \return Returns the nb of creatures */
	int getNumber() {
		return _nb;
	}

	/** Increase nb of creatures of the stack */
	void increase( int nb );

	/** Decrease nb of creatures of the stack */
	void decrease( int nb );

protected:
	Creature * _creature;
	int _nb;
};

/** Counter of creatures */
class CreatureCounter
{
public:
	/** Constrcutor */
	CreatureCounter();

	/** Clears counter */
	void clear();

	/** Adds some creatures to the counter */
	void addCreature( Creature * creature, int nb );

	/** Decreases creatures to the counter */
	void decreaseCreature( Creature * creature, int number );

	/** \return Returns the nb of different creatures */
	int getCreatureNumber();

	/** \return Returns the nb of creature 'num' */
	int getCreatureCount( int num );

	/** \return Returns the nb of creature 'creature' */
	int getCreatureCount( Creature * creature );

	/** \return Returns creature 'num' */
	Creature * getCreature( int num );

protected:
	QList<CreatureStack> _list;

};

/** List of creature (of the same race) */
class Race : public QList<Creature>
{
public:
	/** Constructor */
	Race();

	/** \return Returns the name of this race */
	QString getName() { return _name; }

	/** Sets the name of this race */
	void setName( QString name ) { _name = name; }

private:
	QString _name;
};

/** List of races */
class CreatureList : public QList<Race>
{
public:
	/** Constructor */
	CreatureList();

	/** Return num-th creature */
	Creature * at( int race, int level );

	/** Append new unit */
	void append( QString race, Creature * creature );

	/** Return race-id of creature named 'name' */
	int findRace( QString name );

	/** Return level-id of creature named 'name' */
	int findLevel( QString name );

	/** Init list */
	bool init();

	/** Saves the list in a data file */
	bool save();

	/** return the number of units */
	int count();

	/** \return Returns the nb of races */
	uint countRace() { return QList<Race>::count(); }

	/** \return Returns the race 'i' */
	Race * getRace( int i ) { return QList<Race>::at( i ); }

	/** \return Gives the num of the race 'race' */
	int giveNumRace( QString race );

	/** compute the absolute number of the creature associated at 'unit' */
	friend int computeSequence( GenericFightUnit * unit );
};

/*              ------------------------------
 *                         CreatureHandler
 *              ------------------------------ */

/** comment for the class */
class CreatureHandler : public QXmlDefaultHandler
{

public:
	/** Constructor */
	CreatureHandler( CreatureList * list );

	/** Return the error protocol if parsing failed */
	QString errorProtocol() { return _errorProt; }

	/** Before starting parsing */
	bool startDocument();

	/** Define Start elements and associated actions */
	bool startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts );

	/** Define End elements and associated actions */
	bool endElement( const QString& namespaceURI, const QString& localName, const QString& qName );

	/** Define what to do of characters */
	bool characters( const QString& ch );

	/** Error function */
	bool fatalError( const QXmlParseException& exception );

private:
	int _resource;
	QString _errorProt;
	QString _race, _name;
	CreatureList * _list;
	Creature * _creature;
	enum State {
		StateInit,
		StateDocument,
		StateRace,
		StateCreature,
		StateAttack,
		StateDefense,
		StateHealth,
		StateMove,
		StateRange,
		StateMinDamages,
		StateMaxDamages,
		StateMorale,
		StateLuck,
		StateCost,
		StateMantCost,
	};
	State _state;
};


#endif // CREATURE_H
