/****************************************************************
**
** Attal : Lords of Doom
**
** sectionExperience.h
** section for setting levels of experience
**
** Version : $Id: sectionExperience.h,v 1.1 2004/05/08 13:59:10 audoux Exp $
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


#ifndef SECTIONEXPERIENCE_H
#define SECTIONEXPERIENCE_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "themeEditor/sectionWidget.h"


class AskIntList;

/*              ------------------------------
 *                     SectionExperience
 *              ------------------------------ */


class SectionExperience : public Section
{
 	Q_OBJECT
public:
	/** Constructor */
	SectionExperience( QWidget * parent = 0, const char * name = 0 );

	/** Clears the section */
	void clear();

	/** Initializes the section */
	void init();

	/** Saves the data */
	void save();

protected:
	AskIntList * _list;
};

#endif // SECTIONEXPERIENCE_H
