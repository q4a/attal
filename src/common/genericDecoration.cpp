/****************************************************************
**
** Attal : Lords of Doom
**
** genericDecoration.cpp
** manage decoration
**
** Version : $Id: genericDecoration.cpp,v 1.6 2002/11/11 17:41:02 audoux Exp $
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
// application specific include files
#include "common/dataTheme.h"

extern DataTheme DataTheme;
extern QString DATA_PATH;

GenericDecoration::GenericDecoration()
{
	_name = "";
	_info = "";
	_technicEffect = 0;
	_moveEffect = 0;
	_moveFactor = 0;
	_width = 1;
	_height = 1;
	_mainRow = 0;
	_mainCol = 0;
	_dispo = new int * [1];
	_dispo[0] = new int[1];
	_dispo[0][0] = 1;
}

GenericDecoration::~GenericDecoration()
{
	clear();
}

void GenericDecoration::clear()
{
	if( _dispo ) {
		for( int i = 1; i < _height; i++ ) {
			delete [] _dispo[i];
			_dispo[i] = 0;
		}
		delete [] _dispo;
	}
	_dispo = 0;
	_height = 0;
	_width = 0;
	_mainRow = 0;
	_mainCol = 0;
}

void GenericDecoration::setDispoSize( int height, int width )
{
	if( ( height > 0 ) && ( width > 0 ) ) {
		int i;
		clear();
		_width = width;
		_height = height;
		_dispo = new int * [height];
		for( i = 0; i < height; i++ ) {
			_dispo[i] = new int[width];
		}
		for( i = 0; i < height; i++ ) {
			for( int j = 0; j < width; j++ ) {
				_dispo[i][j] = 0;
			}
		}
	}
}

int GenericDecoration::getDispo( int row, int col )
{
	int ret = 0;

	if( ( row >= 0 ) && ( row < _height ) && ( col >= 0 ) && ( col < _width ) ) {
		ret = _dispo[row][col];
	}

	return ret;
}

void GenericDecoration::setDispo( int row, int col, int type )
{
	if( ( row >= 0 ) && ( row < _height ) && ( col >= 0 ) && ( col < _width ) ) {
		_dispo[row][col] = type;
		if( type == 1 ) {
			_mainRow = row;
			_mainCol = col;
		}
	}
}

void GenericDecoration::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<decoration>" << endl;
	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;
	indentation( ts, indent );
	*ts << "\t<info>" << _info << "</info>" << endl;
	indentation( ts, indent );
	*ts << "\t<move>" << endl;
	indentation( ts, indent );
	*ts << "\t\t<effect>" << _moveEffect << "</effect>" << endl;
	indentation( ts, indent );
	*ts << "\t\t<factor>" << _moveFactor << "</factor>" << endl;
	indentation( ts, indent );
	*ts << "\t</move>" << endl;
	indentation( ts, indent );
	*ts << "\t<technic>" << endl;
	indentation( ts, indent );
	*ts << "\t\t<effect>" << _technicEffect << "</effect>" << endl;
	indentation( ts, indent );
	*ts << "\t</technic>" << endl;
	indentation( ts, indent );
	*ts << "\t<disposition height=\"" << _height << "\" width=\""  << _width << "\">" << endl;

	for( int i = 0; i < _height; i++ ) {
		indentation( ts, indent );
		*ts << "\t\t";
		for( int j = 0; j < _width; j++ ) {
			*ts << _dispo[i][j];
			if( j != ( _width-1 ) ) {
				*ts << " ";
			}
		}
		*ts << endl;
	}

	indentation( ts, indent );
	*ts << "\t</disposition>" << endl;
	indentation( ts, indent );
	*ts << "</decoration>" << endl;
}

/***********************************************************************/

DecorationList::DecorationList()
{
	setAutoDelete( true );
}

bool DecorationList::init()
{
	clear();
	DecorationHandler handler( this );
	QFile file( DATA_PATH + "decorations.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "decorations.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

bool DecorationList::save()
{
	QString filename = DATA_PATH + "decorations.dat";
	QFile file( filename );

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE decorations>" << endl;
	ts << "<decorations>" << endl;

	for( uint i = 0; i < count(); i++ ) {
		//GenericDecoration * decor = at( i );
		at( i )->save( &ts, 1 );
	}
	
	ts << "</decorations>" << endl;
	
	file.close();
	
	return true;
}



/***********************************************************************/

DecorationHandler::DecorationHandler( DecorationList * list )
{
	_list = list;
}

bool DecorationHandler::startDocument()
{
	_errorProt = "";
	_list->clear();	
	_state = StateInit;
	
	return true;
}
	
bool DecorationHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "decorations" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "decoration" && _state == StateDocument ) {
		_state = StateDecoration;
		_decor = new GenericDecoration();
	} else if ( qName == "disposition" && _state == StateDecoration ) {
		_state = StateDisposition;
		_height = atts.value( "height" ).toInt();
		_width = atts.value( "width" ).toInt();
		if( ( _height != 0 ) && ( _width != 0 ) ) {
			_decor->setDispoSize( _height, _width );
		}
	} else if ( qName == "name" && _state == StateDecoration ) {
		_state = StateName;
	} else if ( qName == "info" && _state == StateDecoration ) {
		_state = StateInfo;
	} else if ( qName == "move" && _state == StateDecoration ) {
		_state = StateMove;
	} else if ( qName == "effect" && _state == StateMove ) {
		_state = StateMoveEffect;
	} else if ( qName == "factor" && _state == StateMove ) {
		_state = StateMoveFactor;
	} else if ( qName == "technic" && _state == StateDecoration ) {
		_state = StateTechnic;
	} else if ( qName == "effect" && _state == StateTechnic ) {
		_state = StateTechnicEffect;
	} else {
		return false;
	}
	return true;
}

bool DecorationHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateDecoration:
		_state = StateDocument;
		_list->append( _decor );
		break;
	case StateDisposition:
		_state = StateDecoration;
		break;
	case StateName:
		_state = StateDecoration;
		break;
	case StateInfo:
		_state = StateDecoration;
		break;
	case StateMove:
		_state = StateDecoration;
		break;
	case StateMoveEffect:
		_state = StateMove;
		break;
	case StateMoveFactor:
		_state = StateMove;
		break;
	case StateTechnic:
		_state = StateDecoration;
		break;
	case StateTechnicEffect:
		_state = StateTechnic;
		break;
	default:
		// do nothing
		break;
	}
	return true;
}

bool DecorationHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateDisposition:{
		if( ( _height > 0 ) && ( _width > 0 ) ) {
			QStringList li = QStringList::split( " ", ch_simplified );
			for( int i = 0; i < _height; i++ ) {
				for( int j = 0; j < _width; j++ ) {
					_decor->setDispo( i, j, li[ (i*_width) + j ].toInt() );
				}
			}
		}
		} break;
	case StateName:
		_decor->setName( ch_simplified );
		break;
	case StateInfo:
		_decor->setInfo( ch_simplified );
		break;
	case StateMoveEffect:
		_decor->setMoveEffect( ch_simplified.toInt() );
		break;
	case StateMoveFactor:
		_decor->setMoveFactor( ch_simplified.toInt() );
		break;
	case StateTechnicEffect:
		_decor->setTechnicEffect( ch_simplified.toInt() );
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
