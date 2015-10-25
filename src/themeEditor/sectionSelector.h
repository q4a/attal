/****************************************************************
**
** Attal : Lords of Doom
**
** sectionSelector.h
** main widget, allows modif in diff theme sections
**
** Version : $Id: sectionSelector.h,v 1.3 2003/04/10 15:37:26 audoux Exp $
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

 
#ifndef SECTIONSELECTOR_H
#define SECTIONSELECTOR_H
 
 
// generic include files
// include files for QT
#include <qtabwidget.h>
// application specific include files
 
class Section;
class SectionBase;
class SectionLord;

/**              ------------------------------
 *                          SectionSelector
 **              ------------------------------ */



class SectionSelector : public QTabWidget
{
	Q_OBJECT
public:
	/** Constructor */
	SectionSelector( QWidget * parent = 0, const char * name = 0 );

	/** Saves the sections */
	void save();

public slots:
	/** Slot if something changed */
        void slot_changed( QWidget * widget );

	/** Slot if creatures changed */
	void slot_creaturesChanged();

	/** Slot if general attributes changed */
	void slot_generalChanged();

private:
	Section * _current;
	SectionLord * _lord;
	SectionBase * _base;
};

#endif // SECTIONSELECTOR_H
