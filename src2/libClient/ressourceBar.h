/****************************************************************
**
** Attal : Lords of Doom
**
** ressourceBar.h
** show ressources of the player (gold, ...)
**
** Version : $Id: ressourceBar.h,v 1.4 2007/10/12 21:44:27 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/09/2000
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

 
#ifndef RESSOURCEBAR_H
#define RESSOURCEBAR_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QLabel>
// application specific includes
#include "libCommon/genericPlayer.h"
 
 
/*              ------------------------------
 *                         RessourceBar
 *              ------------------------------ */


/** comment for the class */
class RessourceBar : public QWidget
{
 
public:
	/** Construtcor */
	RessourceBar( QWidget * parent = 0, const char * name = 0 );

	/** Initialize player */
	void setPlayer( GenericPlayer * );
	
	/** Reinit info displayed */
	void reinit();
	
private:
	GenericPlayer * _player;
	QLabel * _labRessource[MAX_RESS], * _totPopul;
	QLabel * _labPopul;
};
 
#endif // RESSOURCEBAR_H   
