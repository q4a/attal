/****************************************************************
**
** Attal : Lords of Doom
**
** fightUnit.h
** Manage Units
**
** Version : $Id: fightUnit.h,v 1.28 2009/06/05 16:52:52 lusum Exp $
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


#ifndef FIGHTUNIT_H
#define FIGHTUNIT_H


// generic include files
// include files for QT
#include <QGraphicsScene>
#include <QQueue>
// application specific include files
#include "libCommon/attalSocket.h"
#include "libCommon/unit.h"
#include "libCommon/creature.h"
#include "libClient/attalSprite.h"

class FightCell;
class GraphicalFightCell;

/*              ------------------------------
 *                         FightUnit
 *              ------------------------------ */


/** comment for the class */
class FightUnit : public AttalSprite, public GenericFightUnit
{

public:
	/** Constructor */
	FightUnit( QGraphicsScene * canvas );

	FightUnit( QGraphicsScene * canvas, GenericFightUnit * unit, bool mirror );

	enum { 
		Type = QGraphicsItem::UserType + 2234
	};

	/** Hit unit by 'nb' hitpoints */
	int hit( int nb );

	/** unit go to cell */
	void goTo( FightCell * cell );

	void setPosition( FightCell * cell );

	virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );

	/** Advance animation */
	void advance( int stage );

	/** Highlight (or not) the unit if active (or not) */
	void setActive( bool state );

	void setAlpha( bool state );
	
	/** Draw destroyed unit (or not) if destroyed (or not) */
	void setDestroyed( bool state );

	void animate (CreatureAnimationType current);

	bool canAnimate (CreatureAnimationType current);
	
	bool isAnimated () { return getCreature()->isAnimated(); }
	
	inline bool isMoving () { return _isMoving; }
	
	CreatureAnimationType getTypeAnimation() { return _currentAnimationType; }
    	
	bool hit( const QPointF & );

	static const int RTTI;
	
	int type() const;

protected:
	void initCreatureForMove( GraphicalFightCell * cell );

	QPixmap imageAdvanced() ;
	QPixmap _alpix, _hpix;
	uint _cpt;
	FightCell * _moving;
	QQueue<FightCell *> _fifo;
	bool _destroyed, _isMoving, _isActive;

	CreatureAnimationType _currentAnimationType;
};

#endif // FIGHTUNIT_H
