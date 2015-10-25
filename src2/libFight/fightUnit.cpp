/****************************************************************
**
** Attal : Lords of Doom
**
** fightUnit.cpp
** Manage Units
**
** Version : $Id: fightUnit.cpp,v 1.63 2014/06/20 16:12:24 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

// generic include files
#include <math.h>

// include files for QT
#include <QBrush>
#include <QPainter>
#include <QRectF>

// application specific include files
#include "conf.h"

#include "libCommon/attalSettings.h"
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

#include "libFight/fightCell.h"

#include "libClient/imageTheme.h"

/** rtti number for GraphicalFightUnit class */
const int FightUnit::RTTI = Type;


FightUnit::FightUnit( QGraphicsScene * canvas )
: AttalSprite( (*ImageTheme.getCreature(0))[0], canvas ), GenericFightUnit()
{
	_cpt = 0;
	_isMoving = false;
	_isActive = false;
 	setFrame( 0 );
	setZValue( CAN_F_UNIT );
}

FightUnit::FightUnit( QGraphicsScene * canvas, GenericFightUnit * unit, bool mirror )
	: AttalSprite( (*ImageTheme.getCreature(0))[0], canvas ),
	  GenericFightUnit( *unit )
{
	_cpt = 0;
	_isMoving = false;
	_destroyed = false;
	_isActive = false;
	setZValue( CAN_F_UNIT );

	QList<QPixmap> * sequence 	= (*ImageTheme.getCreature(DataTheme.creatures.computeSequence( unit )))[ (mirror  ^ getCreature ()->isAnimated()) ? 1 : 0 ];
	if( sequence ) {
		setSequence( sequence );
	}
	_lookingToRight = mirror;

	setFrame( 0 );

}

void FightUnit::initCreatureForMove( GraphicalFightCell *cell )
{
	Creature* creature = getCreature ();

	setFrame ( creature->getFirstAnimationFrame( Moving ) );

	// bounding rect is in item coordinate
	QRectF endRect = cell->boundingRect();
	QPointF endPoint = endRect.topLeft();
	QPointF sceneEndPoint = cell->mapToScene( endPoint ) ;

	if( _moving ) {
		QRectF startRect = _moving->boundingRect();
		QPointF startPoint = startRect.topLeft();
		QPointF sceneStartPoint = _moving->mapToScene( startPoint ) ;

		double dx = double ( sceneEndPoint.x() -  sceneStartPoint.x() ) / 10.0;
		double dy = double ( sceneEndPoint.y() -  sceneStartPoint.y() ) / 10.0;
		setVelocity( dx, dy );
	} else {
		_cpt = 10;
		setVelocity( 0, 0 );
	}
}

int FightUnit::hit( int nb )
{
	int ret = GenericFightUnit::hit( nb );
	
	if( _number == 0 ) {
		if( canAnimate( Dying  ) ) {
			animate(Dying);
		} else {
			setDestroyed( true );
		}
	}

	return ret;
}


void FightUnit::goTo( FightCell * cell )
{
	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	Creature* creature = getCreature ();
	uint race = creature->getRace ();
	uint level = creature->getLevel ();

	if( fsettings.isAnimationEnabled ) {
		if( DataTheme.creatures.at( race , level )->isAnimated() ) {
			_currentAnimationType = Moving;
			_isMoving = true;
			if( creature->getNumFrames() > 0 ) {
				_cpt = 10;
				setAnimated ( true );
				if( _fifo.count() == 0 ) {
					_moving = (FightCell*)getCell();

				}
				_fifo.enqueue( cell );
			}
		} else {
			setPosition( cell );
		}
	} else {
		setPosition( cell );
	}

	GenericFightUnit::goTo( (GenericFightCell *)cell );
}

void FightUnit::setPosition( FightCell * cell )
{
	//logDD("set position, cell %p, x %f, y %f", cell, cell->x(), cell->y());
	GenericFightUnit::goTo( (GenericFightCell *)cell );
	
	if( (FightCell*)this->getNeibCell() && isLookingToRight()) {
		cell = (FightCell*)this->getNeibCell();
	}

	QRectF rect = cell->boundingRect();

	//logDD(" setPos X rect %f, offset %f, width %f\n Y rect %f, offset %f,height %f\n rect width %f, rect height %f ", rect.x() , creature->getXOffset() , boundingRect().width() ,	rect.y() , creature->getYOffset(), boundingRect().height() ,rect.width(),  rect.height() );
	int xoffset = _lookingToRight ? _creature->getXOffset() : _creature->getXOffsetMirror();
	int yoffset = _creature->getYOffset();

	setPos( cell->x() + xoffset - boundingRect().width() + 2 * rect.width(), cell->y() + ( rect.height() ) - boundingRect().height() + yoffset );

	setZValue( CAN_F_UNIT + cell->getRow() );

}

void FightUnit::advance( int stage )
{
	if( animated() ) {

		uint race = _creature->getRace ();
		uint level = _creature->getLevel ();
		if( DataTheme.creatures.at( race , level )->isAnimated() ) {
			if( stage == 1 ) {

				switch ( _currentAnimationType )
				{
					case Moving:

						if( frame() >= _creature->getLastAnimationFrame( Moving ) ) {
							setFrame( _creature->getFirstAnimationFrame( Moving ) );
						}
						_cpt++;
						if( _cpt >= 10 ) {
							_cpt = 0;
							if( _fifo.count() > 0 ) {
								FightCell * temp = _fifo.dequeue();
								initCreatureForMove( temp );
								_moving = temp;
							} else {
								setFrame( _creature->getFirstAnimationFrame( Moving ) );
								if( _moving ) {
									setPosition( _moving );
								}
								setVelocity( 0, 0 );
								_moving = 0;
								setAnimated( false );
								setActive( false );
								_isMoving = false;
							}
						}
						break;
					case Shooting:
					case ShootHigh:
					case ShootLow:
					case Defending:
					case Fighting:
					case AttackHigh:
					case AttackLow:
						if( frame() >= _creature->getLastAnimationFrame( _currentAnimationType ) ) {
							setFrame( _creature->getFirstAnimationFrame( Moving ) );
							setAnimated ( false );
						}
						_isMoving = false;
						setPosition( (FightCell*)this->getCell() );
						break;
					case Selecting:
						if( frame() >= _creature->getLastAnimationFrame( Selecting ) ) {
							setFrame( _creature->getFirstAnimationFrame( Selecting ) );
							//setAnimated ( false );
						}
						_isMoving = false;
						break;
					case Dying:
						if( frame() >= _creature->getLastAnimationFrame( Dying ) ) {
							setFrame( _creature->getFirstAnimationFrame( Dying ) );
							setAnimated ( false );
							setDestroyed( true );
						}
						_isMoving = false;
						break;
					default:
					 	break;
				}
				AttalSprite::advance ( stage );
			} else {
			}
		} else {
			//logDD( "advance not animated %d %d", race, level );
			_isMoving = false;
		}
	}
}

void FightUnit::paint ( QPainter *  painter , const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	int numItems = getNumber ();

	QRectF r = boundingRect ();

	int x,y;
	int width = (int) r.width ();
	int height = (int) r.height ();
	if( numItems > 0 ) {
		if(!_destroyed) {
			int xoffset = _lookingToRight ? _creature->getXOffset() : _creature->getXOffsetMirror();
			int yoffset = _creature->getYOffset();
			x = width - 60 -  xoffset;
			y = height - 32 -  yoffset;
		} else {
			x = width - 30;
			y = height - 16;
		}
	} else {
		x = width - 30;
		y = height - 16;
	}

	if( ! _destroyed ) {
		QFont oldFont = painter->font ();
		QPen oldPen = painter->pen ();
		QBrush oldBrush = painter->brush ();
		QFont f = oldFont;
		QPen pen ( QColor ( 100, 255 , 255 ) );
		painter->setPen ( pen );
		QBrush br ( QColor ( 200, 150, 150 ) );
		painter->setBrush ( br );

		painter->drawRect ( int (r.left ()) + x  , int( r.top ()) + y , 30, 16 );

		QPen fontPen ( QColor ( 120, 130, 240 ) );
		painter->setPen ( fontPen );

		f.setPixelSize ( 12 );
		painter->setFont ( f );

		QString s = QString::number ( numItems );
		painter->drawText ( int (r.left ()) + x  , int( r.top () )+ y , 30, 16, Qt::AlignCenter, s );

		painter->setFont ( oldFont );
		painter->setPen ( oldPen );
		painter->setBrush ( oldBrush );
	}
	AttalSprite::paint( painter,  option,  widget );
}

QPixmap FightUnit::imageAdvanced() 
{
	return image();
}

void FightUnit::setDestroyed( bool state )
{
	if( state ) {
		_destroyed = state;
		setAnimated( false );
		setZValue( CAN_F_UNIT_DEAD );
		if( canAnimate( Dead  )) {
			setFrame ( _creature->getFirstAnimationFrame( Dead ) );
		} else {
			setSequence( ImageTheme.deadCreature );
			setFrame( 0 );
		}
	}
}

bool FightUnit::canAnimate (CreatureAnimationType current)
{ 
	bool ret = false;
	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	if( fsettings.isAnimationEnabled ) {
		if (_creature->getFirstAnimationFrame( current )!= -1) {
			ret = true;
		}
	}
	return ret;
}

void FightUnit::animate(CreatureAnimationType current)
{
	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	uint race = _creature->getRace ();
	uint level = _creature->getLevel ();

	if( fsettings.isAnimationEnabled ) {
		if( DataTheme.creatures.at( race , level )->isAnimated() ) {
			_currentAnimationType = current;

			setAnimated ( true );

			setFrame ( _creature->getFirstAnimationFrame( current ) );
		}
	}
}

bool FightUnit::hit( const QPointF & p )
{
	QImage ima = image().toImage();
	bool ret = false;
	int ix = (int)(p.x()-scenePos().x());
	int iy = (int)(p.y()-scenePos().y());
	    
	if ( ima.valid( ix , iy ) ) {
		QRgb pixel = ima.pixel( ix, iy );
		ret = ( qAlpha( pixel ) != 0 );
		
	}
	return ret;
}

void FightUnit::setActive( bool state ) 
{
	_isActive = state;
	if( state ) {
		setZValue( CAN_F_UNIT_HL );
	} else {
		setZValue( CAN_F_UNIT );
	}

	if(_creature->isAnimated()) {
		if(state) {
			animate(Selecting);
		} else {
			if(_currentAnimationType == Selecting) {
				setAnimated ( false );
			}
		}
	}

}

void FightUnit::setAlpha(bool state)
{
	if(_destroyed) {
		return;
	}

	if( state ) {
		if(!_creature->isAnimated()) {
			setOpacity( 0.6 );
		} else {
			setOpacity( 0.7 );
		}
	} else {
		setOpacity( 1 );
	}
}

int FightUnit::type() const
{
	// Enable the use of qgraphicsitem_cast with this item.
	return Type;
}
