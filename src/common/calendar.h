/****************************************************************
**
** Attal : Lords of Doom
**
** calendar.h
** class for managing days, weeks, months
**
** Version : $Id: calendar.h,v 1.4 2001/03/05 16:17:03 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/01/2001
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

 
#ifndef CALENDAR_H
#define CALENDAR_H
 
 
// generic include files
// include files for QT
#include <qobject.h>
// application specific include files
 
 
/*              ------------------------------
 *                         Calendar
 *              ------------------------------ */



/** comment for the class */
class Calendar : public QObject
{
	Q_OBJECT
public:
	/** Constructor */
	Calendar();
	
	/** Set date before first day (first week, first month) */
	void reinit();
	
	/** Go to next day */
	void newDay();
	
	/** Return the current day */
	int getDay() { return _day; }
	
	/** Return the current week */
	int getWeek() { return _week; }
	
	/** Return the current month */
	int getMonth() { return _month; }
	
signals:
	/** Signal when date change */
	void sig_changed();
	
private:
	int _day, _week, _month;
};
 
#endif // CALENDAR_H   
