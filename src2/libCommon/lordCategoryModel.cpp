/****************************************************************
**
** Attal : Lords of Doom
**
** lordCategoryModel.cpp
** Manages category of lord models
**
** Version : $Id: lordCategoryModel.cpp,v 1.18 2009/10/28 11:57:34 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

#include "lordCategoryModel.h"

// generic include files
#include <stdlib.h>
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"

extern QString DATA_PATH;

//
// ----- LordCategoryModel -----
//

LordCategoryModel::LordCategoryModel()
: _name( "" ), _description( "" ), _race( 0 )
{
	_attack = 1;
	_defense = 1;
	_power = 1;
	_knowledge = 1;
	_charisma = 1;
}

void LordCategoryModel::save( QTextStream & ts, int indent )
{
	indentation( &ts, indent );
	ts << "<category race=\"" << _race << "\">" << endl;
	indentation( &ts, indent + 1 );
	ts << "<name>" << getName() << "</name>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<description>" << getName() << "</description>" << endl;
	indentation( &ts, indent + 1 );
	ts << "<evolution>" << endl;
	indentation( &ts, indent + 2 );
	ts << "<characteristic type=\"" << getCharacName( ATTACK ) << "\">";
	ts << _attack << "</characteristic>" << endl;
	indentation( &ts, indent + 2 );
	ts << "<characteristic type=\"" << getCharacName( DEFENSE ) << "\">";
	ts << _defense << "</characteristic>" << endl;
	indentation( &ts, indent + 2 );
	ts << "<characteristic type=\"" << getCharacName( POWER ) << "\">";
	ts << _power << "</characteristic>" << endl;
	indentation( &ts, indent + 2 );
	ts << "<characteristic type=\"" << getCharacName( KNOWLEDGE ) << "\">";
	ts << _knowledge << "</characteristic>" << endl;
	indentation( &ts, indent + 2 );
	ts << "<characteristic type=\"" << getCharacName( CHARISMA ) << "\">";
	ts << _charisma << "</characteristic>" << endl;
	indentation( &ts, indent + 1 );
	ts << "</evolution>" << endl;
	indentation( &ts, indent );
	ts << "</category>" << endl;
}

void LordCategoryModel::setEvolution( LordCharac charac, uint value )
{
	switch( charac ) {
	case ATTACK:
		_attack = value;
		break;
	case DEFENSE:
		_defense = value;
		break;
	case POWER:
		_power = value;;
		break;
	case KNOWLEDGE:
		_knowledge = value;
		break;
	case CHARISMA:
		_charisma = value;
		break;
	default:
		break;
	}
}

uint LordCategoryModel::getEvolution( LordCharac charac )
{
	uint ret;

	switch( charac ) {
	case ATTACK:
		ret = _attack;
		break;
	case DEFENSE:
		ret = _defense;
		break;
	case POWER:
		ret = _power;
		break;
	case KNOWLEDGE:
		ret = _knowledge;
		break;
	case CHARISMA:
		ret = _charisma;
		break;
	default:
		ret = 0;
	}

	return ret;
}

LordCharac LordCategoryModel::getRandomEvolution()
{
	LordCharac ret = ATTACK;
	uint sum = _attack + _defense + _power + _knowledge + _charisma;

	if( sum > 0 ) {
		uint alea = rand() % sum;

		if( alea < _attack ) {
			ret = ATTACK;
		} else if( alea < ( _attack + _defense ) ) {
			ret = DEFENSE;
		} else if( alea < ( _attack + _defense + _power ) ) {
			ret = POWER;
		} else if( alea < ( _attack + _defense + _power + _knowledge) ) {
			ret = KNOWLEDGE;
		} else {
			ret = CHARISMA;
		}
	}

	return ret;
}

//
// ----- LordCategoryList -----
//

LordCategoryList::LordCategoryList()
{
}

LordCategoryList::~LordCategoryList()
{
	clearList();
}

bool LordCategoryList::init()
{
	clearList();

	LordCategoryHandler handler( this );
	QString filename = "lordCategories.dat";
	QFile file( DATA_PATH + filename );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + filename ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool LordCategoryList::save()
{
	QString filename = DATA_PATH + "lordCategories.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE lordCategories>" << endl;
	ts << "<lordCategories>" << endl;

	LordCategoryModel * category;
	for( int i = 0; i < count(); i++ ) {
		category = at( i );
		if( category ) {
			category->save( ts, 1 );
		}
	}

	ts << "</lordCategories>" << endl;

	file.close();

	return true;
}

bool LordCategoryList::remove( uint index )
{
	delete takeAt(index);
	return true;
}

void LordCategoryList::clearList()
{
	while (!isEmpty()) {
		delete takeFirst();
	}
}


//
// ----- LordCategoryHandler -----
//

LordCategoryHandler::LordCategoryHandler( LordCategoryList * list )
{
	_list = list;
	_category = 0;
}

LordCategoryHandler::~LordCategoryHandler()
{
}


bool LordCategoryHandler::startDocument()
{
	_errorProt = "";
	_list->clearList();

	_state = StateInit;
	return true;
}

bool LordCategoryHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & atts )
{
	if( qName == "lordCategories" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "category" && _state == StateDocument ) {
		_state = StateCategory;
		_category = new LordCategoryModel();
		_category->setRace( atts.value( "race" ).toUInt() );
	} else if ( qName == "name" && _state == StateCategory ) {
		_state = StateName;
	} else if( qName == "description" && _state == StateCategory ) {
		_state = StateDescription;
	} else if( qName == "evolution" && _state == StateCategory ) {
		_state = StateEvolution;
	} else if( qName == "characteristic" && _state == StateEvolution ) {
		_state = StateCharac;
		charac = detectCharac( atts.value( "type" ) );
	} else {
		// error
		return false;
	}
	return true;
}

bool LordCategoryHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateCategory:
		_state = StateDocument;
		_list->append( _category );
		break;
	case StateName:
		_state = StateCategory;
		break;
	case StateDescription:
		_state = StateCategory;
		break;
	case StateEvolution:
		_state = StateCategory;
		break;
	case StateCharac:
		_state = StateEvolution;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool LordCategoryHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateName:
		_category->setName( ch_simplified );
		break;
	case StateDescription:
		_category->setDescription( ch_simplified );
		break;
	case StateCharac:
		_category->setEvolution( charac, ch_simplified.toUInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool LordCategoryHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}


