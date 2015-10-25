/****************************************************************
**
** Attal : Lords of Doom
**
** ressourceBar.cpp
** show ressources of the player (gold, ...)
**
** Version : $Id: ressourceBar.cpp,v 1.15 2008/04/06 14:18:04 lusum Exp $
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
#include <QLabel>
#include <QLayout>
#include <QString>

// application specific includes
#include "libCommon/dataTheme.h"

#include "libClient/imageTheme.h"
#include "libClient/gui.h"

RessourceBar::RessourceBar( QWidget * parent, const char * /* name */)
	:QWidget( parent )
{
	_player = 0;
	
	QHBoxLayout * lay = new QHBoxLayout( this );
	
	lay->addStretch( 1 );

	//_labPopul = new QLabel( this );
	//_totPopul = new QLabel( this );
	//lay->addWidget( _labPopul );
	//lay->addWidget( _totPopul );
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
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
	
		//QPixmap pix = ImageTheme.getPopulationIcon()->copy(ImageTheme.getPopulationIcon()->rect());
		//_labPopul->setPixmap( pix );	
		//_labPopul->setToolTip( "Population" );	
		//s.sprintf( " %d  ", _player->getPopulation() );
		//_totPopul->setText( s );
		//FIXEDSIZE( _totPopul );

		int count = DataTheme.resources.count();

		GenericResourceList * rlist = _player->getResourceList();

		for( int i = 0; i <  count ; i++ ) {
			int value = rlist->getValue( i );
			s.sprintf( "%s : %d", qPrintable ( DataTheme.resources.getRessource( i ) ), value );
			_labRessource[i]->setText( s );
			if( value  > 0 ) {
				_labRessource[i]->show();
			} else {
				_labRessource[i]->hide();
			}
			FIXEDSIZE( _labRessource[i] );
		}
	}
}

