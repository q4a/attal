/****************************************************************
**
** Attal : Lords of Doom
**
** genericBuilding.h
** in order to manage buildings...
**
** Version : $Id: genericBuilding.h,v 1.33 2002/10/20 20:12:48 audoux Exp $
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
#include <qstring.h>
#include <qlist.h>
#include <qxml.h>
// application specific include files
#include "common/action.h"
#include "common/log.h" 
#include "common/unit.h"
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
	GenericBuilding();
	
	/** Destructor */
	virtual ~GenericBuilding();

	/** Returns return the id of the building */
	int getId() { return _id; }

	/** Sets the id of this building */
	void setId( int id ) { _id = id; }

	/** Load building */
	virtual bool load( QTextStream * );

	/** Save building */
	void save( QTextStream * ts, int indent = 0 );

	/** \returns Returns the type of the building */
	int getType() { return _type; }

	/** Stes the type of the building */
	virtual void setType( int type ) { _type = type; }

	/** Set owner of the building */
	void setOwner( GenericPlayer * player ) { _player = player; }

	/** Return owner of the building */
	GenericPlayer * getOwner() { return _player; }

	/** Set type of the building */
	int getBuildingType() { return _type; }

	/** Return type of the building */
	void setBuildingType( int type ) { _type = type; }

	/** Enter in the building */
	virtual void enter( GenericLord * lord );

	/** Exit of the building */
	virtual void out( GenericLord * lord ) {}

	/** \return Returns the action list associated to this building */
	QList<Action> getActionList( Action::ActionType type );

	/** \return Returns the building (model) name */
	QString getName();

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

protected:
	int _id, _type;
	GenericPlayer * _player;
	GenericCell * _currentCell;
	QList<GenericLord> _lords;
	Condition * _condition;
};

/** Model of building */
class GenericBuildingModel
{
public:
	/** Constructor */
	GenericBuildingModel();

	/** \return Returns the name of the model of building */
	QString getName() { return _name; }

	/** Sets the name of the model of building */
	void setName( QString name ) { _name = name; }

	/** \return Returns a description of the building */
	QString getDescription() { return _description; }

	/** Sets a description of the building */
	void setDescription( QString description ) { _description = description; }

	/** \return Returns the disposition info of case (row, col) */
	int getDispo( int row, int col ) { return _dispo[row][col]; }

	/** Sets the disposition of the case (row, col). 0 means 'free', 1 means 'door' and 2 means 'occupied' */
	void setDispo( int row, int col, int disp );

	/** \return Returns the row of the door */
	int getDoorRow() { return _doorRow; }

	/** \return Returns the col of the door */
	int getDoorCol() { return _doorCol; }

	/** \return Returns the type of building */
	int getType() { return _type; }

	/** Sets the type of building */
	void setType( int type ) { _type = type; }

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
	QList<Action> * getActionList() {
		return _actionList;
	}

protected:
	QString _name, _description;
	QList<Action> * _actionList;
	int _type, _nbFrame, _animFreq;
	int _dispo[5][5];
	int _doorRow, _doorCol;
};

/** list of building model */
class BuildingList : public QList<GenericBuildingModel>
{
public:
        /** Constructor */
        BuildingList();

        /** Return base corresponding to 'type' */
        GenericBuildingModel * at( int type ) { return QList<GenericBuildingModel>::at( type ); }

        /** Init list */
        bool init();

	/// XXX: todo
	/** Save the list (used by theme editor) - Not implemented yet */
	void save() {}
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
	ElementaryAction * _elementary;

	enum State {
		StateInit,
		StateDocument,
		StateBuilding,
		StateName,
		StateDescription,
		StateDisposition,
		StateFrame,
		StateAnim,
		StateAction,
		StateElementary/*,
		StateElementaryArg,
		StateElementaryValue*/
	};
	State _state;
};



#endif // GENERICBUILDING_H
