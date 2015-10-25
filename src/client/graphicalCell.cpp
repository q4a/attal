/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalCell.cpp
** this class draws an animated cell
**
** Version : $Id: graphicalCell.cpp,v 1.14 2002/11/01 10:50:58 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

#include "graphicalCell.h"


// generic include files

// include files for QT
#include <qcolor.h>
#include <qpointarray.h>
#include <qcanvas.h>
#include <qapplication.h>
#include <qlist.h>
// application specific include
#include "client/cell.h"
#include "client/imageTheme.h"

#include "common/dataTheme.h"
#include "common/genericDecoration.h"
#include "common/log.h"

#include "conf.h"

/** size in pixel of a cell */
const int GraphicalCell::_size = 30 ;

/** rtti number for GraphicalCell class */
const int GraphicalCell::RTTI = 2345;

extern QString IMAGE_PATH;
extern ImageTheme ImageTheme;
extern DataTheme DataTheme;


//
// ----- GraphicalCell -----
//

/** create an animated cell from pixmaps at position row,col */
GraphicalCell::GraphicalCell( int row, int col , QCanvas *canvas)
	: QCanvasSprite( ImageTheme.cells, canvas )
{	
	collisions( true );
	setFrame( 0 );	
	move( _size*col, _size*row );
	setZ( CAN_GROUND );
	show();
}

GraphicalCell::~GraphicalCell()
{

}

/** return if a point is on the cell */
bool GraphicalCell::hit( const QPoint &p ) const
{
	return boundingRect().contains( p );
}

//
// ----- Transition -----
//

Transition::Transition( Cell * parent, QCanvas *canvas)
	: GraphicalCell( parent->getRow(), parent->getCol(), canvas )
{
	_parent = parent;
}

void Transition::setTransition( int transition, int type )
{
	setZ( CAN_TRANS );

	// XXX: Ugly
	if( type == 0) {
		type = 1;
	}
	setSequence( ImageTheme.transition[type-1] );
	
	setFrame( transition - 1 );	
}

//
// ----- Decoration -----
//

Decoration::Decoration( Cell * parent, QCanvas *canvas)
	: GraphicalCell( parent->getRow(), parent->getCol(), canvas )
{
	_parent = parent;
	setZ( CAN_DECO );
	setSequence( ImageTheme.decoration );
	setFrame( 0 );
}

void Decoration::setDecoration( int decoration )
{
	setFrame( decoration - 1 );

	GenericDecoration * decor = DataTheme.decorations.at( decoration - 1 );
	if( ( decor->getDispoWidth() > 1 ) || ( decor->getDispoHeight() > 1 ) ) {
		//set pos
		//GraphicalBase::setPosition( (Cell *)_map->at( row, col ), ( 4 - getDoorRow() ), ( 0 - getDoorCol() ) );

		int offsetRow = decor->getDispoHeight() - ( 1 + decor->getMainRow() );
		int offsetCol = 0 - decor->getMainCol();

		QRect rect = _parent->boundingRect();
		move( rect.x() + ( offsetCol * rect.width() ),
		rect.y() + rect.height() - boundingRect().height() + ( offsetRow * rect.height() ) );

		canvas()->update();
	}
}
