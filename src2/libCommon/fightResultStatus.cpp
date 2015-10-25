/****************************************************************
**
** Attal : Lords of Doom
**
** fightResultStatus.cpp
** small class for managing result of fight
**
** Version : $Id: fightResultStatus.cpp,v 1.1 2005/12/03 22:20:28 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/12/2005
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

#include "fightResultStatus.h"
 
// generic include files
// include files for QT
// application specific include files


FightResultStatus::FightResultStatus()
{
	clear();
}

void FightResultStatus::clear()
{
	_attackWin = false;
	_defenseWin = false;
	_fled = false;
	_surrendered = false;
}

void FightResultStatus::setAttackWin()
{
	_attackWin = true;
	_defenseWin = false;
}

void FightResultStatus::setDefenseWin()
{
	_attackWin = false;
	_defenseWin = true;
}
	
void FightResultStatus::setFled( bool state )
{
	_fled = state;
}

void FightResultStatus::setSurrendered( bool state )
{
	_surrendered = state;
}

bool FightResultStatus::isFightFinished()
{
	if( _attackWin || _defenseWin ) {
		return true;
	} else {
		return false;
	}
}

void FightResultStatus::fromUChar( unsigned char result )
{
	if( ( result &	0x01 ) == 0x01 ) {
		_attackWin = true;
	}
	if( ( result &	0x02 ) == 0x02 ) {
		_defenseWin = true;
	}
	if( ( result &	0x04 ) == 0x04 ) {
		_fled = true;
	}
	if( ( result &	0x08 ) == 0x08 ) {
		_surrendered = true;
	}
}

unsigned char FightResultStatus::toUChar()
{
	unsigned char ret = 0;

	if( _attackWin ) {
		ret |= 0x01;
	}
	if( _defenseWin ) {
		ret |= 0x02;
	}
	if( _fled ) {
		ret |= 0x04;
	}
	if( _surrendered ) {
		ret |= 0x08;
	}
	

	return ret;
}

