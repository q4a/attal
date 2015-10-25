/****************************************************************
**
** Attal : Lords of Doom
**
** sectionSelector.h
** main widget, allows modif in diff theme sections
**
** Version : $Id: sectionSelector.h,v 1.7 2007/09/09 16:33:41 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QTabWidget>

// application specific include files
 
class Section;
class SectionBase;
class SectionLord;
class SectionLordCategory;
class SectionGeneral;
class SectionArtefact;
class SectionTeam;
class SectionBuilding;
class SectionCreature;
class SectionDecoration;
class SectionGround;
class SectionExperience;
class SectionResource;
class SectionMachine;


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
	
	/** Saves all the sections */
	void saveAll();

public slots:
	/** Slot if something changed */
	void slot_changed( int index );

	/** Slot if creatures changed */
	void slot_creaturesChanged();

	/** Slot if general attributes changed */
	void slot_generalChanged();

	/** Slot if lord categories attributes changed */
	void slot_lordCategoryChanged();

private:
	Section * _current;
	SectionLordCategory * _lordCategories;
	SectionGeneral * _general;
	SectionArtefact * _artefact;
	SectionTeam * _team;
	SectionBuilding * _building;
	SectionCreature * _creature;
	SectionDecoration * _decoration;
	SectionGround * _ground;
	SectionExperience * _experience;
	SectionResource * _resource;
	SectionMachine * _machine;
	SectionBase * _base;
	SectionLord * _lord;
};

#endif // SECTIONSELECTOR_H
