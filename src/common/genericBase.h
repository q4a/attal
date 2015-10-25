/****************************************************************
**
** Attal : Lords of Doom
**
** genericBase.h
** in order to manage bases...
**
** Version : $Id: genericBase.h,v 1.36 2003/04/10 15:36:47 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/08/2000
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

 
#ifndef GENERICBASE_H
#define GENERICBASE_H
 

// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
#include <qxml.h>
// application specific include files
#include "common/creature.h"
#include "common/define.h"
#include "common/log.h"
#include "common/genericLord.h"
#include "common/genericInsideBuilding.h"

class QTextStream;

class InsideAction;
class GenericBuilding;
class GenericFightUnit;
class GenericPlayer;
//class InsideBuildingModel;

/**              ------------------------------
 *                          GenericBase
 **              ------------------------------ */

class GenericBase
{
public:
	/** Constructor */
	GenericBase();

	virtual ~GenericBase();
	
	/** When a lord enters in the base */
	virtual void enter( GenericLord * lord ) { 
		if( !setVisitorLord( lord ) ) {
			logEE( "Enter base failed" );
		}
	}
	
	/** When the lord goes out of the base */
	virtual void out( GenericLord * lord ) {
		if( isVisitorLord() ) {
			removeVisitorLord();
		} else { 
			logEE( "No visiting lord found" );
		}
	}
	
	/** Load base */
	virtual bool load( QTextStream * );

	/** Save base */
	void save( QTextStream * ts, int indent = 0 );

	/** Collect ressource 'num' */
	int collectRessource( int num );
	
	/** Set id to the base */
	void setId( int id ) { _id = id; }
	
	/** Get id */
	int getId() { return _id; }
	
	/** Set 'race' */
	virtual void setRace( int race ) { _race = race; }

	/** Get 'race' */
	int getRace() { return _race; }

	/** Set name */
	void setName( QString name ) { _name = name; }
	
	/** Get name */
	QString getName() { return _name; } 

	GenericCell * getCell() { return _currentCell; }

	virtual void setPosition( GenericCell * cell );

	/** Set owner of the building */
	void setOwner( GenericPlayer * player ) { _player = player; }

	/** Return owner of the building */
	GenericPlayer * getOwner() { return _player; }
	
	/** Exchange lord (garrison and visitor) */
	void exchangeLords();
	
	/** Set visitor */
	inline bool setVisitorLord( GenericLord * lord );
	
	/** Set garrison */
	inline bool setGarrisonLord( GenericLord * lord );
	
	/** Get visitor */
	inline GenericLord * getVisitorLord();
	
	/** Get visitor unit 'num' */
	inline GenericFightUnit * getVisitorUnit( int num );
	
	/** Set visitor unit 'num' */
	inline void setVisitorUnit( int num, GenericFightUnit * troop );
	
	/** Get garrison */
	inline GenericLord * getGarrisonLord();
	
	/** Get garrison unit 'num' */
	inline GenericFightUnit * getGarrisonUnit( int num );
	
	/** Set garrison unit 'num' */
	inline void setGarrisonUnit( int num, GenericFightUnit * troop );
	
	/** Remove visitor */
	inline GenericLord * removeVisitorLord();
	
	/** Remove garrison */
	inline GenericLord * removeGarrisonLord();
	
	/** Tell if there is a visitor */
	inline bool isVisitorLord();
	
	/** Tell if there is a garrison */
	inline bool isGarrisonLord();

	/** Give unit 'num' of the base */
	GenericFightUnit * getUnit( int num ) { return _units[num]; }
	
	/** Set unit 'num' of the base */
	void setUnit( int num, GenericFightUnit * troop ) { _units[num] = troop; }

	bool canAddGarrison( Creature * creature );

	void addGarrison( Creature * creature, int num );
	
	int getDoorRow();
	
	int getDoorCol();
	
	int getDispo( int row, int col );
	
	int getPopulation() { return _population; }

	void setPopulation( int population ) { _population = population; }

	void getInitPopul();

	QString getModelName();

	void addBuilding( GenericInsideBuilding * );

	void addBuilding( int type );

	uint getBuildingCount() { return _buildings.count(); }

	GenericInsideBuilding * getBuilding( uint num );

	GenericInsideBuilding * getBuildingByType( uint type );

	bool hasBuildingType( int type );

	void removeBuilding( GenericInsideBuilding * building );
	
	void removeBuilding( int type );

	void removeBuildingType( int type );

	uint getForbiddenBuildingsNumber();

	int getForbiddenBuilding( uint num );

	void addForbiddenBuilding( int num );

	bool isForbidden( int num );

	bool canBuild() { return _canBuild; }

	void canBuild( bool state ) { _canBuild = state; }
	
	bool isUnitBought() { return _bought; }

	void isUnitBought( bool state ) { _bought = state; }

	int getCreatureProduction( Creature * creature );

	void initCreatureProduction();

	void buyCreature( Creature * creature, int number );

	int getVision();

protected:
	int _id;
	int _race;
	QString _name;
	bool _canBuild;
	bool _bought;
	GenericCell * _currentCell;
	uint _population;

	GenericPlayer * _player;
	QList<GenericInsideBuilding> _buildings;
	GenericLord * _visitorLord, * _garrisonLord;
	GenericFightUnit * _units[ MAX_UNIT ];
	QList<int> _forbidden;
	CreatureCounter _counter;
};


/** Model for genericBase */
class GenericBaseModel
{
public:
	/** Constructor */
	GenericBaseModel();

	/** \return Returns the name of the model */
	QString getModelName() { return _modelName; }

	/** Sets the name of the model */
	void setModelName( QString name ) { _modelName = name; }
	
	int getDoorRow() { return _doorRow; }
	
	int getDoorCol() { return _doorCol; }

	int getDispo( int row, int col ) { return _dispo[row][col]; }
	
	void setDispo( int row, int col, int disp );
	
	int getPopulation() { return _population; }

	void setPopulation( int population ) { _population = population; }

	int getRace() { return _race; }

	void setRace( int race ) { _race = race; }

	int getVision() { return _vision; }

	void setVision( int vision ) { _vision = vision; }
	
	void addBuilding( InsideBuildingModel * );

	void removeBuilding( int num );
	
	InsideBuildingModel * getBuildingModel( int );

	uint getBuildingCount() { return _buildings.count(); }

protected:	
	QString _modelName;
	int _race;
	int _doorRow, _doorCol;
	int _dispo[5][5];	
	int _population;
	int _vision;
	QList<InsideBuildingModel> _buildings;
};

/** List of GenericBaseModel */
class BaseList : public QList<GenericBaseModel>
{
public:
	/** Constructor */
	BaseList();

	/** Return base corresponding to 'type' */
	GenericBaseModel * at( int type ) { return QList<GenericBaseModel>::at( type ); }
	
	/** Return base corresponding to 'typeName' */
	GenericBaseModel * at( QString typeName );
	
// 	/** Clear list */
// 	void clear();

	void save() {}
	
	/** Init list */
	bool init();
	
// 	/** Add a new type of base */
// 	int addType( QString typeName );
	
// 	/** Get type name corresponding to 'type' */
// 	QString getTypeName( int type ) { return *(_type.at( type )); }
	
private:
	QList<QString> _type;
};

/** Parser for BaseList */
class BaseHandler : public QXmlDefaultHandler
{

public:
	/** Construtor */
	BaseHandler( BaseList * list );

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
	QString _errorProt;
	BaseList * _list;
	GenericBaseModel * _base;
	int _race, _level;
	InsideBuildingModel * _building;
	InsideAction * _action;
	QTextStream * _stream;
	QString _baseName;
	int _ress,_mress;

	enum State {
		StateInit,
		StateDocument,
		StateBase,
		StateName,
		StateDisposition,
		StatePopulation,
		StateBuilding,
		StateBuildingName,
		StateBuildingDescription,
		StateBuildingCost,
		StateBuildingMantCost,
		StateBuildingRequire,
		StateBuildingReplace,
		StateBuildingX,
		StateBuildingY,
		StateBuildingAction,
		StateBuildingActionParam
	};
	State _state;
};


/*************************************************************/

inline bool GenericBase::setVisitorLord( GenericLord * lord )
{
	if( isVisitorLord() )
		return false;
	
	_visitorLord = lord;
	return true;
}

inline bool GenericBase::setGarrisonLord( GenericLord * lord )
{
	if( isGarrisonLord() )
		return false;
	
	_garrisonLord = lord;
	return true;
}

inline GenericLord * GenericBase::getVisitorLord()
{
	return _visitorLord;
}

inline GenericFightUnit * GenericBase::getVisitorUnit( int num )
{
	if( isVisitorLord() ) {
		return getVisitorLord()->getUnit( num );
	}
	
	return 0;
}

inline void GenericBase::setVisitorUnit( int num, GenericFightUnit * troop )
{
	if( isVisitorLord() ) {
		getVisitorLord()->setUnit( num, troop );
	}
}

inline GenericLord * GenericBase::getGarrisonLord()
{
	return _garrisonLord;
}
	
inline GenericFightUnit * GenericBase::getGarrisonUnit( int num )
{
	if( isGarrisonLord() ) {
		return getGarrisonLord()->getUnit( num );
	} else {
		return _units[num];
	}
}

inline void GenericBase::setGarrisonUnit( int num, GenericFightUnit * troop )
{
	if( isGarrisonLord() ) {
		getGarrisonLord()->setUnit( num, troop );
	} else {
		_units[num] = troop;
	}
}

inline GenericLord * GenericBase::removeVisitorLord()
{
	GenericLord * temp = _visitorLord;
	_visitorLord = 0;
	
	return temp;
}

inline GenericLord * GenericBase::removeGarrisonLord()
{
	GenericLord * temp = _garrisonLord;
	_garrisonLord = 0;
	
	return temp;
}

inline bool GenericBase::isVisitorLord()
{
	return( _visitorLord != 0 );
}

inline bool GenericBase::isGarrisonLord()
{
	return( _garrisonLord != 0 );
}

#endif // GENERICBASE_H   
