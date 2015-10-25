/****************************************************************
**
** Attal : Lords of Doom
**
** sectionWidget.h
** widgets for building sections
**
** Version : $Id: sectionWidget.h,v 1.6 2003/01/07 21:10:31 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

 
#ifndef SECTIONWIDGET_H
#define SECTIONWIDGET_H
 
 
// generic include files
// include files for QT
#include <qwidget.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qcheckbox.h>
// application specific include files
#include "client/askWidget.h"

class QLabel;
class QComboBox;
class QPixmap;
class QPushButton;

/*              ------------------------------
 *                         SectionWidget
 *              ------------------------------ */



/** comment for the class */
class SelectionWidget : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	SelectionWidget( QWidget * parent = 0, const char * name = 0 );
	
	void setTitle( QString title );
	
signals:
	void sig_first();
	void sig_previous();
	void sig_next();
	void sig_last();
	void sig_new();
	void sig_del();
	
private:
	QLabel * _labTitle;
};

/** Pure virtual class for sections */
class Section : public QWidget
{
public:
	Section( QWidget * parent = 0, const char * name = 0 );

	virtual void save() = 0;
};

/** Pure virtual class with usefull api for section */
class GenericSection : public Section
{
	Q_OBJECT
public:
	/** Constructor */
	GenericSection( QWidget * parent = 0, const char * name = 0 );

	/** Selects the first item */
	virtual void selectFirst() = 0;

	/** Selects the previous item */
	virtual void selectPrevious() = 0;

	/** Selects the next item */
	virtual void selectNext() = 0;

	/** Selects the last item */
	virtual void selectLast() = 0;

	/** Creates new item */
	virtual void selectNew() = 0;

	/** Deletes current item */
	virtual void selectDel() = 0;

	/** Saves the data */
	virtual void save() = 0;

	/** Sets title to the section */
	void setTitle( QString title ) { _select->setTitle( title ); }

public slots:

	/** Slot when 'First' button is clicked */
        void slot_first() {
		selectFirst();
	}

	/** Slot when 'Previous' button is clicked */
	void slot_previous() {
		selectPrevious();
	}

	/** Slot when 'Next' button is clicked */
	void slot_next() {
		selectNext();
	}

	/** Slot when 'Last' button is clicked */
	void slot_last() {
		selectLast();
	}

	/** Slot when 'New' button is clicked */
	void slot_new() {
		selectNew();
	}

	/** Slot when 'Del' button is clicked */
	void slot_del() {
		selectDel();
	}

protected:
	SelectionWidget * _select;
	QWidget * _mainWidget;
};

#endif // SECTIONWIDGET_H
