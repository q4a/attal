/****************************************************************
**
** Attal : Lords of Doom
**
** genericInsideBuilding.cpp
** describes building inside bases
**
** Version : $Id: genericInsideBuilding.cpp,v 1.18 2009/09/11 18:01:57 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include "libCommon/action.h"
#include "libCommon/dataTheme.h"

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

uint GenericInsideBuilding::getMantCost( uint ress )
{
	return DataTheme.bases.at( _race )->getBuildingModel( _level )->getMantCost( ress );
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
	_action = NULL;
	for( int i = 0; i < MAX_RESS; i++ ) {
		_cost[i] = 0;
		_mcost[i] = 0;
	}
}

InsideBuildingModel::~InsideBuildingModel()
{
	if( _action ) {
		delete _action;
	}
}

void InsideBuildingModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<building>" << endl;

	indentation( ts, indent );
	*ts << "\t<name>" << getName() << "</name>" << endl;

	indentation( ts, indent );
	*ts << "\t<description>" << getDescription() << "</description>" << endl;

	indentation( ts, indent );
	*ts << "\t<x>" << getX() << "</x>" << endl;
	indentation( ts, indent );
	*ts << "\t<y>" << getY() << "</y>" << endl;

	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( getCost( i ) > 0 ) {
			indentation( ts, indent );
			*ts << "\t<cost ressource=\"" << i << "\">";
			*ts << getCost( i ) << "</cost>" << endl;
		}
	}
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		if( getMantCost( i ) > 0 ) {
			indentation( ts, indent );
			*ts << "\t<mcost ressource=\"" << i << "\">";
			*ts << getMantCost( i ) << "</mcost>" << endl;
		}
	}
	if( getAction() ) {
		indentation( ts, indent );
		*ts << "\t<action type=\"" << (uint) ( getAction()->getType() )  << "\">" << endl; 
		for( uint i = 0; i < getAction()->getParamNumber(); ++i ) {
			indentation( ts, indent+1 );
			*ts << "\t<param>" << getAction()->getParam( i )  << "</param>" << endl;
		}
		indentation( ts, indent+1 );
		*ts << "</action>" << endl;
	}
	int nbReq = getRequirementNumber();
	for( int i = 0; i < nbReq; ++i ) {
		indentation( ts, indent );
		*ts << "\t<require>" << getRequirement( i ) << "</require>" << endl;
	}

	indentation( ts, indent );
	*ts << "</building>" << endl;
}

void InsideBuildingModel::setAction( InsideAction * action ) 
{ 
	if( _action ) {
		delete _action;
	}
	_action = action; 
}


void InsideBuildingModel::addRequirement( int req )
{
	_require.append( req );
}

int InsideBuildingModel::getRequirement( int num )
{
	int ret = 0;
	if( num < _require.count() ) {
		ret =  _require.at( num );
	}
	return ret;
}

uint InsideBuildingModel::getCost( uint ress )
{
	uint ret = 0;
	if( (int)ress < DataTheme.resources.count() ) {
		ret = _cost[ress];
	}
	return ret;
}

void InsideBuildingModel::setCost( uint ress, uint cost )
{
	if( (int)ress < DataTheme.resources.count() ) {
		_cost[ress] = cost;
	}
}

uint InsideBuildingModel::getMantCost( uint ress )
{
	uint ret = 0;
	if( (int)ress < DataTheme.resources.count() ) {
		ret = _mcost[ress];
	}
	return ret;
}

void InsideBuildingModel::setMantCost( uint ress, uint cost )
{
	if( (int)ress < DataTheme.resources.count() ) {
		_mcost[ress] = cost;
	}
}

QString InsideBuildingModel::getBuildDescActions()
{
	ResourceList & themeResources = DataTheme.resources;
	uint nbRes = themeResources.count();
	uint i;
	QString text = "";
	QString cost = "";
	QString req = "";
	
	if( !_description.isEmpty() ) {
		text += tr("Description:") + "\n" + _description  +  "\n";
	}
	
	if( _action ) {
		switch( _action->getType() ) {
			case INSIDE_NONE: 
				text+=tr("Not implemented");
				break;
			case INSIDE_VILLAGE: 
				text+=tr("Main Building (not sellable)");
				break;
			case INSIDE_CASTLE: 
				text+=tr("Castle");
				break;
			case INSIDE_CREA:{ 
							 /* specify what kind and number of units produced */
							 text+=tr("Produce ");
							 if(_action->getParamNumber() == 3) {
								 text+=QString("%1 ").arg(_action->getParam(2));
								 text+= DataTheme.creatures.at(_action->getParam(0),_action->getParam(1))->getName();
							 }
							 break;
						 }
			case INSIDE_MARKET: 
						 text+=tr("Market");
						 break;
			case INSIDE_TAVERN: 
						 text+=tr("Lord's building");
						 break;
			default: 
						 break;
		}
	} else {
		text+=tr("Not implemented");
	}
	
	text +=  "\n" ;
	
	for( i = 0; i < nbRes; ++i ) {
		if( getCost( i ) != 0 ) {
			if( cost != "" ) {
				cost += " + ";
			}
			cost += QString::number( getCost( i ) )
				+ " " + themeResources.getRessource( i );
		}
	}
	if( cost == "" ) {
		cost = "0";
	}
	text +=  tr("Cost: ")  + cost;
	
	for( i = 0; (int)i < getRequirementNumber(); ++i ) { // Again, possibly  dangerous range limiting and a check with <limit.h> may be necessary
		if( i > 0 ) {
			req += ", ";
		}
		req += DataTheme.bases.at( _race )->getBuildingModel( getRequirement( i ) )->getName();
	}

	if( getRequirementNumber() > 0 ) {
		text +=  "\n" ;
		text +=  tr("Require: ")  + req;
	}
		
	return text;
}
