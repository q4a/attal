/****************************************************************
**
** Attal : Lords of Doom
**
** askWidget.h
** Classes for asking values to user
**
** Version : $Id: askWidget.h,v 1.3 2003/05/10 08:29:15 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/01/2003
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


#ifndef ASKWIDGET_H
#define ASKWIDGET_H


// generic include files
// include files for QT
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qstring.h>
#include <qwidget.h>
// application specific include files

class QComboBox;
class QLabel;
class QListBox;
class QListBoxItem;
class QPixmap;
class QPushButton;

/** Usefull widget for asking a string */
class AskString : public QWidget
{
public:
	/** Constructor */
	AskString( QString text = "", QWidget * parent = 0, const char * name = 0 );

	/** Set the text of the question */
	void setText( QString text );

	/** \return Returns the value asked (a string) */
	QString getValue() { return _value->text(); }

	/** Sets the value */
	void setValue( QString value ) { _value->setText( value ); }

	/** Sets the width of the lineedit */
	void setValueSize( int size ) { _value->setFixedWidth( size ); }

private:
	QLineEdit * _value;
	QLabel * _label;
};

/** Usefull widget for asking a boolean */
class AskBool : public QWidget
{
public:
	/** Constructor */
	AskBool( QString text = "", QWidget * parent = 0, const char * name = 0 );

	/** Set the text of the question */
	void setText( QString text );

	/** \return Returns the value */
	bool getValue() { return _value->isChecked(); }

	/** Sets the value */
	void setValue( bool value ) { _value->setChecked( value ); }

private:
	QCheckBox * _value;
};

/** Usefull widget for asking an int */
class AskInt : public QWidget
{
public:
	/** Constructor */
	AskInt( QString text = "", QWidget * parent = 0, const char * name = 0 );

	/** Sets text of the question */
	void setText( QString text );

	/** \return Returns the value */
	int getValue() { return _value->value(); }

	/** Sets the value */
	void setValue( int value ) { _value->setValue( value ); }

	/** Sets the 'min' value */
	void setMinValue( int val ) { _value->setMinValue( val ); }

	/** Sets the 'max' value */
	void setMaxValue( int val ) { _value->setMaxValue( val ); }

private:
	QSpinBox * _value;
	QLabel * _label;
};

/** Usefull widget for asking a case in an enum */
class AskCombo : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	AskCombo( QString text = "", QWidget * parent = 0, const char * name = 0 );

	/** Clears the combo */
	void clear();

	/** Sets the text of the question */
	void setText( QString text );

	/** Inserts an item in the combo */
	void insertItem( QString item );

	/** \return Returns current item */
	int currentItem();

	/** Sets current item to num */
	void setCurrentItem( int num );

signals:
	void sig_activated( int );

private:
	QLabel * _label;
	QComboBox * _combo;
};

/** Useful widget for asking a pixmap */
class AskPixmap : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	AskPixmap( bool display, QString destination, QString text = "", QWidget * parent = 0, const char * name = 0 );

	/** Destructor */
	~AskPixmap();

	/** \return Returns the value (path to the pixmap) */
	QString getValue() { return _value; }

	/** Sets path to the pixmap */
	void setValue( QString value );

	/** \return Returns destination of the pixmap */
	QString getDestination() { return _destination; }

	/** Sets destination of the pixmap */
	void setDestination( QString destination );

	/** Save pixmap to the destination */
	void save();

public slots:
	/** Slot for loading the pixmap */
	void slot_loadPixmap();

private:
	void updateDisplay();

	QPixmap * _pix;
	QString _value, _destination;
	QPushButton * _butLoad;
	bool _display;
};

class AskList : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	AskList( QWidget * parent = 0, const char * name = 0 );

	void setLabel( QString label );

	/** Sets limit of the list. 0 means no limit */
	void setLimit( int limit );

	/** Returns the current number of items */
	uint count();

	void clear();

public slots:
	/** Slot for adding a new item */
	void slot_new();

	/** Slot for deleting the current item */
	void slot_del();

	/** Slot for putting the current item up */
	void slot_up();

	/** Slot for putting the current item down */
	void slot_down();

	/** Slot for changing the value of an item */
	void slot_change( QListBoxItem * item );

protected:
	virtual QString askValue( QString val, bool & ok );

	int _limit;
	QListBox * _list;
	QLabel * _label;
	QPushButton * _pbNew, * _pbDel, * _pbUp, * _pbDown;
};

class AskStringList : public AskList
{
public:
	AskStringList( QWidget * parent = 0, const char * name = 0 );

	void addValue( QString value );

	void setValue( int num, QString value );

	QString getValue( int num );

protected:
	//virtual QString askValue();
};

class AskIntList : public AskList
{
public:
	AskIntList( QWidget * parent = 0, const char * name = 0 );

	void addValue( int value );

	void setValue( int num, int value );

	int getValue( int num );

	void setMinValue( int value );

	void setMaxValue( int value );

protected:
	virtual QString askValue( QString value, bool & ok );

	int _min, _max;
};




#endif // ASKWIDGET_H
