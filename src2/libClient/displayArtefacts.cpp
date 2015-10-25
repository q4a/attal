/****************************************************************
**
** Attal : Lords of Doom
**
** displayArtefacts.cpp
** Displays the artefacts of a lord
**
** Version : $Id: displayArtefacts.cpp,v 1.9 2008/07/01 23:11:19 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/12/2003
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

#include "displayArtefacts.h"

// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/artefactManager.h"
#include "libCommon/dataTheme.h"

#include "libClient/graphicalArtefact.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/player.h"

extern QString IMAGE_PATH;

//
// ----- DisplayArtefacts -----
//

DisplayArtefacts::DisplayArtefacts( Player * player, QWidget * parent, const char * /* name */ )
	: QFrame( parent )
{
	_player = player;
	_index = 0;
	uint i;

	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(QPixmap(IMAGE_PATH + "misc/artefactLord.png")));
	setPalette(palette);

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->addStretch( 1 );

	QHBoxLayout * layH1 = new QHBoxLayout();

	QPushButton * pb1 = new QPushButton( this );
	pb1->setText( "<" );
	FIXEDSIZE( pb1 );
	layH1->addWidget( pb1 );

/// XXX : to remove cause use the old api for artefacts (no more used), leave it as example for a new
	/// function that match new api...
/*
	/// WIN: was "_used = new (IconArtefact *) [DataTheme.specifications.getArtefactNumber()];"
	_used = new IconArtefact * [DataTheme.specifications.getArtefactNumber()];
	SpecificationArtefact * temp;

	for( i = 0; i < DataTheme.specifications.getArtefactNumber(); i++ ) {
		temp = DataTheme.specifications.getArtefact( i );
		//_used[i] = new IconArtefact( this );
		//_used[i]->move( temp->getX(), temp->getY() );
	}
*/
	layH1->addStretch( 1 );

	for( i = 0; i < 3; i++ ) {
		_ico[i] = new IconArtefact( this );
		_ico[i]->clear();
		layH1->addWidget( _ico[i] );
	}

	layH1->addStretch( 1 );
	QPushButton * pb2 = new QPushButton( this );
	pb2->setText( ">" );
	FIXEDSIZE( pb2 );
	layH1->addWidget( pb2 );

	layout->addLayout( layH1 );

	layout->activate();

	connect( pb1, SIGNAL( clicked() ), SLOT( slot_prevArtefact() ) );
	connect( pb2, SIGNAL( clicked() ), SLOT( slot_nextArtefact() ) );
}

DisplayArtefacts::~DisplayArtefacts()
{
}

void DisplayArtefacts::reinit()
{
	GenericLord * lord = _player->getSelectedLord();

	if(lord){
		for( int i = 0; i < 3; i++ ) {
			if( ( _index + i ) < lord->getArtefactManager()->getArtefactNumber() ) {
				_ico[i]->setType( lord->getArtefactManager()->getArtefact( _index + i )->getType() );
			} else {
				_ico[i]->clear();
			}
		}
	}
}

void DisplayArtefacts::slot_nextArtefact()
{
	GenericLord * lord = _player->getSelectedLord();

	if(lord){
		if( _index + 3 < lord->getArtefactManager()->getArtefactNumber() ) {
			_index++;
			reinit();
		}
	}
}

void DisplayArtefacts::slot_prevArtefact()
{
	if( _index > 0 ) {
		_index--;
		reinit();
	}
}

//
// ----- DisplayLordTabArtefacts -----
//

DisplayLordTabArtefacts::DisplayLordTabArtefacts( GenericPlayer * player, QWidget * parent, const char * /* name */)
: QWidget( parent )
{
	_player = player;
	_index = 0;
	uint i;

	QPushButton * pb1 = new QPushButton( this );
	pb1->setText( "<" );
	FIXEDSIZE( pb1 );

	QPushButton * pb2 = new QPushButton( this );
	pb2->setText( ">" );
	FIXEDSIZE( pb2 );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pb1 );
	
	for( i = 0; i < 4; i++ ) {
		_ico[i] = new IconArtefact( this );
		_ico[i]->clear();
		layH1->addWidget( _ico[i] );
	}

	layH1->addWidget( pb2 );
	layH1->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addStretch( 1 );
	layout->activate();

	connect( pb1, SIGNAL( clicked() ), SLOT( slot_prevArtefact() ) );
	connect( pb2, SIGNAL( clicked() ), SLOT( slot_nextArtefact() ) );
}

void DisplayLordTabArtefacts::reinit()
{
	GenericLord * lord = _player->getSelectedLord();

	if( lord ) {
		for( int i = 0; i < 4; i++ ) {
			if( ( _index + i ) < lord->getArtefactManager()->getArtefactNumber() ) {
				_ico[ i ]->setType( lord->getArtefactManager()->getArtefact( _index + i )->getType() );
			} else {
				_ico[ i ]->clear();
			}
		}
	}
}

void DisplayLordTabArtefacts::slot_nextArtefact()
{
	GenericLord * lord = _player->getSelectedLord();

	if(lord){
		if( _index + 3 < lord->getArtefactManager()->getArtefactNumber() ) {
			_index++;
			reinit();
		}
	}
}

void DisplayLordTabArtefacts::slot_prevArtefact()
{
	if( _index > 0 ) {
		_index--;
		reinit();
	}
}

