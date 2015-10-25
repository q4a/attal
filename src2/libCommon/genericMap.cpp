/****************************************************************
**
** Attal : Lords of Doom
**
** genericMap.cpp
** gereric class for managing (not graphical) map
**
** Version : $Id: genericMap.cpp,v 1.41 2009/10/22 17:16:15 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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

#include "genericMap.h"
 
// generic include files
#undef __USE_ISOC99
#include <math.h>
// include files for QT
#include <QFile>
#include <QString>
#include <QTextStream>

// application specific include files
#include "libCommon/genericBase.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/dataTheme.h"

/** gereric class for managing (not graphical) cells */
GenericMap::GenericMap()
	: _height( 0 ),
	  _width( 0 ),
	  _theCells( 0 )
{
	_path = new PathFinder();
}

GenericMap::~GenericMap()
{
	clear();
}

void GenericMap::clear()
{
	if( _theCells != 0 ) {
		for( uint i = 0; i < _height; i++ ) {
			for( uint j = 0; j < _width; j++ ) {
				delete _theCells[i][j];
			}
			delete [] _theCells[i];
		}
		delete [] _theCells;
	}
	_theCells = 0;
	_width = 0;
	_height = 0;	
	if(_path) {
		delete _path;
		_path = 0;
	}
}

void GenericMap::newUnknownMap( int h, int w )
{
	newMapType( h, w, 0 );
}

void GenericMap::newMapType( int h, int w, int typ )
{
	clear();
	_height = h;
	_width = w;
	_theCells = new GenericCell **[_height];
	uint i;

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	for( i = 0; i < _height; i++ ) {
		for( uint j = 0; j < _width; j++ ) {
			/// XXX: can be improved (smarter code)
			GenericCell * tempo;
			tempo = new GenericCell( i, j );
			tempo->setType( typ );
			_theCells[i][j] = tempo;
		}
	}

	if(!_path) {
		_path = new PathFinder();
	}
	_path->newMap( _height, _width, this );

}

GenericCell * GenericMap::at( uint i, uint j ) {
	if( i < _height && j < _width )  {
		if( _theCells[i][j] != NULL ) {
			return _theCells[i][j]; 
		} else {
			throw "Null cell";
		}
	} else {
		//logEE("row %d, col %d", i, j );
		throw "Point not in map";
	}
}

void GenericMap::changeCell( int i,
		      int j,
		      int typ,
		      int transition,
		      int typtra,
		      uint decorationGroup,
		      uint decorationItem,
					uchar /* diversification */
					)
{
	 //logDD("chg cell i %d j %d typ %d tran %d trancelltype %d decogroup %d decoitem %d", i, j, typ, transition, typtra, decorationGroup, decorationItem );

	_theCells[i][j]->setType( typ );
	_theCells[i][j]->setTransition( transition );
	_theCells[i][j]->setTransitionCellType( typtra );
	_theCells[i][j]->setDecoration( decorationGroup, decorationItem );
	if( decorationGroup  && _theCells[i][j]->getCoeff()==-1) {
		GenericDecoration * deco 	= DataTheme.decorations.at( decorationGroup)->at(decorationItem) ;
		computeStoppable( deco, _theCells[i][j] );
	}
}

bool GenericMap::load( const QString filename )
{
        QFile f( filename );
 
        if (! f.open(QIODevice::ReadOnly) ) {
                logEE("Could not open file %s for reading\n", qPrintable( filename ) );
                return false;
        }
        QTextStream ts( &f );

	int w, h;
	ts >> w;
	ts >> h;
        bool ret = load( &ts, w, h );
 
        f.close();
 
        return ret;
}           

bool GenericMap::load( QTextStream * ts, int width, int heigth )
{
	_width = width;
	_height = heigth;
	uint i, j;
	
	_theCells = new GenericCell **[_height];

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	int val;
	uint item;

	// Loading base cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
			  _theCells[i][j] = new GenericCell( i, j );
			  *ts >> val;
			  _theCells[i][j]->setType( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}

	// Loading diversification cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setDiversification( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}
	
	// Loading transition type
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setTransition( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	// Loading transition cell
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				_theCells[i][j]->setTransitionCellType( val );
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	// Loading decoration of the ground
	for( i = 0; i < _height; i++ ) {
		for( j = 0; j < _width; j++ ) {
			if( ! ts->atEnd() ) {
				*ts >> val;
				if( val ) {
					*ts >> item;
					_theCells[i][j]->setDecoration( val, item );
				}
			} else {
				logEE("Stream too short");
				return false;
			}
		}
	}	
	
	if( _path ) {
		delete _path;
	}
	_path = new PathFinder( _width, _height, this );

	return true;
}


void GenericMap::save( QTextStream * ts )
{
	uint i, j;

	*ts << "\t<map>" << endl;
	*ts << "\t\t<width>" << _width << "</width>" << endl;
	*ts << "\t\t<height>" << _height << "</height>" << endl;
	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getType() << " ";
		}
		*ts << endl;
	}
	*ts << flush;
	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getDiversification() << " ";
		}
		*ts << endl;
	}
	*ts << flush;
	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getTransition() << " ";
		}
		*ts << endl;
	}
	*ts << flush;

	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getTransitionCellType() << " ";
		}
		*ts << endl;
	}
	*ts << flush;

	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getDecorationGroup() << " ";
			if( (int)_theCells[i][j]->getDecorationGroup() ) {
				*ts << (int)_theCells[i][j]->getDecorationItem() << " ";
			}
		}
		*ts << endl;
	}
	*ts << flush;

	*ts << "\t</map>" << endl;
	*ts << flush;
}

void GenericMap::computeStoppable()
{
	for( uint i = 0; i < _height; i++ ) {
		for( uint j = 0; j < _width; j++ ) {
			_theCells[i][j]->setStoppable( true );
			if( _theCells[i][j]->getDecorationGroup()  && _theCells[i][j]->getCoeff()==-1) {
				GenericDecoration * deco 	= DataTheme.decorations.at( _theCells[i][j]->getDecorationGroup())->at(_theCells[i][j]->getDecorationItem()) ;
				computeStoppable( deco, _theCells[i][j] );
			}
			if( _theCells[i][j]->getBase() ) {
				computeStoppable( _theCells[i][j]->getBase() );
			}
			if( _theCells[i][j]->getBuilding() ) {
				computeStoppable( _theCells[i][j]->getBuilding() );
			}
		}
	}
}

void GenericMap::computeStoppable( GenericBase * base )
{
	if( base->getCell() ) {
		GenericBaseModel * baseModel = DataTheme.bases.at( base->getRace() );
		int col = base->getCell()->getCol() - base->getDoorCol();
		int row = base->getCell()->getRow() - base->getDoorRow();
		for( uint i = 0; i < baseModel->getHeight(); i++ ) {
			for( uint j = 0; j < baseModel->getWidth(); j++ ) {
				if( base->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED ) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
					&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						_theCells[row+i][col+j]->setStoppable( false );
					}
				}
			}
		}
	}
}

void GenericMap::computeStoppable( GenericDecoration * deco , GenericCell * cell )
{
	int col = cell->getCol() - deco->getDoorCol();
	int row = cell->getRow() - deco->getDoorRow();
	for( uint i = 0; i < deco->getHeight(); i++ ) {
		for( uint j = 0; j < deco->getWidth(); j++ ) {
			if(deco->getDisposition( i, j )){
				if( deco->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED  || deco->getDisposition( i, j ) == GenericMapDisposition::DOOR) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
							&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						_theCells[row+i][col+j]->setStoppable( false );
					}
				}
			}
		}
	}
}

void GenericMap::computeStoppable( GenericBuilding * building )
{
	if( building->getCell() ) {
		GenericBuildingModel * buildingModel = DataTheme.buildings.at( building->getType() );
		int col = building->getCell()->getCol() - building->getDoorCol();
		int row = building->getCell()->getRow() - building->getDoorRow();
		for( uint i = 0; i < buildingModel->getHeight(); i++ ) {
			for( uint j = 0; j < buildingModel->getWidth(); j++ ) {
				if( building->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED ) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
					&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						_theCells[row+i][col+j]->setStoppable( false );
					}
				}
			}
		}
	}
}

bool GenericMap::isPlaceBaseFree( int race , GenericCell * cell)
{
	if( cell ) {
		GenericBaseModel * baseModel = DataTheme.bases.at( race );
		int col = cell->getCol() - baseModel->getDoorCol();
		int row = cell->getRow() - baseModel->getDoorRow();
		for( uint i = 0; i < baseModel->getHeight(); i++ ) {
			for( uint j = 0; j < baseModel->getWidth(); j++ ) {
				if( baseModel->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED ) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
					&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						if(!_theCells[row+i][col+j]->isStoppable())
							return false;
					}
				}
			}
		}
	}
	return true;
}

bool GenericMap::isPlaceDecoFree( GenericDecoration * deco , GenericCell * cell )
{
	int col = cell->getCol() - deco->getDoorCol();
	int row = cell->getRow() - deco->getDoorRow();
	for( uint i = 0; i < deco->getHeight(); i++ ) {
		for( uint j = 0; j < deco->getWidth(); j++ ) {
			if(deco->getDisposition( i, j )){
				if( deco->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED  || deco->getDisposition( i, j ) == GenericMapDisposition::DOOR) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
							&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						if(!_theCells[row+i][col+j]->isStoppable())
							return false;
					}
				}
			}
		}
	}
	return true;
}

bool GenericMap::isPlaceBuilFree( int type , GenericCell * cell)
{
	if( cell ) {
		GenericBuildingModel * buildingModel = DataTheme.buildings.at( type );
		int col = cell->getCol() - buildingModel->getDoorCol();
		int row = cell->getRow() - buildingModel->getDoorRow();
		for( uint i = 0; i < buildingModel->getHeight(); i++ ) {
			for( uint j = 0; j < buildingModel->getWidth(); j++ ) {
				if( buildingModel->getDisposition( i, j ) == GenericMapDisposition::OCCUPIED ) {
					if( ( int( row + i ) >= 0 ) && ( int( col + j ) >= 0 )
					&& ( ( row + i ) < _height ) && ( ( col + j ) < _width ) ) {
						if(!_theCells[row+i][col+j]->isStoppable()){
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool GenericMap::inMap( uint row, uint col )
{
	//map rows goes from 0 to height-1, map cols goes from 0 to width-1
	//row and col are unit so always positive
	if( row < _height && col < _width )  {
		return true;
	} else {
		//logEE("Point not in map row %d , col %d , height %d, width %d", row, col , _height, _width);
		return false;
	}

}

int GenericMap::computeMinimalNextCost( GenericLord * lord )
{
	int ret = 0;
	int cost = 0;

	GenericCell * cell = lord->getCell();

	uint row = cell->getRow();
	uint col = cell->getCol();

	if( ( row > 0 ) && ( col > 0 ) ) {
		cost = PathFinder::computeCostMvt( cell, at( row - 1, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( row > 0 ) {
		cost = PathFinder::computeCostMvt( cell, at( row - 1, col ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row > 0 ) && ( col < _width - 1 ) ) {
		cost =	PathFinder::computeCostMvt( cell, at( row - 1, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( col > 0 ) {
		cost = PathFinder::computeCostMvt( cell, at( row, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( col < _width - 1 ) {
		cost = PathFinder::computeCostMvt( cell, at( row, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row < _height - 1 ) && ( col > 0 ) ) {
		cost = PathFinder::computeCostMvt( cell, at( row + 1, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( row < _height - 1 ) {
		cost = PathFinder::computeCostMvt( cell, at( row + 1, col ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row < _height - 1 ) && ( col < _width - 1 ) ) {
		cost = PathFinder::computeCostMvt( cell, at( row + 1, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	return ret;
}


void GenericMap::clearData()
{
	uint i,j;
		
	for( i = 0; i < (uint) _height; i++ ) {
		for( j = 0; j < (uint) _width; j++ ) {
			if( _theCells[i][j]->getLord() ) {
				_theCells[i][j]->setLord( 0 );
			}
			if( _theCells[i][j]->getBuilding() ) {
				_theCells[i][j]->setBuilding( 0 );
			}
			if( _theCells[i][j]->getBase() )	{
				_theCells[i][j]->setBase( 0 );
			}
			if( _theCells[i][j]->getCreature() ) {
				_theCells[i][j]->setCreature( 0 );
			}
			if( _theCells[i][j]->getTransition() ) {
				_theCells[i][j]->setTransition( 0 );
				_theCells[i][j]->setTransitionCellType( 0 );
			}
			if( _theCells[i][j]->getType() ) {
				_theCells[i][j]->setType( 0 );
			}
			if( _theCells[i][j]->getCoeff() ) {
				_theCells[i][j]->setCoeff( 0 );
			}
			if( _theCells[i][j]->getDecorationGroup() ) {
				_theCells[i][j]->setDecoration( 0, 0 );
			}
			if( _theCells[i][j]->getEvent() ) {
				_theCells[i][j]->setEvent( 0 );
			}
		}
	}

}

QList<GenericCell *> GenericMap::giveRadiusCell( GenericCell * cell, int radius )
{
	QList<GenericCell *> list;
	int row= cell->getRow();
	int col= cell->getCol();

	uint i,j;
	uint initRow;
	uint initCol;
	uint endRow;
	uint endCol;

	initRow = std::max( row-radius , 0 );
	initCol = std::max( col-radius , 0 );
	endRow =  std::min( row+radius,  (int)( _height -1) );
	endCol = std::min( col+radius, (int)( _width -1));

	for( i = initRow; i <= endRow; i++){
		for( j = initCol; j <= endCol; j++ ) {
			if( computeDist( row, col,  i, j ) <= radius	) {
				list.append( at( i, j ) );
			}
		}
	}
	return list;
}

int GenericMap::computeDist( GenericCell * cell1, GenericCell * cell2 )
{
	int ret = 0;
	int row1 = cell1->getRow();
	int col1 = cell1->getCol();
	int row2 = cell2->getRow();
	int col2 = cell2->getCol();

	ret = computeDist( row1, col1 , row2, col2 );
	return ret;
}


int GenericMap::computeDist( int row1, int col1 , int row2, int col2 )
{ 
	return int( sqrt( ( (col1-col2)*(col1-col2) ) + ( (row1-row2)*(row1-row2) ) ) );
}
