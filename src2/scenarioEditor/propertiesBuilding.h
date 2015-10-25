/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesBuilding.h
** Defines properties box for buildings on map
**
** Version : $Id: propertiesBuilding.h,v 1.6 2008/01/07 17:06:19 lusum Exp $
**
** Author(s) : Sardi Carlo - Pascal Audoux
**
** Date : 22/04/2004
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

 
#ifndef PROPERTIESBUILDING_H
#define PROPERTIESBUILDING_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QLabel>
#include <QList>

// application specific include files
#include "libCommon/genericPlayer.h"
#include "libCommon/genericBuilding.h"
#include "libClient/askCost.h"

class QComboBox;
class QListWidget;

class GenericLord;
class GenericBuilding;

/*              ------------------------------
 *                         propertiesBuilding
 *              ------------------------------ */


/** Dialog for editing the properties of the building */
class PropertiesBuilding : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesBuilding( QList<GenericPlayer *> * players, QWidget * parent = 0, const char * name = 0 );

	void init( GenericBuilding * building );

	void refresh( QList<GenericPlayer *> * players );

public slots:
	void slot_accept();

	void slot_owner( int );

private:
	QList<GenericPlayer *> * _players;
	GenericBuilding * _building;
	QListWidget * _listBuilding;
	QLabel * _name;
	QComboBox * _ownerList;
	AskCost * _resBuil;
};

#endif // PROPERTIESBUILDING_H
