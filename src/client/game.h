/****************************************************************
**
** Attal : Lords of Doom
**
** game.h
** Manages the whole game
**
** Version : $Id: game.h,v 1.46 2003/01/24 18:02:44 lusum Exp $
**
** Author(s) : Pascal Audoux
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
#include <qwidget.h>
#include <qlabel.h>
#include <qdialog.h>

// application specific includes
#include "client/player.h"
#include "client/cell.h"
#include "client/map.h"
#include "client/miniMap.h"
#include "client/gameControl.h"
#include "client/gameDescription.h"

#include "common/attalSocket.h"


class Calendar;
class ChatWidget;
class DisplayLord;
class GameInfo;
class GenericBase;
class LordExchange;
class RessourceBar;

class QPixmap;
class QVBoxLayout;

/*              ------------------------------
 *                         Game
 *              ------------------------------ */


/** comment for the class */
class Game : public QWidget, public GameDescription
{
	Q_OBJECT
public:
	/** Constructor */
	Game( QWidget * parent = 0, const char * name = 0 );
	
	/** Destructor : delete player and lord */
	~Game();

	/** Associate a socket to the Game (for exchange with the server) */
	void setSocket( AttalSocket * sock ) {
		_socket = sock;
	}

	/** Lord 'lord' enters in base 'base' */
	void enter( GenericLord * lord, GenericBase * base );

	/** Lord 'lord' enters in building 'build' */
	void enter( GenericLord * lord, GenericBuilding * building );

	/** Handle socket data */
	void handleSocket();

	/** Begin game with 'nb' players */
	void beginGame( int nb );

	/** End game */
	void endGame();

	/** Begin turn */
	void beginTurn();

	/** End turn */
	void endTurn();

	/** Player active is player 'num' */
	void playerActive( char num );

	/** Activate next lord */
	void nextLord() { _player->nextLord(); }

	/** Return the player associated to the game */
	Player * getPlayer() { return _player; }

	/** Sets the name of the player */
	void setPlayerName( QString name );

	enum MapState {
		MS_NOTHING,
		MS_LORD,
		MS_BASE,
		MS_TECHNIC
	};

	void setState( MapState state ) { _state = state; }

	MapState getState() { return _state; }

	void displayMiniMap( bool state );
	
signals:
	void sig_base( GenericBase * );
	void sig_fight( GenericLord * lord, CLASS_FIGHTER cla );

public slots:
	/** Slot for displaying lord */
	void slot_displayLord();

	/** Slot for displaying base */
	void slot_displayBase();

	/** */
	void slot_message( QString );

private slots:
	void slot_endTurn() { endTurn(); }
	void slot_mouseMoved( Cell *cell );
	void slot_mouseLeftPressed( Cell * cell );
	void slot_mouseRightPressed( Cell * cell );

private:
	void handleClickNothing( Cell * cell );
	void handleClickLord( Cell * cell );
	void handleClickBase( Cell * cell );
	void handleClickTechnic( Cell * cell );

	/** Manage socket SO_MSG */
	void socketMsg();

	/** Manage socket SO_GAME */
	void socketGame();

	/** Manage socket SO_TURN */
	void socketTurn();

	/** Manage socket SO_MVT */
	void socketMvt();

	/** Manage socket SO_MODIF */
	void socketModif();
	void socketModifLord();
	void socketModifBase();
	void socketModifBuilding();
	void socketModifArtefact();
	void socketModifPlayer();
	void socketModifCreature();
	void socketModifEvent();

	/** Manage socket SO_CONNECT */
	void socketConnect();

	/** Manage socket SO_EXCH */
	void socketExchange();
	void socketModifLordVisit();
	void socketModifLordNew();
	void socketModifLordUnit();
	void socketQR();
	void socketFight();

	void exchangeUnits();
	
	void exchangeBaseUnits();

	Player * _player;
	AttalSocket * _socket;
	Map * _map;
	RessourceBar * _ressourceBar;
	GameControl * _control;
	GameInfo * _gameInfo;
	DisplayLord * _dispLord;
	LordExchange * _lordExchange;
	MiniMap * _miniMap;
	ChatWidget * _chat;
	bool _isPlaying;

	MapState _state;
};

/** Dialog for displaying message during the game */
class GameMessage : public QDialog
{
public:
	/** Constructor */
	GameMessage( QWidget * parent = 0, const char * name = 0 );

	/** Adds text to the dialog */
	void addText( QString text );

	/** Adds pixmap to the dialog */
	void addPixmap( QPixmap * pixmap );

protected:
	QVBoxLayout * _layout;

};


#endif // GAME_H


