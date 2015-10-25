/****************************************************************
**
** Attal : Lords of Doom
**
** priceMarket.cpp
** compute prices on markets (for trading resources)
**
** Version : $Id: priceMarket.cpp,v 1.10 2008/11/09 18:49:31 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 17/01/2001
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

#include "priceMarket.h"
 
// generic include files
#include <algorithm>

// include files for QT
// application specific include files
#include "libCommon/dataTheme.h"

PriceMarket::PriceMarket()
{
	uint nbRes = DataTheme.resources.count();
	_resources = new int[ nbRes ];
	for( uint i = 0; i < nbRes; i++ ) {
		_resources[i] = 100;
	}
}

PriceMarket::~PriceMarket()
{
	delete [] _resources;
}

void PriceMarket::reinit( GenericPlayer * )
{

}

float PriceMarket::getResourceInResourceFloat( int dest, int prov )
{
	return( (float)_resources[dest] / (float) _resources[prov] );
}

void PriceMarket::printPrices()
{
	uint nbRes = DataTheme.resources.count();
	for( uint i = 0; i < nbRes; i++ ) {
		logDD("price res %d is %d", i, _resources[i]);
	}
}
