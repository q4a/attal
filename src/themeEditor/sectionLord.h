/****************************************************************
**
** Attal : Lords of Doom
**
** sectionLord.h
** section for specifying lords
**
** Version : $Id: sectionLord.h,v 1.8 2003/04/25 19:04:30 audoux Exp $
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

 
#ifndef SECTIONLORD_H
#define SECTIONLORD_H
 
 
// generic include files
// include files for QT
#include <qframe.h>
// application specific include files
#include "themeEditor/sectionWidget.h"
#include "common/genericLord.h"

class QComboBox;
class DisplayTroops;
class EditTroop;
class GenericFightUnit;
class GenericLord;

/**              ------------------------------
 *                          SectionLord
 **              ------------------------------ */


class SectionLord : public GenericSection
{
	Q_OBJECT
public:
	/** Constructor */
	SectionLord( QWidget * parent = 0, const char * name = 0 );

	/** Clears the section */
	void clear();

	/** Initialize the section */
	void init();

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

	/** Deletes the current item */
	void selectDel();

	/** Saves the data */
	void save();

private:
	AskString * _name, * _category;
	AskCombo * _race;
	AskInt * _attack, * _defense;
	AskInt * _power, * _knowledge, * _movemax;
	AskInt * _vision;
	AskPixmap * _bigPix, * _smallPix;
	DisplayTroops * _troops;

	uint _idLord;
};

/** Widget displaying all troops of a lord */
class DisplayTroops : public QWidget
{
public:
	/** Constrcutor */
	DisplayTroops( QWidget * parent = 0, const char * name = 0 );

	/** Clears all the troops */
	void clear();

	/** Updates data */
	void updateData();

	/** Inits display */
	void init( GenericLordModel * lord );

	/** Saves the data */
	void save( GenericLordModel * lord );
	
private:
	EditTroop * _troop[MAX_UNIT];
};

/** Widget for editing a troop of a lord */
class EditTroop : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	EditTroop( int num, QWidget * parent = 0, const char * name = 0 );

	/** Clears the widget */
	void clear();

	/** Updates data */
	void updateData();

	/** Inits the widget with 'unit' */
	void init( GenericFightUnit * unit );

	/** Saves the data */
	void save( int i, GenericLordModel * lord );

public slots:
	/** Slot if races have changed */
        void slot_raceChanged( int race );

private:
	GenericFightUnit * _unit;
	AskCombo * _race, * _name;
	AskInt * _number;
};


#endif // SECTIONLORD_H
