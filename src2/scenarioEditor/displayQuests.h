/****************************************************************
**
** Attal : Lords of Doom
**
** displayQuests.h
** Dialog for displaying quests
**
** Version : $Id: displayQuests.h,v 1.4 2005/11/12 22:17:10 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/05/2004
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


#ifndef DISPLAYQUESTS_H
#define DISPLAYQUESTS_H


// generic include files
// include files for QT
#include <QDialog>
#include <QWidget>
// application specific include files

class QListWidget;

class GameData;
class QuestManager;

/*              ------------------------------
 *                       DisplayQuests
 *              ------------------------------ */


class DisplayQuests : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayQuests( QWidget * parent = 0, const char * name = 0 );

	void reinit( QuestManager * quests );

public slots:
	void slot_main();

	void slot_edit();

	void slot_add();

	void slot_del();

	void slot_accept();

protected:
	QuestManager * _quests;
	QListWidget * _list;
};

#endif // DISPLAYQUESTS_H
