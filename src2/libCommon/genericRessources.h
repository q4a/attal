/****************************************************************
**
** Attal : Lords of Doom
**
** genericRessources.h
** Management of ressources infos
**
** Version : $Id: genericRessources.h,v 1.22 2008/06/19 20:47:35 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/01/2001
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

 
#ifndef GENERICRESSOURCES_H
#define GENERICRESSOURCES_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>
#include <QXmlDefaultHandler>
// application specific include files
#include "libCommon/action.h"

class QTextStream;

class GenericResource
{

	public:

		GenericResource();
		
		int getValue() { return _value; }

		void setValue( int value ) { _value = value; }
		
		int getId() { return _id; }

		void setId( int id ) { _id = id; }
		
		void decreaseValue( int val );
		
		void increaseValue( int val );
		
		void modPercValue( int percentage );
		
		int getMinValue() { return _minValue; }

		void setMinValue( int value ) { _minValue = value; }

		void updateRandGrowth( int coeff );

		static int getRandGrowth( int coeff );

		void save( QTextStream * ts, int indent );
	
	private:
		int _value;
		int _minValue;
		int _id;
};

class GenericResourceList : public QList<GenericResource *>
{
public:
	/** Constructor */
	GenericResourceList();

	/** Destructor */
	virtual ~GenericResourceList();

	void setListValues(GenericResourceList * other);

	/** Return resource corresponding to 'id' */
	GenericResource * get( uint id );

	/** Change ressource 'num' number */
	void setValue( uint num, uint val );

	void decreaseValue( uint num, uint val );

	void increaseValue( uint num, uint val );

	/** Return number of ressource 'num' */
	int getValue( uint num );
		
	void modPercValue( uint num, int percentage );
		
	void updateRandGrowth( uint num, int coeff );

	void removeResources( GenericResourceList * list );
	
	void addResources( GenericResourceList * list );

	QString getAutoDescription();

	void print();

	bool isRess();
	
 	/** Clear list */
 	void clear();

};


 
class ResourceModel
{
	public:
		
		enum ResourceType {
			RE_LOC_PRESERVABLE,
			RE_LOC_NOPRESERVABLE,
			RE_GLOB_PRESERVABLE,
			RE_GLOB_NOPRESERVABLE
		};

		ResourceModel();

		QString getResourceName() { return _name; }

		void setResourceName( QString name ) { _name = name; }

		int getId() { return _id; }

		void setId( int id );

		bool isPreservable();
		
		bool isGlobal();
		
		QString getFileName() { return _filename; }

		void setFileName( QString name ) { if( !name.isEmpty() ) _filename = name; }
		
		QString getSmallFileName() { return _smallfilename; }

		void setSmallFileName( QString name ) { if( !name.isEmpty() ) _smallfilename = name; }
		
		void setType( ResourceType type ) { _type = type; }
	
		ResourceType getType() { return _type; }
	
		void save( QTextStream * ts, int indent = 0 );
		
	private:
		ResourceType _type;
		int _id;
		QString _name;
		QString _filename;
		QString _smallfilename;

};

class ResourceList : public QList<ResourceModel *>
{
public:
	/** Constructor */
	ResourceList();

	/** Destructor */
	virtual ~ResourceList();

	/** Return resource corresponding to 'id' */
	ResourceModel * get( int id );
	
 	/** Clear list */
 	void clear();

	bool save();
	
	/** Init list */
	bool init();

	QString getRessource( uint num );
	
private:
	QList<QString> _type;
};


/** Parser for ResourceList */
class ResourceHandler : public QXmlDefaultHandler
{

public:
	/** Construtor */
	ResourceHandler( ResourceList * list );

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
	ResourceList * _list;
	ResourceModel * _ress;
	QTextStream * _stream;
	QString _filename, _smallFilename;

	enum State {
		StateInit,
		StateDocument,
		StateResource,
		StateResourceId,
		StateResourceType,
		StateResourceName,
		StateFilename,
		StateSmallFilename
	};
	State _state;
};



 
/*              ------------------------------
 *                         GenericRessources
 *              ------------------------------ */



/** comment for the class */
class GenericRessources : public QList<QString>
{
 
public:
	/** Construtor : load ressources info from 'filename' */
	GenericRessources();
	
	/** Destructor (for pointer) */
	virtual ~GenericRessources();	
	
	/** Initializes ressources */
	bool init();
	
	bool save();

	void update( int num, QString val );
	
	/** Return the ressources 'num' if possible */
	QString getRessource( uint num );
};
 
#endif // GENERICRESSOURCES_H
