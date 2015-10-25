/****************************************************************
**
** Attal : Lords of Doom
**
** questConditionEditor.h
** Editor of conditions used in quests
**
** Version : $Id: questConditionEditor.h,v 1.8 2006/10/05 20:35:45 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 14/06/2004
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


#ifndef QUESTCONDITIONEDITOR_H
#define QUESTCONDITIONEDITOR_H


// generic include files
// include files for QT
#include <QDialog>
#include <QLabel>

#include <QTreeWidget>
#include <QMenu>
// application specific include files


class QLabel;
class QListWidget;
class QPushButton;

class QuestCondition;
class QuestConditionItem;

/*              ------------------------------
 *                    QuestConditionEditor
 *              ------------------------------ */


class QuestConditionEditor : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	QuestConditionEditor( QWidget * parent = 0, const char * name = 0 );

	void setTitle( const QString & title );

	void setCondition( QuestCondition * condition ) { _condition = condition; }

	QuestCondition * getCondition() { return _condition; }

public slots:
	void slot_edit();

protected:
	QLabel * _title;
	QuestCondition * _condition;
};


/** */
class QuestConditionDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	QuestConditionDialog( QuestCondition * condition, QWidget * parent = 0, const char * name = 0 );

	void buildTree();

	void buildSubTree( QuestConditionItem * root, QuestCondition * condition );
public slots:

	void contextMenuEvent(QContextMenuEvent *e);

	void slot_actionPropertiesCondition();

	void slot_actionAddCondition();

	void slot_actionDelCondition();

protected:
	QuestCondition * _condition;
	QTreeWidget * _list;
	QMenu * _menu;
	QuestConditionItem * _contextItem;
};

/** */
class QuestConditionItem : public QTreeWidgetItem
{
public:
	/** Constructor */
	QuestConditionItem( QTreeWidget * parent );

	QuestConditionItem(QuestConditionItem*parent);

	void setCondition( QuestCondition * condition );

	QuestCondition * getCondition() { return _condition; }

protected:
	QuestCondition * _condition;
};

/** */
class NewQuestConditionDialog : public QDialog
{
	Q_OBJECT
public:
	NewQuestConditionDialog( QWidget * parent = 0, const char * name = 0 );

	QuestCondition * getNewCondition();

public slots:
	void slot_select();

protected:
	enum TypeCondition {
		TC_AND = 0,
		TC_OR,
		TC_DATE,
		TC_LORD,
		TC_PLAYER,
		TC_NONE
	};

	QListWidget * _list;
	TypeCondition _type;
};


#endif // QUESTCONDITIONEDITOR_H
