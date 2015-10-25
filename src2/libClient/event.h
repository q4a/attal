/****************************************************************
**
** Attal : Lords of Doom
**
** event.h
** Displays events on map
**
** Version : $Id: event.h,v 1.2 2003/08/27 13:11:06 audoux Exp $
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
// application specific include files
#include "libCommon/genericEvent.h"

class GenericCell;

/**             ------------------------------
 *                          Event
 **             ------------------------------ */


class Event : public GenericEvent
{

public:
	/** Constructor */
	Event();

	virtual ~Event() {}

	virtual void setCell( GenericCell * cell );
};

#endif // EVENT_H
