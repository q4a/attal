/****************************************************************
**
** Attal : Lords of Doom
**
** genericCell.cpp
** gereric class for managing (not graphical) cells
**
** Version : $Id: genericCell.cpp,v 1.29 2009/10/22 17:16:15 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Nick Semenov
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
#include <stdlib.h>

// include files for QT
#include <QTextStream>

// application specific include
#include "libCommon/log.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericMap.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/dataTheme.h"
//#include "libCommon/artefact.h"

extern QString DATA_PATH;

/** gereric class for managing (not graphical) cells */
GenericCell::GenericCell( int row, int col )
	: _coeff( -1 ),
	  _row( row ),
	  _col( col ),
	  _transition( 0 ),
	  _transitionCellType( 0 ),
	  _decorationGroup( 0 ),
	  _decorationItem( 0 )
{
	_stop = true;
	_lord = 0;
	_building = 0;
	_base = 0;
	_creature = 0;
	_event = 0;
	_divers = 0;
	_realCoeff = _coeff;
}

GenericCell::~GenericCell()
{

}

int GenericCell::getCoeff()
{
	return _realCoeff;
}

void GenericCell::setType( const int type )
{
	if( type < (int)DataTheme.tiles.count() ) {
		_type = type;
		setCoeff( DataTheme.tiles.getCoeffAt( type ) );
	} else {
		setCoeff( DataTheme.tiles.getCoeffAt( 0 ) );
		_type = 0;
	}
}

void GenericCell::setDiversification( uint divers )
{
	if( divers < DataTheme.tiles.at( _type )->getDiversificationNumber() ) {
		_divers = divers;
	} else {
		_divers = 0;
	}
}

void GenericCell::setBase(  GenericBase * base )
{ 
	
	_base = base; 
	_building = 0; 
	_realCoeff = base == NULL ? _coeff : _coeff + COST_ENTER;

}

void GenericCell::setBuilding(  GenericBuilding * build )
{ 
	_building = build; 
	_base = 0; 
	_realCoeff = build == NULL ? _coeff : _coeff + COST_ENTER;

}

void GenericCell::setRandomDiversification()
{
	CellModel * cell;
	uint divers = 0;
	cell = DataTheme.tiles.at( _type );
	divers = cell->getRandomDiversification();
	setDiversification( divers );
}

bool GenericCell::isFree()
{
	//bool ret( false );

	//ret = ( _lord == 0 ) && ( _event == 0 ) && ( _creature == 0 );

	//return ret;
	return  !_lord && !_event && !_creature;
}

void GenericCell::setDecoration( uint group, uint item )
{
	_decorationGroup = group;
	_decorationItem = item;

	setCoeff( DataTheme.tiles.getCoeffAt( _type ) ); 
	DecorationGroup * decoGroup = DataTheme.decorations.at( _decorationGroup );
	
	if( decoGroup ) {
		for( uint i = 0; i < decoGroup->getEffectNumber(); i++ ) {
			switch( decoGroup->getEffectType( i ) ) {
			case DecorationGroup::NONE:
				break;
			case DecorationGroup::NO_MOVE:
				_coeff = -1;
				break;
			case DecorationGroup::DECREASE_MOVECOST:
				if( _coeff != -1 ) {
					uint rate = decoGroup->getEffectParam( i );
					rate += 100;
					_coeff = ( _coeff * 100 ) / rate;
				}
				break;
			case DecorationGroup::INCREASE_MOVECOST:
				if( _coeff != -1 ) {
					uint rate = decoGroup->getEffectParam( i );
					rate += 100;
					_coeff = ( _coeff * rate ) / 100;
				}
				break;
			case DecorationGroup::NO_TECHNICAL:
				break;
			case DecorationGroup::MAX_TECHNICAL:
				break;
			case DecorationGroup::NO_BLOCK_DECO:
				break;
			default:
				logEE( "Should not happen" );
				break;
			}
		}
	}
	_realCoeff = _coeff;
}

//
// ----- CellModel -----
//

CellModel::CellModel( const QString & name, int coeff )
: _name( name ), _coeff( coeff )
{
	_color = Qt::black;
}

CellModel::~CellModel()
{
	while( ! _diversification.isEmpty() ) {
		delete _diversification.takeFirst();
	}
}

void CellModel::addDiversification( uint weight )
{
	_diversification.append( new uint( weight ) );
}

void CellModel::removeDiversification( uint num )
{
	if( _diversification.count() > (int)num ) { // Possibly dangerous, if paranoid use <limit.h>
		uint * div = _diversification.at( num );
		_diversification.removeAll( div );
		delete div;
	}
}

uint CellModel::getDiversificationNumber()
{
	return _diversification.count();
}

uint CellModel::getDiversification( uint num )
{
	uint ret = 0;
	if( (int)num < _diversification.count() ) {
		ret = * ( _diversification.at( num ) );
	}

	return ret;
}

uint CellModel::getRandomDiversification()
{
	uint ret = 0;
	uint i;
	uint nb = getDiversificationNumber();
	uint total = 0, val = 0;

	for( i = 0; i < nb; i++ ) {
		total += getDiversification( i );
	}
	val = rand() % total;
	total = 0;
	for( i = 0; i < nb; i++ ) {
		total += getDiversification( i );
		if( val < total ) {
			ret = i;
			break;
		}
	}

	return ret;
}

void CellModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<tile>" << endl;

	indentation( ts, indent + 1 );
	*ts << "<name>" << _name << "</name>" << endl;

	indentation( ts, indent + 1 );
	*ts << "<coef>" << _coeff << "</coef>" << endl;

	indentation( ts, indent + 1 );
	*ts << "<color>" << endl;

	indentation( ts, indent + 2 );
	*ts << "<red>" << _color.red() << "</red>" << endl;
	indentation( ts, indent + 2 );
	*ts << "<green>" << _color.green() << "</green>" << endl;
	indentation( ts, indent + 2 );
	*ts << "<blue>" << _color.blue() << "</blue>" << endl;

	indentation( ts, indent + 1 );
	*ts << "</color>" << endl;

	uint nbDivers = _diversification.count();
	for( uint i = 0; i < nbDivers; i++ ) {
		indentation( ts, indent + 1 );
		*ts << "<diversification>" << ( * ( _diversification.at( i ) ) ) << "</diversification>" << endl;
	}

	indentation( ts, indent );
	*ts << "</tile>" << endl;
}


//
// ----- CellModelList -----
//

CellModelList::CellModelList()
{
	_width = 30;
	_height = 30;
}

CellModelList::~CellModelList()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}


bool CellModelList::save()
{
	QString filename = DATA_PATH + "tiles.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable( filename ) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE tiles>" << endl;
	ts << "<tiles>" << endl;
	
	indentation( &ts, 1 );
	ts << "<width>" << _width << "</width>" << endl;
	
	indentation( &ts, 1 );
	ts << "<height>" << _height << "</height>" << endl;

	for( int i = 1; i < count(); i++ ) {
		at( i )->save( &ts, 1 );
	}

	ts << "</tiles>" << endl;

	file.close();

	return true;
}

bool CellModelList::init()
{
	clear();
	CellModelHandler handler( this );
	QFile file( DATA_PATH + "tiles.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "tiles.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}
	return true;
}


void CellModelList::print()
{
	logDD( "not implemented yet" );
}

void CellModelList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

//
// ----- CellModelHandler -----
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

bool CellModelHandler::startElement( const QString &, const QString &, const QString& qName, const QXmlAttributes & )
{
	if( qName == "tiles" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "height" && _state == StateDocument ) {
		_state = StateHeight;
	} else if ( qName == "width" && _state == StateDocument ) {
		_state = StateWidth;
	} else if ( qName == "tile" && _state == StateDocument ) {
		_state = StateTile;
		_model = new CellModel();
	} else if ( qName == "name" && _state == StateTile ) {
		_state = StateName;
	} else if ( qName == "coef" && _state == StateTile ) {
		_state = StateCoeff;
	} else if ( qName == "color" && _state == StateTile ) {
		_state = StateColor;
		_red = 0;
		_green = 0;
		_blue = 0;
	} else if ( qName == "red" && _state == StateColor ) {
		_state = StateColorRed;
	} else if ( qName == "green" && _state == StateColor ) {
		_state = StateColorGreen;
	} else if ( qName == "blue" && _state == StateColor ) {
		_state = StateColorBlue;
	} else if ( qName == "diversification" && _state == StateTile ) {
		_state = StateDiversification;
	} else {
		// error
		return false;
	}
	return true;
}

bool CellModelHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateHeight:
		_state = StateDocument;
		break;
	case StateWidth:
		_state = StateDocument;
		break;
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
	case StateColor:
		_model->setColor( QColor( _red, _green, _blue ) );
		_state = StateTile;
		break;
	case StateColorRed:
		_state = StateColor;
		break;
	case StateColorGreen:
		_state = StateColor;
		break;
	case StateColorBlue:
		_state = StateColor;
		break;
	case StateDiversification:
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
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateHeight:
		_list->setHeight( ch_simplified.toInt() );
		break;
	case StateWidth:
		_list->setWidth( ch_simplified.toInt() );
		break;
	case StateName:
		_model->setName( ch_simplified );
		break;
	case StateCoeff:
		_model->setCoeff( ch_simplified.toInt() );
		break;
	case StateColorRed:
		_red = ch_simplified.toInt();
		break;
	case StateColorGreen:
		_green = ch_simplified.toInt();
		break;
	case StateColorBlue:
		_blue = ch_simplified.toInt();
		break;
	case StateDiversification:
		_model->addDiversification( ch_simplified.toInt() );
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

	QList<GenericCell *> neighbours;
	
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
	int transType = 0;

	for( int i = 0; i < neighbours.count(); i++ ) {
		if( neighbours.at( i )->getType() != type ) {
			transType = neighbours.at( i )->getType();
		}
	}

	int flag = 0;
	for( int i = 0; i < neighbours.count(); i++ ) {
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

	QList<GenericCell *> neighbours;

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

/**
		Cells:
		123
		456
		789

		Types:
		5 - lonely cell, 0	14	00100
		25 		1		12	00101
		45 		2		15	00110
		56 		3		13    01100
		58 		4		14	10100
		245  	5		9	00111
		256  	6		8	01101
		258  	7		1	10101
		456		8		2	01110
		458		9		10	10110
		568    	10		11	11100
		2456	11		5	01111
		2458	12		6	10111
		2568	13		4	11101
		4568	14		7	11110
		24568	15		3	11111
*/

const char roadTypeTable[ 16 ] = {
	13, 11, 14, 12, 13,  8,  7,  0, 
	1,   9, 10,  4,  5,  3,  6,  2
};

const char binaryTable[ 16 ] = {
	 4,  5,  6, 12, 20,  7, 13, 21,
	14, 22, 28, 15, 23, 29, 30, 31
};

int getRoadTypeByBinary( char binary )
{
	int index = -1;
	
	for( int i = 0; i < 16; i++ ) {
		if ( binary == binaryTable[ i ] )
		{
			index = i;
			break;
		} 
	}
	
	if( index == -1 ) {
		return -1;
	}
	
	return roadTypeTable[ index ];
}

char smallCompute( GenericMap* theMap, GenericCell* cell, uint decorationGroup )
{
	int row = cell->getRow();
	int col = cell->getCol();
	
	int height = theMap->getHeight();
	int width = theMap->getWidth();
	
	GenericCell * neighbourUp = 0;
	GenericCell * neighbourLeft = 0;
	GenericCell * neighbourRight = 0;
	GenericCell * neighbourDown = 0;
	
	char result = 0;
	
	if( cell->getDecorationGroup() == decorationGroup ) {
		result |= 4;
	}
	
	if ( row > 0 ) {
		neighbourUp = theMap->at ( row - 1, col );
		if( neighbourUp->getDecorationGroup() == decorationGroup ) {
			result |= 1;
		}
	}
	
	if ( col > 0 ) {
		neighbourLeft = theMap->at ( row, col - 1 );
		if( neighbourLeft->getDecorationGroup() == decorationGroup ) {
			result |= 2;
		}
	}
	
	if ( row < height - 1 ) {
		neighbourDown = theMap->at ( row + 1, col );
		if( neighbourDown->getDecorationGroup() == decorationGroup ) {
			result |= 16;
		}
	}
	
	if ( col < width - 1 ) {
		neighbourRight = theMap->at ( row, col + 1  );
		if( neighbourRight->getDecorationGroup() == decorationGroup ) {
			result |= 8;
		}
	}
	
	return result;
}

void computeAndChangeRoadType( GenericMap* theMap, GenericCell* cell, uint decorationGroup )
{
	cell->setDecoration( decorationGroup, 0 );

	int row = cell->getRow();
	int col = cell->getCol();
	
	int height = theMap->getHeight();
	int width = theMap->getWidth();
	
	char binary = smallCompute( theMap, cell, decorationGroup );
	
	int roadType = getRoadTypeByBinary( binary );
	
	if( roadType != -1 ) {
		cell->setDecoration( decorationGroup, roadType );
	}
	
	GenericCell* neighbourUp = 0;
	GenericCell* neighbourLeft = 0;
	GenericCell* neighbourRight = 0;
	GenericCell* neighbourDown = 0;
	
	if ( row > 0 ) {
		neighbourUp = theMap->at( row - 1, col );
		
		char newBinary = smallCompute( theMap, neighbourUp, decorationGroup );
		int newRoadType = getRoadTypeByBinary ( newBinary );
		
		if( newRoadType != -1 ) {
			neighbourUp->setDecoration( decorationGroup, newRoadType );
		}
	}
	
	if( col > 0 ) {
		neighbourLeft = theMap->at( row, col - 1 );
		
		char newBinary = smallCompute ( theMap, neighbourLeft, decorationGroup );
		int newRoadType = getRoadTypeByBinary ( newBinary );
		
		if( newRoadType != -1 ) {
			neighbourLeft->setDecoration( decorationGroup, newRoadType );
		}
	}
	
	if( row < height - 1 ) {
		neighbourDown = theMap->at( row + 1, col );
		
		char newBinary = smallCompute( theMap, neighbourDown, decorationGroup );
		int newRoadType = getRoadTypeByBinary ( newBinary );
		
		if( newRoadType != -1 ) {
			neighbourDown->setDecoration ( decorationGroup, newRoadType );
		}
	}
	
	if ( col < width - 1 ) {
		neighbourRight = theMap->at( row, col + 1  );
		
		char newBinary = smallCompute ( theMap, neighbourRight, decorationGroup );
		int newRoadType = getRoadTypeByBinary ( newBinary );
		
		if( newRoadType != -1 ) {
			neighbourRight->setDecoration ( decorationGroup, newRoadType );
		}
	}
	
}

