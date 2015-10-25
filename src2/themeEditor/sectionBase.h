/****************************************************************
**
** Attal : Lords of Doom
**
** sectionBase.h
** section specifying bases
**
** Version : $Id: sectionBase.h,v 1.16 2009/11/19 22:14:10 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

 
#ifndef SECTIONBASE_H
#define SECTIONBASE_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QComboBox>
// application specific include files
#include "libClient/askCost.h"
#include "themeEditor/sectionWidget.h"
#include "themeEditor/buildingAction.h"

class QComboBox;
class MapDispositionEditor;
class SectionInsideBuilding;

/*              ------------------------------
 *                         SectionBase
 *              ------------------------------ */



/** comment for the class */
class SectionBase : public GenericSection
{
	Q_OBJECT
public:
	/** Constructor */
	SectionBase( QWidget * parent = 0, const char * name = 0 );

	void updateData();

	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates a new item */
	void selectNew();

	/** Deletes the current item */
	void selectDel();

	/** Saves the data */
	void save();

	/** Inits the section */
	void init();

	void clear();

public slots:
	void slot_preview();
	
private:
	int _idBase;
	AskString * _name;
	AskInt * _vision;
	AskCost * _resCost;
	AskCost * _resBase;
	BuildingAction * _buildingAction;
	MapDispositionEditor * _dispo;
	SectionInsideBuilding * _inside;
};

class SectionInsideBuilding : public QWidget
{
	Q_OBJECT
public:
	SectionInsideBuilding( QWidget * parent = 0, const char * name = 0 );

	void setBase( uint base );

	/** Saves the data */
	void save();

	/** Inits the section */
	void init();

	void clear();

public slots:
	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates a new item */
	void selectNew();

	/** Deletes the current item */
	void selectDel();

protected:
	int _idBase, _idBuild;
	SelectionWidget * _select;
	AskString * _name, * _description;
	AskInt * _x, * _y ;
	AskCost * _maintCost,* _cost;
	AskIntList * _params;
	AskIntList * _requirements;
	QComboBox * _comboInAct;
};

#endif // SECTIONBASE_H
