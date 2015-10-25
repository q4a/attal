/****************************************************************
**
** Attal : Lords of Doom
**
** attalSprite.cpp
** class to implement animations
**
** Version : $Id: attalSprite.cpp,v 1.15 2013/11/18 20:54:07 lusum Exp $
**
** Author(s) : Sardi Carlo
**
** Date : 28/08/2006
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

#include "attalSprite.h"

// generic include files
// include files for QT
#include<QPainter>
#include <QStyleOptionGraphicsItem>
// application specific include files
#include "libCommon/log.h"

/*partially taken from examples of qt4.2 libraries (gpl licensed)*/

/** add comments here */
	AttalSprite::AttalSprite( QList<QPixmap> * animation,QGraphicsScene *scene)
: QGraphicsItem(0, scene), currentFrame(0), frames(animation), vx(0), vy(0)
{
	_animated = false;
	//_timer = 0;
	//_period = 10;
	//_phase = 0;
	//_isAdv = false;
}

AttalSprite::~AttalSprite()
{
	/*
	if( _timer ) {
		killTimer(_timer);
		_timer = 0;
	}
	*/
}

void AttalSprite::setFrame(int frame)
{
	if (!frames->isEmpty()) {
		prepareGeometryChange();
		currentFrame = frame % frames->size();
	}
}

void AttalSprite::advance(int phase)
{
	if( _animated ) {
		if (phase == 1 && !frames->isEmpty()) {
			setFrame(currentFrame + 1);
		}
	}
	if (vx || vy)
		moveBy(vx, vy);
}

void AttalSprite::setSequence(QList<QPixmap> * sequence)
{
	frames = sequence;
}

void AttalSprite::setAnimated( bool animated ) 
{
	/*
	if( animated ) {
		_timer = startTimer( _period );
	} else {
		if( _timer ) {
			killTimer(_timer);
			_timer = 0;
		}
	}
	*/
	_animated = animated; 
}
//no more used , code used for sprite-based timer
/*
void AttalSprite::setAdvancePeriod( int period ) 
{
	_period =  period;
	if( _timer && _animated ) {
		killTimer( _timer );
		_timer = startTimer( _period );
	}
}

void AttalSprite::timerEvent(QTimerEvent *event)
{
	if( !_isAdv) {
		_isAdv = true;
		if( _phase == 0 ) { 
			_phase = 1; 
		} else {
			_phase  = 0;
		}
		advance( _phase );
		_isAdv = false;
	}
}
*/
QRectF AttalSprite::boundingRect() const
{
	return QRectF(QPointF( 0 , 0 ) , frames->at(currentFrame).size() );
}

void AttalSprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		                               QWidget * /* widget */)
{
	painter->setClipRect( option->exposedRect );
	painter->drawPixmap(0, 0, frames->at(currentFrame));
}

