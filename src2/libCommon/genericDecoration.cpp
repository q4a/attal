/****************************************************************
**
** Attal : Lords of Doom
**
** genericDecoration.cpp
** manage decoration
**
** Version : $Id: genericDecoration.cpp,v 1.20 2009/10/28 11:54:47 lusum Exp $
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

#include "genericDecoration.h"

// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"

extern QString DATA_PATH;

GenericDecoration::GenericDecoration()
{
	_group = 0;
}

GenericDecoration::~GenericDecoration()
{
	clear();
}

void GenericDecoration::clear()
{
	GenericMapDisposition::clear();
}

void GenericDecoration::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<decoration>" << endl;
	
	GenericMapDisposition::save( ts, indent + 1 );
		
	indentation( ts, indent );
	*ts << "</decoration>" << endl;
}


//
// ----- DecorationGroup -----
//
DecorationGroup::DecorationGroup()
{
	_name = "";
	_info = "";
}

DecorationGroup::~DecorationGroup()
{
	clear();
}

void DecorationGroup::clear()
{
	_effectTypeList.clear();
	_effectParamList.clear();
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}



void DecorationGroup::save( QTextStream * ts, int indent )
{
	uint nb = count();
	
	indentation( ts, indent );
	*ts << "<group>" << endl;
	
	indentation( ts, indent+1 );
	*ts << "\t<name>" << _name << "</name>" << endl;
	indentation( ts, indent+1 );
	*ts << "\t<info>" << _info << "</info>" << endl;
	
	for( int i = 0; i < _effectTypeList.count(); ++i ) {
		indentation( ts, indent+1 );
		*ts << "\t<effect type=\"" << _effectTypeList.at( i ) << "\">";
		*ts << _effectParamList.at( i ) << "</effect>" << endl;
	}
	
	for( uint i = 0; i < nb; i++ ) {
		at( i )->save( ts, indent + 1 );
	}
	
	indentation( ts, indent );
	*ts << "</group>" << endl;
}

uint DecorationGroup::getEffectNumber()
{
	return _effectTypeList.count();
}

DecorationGroup::EffectType DecorationGroup::getEffectType( uint num )
{
	EffectType ret = NONE;

	if( (int)num < _effectTypeList.count() ) {
		ret = _effectTypeList.at( num );
	}

	return ret;
}

uint DecorationGroup::getEffectParam( uint num )
{
	uint ret = 0;

	if( (int)num < _effectParamList.count() ) {
		ret = _effectParamList.at( num );
	}

	return ret;
}

void DecorationGroup::addEffect( DecorationGroup::EffectType type, uint param )
{
	_effectTypeList.append( type );
	_effectParamList.append( param );
}

void DecorationGroup::clearEffects()
{
	_effectTypeList.clear();
	_effectParamList.clear();
}

QString DecorationGroup::getEffectTypeString( EffectType type )
{
	QString ret = "Unknown";

	switch( type ) {
	case NONE:
		ret = "None";
		break;
	case NO_MOVE:
		ret = "No move";
		break;
	case DECREASE_MOVECOST:
		ret = "Decrease move cost";
		break;
	case INCREASE_MOVECOST:
		ret = "Increase move cost";
		break;
	case NO_TECHNICAL:
		ret = "No technical";
		break;
	case MAX_TECHNICAL:
		ret = "Max technical";
		break;
	case NO_BLOCK_DECO:
		ret = "No blocked decoration";
		break;
	}

	return ret;
}

//
// ----- DecorationList -----
//

DecorationList::DecorationList()
{
	
}

DecorationList::~DecorationList()
{
	clear();	
}

void DecorationList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}


bool DecorationList::init()
{
	clear();
	DecorationHandler handler( this );
	QFile file( DATA_PATH + "decorations.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "decorations.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool DecorationList::save()
{
	QString filename = DATA_PATH + "decorations.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE decorations>" << endl;
	ts << "<decorations>" << endl;

	for( int i = 1; i < count(); i++ ) {
		at( i )->save( &ts, 1 );
	}
	
	ts << "</decorations>" << endl;
	
	file.close();
	
	return true;
}


//
// ----- DecorationHandler -----
//

DecorationHandler::DecorationHandler( DecorationList * list )
{
	_list = list;
}

bool DecorationHandler::startDocument()
{
	_errorProt = "";
	_list->clear();	
	_state = StateInit;
	_list->append( new DecorationGroup() );
	return true;
}
	
bool DecorationHandler::startElement( const QString &, const QString &, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "decorations" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "group" && _state == StateDocument ) {
		_state = StateGroup;
		_group = new DecorationGroup();
	} else if ( qName == "name" && _state == StateGroup ) {
		_state = StateName;
	} else if ( qName == "info" && _state == StateGroup ) {
		_state = StateInfo;
	} else if ( qName == "effect" && _state == StateGroup ) {
		_state = StateEffect;
		_type = (DecorationGroup::EffectType) atts.value( "type" ).toInt();
		_param = 0;
	} else if ( qName == "decoration" && _state == StateGroup ) {
		_state = StateDecoration;
		_decor = new GenericDecoration();
	} else if ( qName == "disposition" && _state == StateDecoration ) {
		_state = StateDisposition;
		_height = atts.value( "height" ).toInt();
		_width = atts.value( "width" ).toInt();
		if( ( _height != 0 ) && ( _width != 0 ) ) {
			_decor->init( _height, _width );
		}
	} else {
		return false;
	}
	return true;
}

bool DecorationHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateGroup:
		_list->append( _group );
		_state = StateDocument;
		break;
	case StateName:
		_state = StateGroup;
		break;
	case StateInfo:
		_state = StateGroup;
		break;
	case StateEffect:
		_state = StateGroup;
		_group->addEffect( _type, _param );
		break;
	case StateDecoration:
		_state = StateGroup;
		_group->append( _decor );
		_decor->setGroup( _group );
		break;
	case StateDisposition:
		_state = StateDecoration;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool DecorationHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateDisposition:{
		if( ( _height > 0 ) && ( _width > 0 ) ) {
			QStringList li = ch_simplified.split( " " );
			GenericMapDisposition::DispositionType type;
			for( int i = 0; i < _height; i++ ) {
				for( int j = 0; j < _width; j++ ) {
					type = GenericMapDisposition::DispositionType( li[ i*_width + j ].toInt() );
					_decor->setDisposition( i, j, type );
				}
			}
		}
		} break;
	case StateName:
		_group->setName( ch_simplified );
		break;
	case StateInfo:
		_group->setInfo( ch_simplified );
		break;
	case StateEffect:
		_param = ch_simplified.toInt();
		break;
	default:
	    return false;
    }

    return true;
}

bool DecorationHandler::fatalError( const QXmlParseException& exception )
{
	_errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
		      .arg( exception.message() )
		      .arg( exception.lineNumber() )
		      .arg( exception.columnNumber() );

	return QXmlDefaultHandler::fatalError( exception );
}
