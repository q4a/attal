/****************************************************************
**
** Attal : Lords of Doom
**
** genericDecoration.h
** manage decoration
**
** Version : $Id: genericDecoration.h,v 1.9 2005/11/11 20:47:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/06/2001
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

 
#ifndef GENERICDECORATION_H
#define GENERICDECORATION_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QTextStream>
#include <QXmlDefaultHandler>

// application specific include files
#include "libCommon/genericMapDisposition.h"

class DecorationGroup;

/*              ------------------------------
 *                         GenericDecoration
 *              ------------------------------ */


class GenericDecoration : public GenericMapDisposition
{

public:
	/** Constructor */
	GenericDecoration();

	/** Destructor */
	virtual ~GenericDecoration();

	/** Saves the data of a decoration */
	void save( QTextStream * ts, int indent );
	
	void setGroup( DecorationGroup * group ) { _group = group; }
	
	DecorationGroup * getGroup() { return _group; }

private:
	void clear();
	
	DecorationGroup * _group;
};

class DecorationGroup : public QList<GenericDecoration *>
{
public:
	enum EffectType {
		NONE = 0,
		NO_MOVE,
		DECREASE_MOVECOST,
		INCREASE_MOVECOST,
		NO_TECHNICAL,
		MAX_TECHNICAL,
		NO_BLOCK_DECO
	};
	
	/** Constructor */
	DecorationGroup();
	
	/** Destructor */
	virtual ~DecorationGroup();

	void clear();
	
	/** \return Returns the name of this type of decoration */
	const QString & getName() { return _name; }

	/** Sets a name to the decoration */
	void setName( const QString & name ) { _name = name; }

	/** \return Returns some information about this decoration */
	const QString & getInfo() { return _info; }

	/** Sets some info about this decoration */
	void setInfo( const QString & info ) { _info = info; }

	static QString getEffectTypeString( EffectType type );

	uint getEffectNumber();

	EffectType getEffectType( uint num );

	uint getEffectParam( uint num );

	void addEffect( EffectType, uint param );
	
	void clearEffects();
	
	/** Saves the data of a decoration */
	void save( QTextStream * ts, int indent );
	
protected:
	QString _name, _info;
	QList<EffectType> _effectTypeList;
	QList<uint> _effectParamList;
};

/** List for managing the different decoration group */
class DecorationList : public QList<DecorationGroup *>
{
public:
	/** Constructor */
	DecorationList();

	/** Destructor */
	virtual ~DecorationList();

	/** Init list */
	bool init();

	/** Save the list on data file */
	bool save();

	void clear();
};

/** Parser for the DecorationList */
class DecorationHandler : public QXmlDefaultHandler
{
public:
	/** Constructor */
	DecorationHandler( DecorationList * list );

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
	DecorationList * _list;
	DecorationGroup * _group;
	GenericDecoration * _decor;
	int _height, _width;
	DecorationGroup::EffectType _type;
	uint _param;

	QString _errorProt;

	enum State {
		StateInit,
		StateDocument,
		StateGroup,
		StateDecoration,
		StateDisposition,
		StateName,
		StateInfo,
		StateEffect,
		StateEffectParam
	};
	State _state;
};

#endif // GENERICDECORATION_H
