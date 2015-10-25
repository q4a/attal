/****************************************************************
**
** Attal : Lords of Doom
**
** priceMarket.cpp
** compute prices on markets (for trading ressources)
**
** Version : $Id: priceMarket.cpp,v 1.1 2001/01/17 14:23:09 audoux Exp $
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
// include files for QT
// application specific include files

PriceMarket::PriceMarket()
{
	_ressources = new int[ MAX_RESS ];
	for( int i = 0; i < MAX_RESS; i++ ) {
		_ressources[i] = 1;
	}
}

void PriceMarket::reinit( GenericPlayer * player )
{
	
}

int PriceMarket::getRessourceInRessource( char dest, char prov )
{
	return( QMAX( (_ressources[dest] / _ressources[prov] ), 1 ) );
}
