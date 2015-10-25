/****************************************************************
**
** Attal : Lords of Doom
**
** action.cpp
** Manage actions of 'buildings'
**
** Version : $Id: action.cpp,v 1.3 2002/05/09 21:36:28 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/01/2002
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

#include "action.h"
 
// generic include files
// include files for QT
// application specific include files


// 
// ----- Action -----
// 

Action::Action()
{
	_list.setAutoDelete( true );	
	_type = Action::DATE;
}

//
// ----- Elementary Action -----
//

ElementaryAction::ElementaryAction()
{
	_type = ElementaryAction::ATTACK;
	_arg = 0;
	_coeff = 0;
}

//
// ----- InsideAction -----
//

InsideAction::InsideAction()
{
	_params.setAutoDelete( true );
	_type = 0;
}

void InsideAction::addParam( int param )
{
	int * p = new int( param );
	_params.append( p );
}

int InsideAction::getParam( uint num )
{
	int ret = 0;

	if( num < _params.count() ) {
		ret = * ( _params.at( num ) );
	}

	return ret;
}



