/****************************************************************
**
** Attal : Lords of Doom
**
** gameControl.h
** Widget on the right, controlling the game
**
** Version : $Id: gameControl.h,v 1.19 2009/09/05 17:51:21 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/09/2000
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

 
#ifndef GAMECONTROL_H
#define GAMECONTROL_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QWidget>

// application specific includes
#include "libClient/player.h"
 
class QPushButton;
class QSignalMapper;

class AttalButton;
class CentralControl;
class ScrollLord;
class ScrollBase;
 
/*              ------------------------------
 *                         GameControl
 *              ------------------------------ */


class GameControl : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	GameControl( QWidget * parent = 0, Player * player = 0 );
	
	/** Reinit widgets */
	void reinit();
	
	/** Select first lord (if none : first base) */
	void selectFirst();

	/** Disable if no game is running */
	void disableGame();
	
public slots:
	/** Slot when lord selected */
	void slot_lordSelected();
	
	/** Slot when base selected */
	void slot_baseSelected();
	
	/** Activate if a game is running */
	void enableGame();
	
signals:
	/** Signal if lord selected clicked */
	void sig_lord();
	
	/** Signal if base selected clicked */
	void sig_base();
	
	/** Signal if 'End Turn' button clicked */
	void sig_endTurn();
	
	void sig_gotoLord();

	void sig_nextLord();
	
	void sig_enableGame( bool );

private:
	CentralControl * _cen;
	ScrollLord * _scrL;
	ScrollBase * _scrB;
	Player * _player;
};

/**Central widget of the GameControl (other actions) */
class CentralControl : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	CentralControl( QWidget * parent = 0, Player * player = 0 );

	/** Disable panel (not in game) */
	void disableGame();
	
	/** Enable panel (in game) */
	void enableGame();
	
signals:
	/** Signal if 'End Turn' button is clicked */
	void sig_endTurn();
	
	void sig_gotoLord();
	
	void sig_nextLord();

//private slots:
//	void slot_nextLord() { _player->nextLord(); }
	
private:
	Player * _player;
	QPushButton * _pbNext, * _pbTurn;
};

/** Scrolling list of buttons */
class ScrollList : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ScrollList( bool horizontal, uint nbItem, QWidget * parent = 0, Player * player = 0 );

	~ScrollList();

	/** Reinit info displayed */
	virtual void reinit() =0;

	/** Select a button in the list */
	void select( int );

	/** Deselect all the buttons */
	void deselect();

	void repaintButtons(uint num);

public slots:
	/** Slot when 'up' arrow is clicked */
	void slot_up();

	/** Slot when 'down' arrow is clicked */
	void slot_down();
	
	void slot_reinit();
	
	/** Enable 'in game' features */
	void enableGame();

	/** Slot when a button is clicked */
	virtual void slot_clicked( int ) {}

protected:
	virtual uint getListCount() = 0;

	QSignalMapper * _sigmap;
	AttalButton ** _buttons;
	Player * _player;
	uint _current;
	int _selected;
	bool _horizontal;
	uint _nbItem;
};

/** Scrolling list of lords */
class ScrollLord : public ScrollList
{
	Q_OBJECT
public:
	/** Constructor */
	ScrollLord( bool horizontal, uint nbItem, QWidget * parent = 0 , Player * player = 0  );

	/** Reinit info displayed */
	void reinit();

	/** Select button 'num' */
	void select( int num );

	/** Deselect all buttons */
	void deselect();

	uint getListCount();

signals:
	/** Signal when lord is clicked */
	void sig_lord();

	/** Signal when lord is selected */
	void sig_lordSelected( GenericLord *);

	void sig_unSelectLord();

public slots:
	/** Slot when a button is clicked */
	void slot_clicked( int );

protected:
	QList<GenericLord *> _lordList;

};

/** Scrolling list of bases */
class ScrollBase : public ScrollList
{
	Q_OBJECT
public:
	/** Constructor */
	ScrollBase( bool horizontal, uint nbItem, QWidget * parent = 0 , Player * player = 0 );
	
	/** Reinit info displayed */
	void reinit();	
	
	/** Select button 'num' */
	void select( int num );
	
	/** Deselect all the buttons */
	void deselect();

	uint getListCount();	
	
signals:
	/** Signal when base is clicked */
	void sig_base();
	
	/** Signal when base is selected */
	void sig_baseSelected();
	
	void sig_unSelectLord();
	
public slots:
	/** Slot when a button is clicked */
	void slot_clicked( int );


};

#endif // GAMECONTROL_H   
