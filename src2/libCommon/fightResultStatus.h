/****************************************************************
**
** Attal : Lords of Doom
**
** fightResultStatus.h
** small class for managing result of fight
**
** Version : $Id: fightResultStatus.h,v 1.1 2005/12/03 22:20:28 audoux Exp $
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

 
#ifndef FIGHTRESULTSTATUS_H
#define FIGHTRESULTSTATUS_H
 
 
// generic include files
// include files for QT
// application specific include files
 
 
/*              ------------------------------
 *                     FightResultStatus
 *              ------------------------------ */


class FightResultStatus
{
 
public:
	/** Constructor */
	FightResultStatus();

	void setAttackWin();

	void setDefenseWin();
	
	void setFled( bool state = true );

	void setSurrendered( bool state = true );

	bool isFightFinished();

	bool hasAttackWin() { return _attackWin; }

	bool hasDefenseWin() { return _defenseWin; }

	bool hasFled() { return _fled; }

	bool hasSurrendered() { return _surrendered; }

	void fromUChar( unsigned char result );

	unsigned char toUChar();	

	void clear();

protected:
	bool _attackWin, _defenseWin, _fled, _surrendered;
};
 
#endif // FIGHTRESULTSTATUS_H   
