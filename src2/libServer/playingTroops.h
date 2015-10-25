/****************************************************************
**
** Attal : Lords of Doom
**
** playingTroops.h
** manage playing troops
**
** Version : $Id: playingTroops.h,v 1.1 2006/08/30 18:56:28 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/05/2001
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


#ifndef PLAYINIGTROOPS_H
#define PLAYINIGTROOPS_H

#include "libCommon/unit.h"

#include <QList>

/**              ------------------------------
 *                          PlayingTroops
 **              ------------------------------ */

class PlayingTroops : public QList<GenericFightUnit *>
{
public:
	PlayingTroops( bool order = true );

	void add( GenericFightUnit * unit ) {
		append( unit );
		qSort(begin(), end());
	}

 	GenericFightUnit * take();

protected:
	int compareItems( GenericFightUnit * i1, GenericFightUnit * i2 );

private:
	bool _order;
};

#endif //PLAYINIGTROOPS_H

