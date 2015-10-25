/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalFightUnit.cpp
** draw a unit on the fight map
**
** Version : $Id: graphicalFightUnit.cpp,v 1.6 2002/09/01 17:04:30 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2000
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

#include "graphicalFightUnit.h"


// generic include files

// include files for QT
#include <qrect.h>
#include <qbrush.h>
// application specific include
#include "client/imageTheme.h"
#include "common/log.h"
#include "conf.h"


/** rtti number for GraphicalFightUnit class */
const int GraphicalFightUnit::RTTI = 2234;

extern ImageTheme ImageTheme;

/** add comments here */
GraphicalFightUnit::GraphicalFightUnit( QCanvas * canvas )
	: QCanvasSprite( (*ImageTheme.creatures[0])[0], canvas )
{
 	collisions( true );
 	setFrame( 0 );
	setZ( CAN_LORD );
	show();

	/*QBrush brush( yellow );
	_numBox = new QCanvasRectangle( canvas );
	_numBox->setBrush( brush );
	_numBox->collisions( true );
	_numBox->setZ( CAN_ARROW );
	_numBox->setSize( 50, 25 );
	_numBox->show();*/
}

/** set the hero on the cell */
void GraphicalFightUnit::setPosition( GraphicalFightCell *cell )
{
	QRect rect = cell->boundingRect();
	//move( rect.x() + (rect.width()  - boundingRect().width() )/2 ,
	//      rect.y() + (rect.height() - boundingRect().height())/2 );
	move( rect.x(), rect.y() + ( rect.height() ) - boundingRect().height() );
	//_numBox->move( x(), y() + boundingRect().height() - 25 );
	canvas()->update();
}

void GraphicalFightUnit::goTo( GraphicalFightCell *cell )
{
	// XXX TODO : animation
	setPosition( cell );

}

void GraphicalFightUnit::advance(int stage)
{

}

QCanvasPixmap* GraphicalFightUnit::imageAdvanced() const
{
	return image();
}

void GraphicalFightUnit::setDestroyed( bool state )
{
	if( state ) {
		setSequence( ImageTheme.deadCreature );
		setFrame( 0 );
	}
}


/*! Destroys the object and frees any allocated resources.

*/

GraphicalFightUnit::~GraphicalFightUnit()
{
	logDD("delete GraphicalFightUnit");
}
