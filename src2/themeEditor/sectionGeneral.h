/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGeneral.h
** section with general info about theme
**
** Version : $Id: sectionGeneral.h,v 1.4 2005/11/17 22:53:21 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

 
#ifndef SECTIONGENERAL_H
#define SECTIONGENERAL_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QCheckBox>
// application specific include files
#include "themeEditor/sectionWidget.h"

/*              ------------------------------
 *                         SectionDate
 *              ------------------------------ */


class SelectionDate : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	SelectionDate( QWidget * parent = 0, const char * name = 0 );
	
	void setTitle( QString title );
	
signals:
	void sig_first();
	void sig_previous();
	void sig_next();
	void sig_last();

private:
	QLabel * _labTitle;
};



 
/*              ------------------------------
 *                         SectionGeneral
 *              ------------------------------ */



/** comment for the class */
class SectionGeneral : public Section
{
	Q_OBJECT
public:
	/** Constructor */
	SectionGeneral( QWidget * parent = 0, const char * name = 0 );

	/** Saves the data */
	void save();
	
	/** inits the data */
	void init();

signals:
	/** Signal when stg change */
	void sig_changed();
	
public slots:
	void slot_firstDay();
	void slot_previousDay();
	void slot_nextDay();
	void slot_lastDay();

	void slot_firstWeek();
	void slot_previousWeek();
	void slot_nextWeek();
	void slot_lastWeek();

	void slot_firstMonth();
	void slot_previousMonth();
	void slot_nextMonth();
	void slot_lastMonth();
	

private:
	AskCombo * _vision;
	SelectionDate * _day, * _week, * _month;
	AskInt * _nbUnit;
	AskPixmap * _logo;
	AskString * _weekName, * _monthName, * _dayName;
	uint _dayN, _weekN, _monthN;
};
 
#endif // SECTIONGENERAL_H   
