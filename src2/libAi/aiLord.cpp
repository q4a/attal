/****************************************************************
**
** Attal : Lords of Doom
**
** aiLord.cpp
** manages an ai lord
**
** Version : $Id: aiLord.cpp,v 1.9 2008/07/10 15:52:37 lusum Exp $
**
** Author(s) : Sardi Carlo
**
** Date : 27/07/2005
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

#include "aiLord.h"

// generic include files
// include files for QT
// application specific include files

#include "libCommon/genericMapCreature.h"
#include "libCommon/unit.h"

extern QString DATA_PATH;

//
// ----- GenericLord -----
//

AiLord::AiLord( int id ):GenericLord( id )
{
	for(int i = PR_ENEMY;i<=PR_LAST;i++){ 
		_priorities[i]=(i*10);
	}	
	_shouldStop = false;

}

AiLord::~AiLord()
{
}

void AiLord::reinit()
{
}

void AiLord::initParams()
{
	_shouldStop = false;
	_curPrio = PR_LAST * 10;
	_destinationCell = getCell();
	_mincost = getCharac( MOVE ) + 10;

}
