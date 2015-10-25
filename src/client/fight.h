/****************************************************************
**
** Attal : Lords of Doom
**
** fight.h
** Manages the whole game
**
** Version : $Id: fight.h,v 1.26 2003/02/03 22:21:28 audoux Exp $
**
** Author(s) : Cyrille Verrier
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


#ifndef FIGHT_H
#define FIGHT_H


// include files for QT
#include <qwidget.h>
#include <qlabel.h>
#include <qlist.h>

// application specific includes
#include "common/attalSocket.h"
#include "client/lord.h"
#include "client/fightCell.h"
#include "client/fightMap.h"
#include "client/fightUnit.h"

class FightControl;
class FightMapView;
class Game;
class PopupUnit;
class ChatWidget;

/*              ------------------------------
 *                         Fight
 *              ------------------------------ */


/** comment for the class */
class Fight : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	Fight( QWidget * parent = 0, const char * name = 0 );

	/** delete player and lord */
	~Fight();

	/** Reinit widget */
	void reinit();

	/** Set socket for sending infos to the server */
	void setSocket( AttalSocket * sock ) {
		_socket = sock;
	}

	void setGame( Game * game ) { _game = game; }

	void handleSocket();

	void setOpponent( GenericLord * lord );

	void setLord( GenericLord * lord, CLASS_FIGHTER fighter );

	void setUnit( GenericFightUnit * unit, CLASS_FIGHTER fighter, int num );

	GenericFightUnit * getUnit( int num, CLASS_FIGHTER fighter );

	bool isAttack() { return _isAttack; }

	GenericLord * getAttack() { return _lordAtt; }

	GenericLord * getDefense() { return _lordDef; }

	//int *getLostAtt() { return (int *)_lostAtt; }

	//int *getLostDef() { return (int *)_lostDef; }

	uint getCasualtiesNumber( CLASS_FIGHTER fighter );

	GenericFightUnit * getCasualtiesUnit( CLASS_FIGHTER fighter, int numUnit );

signals:
	void sig_quit();

private slots:
	/** slot for managing mouse movements */
	void slot_mouseMoved( FightCell * cell );

	/** slot for managing right click */
	void slot_mouseRightPressed( FightCell * cell );

	/** slot for managing left click */
	void slot_mouseLeftPressed( FightCell * cell );

	/** slot for managing button release */
	void slot_mouseReleased();

	void slot_wait( void );

	void slot_flee( void );

	void slot_defend( void );

private:
	void socketMsg();

	void socketFight();

	void socketFightCell();

	void socketFightModify();

	void socketFightMove();

	void socketFightDamage();

	void setActive( CLASS_FIGHTER, int );

	void moveUnit( FightCell * cell );

	int giveNum( GenericFightUnit * unit );

	CLASS_FIGHTER giveClass( GenericFightUnit * unit );

	bool isOpponent( GenericFightUnit * unit );

	void addCasualties( CLASS_FIGHTER fighter, int race, int level, uint nb );

	void showFightResults( char result );

	void updateUnits( void );

	bool _isAttack;
	bool _isActive;
	bool _isCreature;
	GenericLord * _lordAtt, * _lordDef;

	//int _lostAtt[ MAX_UNIT ], _lostDef[ MAX_UNIT ];
	QList<GenericFightUnit> * _lostAttack, * _lostDefense;
	
	FightUnit * _activeUnit;
	AttalSocket * _socket;
	FightMap * _map;
	FightMapView * _view;
	FightUnit * _unitsAtt[MAX_UNIT], * _unitsDef[MAX_UNIT];
	FightControl * _control;
	PopupUnit * _popup;
	Game * _game;
};

/** Controlboard in fight mode */
class FightControl : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	FightControl( QWidget * parent = 0, const char * name = 0 );

	/** Displays new message */
	void newMessage( QString msg );

	/** Clear the message box */
	void clear();

private:

	ChatWidget * _chat;

public slots:
	/** Slot if 'Wait' button pressed */
	void slot_waitPressed( void ) { emit sig_wait(); }

	/** Slot if 'Flee' button pressed */
	void slot_fleePressed( void ) { emit sig_retreat(); }

signals:
	/** Signal 'Spell' button clicked */
	void sig_useSpell();

	/** Signal 'Quit' button clicked */
	void sig_quit();

	/** Signal 'Autofight' button clicked */
	void sig_autoFight();

	/** Signal 'Wait' button clicked */
	void sig_wait();

	/** Signal 'Defend' button clicked */
	void sig_defend();

	/** Signal 'Retreat' button clicked */
	void sig_retreat();

	/** Signal 'Surrender' button clicked */
	void sig_surrender();
};

#endif // FIGHT_H
