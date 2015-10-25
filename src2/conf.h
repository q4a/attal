/****************************************************************
**
** Attal : Lords of Doom
**
** conf.h
** this file contains all generic datas
**
** Version : $Id: conf.h,v 1.13 2009/06/05 16:52:52 lusum Exp $
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

#include <QString>

#define ATTAL_PORT 1717

const QString IMAGE_DEFAULT_PATH( "images/" );

const QString MUSIC_DEFAULT_PATH( "musics/" );

const QString SOUND_DEFAULT_PATH( "sounds/" );

const QString DATA_DEFAULT_PATH( "data/" );

const QString CAMPAIGN_DEFAULT_PATH( "campaigns/" );

const QString SCENARIO_DEFAULT_PATH( "scenarii/" );

const QString TRANSLATIONS_DEFAULT_PATH( "translations/" );

const QString THEME_DEFAULT( "medieval" );

#ifdef WIN32
const QString CONFIG_DEFAULT_PATH( "attal/" );
#else
const QString CONFIG_DEFAULT_PATH( ".attal/" );
#endif

enum CanvasHeight {
	CAN_GROUND,
	CAN_TRANS,
	CAN_DECO,
	CAN_ARTEFACT,
	CAN_BUILDING,
	CAN_CREATURE,
	CAN_ARROW=1000,
	CAN_NUM_ARROW,
	CAN_LORD=10000
};

enum CanvasFightHeight {
	CAN_F_UNIT_DEAD,
	CAN_F_GROUND,
	CAN_F_OBSTACLE,
	CAN_F_UNIT,
	CAN_F_UNIT_HL
};
#endif // CONF_H
