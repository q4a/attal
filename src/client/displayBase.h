/****************************************************************
**
** Attal : Lords of Doom
**
** displayBase.h
** inside the base
**
** Version : $Id: displayBase.h,v 1.20 2002/10/20 20:21:50 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/09/2000
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

 
#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H
 
 
// generic include files
// include files for QT
#include <qdialog.h>
#include <qlabel.h>
// application specific includes
#include "common/unit.h"
#include "client/displayCreature.h"
#include "client/player.h"
#include "client/widget.h"

class AttalSocket;
class Game;
class GenericBase;
class GenericLord;
class GenericInsideBuilding;
class BaseSummary;
class BaseLords;
class BaseButtons;
class UnitSummary;
class BaseTroop;
class InsideBase;

/*              ------------------------------
 *                         DisplayBase
 *              ------------------------------ */

#define NB_CREATURE_BASE 7

/** Display inside of a base */
class DisplayBase : public QWidget
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayBase( QWidget * parent = 0, const char * name = 0 );

	/** Initialize the player */
	void setPlayer( Player * player );

	/** Initialize the base to display */
	void setBase( GenericBase * base );

	/** Reinits the display of the base */
	void reinit();

	/** Set socket for sending infos to server */
	void setSocket( AttalSocket * sock );

	/** Gives game info to this object */
	void setGame( Game * game ) { _game = game; }

	/** Handles socket's data */
	void handleSocket();

signals:
	/** Signal for quitting */
	void sig_quit();

public slots:
	/** Slot when a building is clicked */
	void slot_building( GenericInsideBuilding * );

private:
	void actionAllBuildings();

	void actionAllCreatures();

	void actionSomeCreatures( GenericInsideBuilding * building );

	void actionMarket();

	Player * _player;
	GenericBase * _base;
	AttalSocket * _socket;
	BaseSummary * _baseSummary;
	BaseLords * _baseLords;
	BaseButtons * _baseButtons;
	InsideBase * _inside;
	Game * _game;
};

/** Widget for displaying a summary of the base info */
class BaseSummary : public QWidget
{
public:
	/** Constructor */
	BaseSummary( QWidget * parent = 0, const char * name = 0 );

	/** Initialize with the current base to display */
	void setBase( GenericBase * base );

	void reinit();

	/** Initialize the player */
	void setPlayer( Player * player );

private:
	UnitSummary * _unitIco[8];
	GenericBase * _base;
	Player * _player;
};

/** Displays the lors of the base */
class BaseLords : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	BaseLords( QWidget * parent = 0, const char * name = 0 );

	/** Initialize with the current base to display */
	void setBase( GenericBase * base );

	/** Reinit the widget */
	void reinit();

	/** Initialize the player */
	void setPlayer( Player * _player );

	/** Tell if there is a visiting lord */
	bool isVisitor() {
		return (_visitor == 0);
	}

	/** Sets the socket for requesting the server */
	void setSocket( AttalSocket * socket ) {
		_socket = socket;
	}

public slots:
	/** Slot for exchange button */
	void slot_exchange();

	/** Slot if the garrison lord is clicked */
	void slot_photoGarrison();

	/** Slot if the visitor lord is clicked */
	void slot_photoVisitor();

	/** Slot if the 'num'-th unit of garrison is clicked */
	void slot_unitGarrison( int num );

	/** Slot if the 'num'-th unit of visit is clicked */
	void slot_unitVisitor( int num );

signals:
	/** Signal for exchanging lords */
	void sig_exchange( bool st );

private:
	void unselectUnits();
	void unselectPhoto();
	void exchangeLords();
	void exchangeUnitsGV();
	void exchangeUnitsVG();
	void exchangeUnitsGG( int u1, int u2 );
	void exchangeUnitsVV( int u1, int u2 );

	bool _isExchange;
	BaseTroop * _garrison, * _visitor;
	GenericBase * _base;
	Player * _player;
	bool _garrisonSelected, _visitorSelected;
	int _unitGarrison, _unitVisitor;
	AttalSocket * _socket;
};

/** Button for changing of base */
class BaseButtons : public QWidget
{
	Q_OBJECT
public:
	/** Constructors */
	BaseButtons( QWidget * parent = 0, const char * name = 0 );

	/** Initialize the player */
	void setPlayer( Player * player ) { _player = player; }

public slots:
	/** Slot for exchanging lords */
	void slot_exchange( bool st );

signals:
	/** Signal for quitting */
	void sig_quit();

	/** Signal for exhanging */
	void sig_exchange();

private:
	Player * _player;
	QPushButton * _pbExch;

};

/** Summary of units of a lord */
class UnitSummary : public QWidget
{
public:
	/** Constructor */
	UnitSummary( QWidget * parent = 0, const char * name = 0 );

	/** Initialize the creature to display */
	void setCreature( int id );

	/** Initialize the player */
	void setPlayer( Player * player ) { _player = player; }

	/** Det the number of creatures */
	void setNumber( int num );

private:
	QLabel * _ico, * _nb;
	Player * _player;
};

/** Display troops inside base */
class BaseTroop : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	BaseTroop( bool isGarrison, QWidget * parent = 0, const char * name = 0 );

	/** Clear BaseTroop Widget */
	void clear();

	/** Initialize the player */
	void setPlayer( Player * player ) { _player = player; }

	/** Initialize the lord to display */
	void setLord( GenericLord * lord );

	/** Initialize the base to display */
	void setBase( GenericBase * base );

	/** Reinit the widget (to synchronize change) */
	void reinit();

	/** Select the 'photo' widget */
	void selectPhoto() {}

	/** Unselect the 'photo' widget */
	void unselectPhoto() {}

	/** Select unit 'num' */
	void selectUnit( int num ) {}

	/** Unselect all units */
	void unselectUnits() {}

signals:
	/** Signal if photo clicked */
	void sig_photo();

	/** Signal if unit clicked */
	void sig_unit( int );

private:
	Icon * _photo;
	Icon * _units[MAX_UNIT];
	bool _isGarrison;
	GenericLord * _lord;
	GenericBase * _base;
	Player * _player;
};

#endif // DISPLAYBASE_H
