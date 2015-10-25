/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelection.h
** Allow selection of specific item panels
**
** Version : $Id: itemSelection.h,v 1.21 2007/05/08 15:33:57 lusum Exp $
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
#include <QButtonGroup>
#include <QPolygon>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>

// application specific include files

class QComboBox;
class QStackedWidget;

class GenericCell;
class GraphicalGameData;
class Screen;

/** Generic class for selecting items */
class ItemSelection : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ItemSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	virtual void handleLeftClick( GenericCell * ) {}

	/** Sets theme data to the selection panel */
	void setGameData( GraphicalGameData * data ) { _data = data; }
	
	bool isPlace(GenericCell * cell);

protected:
	void addButton( QPixmap  pixmap, int pos, bool breakLine = false );
	void addButton( const QString& text, int pos, bool breakLine = false );
	void addNewWidget( QWidget * button, bool breakLine = false );
	void addNewLine();
	int button();
	GraphicalGameData * _data;
protected:
	QButtonGroup *_bg;
	int _currentX;
	int _currentY;
	int _currentHeight;
};

/** Selection of transition */
class TransitionSelection : public ItemSelection
{
public:
	/** Constructor */
	TransitionSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of decoration group */
class DecorationGroupSelection : public ItemSelection
{
	Q_OBJECT
public:
	/** Constructor */
	DecorationGroupSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
	
public slots:
	void slot_selectGroup( int group );
	
protected:
	QComboBox * _groups;
	QStackedWidget * _stack;
};


/** Selection of decoration */
class DecorationSelection : public ItemSelection
{
	Q_OBJECT
public:
	/** Constructor */
	DecorationSelection( uint group, QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
	
protected:
	uint _group;
	bool _isRoad;
	bool _isTree;
};

/** Selection of a building */
class BuildingSelection : public ItemSelection
{
public:
	/** Constructor */
	BuildingSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of a base */
class BaseSelection : public ItemSelection
{
public:
	/** Constructor */
	BaseSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of a lord */
class LordSelection : public ItemSelection
{
public:
	/** Constructor */
	LordSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );

};

/** Selection of an artefact */
class ArtefactSelection : public ItemSelection
{
public:
	/** Constructor */
	ArtefactSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of a bonus/chest */
class BonusSelection : public ItemSelection
{
public:
	/** Constructor */
	BonusSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of a creature */
class CreatureSelection : public ItemSelection
{
public:
	/** Constructor */
	CreatureSelection( QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );
};

/** Selection of a tool */
class ToolSelection : public ItemSelection
{
public:
	/** Constructor */
	ToolSelection( Screen * screen, QWidget * parent = 0, const char * name = 0 );

	/** Manage left click */
	void handleLeftClick( GenericCell * );

protected:
	Screen * _screen;

	QPolygon _path;
	
	void findPath ( QPoint& point1, QPoint& point2 );
};


#endif // ITEMSELECTION_H
