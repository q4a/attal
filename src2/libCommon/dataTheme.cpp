/****************************************************************
**
** Attal : Lords of Doom
**
** dataTheme.cpp
** class for gathering all informations about theme (data part)
**
** Version : $Id: dataTheme.cpp,v 1.16 2007/05/06 09:42:41 fdarling Exp $
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

#define NO_EXTERN_DATATHEME
#include "dataTheme.h"

// generic include files
// include files for QT
#include <QTextStream>
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
	_isLoaded = false;
}

bool DataTheme::init()
{
	_isLoaded = general.init();
	_isLoaded = _isLoaded && artefacts.init();
	_isLoaded = _isLoaded && teams.init();
	_isLoaded = _isLoaded && resources.init();
	_isLoaded = _isLoaded && decorations.init();
	_isLoaded = _isLoaded && creatures.init();
	_isLoaded = _isLoaded && techList.init();
	_isLoaded = _isLoaded && lordCategories.init();
	_isLoaded = _isLoaded && lords.init();
	_isLoaded = _isLoaded && bases.init();
	_isLoaded = _isLoaded && buildings.init();
	_isLoaded = _isLoaded && tiles.init();
	_isLoaded = _isLoaded && machines.init();
	_isLoaded = _isLoaded && lordExperience.init();
	_isLoaded = _isLoaded && _category.init();

	return isLoaded();
}

void DataTheme::save()
{
	general.save();
	artefacts.save();
	teams.save();
	resources.save();
	decorations.save();
	creatures.save();
	techList.save();
	lordCategories.save();
	lords.save();
	bases.save();
	buildings.save();
	tiles.save();
	machines.save();
	lordExperience.save();
	_category.save();
}

void DataTheme::clear()
{
	general.clear();
	artefacts.clear();
	teams.clear();
	resources.clear();
	decorations.clear();
	creatures.clear();
	techList.clearing();
	lordCategories.clearList();
	lords.clear();
	bases.clear();
	buildings.clear();
	tiles.clear();
	machines.clear();
	lordExperience.clear();
	_category.clear();
}

QString DataTheme::getCategoryName( uint categ )
{
	return _category.getCategoryName( categ );
}

uint DataTheme::computeCategory( int nb )
{
	return (uint)_category.computeCategory( nb );
}

QString DataTheme::computeCategoryName( int nb )
{
	return _category.getCategoryName( nb );
}

uint DataTheme::getRandomInCategory( uchar categ )
{
	return (uint)_category.getRandomInCategory(categ);
}

uint DataTheme::getLordCategory( LordCategoryModel * category )
{
	uint nbCat = lordCategories.count();
	for( uint i = 0; i < nbCat; i++ ) {
		if( category == lordCategories.at( i ) ) {
			return i;
		}
	}

	return 0;
}
