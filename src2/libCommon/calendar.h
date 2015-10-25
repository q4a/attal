/****************************************************************
**
** Attal : Lords of Doom
**
** calendar.h
** class for managing days, weeks, months
**
** Version : $Id: calendar.h,v 1.17 2007/04/06 12:55:06 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QObject>
#include <QVector>
// application specific include files
 
#define FIRST_LEVEL 7
#define SECOND_LEVEL 4
#define THIRD_LEVEL 12
#define MAX_NAME 30
#define MAX_LEVEL 3


class QTextStream;
 
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
	uint getDay() { return _day; }
	
	/** Return the current number of day */
	uint getDayNumber() { return _daynum; }

	/** Return the current week */
	uint getWeek() { return _week; }

	/** Return the current month */
	uint getMonth() { return _month; }
	
	/** Return the current year */
	uint getYear() { return _year; }
	
	/** Set the current day */
	void setDay(uint day) { if (day <= FIRST_LEVEL ) _day = day ;}
	
	/** Set the current day number */
	void setDayNumber(uint daynum) { _daynum = daynum; }

	/** Set the current week */
	void setWeek(uint week) { if (week <= SECOND_LEVEL ) _week = week; }

	/** Set the current month */
	void setMonth(uint month) { if (month <= THIRD_LEVEL ) _month = month; }
	
	/** Set the current year */
	void setYear(uint year) {  _year = year; }

	//set and get current date	
	void setDateByType(uint type, uint value);

	uint getDateByType(uint type);

	//set and get starting scenario date	
	void setInitialDateByType(uint type, uint value);

	uint getInitialDateByType( uint type );
	
	QString getDate(); 
	
	QString getDayName(); 
	
	int getTurn() { return _turn; }
	
	void setTurn( uint turn ) { _turn = turn; }

	uint getCurrentAbsoluteDate();

	uint getStartAbsoluteDate();

	static uint getAbsoluteDate( uint day, uint week, uint month, uint year );

	void save( QTextStream * ts, int indent );

signals:
	/** Signal when date change */
	void sig_changed();

private:
	uint _day, _daynum, _week, _month, _year, _turn;
	uint _startday, _startweek, _startmonth, _startyear, _startdaynum;
};

/** comment for the class */
class CalendarModel
{
 
public:
	/** Constructor */
	CalendarModel();

	/** Destructor */
	virtual ~CalendarModel();

	QString getLevelName(uint level, uint num);
	 
	void setLevelName(uint level, uint num, const QString & name);
	
	uint getLevelValue(uint level);

	void setLevelValue( uint level , uint value );

protected:
	uint _level[3];
	QVector<QString *> * _name;

};


 
#endif // CALENDAR_H   
