/****************************************************************
**
** Attal : Lords of Doom
**
** genericEvent.cpp
** this is a template for all .cpp files
**
** Version : $Id: genericEvent.cpp,v 1.5 2007/10/14 15:21:35 lusum Exp $
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

#include "genericEvent.h"

// generic include files
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/condition.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericChest.h"
#include "libCommon/log.h"

//
// ----- GenericEvent -----
//

GenericEvent::GenericEvent()
{
	_type = EventNone;
	_cell = NULL;
	_condition = 0;
	_artefact = NULL;
	_bonus = NULL;
	_chest = NULL;
}

GenericEvent::~GenericEvent()
{
	if( _artefact ) {
		delete _artefact;
	}
	if( _bonus ) {
		delete _bonus;
	}
	if( _chest ) {
		delete _chest;
	}
}

void GenericEvent::setCell( GenericCell * cell )
{
	TRACE("GenericEvent::setCell");
	_cell = cell;
}

void GenericEvent::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<event type=\"" << (uint)_type << "\">" << endl;

	indentation( ts, indent+1 );
	*ts << "<col>" << _cell->getCol() << "</col>" << endl;

	indentation( ts, indent+1 );
	*ts << "<row>" << _cell->getRow() << "</row>" << endl;

	if( _artefact ) {
		_artefact->save( ts, indent+1 );
	}
	if( _bonus ) {
		_bonus->save( ts, indent+1 );
	}
	if( _chest ) {
		_chest->save( ts, indent+1 );
	}

	indentation( ts, indent );
	*ts << "</event>" << endl;
	*ts << flush;
}

void GenericEvent::setArtefact( GenericArtefact * artefact )
{
	TRACE("GenericEvent::setArtefact");

	_type = EventArtefact;
	_artefact = artefact;
	if( _bonus ) {
		delete _bonus;
		_bonus = NULL;
	}
	if( _chest ) {
		delete _chest;
		_chest = NULL;
	}
}

void GenericEvent::setBonus( GenericBonus * bonus )
{
	TRACE("GenericEvent::setBonus");

	_type = EventBonus;
	_bonus = bonus;
	if( _artefact ) {
		delete _artefact;
		_artefact = NULL;
	}
	if( _chest ) {
		delete _chest;
		_chest = NULL;
	}
}

void GenericEvent::setChest( GenericChest * chest )
{
	TRACE("GenericEvent::setChest");

	_type = EventChest;
	_chest = chest;
	if( _artefact ) {
		delete _artefact;
		_artefact = NULL;
	}
	if( _bonus ) {
		delete _bonus;
		_bonus = NULL;
	}
}

