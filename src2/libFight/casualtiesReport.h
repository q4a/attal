/****************************************************************
**
** Attal : Lords of Doom
**
** casualtiesReport.h
** Report widget about casualties of a lord
**
** Version : $Id: casualtiesReport.h,v 1.3 2005/12/12 10:36:33 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/12/2005
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

 
#ifndef CASUALTIESREPORT_H
#define CASUALTIESREPORT_H
 
 
// generic include files
// include files for QT
#include <QFrame>
#include <QString>

// application specific include files
#include "libCommon/attalSocket.h"


class QListWidget;

class Creature;
class Fight;
class GenericLord;
class Icon;

/*              ------------------------------
 *                      CasualtiesReport
 *              ------------------------------ */


class CasualtiesReport : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	CasualtiesReport( CLASS_FIGHTER fighter, Fight * fight, QWidget * parent = 0, const char * name = 0 );

protected:
	void displayCasualtiesLord( GenericLord * lord );

	void displayCasualtiesCreature( Creature * creature );

	Fight * _fight;
	CLASS_FIGHTER _fighter;
	QListWidget * _list;
	Icon * _photo;
	QString _reportTitle;
};
 
#endif // CASUALTIESREPORT_H
