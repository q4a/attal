/****************************************************************
**
** Attal : Lords of Doom
**
** campaign.cpp
** manage campaign
**
** Version : $Id: campaign.cpp,v 1.5 2009/12/27 23:36:50 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 21/08/2004
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

#include "campaign.h"
 
// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/log.h"


//
// ----- Campaign -----
//

Campaign::Campaign()
{
	_current = 0;
	_filename = "";
}

Campaign::~Campaign()
{
	
}

bool Campaign::load( const QString & filename )
{	
	TRACE("const QString & filename %s", qPrintable( filename ) );

	_filename = filename;
	CampaignParser handler( this );
	QFile file( filename );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();

	if( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( filename ), qPrintable( handler.errorProtocol() ) );
	}

	return ok;

}

int Campaign::getScenarioNumber()
{
	return _listScenarii.count();
}
	
void Campaign::addScenario( const QString & scenarioName )
{
	_listScenarii.append( scenarioName );
}

void Campaign::clearScenarii()
{
	_listScenarii.clear();
}

void Campaign::replaceScenario( int i, const QString & scenarioName )
{
	_listScenarii.replace( i, scenarioName );
}
	
QString Campaign::getScenario( int num )
{
	QString ret = "";
	
	if( num < _listScenarii.count() ) {
		ret = _listScenarii.at(num);
	}
	
	return ret;
}

int Campaign::getNextScenario()
{
	TRACE("Campaign::getNextScenario");

	int ret = -1;

	int nextScen = _current + 1;
	
	TRACE(" nextScen %d", nextScen);
	if( nextScen < _listScenarii.count() ) {
		ret = nextScen;
	}

	return ret;

}

void Campaign::save( QTextStream * ts )
{
	(*ts) << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE campaign>" << endl;
	(*ts) << "<campaign>" << endl;
	(*ts) << "\t<name>" << _name << "</name>" << endl;
        (*ts) << "\t<theme>medieval</theme>" << endl;
        (*ts) << "\t<description>" << endl;
	(*ts) << "\t\t" << _description << endl;
	(*ts) << "\t</description>" << endl;
        (*ts) << "\t<current>" << _current << "</current>" << endl;
	for( int i = 0; i < _listScenarii.count(); i++ ) {
		(*ts) << "\t<scenario>" << _listScenarii.at( i ) << "</scenario>" << endl;
	}
	(*ts) << "</campaign>" << endl;
}


//
// ----- CampaignParser -----
//

CampaignParser::CampaignParser( Campaign * campaign )
{
	_campaign = campaign;
}

bool CampaignParser::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_state = StateInit;
	return true;
}

bool CampaignParser::startElement( const QString & /* namespaceURI */, const QString & /* localName */, 
	const QString & qName, const QXmlAttributes & /* atts */ )
{
	bool ret = true;

	if( qName == "campaign" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "name" && _state == StateDocument ) {
		_state = StateName;
	} else if( qName == "description" && _state == StateDocument ) {
		_state = StateDescription;
	} else if( qName == "current" && _state == StateDocument ) {
		_state = StateCurrent;
	} else if( qName == "theme" && _state == StateDocument ) {
		_state = StateTheme;
	} else if( qName == "scenario" && _state == StateDocument ) {
		_state = StateScenario;
	} else {
		logEE( "Not found %s", qPrintable(qName) );
		return false;
	}
	return ret;
}

bool CampaignParser::endElement( const QString & /* namespaceURI */, const QString & /* localName */, const QString & /* qName */ )
{
	bool ret = true;

	switch ( _state ) {
	case StateName:
		_state = StateDocument;
		break;
	case StateDescription:
		_state = StateDocument;
		break;
	case StateCurrent:
		_state = StateDocument;
		break;
	case StateTheme:
		_state = StateDocument;
		break;
	case StateScenario:
		_state = StateDocument;
		break;
	default:
		break;
    }
    return ret;
}

bool CampaignParser::characters( const QString & ch )
{
	bool ret = true;
	QString ch_simplified = ch.simplified();

	if ( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateName:
		_campaign->setName( ch_simplified );
		break;
	case StateDescription:
		_campaign->setDescription( ch_simplified );
		break;
	case StateCurrent:
		_campaign->setCurrentScenario( ch_simplified.toInt() );
		break;
	case StateTheme:
		_campaign->setTheme( ch_simplified );
		break;
	case StateScenario:
		_campaign->addScenario( ch_simplified );
		break;
	default:
		ret = false;
	}
	return ret;
}


bool CampaignParser::fatalError( const QXmlParseException & exception )
{
     logEE( "state %d", _state );

    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


