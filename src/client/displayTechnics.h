/****************************************************************
**
** Attal : Lords of Doom
**
** displayTechnics.h
** Dialog for displaying the technics of a lord
**
** Version : $Id: displayTechnics.h,v 1.1 2003/05/08 20:18:54 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/05/2003
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


#ifndef DISPLAYTECHNICS_H
#define DISPLAYTECHNICS_H


// generic include files
// include files for QT
#include <qdialog.h>
// application specific include files


/*              ------------------------------
 *                       DisplayTechnics
 *              ------------------------------ */



/** comment for the class */
class DisplayTechnics : public QDialog
{

public:
	/** Constructor */
	DisplayTechnics( QWidget * parent = 0, const char * name = 0 );
};

#endif // DISPLAYTECHNICS_H


