/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGround.h
** sectin for specifying grounds
**
** Version : $Id: sectionGround.h,v 1.2 2002/10/20 15:14:40 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

 
#ifndef SECTIONGROUND_H
#define SECTIONGROUND_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
// application specific include files
#include "themeEditor/sectionWidget.h"
 
/*              ------------------------------
 *                         SectionGround
 *              ------------------------------ */



/** comment for the class */
class SectionGround : public Section
{
 
public:
	/** Constructor */
	SectionGround( QWidget * parent = 0, const char * name = 0 );

	/** Saves the data */
	void save();
};
 
#endif // SECTIONGROUND_H   
