/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.h
** draw bases and buildings on the map
**
** Version : $Id: graphicalBuilding.h,v 1.6 2008/06/12 14:12:49 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2000
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

#ifndef GRAPHICALBUILDING_H
#define GRAPHICALBUILDING_H
 
// generic include files
// include files for QT
#include <QGraphicsScene>

// application specific includes
#include "libClient/attalSprite.h"
#include "libCommon/genericCell.h"
#include "libClient/flag.h"

class GenericPlayer;

/** Build the graphical part of the Lord */
class GraphicalBuilding : public AttalSprite
{
public:
	/** Constructor */
	GraphicalBuilding( QGraphicsScene * canvas );
	
	GraphicalBuilding( QList<QPixmap> * array, QGraphicsScene * canvas );
	
	virtual ~GraphicalBuilding();
	
	enum { 
		Type = QGraphicsItem::UserType + 1000
	};

	/** Set the Lord on the cell */
	virtual void setPosition( GenericCell *cell, int offsetRow=0, int offsetCol=0 );
	
	/** Advance animation */
	void advance( int stage );
	
	int type() const;
	
	static const int RTTI;

	void setImageSequence( int type );

	void setAnimation( int nbFrame, int freq );
	
	virtual void setOwner( GenericPlayer * player );

protected:
	QPixmap imageAdvanced();
	
	Flag * _flag;
	int _frame, _nbFrame, _freq, _clock;

};

/** Graphical aspect of the base on the map */
class GraphicalBase : public GraphicalBuilding
{
public:
	/** Constructor */
	GraphicalBase( QGraphicsScene * canvas );

	/** Sets the race of the base */
	void setRace( int );
};

#endif // GRAPHICALBUILDING_H
