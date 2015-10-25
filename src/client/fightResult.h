/****************************************************************
**
** Attal : Lords of Doom
**
** fightResult.h
** Displaying the result of a fight
**
** Version : $Id: fightResult.h,v 1.5 2002/10/20 16:15:49 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/12/2000
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


#ifndef FIGHTRESULT_H
#define FIGHTRESULT_H


// generic include files
// include files for QT
#include <qdialog.h>
// application specific includes

class QLabel;
class QHBoxLayout;
class QFrame;
class Icon;
class Creature;
class MainResult;
class CasualtiesList;
class GenericLord;
class Fight;
class GenericFightUnit;

/*              ------------------------------
 *                         FightResult
 *              ------------------------------ */



/** Dialog displaying the result of the fight */
class FightResult : public QDialog
{

public:
	/** Constructor */
	FightResult( Fight * fight, char result, QWidget * parent = 0, const char * name = 0 );

private:
	MainResult * _mainResult;
	CasualtiesList * _attackList, * _defenseList;
};

/** Main widget of the result of the fight */
class MainResult : public QWidget
{
public:
	/** Constructor */
	MainResult( GenericLord * attack, GenericLord * defense, QWidget * parent = 0, const char * name = 0 );

private:
	Icon * _photo1, * _photo2;
};

/** Display list of casualties */
class CasualtiesList : public QWidget
{
public:
	/** Constructor */
	CasualtiesList( QWidget * parent = 0, const char * name = 0 );

	/** Add a new casualty to the list */
	void addCasualty( GenericFightUnit * unit );

	/** Clear the list */
	void clear();

private:
	QFrame * _present, * _numbers;
	QHBoxLayout * _layPresent, * _layNumbers;
	QLabel * _none;
};

#endif // FIGHTRESULT_H
