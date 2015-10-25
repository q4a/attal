/****************************************************************
**
** Attal : Lords of Doom
**
** gameDescription.cpp
** description of game info
**
** Version : $Id: gameDescription.cpp,v 1.17 2002/09/08 14:05:51 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/01/2001
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

#include "gameDescription.h"

// generic include files
// include files for QT
#include <qfile.h>
#include <qtextstream.h>
// application specific include files
#include "common/calendar.h"
#include "common/dataTheme.h"
#include "common/genericLord.h"
#include "common/log.h"
#include "common/unit.h"

#include "client/graphicalArtefact.h"

extern DataTheme DataTheme;
extern QString IMAGE_PATH;
extern QString DATA_PATH;


GameDescription::GameDescription()
	: _nbPlayer( 0 )
{
	_calendar = new Calendar();
	_calendar->reinit();
	_playersName = 0;
	_playersColor = 0;
	_artefacts.setAutoDelete( true );
}

GameDescription::~GameDescription()
{
	delete _calendar;
}

void GameDescription::setPlayerNumber( int nb )
{
	_nbPlayer = nb;
	_playersName = new QString[ _nbPlayer ];
	_playersColor = new QColor[ _nbPlayer ];
}

Artefact * GameDescription::getArtefactById( int id )
{
	Artefact * ret = 0;
	for( uint i = 0; i < _artefacts.count(); i++ ) {
		if( _artefacts.at( i )->getId() == id ) {
			ret = _artefacts.at( i );
		}
	}
	return ret;
}

void GameDescription::initLords()
{
	uint nbLord = DataTheme.lords.count();
	uint i;

	_realLords = new Lord * [ nbLord ];
	for( i = 0; i < nbLord; i++ ) {
		_realLords[i] = (Lord *)DataTheme.lords.at( i );
	}
}



