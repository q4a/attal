/****************************************************************
**
** Attal : Lords of Doom
**
** conf.h
** this file contains all generic datas
**
** Version : $Id: conf.h,v 1.16 2002/09/14 00:20:51 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
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

 
#ifndef CONF_H
#define CONF_H

#include <qstring.h>

#define ATTAL_PORT 1717

const QString IMAGE_DEFAULT_PATH( "images/" );

const QString DATA_DEFAULT_PATH( "data/" );

const QString THEME_DEFAULT( "medieval" );

enum CanvasHeight {
	CAN_GROUND,
	CAN_TRANS,
	CAN_DECO,
	CAN_ARTEFACT,
	CAN_BUILDING,
	CAN_CREATURE,
	CAN_LORD=1000,
	CAN_ARROW=10000
};

enum CanvasFightHeight {
	CAN_F_GROUND,
	CAN_F_OBSTACLE,
	CAN_F_UNIT
};
#endif // CONF_H
