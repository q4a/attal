/****************************************************************
**
** Attal : Lords of Doom
**
** genericBase.h
** in order to manage bases...
**
** Version : $Id: genericBase.h,v 1.34 2009/11/19 22:14:10 lusum Exp $
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
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>

// application specific include files
#include "libCommon/action.h"
#include "libCommon/creature.h"
#include "libCommon/define.h"
#include "libCommon/log.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericMapDisposition.h"
#include "libCommon/genericRessources.h"

class QTextStream;

class InsideAction;
class GenericBuilding;
class GenericFightUnit;
class GenericPlayer;
class PriceMarket;

/**              ------------------------------
 *                          GenericBase
 **              ------------------------------ */

class GenericBase
{
public:

	enum BaseState{
		BASE_BUY,
		BASE_SELL,
		BASE_UNIT_BOUGHT,
		BASE_LAST
	};
		

	/** Constructor */
	GenericBase( int race = 0 );

	virtual ~GenericBase();
	
	/** When a lord enters in the base */
	virtual void enter( GenericLord * lord ) { 
		if( !setVisitorLord( lord ) ) {
			logEE( "Enter base failed" );
		}
	}
	
	/** When the lord goes out of the base */
	virtual void out( GenericLord * ) {
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
	virtual void setRace( int race );

	/** Get 'race' */
	int getRace() { return _race; }

	/** Set name */
	void setName( const QString & name ) { _name = name; }
	
	/** Get name */
	QString getName() { return _name; } 

	GenericCell * getCell() { return _currentCell; }

	virtual void setPosition( GenericCell * cell );

	/** Set owner of the building */
	virtual void setOwner( GenericPlayer * player ) { _player = player; }

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

	void addUnit( int pos , int race , int level , int nb );

	uint getDoorRow();

	uint getDoorCol();
	
	uint getDispoHeight();

	uint getDispoWidth();

	GenericMapDisposition::DispositionType getDisposition( uint row, uint col );

	QString getModelName();

	void addBuilding( GenericInsideBuilding * );

	void addBuilding( int type );

	uint getBuildingCount() { return _buildings.count(); }

	GenericInsideBuilding * getBuilding( uint num );

	GenericInsideBuilding * getBuildingByType( uint type );

	bool hasBuildingType( uint type );

	void removeBuilding( GenericInsideBuilding * building );
	
	void removeBuilding( int type );

	void removeBuildingType( uint type );

	void updateBuilding( uchar level, bool create );

	uint getForbiddenBuildingsNumber();

	int getForbiddenBuilding( uint num );

	void addForbiddenBuilding( int num );

	bool isForbidden( int num );

	/** Check building requirements */
	bool canBuildBuilding( InsideBuildingModel * building );

	bool getState( BaseState type ) { return _states[type]; }

	void setState( BaseState type , bool state ) { _states[type] = state; }
	
	int getCreatureProduction( Creature * creature );

	int getCreatureProduction( int num );

	void setCreatureProduction( Creature * creature, int value);

	void initCreatureProduction();

	bool canProduceCreature( Creature * creature );

	Creature  * getCreatureProduced(int num);

	int getCreatureNumber();

	void addCreature( int race, int level, int number );

	void buyCreature( Creature * creature, int number );

	int getVision();
	
	PriceMarket * getPriceMarket() { return _price;}
	
	void setPriceMarket(PriceMarket * price) { _price = price;}

	QString getBaseDescription();

	void getInitResources();
	
	GenericResourceList * getResourceList() { return _ress; }

	QList<Action *> getActionList( Action::ActionType type );

protected:
	int _id;
	int _race;
	QString _name;
	bool _states[BASE_LAST];
	GenericCell * _currentCell;

	GenericPlayer * _player;
	QList<GenericInsideBuilding *> _buildings;
	GenericLord * _visitorLord, * _garrisonLord;
	GenericFightUnit * _units[ MAX_UNIT ];
	QList<int> _forbidden;
	CreatureCounter _counter;
	PriceMarket  * _price;
	GenericResourceList * _ress;
};


/** Model for genericBase */
class GenericBaseModel : public GenericMapDisposition
{
public:
	/** Constructor */
	GenericBaseModel();
	
	/** Destructor */
	virtual ~GenericBaseModel();

	/** \return Returns the name of the model */
	QString getModelName() { return _modelName; }

	/** Sets the name of the model */
	void setModelName( const QString & name ) { _modelName = name; }

	int getRace() { return _race; }

	void setRace( int race ) { _race = race; }

	int getVision() { return _vision; }

	void setVision( int vision ) { _vision = vision; }
	
	void addBuilding( InsideBuildingModel * );

	void removeBuilding( uint num );
	
	InsideBuildingModel * getBuildingModel( int );

	uint getBuildingCount() { return _buildings.count(); }
	
	PriceMarket * getPriceMarket() { return _price;}
	
	void setPriceMarket( PriceMarket * price );
	
	GenericResourceList * getResourceList() { return _ress; }

	/** Add an action associated to this model of base */
	void addAction( Action * action ) {
		_actionList->append( action );
	}

	/** \return Returns the list of actions associated to this model */
	QList<Action *> * getActionList() {
		return _actionList;
	}

	/** Clear the list of actions associated to this model of base */
	void clearActions() { _actionList->clear(); }
	
	void save( QTextStream * ts, int indent = 0 );
	
protected:	
	QString _modelName;
	int _race;
	int _vision;
	QList<InsideBuildingModel *> _buildings;
	QList<Action *> * _actionList;
	PriceMarket * _price;
	GenericResourceList * _ress;
};

/** List of GenericBaseModel */
class BaseList : public QList<GenericBaseModel *>
{
public:
	/** Constructor */
	BaseList();

	/** Destructor */
	virtual ~BaseList();

	/** Return base corresponding to 'type' */
	GenericBaseModel * at( int type ) { return QList<GenericBaseModel *>::at( type ); }
	
	/** Return base corresponding to 'typeName' */
	GenericBaseModel * at( QString typeName );
	
 	/** Clear list */
 	void clear();

	bool save();
	
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
	int _unRace, _unLevel;
	InsideBuildingModel * _building;
	InsideAction * _action;
	QTextStream * _stream;
	QString _baseName;
	int _ress,_mress,_csress;
	int _res;
	uint _height, _width;
	PriceMarket * _price;
	Action * _baseAction;
	ElementaryAction * _elementary;

	enum State {
		StateInit,
		StateDocument,
		StateBase,
		StateName,
		StateVision,
		StateDisposition,
		StatePopulation,
		StatePopulationValue,
		StatePopulationGrowth,
		StatePopulationLoss,
		StateResource,
		StateRessPrice,
		StateRessCost,
		StateAction,
		StateElementary,
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
