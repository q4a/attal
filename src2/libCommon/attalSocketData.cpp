/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocketData.cpp
** socket data for Attal over network
**
** Version : $Id: attalSocketData.cpp,v 1.2 2007/09/10 19:59:45 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 31/10/2000
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

#include "attalSocketData.h"

 
// generic include files
//#include <algorithm>

// include files for QT
// application specific includes

//
// ----- AttalSocketData -----
//

AttalSocketData::AttalSocketData()
{
	_lenOut = 4;
	_lenIn = 3;
}
	
AttalSocketData::~AttalSocketData()
{
}

void AttalSocketData::init( uchar c1, uchar c2, uchar c3 )
{
	_bufOut[1] = c1;
	_bufOut[2] = c2;
	_bufOut[3] = c3;
}

void AttalSocketData::appendInt( int i )
{
	memcpy( _bufOut + _lenOut, &i, 4 );
	_lenOut += 4;
}

void AttalSocketData::appendChar( unsigned char c )
{
	_bufOut[_lenOut] = c;
	_lenOut++;
}

void AttalSocketData::reReadData()
{
	_lenIn = 3;
}

int AttalSocketData::readInt()
{
	int ret;
	memcpy( &ret, _bufIn + _lenIn, 4 );
	_lenIn += 4;
	return ret;
}

unsigned char AttalSocketData::readChar()
{
	unsigned char ret = _bufIn[_lenIn];
	_lenIn++;
	return ret;
}

void AttalSocketData::setData( AttalSocketData * data )
{
	for( int i = 0; i < 256; i++ ) {
		_bufIn[i] = data->_bufIn[i];
		_bufOut[i] = data->_bufOut[i];
	}

	_lenIn = data->_lenIn;
	_lenOut = data->_lenOut;
}

AttalSocketData AttalSocketData::getData()
{
	AttalSocketData data;

	for( int i = 0; i < 256; i++ ) {
		data._bufIn[i] = _bufIn[i];
		data._bufOut[i] = _bufOut[i];
	}

	 data._lenIn = _lenIn;
	 data._lenOut = _lenOut;

	 return data;
}

