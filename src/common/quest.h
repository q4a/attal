/****************************************************************
**
** Attal : Lords of Doom
**
** quest.h
** Manages quests in Attal
**
** Version : $Id: quest.h,v 1.1 2002/04/02 21:37:04 audoux Exp $
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


#ifndef QUEST_H
#define QUEST_H


// generic include files
// include files for QT
// application specific include files


/*              ------------------------------
 *                         Quest
 *              ------------------------------ */



/** comment for the class */
class Quest
{

public:
	/** Constructor */
	Quest();

	enum QuestState {
		QS_SLEEP,
		QS_START,
		QS_SUCCESS,
		QS_FAILURE
	};

protected:
	QuestState _state;
};

#endif // QUEST_H
