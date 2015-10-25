/****************************************************************
**
** Attal : Lords of Doom
**
** genericInsideBuilding.cpp
** describes building inside bases
**
** Version : $Id: genericInsideBuilding.cpp,v 1.8 2003/01/08 17:55:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 04/01/2002
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

#include "genericInsideBuilding.h"
 
// generic include files
// include files for QT
// application specific include files
#include "common/action.h"
#include "common/dataTheme.h"


extern DataTheme DataTheme;
//
// ----- GenericInsideBuilding -----
//
GenericInsideBuilding::GenericInsideBuilding()
{
	_race = 0;
	_level = 0;
}

GenericInsideBuilding::~GenericInsideBuilding()
{

}

int GenericInsideBuilding::getX()
{
	return DataTheme.bases.at( _race )->getBuildingModel( _level )->getX();
}

int GenericInsideBuilding::getY()
{
	return DataTheme.bases.at( _race )->getBuildingModel( _level )->getY();
}

int GenericInsideBuilding::getMantCost(int ress)
{
	return DataTheme.bases.at( _race )->getBuildingModel( _level )->getMantCost(ress);
}

//
// ----- InsideBuildingModel -----
//

InsideBuildingModel::InsideBuildingModel()
{
	_race = 0;
	_level = 0;
	_name = "";
	_description = "";
	_x = 0;
	_y = 0;
	_replacement = -1;
	_require.setAutoDelete( true );
	_action = 0;
	for( int i = 0; i < MAX_RESS; i++ ) {
		_cost[i] = 0;
		_mcost[i] = 0;
	}
}

void InsideBuildingModel::addRequirement( int req )
{
	int * item = new int( req );
	_require.append( item );
}

int InsideBuildingModel::getRequirement( int num )
{
	int ret = 0;
	if( (uint)num < _require.count() ) {
		ret =  * (_require.at( num ));
	}
	return ret;
}

int InsideBuildingModel::getCost( int ress )
{
	int ret = 0;
	if( ress < MAX_RESS ) {
		ret = _cost[ress];
	}
	return ret;
}

void InsideBuildingModel::setCost( int ress, int cost )
{
	if( ress < MAX_RESS ) {
		_cost[ress] += cost;
	}
}

int InsideBuildingModel::getMantCost( int ress )
{
	int ret = 0;
	if( ress < MAX_RESS ) {
		ret = _mcost[ress];
	}
	return ret;
}

void InsideBuildingModel::setMantCost( int ress, int cost )
{
	if( ress < MAX_RESS ) {
		_mcost[ress] = cost;
	}
}
