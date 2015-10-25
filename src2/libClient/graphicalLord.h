/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalLord.h
** draw a hero on the map
**
** Version : $Id: graphicalLord.h,v 1.10 2008/04/07 21:06:10 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 09/08/2000
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

#ifndef GRAPHICALLORD_H
#define GRAPHICALLORD_H
 
// generic include files
// include files for QT
#include <QColor>
#include <QImage>
// application specific includes
#include "libClient/attalSprite.h"
#include "libCommon/genericCell.h"
#include "libClient/flag.h"

class GenericPlayer;

/*              ------------------------------
 *                        GraphicalLord
 *              ------------------------------ */

/** build the graphical part of the Lord */
class GraphicalLord : public AttalSprite
{
public:
	/** Constructor */
	GraphicalLord( QGraphicsScene * canvas );

	virtual ~GraphicalLord();
	
	enum { 
		Type = QGraphicsItem::UserType + 1234
	};

	/** Set the Lord on the cell */
	virtual void setCell( GenericCell *cell );
	
	virtual void setVisible( bool state );
	
	/** Advance animation */
	void advance( int stage );

	virtual void highlight( bool state);
	
	virtual void setOwner( GenericPlayer * player );
	
	int type() const;
	
	static const int RTTI;
	
protected:
	QPixmap imageAdvanced();
	Flag * _flag;
	QImage _imabk;
};

#endif // GRAPHICALLORD_H

