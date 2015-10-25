/****************************************************************
**
** Attal : Lords of Doom
**
** event.h
** Displays events on map
**
** Version : $Id: event.h,v 1.1 2002/12/22 19:55:35 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/12/2002
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


#ifndef EVENT_H
#define EVENT_H


// generic include files
// include files for QT
#include <qcanvas.h>
// application specific include files
#include "common/genericEvent.h"

/**             ------------------------------
 *                          Event
 **             ------------------------------ */


class Event : public QCanvasSprite, public GenericEvent
{

public:
	/** Constructor */
	Event( QCanvas * map );

	virtual ~Event() {}

	virtual void setCell( GenericCell * cell );

	virtual void setType( EventType type );
};

#endif // EVENT_H
