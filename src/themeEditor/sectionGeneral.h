/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGeneral.h
** section with general info about theme
**
** Version : $Id: sectionGeneral.h,v 1.4 2003/04/11 10:11:45 audoux Exp $
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

 
#ifndef SECTIONGENERAL_H
#define SECTIONGENERAL_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
#include <qcheckbox.h>
// application specific include files
#include "themeEditor/sectionWidget.h"
 
/*              ------------------------------
 *                         SectionGeneral
 *              ------------------------------ */



/** comment for the class */
class SectionGeneral : public Section
{
	Q_OBJECT
public:
	/** Constructor */
	SectionGeneral( QWidget * parent = 0, const char * name = 0 );

	/** Saves the data */
	void save();

signals:
	/** Signal when stg change */
	void sig_changed();

private:
	AskBool * _fog;
	AskInt * _nbUnit;
	AskPixmap * _logo;
};
 
#endif // SECTIONGENERAL_H   
