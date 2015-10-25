/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelection.cpp
** Allow selection of specific item panels
**
** Version : $Id: itemSelection.cpp,v 1.17 2002/12/20 20:29:11 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 22/12/2001
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

#include "itemSelection.h"

// generic include files
// include files for QT
#include <qpushbutton.h>
#include <qsignalmapper.h>
// application specific include files
#include "client/building.h"
#include "client/cell.h"
#include "client/graphicalArtefact.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/lord.h"
#include "client/map.h"
#include "client/mapCreature.h"

#include "common/dataTheme.h"

#include "server/gameData.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;
extern Map * theMap;

//
// ----- ItemSelection -----
//

ItemSelection::ItemSelection( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_button = -1;
	_buttonList.setAutoDelete( false );
	setMinimumWidth( 220 );
}


void ItemSelection::slot_button( int num )
{
	if( _button == -1 ) {
		_button = num;
	} else if( _button != num ) {
		_buttonList.at( _button )->setOn( false );
		_button = num;
	} else {
		_button = -1;
	}
}
//
// ----- TileSelection -----
//

TileSelection::TileSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < ImageTheme.cells->count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( *ImageTheme.cells->image( i ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 + ((i%5)*40), 5 + ((int)(i/5)*40) );
		_buttonList.append( pb );
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );
	
	setMinimumHeight( 10 + ( ( ( ImageTheme.cells->count() / 5 ) + 1 ) * 40 ) );
}


void TileSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		cell->setType( _button );
	}
}

//
// ----- TransitionSelection -----
//

TransitionSelection::TransitionSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	QPushButton * pbAuto = new QPushButton( this );
	pbAuto->setText( "Auto" );
	pbAuto->setToggleButton( true );
	FIXEDSIZE( pbAuto );
	pbAuto->move( 5, 5 );
	_buttonList.append( pbAuto );
	sigmap->setMapping( pbAuto, 0 );
	connect( pbAuto, SIGNAL( clicked() ), sigmap, SLOT( map() ) );

	for( uint i = 0; i < DataTheme.tiles.count() - 1; i++ ) {
		for( uint j = 0; j < NB_TRANSITION; j++ ) {
			QPushButton * pb = new QPushButton( this );
			pb->setPixmap( *ImageTheme.transition[i]->image( j ) );
			pb->setToggleButton( true );
			FIXEDSIZE( pb );
			sigmap->setMapping( pb, ( i * NB_TRANSITION ) + j + 1 );
			connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
			pb->move( 5 + ( ( ( ( i * NB_TRANSITION ) + j )%5 )*40), 45 + ((int)(( ( i * NB_TRANSITION ) + j )/5)*40) );
			_buttonList.append( pb );
		}
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );

	setMinimumHeight( 50 + ( ( ( (( DataTheme.tiles.count() - 1 ) * NB_TRANSITION ) / 5 ) + 1 ) * 40 ) );
}

void TransitionSelection::handleLeftClick( Cell * cell )
{
	if( _button == 0 ) {
		cell->setTransition( computeTransition( (GenericMap*)theMap, (GenericCell *)cell ) );
		cell->setTransitionCellType( computeTransitionCellType( (GenericMap*)theMap, (GenericCell *)cell ) );
	} else if( _button > -1 ) {
		cell->setTransition( ( ( _button - 1 ) % NB_TRANSITION ) + 1 );
		cell->setTransitionCellType( ( ( _button - 1 ) / NB_TRANSITION ) + 1 );
	}
}


//
// ----- DecorationSelection -----
//

DecorationSelection::DecorationSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < ImageTheme.decoration->count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( *ImageTheme.decoration->image( i ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 + ((i%5)*40), 5 + ((int)(i/5)*40) );
		_buttonList.append( pb );
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );
	setMinimumHeight( 10 + ( ( ( ImageTheme.decoration->count() / 5 ) + 1 ) * 40 ) );
}

void DecorationSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		cell->setDecoration( _button + 1 );
	}
}

//
// ----- BuildingSelection -----
//

BuildingSelection::BuildingSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < DataTheme.buildings.count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( * ImageTheme.buildings[i]->image( 0 ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 + ((i%5)*40), 5 + ((int)(i/5)*70) );
		_buttonList.append( pb );
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );
	setMinimumHeight( 10 + ( ( ( DataTheme.buildings.count() / 5 ) + 1 ) * 70 ) );
}

void BuildingSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		if( ( ! cell->getBuilding() )  && ( ! cell->getBase() ) ) {
			Building * building = new Building( theMap );
			building->setType( _button );
			building->setPosition( (GenericCell *)cell );
		}
	}
}

//
// ----- BaseSelection -----
//

BaseSelection::BaseSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < ImageTheme.bases->count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( *ImageTheme.bases->image( i ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 , 5 + ((int)(i)*160) );
		_buttonList.append( pb );
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );
	
	setMinimumHeight( 10 + ( ( ( ImageTheme.bases->count() ) ) * 160 ) );
}

void BaseSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		if( ( ! cell->getBuilding() ) && ( ! cell->getBase() ) ) {
			Base * base = (Base *)_data->getNewBase();
			base->setRace( _button );
			base->setPosition( (GenericCell *)cell );
		}
	}
}
//
// ----- LordSelection -----
//

LordSelection::LordSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	_num = 0;
	QSignalMapper * sigmap = new QSignalMapper( this );

	/*for( uint i = 0; i < ImageTheme.decoration->count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( *ImageTheme.decoration->image( i ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 + ((i%5)*40), 5 + ((int)(i/5)*40) );
		_buttonList.append( pb );
	}*/
	QPushButton * pb = new QPushButton( this );
	pb->setPixmap( *ImageTheme.lords->image( 0 ) );
	pb->setToggleButton( true );
	FIXEDSIZE( pb );
	sigmap->setMapping( pb, 0 );
	connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
	pb->move( 5, 5 );
	_buttonList.append( pb );
	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );

	setMinimumHeight( 15 + ( ( ( ImageTheme.lords->count() / 5 ) + 1 ) * 70 ) );
}

void LordSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		if( ! cell->getLord() ) {
			_num++;
			Lord * lord = (Lord*)_data->getLord( _num );
			lord->setVisible( true );
			lord->setPosition( (GenericCell *)cell );
		}
	}
}

//
// ----- ArtefactSelection -----
//

ArtefactSelection::ArtefactSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	for( uint i = 0; i < ImageTheme.artefacts->count(); i++ ) {
		QPushButton * pb = new QPushButton( this );
		pb->setPixmap( *ImageTheme.artefacts->image( i ) );
		pb->setToggleButton( true );
		FIXEDSIZE( pb );
		sigmap->setMapping( pb, i );
		connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
		pb->move( 5 + ((i%5)*40), 5 + ((int)(i/5)*40) );
		_buttonList.append( pb );
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );

	setMinimumHeight( 10 + ( ( ( ImageTheme.artefacts->count() / 5 ) + 1 ) * 40 ) );
}

void ArtefactSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		Artefact * artefact = (Artefact*)_data->getNewArtefact();
		artefact->setType( _button );
		if( cell->getArtefact() ) {
			Artefact * art = (Artefact*)( cell->getArtefact() );
			delete art;
		}
		cell->setArtefact( artefact );
		artefact->setCell( (GenericCell*)cell );
	}
}

//
// ----- CreatureSelection -----
//

CreatureSelection::CreatureSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	QSignalMapper * sigmap = new QSignalMapper( this );

	int cpt = 0;

	for( uint i = 0; i < DataTheme.countRace(); i++ ) {
		for( uint j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
			QPushButton * pb = new QPushButton( this );
			pb->setPixmap( *ImageTheme.mapCreatures[i]->image( j ) );
			pb->setToggleButton( true );
			FIXEDSIZE( pb );
			sigmap->setMapping( pb, cpt );
			connect( pb, SIGNAL( clicked() ), sigmap, SLOT( map() ) );
			pb->move( 5 + ((j%5)*40), 5 + (i*140) + ((j/5)*70) );
			_buttonList.append( pb );
			cpt++;
		}
	}

	connect( sigmap, SIGNAL( mapped( int ) ), SLOT( slot_button( int ) ) );

	setMinimumHeight( 10 + (DataTheme.countRace()*140) );
}

void CreatureSelection::handleLeftClick( Cell * cell )
{
	if( _button != -1 ) {
		int race, level;
		int cpt = 0;

		for( uint i = 0; i < DataTheme.countRace(); i++ ) {
			for( uint j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
				if( cpt == _button ) {
					race = i;
					level = j;
				}
				cpt++;
			}
		}

		MapCreature * creature = new MapCreature( theMap );
		creature->setCreature( race, level );
		creature->setCategoryNumber( 10 ); /// XXX: to change
		creature->setCell( cell );
		cell->setCreature( creature );
	}
}






