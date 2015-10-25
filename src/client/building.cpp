/****************************************************************
**
** Attal : Lords of Doom
**
** building.cpp
** manages building
**
** Version : $Id: building.cpp,v 1.19 2002/11/08 16:53:55 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2000
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

#include "building.h"


// generic include files
// include files for QT
#include <qmessagebox.h>
// application specific include
#include "client/displayBase.h"
#include "client/map.h"


//
// ----- Base -----
//

Base::Base( Map * map )
  : GraphicalBase( map ),
    GenericBase()
{
	// internal comments
	_map = map;
}

void Base::setPosition( GenericCell * cell )
{
	setPosition( cell->getRow(), cell->getCol() );
}

void Base::setPosition( int row, int col )
{
	GenericBase::setPosition( (GenericCell *)_map->at( row, col ) );
	GraphicalBase::setPosition( (Cell *)_map->at( row, col ), ( 4 - getDoorRow() ), ( 0 - getDoorCol() ) );
	//_map->at( row, col )->setBase( this );
}


void Base::enter( GenericLord * lord )
{
	GenericBase::enter( lord );
	logDD( "You are in a castle" );

}

void Base::setRace( int race )
{
	GenericBase::setRace( race );
	GraphicalBase::setRace( race );
}


//
// ----- Building -----
//

Building::Building( QCanvas * canvas )
  : GraphicalBuilding( canvas ),
    GenericBuilding()
{
	//setFrame( 0 );
	//setAnimation( GenericBuilding::getNbFrame(), GenericBuilding::getAnimFreq() );
	//setType(
}

void Building::setPosition( GenericCell * cell )
{
	GenericBuilding::setPosition( cell );
	GraphicalBuilding::setPosition( (Cell *)cell );
	//cell->setBuilding( this );
}

void Building::setType( int type )
{
	GenericBuilding::setType( type );
	GraphicalBuilding::setType( type );
	setAnimation( GenericBuilding::getNbFrame(), GenericBuilding::getAnimFreq() );
}

