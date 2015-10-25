/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesEvent.h
** Defines properties box for events on map
**
** Version : $Id: propertiesEvent.h,v 1.7 2007/05/03 13:46:54 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 24/04/2004
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


#ifndef PROPERTIESEVENT_H
#define PROPERTIESEVENT_H


// generic include files
// include files for QT
#include <QDialog>
#include <QWidget>
// application specific include files
#include "libClient/askWidget.h"

class GenericArtefact;
class GenericBonus;
class GenericChest;
class GenericEvent;

class PropertiesEventContent;

/*              ------------------------------
 *                       PropertiesEvent
 *              ------------------------------ */

class PropertiesEvent : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesEvent( GenericEvent * event, QWidget * parent = 0, const char * name = 0 );

public slots:
	void slot_accept();

protected:
	void createContentView();

	GenericEvent * _event;
	PropertiesEventContent * _content;
};

/*              ------------------------------
 *                  PropertiesEventContent
 *              ------------------------------ */

class PropertiesEventContent : public QWidget
{
public:
	/** Constructor */
	PropertiesEventContent( QWidget * parent = 0, const char * /*name*/ = 0 )
	: QWidget( parent ) {}

	virtual void save() = 0;
};

/*              ------------------------------
 *                    PropertiesArtefact
 *              ------------------------------ */

class PropertiesArtefact : public PropertiesEventContent
{
public:
	PropertiesArtefact( GenericArtefact * artefact, QWidget * parent = 0, const char * name = 0 );

	void save() {}

protected:
	GenericArtefact * _artefact;
};

/*              ------------------------------
 *                       PropertiesBonus
 *              ------------------------------ */

class PropertiesBonus : public PropertiesEventContent
{
	Q_OBJECT
public:
	PropertiesBonus( GenericBonus * bonus, QWidget * parent = 0, const char * name = 0 );

	void save();

protected:
	void displayResource();
	void displayPrimSkill();
	void displaySkill();
	void displaySpell();

	GenericBonus * _bonus;
	AskInt * _fixed;
	AskCombo * _primSkill;
	AskInt * _value;
};

/*              ------------------------------
 *                       PropertiesChest
 *              ------------------------------ */

class PropertiesChest : public PropertiesEventContent
{
public:
	PropertiesChest( GenericChest * chest, QWidget * parent = 0, const char * name = 0 );

	void save() {}

protected:
	GenericChest * _chest;
};


#endif // PROPERTIESEVENT_H
