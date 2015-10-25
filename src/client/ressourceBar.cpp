/****************************************************************
**
** Attal : Lords of Doom
**
** ressourceBar.cpp
** show ressources of the player (gold, ...)
**
** Version : $Id: ressourceBar.cpp,v 1.9 2002/05/13 21:43:53 audoux Exp $
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

#include "ressourceBar.h"

 
// generic include files
// include files for QT
#include <qlayout.h>
#include <qstring.h>
// application specific includes
#include "client/gui.h"
#include "common/dataTheme.h"

extern DataTheme DataTheme;

/** add comments here */
RessourceBar::RessourceBar( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_player = 0;
	
	QHBoxLayout * lay = new QHBoxLayout( this );
	
	lay->addStretch( 1 );

	for( int i = 0; i < MAX_RESS; i++ ) {
		_labRessource[i] = new QLabel( this );
		lay->addWidget( _labRessource[i] );
		lay->addStretch( 1 );
	}
	lay->activate();
}

void RessourceBar::setPlayer( GenericPlayer * p )
{
	_player = p;
	reinit();
}

void RessourceBar::reinit()
{
	if( _player != 0 ) {
		QString s;
		
		for( uint i = 0; i < DataTheme.ressources.count(); i++ ) {
			s.sprintf( "%s : %d", DataTheme.ressources.getRessource( i ).latin1(), _player->getResource( i ) );
			_labRessource[i]->setText( s );
			FIXEDSIZE( _labRessource[i] );
		}
	}
}

