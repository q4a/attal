/****************************************************************
**
** Attal : Lords of Doom
**
** playingTroops.cpp
** manage playing troops
**
** Version : $Id: playingTroops.cpp,v 1.1 2006/08/30 18:56:28 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/05/2001
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


#include "playingTroops.h"

PlayingTroops::PlayingTroops( bool order )
{
	_order = order;
	//setAutoDelete( false );
}

GenericFightUnit * PlayingTroops::take()
{
	GenericFightUnit * ret = last();
	removeLast();
	return ret;
}

int PlayingTroops::compareItems( GenericFightUnit * unit1, GenericFightUnit * unit2 )
{
	int ret = 0;

	///GenericFightUnit * unit1 = (GenericFightUnit *)i1;
	///GenericFightUnit * unit2 = (GenericFightUnit *)i2;

	if( unit1->getMove() > unit2->getMove() ) {
		ret = 1;
	} else if( unit1->getMove() < unit2->getMove() ) {
		ret = -1;
	}

	if( !_order ) {
		ret = -ret;
	}

	return ret;
}
