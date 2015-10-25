/****************************************************************
**
** Attal : Lords of Doom
**
** skill.cpp
** manages skills of the lords
**
** Version : $Id: skill.cpp,v 1.2 2002/12/15 11:59:36 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/08/2000
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

#include "skill.h"

 
// generic include files
// include files for QT
// application specific includes


//
// ----- Skill -----
//

Skill::Skill()
{
	_id = 0;
	_level = 0;
}

//
// ----- SkillLevel -----
//

SkillLevel::SkillLevel()
{
	_params.setAutoDelete( true );
}

void SkillLevel::addParam( int param )
{
	_params.append( new int( param ) );
}

uint SkillLevel::getParamNumber()
{
	return _params.count();
}

int SkillLevel::getParam( uint num )
{
	int ret = 0;

	if( num < _params.count() ) {
		ret = * ( _params.at( num ) );
	}

	return ret;
}

void SkillLevel::setParam( uint num, int val )
{
	if( num < _params.count() ) {
		*( _params.at( num ) ) = val;
	}
}

//
// ----- SkillModel -----
//

SkillModel::SkillModel()
{

}

