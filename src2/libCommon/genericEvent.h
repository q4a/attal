//Added by qt3to4:
#include <QTextStream>
/****************************************************************
**
** Attal : Lords of Doom
**
** genericEvent.h
** Handles events on map
**
** Version : $Id: genericEvent.h,v 1.6 2005/10/30 22:20:57 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 17/12/2002
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


#ifndef GENERICEVENT_H
#define GENERICEVENT_H


// generic include files
// include files for QT
// application specific include files

class QTextStream;

class Condition;
class GenericArtefact;
class GenericBonus;
class GenericCell;
class GenericChest;

/**              ------------------------------
 *                         GenericEvent
 **              ------------------------------ */

class GenericEvent
{

public:
	enum EventType {
		EventNone = 0,
		EventArtefact,
		EventBonus,
		EventChest
	};

	/** Constructor */
	GenericEvent();

	/** Destructor */
	virtual ~GenericEvent();

	EventType getType() { return _type; }

	GenericCell * getCell() { return _cell; }

	virtual void setCell( GenericCell * cell );

	bool isVisible();

	void setVisible( bool state );

	Condition * getCondition() { return _condition; }

	void setCondition( Condition * condition ) { _condition = condition; }

	GenericArtefact * getArtefact() { return _artefact; }

	void setArtefact( GenericArtefact * artefact );

	GenericBonus * getBonus() { return _bonus; }

	void setBonus( GenericBonus * bonus );

	GenericChest * getChest() { return _chest; }

	void setChest( GenericChest * chest );

	/** Save event */
	void save( QTextStream * ts, int indent = 0 );

protected:
	EventType _type;
	bool _visible;
	GenericCell * _cell;
	Condition * _condition;
	GenericArtefact * _artefact;
	GenericBonus * _bonus;
	GenericChest * _chest;
};

#endif // GENERICEVENT_H
