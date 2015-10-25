/****************************************************************
**
** Attal : Lords of Doom
**
** attalSprite.h
** class to implement animations
**
** Version : $Id: attalSprite.h,v 1.12 2013/11/18 20:54:07 lusum Exp $
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

 
#ifndef ATTALSPRITE_H
#define ATTALSPRITE_H
 
 
// generic include files
// include files for QT
#include <QGraphicsItem>
// application specific include files
 
 
/*              ------------------------------
 *                         AttalSprite
 *              ------------------------------ */


/* taken from examples of qt4.2 libraries (gpl licensed)*/

/** comment for the class */
class AttalSprite: public QGraphicsItem
{

public:
	/** Constructor */
	AttalSprite( QList<QPixmap> * animation, QGraphicsScene *scene = 0);

	/** Destructor */
	~AttalSprite();

	inline void setVelocity(qreal xvel, qreal yvel)
	{ vx = xvel; vy = yvel; }
	
	inline qreal xVelocity() const
	{ return vx; }
	
	void setAnimated( bool animated );

	//void setAdvancePeriod( int period );
	
	bool animated() { return _animated; }

	QPixmap image()  
	{ return frames->at(currentFrame); }
	
	QPixmap image( int numFrame )  
	{ return frames->at(numFrame); }
	
	inline qreal yVelocity() const
	{ return vy; }

	virtual	void advance(int phase);
	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QRectF boundingRect() const;

protected:

	void setFrame(int frame);
	
	inline int frame() const
	{ return currentFrame; }
	
	inline int frameCount() const
	{ return frames->size(); }
	
	inline QPixmap image(int frame) const
	{ return frames->isEmpty() ? QPixmap() : frames->at(frame % frames->size()); }
	
	void setImage(QPixmap pixmap, int frame) const
	{ frames->replace(frame % frames->size(), pixmap); }
	
	void setSequence(QList<QPixmap> * sequence);

	//void timerEvent(QTimerEvent *event);
	
private:
	int currentFrame;
	QList<QPixmap> * frames;
	qreal vx, vy;
	bool _animated;
	//bool _isAdv;
	//int _timer, _period, _phase;
};

 
#endif // ATTALSPRITE_H   
