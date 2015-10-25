/****************************************************************
**
** Attal : Lords of Doom
**
** genericEvent.h
** Handles events on map
**
** Version : $Id: genericEvent.h,v 1.4 2002/12/22 19:51:10 audoux Exp $
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
#include <qlist.h>
// application specific include files
#include "common/genericCell.h"

/**              ------------------------------
 *                         GenericEvent
 **              ------------------------------ */

class GenericEvent
{

public:
	enum EventType {
		ET_EXP = 0,
		ET_RES,
		ET_EXP_RES
	};

	/** Constructor */
	GenericEvent();

	virtual ~GenericEvent() {}

	EventType getType() { return _type; }

	virtual void setType( EventType type ) { _type = type; }

	uint getParamNumber();

	int getParam( uint num );

	void addParam( int val );

	void replaceParam( uint num, int val );

	GenericCell * getCell() { return _cell; }

	virtual void setCell( GenericCell * cell ) { _cell = cell; }

	bool isVisible();

protected:
	EventType _type;
	QList<int> _params;
	GenericCell * _cell;
};

/*
For an event, the first param says if it is visible or not
*/

#endif // GENERICEVENT_H
