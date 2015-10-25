/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalCell.cpp
** this class draws an animated cell
**
** Version : $Id: graphicalCell.cpp,v 1.18 2013/11/18 20:54:07 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier - Sardi Carlo
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
#include <QApplication>
#include <QColor>
// application specific include
#include "libClient/cell.h"
#include "libClient/imageTheme.h"

#include "libCommon/dataTheme.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/log.h"

#include "conf.h"

/** rtti number for Decoration class */
const int Decoration::RTTI = Type;

//
// ----- GraphicalCell -----
//

/** create an animated cell from pixmaps at position row,col */
GraphicalCell::GraphicalCell( int /*row*/, int /*col*/ , QGraphicsScene *canvas)
	: AttalSprite( ImageTheme.cells[0], canvas )
{
	hide(); // HACK make invisible to allow drawBackground to handle terrain/transition drawing
}

//
// ----- Decoration -----
//

Decoration::Decoration( Cell * parent, QGraphicsScene *canvas)
	: AttalSprite( ImageTheme.cells[0], canvas ) // HACK why can't a NULL value for frames be passed?
{
	_parent = parent;
	setZValue( CAN_DECO );
	setFrame( 0 );
	setPos( QPointF( parent->getCol() * DataTheme.tiles.getWidth(), parent->getRow() * DataTheme.tiles.getHeight() ) );
}

/** return if a point is on the cell */
bool Decoration::hit( const QPointF &p ) const
{
	int ix = (int)(p.x())-int(x());
	int iy = (int)(p.y())-int(y());
	    
	return ( ix < (int)DataTheme.tiles.getWidth() &&  iy < (int)DataTheme.tiles.getHeight() ) ;
}

void Decoration::setDecoration( uint group, uint item )
{
	setSequence( ImageTheme.decoration[ group-1 ] );
	setFrame( item );

	DecorationGroup * decoGroup = DataTheme.decorations.at( group );
	
	GenericDecoration * decor = decoGroup->at( item );
	if( ( decor->getWidth() > 1 ) || ( decor->getHeight() > 1 ) ) {
		int offsetRow = decor->getHeight() - ( 1 + decor->getDoorRow() );
		int offsetCol = 0 - decor->getDoorCol();

		setPos( ( _parent->getCol() + offsetCol ) * DataTheme.tiles.getWidth() ,
				( _parent->getRow() + offsetRow + 1) * DataTheme.tiles.getHeight() - boundingRect().height());
	}
}
