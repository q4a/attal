/****************************************************************
**
** Attal : Lords of Doom
**
** gameDescription.h
** description of game info
**
** Version : $Id: gameDescription.h,v 1.13 2002/09/30 21:14:33 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/01/2001
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

 
#ifndef GAMEDESCRIPTION_H
#define GAMEDESCRIPTION_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qcolor.h>
#include <qpixmap.h>
#include <qlist.h>
// application specific include files
#include "common/genericLord.h"

#include "client/lord.h"
#include "client/graphicalArtefact.h"

class Calendar;

 
/*              ------------------------------
 *                         GameDescription
 *              ------------------------------ */



/** comment for the class */
class GameDescription
{
 
public:
	/** Construtor */
	GameDescription();

	/** Virtual Destructor */
	~GameDescription();
	
	/** Set number of players */
	void setPlayerNumber( int nb ); 
	
	/** Return the number of players */
	int getPlayerNumber() { return _nbPlayer; }
	
	/** Give a name 'name' for player 'num' */
	void setPlayerName( int num, QString name ) {
		_playersName[num] = name;
	}
	
	/** Return the name of the player 'num' */
	QString getPlayerName( int num ) {
		return _playersName[ num ];
	}
	
	/** Give a color 'color' for player 'num' */
	void setPlayerColor( int num, QColor color ) {
		_playersColor[num] = color;
	}
	
	/** Return the color of the player 'num' */
	QColor getPlayerColor( int num ) {
		return _playersColor[ num ];
	}
	
	/** Init lords for the game */
	void initLords();
	
	/** Init bases for the game */
	//void initBases();

	GenericLord * getLord( int id ) {
		return _realLords[id];
	}

	Artefact * getArtefactById( int id );
	
protected:
	Calendar * _calendar;
	QString * _playersName;
	QColor * _playersColor;
	int _nbPlayer;
	Lord ** _realLords;
	QList<Artefact> _artefacts;
};
 
#endif // GAMEDESCRIPTION_H   
