/****************************************************************
**
** Attal : Lords of Doom
**
** gameInfo.h
** Widget giving info during the game
**
** Version : $Id: gameInfo.h,v 1.4 2001/03/05 16:43:48 audoux Exp $
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
#include <qlayout.h>
#include <qwidget.h>
#include <qcolor.h>
// application specific include files
 
class QLabel; 
class Calendar;

/*              ------------------------------
 *                         GameInfo
 *              ------------------------------ */



/** comment for the class */
class GameInfo : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	GameInfo( Calendar * calendar, QWidget * parent = 0, const char * name = 0 );

	/** Display the logo */
	void nothing();
	
	/** Display 'waiting for player' animation */
	void waitPlayer( QColor color );
	
private slots:
	/** Slot for updating date info */
	void slot_dateChanged();
	
private:
	QWidget * _main;
	QWidget * _waitPlayer;
	
	QLabel * _nothing;
	QLabel * _dateM, * _dateW, * _dateD;
	Calendar * _calendar;
};
 
#endif // GAMEINFO_H   
