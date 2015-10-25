/****************************************************************
**
** Attal : Lords of Doom
**
** fightControl.h
** Widget for the control board in fight mode
**
** Version : $Id: fightControl.h,v 1.3 2007/07/01 19:58:11 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

 
#ifndef FIGHTCONTROL_H
#define FIGHTCONTROL_H
 
// generic include files
// include files for QT
#include <QString>
#include <QWidget>
// application specific include files
 

class QHBoxLayout;
class QVBoxLayout;

class ChatWidget;

/*              ------------------------------
 *                         FightControl
 *              ------------------------------ */

/** Controlboard in fight mode */
class FightControl : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	FightControl( QWidget * parent = 0, const char * name = 0 );

	/** Displays new message */
	void newMessage( const QString & msg );

	/** Clear the message box */
	void clear();
	
	void updateResolutionMode();

public slots:
	/** Slot if 'Wait' button pressed */
	void slot_waitPressed( void ) { emit sig_wait(); }

	/** Slot if 'Flee' button pressed */
	void slot_fleePressed( void ) { emit sig_retreat(); }

	void slot_controlPressed ( void )
	{
		emit sig_control ();
	}

signals:
	/** Signal Control button clicked */
	void sig_control();

	/** Signal 'Spell' button clicked */
	void sig_useSpell();

	/** Signal 'Quit' button clicked */
	void sig_quit();

	/** Signal 'Autofight' button clicked */
	void sig_autoFight();

	/** Signal 'Wait' button clicked */
	void sig_wait();

	/** Signal 'Defend' button clicked */
	void sig_defend();

	/** Signal 'Retreat' button clicked */
	void sig_retreat();

	/** Signal 'Surrender' button clicked */
	void sig_surrender();
	
	void sig_message( QString );
	
	void sig_newMessage( const QString & );

private:
	ChatWidget * _chat;
	QHBoxLayout * _layout;
	QVBoxLayout * _layV1;	
};

 
#endif // CLASSNAME_H   
