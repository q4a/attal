/****************************************************************
**
** Attal : Lords of Doom
**
** buildingAction.h
** Widget for editing actions of building
**
** Version : $Id: buildingAction.h,v 1.8 2007/12/24 14:05:09 lusum Exp $
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


#ifndef BUILDINGACTION_H
#define BUILDINGACTION_H


// generic include files
// include files for QT
#include <QDialog>
#include <QListWidget>
// application specific include files
#include "libCommon/action.h"
#include "libClient/askWidget.h"
#include "themeEditor/editWidget.h"

class QSpinBox;
class QComboBox;

/*              ------------------------------
 *                        buildingAction
 *              ------------------------------ */

class BuildingAction : public EditWidget
{
	Q_OBJECT
public:
	BuildingAction( QWidget * parent = 0, const char * name = 0 );

	void init( QList<Action *>  * list );

	QList<Action *> save();

public slots:
	void slot_add();

	void slot_edit();

	void slot_del();

};

/*              ------------------------------
 *                     buildingActionItem
 *              ------------------------------ */

class BuildingActionItem : public QListWidgetItem
{
public:
	/** Constructor */
	BuildingActionItem( Action::ActionType action, int actValue,
			ElementaryAction::ElementaryType modif, uint param, uint param1, int value );

	Action::ActionType getActionType() { return _action; }

	void setActionType( Action::ActionType action ) { _action = action; computeLabel(); }

	ElementaryAction::ElementaryType getModifType() { return _modif; }

	void setModifType( ElementaryAction::ElementaryType modif ) { _modif = modif; computeLabel(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; computeLabel(); }

	uint getParam1() { return _param1; }

	void setParam1( uint param ) { _param1 = param; computeLabel(); }

	int getValue() { return _value; }

	void setValue( int value ) { _value = value; computeLabel(); }

	int getActValue() { return _actValue; }

	void setActValue( int value ) { _actValue = value; computeLabel(); }

	void computeLabel();

protected:
	Action::ActionType _action;
	ElementaryAction::ElementaryType _modif;
	uint _param, _param1;
	int _value, _actValue;
};

/*              ------------------------------
 *                   DialogbuildingAction
 *              ------------------------------ */

class DialogBuildingAction : public QDialog
{
	Q_OBJECT
public:
	DialogBuildingAction( QWidget * parent = 0, const char * name = 0 );

	Action::ActionType getActionType() { return _action; }

	void setActionType( Action::ActionType action ) { _action = action; updateDialog(); }

	ElementaryAction::ElementaryType getModifType() { return _modif; }

	void setModifType( ElementaryAction::ElementaryType modif ) { _modif = modif; updateDialog(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; updateDialog(); }
	
	uint getParam1() { return _param1; }

	void setParam1( uint param ) { _param1 = param; updateDialog(); }

	int getValue() { return _value; }

	void setValue( int value ) { _value = value; updateDialog(); }
	
	int getActValue() { return _actValue; }

	void setActValue( int value ) { _actValue = value; updateDialog(); }

	void updateDialog();

public slots:
	void accept();

	void slot_modif( int index );
	
	void slot_action( int index );

protected:
	Action::ActionType _action;
	ElementaryAction::ElementaryType _modif;
	uint _param, _param1;
	int _value, _actValue;

	QComboBox * _comboAction, * _comboModif;
	QSpinBox * _spinParam, * _spinParam1,* _spinValue, * _spinActValue;
	QLabel * _labParam, *  _labParam1, * _labValue, * _labActValue;
};



#endif // BUILDINGACTION_H
