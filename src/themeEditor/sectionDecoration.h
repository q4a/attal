/****************************************************************
**
** Attal : Lords of Doom
**
** sectionDecoration.h
** section for specifying decorations
**
** Version : $Id: sectionDecoration.h,v 1.3 2002/12/15 12:56:59 audoux Exp $
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
// application specific include files
#include "sectionWidget.h"
 
/*              ------------------------------
 *                         SectionDecoration
 *              ------------------------------ */



/** comment for the class */
class SectionDecoration : public GenericSection
{
	//Q_OBJECT
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
	
private:
	AskString * _name, * _info;
	AskCombo * _moveEffect, * _technicEffect;
	AskPixmap * _image;
	
	int _num;
};
 
#endif // SECTIONDECORATION_H   
