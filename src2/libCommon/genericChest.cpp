/****************************************************************
**
** Attal : Lords of Doom
**
** genericChest.cpp
** Class for managing treasury chests on map
**
** Version : $Id: genericChest.cpp,v 1.5 2005/12/11 10:34:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2003
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

#include "genericChest.h"

// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"

GenericChest::GenericChest()
{
}

GenericChest::~GenericChest()
{
}

void GenericChest::addParam( uint value )
{
	_params.append( value );
}

uint GenericChest::getParamNumber()
{
	return _params.count();
}

uint GenericChest::getParam( uint num )
{
	uint ret = 0;

	if( (int)num < _params.count() ) {
		ret = _params.at( num );
	}
	return ret;
}


void GenericChest::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<chest>" << endl;

	uint nbParam = _params.count();
	for( uint i = 0; i < nbParam; i++ ) {
		indentation( ts, indent+1 );
		*ts << "<param>" << _params.at( i ) << "</param>" << endl;
	}

	indentation( ts, indent );
	*ts << "</chest>" << endl;
	*ts << flush;
}

