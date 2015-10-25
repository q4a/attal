/****************************************************************
**
** Attal : Lords of Doom
**
** CreatureAnimation.h
** Widget for editing actions of building
**
** Version : $Id: creatureAnimation.h,v 1.4 2006/10/03 20:40:44 lusum Exp $
**
** Author(s) : Sardi Carlo - Pascal Audoux
**
** Date : 29/04/2004
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


#ifndef CREATUREANIMATION_H
#define CREATUREANIMATION_H


// generic include files
// include files for QT
#include <QDialog>
#include<QListWidget>
// application specific include files
#include "libCommon/creature.h"
#include "themeEditor/editWidget.h"

class QSpinBox;
class QComboBox;


/*              ------------------------------
 *                        CreatureAnimation
 *              ------------------------------ */

class CreatureAnimation : public EditWidget
{
	Q_OBJECT
public:
	CreatureAnimation( QWidget * parent = 0, const char * name = 0 );

	void init( Creature * creature );

	void save();

public slots:
	void slot_add();

	void slot_edit();

	void slot_del();

protected:
	Creature * _creature;
};

/*              ------------------------------
 *                     CreatureAnimationItem
 *              ------------------------------ */

class CreatureAnimationItem : public QListWidgetItem
{
public:
	/** Constructor */
	CreatureAnimationItem( CreatureAnimationType type, uint first, uint last );

	CreatureAnimationType getModifType() { return _type; }

	void setModifType( CreatureAnimationType type ) { _type = type; computeLabel(); }

	uint getFirst() { return _first; }

	void setFirst( uint first ) { _first = first; computeLabel(); }

	uint getLast() { return _last; }

	void setLast( int last ) { _last = last; computeLabel(); }

	void computeLabel();

protected:
	CreatureAnimationType _type;
	uint _first;
	uint _last;
};

/*              ------------------------------
 *                   DialogCreatureAnimation
 *              ------------------------------ */

class DialogCreatureAnimation : public QDialog
{
	Q_OBJECT
public:
	DialogCreatureAnimation( QWidget * parent = 0, const char * name = 0 );

	CreatureAnimationType getModifType() { return _modif; }

	void setModifType( CreatureAnimationType modif ) { _modif = modif; updateDialog(); }

	uint getFirst() { return _first; }

	void setFirst( uint first ) { _first = first; updateDialog(); }

	int getLast() { return _last; }

	void setLast( uint last ) { _last = last; updateDialog(); }

	void updateDialog();

public slots:
	void accept();

protected:
	CreatureAnimationType _modif;
	uint _first;
	uint _last;

	QComboBox  * _comboModif;
	QSpinBox * _spinFirst, * _spinLast;
};



#endif // CREATUREANIMATION_H
