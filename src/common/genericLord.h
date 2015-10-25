/****************************************************************
**
** Attal : Lords of Doom
**
** genericLord.h
** manages a lord and its units
**
** Version : $Id: genericLord.h,v 1.47 2003/04/25 18:59:32 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 25/08/2000
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

 
#ifndef GENERICLORD_H
#define GENERICLORD_H
 
 
// generic include files
// include files for QT
#include <qlist.h>
// application specific include files
#include "common/artefact.h"
#include "common/define.h"
#include "common/technic.h"
#include "common/specialty.h"
#include "common/skill.h"

class QTextStream;
//class GenericArtefact;
class Creature;
class GenericFightUnit;
class GenericCell;
class GenericPlayer;
class GenericLordModel;

/*              ------------------------------
 *                         GenericLord
 *              ------------------------------ */

enum LordCharac {
	ATTACK = 1,
	DEFENSE,
	POWER,
	KNOWLEDGE,
	MOVE,
	MAXMOVE,
	TECHNICPOINT,
	MAXTECHNICPOINT,
	MORALE,
	LUCK,
	VISION,
	EXPERIENCE
};

LordCharac detectCharac( QString type );

/** comment for the class */
class GenericLord
{

public:
	/** Constructor */
	GenericLord();

	/// XXX/ Todo !!
	/** Destructor */
	virtual ~GenericLord();

	/** Copy lord */
	void copy( GenericLord * );

	/** Load lord */
	bool load( QTextStream * );

	/** Save lord */
	void save( QTextStream *, int indent = 0 );

	/** Set Id of the lord */
	void setId( int id );

	/** Return Id of the lord */
	int getId() { return _id; }

	void reinit();

	/** Set owner of the lord */
	void setOwner( GenericPlayer * player ) { _player = player; }

	/** Get owner of the lord */
	GenericPlayer * getOwner() { return _player; }

	/** Get name of the lord */
	QString getName();

	/** Get category name of the lord */
	QString getCategoryName();

	/** get type of lord */
	int getRace();

	/** Intializes the lord position*/
	virtual void setPosition( GenericCell * cell ) { moveTo( cell ); }

	/** Move lord to cell */
	virtual void moveTo( GenericCell * cell );

	/** Get cell where is the lord */
	GenericCell * getCell() { return _currentCell; }

	/** Set lord sleeping (or not) */
	void setSleeping( bool st ) { _sleeping = st; }

	/** Tell if lord is sleeping */
	bool isSleeping() { return _sleeping; }

	/** Tell if lord is active */
	bool isActive() { return true; }

	/** Set (or not) a technical book */
	void setBook( bool st ) { _hasBook = st; }
	/** Tell if the lord has a technical book */
	bool isBook() { return _hasBook; }

	/** Set unit 'num' */
	void setUnit( int num, GenericFightUnit * troop ) { _units[num] = troop; }
	
	/** Return the unit 'num' */
	GenericFightUnit * getUnit( int num ) { return _units[num]; }

	void clearUnits();
	
	int countUnits();

	bool addCreatures( Creature * creature, int number );
	
	/** Add artefact */
	void addArtefact( GenericArtefact * artefact );
	
	/** Get artefact 'num' */
	GenericArtefact * getArtefact( int num );
	
	/** Remove artefact */
	void removeArtefact( int num );

	void removeArtefact( GenericArtefact * artefact );

	void clearArtefacts();
	
	/** Return the nb of artefacts */
	int numArtefact(); /*{ return _artefacts.count(); }*/
	
	/** Compute end of turn */
	void endTurn();
	
	/** Collect ressource 'num' */
	int collectRessource( int num ) { return 0; }

	/** Return level of experience of the lord */
	int getLevel();

	int getCharac( LordCharac type );

	int getBaseCharac( LordCharac type );

	void setBaseCharac( LordCharac type, int value );

	void increaseBaseCharac( LordCharac type, int value );

	void decreaseBaseCharac( LordCharac type, int value );

	int computeCostMvt( GenericCell * cell );

	int computeCostMvt( GenericCell * start, GenericCell * end );

	void removeFromGame();

	void getOut();

	virtual void setVisible( bool state );

	bool isVisible() { return _visible; }

	/** Adds a new war machine */
	void addMachine( int id );

	/** \return Returns the number of machines owned */
	uint getMachineNumber();

	/** \return Return the id of the num-th machine */
	uint getMachine( uint num );

	/** Removes the num-th machine */
	void removeMachine( uint num );

	/** Tells if this machine is owned */
	bool hasMachine( int id );

	/** Tells if this machine type is owned */
	bool hasMachineType( int type );

protected:
	int _id;

	GenericFightUnit * _units[ MAX_UNIT ];
	QList<GenericArtefact> _artefacts;
	Specialty _special;
	TechnicalBook _book;
	Skill _skills[ MAX_SKILL ];
	GenericPlayer * _player;
	GenericCell * _currentCell;

	bool _sleeping;

	bool _hasBook, _visible;
	int _move, _maxMove;
	int _technicPoints, _maxTechnicPoints;
	int _morale, _luck;
	int _experience;
	int _power, _knowledge;
	int _attack, _defense;
	int _vision;
	GenericLordModel * _model;
	QList<int> _machines;
};

/** Model of genericLord */
class GenericLordModel
{
public:
	GenericLordModel();

	/** Set name of the lord */
	void setName( QString name ) { _name = name; }

	/** Get name of the lord */
	QString getName() { return _name; }

	void setCategory( QString category ) { _category = category; }

	QString getCategory() { return _category; }

	/** set race of lord */
	void setRace( int race ) { _race = race; }

	/** get type of lord */
	int getRace() { return _race; }

	/** Set (or not) a technical book */
	void setBook( bool st ) { _hasBook = st; }
	/** Tell if the lord has a technical book */
	bool isBook() { return _hasBook; }

	/** Set unit 'num' */
	void setUnit( int num, GenericFightUnit * troop ) { _units[num] = troop; }

	/** Return the unit 'num' */
	GenericFightUnit * getUnit( int num ) { return _units[num]; }

	/** \return Returns level of experience of the lord */
	int getLevel() { return 3; }

	/** Changes value of the charac 'type' */
	void setBaseCharac( LordCharac type, int value );

	int getBaseCharac( LordCharac type );

	/** Adds a new war machine */
	void addMachine( int id );

	/** \return Returns the number of machines owned */
	uint getMachineNumber();

	/** \return Return the id of the num-th machine */
	uint getMachine( uint num );

	/** Removes the num-th machine */
	void removeMachine( uint num );

	void save( QTextStream & ts, int indent );

private:
	int _race;
	QString _name, _category;
	Specialty _special;
	GenericFightUnit * _units[ MAX_UNIT ];
	Skill _skills[ MAX_SKILL ];
	bool _hasBook;
	int _move, _maxMove;
	int _technicPoints, _maxTechnicPoints;
	int _morale, _luck;
	int _experience;
	int _power, _knowledge;
	int _attack, _defense;
	int _knowledgePoints;
	int _vision;
	QList<int> _machines;
};

/** List of GenericLordModel */
class LordList : public QList<GenericLordModel>
{
public:
	/** Constructor */
	LordList();

	/** Return lord in the list */
	GenericLordModel * at( int num ) { return QList<GenericLordModel>::at( num ); }

	/** Init list */
	bool init();

	/** Save on file */
	bool save();
};

/** Parser for LordList */
class LordHandler : public QXmlDefaultHandler
{

public:
	/** this is the construtor */
	LordHandler( LordList * list );

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
	QString _errorProt, _name;
	LordList * _list;
	GenericLordModel * _lord;
	GenericFightUnit * _unit;
	LordCharac _charac;
	int _level, _race;
	int _type;
	int _numUnit, _numLord;
	enum State {
		StateInit,
		StateDocument,
		StateLord,
		StateRace,
		StateCategory,
		StateCharac,
		StateUnit,
		StateUnitRace,
		StateUnitLevel,
		StateMachine,
		StateNumber
	};
	State _state;
};

#endif // GENERICLORD_H   

