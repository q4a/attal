/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesLord.h
** Properties for lords
**
** Version : $Id: propertiesLord.h,v 1.3 2003/05/20 12:19:50 audoux Exp $
**
** Author(s) : Pascal Audoux & Nick Semenov
**
** Date : 07/12/2002
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


#ifndef PROPERTIESLORD_H
#define PROPERTIESLORD_H


// generic include files
// include files for QT
#include <qframe.h>
#include <qlist.h>
#include <qtabdialog.h>
// application specific include files
#include "common/genericPlayer.h"

#include "client/pixmapSpinBox.h"

class QComboBox;

class AskCombo;
class AskInt;
class GenericLord;
class PropertiesLordGeneral;
class PropertiesLordUnits;
class PropertiesLordArtefacts;

/**              ------------------------------
 *                        PropertiesLord
 **              ------------------------------ */

class PropertiesLord : public QTabDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesLord( QList<GenericPlayer> * players, QWidget * parent = 0, const char * name = 0 );

	void init( GenericLord * lord );

public slots:
	virtual void accept();

private:
	QList<GenericPlayer> * _players;
	GenericLord * _lord;

	PropertiesLordGeneral * _general;
	PropertiesLordUnits * _units;
	PropertiesLordArtefacts * _artefacts;
};

/** Frame, which has one of characteristics of general (attack, defense..) */
class GeneralCharacteristic : public PixmapSpinBox
{
public:
       GeneralCharacteristic (int number, QWidget* parent = 0, const char* name = 0);

        void init ( GenericLord* lord);

        void save ( GenericLord* lord );

private:

      // gets Pixmap by number
       void getPixmap ();

       // gets Text by number
       QString getText ();

        /** rememeber the number of characteristic */
        int _num;

        QPixmap _pix;
        
};

class PropertiesLordGeneral : public QWidget
{
public:
	PropertiesLordGeneral( QList<GenericPlayer> * players, QWidget * parent = 0, const char * name = 0 );

	void init( GenericLord * lord );

	void accept();

protected:
	QComboBox * _comboPlayer;
	QList<GenericPlayer> * _players;
	GenericLord * _lord;

       /** characteristics of the general */
       GeneralCharacteristic* generalChar[12];
};

class EditTroop : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	EditTroop( int num, QWidget * parent = 0, const char * name = 0 );

	/** Clears the widget */
	void clear();

	/** Updates data */
	void updateData();

	/** Inits the widget with 'unit' */
	void init( GenericFightUnit * unit );

	/** Saves the data */
	void save( int i, GenericLord * lord );

public slots:
	/** Slot if races have changed */
        void slot_raceChanged( int race );

private:
	GenericFightUnit * _unit;
	AskCombo * _race, * _name;
	AskInt * _number;
};

class PropertiesLordUnits : public QWidget
{
public:
	PropertiesLordUnits( QWidget * parent = 0, const char * name = 0 );

	void init( GenericLord * lord );

	void accept();

protected:
	GenericLord * _lord;
	EditTroop * _troop[MAX_UNIT];
};

class PropertiesLordArtefacts : public QWidget
{
public:
	PropertiesLordArtefacts( QWidget * parent = 0, const char * name = 0 );

	void init( GenericLord * lord );

	void accept();

protected:
	GenericLord * _lord;
};


#endif // PROPERTIESLORD_H
