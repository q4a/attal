/****************************************************************
**
** Attal : Lords of Doom
**
** genericBuilding.h
** in order to manage buildings...
**
** Version : $Id: genericBuilding.h,v 1.20 2009/11/19 22:14:10 lusum Exp $
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

 
#ifndef GENERICBUILDING_H
#define GENERICBUILDING_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>

// application specific include files
#include "libCommon/action.h"
#include "libCommon/genericMapDisposition.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"
#include "libCommon/genericRessources.h"
//#include "common/genericLord.h"

class QTextStream; 

class GenericPlayer;
class GenericLord;
class GenericCell;
class Condition;


/*              ------------------------------
 *                         GenericBuilding
 *              ------------------------------ */


/** comment for the class */
class GenericBuilding
{
 
public:
	/** Constructor */
	GenericBuilding( int type = 0 );
	
	/** Destructor */
	virtual ~GenericBuilding();

	/** Returns return the id of the building */
	int getId() { return _id; }

	/** Sets the id of this building */
	void setId( int id ) { _id = id; }

	void getInitResources();

	/** Load building */
	virtual bool load( QTextStream * );

	/** Save building */
	void save( QTextStream * ts, int indent = 0 );

	/** \returns Returns the type of the building */
	/** type mean the position of building in DataTheme.buildings list */
	int getType() { return _type; }

	/** Set owner of the building */
	virtual void setOwner( GenericPlayer * player ) { _player = player; }

	/** Return owner of the building */
	GenericPlayer * getOwner() { return _player; }
	
	/** Enter in the building */
	virtual void enter( GenericLord * lord );

	/** Exit of the building */
	virtual void out( GenericLord * ) {}

	/** \return Returns the action list associated to this building */
	QList<Action *> getActionList( Action::ActionType type );

	/** \return Returns the building (model) name */
	QString getName();
	
	int getVision();

	QString getBuildDescription();

	/** Returns the nb of frame for the animation */
	uint getNbFrame();

	/** Returns the freq of the animation */
	int getAnimFreq();

	/** Set position of the building on the map */
	virtual void setPosition( GenericCell * cell );

	/** Return cell of the map where is the building */
	GenericCell * getCell() { return _currentCell; }

	/** \return Returns true if this building has already be visited */
	bool hasBeenVisited();

	/** \return Returns true if the lord has already visited this building */
	bool hasBeenVisited( GenericLord * lord );

	/** \return Returns the condition for entering in this building */
	Condition * getCondition() { return _condition; }
	
	uint getDoorRow();

	uint getDoorCol();
	
	uint getDispoHeight();

	uint getDispoWidth();

	GenericMapDisposition::DispositionType getDisposition( uint row, uint col );
	
	GenericResourceList * getResourceList() { return _ress; }

protected:
	int _id;
	GenericPlayer * _player;
	GenericCell * _currentCell;
	QList<GenericLord *> _lords;
	Condition * _condition;
	GenericResourceList * _ress;

private:
	int _type;

};

/** Model of building */
class GenericBuildingModel : public GenericMapDisposition
{
public:
	/** Constructor */
	GenericBuildingModel();

	/** Destructor */
	virtual ~GenericBuildingModel();

	/** \return Returns the name of the model of building */
	QString getName() { return _name; }

	/** Sets the name of the model of building */
	void setName( const QString & name ) { _name = name; }

	int getVision() { return _vision; }

	void setVision( int vision ) { _vision = vision; }
	
	/** \return Returns a description of the building */
	QString getDescription() { return _description; }
	
	/** Sets a description of the building */
	void setDescription( const QString & description ) { _description = description; }
	
	QString getBuildAutoDescription();

	/** \return Returns the nb of frames for the animation */
	uint getNbFrame() { return _nbFrame; }

	/** Sets the nb of frames used for the animation */
	void setNbFrame( int nb ) { _nbFrame = nb; }

	/** \return Returns the animation frequency */
	int getAnimFreq() { return _animFreq; }

	/** Sets the animation frequency */
	void setAnimFreq( int freq ) { _animFreq = freq; }

	/** Add an action associated to this model of building */
	void addAction( Action * action ) {
		_actionList->append( action );
	}

	/** \return Returns the list of actions associated to this model */
	QList<Action *> * getActionList() {
		return _actionList;
	}

	/** \return Returns the list of actions associated to this model and to a particular type of action*/
	QList<Action *> getActionList( Action::ActionType type );
	
	GenericResourceList * getResourceList() { return _ress; }
	
	/** Clear the list of actions associated to this model of building */
	void clearActions() { _actionList->clear(); }
	
	void save( QTextStream * ts, int indent = 0 );

protected:
	QString _name, _description;
	QList<Action *> * _actionList;
	int _nbFrame, _animFreq;
	GenericResourceList * _ress;
	int _vision;
};

/** list of building model */
class BuildingList : public QList<GenericBuildingModel *>
{
public:
	/** Constructor */
	BuildingList();

	/** Destructor */
	virtual ~BuildingList();

	/** Return base corresponding to 'type' */
	GenericBuildingModel * at( int type ) { return QList<GenericBuildingModel *>::at( type ); }

	/** Init list */
	bool init();

	/** clear list */
	void clear();

	/** Save the list (used by theme editor)*/
	bool save();
};

/** parser for the building list */
class BuildingHandler : public QXmlDefaultHandler
{
public:

	/** Construtor */
	BuildingHandler( BuildingList * list );

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
	BuildingList * _list;
	GenericBuildingModel * _building;
	Action * _action;
	uint _height, _width;
	int _res;
	ElementaryAction * _elementary;

	enum State {
		StateInit,
		StateDocument,
		StateBuilding,
		StateName,
		StateVision,
		StateDescription,
		StateDisposition,
		StateFrame,
		StateAnim,
		StateResource,
		StateAction,
		StateElementary/*,
		StateElementaryArg,
		StateElementaryValue*/
	};
	State _state;
};



#endif // GENERICBUILDING_H
