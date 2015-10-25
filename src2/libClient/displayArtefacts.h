/****************************************************************
**
** Attal : Lords of Doom
**
** displayArtefacts.h
** Displays the artefacts of a lord
**
** Version : $Id: displayArtefacts.h,v 1.6 2008/07/01 23:11:19 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/12/2003
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

 
#ifndef DISPLAYARTEFACTS_H
#define DISPLAYARTEFACTS_H
 
 
// generic include files
// include files for QT
#include <QFrame>
// application specific include files
#include "libClient/widget.h"


class GenericPlayer;
class Player;

/*              ------------------------------
 *                     DisplayArtefacts
 *              ------------------------------ */


/** artefact information of the lord */
class DisplayArtefacts : public QFrame
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayArtefacts( Player * player, QWidget * parent = 0, const char * name = 0 );

	/** Destructor */
	~DisplayArtefacts();

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate() { reinit(); }

public slots:
	void slot_nextArtefact();
	void slot_prevArtefact();

private:
	Player * _player;
	IconArtefact * _ico[3];
	uint _index;
};

class DisplayLordTabArtefacts : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayLordTabArtefacts( GenericPlayer * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

public slots:
	void slot_nextArtefact();
	void slot_prevArtefact();

protected:
	GenericPlayer * _player;
	IconArtefact * _ico[ 4 ]; /// XXX TODO why 4...
	
	uint _index;
};

 
#endif // DISPLAYARTEFACTS_H
