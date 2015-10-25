/****************************************************************
**
** Attal : Lords of Doom
**
** calendar.cpp
** class for managing days, weeks, months
**
** Version : $Id: calendar.cpp,v 1.21 2007/04/06 12:55:06 lusum Exp $
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

// generic include files
// include files for QT
#include <QTextStream>


// application specific include files
#include "calendar.h"

#include "libCommon/dataTheme.h"

/** add comments here */
Calendar::Calendar()
	:_day( 0 ),
	 _week( 1 ),
	 _month( 1 ),
	 _year( 1044 ),
	 _turn ( 0 )
{

}

void Calendar::reinit()
{
	_day = 0;
	_daynum = 0;
	_month = 1;
	_week = 1;
	_year = 1044;
	_turn = 0;
}

void Calendar::newDay()
{
	_day++;
	_daynum++;
	_turn++;
	if( _day > FIRST_LEVEL ) {
		_day = 1;
		_week++;
	}
	if( _week > SECOND_LEVEL ) {
		_week = 1;
		_month++;
		_daynum = 1;
	}
	if( _month > THIRD_LEVEL ) {
		_month = 1;
		_year++;
	}
	
	emit sig_changed();
}

uint Calendar::getCurrentAbsoluteDate() 
{
	uint absoluteDate;
	absoluteDate = getAbsoluteDate( _day , _week , _month , _year);
	return absoluteDate;
}

uint Calendar::getStartAbsoluteDate()
{
	uint absoluteDate;
	absoluteDate = getAbsoluteDate( _startday ,  _startweek , _startmonth , _startyear);
	return absoluteDate;
}	

uint Calendar::getAbsoluteDate( uint day, uint week, uint month, uint year )
{
	uint absoluteDate = 0;
	absoluteDate = day + ( week * 7 ) + ( month * 4 * 7 ) + year * 365;
	return absoluteDate;
}

QString Calendar::getDate() 
{
	CalendarModel * tmpCal = DataTheme.general.getCalendar();
	Q_CHECK_PTR(tmpCal);
	//return "";
	return tmpCal->getLevelName(0,(_day-1)) +	QString( ", %1 " ).arg( _daynum ) + tmpCal->getLevelName(2,(_month-1)) + QString( ", %1 " ).arg( _year );
}

QString Calendar::getDayName() 
{
	CalendarModel * tmpCal = DataTheme.general.getCalendar();
	Q_CHECK_PTR(tmpCal);
	return tmpCal->getLevelName(0,(_day-1));
}

void Calendar::setDateByType(uint type, uint value)
{
	switch(type) {
		case 0:
			if (value <= FIRST_LEVEL ) _day = value ;
			break;
		case 1: 
			_daynum = value;
			break;
		case 2:
			if (value <= SECOND_LEVEL ) _week = value;
			break;
		case 3:
			if (value <= THIRD_LEVEL ) _month = value;
			break;
		case 4:
			_year = value;
			break;
		default:
			break;
	}

	//date is set before initial date, so when initial date is set, a valid data is present
	setInitialDateByType(type, value);
}

void Calendar::setInitialDateByType(uint type, uint value)
{
	switch(type) {
		case 0:
			if (value <= FIRST_LEVEL ) _startday = value ;
			break;
		case 1: 
			_startdaynum = value;
			break;
		case 2:
			if (value <= SECOND_LEVEL ) _startweek = value;
			break;
		case 3:
			if (value <= THIRD_LEVEL ) _startmonth = value;
			break;
		case 4:
			_startyear = value;
			break;
		default:
			break;
	}
}

uint Calendar::getDateByType( uint type )
{
	switch(type) {
		case 0:
			return _day;
			break;
		case 1: 
			return _daynum;
			break;
		case 2:
			return _week;
			break;
		case 3:
			return _month;
			break;
		case 4:
			return _year;
			break;
		default:
			logEE( "Error, unknow type of date" );
			return _day;
			break;
	}

	
}

uint Calendar::getInitialDateByType( uint type )
{
	switch(type) {
		case 0:
			return _startday;
			break;
		case 1: 
			return _startdaynum;
			break;
		case 2:
			return _startweek;
			break;
		case 3:
			return _startmonth;
			break;
		case 4:
			return _startyear;
			break;
		default:
			logEE( "Error, unknow type of date" );
			return _startday;
			break;
	}

	
}

void Calendar::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<calendar>" << endl;

	for( uint i = 0; i < 5; i++ ) {
			indentation( ts, indent + 1 );
			*ts << "<value type=\"" << i << "\">";
			*ts << getDateByType( i ) << "</value>" << endl;
	}

	indentation( ts, indent + 1 );
	*ts << "<startcalendar>" << endl;

	for( uint i = 0; i < 5; i++ ) {
			indentation( ts, indent + 2 );
			*ts << "<value type=\"" << i << "\">";
			*ts << getInitialDateByType( i ) << "</value>" << endl;
	}
	indentation( ts, indent + 1 );
	*ts << "</startcalendar>" << endl;
	
	indentation( ts, indent + 1 );
	*ts << "<turn>" << getTurn() << "</turn>" << endl;
	indentation( ts, indent );
	*ts << "</calendar>" << endl;
	*ts << flush;

}

//
// ----- CalendarModel -----
//

CalendarModel::CalendarModel()
{
	_name = 0;
	_level[0] = 7;
	_level[1] = 4;
	_level[2] = 12;
	_name = new QVector<QString *>(MAX_NAME * MAX_LEVEL); 
}	

CalendarModel::~CalendarModel()
{

	if (_name)
	{
		for (int i = 0; i < _name->size(); ++i) {
			QString * ptr = _name->at(i);
			(*_name)[i] = 0;
			delete ptr;
		}
		delete _name;
		_name = 0;
	}
	
}

QString CalendarModel::getLevelName (uint level, uint num )
{
	if( level < MAX_LEVEL){
		if( num < _level[level] ) {
			if( _name->at( level * MAX_NAME + num ) ) {
				//logDD(" level %d, num %d",level,num);
				//logDD("name %s",_name->at(level * 30 +num)->toLatin1());
				return *_name->at(level * MAX_NAME +num);
			}
		}
	}
	return "";

}

void CalendarModel::setLevelName (uint level, uint num, const QString & name )
{
	//logDD("level %d, num %d",level,num);
	//logDD("name %s",_name->at(level * 30 +num)->toLatin1());
	if( ( level < MAX_LEVEL ) && ( name != "" ) ) {
		if( num < _level[level] ) {
			int tmp = level * MAX_NAME + num;
			if((*_name)[tmp]) {
				delete (*_name)[tmp];
			}
			(*_name)[ tmp ] = new QString(name);
		}
	}
}

uint CalendarModel::getLevelValue( uint level) 
{
	if( level < MAX_LEVEL)
		return _level[level];
	return 0;
}

void CalendarModel::setLevelValue( uint level, uint value ) 
{
	if( level < MAX_LEVEL)
		_level[level] = value; 
}

