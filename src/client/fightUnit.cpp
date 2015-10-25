/****************************************************************
**
** Attal : Lords of Doom
**
** fightUnit.cpp
** Manage Units
**
** Version : $Id: fightUnit.cpp,v 1.10 2002/09/08 21:44:03 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/12/2000
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

#include "fightUnit.h"


#include <qimage.h>
// generic include files
// include files for QT
// application specific include files
#include "common/creature.h"
#include "common/log.h"
#include "client/fightCell.h"
#include "client/imageTheme.h"

extern ImageTheme ImageTheme;

FightUnit::FightUnit( QCanvas * canvas )
	: GraphicalFightUnit( canvas ),
	  GenericFightUnit()
{

}

FightUnit::FightUnit( QCanvas * canvas, GenericFightUnit * unit, bool mirror )
	: GraphicalFightUnit( canvas ),
	  GenericFightUnit( *unit )
{
	QCanvasPixmapArray * sequence = (*ImageTheme.creatures[computeSequence( unit )])[ (mirror) ? 1 : 0 ];

	if( sequence ) {
		GraphicalFightUnit::setSequence( sequence );
	}
}

/*!

*/

int FightUnit::hit( int nb )
{
	int ret = GenericFightUnit::hit( nb );
	if( _number == 0 ) {
		GraphicalFightUnit::setDestroyed( true );
	}

	return ret;
}


/*!

*/

void FightUnit::goTo( FightCell * cell )
{
	if( _currentFightCell )
		_currentFightCell->setUnit( NULL );

	GraphicalFightUnit::goTo( (GraphicalFightCell *)cell );
	GenericFightUnit::goTo( (GenericFightCell *)cell );
	cell->setUnit( (GenericFightUnit *)this );
}



