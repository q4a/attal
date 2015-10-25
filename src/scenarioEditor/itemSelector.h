/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelector.h
** Allow selection of item panels
**
** Version : $Id: itemSelector.h,v 1.6 2002/10/20 20:21:50 audoux Exp $
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
#include <qwidget.h>
#include <qscrollview.h>
// application specific include files
 
 
/*              ------------------------------
 *                         ItemSelector
 *              ------------------------------ */

class QComboBox;
class QWidgetStack;
class Cell;
class GameData;
class ItemSelection;

/** comment for the class */

class ItemSelector : public QWidget
{

public:
	/** Constructor */
	ItemSelector( GameData * data, QWidget * parent = 0, const char * name = 0 );

	void handleLeftClick( Cell * );

private:
	void init();
	QScrollView * view;
	QComboBox * _selector;
	QWidgetStack * _stack;
	GameData * _data;
};

/** Scrollview for ItemSelection objects */
class ItemScroll : public QScrollView
{
public:
	ItemScroll( QWidget * parent = 0 );

	void addItem( ItemSelection * item );
	
	void handleLeftClick( Cell * );
	
private:
	ItemSelection * _item;
};

 
#endif // ITEMSELECTOR_H   
