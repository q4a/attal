/****************************************************************
**
** Attal : Lords of Doom
**
** genericMap.cpp
** gereric class for managing (not graphical) map
**
** Version : $Id: genericMap.cpp,v 1.23 2002/12/29 20:40:29 lusum Exp $
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

#include "genericMap.h"
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
// application specific include files
#include "common/log.h"
#include "common/pathFinder.h"
#include "common/genericBase.h"

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
		for( int i = 0; i < _height; i++ ) {
			delete [] _theCells[i];
		}
		delete [] _theCells;
	}
	_theCells = 0;
	_width = 0;
	_height = 0;
}

void GenericMap::newUnknownMap( int h, int w )
{
	clear();
	_height = h;
	_width = w;
	_theCells = new GenericCell **[_height];
	int i;

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	for( i = 0; i < _height; i++ ) {
		for( int j=0; j < _width; j++ ) {
			/// XXX: can be improved (smarter code)
			GenericCell * tempo;
			tempo = new GenericCell( i, j );
			tempo->setType( 0 );
			_theCells[i][j] = tempo;
		}
	}

	_path->newMap( _height, _width, this );
}


void GenericMap::changeCell( int i,
		      int j,
		      int typ,
		      int transition,
		      int typtra,
		      int decoration )
{
	//logDD("chg cell %d %d %d %d %d %d", i, j,
	//      typ, transition, typtra, decoration );

	if( _theCells[i][j]->getType() == 0 ) {
		_theCells[i][j]->setType( typ );
		_theCells[i][j]->setTransition( transition );
		_theCells[i][j]->setTransitionCellType( typtra );
		_theCells[i][j]->setDecoration( decoration );
	} else {
		logEE( "Cell type should be unknown" );
	}

}

bool GenericMap::load( const QString filename )
{
        QFile f( filename );
 
        if (! f.open(IO_ReadOnly) ) {
                logEE("Could not open file %s for reading\n", filename.latin1() );
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
	int i, j;
	
	_theCells = new GenericCell **[_height];

	for( i = 0; i < _height; i++ ) {
		_theCells[i] = new GenericCell *[_width];
	}
	int val;
	
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
				_theCells[i][j]->setDecoration( val );
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
	int i;
	
	*ts << "\t<map>" << endl;
	*ts << "\t\t<width>" << _width << "</width>" << endl;
	*ts << "\t\t<height>" << _height << "</height>" << endl;
	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( int j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getType() << " ";	
		}
		*ts << endl;
	}
	*ts << endl;
	
	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( int j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getTransition() << " ";
		}
		*ts << endl;
	}

	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( int j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getTransitionCellType() << " ";
		}
		*ts << endl;
	}

	*ts << endl;

	for( i = 0; i < _height; i++ ) {
		*ts << "\t\t";
		for( int j = 0; j < _width; j++ ) {
			*ts << (int)_theCells[i][j]->getDecoration() << " ";
		}
		*ts << endl;
	}

	*ts << "\t</map>" << endl;
}

void GenericMap::computeStoppable()
{
	for( int i = 0; i < _width; i++ ) {
		for( int j = 0; j < _height; j++ ) {
			if( _theCells[i][j]->getBase() ) {
				computeStoppable( _theCells[i][j]->getBase() );
			}
		}
	}
}

void GenericMap::computeStoppable( GenericBase * base )
{
	if( base->getCell() ) {
		int col = base->getCell()->getCol() - base->getDoorCol();
		int row = base->getCell()->getRow() - base->getDoorRow();
		for( int i = 0; i < 5; i++ ) {
			for( int j = 0; j < 5; j++ ) {
				if( base->getDispo( i, j ) == 2 ) {
					if( ( row + i >= 0 ) && ( col + j >= 0 )
					&& ( row + i < _height ) && ( col + j < _width ) ) {
						//logDD("stop %d %d", row+i, col+j );
						_theCells[row+i][col+j]->setStoppable( false );
					}
				}
			}
		}
	}
}

int GenericMap::computeMinimalNextCost( GenericLord * lord )
{
	int ret = 0;
	int cost = 0;

	GenericCell * cell = lord->getCell();

	int row = cell->getRow();
	int col = cell->getCol();

	if( ( row > 0 ) && ( col > 0 ) ) {
		cost = lord->computeCostMvt( cell, at( row - 1, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( row > 0 ) {
		lord->computeCostMvt( cell, at( row - 1, col ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row > 0 ) && ( col < _width - 1 ) ) {
		lord->computeCostMvt( cell, at( row - 1, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( col > 0 ) {
		lord->computeCostMvt( cell, at( row, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( col < _width - 1 ) {
		lord->computeCostMvt( cell, at( row, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row < _height - 1 ) && ( col > 0 ) ) {
		lord->computeCostMvt( cell, at( row + 1, col - 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( row < _height - 1 ) {
		lord->computeCostMvt( cell, at( row + 1, col ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	if( ( row < _height - 1 ) && ( col < _width - 1 ) ) {
		lord->computeCostMvt( cell, at( row + 1, col + 1 ) );
	}
	if( ( ret == 0 ) || ( ( cost > 0 ) && cost < ret ) ) {
		ret = cost;
	}

	return ret;
}


void GenericMap::cleanData()
{
	uint i,j;
		
	for( i = 0; i < (uint) _height; i++ ) {
		for( j = 0; j < (uint) _width; j++ ) {
			if( _theCells[i][j]->getLord() ) {
				_theCells[i][j]->setLord( 0 );
			}
			if( _theCells[i][j]->getBuilding() ) {
				_theCells[i][j]->setBuilding(0);
			}
			if( _theCells[i][j]->getBase() )	{
				_theCells[i][j]->setBase(0);
			}
			if( _theCells[i][j]->getArtefact() ) {
				_theCells[i][j]->setArtefact(0);
			}
			if( _theCells[i][j]->getCreature() ) {
				_theCells[i][j]->setCreature(0);
			}
			if( _theCells[i][j]->getTransition() ) {
				_theCells[i][j]->setTransition(0);
				_theCells[i][j]->setTransitionCellType( 0 );
			}
			if( _theCells[i][j]->getType() ) {
				_theCells[i][j]->setType(0);
			}
			if( _theCells[i][j]->getCoeff() ) {
				_theCells[i][j]->setCoeff(0);
			}
			if( _theCells[i][j]->getDecoration() ) {
				_theCells[i][j]->setDecoration(0);
			}
			if( _theCells[i][j]->getEvent() ) {
				_theCells[i][j]->setEvent(0);
			}
		}
	}


}

