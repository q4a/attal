/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.cpp
** draw a hero on the map
**
** Version : $Id: graphicalLord.cpp,v 1.26 2008/04/10 20:11:57 lusum Exp $
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

#include "graphicalLord.h"

 
// generic include files

// include files for QT
#include <QColor>
#include <QRect>
#include <QPixmap>
// application specific include
#include "conf.h"

#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"

/** rtti number for GraphicalLord class */
const int GraphicalLord::RTTI = Type;

GraphicalLord::GraphicalLord( QGraphicsScene * canvas )
	: AttalSprite( ImageTheme.lords, canvas ) 
{
 	setFrame( 0 );
	setZValue( CAN_LORD );
	setSequence( ImageTheme.lords );
	
	_flag = new Flag( canvas );
}

GraphicalLord::~GraphicalLord()
{
	if( _flag ) {
		delete _flag;
	}
}

/** set the hero on the cell */
void GraphicalLord::setCell( GenericCell *cell )
{
	if( cell ) {
		TRACE("GraphicalLord::setCell"); 

		setPos( cell->getCol() * DataTheme.tiles.getWidth() , (cell->getRow() + 1) * DataTheme.tiles.getHeight() - boundingRect().height() );
		if( _flag ) {
			_flag->setPos( cell->getCol() * DataTheme.tiles.getWidth() , (cell->getRow() + 1) * DataTheme.tiles.getHeight() - boundingRect().height() );
			_flag->setZValue( CAN_LORD + cell->getRow() + 1 );
		}

		setZValue( CAN_LORD + cell->getRow() );
		setVisible(true);
	} else {
		setVisible(false);
	}
}

void GraphicalLord::setVisible( bool state )
{
	AttalSprite::setVisible( state );
	if( _flag ) {
		_flag->setVisible( state );
	}
}

void GraphicalLord::advance( int /*stage*/ )
{
	/// XXX: TODO
	//logEE( "not yet implemented" );
}

void GraphicalLord::setOwner( GenericPlayer * player )
{
	_flag->setOwner( player );
	_imabk = image().toImage();
}

void GraphicalLord::highlight( bool state)
{
	if( state ) {
		setFrame(1);
	} else {
		setFrame(0);
	}

}

/*
void GraphicalLord::setColor( QColor color )
{
	QImage ima = image()->toImage();
	uint *p;
	for( int i = 0; i < 8; i++ ) 
		for( int j = 0; j < 8; j++ ) {
			p = (uint *)ima.scanLine(i) + j;
			*p = color.rgb();
		}
	image()->convertFromImage( ima );
}*/

QPixmap GraphicalLord::imageAdvanced() 
{
	return image();
}

int GraphicalLord::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}
