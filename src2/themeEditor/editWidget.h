/*****************************************************************
**
** Attal : Lords of Doom
**
** editWidget.h
** Widget for editing
**
** Version : $Id: editWidget.h,v 1.1 2006/10/03 20:41:43 lusum Exp $
**
** Author(s) : Sardi Carlo
**
** Date : 03/09/2006
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

#ifndef EDITWIDGET_H
#define EDITWIDGET_H

// generic include files
// include files for QT
#include<QListWidget>
// application specific include files

class QPushButton;

class EditWidget : public QWidget
{
	Q_OBJECT
public:
	EditWidget( QWidget * parent = 0, const char * name = 0 );

protected:
	QListWidget * _listBox;
	QPushButton * _pbAdd, * _pbEdit, * _pbDel;
};

#endif
