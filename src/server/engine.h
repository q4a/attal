/****************************************************************
**
** Attal : Lords of Doom
**
** engine.h
** the game engine !
**
** Version : $Id: engine.h,v 1.39 2003/02/10 21:52:05 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/10/2000
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

 
#ifndef ENGINE_H
#define ENGINE_H
 
 
// generic include files
// include files for QT
#include <qobject.h>
#include <qlist.h>
// application specific includes
#include "server/attalServer.h"
#include "server/fightEngine.h"
#include "server/gameData.h"
#include "common/calendar.h"
#include "common/genericBase.h"
#include "common/genericMap.h"
#include "common/genericLord.h"
#include "common/genericPlayer.h"

/*              ------------------------------
 *                         Engine
 *              ------------------------------ */

class ScenarioParser;

/** comment for the class */
class Engine : public QObject, public GameData
{
	Q_OBJECT
public:
	/** Constructor */
	Engine( AttalServer * serv );

	/** Start game */
	void startGame();
	
	/** Load a game */
	bool loadGame( QString filename );
	
	/** Save the game */
	bool saveGame( QString filename );
	
	/** End the game */
	void endGame();
	
	/** Start a fight */
	void startFight();
	void startFight( int lordAttack, GenericLord *lordDefense );
	void startFight( int lordAttack, GenericMapCreature * creature );
	
	/** End the fight */
	void endFight();

	/** Reinit game engine */
	void reinit();
	
	/** Activate the next player */
	void nextPlayer();
	
public slots:
	/** Slot who reads socket */
	void slot_readSocket( int );

	/** Slot for managing new players */
	void slot_newPlayer( AttalPlayerSocket * player );

	/** Slot for removing a player disconnected */
	void slot_endConnection( QString name );

	/** Slot catching end of fight */
	void slot_endFight( char result );

signals:
	void sig_newPlayer( AttalPlayerSocket * player );

private:
	void stateNotPlaying( int num );
	void stateInGame( int num );
	void stateInFight( int num );
	void handleMessage();
	void handleInGameMvt( int num );
	void handleInGameExchange();
	void handleInGameModif();
	void handleInGameModifBase();
	void handleInGameModifLord();

	// enter into building (if return == true)
	bool handleBuildingEnter( GenericBuilding * building, GenericLord * lord );
	void handleBuildingAction( Action * action, GenericLord * lord );

	void handleCreatTurn( GenericPlayer * player, GenericLord * lord );
	void handleBaseTurn( GenericPlayer * player, GenericBase * base );
	void handleBuildingTurn( GenericPlayer * player, GenericBuilding * building );
	void handleBuildingAction( Action * action, GenericPlayer * player );

	uint getAlivePlayersNumber();
	void exchangeUnits();
	void exchangeBaseUnits();

	void updateMapVision( QList<GenericCell> & removed, QList<GenericCell> & added );

	enum State {
		NOT_PLAYING,
		IN_GAME,
		IN_FIGHT
	};

	bool _isCreature;

	AttalServer * _server;
	FightEngine * _fight;
	State _state;
	GenericPlayer * _currentPlayer;
	Calendar * _calendar;
	int _counter;

};

#endif // ENGINE_H

















