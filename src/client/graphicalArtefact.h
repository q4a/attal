/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalArtefact.h
** Display artefacts on the map
**
** Version : $Id: graphicalArtefact.h,v 1.1 2002/01/19 23:52:41 audoux Exp $
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
#include <qcanvas.h>
// application specific include files
#include "common/artefact.h"
 
class GenericCell;
class GenericLord;

/*              ------------------------------
 *                         Artefact
 *              ------------------------------ */



/** comment for the class */
class Artefact : public QCanvasSprite, public GenericArtefact
{
 
public:
	/** Constructor */
	Artefact( QCanvas * canvas );

	void setCell( GenericCell * cell );

	void setLord( GenericLord * lord );
	
	void setType( int type );

        static const int RTTI;
	/** Return RTTI */
	int rtti() const { return RTTI; };
};
 
#endif // GRAPHICALARTEFACT_H   
