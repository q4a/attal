/****************************************************************
**
** Attal : Lords of Doom
**
** genericLord.h
** manages a lord and its units
**
** Version : $Id: genericLord.h,v 1.41 2008/11/12 18:21:30 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QList>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/define.h"
#include "libCommon/skill.h"
#include "libCommon/specialty.h"
#include "libCommon/technic.h"

class QTextStream;

//class GenericArtefact;
class ArtefactManager;
class Creature;
class GenericFightUnit;
class GenericMapCreature;
class GenericCell;
class GenericPlayer;
class GenericLordModel;
class LordCategoryModel;

enum LordPriority {
	PR_ENEMY = 1,
	PR_BASE,
	PR_BASEOWNER,
	PR_ART,
	PR_BONUS,
	PR_CHEST,
	PR_BUILD_ENEMY,
	PR_BUILD,
	PR_CREAT,
	PR_EXPLORE,
	PR_LAST 
};

/*              ------------------------------
 *                         GenericLord
 *              ------------------------------ */

LordCharac detectCharac( const QString & type );

QString getCharacName( LordCharac charac );

class GenericLord
{

public:
	/** Constructor */
	GenericLord( int id );

	GenericLord( int id, GenericMapCreature * creature );

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
	virtual void setOwner( GenericPlayer * player ); 

	/** Get owner of the lord */
	GenericPlayer * getOwner() { return _player; }

	/** Get name of the lord */
	QString getName();

	/** Get category name of the lord */
	QString getCategoryName();

	/** get type of lord */
	int getRace();

	/** Set the lord cell*/
	virtual void setCell( GenericCell * cell );

	/** Get cell where is the lord */
	GenericCell * getCell() { return _currentCell; }

	/** Set lord sleeping (or not) */
	void setSleeping( bool st ) { _sleeping = st; }

	/** Tell if lord is sleeping */
	bool isSleeping() { return _sleeping; }
	
	/** Tell if lord is a garrison lord */
	bool isGarrison();

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

	void newTurn();
	
	int countUnits();

	bool addCreatures( Creature * creature, int number );

	void updateUnit( uchar pos ,uchar race, uchar level, int nb, uchar move, int health);

	ArtefactManager * getArtefactManager() { return _artefactManager; }

	/** Compute end of turn */
	void endTurn();
	
	/** Collect ressource 'num' */
	int collectRessource( int ) { return 0; }

	int getCharac( LordCharac type );

	int getBaseCharac( LordCharac type );

	void setBaseCharac( LordCharac type, int value );

	void increaseBaseCharac( LordCharac type, int value );

	void decreaseBaseCharac( LordCharac type, int value );

	void removeFromGame();
	
	uint getCost( uint res );

	void setCost( uint res, uint cost );

	void getOut();

	virtual void setVisible( bool state );

	bool isVisible() { return _visible; }
	
	/** Set the destination of the lord */
	void setDestination( GenericCell * cell ) { _dest = cell; }
	
	/** Get the destination of the lord */
	GenericCell * getDestination() { return _dest; }

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

	uint computeForceIndicator(bool real);

	uint computeCreatureMaxManaged();

	void printLordData();

	QString getLordDescription();

	static const uchar MAX_LORDS=255;

protected:
	int _id;

	GenericFightUnit * _units[ MAX_UNIT ];
	ArtefactManager * _artefactManager;
	Specialty _special;
	TechnicalBook _book;
	Skill _skills[ MAX_SKILL ];
	uint _cost[MAX_RESS];
	QList<int> _machines;
	int _priorities[PR_LAST+1];

private:	

	bool setLordModelInit(int id );
	void setLordModelCreature(int id , GenericMapCreature * creature );
	void setLordModelConstr( int id );

	void updateLordModelMachines();
	void updateLordModelUnits();
	void updateLordModelCharac();

	GenericPlayer * _player;
	GenericCell * _currentCell;
	GenericCell * _dest;

	bool _hasBook;
	int _move, _maxMove;
	int _technicPoints, _maxTechnicPoints;
	int _morale, _luck;
	int _experience;
	int _power, _knowledge;
	int _attack, _defense;
	int _vision, _charisma;
	int _level;
	bool _visible;
	bool _sleeping;
	GenericLordModel * _model;


};

/** Model of genericLord */
class GenericLordModel
{
public:
	GenericLordModel();
	
	~GenericLordModel();

	/** Set name of the lord */
	void setName( const QString & name ) { _name = name; }

	/** Get name of the lord */
	QString getName() { return _name; }

	void setCategory( LordCategoryModel * category ) { _category = category; }

	LordCategoryModel * getCategory() { return _category; }

	/** get race of lord */
	uint getRace();

	/** Set (or not) a technical book */
	void setBook( bool st ) { _hasBook = st; }
	/** Tell if the lord has a technical book */
	bool isBook() { return _hasBook; }

	/** Set unit 'num' */
	void setUnit( int num, GenericFightUnit * troop ) { _units[num] = troop; }

	/** Return the unit 'num' */
	GenericFightUnit * getUnit( int num ) { return _units[num]; }
	
	int countUnits();

	/** Changes value of the charac 'type' */
	void setBaseCharac( LordCharac type, int value );

	int getBaseCharac( LordCharac type );

	/** Adds a new war machine */
	void addMachine( int id );

	QString getLordDescription();

	/** \return Returns the number of machines owned */
	uint getMachineNumber();

	/** \return Return the id of the num-th machine */
	uint getMachine( uint num );
	
	uint getCost( uint res );

	void setCost( uint res, uint cost );

	/** Removes the num-th machine */
	void removeMachine( uint num );

	void save( QTextStream & ts, int indent );

private:
	QString _name;
	LordCategoryModel * _category;
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
	int _vision, _charisma;
	int _level;
	
	uint _cost[ MAX_RESS ];
	QList<int> _machines;
};

/** List of GenericLordModel */
class LordList : public QList<GenericLordModel *>
{
public:
	/** Constructor */
	LordList();

	/** Destructor */
	virtual ~LordList();

	/** Return lord in the list */
	GenericLordModel * at( int num ) { return QList<GenericLordModel *>::at( num ); }

	/** Init list */
	bool init();

	/** Save on file */
	bool save();

	void clear();
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
	int _resource;
	int _level, _race;
	int _type;
	int _numUnit, _numLord;
	enum State {
		StateInit,
		StateDocument,
		StateLord,
		StateName,
		StateCharac,
		StateCost,
		StateUnit,
		StateUnitRace,
		StateUnitLevel,
		StateMachine,
		StateNumber
	};
	State _state;
};

#endif // GENERICLORD_H   

