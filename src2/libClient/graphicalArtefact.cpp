/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalArtefact.cpp
** Display artefacts on the map
**
** Version : $Id: graphicalArtefact.cpp,v 1.14 2007/05/13 17:56:04 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/01/2002
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

#include "graphicalArtefact.h"
 
// generic include files
// include files for QT
// application specific include files
#include "conf.h"

#include "libCommon/genericCell.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"
#include "libClient/lord.h"

const int Artefact::RTTI = Type;

Artefact::Artefact( QGraphicsScene * canvas )
	:AttalSprite( ImageTheme.artefacts, canvas )
{
	TRACE("Artefact::constructor");
	
	setFrame( 0 );
	setZValue( CAN_ARTEFACT );
}

void Artefact::setCell( GenericCell * cell )
{
	TRACE("Artefact::setCell");
	setPos( cell->getCol() * DataTheme.tiles.getWidth() , (cell->getRow() + 1) * DataTheme.tiles.getHeight() - boundingRect().height() );
}

void Artefact::setLord( GenericLord * /*lord*/ )
{
	/// XXX: remove arg ?
	//GenericArtefact::setLord( lord );
	hide(); // TODO use delete this; instead?
}

void Artefact::setType( uint type )
{
	TRACE("Artefact::setType");
	GenericArtefact::setType( type );
	setFrame( type );
}

int Artefact::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}
