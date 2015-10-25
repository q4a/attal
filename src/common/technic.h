/****************************************************************
**
** Attal : Lords of Doom
**
** technic.h
** deals with Technics and around
**
** Version : $Id: technic.h,v 1.4 2002/02/26 22:31:25 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/01/2001
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

 
#ifndef GENERICTECHNIC_H
#define GENERICTECHNIC_H
 
// generic include files
// include files for QT
#include <qxml.h>
#include <qlist.h>
#include <qstring.h>
#include <qstringlist.h>
// application specific includes


/*              ------------------------------
 *                         GenericTechnic
 *              ------------------------------ */



/** comment for the class */
class GenericTechnic
{
 
public:
	/** Constructor */
	GenericTechnic();
	
	/** Set cost of the technic */
	void setCost( int val ) { _cost = val; }
	
	/** Return cost of the technic */
	int getCost() { return _cost; }
	
	/** Set class of the technic */
	void setClass( int clas ) { _class = clas; }
	
	/** Return class of the technic */
	int getClass() { return _class; }
	
	/** Set level of the technic */
	void setLevel( int level ) { _level = level; }
	
	/** Return level of the technic */
	int getLevel() { return _level; }
	
	/** Set type of the technic */
	void setType( int typ ) { _type = typ; }
	
	/** Return type of the technic */
	int getType() { return _type; }
	
	/** Set name of the technic */
	void setName( QString name ) { _name = name; }
	
	/** Return name of the technic */
	QString getName() { return _name; }
	
	/** Tell if it is a fighting technic */
	bool isFight() { return true; }
	
protected:
	int _cost;
	int _type;
	int _class;
	int _level;
	QString _name;
};
 
/*              ------------------------------
 *                         TechnicList
 *              ------------------------------ */



/** comment for the class */
class TechnicList : public QList<GenericTechnic>
{
 
public:
	/** Constructor */
	TechnicList();

	~TechnicList();
	
	/** Save list */
	void save() {}
	
	/** Clear list */
	void clearing();
	
	/** Init list */
	bool init();
	
	/** Print list (for debug) */
	void print();
	
	/** Return technic 'type' */
	GenericTechnic * at( int type ) { return QList<GenericTechnic>::at( type ); }

	/** Add a domain name */
	int addDomain( QString domainName );
	
private:
	void clear();

	QList<QString> _domain;
};
 

/*              ------------------------------
 *                         TechnicalBook
 *              ------------------------------ */



/** comment for the class */
class TechnicalBook
{
 
public:
	/** Constructor */
	TechnicalBook();
	
	/** Add a technic */
	void add( GenericTechnic * );
	
private:
	QList<GenericTechnic> _theBook;
};
  

/*              ------------------------------
 *                         TechnicHandler
 *              ------------------------------ */



/** comment for the class */
class TechnicHandler : public QXmlDefaultHandler
{
 
public:
	/** Constructor */
	TechnicHandler( TechnicList * list );
	
	/**  Return the error protocol if parsing failed */
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
	TechnicList * _list;
	GenericTechnic * _tech;
	QString _errorProt, _name;
	int _val;
	int _cla;
	enum State {
		StateInit,
		StateDocument,
		StateDomain,
		StateTechnic,
		StateLevel,
		StateCost 
	};
	State _state;
};
  

#endif // GENERICTECHNIC_H   
