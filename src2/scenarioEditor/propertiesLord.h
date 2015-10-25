/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesLord.h
** Properties for lords
**
** Version : $Id: propertiesLord.h,v 1.17 2009/11/11 00:22:58 lusum Exp $
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
#include <QDialog>
#include <QFrame>
#include <QList>
#include <QPixmap>

// application specific include files
#include "libCommon/genericPlayer.h"

#include "libClient/pixmapSpinBox.h"

class QTableWidget;

class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTabWidget;

class AskCombo;
class AskInt;
class GenericLord;
class PropertiesLordGeneral;
class PropertiesLordUnits;
class PropertiesLordArtefacts;
class PropertiesLordWarMachines;


/**              ------------------------------
 *                        PropertiesLord
 **              ------------------------------ */

class PropertiesLord : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesLord( QList<GenericPlayer *> * players, QWidget * parent = 0 );

	void init( GenericLord * lord, uint * ids, int numLords );

public slots:
	virtual void accept();

private:
	QList<GenericPlayer *> * _players;
	GenericLord * _lord;

	QTabWidget * _tabWidget;

	PropertiesLordGeneral * _general;
	PropertiesLordUnits * _units;
	PropertiesLordArtefacts * _artefacts;
	PropertiesLordWarMachines * _warMachines;
};

/** Frame, which has one of characteristics of general (attack, defense..) */
class GeneralCharacteristic : public PixmapSpinBox
{
public:
	GeneralCharacteristic (int number, QWidget* parent = 0 );

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
  Q_OBJECT
     
public:
	PropertiesLordGeneral( QList<GenericPlayer *> * players, QWidget * parent = 0 );

	void init( GenericLord * lord, uint * ids, int numLords );

	void accept();
	
	bool hasNoOwner();

protected:
	QComboBox * _comboPlayer;
	QListWidget * _listLords;
	QLabel* _faceLord;
	QList<GenericPlayer *> * _players;
	GenericLord * _lord;
	QPushButton* _defaultButton;

	/** ids, what user CAN use for this lord (they are not used by other lords)*/
	int _ids[100];

	/** characteristics of the general */
	GeneralCharacteristic* generalChar[12];

private slots:
	void slot_lordChanged();

	void slot_defaultButtonPushed();
};

class EditTroop : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	EditTroop( int num, QWidget * parent = 0 );

	/** Clears the widget */
	void clear();

	/** Updates data */
	void updateData();

	/** Inits the widget with 'unit' */
	void init( GenericFightUnit * unit );

	/** Saves the data */
	void save( int i, GenericLord * lord );
	
	void save( int i, GenericBase * base );

	bool hasUnits ();


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
	PropertiesLordUnits( QWidget * parent = 0 );

	void init( GenericLord * lord );

	void accept();

	bool hasNoneUnits ();

protected:
	GenericLord * _lord;
	EditTroop * _troop[MAX_UNIT];
};

/* very simple: 2 lists.. no checkboxes, only 2 buttons  */

class DoubleTableWidget : public QWidget
{
      Q_OBJECT
public:
	DoubleTableWidget( QWidget * parent = 0 );

	void init( GenericLord * lord );

	void accept();

protected:
	GenericLord * _lord;

	QTableWidget * _leftList;

	QTableWidget * _rightList;

	/* true if belongs to lord, false elsethere*/
	bool _listItems[100];

	void setRowTable(int rows);

	virtual void fillList ();

	virtual void refreshList ();

private slots:

	void addButtonPushed ();

	void removeButtonPushed ();

};

class PropertiesLordWarMachines : public DoubleTableWidget
{
	Q_OBJECT
public:
	PropertiesLordWarMachines( QWidget * parent = 0 );

	void accept();

protected:

	void fillList ();

	void refreshList ();

};

class PropertiesLordArtefacts : public DoubleTableWidget
{
	Q_OBJECT
public:
	PropertiesLordArtefacts( QWidget * parent = 0 );

	void accept();

protected:

	void fillList();

	void refreshList();
};

#endif // PROPERTIESLORD_H
