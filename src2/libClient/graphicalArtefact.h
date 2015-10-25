/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalArtefact.h
** Display artefacts on the map
**
** Version : $Id: graphicalArtefact.h,v 1.4 2006/10/02 17:51:39 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/01/2002
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

 
#ifndef GRAPHICALARTEFACT_H
#define GRAPHICALARTEFACT_H
 
 
// generic include files
// include files for QT
// application specific include files
#include "libCommon/artefact.h"
#include "libClient/attalSprite.h"
 
class GenericCell;
class GenericLord;

/*              ------------------------------
 *                         Artefact
 *              ------------------------------ */



/** comment for the class */
class Artefact : public AttalSprite, public GenericArtefact
{
 
public:
	/** Constructor */
	Artefact( QGraphicsScene * canvas );
	
	enum { 
		Type = QGraphicsItem::UserType + 1007
	};

	void setCell( GenericCell * cell );

	void setLord( GenericLord * lord );
	
	virtual void setType( uint type );

	static const int RTTI;

	int type() const;
};
 
#endif // GRAPHICALARTEFACT_H   
