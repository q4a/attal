/****************************************************************
**
** Attal : Lords of Doom
**
** dataTheme.cpp
** class for gathering all informations about theme (data part)
**
** Version : $Id: dataTheme.cpp,v 1.15 2003/04/10 15:35:56 audoux Exp $
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

#include "dataTheme.h"

// generic include files
// include files for QT
// application specific include files

DataTheme DataTheme;


void indentation( QTextStream * ts, int num )
{
	for( int i = 0; i < num; i++ ) {
		(*ts) << "\t";
	}
}

//
// ----- DataTheme -----
//

DataTheme::DataTheme()
{
	_fow = FOW_ONCE;
	_population = false;
}

void DataTheme::init()
{
	artefacts.init();
	teams.init();
	ressources.init();
	decorations.init();
	creatures.init();
	techList.init();
	lords.init();
	bases.init();
	buildings.init();
	tiles.init();
	specifications.init();
	machines.init();
	_category.init();
}

void DataTheme::save()
{
	artefacts.save();
	teams.save();
	ressources.save();
	decorations.save();
	creatures.save();
	techList.save();
	lords.save();
	bases.save();
	buildings.save();
	tiles.save();
	specifications.save();
	machines.save();
	_category.save();
}

void DataTheme::clear()
{
	artefacts.clear();
	teams.clear();
	ressources.clear();
	decorations.clear();
	creatures.clear();
	techList.clearing();
	lords.clear();
	bases.clear();
	buildings.save();
	tiles.clear();
	specifications.clear();
	machines.clear();
	_category.clear();
}

QString DataTheme::getCategoryName( uint categ )
{
	QString ret = _category.getCategoryName( categ );

	return ret;
}

uint DataTheme::computeCategory( int nb )
{
	uint ret = _category.computeCategory( nb );

	return ret;
}

QString DataTheme::computeCategoryName( int nb )
{
	QString ret = _category.getCategoryName( nb );

	return ret;
}

uint DataTheme::getRandomInCategory( uchar categ )
{
	uint ret = _category.getRandomInCategory(categ);

	return ret;
}

