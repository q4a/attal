/****************************************************************
**
** Attal : Lords of Doom
**
** warMachine.h
** Manages war machines (first aid, balist...)
**
** Version : $Id: warMachine.h,v 1.6 2005/11/11 20:47:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/11/2002
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


#ifndef WARMACHINE_H
#define WARMACHINE_H


// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QTextStream>
#include <QXmlDefaultHandler>

// application specific include files


/**              ------------------------------
 *                         WarMachine
 **              ------------------------------ */



class WarMachine
{
public:
	enum MachineType {
		WM_HEAL = 0,
		WM_AMMO,
		WM_BALIST,	// XXX: we should find another name for 'stg that can kill enemy units'
		WM_CATAPULT	// XXX: another name for 'stg that can destroy walls and defenses' ?
	};

	/** Constructor */
	WarMachine();

	/** Destructor */
	virtual ~WarMachine();

	int getId() { return _id; }

	void setId( int id ) { _id = id; }

	MachineType getType() { return _type; }

	void setType( MachineType type ) { _type = type; }

	QString getName() { return _name; }

	void setName( const QString & name ) { _name = name; }

	int getParam( uint num );

	void addParam( int val );
	
	void addParam0( int val );

	void save( QTextStream & ts, int indent );

protected:
	int _id;
	MachineType _type;
	QString _name;
	QList<int *> _params;
};

class WarMachineList : public QList<WarMachine *>
{
public:
	/** Constructor */
	WarMachineList();

	/** Destructor */
	virtual ~WarMachineList();

	bool init();

	bool save();

	void clear();
};

class WarMachineHandler : public QXmlDefaultHandler
{

public:
	WarMachineHandler( WarMachineList * list );

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

protected:
	QString _errorProt;
	WarMachineList * _list;
	WarMachine * _machine;
	
	enum State {
		StateInit,
		StateDocument,
		StateMachine,
		StateType,
		StateParam
	};
	State _state;

};



#endif // WARMACHINE_H
