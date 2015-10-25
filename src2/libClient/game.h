/****************************************************************
**
** Attal : Lords of Doom
**
** game.h
** Manages the whole game
**
** Version : $Id: game.h,v 1.105 2010/08/06 22:18:39 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 17/08/2000
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


#ifndef GAME_H
#define GAME_H


// include files for QT
#include <QLabel>
#include <QPixmap>
#include <QWidget>

// application specific includes
#include "libCommon/genericCell.h"
#include "libClient/map.h"
#include "libClient/player.h"
#include "libClient/graphicalGameData.h"
#include "libClient/widget.h"

#include "libCommon/attalSocket.h"
#include "libCommon/log.h"

class GenericBase;

/*              ------------------------------
 *                         Game
 *              ------------------------------ */


/** comment for the class */
class Game : public QWidget, public GraphicalGameData
{
	Q_OBJECT
public:
	/** Constructor */
	/** Associate a socket to the Game (for exchange with the server) */
	Game( QWidget * parent = 0, AttalSocket * socket = 0 );
	
	/** Destructor : delete player and lord */
	~Game();
	
	/** Reinit */
	void reinit();

	/** Lord 'lord' enters in base 'base' */
	void enter( GenericLord * lord, GenericBase * base );

	/** Lord 'lord' enters in building 'build' */
	void enter( GenericLord * lord, GenericBuilding * building );

	/** Handle socket data */
	void handleSocket();

	/** End game */
	void endGame();
	
	/** Begin turn */
	void beginTurn();

	/** Player active is player 'num' */
	void playerActive( char num );

	/** Activate next base */
	void nextBase();

	/** Return the player associated to the game */
	Player * getGamePlayer() { return (Player * ) _player; }

	/** Sets the name of the player */
	void setPlayerName( QString name );
	
	enum MapState {
		MS_NOTHING,
		MS_LORD,
		MS_BASE,
		MS_TECHNIC
	};
	
	enum QRCreature {
		QRFLEE,
		QRMERCENARY,
		QRJOIN
	};

	void setState( MapState state );

	MapState getState() { return _state; }

	void displayMiniMap( bool state );
	
	void displayFullScreen( bool state );
	
	void updateOptions();
	
	void setAdvancePeriod( int period );
	
	void computePath( GenericLord * lord, GenericCell * cell );

	void restartTimer();
	
	void stopTimer();
	
signals:
	void sig_base( GenericBase * );

	void sig_fight( GenericLord * lord, CLASS_FIGHTER cla );

	void sig_statusMsg( const QString & );
	
	void sig_scenInfo( const QString & );
	
	void sig_Center( int , int );

	void sig_Center( GenericCell * );

	void sig_endGame();
	
	void sig_endTurn();
	
	void sig_beginTurn();

	void sig_result(bool);
	
	/* signal to enable some buttons */
	void sig_enableGame( bool );
		
	void sig_statusBar();
	
	void sig_newMessage( const QString & );

	void sig_baseReinit();
	
	void sig_lordReinit();
	
	void sig_cellChanged( int , int );
	
	void sig_clearPath();
	
	void sig_redrawMap( Map * );
	
	void sig_updateWidget();
	
	void sig_baseUpdate();

	void sig_modifBaseBuilding( int, int, uchar, bool );

	void sig_tavernInfo( char );
				
	void sig_tavernLord( int, bool );
	
public slots:
	/** Slot for displaying lord */
	void slot_displayLord();

	/** Slot for displaying base */
	void slot_displayBase();
	
	void slot_lordSelected( GenericLord * lord );
	
	void slot_baseSelected();

	/** */
	void slot_message( QString );

	void slot_gameInfo();

	void slot_sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );
	
	void slot_sendExchangeUnitSplit( GenericLord * lord, int unit, int num1, int pos2, int num2  );

	void slot_sendLordUnit( GenericLord * lord, int num, int value );
	
	void slot_sendExchangeArtefact( GenericLord * lord1, int item, GenericLord * lord2 );
	
	void updateAnimations();
	
	void updateDispositionMode();
	
	void unSelectLord();
	
	/** Activate next lord */
	void nextLord();
	
private slots:
	void endTurn();
	void slot_mouseMoved( GenericCell *cell );
	void slot_mouseLeftPressed( GenericCell * cell );
	void slot_mouseRightPressed( GenericCell * cell );
	void slot_mouseReleased();
	void sendEndTurn();
	void gotoLord();

protected:
	void timerEvent(QTimerEvent *event);

private:
	inline uchar getCla1();
	inline uchar getCla2();
	inline uchar getCla3();

	void initWidgets();

	void handleClickNothing( GenericCell * cell );
	void handleClickLord( GenericCell * cell );
	void handleClickBase( GenericCell * cell );

	/** Manage socket SO_MSG */
	void socketMsg();

	/** Manage socket SO_GAME */
	void socketGame();
	/** Begin game with 'nb' players */
	void socketGameBegin();
	void socketGameLost();
	void socketGameWin();
	void socketGameInfo();
	void socketGameInfoTeam();
	void socketGameCalendar();
	void socketGameTavern();

	void socketGameTavernInfo();
	void socketGameTavernLord();

	/** Manage socket SO_TURN */
	void socketTurn();
	void socketTurnPlay();

	/** Manage socket SO_MVT */
	void socketMvt();

	/** Manage socket SO_MODIF */
	void socketModif();
	void socketModifCell();
	void socketModifLord();
	void socketModifBase();
	void socketModifBuilding();
	void socketModifArtefact();
	void socketModifPlayer();
	void socketModifCreature();
	void socketModifMap();

	void socketModifEvent();
	void socketEventNew();
	void socketEventDel();

	void socketModifLordVisit();
	void socketModifLordNew();
	void socketModifLordUnit();
	void socketModifLordRemove();
	void socketModifLordCharac();
	void socketModifLordGarrison();
	void socketModifLordMachine();

	void socketModifBaseNew();
	void socketModifBaseOwner();
	void socketModifBaseName();
	void socketModifBaseBuilding();
	void socketModifBaseUnit();
	void socketModifBaseResources();
	void socketModifBaseProduction();
	void socketModifBaseState();
	
	void socketModifBuildingNew();
	void socketModifBuildingOwner();
	void socketModifBuildingResources();
		
	void socketModifCreatureNew();
	void socketModifCreatureUpdate();
	void socketModifCreatureRemove();
	void socketModifCreatureRess();

	/** Manage socket SO_CONNECT */
	void socketConnect();
	void socketConnectOk();
	void socketConnectId();
	void socketConnectName();

	/** Manage socket SO_EXCH */
	void socketExchange();
	void exchangeStart();
	void exchangeUnits();
	void exchangeArtefact();	
	void exchangeBaseUnits();

	void socketQR();
	void socketQRMsgNext();
	void socketQRMsgEnd();
	void socketQRLevel();
	void socketQRChest();
	void socketQRCreature(QRCreature state);

	void socketFight();
	void socketFightInit();
	void socketFightEnd();

	AttalSocket * _socket;
	GenericCell * _currentCell;
	bool _isPlaying;
	QString _msg;
	bool _redraw;
	int _idTime, _period;
	QString _scenDesc;

	MapState _state;
};


#endif // GAME_H


