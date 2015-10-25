/****************************************************************
**
** Attal : Lords of Doom
**
** priceMarket.h
** compute prices on markets (for trading ressources)
**
** Version : $Id: priceMarket.h,v 1.2 2001/03/05 16:17:03 audoux Exp $
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
#include "common/genericPlayer.h"
 
/*              ------------------------------
 *                         PriceMarket
 *              ------------------------------ */



/** comment for the class */
class PriceMarket
{
 
public:
	/** Constructor */
	PriceMarket();
	
	/** Reinit market */
	void reinit( GenericPlayer * player );
		
	/** Set price of ressource ress */
	void setRessourcePrice( char ress, uint num ) {
		_ressources[ress] = num;
	}
	
	/** Get 'price' of ressource dest in ressource prov */
	int getRessourceInRessource( char dest, char prov );
	
private:
	int * _ressources;
};
 
#endif // PRICEMARKET_H   
