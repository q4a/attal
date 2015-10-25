/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapCreature.cpp
** Manages creatures on map
**
** Version : $Id: genericMapCreature.cpp,v 1.2 2002/09/26 21:11:56 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/09/2002
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

#include "genericMapCreature.h"

// generic include files
// include files for QT
// application specific include files
#include "common/dataTheme.h"

extern DataTheme DataTheme;

GenericMapCreature::GenericMapCreature()
{
	_creature = 0;
	_stacks.setAutoDelete( true );
}

void GenericMapCreature::setCreature( int race, int level )
{
	_creature = DataTheme.creatures.at( race, level );
}


int GenericMapCreature::getRace()
{
	int ret = 0;

	if( _creature ) {
		ret = _creature->getRace();
	}

	return ret;
}

int GenericMapCreature::getLevel()
{
	int ret = 0;

	if( _creature ) {
		ret = _creature->getLevel();
	}

	return ret;
}

int GenericMapCreature::getCategoryNumber()
{
	int ret = 0;

	/// XXX: to change
	ret = 1;

	return ret;
}

void GenericMapCreature::addStack( int number )
{
	_stacks.append( new int( number ) );
}

int GenericMapCreature::getStack( int num )
{
	int ret = 0;

	if( (uint)num < _stacks.count() ) {
		ret = * ( _stacks.at( num ) );
	}

	return ret;
}



