/****************************************************************
**
** Attal : Lords of Doom
**
** lordCategoryModel.h
** Manages category of lord models
**
** Version : $Id: lordCategoryModel.h,v 1.7 2007/05/04 21:43:35 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/05/2004
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

 
#ifndef LORDCATEGORYMODEL_H
#define LORDCATEGORYMODEL_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QTextStream>
#include <QXmlDefaultHandler>
// application specific include files
#include "libCommon/define.h"

class LordCategoryModel
{
public:
	/** Constructor */
	LordCategoryModel();

	void save( QTextStream & ts, int indent );

	const QString & getName() {
		return _name;
	}

	void setName( const QString & name ) { _name = name; }

	const QString & getDescription() {
		return _description;
	}

	void setDescription( const QString & description ) { _description = description; }

	/** set race of lord category */
	void setRace( uint race ) { _race = race; }

	/** get race of lord category */
	uint getRace() { return _race; }

	void setEvolution( LordCharac charac, uint value );

	uint getEvolution( LordCharac charac );

	LordCharac getRandomEvolution();

protected:
	QString _name, _description;
	uint _race;
	uint _attack, _defense, _power, _knowledge, _charisma;
};

class LordCategoryList : public QList<LordCategoryModel *>
{
public:
	LordCategoryList();

	~LordCategoryList();

	/** Init list */
	bool init();

	/** Save on file */
	bool save();
	
	bool remove( uint index );

	void clearList();

};

/** Parser for LordCategoryList */
class LordCategoryHandler : public QXmlDefaultHandler
{

public:
	/** this is the construtor */
	LordCategoryHandler( LordCategoryList * list );
	
	/** Destructor */
	~LordCategoryHandler();

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
	LordCategoryList * _list;
	LordCategoryModel * _category;
	LordCharac charac;

	enum State {
		StateInit,
		StateDocument,
		StateCategory,
		StateName,
		StateDescription,
		StateEvolution,
		StateCharac
	};
	State _state;
};



 
#endif // LORDCATEGORYMODEL_H
