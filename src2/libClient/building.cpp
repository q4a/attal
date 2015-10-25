/****************************************************************
**
** Attal : Lords of Doom
**
** building.cpp
** manages building
**
** Version : $Id: building.cpp,v 1.12 2008/06/12 14:12:49 lusum Exp $
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
#include <QMessageBox>
// application specific include
#include "libCommon/genericPlayer.h"

#include "libClient/displayBase.h"
#include "libClient/map.h"


//
// ----- Base -----
//

Base::Base( Map * map, int race )
  : GraphicalBase( map ),
    GenericBase( race )
{
	// internal comments
	_map = map;
	GraphicalBase::setRace( race );
}

void Base::setPosition( GenericCell * cell )
{
	setPosition( cell->getRow(), cell->getCol() );
}

void Base::setPosition( int row, int col )
{
	GenericBase::setPosition( (GenericCell *)_map->at( row, col ) );
	// dispositions start from top/left, starting with index 0, cell start from bottom/right
	// so getDoorCol() is the orizontal offset, orizontal offset is dispoHeight - doorRow
	GraphicalBase::setPosition( (GenericCell *)_map->at( row, col ), ( getDoorRow()-getDispoHeight() + 1 ), ( - getDoorCol() ) );
	//_map->at( row, col )->setBase( this );
}


void Base::enter( GenericLord * lord )
{
	GenericBase::enter( lord );
	TRACE("Base::enter You are in a castle, lord %p", lord );
}

void Base::setRace( int race )
{
	GenericBase::setRace( race );
	GraphicalBase::setRace( race );
}

void Base::setOwner( GenericPlayer * player )
{
	GenericBase::setOwner( player );
	GraphicalBase::setOwner( player );
}

//
// ----- Building -----
//

Building::Building( QGraphicsScene * canvas , int type )
  : GraphicalBuilding( canvas ),
    GenericBuilding( type )
{
	GraphicalBuilding::setImageSequence( type );
	setAnimation( GenericBuilding::getNbFrame(), GenericBuilding::getAnimFreq() );
}

void Building::setPosition( GenericCell * cell )
{
	GenericBuilding::setPosition( cell );
	// dispositions start from top/left, starting with index 0, cell start from top/left
	// so - getDoorCol() is the orizontal offset, vertical offset is dispoHeight + 1 - doorRow
	TRACE("void Building::setPosition( GenericCell * cell ) row %d, cell col %d, getDoorRow %d, getDispoHeight %d, getDispoWidth %d, getDoorCol %d",cell->getRow(), cell->getCol(), getDoorRow(), getDispoHeight(), getDispoWidth(), getDoorCol());

	GraphicalBuilding::setPosition( cell ,  (  getDispoHeight()-1 - getDoorRow() ),  - getDoorCol() );
}

void Building::setOwner( GenericPlayer * player )
{
	GenericBuilding::setOwner( player );
	GraphicalBuilding::setOwner( player );
}
