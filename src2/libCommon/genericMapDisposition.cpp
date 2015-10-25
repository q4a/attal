/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapDisposition.cpp
** Manage disposition on the map of the building, base...
**
** Version : $Id: genericMapDisposition.cpp,v 1.6 2005/10/30 22:20:57 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/05/2004
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

#include "genericMapDisposition.h"

// generic include files
#include <algorithm>
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"

GenericMapDisposition::GenericMapDisposition()
: _dispo( 0 ), _doorRow( 0 ), _doorCol( 0 ), _height( 1 ), _width( 1 )
{
	_dispo = new DispositionType * [1];
	_dispo[0] = new DispositionType[1];
	_dispo[0][0] = FREE;
}

GenericMapDisposition::~GenericMapDisposition()
{
	clear();
}

void GenericMapDisposition::clear()
{
	if( _dispo ) {
		for( uint i = 0; i < _height; i++ ) {
			delete [] _dispo[i];
		}
		delete [] _dispo;
	}
	_dispo = 0;
}

void GenericMapDisposition::init( uint height, uint width )
{
	uint i, j;

	clear();

	_dispo = new DispositionType * [ height ];
	for( i = 0; i < height; i++ ) {
		_dispo[i] = new DispositionType[ width ];
		for( j = 0; j < width; j++ ){
			_dispo[i][j] = FREE;
		}
	}
	_height = height;
	_width = width;
}

void GenericMapDisposition::resize( uint newHeight, uint newWidth )
{
	uint i, j;
	uint oldHeight = _height;
	uint oldWidth = _width;
	DispositionType ** dispo;
	
	dispo = new DispositionType * [ _height ];
	for( i = 0; i < _height; i++ ) {
		dispo[i] = new DispositionType[ _width ];
		for( j = 0; j < _width; j++ ){
			dispo[i][j] = _dispo[i][j];
		}
	}
	if( _dispo ) {
		for( i = 0; i < _height; i++ ) {
			delete [] _dispo[i];
		}
		delete [] _dispo;
	}
	_height = newHeight;
	_width = newWidth;
	
	_dispo = new DispositionType * [ _height ];
	for( i = 0; i < _height; i++ ) {
		_dispo[i] = new DispositionType[ _width ];
		for( j = 0; j < _width; j++ ){
			_dispo[i][j] = FREE;
		}
	}
	for( i = 0; i < std::min( newHeight, oldHeight ); i++ ) {
		for( j = 0; j < std::min( newWidth, oldWidth ); j++ ) {
			_dispo[ newHeight - i - 1 ][ j ] = dispo[ oldHeight - i - 1 ][ j ];
		}
	}
	for( i = 0; i < oldHeight; i++ ) {
		delete [] dispo[ i ];
	}
	delete [] dispo;
}

bool GenericMapDisposition::save( QTextStream * ts, uint indent )
{
	uint i, j;
	indentation( ts, indent );
	*ts << "<disposition height=\"" << _height << "\" width=\""  << _width << "\">" << endl;

	for( i = 0; i < _height; i++ ) {
		indentation( ts, indent + 1 );
		for( j = 0; j < _width; j++ ) {
			*ts << (int)_dispo[i][j];
			if( j != ( _width - 1 ) ) {
				*ts << " ";
			}
		}
		*ts << endl;
	}

	indentation( ts, indent );
	*ts << "</disposition>" << endl;

	return true;
}

void GenericMapDisposition::setDisposition( uint row, uint col, DispositionType disposition )
{
	if( ( row >= _height ) || ( col >= _width ) ) {
		return;
	}

	_dispo[row][col] = disposition;
	if( disposition == DOOR ) {
		_doorCol = col;
		_doorRow = row;
	}
}
