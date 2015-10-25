/****************************************************************
**
** Attal : Lords of Doom
**
** graphicalArtefact.cpp
** Display artefacts on the map
**
** Version : $Id: graphicalArtefact.cpp,v 1.1 2002/01/19 23:52:41 audoux Exp $
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

#include "graphicalArtefact.h"
 
// generic include files
// include files for QT
// application specific include files
#include "conf.h"
#include "client/imageTheme.h"
#include "client/cell.h"
#include "common/genericCell.h"
#include "client/lord.h"

extern ImageTheme ImageTheme;

const int Artefact::RTTI = 1007;

Artefact::Artefact( QCanvas * canvas )
	:QCanvasSprite( ImageTheme.artefacts, canvas )
{
       collisions( true );
       setFrame( 0 );
       setZ( CAN_ARTEFACT );
       show();
}

void Artefact::setCell( GenericCell * cell )
{
	GenericArtefact::setCell( cell );
	QRect rect = ( (Cell*)cell )->boundingRect();
	move( rect.x() , rect.y() + rect.height() - boundingRect().height() );
	show();
        canvas()->update();
}

void Artefact::setLord( GenericLord * lord )
{
	GenericArtefact::setLord( lord );
	hide();
}

void Artefact::setType( int type )
{
	GenericArtefact::setType( type );
	setFrame( type );
}

