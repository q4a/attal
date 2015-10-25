/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.cpp
** draw bases and buildings on the map
**
** Version : $Id: graphicalBuilding.cpp,v 1.14 2008/06/12 14:12:49 lusum Exp $
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
#include <QRectF>
// application specific includes
#include "conf.h"

#include "libCommon/attalSettings.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"

/** rtti number for GraphicalLord class */
const int GraphicalBuilding::RTTI = Type;

//
// ----- GraphicalBuilding -----
//

/** add comments here */
GraphicalBuilding::GraphicalBuilding( QGraphicsScene * canvas )
	: AttalSprite( ImageTheme.buildings[ 0 ], canvas )
{
 	setFrame( 0 );
	setAnimated( true );
	setZValue( CAN_BUILDING );
	_frame = 0;
	_nbFrame = 1;
	_freq = 1;
	_clock = 0;
	
	_flag = new Flag( canvas );
	_flag->hide();
}

GraphicalBuilding::GraphicalBuilding( QList<QPixmap> * array, QGraphicsScene * canvas )
	: AttalSprite( array, canvas )
{
 	setFrame( 0 );
	setZValue( CAN_BUILDING );
	_frame = 0;
	_nbFrame = 1;
	_freq = 1;
	_clock = 0;
	
	_flag = new Flag( canvas );
	_flag->hide();
}

GraphicalBuilding::~GraphicalBuilding()
{
	if( _flag ) {
		delete _flag;
	}
}


void GraphicalBuilding::setPosition( GenericCell * cell, int offsetRow, int offsetCol )
{
  TRACE("void GraphicalBuilding::setPosition( GenericCell * cell row %d, col %d, int offsetRow %d, int offsetCol %d )", cell->getRow(), cell->getCol(), offsetRow, offsetCol);

	setPos( ( cell->getCol() + offsetCol ) * DataTheme.tiles.getWidth() ,
		( cell->getRow() + offsetRow + 1 ) * DataTheme.tiles.getHeight() - boundingRect().height());
	if( _flag ) {
		_flag->setPos( ( cell->getCol() + offsetCol ) * DataTheme.tiles.getWidth() ,
			( cell->getRow() + offsetRow + 1 ) * DataTheme.tiles.getHeight() - boundingRect().height() );
		_flag->setZValue( CAN_LORD + cell->getRow() + 1 );
		_flag->hide();
	}
}

void GraphicalBuilding::advance( int /*stage*/ )
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

QPixmap GraphicalBuilding::imageAdvanced() 
{
	/// logDD("imageAdvanced");
	return image();
}

void GraphicalBuilding::setImageSequence( int type )
{
	setSequence( ImageTheme.buildings[ type ] );
	setFrame( 0 );
}

void GraphicalBuilding::setAnimation( int nbFrame, int freq )
{
	_nbFrame = nbFrame;
	_freq = freq;
	if( _nbFrame > 1 ) {
		setAnimated( true );
	} else {
		setAnimated( false );
	}
}

void GraphicalBuilding::setOwner( GenericPlayer * player )
{
	if( player ) {
		_flag->show();
		_flag->setOwner( player );
	} else {
		_flag->hide();
	}
}

int GraphicalBuilding::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}

//
// ----- GraphicalBase -----
//

GraphicalBase::GraphicalBase( QGraphicsScene * canvas )
	: GraphicalBuilding( ImageTheme.bases, canvas )
{
 	setFrame( 0 );
	setZValue( CAN_BUILDING );
}

void GraphicalBase::setRace( int race )
{
	setFrame( race );
}
