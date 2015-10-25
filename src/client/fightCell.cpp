/****************************************************************
**
** Attal : Lords of Doom
**
** fightCell.cpp
**
** Version : $Id: fightCell.cpp,v 1.7 2001/06/16 14:25:38 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

#include "fightCell.h"

 
// generic include files
#include <stdio.h>
// include files for QT
// application specific includes
#include "common/log.h"


/** Print FightCell info */
void FightCell::displayCell()
{
	printf( "FightCell [%d][%d]\n", getRow(), getCol() );	   
}
