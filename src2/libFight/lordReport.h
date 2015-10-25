/****************************************************************
**
** Attal : Lords of Doom
**
** lordReport.h
** report about lord improvements 
**
** Version : $Id: lordReport.h,v 1.1 2005/12/03 22:23:13 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/12/2005
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

 
#ifndef LORDREPORT_H
#define LORDREPORT_H
 
 
// generic include files
// include files for QT
#include <QWidget>
// application specific include files
 
 
/*              ------------------------------
 *                         LordReport
 *              ------------------------------ */



class LordReport : public QWidget
{
 
public:
	/** Constructor */
	LordReport( QWidget * parent = 0, const char * name = 0 );
};
 
#endif // LORDREPORT_H   
