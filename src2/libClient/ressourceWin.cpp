/****************************************************************
**
** Attal : Lords of Doom
**
** ressourceWin.cpp
** show ressources of the player (gold, ...)
**
** Version : $Id: ressourceWin.cpp,v 1.22 2008/07/01 00:18:56 lusum Exp $
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

#include "ressourceWin.h"

 
// generic include files
// include files for QT
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QString>

// application specific includes
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"

/** add comments here */

TabRessourceWin::TabRessourceWin( QWidget * parent, WinResType type, GenericPlayer * player, GenericResourceList * list)
{
	_ressW = new RessourceWin( this, type, player, list );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _ressW );
	layout->addStretch( 2 );
	connect( parent, SIGNAL( sig_castle() ), _ressW , SLOT( reinit() ));
}

void TabRessourceWin::reinit()
{
	_ressW->reinit();
}

	RessourceWin::RessourceWin( QWidget * parent, WinResType type, GenericPlayer * player, GenericResourceList * list)
:QWidget( parent )
{
	TRACE("RessourceWin  QWidget * parent %p, Player * player %p, GenericResourceList * list %p )", parent, player, list );

	_player = player;
	_list = list;
	int k = 1;

	QGridLayout * lay = new QGridLayout( this );

	lay->setRowStretch(0,1);
	lay->setColumnStretch( 0, 0 );
	lay->setColumnStretch( 1, 1 );
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		_labIcon[i] = new QLabel( this );
		_labRessource[i] = new QLabel( this );
		if(type == RS_GRID) {
			if((i%2) == 0){
				k=i/2+1;
				lay->addWidget( _labIcon[i] , k, 0);
				lay->addWidget( _labRessource[i] , k, 1);
			} else {
				lay->addWidget( _labIcon[i] , k, 2);
				lay->addWidget( _labRessource[i] , k, 3);
			}

		} else if (type == RS_VERT) {
			lay->addWidget( _labIcon[i] , i, 0, Qt::AlignLeft);
			lay->addWidget( _labRessource[i] , i, 1, Qt::AlignLeft);
			k = i;
		}
	}
	
	if( _list ) {
		int m = 1;
		for( int i = 0; i < DataTheme.resources.count(); i++ ) {
			_labIconl[i] = new QLabel( this );
			_labRessourcel[i] = new QLabel( this );
			if(type == RS_GRID) {
				if((i%2) == 0){
					m=i/2+1 + k;
					lay->addWidget( _labIconl[i] , m, 0);
					lay->addWidget( _labRessourcel[i] , m, 1);
				} else {
					lay->addWidget( _labIconl[i] , m, 2);
					lay->addWidget( _labRessourcel[i] , m, 3);
				}
			} else {
				m = i + k;
				lay->addWidget( _labIconl[i] , m , 0, Qt::AlignLeft);
				lay->addWidget( _labRessourcel[i] , m, 1, Qt::AlignLeft);
			}
		}
	}

	if (type == RS_VERT) {
		setMaximumWidth( 100 );
	}

	reinit();
	lay->activate();
}

void RessourceWin::reinit()
{
  TRACE("void RessourceWin::reinit");

	if( _player != 0 ) {
		QString s;
		int value;

		for( int i = 0; i < DataTheme.resources.count(); i++ ) {
			//s.sprintf( "%s : %d", DataTheme.resources.getRessource( i ).toLatin1(), _player->getResource( i ) );
			value = _player->getResourceList()->getValue( i );
			s.sprintf( " %d",  value );
			QPixmap pix = ImageTheme.getResourceSmallIcon( i )->copy(ImageTheme.getResourceSmallIcon( i )->rect());
			_labIcon[i]->setPixmap( pix  );
			_labIcon[i]->setToolTip( DataTheme.resources.getRessource( i ).toLatin1() );
			_labRessource[i]->setText( s );
			bool global = DataTheme.resources.get( i )->isGlobal();
			_labIcon[i]->setVisible( global );
			_labRessource[i]->setVisible( global );
			FIXEDSIZE( _labRessource[i] );
			if( _list ) {
				value = _list->getValue( i );
				s.sprintf( " %d",  value );
				QPixmap pix = ImageTheme.getResourceSmallIcon( i )->copy(ImageTheme.getResourceSmallIcon( i )->rect());
				_labIconl[i]->setPixmap( pix  );
				_labIconl[i]->setToolTip( DataTheme.resources.getRessource( i ).toLatin1() );
				_labRessourcel[i]->setText( s );
				_labIconl[i]->setVisible( !global );
				_labRessourcel[i]->setVisible( !global );
				FIXEDSIZE( _labRessourcel[i] );
			}

		}

	}
}

