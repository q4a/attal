/****************************************************************
**
** Attal : Lords of Doom
**
** decorationEffect.h
** Widget for editing effects of decoration
**
** Version : $Id: decorationEffect.h,v 1.5 2006/10/03 20:40:44 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 15/08/2003
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


#ifndef DECORATIONEFFECT_H
#define DECORATIONEFFECT_H


// generic include files
// include files for QT
#include <QDialog>
#include <QListWidget>
// application specific include files
#include "libCommon/genericDecoration.h"
#include "themeEditor/editWidget.h"

class QSpinBox;
class QComboBox;

/*              ------------------------------
 *                         DecorationEffect
 *              ------------------------------ */

class DecorationEffect : public EditWidget
{
	Q_OBJECT
public:
	DecorationEffect( QWidget * parent = 0, const char * name = 0 );

	void init( DecorationGroup * group );

	void save();

public slots:
	void slot_add();

	void slot_edit();

	void slot_del();

protected:
	DecorationGroup * _group;
};

/*              ------------------------------
 *                         EffectItem
 *              ------------------------------ */

class EffectItem : public QListWidgetItem
{
public:
	EffectItem( DecorationGroup::EffectType type, uint param );

	DecorationGroup::EffectType getType() { return _type; }

	void setType( DecorationGroup::EffectType type ) { _type = type; computeLabel(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; computeLabel(); }

	void computeLabel();

protected:
	uint _param;
	DecorationGroup::EffectType _type;
};

/*              ------------------------------
 *                         DialogEffect
 *              ------------------------------ */

class DialogEffect : public QDialog
{
	Q_OBJECT
public:
	DialogEffect( QWidget * parent = 0, const char * name = 0 );

	DecorationGroup::EffectType getType() { return _type; }

	void setType( DecorationGroup::EffectType type ) { _type = type; updateDialog(); }

	uint getParam() { return _param; }

	void setParam( uint param ) { _param = param; updateDialog(); }

	void updateDialog();

public slots:
	void accept();

protected:
	uint _param;
	DecorationGroup::EffectType _type;
	QComboBox * _combo;
	QSpinBox * _spin;

};



#endif // DECORATIONEFFECT_H
