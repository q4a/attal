/****************************************************************
**
** Attal : Lords of Doom
**
** engine.h
** the game engine !
**
** Version : $Id: engine.h,v 1.43 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QObject>
#include <QList>
#include <QQueue>
#include <QThread>
// application specific includes
#include "libCommon/fightResultStatus.h"
#include "libCommon/gameData.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericMap.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericPlayer.h"

#include "libServer/attalServer.h"
#include "libServer/fightEngine.h"

class Campaign;
class GenericBonus;
class GenericMapCreature;
class QuestionManager;
class ScenarioParser;
class TavernManager;

/*              ------------------------------
 *                         Engine
 *              ------------------------------ */


class Engine : public QThread, public GameData
{
	Q_OBJECT
public:
	/** Constructor */
	Engine( AttalServer * serv );
	
	~Engine();

	void run();

	/** Start game (scenario) */
	void startGame();
	
	/** Start game (campaign) */
	void startCampaign();
	
	void deleteCampaign();

	/** Load a game (campaign) */
	bool loadCampaign( const QString & filename );

	/** Save the game */
	bool saveGame( const QString & filename );

	/** End the game ( called from extern ) */
	void endGameExternal();
	
	/** Start a fight */
	void startFight();
	void startFight( int lordAttack, GenericLord *lordDefense );
	void startFight( int lordAttack, GenericMapCreature * creature );

	/** End the fight */
	void endFight();

	/** Reinit game engine */
	void reinit();
	
	Campaign * getCampaign() { return _campaign; }
	
	enum State {
		NOT_PLAYING,
		IN_GAME,
		IN_QUESTION,
		IN_FIGHT_LORD,
		IN_FIGHT_CREATURE
	};

	State getState() { return _state; }

	void setGameId( int id ) { _gameId = id; }

	int getGameId() { return _gameId; }
	
	void appendPlayersList( QList<AttalPlayerSocket *> sockets );

	void addPlayer( AttalPlayerSocket * socket );

	void addPlayer( GenericPlayer * player );
	
	void removePlayer( GenericPlayer * player );
	
	void removePlayer( AttalPlayerSocket * socket );

	//return the number of lacking players
	int getNeededPlayers() { 	return _nbPlayer - _players.count(); }

	//no read all scenario	
	int getNeededPlayersFast( const QString & filename );

public slots:
	/** Slot who reads socket */
	//void slot_readSocket( int );
	
	void slot_readData( int, AttalSocketData );

	/** Slot for managing new players */
	void slot_newPlayer( AttalPlayerSocket * player );

	/** Slot for removing a player disconnected */
	void slot_endConnection( QString name );

	/** Slot catching end of fight */
	void slot_endFight( FightResultStatus result );

signals:
	void sig_result( int, bool);
	
	void sig_endGame( int );

private:
	enum CreatureAction {
		CreatureJoin,
		CreatureMercenary,
		CreatureFlee,
		CreatureFight
	};

	CreatureAction computeCreatureAction( GenericMapCreature * creature, double ratio );

	void nextCurrentPlayer();
	/** Activate the next player */
	void nextPlayer();

	bool checkMainQuest();
	void updatePlayers();
	void checkPlayerShouldPlay();
	//endGame start from engine..
	void endGameInternal();
	//neutral and real end game
	void endGame();

	void manageSocketState( int num );
	void stateNotPlaying( int num );
	void stateInGame( int num );
	void stateInQuestion( uint num );
	void stateInFight( int num );
	
	void handleMessage( int num );
	void handleCommand( int num , const QString & cmd );

	void handleAnswer();
	void handleInGameMvt( int num );
	void handleInGameExchange();
	void handleInGameModif();
	void handleInGameModifBase();
	void handleInGameModifBaseBuilding();
	void handleInGameModifBaseUnit();
	void handleInGameModifBaseMarket();
	void handleInGameModifLord();
	void handleInGameModifLordGarrison();
	void handleInGameModifLordUnit();
	void handleInGameModifLordBuy();
	void handleInGameTurn();
	
	void handleGameTavern();
	void handleGameTavernInfo();
	void handleGameTavernLord();

	void newDay();
	void updateCreatures();
	void removeCreature( GenericMapCreature * creature );
	void updateProduction();
	void checkNewWeek();

	void handleAnswerCreatureJoin();
	void handleAnswerCreatureMercenary();
	void handleAnswerCreatureFlee();
	
	// enter into building (if return == true)
	bool handleBuildingEnter( GenericBuilding * building, GenericLord * lord );
	
	void handleAction( Action * action, GenericLord * lord );
	void handleAction( Action * action, GenericPlayer * player, GenericResourceList * rlist = NULL );
	void handleActionListDate( QList<Action *> list, GenericPlayer * player, GenericResourceList * rlist = NULL );

	void handleCreatTurn( GenericPlayer * player, GenericLord * lord );
	void handleBaseTurn( GenericPlayer * player, GenericBase * base );
	void handleBuildingTurn( GenericPlayer * player, GenericBuilding * building );

	bool handleOneMove( GenericLord * movingLord, GenericCell * destCell, int num );
	void movingOnLord( GenericLord * movingLord, GenericCell * destCell );
	void movingOnEvent( GenericLord * movingLord, GenericCell * destCell );
	void movingOnArtefact( GenericLord * movingLord, GenericCell * destCell );
	void movingOnBonus( GenericLord * movingLord, GenericCell * destCell );
	void movingOnBonusResource( GenericBonus * bonus );
	void movingOnBonusPrimSkill( GenericLord * movingLord, GenericBonus * bonus );
	void movingOnChest( GenericLord * movingLord, GenericCell * destCell );
	void movingOnBuilding( GenericLord * movingLord, GenericCell * destCell );
	void movingOnBase( GenericLord * movingLord, GenericCell * destCell );
	void movingOnCreature( GenericLord * movingLord, GenericCell * destCell );
	void movingOnFreeCell( GenericLord * movingLord, GenericCell * destCell );

	void exchangeUnits();
	void exchangeArtefact();
	void exchangeBaseUnits();
	void exchangeUnitSplit();

	uint getAlivePlayersNumber();

	bool enoughPlayers();
	void checkResult();

	void updatePlayerPrices( GenericPlayer * player);

	void updateMapVision( QList<GenericCell *> & removed, QList<GenericCell *> & added );
	void updateCellVision( GenericCell * cell, GenericPlayer * player );

	void manageIncreaseExperience( GenericLord * lord, int experience );
	void decreaseMove( GenericLord * movingLord, GenericCell * cell );
	void moveLord( GenericLord * movingLord, GenericCell * cell );
	
	/** Save a campaing **/
	bool saveCampaign( const QString & filename );

	inline int readInt();

	inline unsigned char readChar();

	inline uchar getCla1();
	inline uchar getCla2();
	inline uchar getCla3();

	bool _isProcessing;
	int _num;
	int _gameId;

	AttalServer * _server;
	FightEngine * _fight;
	State _state;
	GenericPlayer * _currentPlayer;
	Campaign * _campaign;

	QuestionManager * _question;
	TavernManager * _tavern;
	QQueue<AttalSocketData> _dataQueue;
	AttalSocketData _currentData;

};

#endif //ENGINE_H
