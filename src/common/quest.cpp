/****************************************************************
**
** Attal : Lords of Doom
**
** quest.cpp
** Manages quests in Attal
**
** Version : $Id: quest.cpp,v 1.1 2002/04/02 21:37:04 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/04/2002
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

#include "quest.h"

// generic include files
// include files for QT
// application specific include files


/** add comments here */
Quest::Quest()
{
	_state = QS_SLEEP;

}

