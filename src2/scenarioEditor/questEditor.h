/****************************************************************
**
** Attal : Lords of Doom
**
** questEditor.h
** Dialog for editing a quest
**
** Version : $Id: questEditor.h,v 1.6 2005/10/30 22:21:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/05/2004
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


#ifndef QUESTEDITOR_H
#define QUESTEDITOR_H


// generic include files
// include files for QT
#include <QDialog>
// application specific include files

class AskBool;
class AskCombo;
class AskString;
class Quest;
class QuestConditionEditor;

/*              ------------------------------
 *                       QuestEditor
 *              ------------------------------ */


class QuestEditor : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	QuestEditor( QWidget * parent = 0, const char * name = 0 );

	void init( Quest * quest );

	void save();

public slots:
	void slot_confirmation();

protected:
	Quest * _quest;
	AskString * _name, * _description;
	AskCombo * _type;
	AskBool * _needConfirmation;
	AskString * _confirmationQuestion;
	QuestConditionEditor * _editStartCondition;
	QuestConditionEditor * _editFailCondition;
	QuestConditionEditor * _editSuccessCondition;
};

#endif // QUESTEDITOR_H
