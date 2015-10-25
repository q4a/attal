/****************************************************************
**
** Attal : Lords of Doom
**
** displayCreature.h
** widget for displaying, buying cretaures
**
** Version : $Id: displayCreature.h,v 1.11 2008/06/30 23:15:58 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 01/01/2001
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


#ifndef DISPLAYCREATURE_H
#define DISPLAYCREATURE_H


// generic include files
// include files for QT
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>

// application specific include files
#include "libCommon/creature.h"

class QScrollBar;

class AttalSocket;
class ComputeCost;
class CreatureCost;
class CreatureInfo;
class CreaturePresentation;
class GenericPlayer;
class GenericBase;
class Icon;
class InfoFrame;
class ResourceLabel;

/*              ------------------------------
 *                         DisplayCreature
 *              ------------------------------ */



/** comment for the class */
class DisplayCreature : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayCreature( QWidget * parent = 0, const char * name = 0 );

	/** Initialize creature to display */
	void setCreature( int race, int level , GenericBase * base);

	Creature * getCreature() { return _creature; }

signals:
	/** Signal if clicked */
	void sig_clicked();

protected:
	void mousePressEvent( QMouseEvent * event );

private:
	CreaturePresentation * _creaturePres;
	CreatureInfo * _creatureInfo;
	Creature * _creature;
	QLabel * _name, * _picture, * _available;
	QLabel * _attack, * _defense, * _damage;
	QLabel * _health, * _speed, * _growth;
};

/** Dialog for buying a creature */
class BuyCreature : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	BuyCreature( QWidget * parent = 0, GenericPlayer * player = 0, GenericBase * base = 0, Creature * creature = 0 );

	/** Sets max number of creatures allowed to buy */
	void setMaxNumber( int max ) { _max = max; }

signals:
	void sig_baseUnitBuy( Creature * , int );

private slots:
	void reinit();
	void slot_all();
	void slot_buy();
	void slot_newValue( int );

private:
	/** Initialize creature to display */
	void init( GenericPlayer * player, GenericBase * base, Creature * creature );

	QLabel * _labPres, * _labDescr;
	Icon * _ico;
	ComputeCost * _cost;
	Creature * _creature;
	GenericBase * _base;
	GenericPlayer * _player;
	int _max;
	
	QLabel * _labAvail, * _labBought, * _labTitle;
	CreatureCost * _individualCost, * _boughtCost;
	QScrollBar * _bar;
	QPushButton * _butBuy, * _butAll;
};

/** Display cost of a creature */
class CreatureCost : public QFrame
{
public:
	/** Constrcutor */
	CreatureCost( QWidget * parent = 0, const char * name = 0 );

	/** Init widget */
	void init( Creature * creature );

	/** Sets number of creature */
	void setNumber( unsigned int nb );

protected:
	void reinit();

	uint _nb;
	Creature * _creature;
	ResourceLabel * _res[MAX_RESS];
};

#endif // DISPLAYCREATURE_H
