/****************************************************************
**
** Attal : Lords of Doom
**
** displayCreature.h
** widget for displaying, buying cretaures
**
** Version : $Id: displayCreature.h,v 1.10 2002/10/20 16:15:49 audoux Exp $
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
#include <qwidget.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
// application specific include files
#include "common/creature.h"

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
	void setCreature( int race, int level );

signals:
	/** Signal if clicked */
	void sig_clicked();

protected:
	void mousePressEvent( QMouseEvent * event );

private:
	CreaturePresentation * _creaturePres;
	CreatureInfo * _creatureInfo;
	Creature * _creature;
};

/** Presentation of a creature */
class CreaturePresentation : public QWidget
{
public:
	/** Constructor */
	CreaturePresentation( QWidget * parent = 0, const char * name = 0 );

	/** Initialize creature to display */
	void setCreature( int race, int level );

private:
	QLabel * _name, * _picture,  * _building, * _available;
};

/** Information about a creature */
class CreatureInfo : public QWidget
{
public:
	/** Constructor */
	CreatureInfo( QWidget * parent = 0, const char * name = 0 );

	/** Initialize creature to display */
	void setCreature( int race, int level );

private:
	InfoFrame * _attack, * _defense, * _damage;
	InfoFrame * _health, * _speed, * _growth;
};

enum TypeInfo {
	INF_ATTACK,
	INF_DEFENSE,
	INF_ARROW,
	INF_DAMAGE,
	INF_HEALTH,
	INF_SPEED,
	INF_GROWTH
};

/** Frame for displaying one information */
class InfoFrame : public QWidget
{
public:
	/** Constructor */
	InfoFrame( TypeInfo info, QWidget * arent = 0, const char * name = 0 );

	/** Set value */
	void setValue( int val );

	/** Set range of values */
	void setValue( int val1, int val2 );

private:
	QLabel * _value;
};

/** Dialog for buying a creature */
class BuyCreature : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	BuyCreature( QWidget * parent = 0, const char * name = 0 );

	/** Initialize creature to display */
	void init( GenericPlayer * player, GenericBase * base, Creature * creature );

	/** Initialize the socket of the dialog for requesting the server */
	void initSocket( AttalSocket * socket ) {
		_socket = socket;
	}

	/** Sets max number of creatures allowed to buy */
	void setMaxNumber( int max ) { _max = max; }

public slots:
	/** Slot for 'All' button clicked */
	void slot_all();

	void slot_buy();

private:
	QLabel * _labPres, * _labDescr;
	Icon * _ico;
	ComputeCost * _cost;
	Creature * _creature;
	AttalSocket * _socket;
	GenericBase * _base;
	GenericPlayer * _player;
	int _max;
};

/** Display cost of creatures to buy */
class ComputeCost : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ComputeCost( QWidget * parent = 0, const char * name = 0 );

	/** Init widget */
	void init( Creature * creature, int max );

	/** \return Returns nb creatures to buy */
	int getNumber();

private slots:
	void slot_newValue( int );

private:
	QScrollBar * _bar;
	CreatureCost * _unitaryCost, * _totalCost;
	int _max;

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
