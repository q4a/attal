/****************************************************************
**
** Attal : Lords of Doom
**
** condition.h
** Manage condition for events/enter building
**
** Version : $Id: condition.h,v 1.1.1.1 2003/06/15 19:02:07 audoux Exp $
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

 
#ifndef CONDITION_H
#define CONDITION_H
 
 
// generic include files
// include files for QT
// application specific include files
 
 
/*              ------------------------------
 *                         Condition
 *              ------------------------------ */



/** comment for the class */
class Condition
{
 
public:
	/** Constructor */
	Condition();

	bool isValidated() { return _state; }

	void setValidated( bool state ) { _state = state; }

private:
	bool _state;
};
 
#endif // CONDITION_H   
