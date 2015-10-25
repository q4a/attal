/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelector.h
** Allow selection of item panels
**
** Version : $Id: itemSelector.h,v 1.8 2007/05/08 15:34:01 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 21/12/2001
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

 
#ifndef ITEMSELECTOR_H
#define ITEMSELECTOR_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QScrollArea>
// application specific include files


/*              ------------------------------
 *                         ItemSelector
 *              ------------------------------ */

class QComboBox;
class QStackedWidget;

class GenericCell;
class GraphicalGameData;
class ItemSelection;
class Screen;

/** comment for the class */

class ItemSelector : public QWidget
{

public:
	/** Constructor */
		ItemSelector( GraphicalGameData * data, Screen * screen = 0, const char * name = 0 );

		void handleLeftClick( GenericCell * );

private:
		enum ItemType {
			ItemTile = 0,
			ItemTransition,
			ItemDecoration,
			ItemBuilding,
			ItemBase,
			ItemLord,
			ItemArtefact,
			ItemBonus,
			ItemCreature,
			ItemTool
		};

		void init();

		QScrollArea * view;
		QComboBox * _selector;
		QStackedWidget * _stack;
		GraphicalGameData * _data;
		Screen * _screen;
};

/** Scrollview for ItemSelection objects */
class ItemScroll : public QScrollArea
{
public:
	ItemScroll( QWidget * parent = 0 );

	void addItem( ItemSelection * item );
	
	void handleLeftClick( GenericCell * );
	
private:
	ItemSelection * _item;
};

 
#endif // ITEMSELECTOR_H   
