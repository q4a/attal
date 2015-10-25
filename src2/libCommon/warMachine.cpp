/****************************************************************
**
** Attal : Lords of Doom
**
** warMachine.cpp
** Manages war machines (first aid, balist...)
**
** Version : $Id: warMachine.cpp,v 1.13 2009/10/28 11:54:47 lusum Exp $
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

#include "warMachine.h"

// generic include files
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

extern QString DATA_PATH;


//
// ----- WarMachine -----
//

WarMachine::WarMachine()
{
}

WarMachine::~WarMachine()
{
	while( ! _params.isEmpty() ) {
		delete _params.takeFirst();
	}
}

int WarMachine::getParam( uint num )
{
	int ret = 0;

	if( (int)num < _params.count() ) {
		ret = * ( _params.at( num ) );
	}

	return ret;
}

void WarMachine::addParam( int val )
{
	_params.append( new int( val ) );
}

void WarMachine::addParam0( int val )
{
	delete _params[ 0 ];
	_params[ 0 ] = new int( val );
}

void WarMachine::save( QTextStream & ts, int indent )
{
	indentation( &ts, indent );
	ts << "<machine name=\"" << getName() << "\">" << endl;
	indentation( &ts, indent + 1 );
	ts << "<type>" << (int)getType() << "</type>" << endl;
	for( int i = 0; i < _params.count(); i++ ) {
		indentation( &ts, indent + 1 );
		ts << "<param>" << *(_params.at( i )) << "</param>" << endl;
	}
	indentation( &ts, indent );
	ts << "</machine>" << endl;
}

//
// ----- WarMachineList -----
//

WarMachineList::WarMachineList()
{
	
}

WarMachineList::~WarMachineList()
{
	clear();
}

void WarMachineList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

bool WarMachineList::init()
{
	clear();
	WarMachineHandler handler( this );
	QFile file( DATA_PATH + "machines.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable(  QString( DATA_PATH + "machines.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool WarMachineList::save()
{
	QString filename = DATA_PATH + "machines.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE machines>" << endl;
	ts << "<machines>" << endl;

	WarMachine * machine;
	for( int i = 0; i < count(); i++ ) {
		machine = at( i );
		if( machine ) {
			machine->save( ts, 1 );
		}
	}

	ts << "</machines>" << endl;

	file.close();

	return true;
}

//
// ----- WarMachineHandler -----
//

WarMachineHandler::WarMachineHandler( WarMachineList * list )
{
	_list = list;
}

bool WarMachineHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();

	_state = StateInit;
	return true;
}

bool WarMachineHandler::startElement( const QString &, const QString &, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "machines" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "machine" && _state == StateDocument ) {
		_state = StateMachine;
		_machine = new WarMachine();
		_machine->setName( atts.value( "name" ) );
	} else if ( qName == "type" && _state == StateMachine ) {
		_state = StateType;
	} else if ( qName == "param" && _state == StateMachine ) {
		_state = StateParam;
	} else {
		// error
		return false;
	}
	return true;
}

bool WarMachineHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateMachine:
		_state = StateDocument;
		_list->append( _machine );
		break;
	case StateType:
		_state = StateMachine;
		break;
	case StateParam:
		_state = StateMachine;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool WarMachineHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateType:
		_machine->setType( (WarMachine::MachineType)ch_simplified.toInt() );
		break;
	case StateParam:
		_machine->addParam( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool WarMachineHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}
