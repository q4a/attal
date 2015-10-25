/****************************************************************
**
** Attal : Lords of Doom
**
** questManager.cpp
** Manages the quests of a scenario
**
** Version : $Id: questManager.cpp,v 1.2 2007/04/01 17:32:39 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "questManager.h"

// generic include files
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"

QuestManager::QuestManager()
{
	_mainQuest = 0;
	//_quests.setAutoDelete( true );
	clear();
}

QuestManager::~QuestManager()
{
	clear();
}

void QuestManager::clear()
{
	if( _mainQuest ) {
		delete _mainQuest;
		_mainQuest = 0;
	}

	while( ! _quests.isEmpty() ) {
		delete _quests.takeFirst();
	}
}

void QuestManager::save( QTextStream * ts, uint indent )
{
	indentation( ts, indent );
	(*ts) << "<quests>" << endl;

	if( _mainQuest ) {
		indentation( ts, indent + 1 );
		(*ts) << "<quest type=\"main\">" << endl;

		_mainQuest->save( ts, indent + 2 );

		indentation( ts, indent + 1 );
		(*ts) << "</quest>" << endl;
	}

	uint nbQuests = _quests.count();
	for( uint i = 0; i < nbQuests; i++ ) {
		indentation( ts, indent + 1 );
		(*ts) << "<quest>" << endl;

		( _quests.at( i ) )->save( ts, indent + 2 );

		indentation( ts, indent + 1 );
		(*ts) << "</quest>" << endl;
	}

	indentation( ts, indent );
	(*ts) << "</quests>" << endl;
}

Quest * QuestManager::getMainQuest()
{
	return _mainQuest;
}

uint QuestManager::getSecondaryQuestNumber()
{
	return _quests.count();
}

Quest * QuestManager::getSecondaryQuest( uint num )
{
	Quest * ret = 0;

	if( (int)num < _quests.count() ) {
		ret = _quests.at( num );
	}

	return ret;
}

void QuestManager::appendSecondaryQuest( Quest * quest )
{
	_quests.append( quest );
}


