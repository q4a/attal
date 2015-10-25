/****************************************************************
**
** Attal : Lords of Doom
**
** categoryManager.cpp
** Manages category for number of creatures
**
** Version : $Id: categoryManager.cpp,v 1.4 2003/01/07 22:51:40 lusum Exp $
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

#include "categoryManager.h"

// generic include files
#include <stdlib.h>
// include files for QT
#include <qtextstream.h>
// application specific include files
#include "common/dataTheme.h"
#include "common/genericPlayer.h"
#include "common/log.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;

//
// ----- CategoryManager -----
//

CategoryManager::CategoryManager()
{
	_max = "Maximum";
	_list.setAutoDelete( true );
}

CategoryManager::~CategoryManager()
{

}


int CategoryManager::count()
{
	return _list.count() + 1;
}

int CategoryManager::getCategoryLimit( uchar num )
{
	int ret = -1;

	if( num < _list.count() ) {
		ret = _list.at( num )->getNumber();
	}

	return ret;
}

QString CategoryManager::getCategoryName( uchar num )
{
	QString ret = _max;

	if( num < _list.count() ) {
		ret = _list.at( num )->getCategory();
	}

	return ret;
}

int CategoryManager::getRandomInCategory( uchar num )
{
	int ret = 0;

	if( num < _list.count() ) {
		int min = 0;
		if( num > 0 ) {
			min = _list.at( num-1 )->getNumber();
		}
		int max = _list.at( num )->getNumber();
		int delta = max - min;
		ret = 1 + min + ( rand() % delta );

	} else {
		ret = _list.last()->getNumber();
	}

	return ret;
}

void CategoryManager::addCategory( QString name, int val )
{
	bool inserted = false;

	for( uint i = 0; i < _list.count(); i++ ) {
		if( ( val < _list.at( i )->getNumber() ) && (! inserted ) ) {
			_list.insert( i, new CategoryNumber( name, val ) );
			inserted = true;
		}
	}
	if( ! inserted ) {
		_list.append( new CategoryNumber( name, val ) );
	}
}

void CategoryManager::addMaxCategory( QString name )
{
	_max = name;
}

int CategoryManager::computeCategory( int val )
{
	int ret = 0;
	bool found = false;

	for( uint i = 0; i < _list.count(); i++ ) {
		if( ( val < _list.at( i )->getNumber() ) && ( ! found ) ) {
			ret = i;
			found = true;
		}
	}
	if( ! found ) {
		ret = _list.count();
	}

	return ret;
}

bool CategoryManager::init()
{
	clear();
	CategoryHandler handler( this );
	QFile file( DATA_PATH + "categories.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "categories.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

void CategoryManager::clear()
{
	_list.clear();
	_max = "Maximum";
}

bool CategoryManager::save()
{
	QString filename = DATA_PATH + "categories.dat";
	QFile file( filename );

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE categories>" << endl;
	ts << "<categories>" << endl;

	for( uint i = 0; i < _list.count(); i++ ) {
		int j;

		CategoryNumber * category = _list.at( i );
		ts << "\t<category>" << endl;
		ts << "\t\t<name>" << category->getCategory() << "</name>" << endl;
		ts << "\t\t<value>" << category->getNumber() << "</value>" << endl;
		ts << "\t</category>" << endl;
	}

	ts << "\t<category type=\"max\">" << endl;
	ts << "\t\t<name>" << _max << "</name>" << endl;
	ts << "\t</category>" << endl;

	ts << "</categories>" << endl;

	file.close();

	return true;
}

//
// ----- CategoryNumber -----
//
CategoryManager::CategoryNumber::CategoryNumber( QString name, int val )
{
	_category = name;
	_number = val;
}

//
// ----- CategoryHandler -----
//

CategoryHandler::CategoryHandler( CategoryManager * category )
{
	_category = category;
}

bool CategoryHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_category->clear();

	_state = StateInit;
	return true;
}

bool CategoryHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "categories" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "category" && _state == StateDocument ) {
		_state = StateCategory;
		if( atts.value( "type" ) == "max" ) {
			_isMax = true;
		} else {
			_isMax = false;
		}
		_value = 0;
		_name = "";
	} else if ( qName == "name" && _state == StateCategory ) {
		_state = StateName;
	} else if ( qName == "value" && _state == StateCategory ) {
		_state = StateValue;
	} else {
		// error
		return false;
	}
	return true;
}

bool CategoryHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateCategory:
		_state = StateDocument;
		if( _isMax ) {
			_category->addMaxCategory( _name );
		} else {
			_category->addCategory( _name, _value );
		}
		break;
	case StateName:
		_state = StateCategory;
		break;
	case StateValue:
		_state = StateCategory;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool CategoryHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_name = ch_simplified;
		break;
	case StateValue:
		_value = ch_simplified.toInt();
		break;
	default:
	    return false;
    }

    return true;
}


bool CategoryHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}


