/****************************************************************
**
** Attal : Lords of Doom
**
** gainLevel.h
** Dialog when lord gain a new level
**
** Version : $Id: gainLevel.h,v 1.2 2001/03/05 16:43:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/12/2000
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

 
#ifndef GAINLEVEL_H
#define GAINLEVEL_H
 
 
// generic include files
// include files for QT
#include <qdialog.h>
#include <qpushbutton.h>
// application specific includes
#include "common/genericLord.h"
#include "common/skill.h"
 
class Icon;
class QLabel;
class Sentence;


/*              ------------------------------
 *                         GainLevel
 *              ------------------------------ */


/** comment for the class */
class GainLevel : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	GainLevel( QWidget * parent = 0, const char * name = 0 );
	
	/** Initialize lord */
	void setLord( GenericLord * lord ) { _lord = lord; reinit(); }
	
	/** Set the primary skill gained */
	void setImprovement( PrimarySkill skill ) { _skill = skill; }
	
	/** Return choice */
	int getChoice() { return _choice; }
	
	/** Reinit info displayed */
	void reinit();
	
private slots:
	/** Slot if choice_1 clicked */
	void slot_choice1() { _choice = 1; _butOk->setEnabled( true ); }
	
	/** Slot if choice_2 clicked */
	void slot_choice2() { _choice = 2; _butOk->setEnabled( true ); }
	
private:
	GenericLord * _lord;
	PrimarySkill _skill;
	Icon * _photo;
	Sentence * _title, * _level, * _spec;
	QPushButton * _butOk, * _choice1, * _choice2;
	int _choice;
};
 
#endif // GAINLEVEL_H   
