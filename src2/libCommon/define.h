/****************************************************************
**
** Attal : Lords of Doom
**
** define.h
** file used for global '#define'
**
** Version : $Id: define.h,v 1.6 2008/05/07 00:02:55 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 30/12/2001
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

 
#ifndef DEFINE_H
#define DEFINE_H

// generic include files
// include files for QT
// application specific include files

#define MAX_UNIT 7
#define MAX_SKILL 8
#define MAX_RESS 12
#define COST_ENTER 4

enum LordCharac {
	ATTACK = 1,
	DEFENSE,
	POWER,
	KNOWLEDGE,
	MOVE,
	MAXMOVE,
	TECHNICPOINT,
	MAXTECHNICPOINT,
	MORALE,
	LUCK,
	VISION,
	EXPERIENCE,
	CHARISMA,
	LEVEL,
	LAST
};

#endif // DEFINE_H
