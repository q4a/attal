/****************************************************************
**
** Attal : Lords of Doom
**
** bonus.cpp
** Displays bonus on map
**
** Version : $Id: bonus.cpp,v 1.9 2007/05/13 17:15:21 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/08/2003
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

#include "bonus.h"

// generic include files
// include files for QT
// application specific include files
#include "conf.h"

#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/imageTheme.h"
#include "libClient/lord.h"

const int Bonus::RTTI = Type;

Bonus::Bonus( QGraphicsScene * canvas )
	:AttalSprite( ImageTheme.bonus[0], canvas )
{
       setFrame( 0 );
       setZValue( CAN_ARTEFACT );
}

void Bonus::setCell( GenericCell * cell )
{
	TRACE("Bonus::setCell");
	setPos( cell->getCol() * DataTheme.tiles.getWidth() , ( cell->getRow() + 1 ) * DataTheme.tiles.getHeight() - boundingRect().height() );
}

void Bonus::setupBonus()
{
	setFrame( 0 );
	setSequence( ImageTheme.bonus[ (uint)_type ] );
	if( _type == BonusResource ) {
		setFrame( getParam( 0 ) );
	}
}

int Bonus::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}
