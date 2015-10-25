/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.cpp
** draw a hero on the map
**
** Version : $Id: graphicalLord.cpp,v 1.13 2002/04/28 19:30:06 audoux Exp $
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
#include <qrect.h>
#include <qimage.h>
#include <qcolor.h>
#include <qlist.h>
// application specific include
#include "client/imageTheme.h"
#include "common/log.h"
#include "conf.h"


/** rtti number for GraphicalLord class */
const int GraphicalLord::RTTI = 1234;

extern QString IMAGE_PATH;

extern ImageTheme ImageTheme;

/** add comments here */
GraphicalLord::GraphicalLord( QCanvas * canvas )
	: QCanvasSprite( ImageTheme.lords, canvas ) 
{
 	collisions( true );
 	setFrame( 0 );
	setZ( CAN_LORD );
	show();
	QList<QPixmap> listPixmapLord;	
	listPixmapLord.setAutoDelete( true );
	listPixmapLord.append( new QPixmap( IMAGE_PATH + "units/lord.png" ) );
	QList<QPoint> listPointLord;
	listPointLord.setAutoDelete( true );
	listPointLord.append( new QPoint(0,0) );
	QCanvasPixmapArray * tryi = new QCanvasPixmapArray( listPixmapLord, listPointLord );
	setSequence( tryi );
}

GraphicalLord::~GraphicalLord()
{

}

/** set the hero on the cell */
void GraphicalLord::setPosition( Cell *cell )
{
	QRect rect = cell->boundingRect();
	move( rect.x() , rect.y() + rect.height() - boundingRect().height() );
	
	canvas()->update();
	setZ( CAN_LORD + cell->getRow() );
	setVisible( true );
}

void GraphicalLord::goTo( GenericCell *cell )
{
      	QRect rect = ( (Cell *)cell )->boundingRect();
	move( rect.x() , rect.y() + rect.height() - boundingRect().height() );
	
	canvas()->update();
	setZ( CAN_LORD + cell->getRow() );
}

void GraphicalLord::advance( int stage )
{

}

void GraphicalLord::setColor( QColor color )
{
	QImage ima = image()->convertToImage();
	uint *p;
	for( int i = 0; i < 8; i++ ) 
		for( int j = 0; j < 8; j++ ) {
			p = (uint *)ima.scanLine(i) + j;
			*p = color.rgb();
		}
	image()->convertFromImage( ima );
}

QCanvasPixmap* GraphicalLord::imageAdvanced() const
{
	return image();
}


