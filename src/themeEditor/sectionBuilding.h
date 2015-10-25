/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBuilding.h
** section specifying buildings
**
** Version : $Id: sectionBuilding.h,v 1.3 2002/10/20 15:14:40 audoux Exp $
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

 
#ifndef SECTIONBUILDING_H
#define SECTIONBUILDING_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "sectionWidget.h"
 
/*              ------------------------------
 *                         SectionBuilding
 *              ------------------------------ */



/** comment for the class */
class SectionBuilding : public GenericSection
{
 
public:
	/** Constructor */
	SectionBuilding( QWidget * parent = 0, const char * name = 0 );

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

	/** Inits the section */
	void init();

	/** Clears the section */
	void clear();

	/** Saves the section */
	void save();


private:
	AskString * _name, * _description;
	AskInt * _type;
	AskPixmap * _item;

	int _num;
};
 
#endif // SECTIONBUILDING_H   
