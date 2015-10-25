/****************************************************************
**
** Attal : Lords of Doom
**
** optionsDialog.h
** Options Dialog in main window of client
** 
** Author(s) : Nickolay Semenov
**
** Date : 23/08/2004
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

 
#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

// include files for QT
#include <QDialog>


class QCheckBox;

class AskCombo;
 
 
/*              ------------------------------
 *                      Options Dialog
 *              ------------------------------ */

class OptionsDialog: public QDialog
{
 	Q_OBJECT
public:
	OptionsDialog ( QWidget* parent = 0, const char * name = 0 );
	
public slots:
	void accept();
	
	void musicVChanged( int value );
	
	void soundVChanged( int value );
	
 	void soundEnable( int state );
 	
	void musicEnable( int state );

signals:
	
	void sig_animation( int );
	
	void sig_dispositions();
	
private:
	void init();
	
	void updateSettings();

	QCheckBox * _animationCheckBox;
	QCheckBox * _saveCheckBox;
	AskCombo * _dispositionMode;
};
 
#endif // OPTIONSDIALOG_H 
