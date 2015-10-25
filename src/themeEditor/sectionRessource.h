/****************************************************************
**
** Attal : Lords of Doom
**
** sectionRessource.h
** section specifying ressources
**
** Version : $Id: sectionRessource.h,v 1.4 2003/05/09 12:45:25 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/06/2001
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

 
#ifndef SECTIONRESSOURCE_H
#define SECTIONRESSOURCE_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
// application specific include files
#include "themeEditor/sectionWidget.h"

class QListBox;
class QPushButton;
class RessourcePresentation; 

/*              ------------------------------
 *                         SectionRessource
 *              ------------------------------ */


/** comment for the class */
class SectionRessource : public GenericSection
{
 
public:
	/** Constructor */
	SectionRessource( QWidget * parent = 0, const char * name = 0 );

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

private:
	int _num;
	AskString * _name;
	AskPixmap * _icon, * _smallIcon;
};

#endif // SECTIONRESSOURCE_H
