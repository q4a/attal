/****************************************************************
**
** Attal : Lords of Doom
**
** artefactAction.h
** Widget for editing actions of artefact
**
** Version : $Id: artefactAction.h,v 1.4 2006/10/03 20:40:44 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/03/2004
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


#ifndef ARTEFACTACTION_H
#define ARTEFACTACTION_H


// generic include files
// include files for QT
#include <QDialog>
#include <QListWidget>
// application specific include files
#include "libCommon/artefact.h"
#include "themeEditor/editWidget.h"

class QSpinBox;
class QComboBox;

/*              ------------------------------
 *                        ArtefactAction
 *              ------------------------------ */

class ArtefactAction : public EditWidget
{
	Q_OBJECT
public:
	ArtefactAction( QWidget * parent = 0, const char * name = 0 );

	void init( GenericArtefactModel * artefact );

	void save();

public slots:
	void slot_add();

	void slot_edit();

	void slot_del();

protected:
	GenericArtefactModel * _artefact;
};

/*              ------------------------------
 *                     ArtefactActionItem
 *              ------------------------------ */

class ArtefactActionItem : public QListWidgetItem
{
public:
	ArtefactActionItem( ArtefactElementaryAction::ActionType action,
		 ArtefactElementaryAction::ModifType modif, uint param, int value );

	ArtefactElementaryAction::ActionType getActionType() { return _action; }

	void setActionType( ArtefactElementaryAction::ActionType action ) { _action = action; computeLabel(); }

	ArtefactElementaryAction::ModifType getModifType() { return _modif; }

	void setModifType( ArtefactElementaryAction::ModifType modif ) { _modif = modif; computeLabel(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; computeLabel(); }

	int getValue() { return _value; }

	void setValue( int value ) { _value = value; computeLabel(); }

	void computeLabel();

protected:
	ArtefactElementaryAction::ActionType _action;
	ArtefactElementaryAction::ModifType _modif;
	uint _param;
	int _value;
};

/*              ------------------------------
 *                   DialogArtefactAction
 *              ------------------------------ */

class DialogArtefactAction : public QDialog
{
	Q_OBJECT
public:
	DialogArtefactAction( QWidget * parent = 0, const char * name = 0 );

	ArtefactElementaryAction::ActionType getActionType() { return _action; }

	void setActionType( ArtefactElementaryAction::ActionType action ) { _action = action; updateDialog(); }

	ArtefactElementaryAction::ModifType getModifType() { return _modif; }

	void setModifType( ArtefactElementaryAction::ModifType modif ) { _modif = modif; updateDialog(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; updateDialog(); }

	int getValue() { return _value; }

	void setValue( int value ) { _value = value; updateDialog(); }

	void updateDialog();

public slots:
	void accept();

protected:
	ArtefactElementaryAction::ActionType _action;
	ArtefactElementaryAction::ModifType _modif;
	uint _param;
	int _value;

	QComboBox * _comboAction, * _comboModif;
	QSpinBox * _spinParam, * _spinValue;
};



#endif // ARTEFACTACTION_H
