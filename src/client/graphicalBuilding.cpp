/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.cpp
** draw a hero on the map
**
** Version : $Id: graphicalBuilding.cpp,v 1.14 2002/10/20 08:54:26 audoux Exp $
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

#include "graphicalBuilding.h"

// generic include files
// include files for QT
#include <qrect.h>
// application specific includes
#include "conf.h"
#include "common/log.h"
#include "client/imageTheme.h"

/** rtti number for GraphicalLord class */
const int GraphicalBuilding::RTTI = 1000;

extern ImageTheme ImageTheme;

//
// ----- GraphicalBuilding -----
//

/** add comments here */
GraphicalBuilding::GraphicalBuilding( QCanvas * canvas )
	: QCanvasSprite( ImageTheme.buildings[ 0 ], canvas )
{
 	collisions( true );
 	setFrame( 0 );
	setAnimated( true );
	setZ( CAN_BUILDING );
	show();
	_frame = 0;
	_nbFrame = 1;
	_freq = 1;
	_clock = 0;
}

GraphicalBuilding::GraphicalBuilding( QCanvasPixmapArray * array, QCanvas * canvas )
	: QCanvasSprite( array, canvas )
{
 	collisions( true );
 	setFrame( 0 );
	setZ( CAN_BUILDING );
	show();
}

/** set the lord on the cell */
void GraphicalBuilding::setPosition( Cell * cell, int offsetRow, int offsetCol )
{
	QRect rect = cell->boundingRect();
	move( rect.x() + ( offsetCol * rect.width() ),
	      rect.y() + rect.height() - boundingRect().height() + ( offsetRow * rect.height() ) );

	canvas()->update();
}

void GraphicalBuilding::advance( int stage )
{
	_clock++;
	if( _clock == _freq ) {
		_clock = 0;
		_frame++;
		if( _frame == _nbFrame ) {
			_frame = 0;
		}
		setFrame( _frame );
	}
}

QCanvasPixmap* GraphicalBuilding::imageAdvanced() const
{
	/// logDD("imageAdvanced");
	return image();
}

void GraphicalBuilding::setType( int type )
{
	setSequence( ImageTheme.buildings[ type ] );
	setFrame( 0 );
}

void GraphicalBuilding::setAnimation( int nbFrame, int freq )
{
	_nbFrame = nbFrame;
	_freq = freq;
	if( _nbFrame == 1 ) {
		setAnimated( false );
	} else {
		setAnimated( true );
	}
}

//
// ----- GraphicalBase -----
//

GraphicalBase::GraphicalBase( QCanvas * canvas )
	: GraphicalBuilding( ImageTheme.bases, canvas )
{
 	collisions( true );
 	setFrame( 0 );
	setZ( CAN_BUILDING );
	show();
}

void GraphicalBase::setRace( int race )
{
	setFrame( race );
}
