/****************************************************************
**
** Attal : Lords of Doom
**
** warMachine.cpp
** Manages war machines (first aid, balist...)
**
** Version : $Id: warMachine.cpp,v 1.2 2003/04/26 08:29:30 audoux Exp $
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
#include <qtextstream.h>
// application specific include files
#include "common/dataTheme.h"
#include "common/log.h"

extern QString DATA_PATH;


//
// ----- WarMachine -----
//

WarMachine::WarMachine()
{
	for( int i = 0; i < 10; i++ ) {
		_params[i] = 0;
	}
}

int WarMachine::getParam( uint num )
{
	int ret = 0;

	if( num < 10 ) {
		ret = _params[num];
	}

	return ret;
}

void WarMachine::setParam( uint num, int val )
{
	if( num < 10 ) {
		_params[num] = val;
	}
}

void WarMachine::save( QTextStream & ts, int indent )
{
	indentation( &ts, indent );
	ts << "<machine name=\"" << getName() << "\">" << endl;
	indentation( &ts, indent + 1 );
	ts << "<type>" << (int)getType() << "</type>" << endl;
	//for( uint i = 0; i < _params.count(); i++ ) {
	//	indentation( &ts, indent + 1 );
	//	ts << "<param>" << *(_params.at( i )) << "</param>" << endl;
	//}
	indentation( &ts, indent );
	ts << "</machine>" << endl;
}

//
// ----- WarMachineList -----
//

WarMachineList::WarMachineList()
{
	setAutoDelete( true );
}

bool WarMachineList::init()
{
	clear();
	WarMachineHandler handler( this );
	QFile file( DATA_PATH + "machines.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "machines.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

bool WarMachineList::save()
{
	QString filename = DATA_PATH + "machines.dat";
	QFile file( filename );

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE machines>" << endl;
	ts << "<machines>" << endl;

	WarMachine * machine;
	for( uint i = 0; i < count(); i++ ) {
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

bool WarMachineHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "machines" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "machine" && _state == StateDocument ) {
		_state = StateMachine;
		_machine = new WarMachine();
		_machine->setName( atts.value( "name" ) );
		_num = -1;
	} else if ( qName == "type" && _state == StateMachine ) {
		_state = StateType;
	} else if ( qName == "param" && _state == StateMachine ) {
		_state = StateParam;
		_num++;
	} else {
		// error
		return false;
	}
	return true;
}

bool WarMachineHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
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
	QString ch_simplified = ch.simplifyWhiteSpace();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateType:
		_machine->setType( (WarMachine::MachineType)ch_simplified.toInt() );
		break;
	case StateParam:
		_machine->setParam( (uint)_num, ch_simplified.toInt() );
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
