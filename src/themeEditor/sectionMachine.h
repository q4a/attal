/****************************************************************
**
** Attal : Lords of Doom
**
** sectionMachine.h
** section for specifying war machines
**
** Version : $Id: sectionMachine.h,v 1.2 2003/05/09 19:09:22 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/11/2002
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


#ifndef SECTIONMACHINE_H
#define SECTIONMACHINE_H


// generic include files
// include files for QT
// application specific include files
#include "sectionWidget.h"

/*              ------------------------------
 *                         SectionArtefact
 *              ------------------------------ */



/** comment for the class */
class SectionMachine : public GenericSection
{
//	Q_OBJECT
public:
	/** Constructor */
	SectionMachine( QWidget * parent = 0, const char * name = 0 );

	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the nextt item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates a new item */
	void selectNew();

	/** Deletes the current item */
	void selectDel();

	/** Clears the section */
	void clear();

	/** Inits the section */
	void init();

	/** Saves the section */
	void save();

private:
	AskString * _name;
	AskInt * _type, * _param1;
	AskPixmap * _icon, * _item;
	AskIntList * _params;
	int _num;
};

#endif // SECTIONARTEFACT_H
