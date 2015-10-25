/****************************************************************
**
** Attal : Lords of Doom
**
** sectionCreature.h
** section for specifying creatures
**
** Version : $Id: sectionCreature.h,v 1.10 2007/05/09 13:21:23 lusum Exp $
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

 
#ifndef SECTIONCREATURE_H
#define SECTIONCREATURE_H
 

// generic include files
// include files for QT
#include <QWidget>
// application specific include files
#include "themeEditor/sectionWidget.h"
#include "themeEditor/creatureAnimation.h"

class AskCost;
class EditCreature;

/*              ------------------------------
 *                         SectionCreature
 *              ------------------------------ */

/** comment for the class */
class SectionCreature : public Section
{
	Q_OBJECT
public:
	/** Constructor */
	SectionCreature( QWidget * parent = 0, const char * name = 0 );

	/** Saves the data */
	void save();

	/** Inits the section */
	void init();

public slots:
	/** Slot for selecting first race */
        void slot_firstRace();

	/** Slot for selecting previous race */
	void slot_previousRace();

	/** Slot for selecting next race */
	void slot_nextRace();

	/** Slot for selecting last race */
	void slot_lastRace();

	/** Slot for creating a new race */
	void slot_newRace();

	/** Slot for deleting the current race */
	void slot_delRace();

	/** slot for selecting first creature of the current race */
        void slot_firstCreature();

	/** slot for selecting previous creature of the current race */
	void slot_previousCreature();

	/** slot for selecting next creature of the current race */
	void slot_nextCreature();

	/** slot for selecting last creature of the current race */
	void slot_lastCreature();

	/** slot for creating a new creature in the current race */
	void slot_newCreature();

	/** slot for deleting the current creature of the current race */
	void slot_delCreature();

signals:
	/** Signal when stg change */
	void sig_changed();

private:
	void removeRace();
	void removeCreature();

	AskString * _nameRace;
	uint _race, _creature;
	SelectionWidget * _selectRace, * _selectCreature;
	EditCreature * _editCreature;
};

/** Edition of a creature */
class EditCreature : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	EditCreature( QWidget * parent = 0, const char * name = 0 );

	/** Saves the data */
	void save( int race, int creature );

	/** Init the widget */
	void init( int race, int creature );

private:
	CreatureAnimation * _creatureAnimation;
	AskString * _name;
	AskInt * _ptAttack, * _ptDefense;
	AskInt * _maxHealth, * _maxMove;
	AskInt * _distAttack;
	AskInt * _XOffset, * _XOffsetMirr, * _YOffset;
	AskInt * _minDamages, * _maxDamages;
	AskInt * _morale, * _luck;
	AskInt * _size;
	AskInt * _numFrames;
	AskCost * _cost, * _maintCost;
	AskPixmap * _pix;
};

#endif // SECTIONCREATURE_H
