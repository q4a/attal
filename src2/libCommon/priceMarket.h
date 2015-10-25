/****************************************************************
**
** Attal : Lords of Doom
**
** priceMarket.h
** compute prices on markets (for trading resources)
**
** Version : $Id: priceMarket.h,v 1.6 2008/11/09 18:49:31 lusum Exp $
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

 
#ifndef PRICEMARKET_H
#define PRICEMARKET_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/genericPlayer.h"
 
/*              ------------------------------
 *                         PriceMarket
 *              ------------------------------ */



/** comment for the class */
class PriceMarket
{
 
public:
	/** Constructor */
	PriceMarket();
	
	~PriceMarket();
	
	/** Reinit market */
	void reinit( GenericPlayer * player );
		
	/** Set price of ressource ress */
	void setResourcePrice( int res, int num ) {
		_resources[res] = num;
	}
	
	/** Get price of ressource ress */
	int getResourcePrice( int res ) {
		return _resources[res];
	}

	float getResourceInResourceFloat( int dest, int prov );

	void printPrices();

private:
	int * _resources;
};
 
#endif // PRICEMARKET_H   
