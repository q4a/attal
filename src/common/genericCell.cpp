/****************************************************************
**
** Attal : Lords of Doom
**
** genericCell.cpp
** gereric class for managing (not graphical) cells
**
** Version : $Id: genericCell.cpp,v 1.19 2003/04/06 20:37:25 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

#include "genericCell.h"


// generic include files
// include files for QT
// application specific include
#include "log.h"
#include "common/genericBase.h"
#include "common/genericBuilding.h"
#include "common/genericEvent.h"
#include "common/genericMap.h"
#include "common/genericMapCreature.h"
#include "common/dataTheme.h"
#include "common/artefact.h"

extern QString DATA_PATH;
extern DataTheme DataTheme;

/** gereric class for managing (not graphical) cells */
GenericCell::GenericCell( int row, int col )
	: _coeff( -1 ),
	  _row( row ),
	  _col( col ),
	  _transition( 0 ),
	  _transitionCellType( 0 ),
	  _decoration( 0 )
{
	_stop = true;
	_lord = 0;
	_building = 0;
	_base = 0;
	_artefact = 0;
	_creature = 0;
	_event = 0;
}

GenericCell::~GenericCell()
{

}

int GenericCell::getCoeff()
{
	if( _building || _base ) {
		return _coeff + COST_ENTER;
	} else {
		return _coeff;
	}
}

void GenericCell::setType( const int type )
{
	_type = type;

	if( type < (int)DataTheme.tiles.count() ) {
		setCoeff( DataTheme.tiles.getCoeffAt( type ) );
	} else {
		setCoeff( DataTheme.tiles.getCoeffAt( 0 ) );
	}
}

bool GenericCell::isFree()
{
	bool ret( false );

	ret = ( _lord == 0 ) && ( _artefact == 0 ) && ( _creature == 0 );

	return ret;
}



//
// ----- CellModelList -----
//

CellModelList::CellModelList()
{
	setAutoDelete( true );
}

void CellModelList::save()
{
	logEE( "CellModelList::save not implemented yet !" );
}

bool CellModelList::init()
{
	clear();
	CellModelHandler handler( this );
	QFile file( DATA_PATH + "tiles.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "tiles.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}
	return true;
}


void CellModelList::print()
{
  logDD( "not implemented yet" );
}

//
// ----- CellModelList -----
//

CellModelHandler::CellModelHandler( CellModelList * list )
{
	_list = list;
}

bool CellModelHandler::startDocument()
{
	_errorProt = "";
	_list->clear();
	_model = new CellModel( "unknown", -1 );
	_list->append( _model );
	_state = StateInit;
	return true;
}

bool CellModelHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "tiles" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "tile" && _state == StateDocument ) {
		_state = StateTile;
		_model = new CellModel();
	} else if ( qName == "name" && _state == StateTile ) {
		_state = StateName;
	} else if ( qName == "coef" && _state == StateTile ) {
		_state = StateCoeff;
	} else {
		// error
		return false;
	}
	return true;
}
	
bool CellModelHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateTile:
		_state = StateDocument;
		_list->append( _model );
		break;
	case StateName:
		_state = StateTile;
		break;
	case StateCoeff:
		_state = StateTile;
		break;
	default:
		// do nothing
		break;
    }
    return true;	
}
	
bool CellModelHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateName:
		_model->setName( ch_simplified );
		break;
	case StateCoeff:
		_model->setCoeff( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}

bool CellModelHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}

//
//
//
int computeFlag( GenericCell * neighbour, GenericCell * center )
{
	int ret = 0;
	int deltaW = neighbour->getCol() - center->getCol();
	int deltaH = neighbour->getRow() - center->getRow();

	switch( deltaH ) {
	case -1:
		switch( deltaW ) {
		case -1:
			ret = 0x01;
			break;
		case 0:
			ret = 0x02;
			break;
		case 1:
			ret = 0x04;
			break;
		}
		break;
	case 0:
		switch( deltaW ) {
		case -1:
			ret = 0x08;
			break;
		case 1:
			ret = 0x10;
			break;
		}
		break;
	case 1:
		switch( deltaW ) {
		case -1:
			ret = 0x20;
			break;
		case 0:
			ret = 0x40;
			break;
		case 1:
			ret = 0x80;
			break;
		}
		break;
	}

	return ret;
}

char transitionTable[256] = {
	0,8,7,7,6,0,7,7,	// 0-7
	5,5,13,13,0,0,13,13,	// 8-15
	4,14,14,0,4,0,14,14,	// 16-23
	0,0,0,0,0,0,0,0,	// 24-31
	3,0,0,0,0,0,0,0,	// 32-39
	5,5,13,13,0,0,13,13,	// 40-47
	0,0,0,0,0,0,0,0,	// 48-55
	0,0,0,0,0,0,0,0,	// 56-63
	2,0,0,0,0,0,0,0,	// 64-71
	15,15,0,0,0,0,0,0,	// 72-79
	16,0,0,0,16,0,0,0,	// 80-87
	0,0,0,0,0,0,0,0,	// 88-95
	2,0,0,0,0,0,0,0,	// 96-103
	15,15,0,0,0,0,0,0,	// 104-111
	16,0,0,0,16,0,0,0,	// 112-119
	0,0,0,0,0,0,0,0,	// 120-127
	1,0,0,0,0,0,0,0,	// 128-135
	0,0,0,0,0,0,0,0,	// 136-143
	4,0,14,14,4,0,14,14,	// 144-151
	0,0,0,0,0,0,0,0,	// 152-159
	0,0,0,0,0,0,0,0,	// 160-167
	0,0,0,0,0,0,0,0,	// 168-175
	0,0,0,0,0,0,0,0,	// 176-183
	0,0,0,0,0,0,0,0,	// 184-191
	2,0,0,0,0,0,0,0,	// 192-199
	15,15,0,0,0,0,0,0,	// 200-207
	16,0,0,0,16,0,0,0,	// 208-215
	0,0,0,0,0,0,0,0,	// 216-223
	2,0,0,0,0,0,0,0,	// 224-231
	15,15,0,0,0,0,0,0,	// 232-239
	16,0,0,0,16,0,0,0,	// 240-247
	0,0,0,0,0,0,0,0		// 248-255
};

int computeTransition( GenericMap * theMap, GenericCell * cell )
{
	int ret = 0;

	int row = cell->getRow();
	int col = cell->getCol();
	int height = theMap->getHeight();
	int width = theMap->getWidth();

	QList<GenericCell> neighbours;
	neighbours.setAutoDelete( false );

	if( row > 0 ) {
		if( col > 0 ) {
			neighbours.append( theMap->at( row - 1, col - 1 ) );
		}
		neighbours.append( theMap->at( row - 1, col ) );

		if( col < width - 1 ) {
			neighbours.append( theMap->at( row - 1, col + 1 ) );
		}
	}
	if( col > 0 ) {
		neighbours.append( theMap->at( row, col - 1 ) );
	}
	if( col < width - 1 ) {
		neighbours.append( theMap->at( row, col + 1 ) );
	}
	if( row < height - 1 ) {
		if( col > 0 ) {
			neighbours.append( theMap->at( row + 1, col - 1 ) );
		}
		neighbours.append( theMap->at( row + 1, col ) );

		if( col < width - 1 ) {
			neighbours.append( theMap->at( row + 1, col + 1 ) );
		}
	}

	int type = cell->getType();
	int i;
	int transType = 0;

	for( i = 0; i < neighbours.count(); i++ ) {
		if( neighbours.at( i )->getType() != type ) {
			transType = neighbours.at( i )->getType();
		}
	}

	int flag = 0;
	for( i = 0; i < neighbours.count(); i++ ) {
		if( neighbours.at( i )->getType() == transType ) {
			flag = flag | computeFlag( neighbours.at( i ), cell );
		}
	}

	ret = transitionTable[ flag ];

	return ret;
}

int computeTransitionCellType( GenericMap * theMap, GenericCell * cell )
{
	int ret = 0;

	int row = cell->getRow();
	int col = cell->getCol();
	int height = theMap->getHeight();
	int width = theMap->getWidth();

	QList<GenericCell> neighbours;
	neighbours.setAutoDelete( false );

	if( row > 0 ) {
		if( col > 0 ) {
			neighbours.append( theMap->at( row - 1, col - 1 ) );
		}
		neighbours.append( theMap->at( row - 1, col ) );

		if( col < width - 1 ) {
			neighbours.append( theMap->at( row - 1, col + 1 ) );
		}
	}
	if( col > 0 ) {
		neighbours.append( theMap->at( row, col - 1 ) );
	}
	if( col < width - 1 ) {
		neighbours.append( theMap->at( row, col + 1 ) );
	}
	if( row < height - 1 ) {
		if( col > 0 ) {
			neighbours.append( theMap->at( row + 1, col - 1 ) );
		}
		neighbours.append( theMap->at( row + 1, col ) );

		if( col < width - 1 ) {
			neighbours.append( theMap->at( row + 1, col + 1 ) );
		}
	}

	int type = cell->getType();

	for( int i = 0; i < neighbours.count(); i++ ) {
		if( neighbours.at( i )->getType() != type ) {
			ret = neighbours.at( i )->getType();
		}
	}

	return ret;
}





