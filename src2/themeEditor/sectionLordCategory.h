/****************************************************************
**
** Attal : Lords of Doom
**
** sectionLordCategory.h
** section for editing lord categories
**
** Version : $Id: sectionLordCategory.h,v 1.3 2009/09/11 13:00:05 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/05/2004
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

 
#ifndef SECTIONLORDCATEGORY_H
#define SECTIONLORDCATEGORY_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "themeEditor/sectionWidget.h"

 
/*              ------------------------------
 *                    SectionLordCategory
 *              ------------------------------ */


class SectionLordCategory: public GenericSection
{
 	Q_OBJECT
public:
	/** Constructor */
	SectionLordCategory( QWidget * parent = 0, const char * name = 0 );

	/** Updates data */
	void updateData();

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

	/** Initializes the section */
	void init();

	/** Saves the data */
	void save();

signals:
	/** Signal when stg change */
	void sig_changed();

protected:
	int _num;
	AskString * _name, * _description;
	AskCombo * _race;
	AskInt * _attack, * _defense;
};
 
#endif // SECTIONLORDCATEGORY_H
