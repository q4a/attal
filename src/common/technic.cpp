/****************************************************************
**
** Attal : Lords of Doom
**
** technic.cpp
** deals with Technics and around
**
** Version : $Id: technic.cpp,v 1.4 2002/02/26 22:31:25 audoux Exp $
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

#include "technic.h"
 
// generic include files
// include files for QT
#include <qfile.h>
#include <qxml.h>
#include <qtextstream.h>
// application specific includes
#include "common/log.h"

extern QString DATA_PATH;

TechnicList techList;

//
// -----GenericTechnic -----
//

GenericTechnic::GenericTechnic()
{
	_cost = 1;
	_type = 0;
	_class = 0;
	_level = 1;
}

//
// ----- TechnicList -----
//

TechnicList::TechnicList()
{
	setAutoDelete( true );
	_domain.setAutoDelete( true );
}

TechnicList::~TechnicList()
{
	clearing();
}

void TechnicList::clearing()
{
	QList<GenericTechnic>::clear();
	_domain.clear();
}

void TechnicList::clear()
{
	QList<GenericTechnic>::clear();
}

bool TechnicList::init()
{
	clearing();
	
	TechnicHandler handler( this );
	QFile file( DATA_PATH + "technics.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s): %s", QString( DATA_PATH + "technics.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}
	
	return true;
}

void TechnicList::print()
{
	for( uint i = 0; i < _domain.count(); i++ ) {
		logDD( "%s", _domain.at(i)->latin1() );
	}
}

int TechnicList::addDomain( QString domainName )
{
	QListIterator<QString> ite( _domain );
	int cpt = 0, ret = -1;
	
	for( ite.toFirst(); ite.current(); ++ite ) {
		if( *(ite.current()) == domainName ) {
			ret = cpt;
			break;
		}
		cpt++;
	}
	if( ret == -1 ) {
		ret = cpt;
		_domain.append( new QString( domainName ) );
	}
	
	return ret;	
}

//
// ----- TechnicalBook -----
//

TechnicalBook::TechnicalBook()
{
	
}

//
// ----- TechnicHandler -----
//

TechnicHandler::TechnicHandler( TechnicList * list )
{
	_list = list;
}

bool TechnicHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clearing();
	_state = StateInit;
	_cla = 0;
	return true;
}

bool TechnicHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "technical" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "domain" && _state == StateDocument ) {
		_state = StateDomain;
		_cla = _list->addDomain( atts.value( "name" ) );
	} else if ( qName == "technic" && _state == StateDomain ) {
		_state = StateTechnic;
		_tech = new GenericTechnic();
		_tech->setClass( _cla );
		_tech->setName( atts.value( "name" ) );
	} else if ( qName == "level" && _state == StateTechnic ) {
		_state = StateLevel;
	} else if ( qName == "cost"  && _state == StateTechnic ) {
		_state = StateCost;
	} else {
		// error
		return false;
	}
	return true;
}

bool TechnicHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateDomain:
	    _state = StateDocument;
	    break;
	case StateTechnic:
	    _state = StateDomain;
	    _list->append( _tech );
	    break;
	case StateLevel:
	    _state = StateTechnic;
	    break;
	case StateCost:
	    _state = StateTechnic;
	    break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool TechnicHandler::characters( const QString& ch )
{
	_val = 0;
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateLevel:
		_tech->setLevel( ch_simplified.toInt() );
		break;
	case StateCost:
		_tech->setCost( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool TechnicHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

