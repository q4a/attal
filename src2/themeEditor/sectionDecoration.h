/****************************************************************
**
** Attal : Lords of Doom
**
** sectionDecoration.h
** section for specifying decorations
**
** Version : $Id: sectionDecoration.h,v 1.8 2005/12/10 13:34:41 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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


#ifndef SECTIONDECORATION_H
#define SECTIONDECORATION_H


// generic include files
// include files for QT
#include <QWidget>
// application specific include files
#include "sectionWidget.h"

class DecorationEffect;
class MapDispositionEditor;

/*              ------------------------------
 *                         SectionDecoration
 *              ------------------------------ */

class SectionDecoration : public GenericSection
{
	Q_OBJECT
public:
	/** Constructor */
	SectionDecoration( QWidget * parent = 0, const char * name = 0 );

	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates new item */
	void selectNew();

	/** Deletes current item */
	void selectDel();

	/** Clears the section */
	void clear();

	/** Initialize the section */
	void init();

	/** Saves the data */
	void save();
	
public slots:
	void slot_firstItem();
	void slot_previousItem();
	void slot_nextItem();
	void slot_lastItem();
	void slot_newItem();
	void slot_delItem();

private:
	AskString * _name, * _info;
	AskCombo * _moveEffect, * _technicEffect;
	AskPixmap * _image;
	DecorationEffect * _decorationEffect;
	SelectionWidget * _selectItem;
	MapDispositionEditor * _dispo;

	int _num;
	int _numItem;
};

#endif // SECTIONDECORATION_H
