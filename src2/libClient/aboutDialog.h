/****************************************************************
**
** Attal : Lords of Doom
**
** aboutDialog.h
** dialog for displaying info about Attal and link to website
**
** Version : $Id: aboutDialog.h,v 1.2 2005/12/12 10:36:33 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/11/2005
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

 
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
 
 
// generic include files
// include files for QT
#include <QDialog>
// application specific include files
 
 
/*              ------------------------------
 *                         AboutDialog
 *              ------------------------------ */


class AboutDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	AboutDialog( QWidget * parent = 0 );
};
 
#endif // ABOUTDIALOG_H   
