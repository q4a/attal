/****************************************************************
**
** Attal : Lords of Doom
**
** sectionTeam.h
** section for specifying teams (flag, color)
**
** Version : $Id: sectionTeam.h,v 1.2 2002/10/20 15:14:40 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2001
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

 
#ifndef SECTIONTEAM_H
#define SECTIONTEAM_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "sectionWidget.h"
 
/*              ------------------------------
 *                         SectionTeam
 *              ------------------------------ */



/** comment for the class */
class SectionTeam : public GenericSection
{
	Q_OBJECT
public:
	/** Constructor */
	SectionTeam( QWidget * parent = 0, const char * name = 0 );

	/** Selects the first item */
	void selectFirst();

	/** Selects teh previous item */
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
        void slot_color();


private:
	int _num;
	AskString * _name;
	QPushButton * _pbColor;
	int _red, _green, _blue;
	AskPixmap * _flag;
};

#endif // SECTIONDECORATION_H
