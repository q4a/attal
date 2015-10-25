/****************************************************************
**
** Attal : Lords of Doom
**
** ressourceWin.h
** show ressources of the player (gold, ...)
**
** Version : $Id: ressourceWin.h,v 1.10 2008/07/01 00:18:56 lusum Exp $
**
** Author(s) : Pascal Audoux - Carlo Sardi
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

 
#ifndef RESSOURCEWIN_H
#define RESSOURCEWIN_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QLabel>
#include <QLayout>
// application specific includes
#include "libCommon/genericPlayer.h"
 
enum WinResType {
	RS_GRID,
	RS_VERT
};

/*              ------------------------------
 *                         RessourceWin
 *              ------------------------------ */


/** comment for the class */
class RessourceWin : public QWidget
{
 
	Q_OBJECT
public:
	/** Construtcor */
	RessourceWin( QWidget * parent = 0, WinResType type = RS_GRID, GenericPlayer * player = NULL, GenericResourceList * list = NULL );

	void setList( GenericResourceList * list ) { _list = list; reinit(); }

public slots:
	/** Reinit info displayed */
	void reinit();
	
private:
	GenericPlayer * _player;
	QLabel * _labRessource[MAX_RESS];
	QLabel * _labIcon[MAX_RESS];
	QLabel * _labRessourcel[MAX_RESS];
	QLabel * _labIconl[MAX_RESS];
	GenericResourceList * _list;
	//QHBoxLayout * _layh[MAX_RESS/2];
};

/*              ------------------------------
 *                         TabRessourceWin
 *              ------------------------------ */


/** comment for the class */
class TabRessourceWin : public QWidget
{
	Q_OBJECT
public:
	/** Construtcor */
	TabRessourceWin( QWidget * parent = 0, WinResType type = RS_GRID, GenericPlayer * player = NULL, GenericResourceList * list = NULL );
	
	void setList( GenericResourceList * list ) { _ressW->setList( list ); }

public slots:
	/** Reinit info displayed */
	void reinit();

private:
	RessourceWin * _ressW;
};


#endif // RESSOURCEWIN_H   
