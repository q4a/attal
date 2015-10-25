/****************************************************************
**
** Attal : Lords of Doom
**
** gameData.h
** Manage data of a whole game
**
** Version : $Id: gameData.h,v 1.13 2002/10/22 18:48:23 audoux Exp $
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

 
#ifndef GAMEDATA_H
#define GAMEDATA_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
// application specific include files
#include "common/log.h"

class QTextStream;

class GenericMap;
class GenericCell;
class GenericPlayer;
class GenericBase;
class GenericLord;
class GenericBuilding;
class GenericArtefact;
class ScenarioParser;
class Quest;
class QuestManager;

/**              ------------------------------
 *                          GameData
 **              ------------------------------ */

class GameData
{

public:
	/** Constructor */
	GameData();

	virtual ~GameData();

	void save( QTextStream * ts );

	/** Reinit data (new game) */
	virtual void reinit();
	
	/** Set the name of the scenario */
	void setScenarioName( QString name ) { _scenarioName = name; }
	
	/** Return the name of the scenario */
	QString getScenarioName() { return _scenarioName; }
	
	/** Set the description of the scenario */
	void setScenarioDescription( QString desc ) { _scenarioDescription = desc; }
	
	/** Return the description of the scenario */
	QString getScenarioDescription() { return _scenarioDescription; }
	
	/** Set the theme used by the scenario */
	void setScenarioTheme( QString theme ) { _scenarioTheme = theme; }

	/** Return the theme used by the scenario */
	QString getScenarioTheme() { return _scenarioTheme; }

	virtual void addPlayer() {}

	virtual void setNbPlayer( int nb ) { _nbPlayer = nb; }

	virtual uint getNbPlayer() { return _nbPlayer; }

	virtual GenericCell * getCell( int row, int col );	

	virtual uint getBaseNumber();

	virtual void loadMap( QTextStream * stream, int width, int height );
	
	virtual GenericBase * getNewBase();
	
	virtual void setBase2Player( int idBase, int player );
	
	virtual void setBuilding2Player( int idBuild, int player );
	
	GenericLord * getLord( int idLord );

	GenericPlayer * getPlayer( int num );
	
	virtual void setLord2Player( int idLord, int player ); 
	
	virtual GenericArtefact * getNewArtefact();

	virtual GenericBuilding * getNewBuilding();

	virtual Quest * getNewQuest();

protected:
	int _nbPlayer;
	GenericMap * _map;
	QList<GenericPlayer> _players;
	QList<GenericBase> _bases;
	QList<GenericBuilding> _buildings;
	QList<GenericLord> _lords;
	QList<GenericArtefact> _artefacts;
	QuestManager * _quests;
       	QString _scenarioName, _scenarioTheme, _scenarioDescription;	
};
 
#endif // GAMEDATA_H   
