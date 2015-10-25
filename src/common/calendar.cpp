/****************************************************************
**
** Attal : Lords of Doom
**
** calendar.cpp
** class for managing days, weeks, months
**
** Version : $Id: calendar.cpp,v 1.4 2001/03/05 16:17:03 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/01/2001
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

#include "calendar.h"
 
// generic include files
// include files for QT
// application specific include files


/** add comments here */
Calendar::Calendar()
	:_day( 0 ),
	 _week( 1 ),
	 _month( 1 )
{

}

void Calendar::reinit()
{
	_day = 0;
	_month = 1;
	_week = 1;
}

void Calendar::newDay()
{
	_day++;
	if( _day > 7 ) {
		_day = 1;
		_week++;
	}
	if( _week > 4 ) {
		_week = 1;
		_month++;
	}
	
	emit sig_changed();
}
