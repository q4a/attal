/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesPlayers.h
** Display properties of players
**
** Version : $Id: propertiesPlayers.h,v 1.6 2005/11/10 23:01:12 audoux Exp $
**
** Author(s) : Nickolay Semenov - Sardi Carlo
**
** Date : 05/01/2002
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


#ifndef PROPERTIESPLAYERS_H
#define PROPERTIESPLAYERS_H

// generic include files
// include files for QT
#include <QDialog>
#include <QList>

// application specific include files
#include "libCommon/genericPlayer.h"
#include "libClient/pixmapSpinBox.h"

class QComboBox;
class QCheckBox;
class QPushButton;
class AskInt;

/*              -------------------------------------------
 *                         PropertiesPlayers
 *              ------------------------------------------ */

class PropertiesPlayers : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesPlayers( QWidget * parent, QList <GenericPlayer *> * players );

	/** Destructor */
	virtual ~PropertiesPlayers();

	void accept ();

private slots:
	void slot_comboBoxItemChanged( int i );

	void slot_okButtonClicked();

	void slot_cancelButtonClicked();

	void slot_flagButtonClicked();


private:
	void setParams ();

	void getParams ();

	void formFlagsList( int numPlayer );

	/** Array of players*/
	GenericPlayer * _players;

	/** Screen players */
	QList<GenericPlayer *> * _screenPlayers;

	/** Number of players */
	uint _numPlayers;

	AskInt * _team;

	QComboBox * _comboPlayers;

	QCheckBox* _aiCheckBox;

	QPushButton* _flagButton;

	PixmapSpinBox * _spinBoxes[10];

	int _currentNumber;

	/** Free flags */
	unsigned char _freeFlagsList[255];

	int _numFreeFlags;

	int _currentFlag;
};

#endif // PROPERTIESPLAYERS_H
