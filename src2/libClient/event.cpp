/****************************************************************
**
** Attal : Lords of Doom
**
** event.cpp
** Displays event on map
**
** Version : $Id: event.cpp,v 1.6 2007/05/13 17:15:21 lusum Exp $
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

#include "event.h"

// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/graphicalArtefact.h"
#include "libClient/bonus.h"
#include "libClient/chest.h"

Event::Event()
{
}

void Event::setCell( GenericCell * cell )
{
	TRACE("Event::setCell ");

	GenericEvent::setCell( cell );
	switch( _type ) {
	case EventArtefact:
		( (Artefact *)_artefact )->setCell( cell );
		break;
	case EventBonus:
		( (Bonus *)_bonus )->setCell( cell );
		break;
	case EventChest:
		( (Chest *)_chest )->setCell( cell );
		break;
	default:
		logEE( "Should not happen" );
		break;
	}
}

