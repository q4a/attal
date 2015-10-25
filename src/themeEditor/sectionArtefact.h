/****************************************************************
**
** Attal : Lords of Doom
**
** sectionArtefact.h
** section for specifying artefacts
**
** Version : $Id: sectionArtefact.h,v 1.2 2002/10/20 15:14:40 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 20/01/2002
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

 
#ifndef SECTIONARTEFACT_H
#define SECTIONARTEFACT_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "sectionWidget.h"
 
/*              ------------------------------
 *                         SectionArtefact
 *              ------------------------------ */



/** comment for the class */
class SectionArtefact : public GenericSection
{
	Q_OBJECT
public:
	/** Constructor */
	SectionArtefact( QWidget * parent = 0, const char * name = 0 );

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
	AskInt * _position;
	AskPixmap * _icon, * _item;
	
	int _num;
};
 
#endif // SECTIONARTEFACT_H   
