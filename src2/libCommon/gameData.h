/****************************************************************
**
** Attal : Lords of Doom
**
** gameData.h
** Manage data of a whole game
**
** Version : $Id: gameData.h,v 1.34 2014/06/22 19:27:31 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QList>
#include <QString>


// application specific include files
#include "libCommon/genericEvent.h"
#include "libCommon/log.h"

class QTextStream;

class Calendar;
class GenericBase;
class GenericBuilding;
class GenericCell;
class GenericLord;
class GenericMap;
class GenericMapCreature;
class GenericPlayer;
class Quest;
class QuestManager;
class ScenarioParser;

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

	void init();

	/** Reinit data (new game) */
	virtual void reinit();

	/** clear lists of data */
	void clearLists();
	
	virtual void initLords();
	
	bool load( const QString & filename );
	
	/** Set the name of the scenario */
	void setScenarioName( const QString & name ) { _scenarioName = name; }
	
	/** Return the name of the scenario */
	QString getScenarioName() { return _scenarioName; }
	
	/** Set the description of the scenario */
	void setScenarioDescription( const QString & desc ) { _scenarioDescription = desc; }
	
	/** Return the description of the scenario */
	QString getScenarioDescription() { return _scenarioDescription; }
	
	/** Set the theme used by the scenario */
	void setScenarioTheme( const QString & theme ) { _scenarioTheme = theme; }

	/** Return the theme used by the scenario */
	QString getScenarioTheme() { return _scenarioTheme; }
	
	void setAttalVersion( const QString & version ) { _attalVersion = version; }

	QString getAttalVersion() { return _attalVersion; }

	void setDateByType( int type, int value );

	uint getDateByType( int type );

	void setCurrentPlayerNum( int num ) { _currPlayNum = num; }
	
	int getCurrentPlayerNum() {  return _currPlayNum; }

	void setTurn( uint turn );

	uint getTurn();

	void setInitialDateByType( int type, int value );

	/** Adds a player to the data */
	virtual void addPlayer() {};

	virtual void setNbPlayer( int nb ) { _nbPlayer = nb; }

	virtual uint getNbPlayer() { return _nbPlayer; }

	virtual GenericCell * getCell( uint row, uint col );	
	
	virtual void loadMap( QTextStream * stream, int width, int height );

	uint getBaseNumber();

	uint getBuildingNumber();
	
	GenericBase * getBase( uint num );
	
	virtual GenericBase * getNewBase( uchar race = 0 );

	virtual GenericBase * getNewBase(	uchar race, int row, int col, uchar id, int nb, const QList<uchar> & forbidlist);
	
	void setBase2Player( int idBase, int player );
	
	void setBuilding2Player( int idBuild, int player );

	void resetOwnership( GenericPlayer * player );

	GenericLord * getLord( int idLord );
	
	uint getLordNumber();

	GenericPlayer * getPlayer( int num );

	uint getPlayerNumber();
	 
	void setPlayerNumber( const uint & nb );

	GenericArtefact * getArtefactById( int id );

	GenericBase * getBaseById( int id );
	
	GenericBuilding * getBuildingById( int id );

	virtual bool setLord2Player( int idLord, int player ); 
	
	virtual GenericEvent * getNewArtefact( int id = -1 );

	virtual GenericEvent * getNewBonus();

	virtual GenericEvent * getNewChest();

	virtual GenericBuilding * getNewBuilding( uchar type );

	virtual GenericBuilding * getNewBuilding( uchar type, int id, int row, int col );

	virtual Quest * getNewQuest();

	Quest * getMainQuest();
  
	virtual GenericMapCreature * getNewMapCreature();

	virtual GenericMapCreature * getNewMapCreature( int row, int col, uchar race, uchar level, int nb, bool looking);

	virtual void removeMapCreature( GenericMapCreature * creature );

	virtual void removeMapCreature( int row, int col );

	virtual void removeEvent( GenericEvent * event );

	virtual void removeEvent( int row, int col );

	virtual void removeBase( GenericBase * base );
	
	virtual void removeBuilding( GenericBuilding * building );

	void setBaseId( GenericBase * base );

	void setBuildingId( GenericBuilding * build );

	Calendar * getCalendar() { return _calendar; }
	
	GenericLord * modifLordNew( int row, int col, uchar id );
	
	GenericLord * modifLordVisit( int row, int col, uchar id, uchar num, uchar present );

	void modifLordUnit( uchar id , uchar pos, uchar race, uchar level, int nb, uchar move, int health	);
	
	void modifLordCharac( char id , char charac, int number );

	void exchangeArtefactLord( uchar idLord1, uchar idLord2, int item);

	void modifBuildingOwner( int row, int col,  int playNum );
	
	void modifBaseOwner( int row, int col,  int playNum );
	
	void modifBaseBuilding( int row, int col, uchar level,	bool create );

	void modifBaseUnit( int row, int col, uchar pos, uchar race , uchar level, int number );

	void modifBaseProduction( int row, int col, uchar race , uchar level, int number );
	
	void modifBaseState( int row, int col, int type,	bool state );
	
	void modifBaseName( int row, int col, const QString & name );

protected:

	int _nbPlayer;
	uint _nbArtefact, _nbBonus, _nbChest;
	GenericMap * _map;
	GenericPlayer * _player;
	int _currPlayNum;
	Calendar * _calendar;
	QList<GenericPlayer *> _players;
	QList<GenericBase *> _bases;
	QList<GenericBuilding *> _buildings;
	QList<GenericLord *> _lords;
	QList<GenericEvent *> _events;
	QList<GenericMapCreature *> _creatures;
	QuestManager * _quests;
	QString _scenarioName, _scenarioTheme, _scenarioDescription;	
	QString _attalVersion;
};
 
#endif // GAMEDATA_H   
