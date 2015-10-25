/****************************************************************
**
** Attal : Lords of Doom
**
** dataTheme.h
** class for gathering all informations about theme (data part)
**
** Version : $Id: dataTheme.h,v 1.17 2003/04/10 22:56:43 audoux Exp $
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
#include <qstring.h>
// application specific include files
#include "common/artefact.h"
#include "creature.h"
#include "common/categoryManager.h"
#include "common/genericCell.h"
#include "common/genericRessources.h"
#include "common/genericDecoration.h"
#include "common/genericLord.h"
#include "common/genericBase.h"
#include "common/genericBuilding.h"
#include "common/genericTeam.h"
#include "common/specifications.h"
#include "common/technic.h"
#include "common/unit.h"
#include "common/warMachine.h"

class QTextStream;


void indentation( QTextStream * ts, int num );

enum FOW_TYPE {
	FOW_ONCE,
	FOW_REAL
};

/**              ------------------------------
 *                          DataTheme
 **              ------------------------------ */

class DataTheme
{

public:
	/** Constructor */
	DataTheme();

	void init();

	void save();

	void clear();

	uint countRace() { return creatures.countRace(); }

	QString getCategoryName( uint categ );

	uint computeCategory( int nb );

	QString computeCategoryName( int nb );

	uint getRandomInCategory( uchar categ );

	FOW_TYPE getFogOfWar() { return _fow; }

	void managePopulation( bool state ) { _population = state; }

	bool isPopulation() { return _population; }

	ArtefactList artefacts;
	GenericRessources ressources;
	DecorationList decorations;
	TechnicList techList;
	CreatureList creatures;
	LordList lords;
	BaseList bases;
	BuildingList buildings;
	TeamList teams;
	CellModelList tiles;
	Specifications specifications;
	WarMachineList machines;

protected:
	CategoryManager _category;
	FOW_TYPE _fow;
	bool _population;
};

#endif // DATATHEME_H
