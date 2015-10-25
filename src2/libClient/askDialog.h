/****************************************************************
**
** Attal : Lords of Doom
**
** askDialog.h
** Dialog for questions to player
**
** Version : $Id: askDialog.h,v 1.6 2005/11/02 21:52:20 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/11/2004
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

 
#ifndef ASKDIALOG_H
#define ASKDIALOG_H
 
 
// generic include files
// include files for QT
#include <QDialog>

// application specific include files
 
class QLabel;
class AttalButton;
 
/*              ------------------------------
 *                         AskDialog
 *              ------------------------------ */


class AskDialog : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	AskDialog();
	
	void setText( const QString & text );
	
	void setYesNo();
	
private:
	QLabel * _text;
	AttalButton * _pb1, * _pb2;
};
 
#endif // ASKDIALOG_H   
