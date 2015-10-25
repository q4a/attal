/****************************************************************
**
** Attal : Lords of Doom
**
** questionManager.h
** Manages questions for the engine
**
** Version : $Id: questionManager.h,v 1.1 2006/07/13 14:10:32 lusum Exp $
**
** Author(s) : Pascal Audoux
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


#ifndef QUESTIONMANAGER_H
#define QUESTIONMANAGER_H


// generic include files
// include files for QT
// application specific include files
#include "libCommon/attalSocket.h"

class GenericLord;
class GenericMapCreature;

/*              ------------------------------
 *                      QuestionManager
 *              ------------------------------ */


class QuestionManager
{

public:
	/** Constructor */
	QuestionManager();

	void setLord( GenericLord * lord ) { _lord = lord; }

	GenericLord * getLord() { return _lord; }
	
	void setCreature( GenericMapCreature * creature ) { _creature = creature; }
	
	GenericMapCreature * getCreature() { return _creature; }

	void setType( CLASS_QR type ) { _type = type; }

	CLASS_QR getType() { return _type; }

	CLASS_ANSWER getAnswerType();

protected:
	GenericLord * _lord;
	GenericMapCreature * _creature;
	CLASS_QR _type;
};

#endif // QUESTIONMANAGER_H
