/****************************************************************
**
** Attal : Lords of Doom
**
** genericEvent.cpp
** this is a template for all .cpp files
**
** Version : $Id: genericEvent.cpp,v 1.4 2002/12/22 19:51:10 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 17/12/2002
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

#include "genericEvent.h"

// generic include files
// include files for QT
// application specific include files


/** add comments here */
GenericEvent::GenericEvent()
{
	_type = (EventType)0;
	_params.setAutoDelete( true );
	_cell = 0;
}

uint GenericEvent::getParamNumber()
{
	return _params.count();
}

int GenericEvent::getParam( uint num )
{
	int ret = 0;

	if( num < _params.count() ) {
		ret = * ( _params.at( num ) );
	}

	return ret;
}

void GenericEvent::addParam( int val )
{
	_params.append( new int( val ) );
}

void GenericEvent::replaceParam( uint num, int val )
{
	if( num < _params.count() ) {
		* ( _params.at( num ) ) = val;
	}
}

bool GenericEvent::isVisible()
{
	bool ret = false;

	if( getParam( 0 ) != 0 ) {
		ret = true;
	}

	return ret;
}

