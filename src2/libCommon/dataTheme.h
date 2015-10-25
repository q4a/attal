/****************************************************************
**
** Attal : Lords of Doom
**
** dataTheme.h
** class for gathering all informations about theme (data part)
**
** Version : $Id: dataTheme.h,v 1.12 2007/11/05 21:54:07 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/06/2001
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

 
#ifndef DATATHEME_H
#define DATATHEME_H
 
 
// generic include files
// include files for QT
#include <QString>
#include <QTextStream>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/creature.h"
#include "libCommon/categoryManager.h"
#include "libCommon/generalOptions.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericRessources.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericBase.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericTeam.h"
#include "libCommon/lordCategoryModel.h"
#include "libCommon/lordExperience.h"
#include "libCommon/technic.h"
#include "libCommon/unit.h"
#include "libCommon/warMachine.h"

class QTextStream;

void indentation( QTextStream * ts, int num );


/**              ------------------------------
 *                          DataTheme
 **              ------------------------------ */

class DataTheme
{

public:
	/** Constructor */
	DataTheme();

	bool init();

	void save();

	void clear();

	bool isLoaded() {
		return _isLoaded;
	}

	uint countRace() { return creatures.countRace(); }

	QString getCategoryName( uint categ );

	uint computeCategory( int nb );

	QString computeCategoryName( int nb );

	uint getRandomInCategory( uchar categ );

	uint getLordCategory( LordCategoryModel * category );

	ArtefactList artefacts;
	ResourceList resources;
	DecorationList decorations;
	TechnicList techList;
	CreatureList creatures;
	LordCategoryList lordCategories;
	LordList lords;
	BaseList bases;
	BuildingList buildings;
	TeamList teams;
	CellModelList tiles;
	WarMachineList machines;
	LordExperience lordExperience;
	GeneralOptions general;

protected:
	CategoryManager _category;
	
	bool _isLoaded;
};

#ifndef NO_EXTERN_DATATHEME
extern DataTheme DataTheme;
#else
#undef NO_EXTERN_DATATHEME
#endif

#endif // DATATHEME_H
