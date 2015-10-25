/****************************************************************
**
** Attal : Lords of Doom
**
** gameInfo.h
** Widget giving info during the game
**
** Version : $Id: gameInfo.h,v 1.8 2005/11/12 19:41:15 audoux Exp $
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

 
#ifndef GAMEINFO_H
#define GAMEINFO_H
 
 
// generic include files
// include files for QT
#include <QColor>
#include <QLabel>
#include <QLayout>
#include <QMouseEvent>
#include <QStackedWidget>
#include <QWidget>

// application specific include files
#include "libCommon/define.h" 

class QLabel; 

class Calendar;
class GenericLord;
class GenericBase;
class Icon;

/*              ------------------------------
 *                         GameInfo
 *              ------------------------------ */

class GameInfo : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	GameInfo( Calendar * calendar, QWidget * parent = 0, const char * name = 0 );

	/** Display the logo */
	void nothing();
	
	void setStatePlayer( bool state );

	/** Display 'waiting for player' animation */
	void waitPlayer( uint num );
	
private slots:
	/** Slot for updating date info */
	void slot_dateChanged();
	
private:
	QWidget * _main;
	QLabel * _waitPlayer;
	
	QLabel * _nothing, *_isPlay;
	QLabel * _dateM, * _dateW, * _dateD;
	Calendar * _calendar;
};

class LordInfo : public QWidget
{
public:
	/** Constructor */
	LordInfo( QWidget * parent = 0, const char * name = 0 );

	void init( GenericLord * lord );
	
	QLabel * _numUnit[ MAX_UNIT ];
	
protected:
	GenericLord * _lord;
	Icon * _photo;
	QLabel * _name, * _level;
	Icon * _units[ MAX_UNIT ];
};

class BaseInfo : public QWidget
{
public:
	/** Constructor */
	BaseInfo( QWidget * parent = 0, const char * name = 0 );

	void init( GenericBase * base );
	
protected:
	GenericBase * _base;
	QLabel * _name;
	QLabel * _info;
};

class InfoStack : public QStackedWidget
{
public:
	InfoStack( QWidget * parent );
	
	void init( QWidget * game, QWidget * lord, QWidget * base );
	
	void raiseLord();
	
	void raiseBase();
	
	void raiseInfo();
	
protected:
	virtual void mousePressEvent( QMouseEvent * e );

	QWidget * _info, * _lord, * _base;
	
	QWidget * _current;	
};

 
#endif // GAMEINFO_H   
