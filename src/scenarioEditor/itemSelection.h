/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelection.h
** Allow selection of specific item panels
**
** Version : $Id: itemSelection.h,v 1.7 2002/11/14 21:17:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 22/12/2001
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

 
#ifndef ITEMSELECTION_H
#define ITEMSELECTION_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlist.h>
// application specific include files

class Cell;
class GameData;

/** Generic class for selecting items */
class ItemSelection : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ItemSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	virtual void handleLeftClick( Cell * ) {}

	/** Sets theme data to the selection panel */
	void setGameData( GameData * data ) { _data = data; }

public slots:
	/** Slot button pressed */
	void slot_button( int nb );

protected:
	int _button;
	QList<QPushButton> _buttonList;
	GameData * _data;
};

/** Selection of tiles */
class TileSelection : public ItemSelection
{
public:
	/** Constructor */
	TileSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of transition */
class TransitionSelection : public ItemSelection
{
public:
	/** Constructor */
	TransitionSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of decoration */
class DecorationSelection : public ItemSelection
{
public:
	/** Constructor */
	DecorationSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of a building */
class BuildingSelection : public ItemSelection
{
public:
	/** Constructor */
	BuildingSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of a base */
class BaseSelection : public ItemSelection
{
public:
	/** Constructor */
	BaseSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of a lord */
class LordSelection : public ItemSelection
{
public:
	/** Constructor */
	LordSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );

private:
	int _num;
};

/** Selection of an artefact */
class ArtefactSelection : public ItemSelection
{
public:
	/** Constructor */
	ArtefactSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};

/** Selection of a creature */
class CreatureSelection : public ItemSelection
{
public:
	/** Constructor */
	CreatureSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( Cell * );
};


#endif // ITEMSELECTION_H
