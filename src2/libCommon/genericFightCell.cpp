/****************************************************************
**
** Attal : Lords of Doom
**
** genericFightCell.cpp
** gereric class for managing (not graphical) fightCells
**
** Version : $Id: genericFightCell.cpp,v 1.3 2007/09/21 20:46:42 lusum Exp $
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

#include "genericFightCell.h"

 
// generic include files
// include files for QT
// application specific includes

/** gereric class for managing (not graphical) fightCells */
GenericFightCell::GenericFightCell( int row, int col )
  :_coeff( -1 ),
   _row( row ),
   _col( col )
{
	_access = AttalCommon::UNKNOWN_ACCESS;
	_unit = 0;
	_headUnit = 0;
	_dist = 0;
}

GenericFightCell::~GenericFightCell()
{

}

