/****************************************************************
**
** Attal : Lords of Doom
**
** askChest.h
** Dialog for asking the player about the chest content
**
** Version : $Id: askChest.h,v 1.2 2005/10/30 22:20:56 audoux Exp $
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


#ifndef ASKCHEST_H
#define ASKCHEST_H


// generic include files
// include files for QT
#include <QDialog>
// application specific include files


/*              ------------------------------
 *                          AskChest
 *              ------------------------------ */



class AskChest : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	AskChest();

	uchar getResult() {
		return _result;
	}

public slots:
	void slot_gold();

	void slot_experience();

protected:
	uchar _result;
};

#endif // ASKCHEST_H
