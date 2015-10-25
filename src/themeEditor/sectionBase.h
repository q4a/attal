/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBase.h
** section specifying bases
**
** Version : $Id: sectionBase.h,v 1.4 2003/04/11 10:11:45 audoux Exp $
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

 
#ifndef SECTIONBASE_H
#define SECTIONBASE_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
// application specific include files
#include "themeEditor/sectionWidget.h"


class SectionInsideBuilding;

/*              ------------------------------
 *                         SectionBase
 *              ------------------------------ */



/** comment for the class */
class SectionBase : public GenericSection
{
 
public:
	/** Constructor */
	SectionBase( QWidget * parent = 0, const char * name = 0 );

	void updateData();

	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates a new item */
	void selectNew();

	/** Deletes the current item */
	void selectDel();

	/** Saves the data */
	void save();

	/** Inits the section */
	void init();

	void clear();
private:
	int _idBase;
	AskString * _name;
	AskInt * _population;
	SectionInsideBuilding * _inside;
};

class SectionInsideBuilding : public QWidget
{
	Q_OBJECT
public:
	SectionInsideBuilding( QWidget * parent = 0, const char * name = 0 );

	void setBase( int base );

	/** Saves the data */
	void save();

	/** Inits the section */
	void init();

	void clear();

public slots:
	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates a new item */
	void selectNew();

	/** Deletes the current item */
	void selectDel();

protected:
	int _idBase, _idBuild;
	SelectionWidget * _select;
	AskString * _name, * _description;
	AskInt * _x, * _y;
};

#endif // SECTIONBASE_H
