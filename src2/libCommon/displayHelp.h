/****************************************************************
**
** Attal : Lords of Doom
**
** displayHelp.h
** Dialog for displaying help (used by all exe)
**
** Version : $Id: displayHelp.h,v 1.3 2006/03/26 13:39:44 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/11/2004
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

 
#ifndef DISPLAYHELP_H
#define DISPLAYHELP_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QWidget>
// application specific include files
 

class QTextBrowser;
 
/*              ------------------------------
 *                         DisplayHelp
 *              ------------------------------ */


class DisplayHelp : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayHelp( QWidget * parent = 0, const char * name = 0 );

public slots:
	void slot_accept();
	
protected:
	QTextBrowser * _text;
};
 
#endif // DISPLAYHELP_H   
