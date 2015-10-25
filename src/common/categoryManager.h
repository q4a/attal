/****************************************************************
**
** Attal : Lords of Doom
**
** categoryManager.h
** Manages category for number of creatures
**
** Version : $Id: categoryManager.h,v 1.6 2003/01/07 22:51:40 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 16/09/2002
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


#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H


// generic include files
// include files for QT
#include <qlist.h>
#include <qstring.h>
#include <qxml.h>
// application specific include files


/**              ------------------------------
 *                        CategoryManager
 **              ------------------------------ */


class CategoryManager
{
public:
	/** Constructor */
	CategoryManager();
	
	virtual ~CategoryManager();

	bool init();

	void clear();

	bool save();

	/** \return Returns the nb of category */
	int count();

	/** \return Returns the upper limit of the category, or -1 if max category */
	int getCategoryLimit( uchar num );

	/** \return Returns the name of the category 'num' */
	QString getCategoryName( uchar num );

	/** \return Computes a random number in the category 'num' */
	int getRandomInCategory( uchar num );

	/** Adds a new category with a name and an upper limit */
	void addCategory( QString name, int val );

	/** Sets the name of the max category */
	void addMaxCategory( QString name );

	/** \return Compute the category of this number of creatures */
	int computeCategory( int val );

protected:
	/** class for managing numbers in category */
	class CategoryNumber {
	public:
		/** Constructor */
		CategoryNumber( QString name = "", int val = 0 );

		/** \return Returns the nb */
		int getNumber() {
			return _number;
		}

		/** Sets the nb */
		void setNumber( int number ) {
			_number = number;
		}

		/** \return Returns the category name */
		QString getCategory() {
			return _category;
		}

		/** Sets the category name */
		void setCategory( QString category ) {
			_category = category;
		}
	protected:
		int _number;
		QString _category;
	};

	QString _max;
	QList<CategoryNumber> _list;
};

/** Parser for CategoryManager */
class CategoryHandler : public QXmlDefaultHandler
{

public:
	/** this is the construtor */
	CategoryHandler( CategoryManager * list );

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
	CategoryManager * _category;
	bool _isMax;
	QString _name;
	int _value;
	enum State {
		StateInit,
		StateDocument,
		StateCategory,
		StateName,
		StateValue
	};
	State _state;
};

#endif // CATEGORYMANAGER_H
