/****************************************************************
**
** Attal : Lords of Doom
**
** tavernManager.cpp
** Manages tavern (available lords)
**
** Version : $Id: tavernManager.cpp,v 1.2 2006/11/28 21:27:30 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/02/2005
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

#include "tavernManager.h"
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/log.h"
#include "libCommon/gameData.h"


//
// ----- TavernManager -----

TavernManager::TavernManager( GameData * data )
{
	_data = data;
}

void TavernManager::reinit()
{

}
	
uint TavernManager::getTavernLordNumber( GenericBase * /* base */)
{
	return 1;
}
	
GenericLord * TavernManager::getTavernLord( GenericBase * base, uint /* numLord */ )
{
	GenericLord * ret = 0;
	GenericLord * tempLord;
	uint nbBase, nbLord, num, cpt;
	
	nbBase = _data->getBaseNumber();
	nbLord = _data->getLordNumber();
	
	num = nbBase;
	
	for( uint i = 0; i < nbBase; i++ ) {
		if( base == _data->getBase( i ) ) {
			num = i;
			break;
		}
	}
	
	if( num < nbBase ) {
		cpt = 0;
		for( uint i = 1; i < nbLord; i++ ) {
			tempLord = _data->getLord( i );
			if( cpt == num ) {
				if( tempLord->getOwner() == 0 ) {
					ret = tempLord;
					break;
				}
			} else {
				if( tempLord->getOwner() == 0 ) {
					cpt++;
				}
			}
		}	
	}
	
	return ret;
}
	
void TavernManager::newDay()
{
	/// XXX: will be use later, when more complex tavern management mechanism will be implemented
}
