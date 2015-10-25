/****************************************************************
**
** Attal : Lords of Doom
**
** Author(s) : Nick Semenov
**
** Date : 06/12/2003
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


#ifndef FIGHTSETTINGSDIALOG_H
#define FIGHTSETTINGSDIALOG_H


// generic include files
// include files for QT
#include <QDialog>
// application specific include files
class QCheckBox;
class AskInt;

/*              ------------------------------
 *                         FightSettingsDialog
 *              ------------------------------ */


class FightSettingsDialog: public QDialog
{
	Q_OBJECT
public:
	FightSettingsDialog(QWidget* parent);

	void accept ();

private slots:
	void slot_okButtonClicked();

	void slot_cancelButtonClicked();

	void slot_animationCheckBoxClicked();

private:
	QCheckBox * _animationCheckBox;

	AskInt * _animationSpeed;

	QCheckBox * _cellsCheckBox;

};

#endif // FIGHTSETTINGSDIALOG_H
