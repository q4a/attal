/****************************************************************
**
** Attal : Lords of Doom
**
** questionManager.cpp
** Manages questions for the engine
**
** Version : $Id: questionManager.cpp,v 1.1 2006/07/13 14:10:32 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 05/05/2004
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

#include "questionManager.h"

// generic include files
// include files for QT
// application specific include files


QuestionManager::QuestionManager()
{
	_lord = 0;
	_creature = 0;
	_type = C_QR_MSG_END;
}

CLASS_ANSWER QuestionManager::getAnswerType()
{
	CLASS_ANSWER ret = ANSWER_YESNO;
	switch( _type ) {
	case C_QR_CHEST:
		ret = ANSWER_ENUM;
		break;
	case C_QR_QUEST:
		ret = ANSWER_YESNO;
		break;
	case C_QR_CREATURE_FLEE:
		ret = ANSWER_YESNO;
		break;
	case C_QR_CREATURE_MERCENARY:
		ret = ANSWER_YESNO;
		break;
	case C_QR_CREATURE_JOIN:
		ret = ANSWER_YESNO;
		break;
	default:
		//ret = ANSWER_YESNO;
		break;
	}

	return ret;
}

