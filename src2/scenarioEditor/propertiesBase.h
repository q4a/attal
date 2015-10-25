/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesBase.h
** Defines properties box for bases on map
**
** Version : $Id: propertiesBase.h,v 1.12 2008/01/07 15:06:12 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 29/12/2001
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

 
#ifndef PROPERTIESBASE_H
#define PROPERTIESBASE_H
 
#include "propertiesLord.h"
 
// generic include files
// include files for QT
#include <QDialog>
#include <QList>
// application specific include files
#include "libCommon/genericPlayer.h"
#include "libClient/askCost.h"

class QComboBox;
class QTabWidget;

class QTreeWidget;

class AskString;
class GenericLord;
class GenericBase;
class GenericPlayer;
class GeneralPropertiesBase;
class PropertiesBaseUnits;

/*              ------------------------------
 *                         PropertiesBase
 *              ------------------------------ */

class PropertiesBase : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesBase( QList<GenericPlayer *> * players, QWidget * parent = 0, const char * name = 0 );

	void init( GenericBase * base );

public slots:
	virtual void accept();

private:
	QList<GenericPlayer *> * _players;
	GenericBase * _base;

	QTabWidget * _tabWidget;
	GeneralPropertiesBase * _general;
	PropertiesBaseUnits * _units;
};



/** Dialog for editing the properties of the base */
class GeneralPropertiesBase : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	GeneralPropertiesBase( QList<GenericPlayer *> * players, QWidget * parent = 0, const char * name = 0 );

	void init( GenericBase * base );

	void refresh( QList<GenericPlayer *> * players );
	
	void accept();

	void setOwner();

private:
	QList<GenericPlayer *> * _players;
	GenericBase * _base;
	QTreeWidget * _listBuilding;
	QComboBox * _ownerList;
	AskString * _name;
	AskCost * _resBase;
};

class PropertiesBaseUnits : public QWidget
{
public:
	PropertiesBaseUnits( QWidget * parent = 0, const char * name = 0 );

	void init( GenericBase * base );

	void accept();

	bool hasNoneUnits ();

protected:
	GenericBase * _base;
	EditTroop * _troop[MAX_UNIT];
};



#endif // PROPERTIESBASE_H
