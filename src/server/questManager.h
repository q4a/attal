/****************************************************************
**
** Attal : Lords of Doom
**
** questManager.h
** Manages the quests of a scenario
**
** Version : $Id: questManager.h,v 1.1 2002/04/06 15:19:43 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/04/2002
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


#ifndef QUESTMANAGER_H
#define QUESTMANAGER_H


// generic include files
// include files for QT
#include <qlist.h>
// application specific include files
#include "common/quest.h"


/*              ------------------------------
 *                         QuestManager
 *              ------------------------------ */



/** comment for the class */
class QuestManager : public QList<Quest>
{

public:
	/** Constructor */
	QuestManager();
};

#endif // QUESTMANAGER_H
