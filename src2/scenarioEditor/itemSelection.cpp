/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelection.cpp
** Allow selection of specific item panels
**
** Version : $Id: itemSelection.cpp,v 1.48 2008/06/12 13:16:15 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <algorithm>

// include files for QT
#include <QComboBox>
#include <QLayout>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QStackedWidget>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/genericCell.h"

#include "libClient/attalButton.h"
#include "libClient/bonus.h"
#include "libClient/building.h"
#include "libClient/chest.h"
#include "libClient/event.h"
#include "libClient/graphicalArtefact.h"
#include "libClient/graphicalGameData.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/lord.h"
#include "libClient/map.h"
#include "libClient/mapCreature.h"

#include "scenarioEditor/screen.h"

extern Map * theMap;

bool isOdd( int number )
{
	return( ( number % 2 ) == 1 );
}

//
// ----- ItemSelection -----
//

ItemSelection::ItemSelection( QWidget * parent, const char * /* name */ )
	:QWidget( parent )
{
	_bg = new QButtonGroup( this );
	//_bg->hide();
	_bg->setExclusive( true );
	setMinimumWidth( 220 );
	_currentX = 5;
	_currentY = 5;
	_currentHeight = 5;
}

void ItemSelection::addButton( QPixmap pixmap, int pos, bool breakLine )
{
	AttalButton * pb = new AttalButton( this );
	pb->setPixmap( pixmap );
	pb->setCheckable( true );
	_bg->addButton( pb, pos );
	pb->setFixedSize( QSize( 40, 40 ).expandedTo( pb->sizeHint() ) );
	addNewWidget( pb, breakLine );
}

void ItemSelection::addButton( const QString & text, int pos, bool breakLine )
{
	QPushButton * pb = new QPushButton( text, this );
	pb->setCheckable( true );
	_bg->addButton( pb, pos );
	pb->setFixedSize( QSize(40,40).expandedTo(pb->sizeHint()));
	addNewWidget( pb, breakLine );
}

void ItemSelection::addNewWidget( QWidget * widget, bool breakLine )
{
	if( ( _currentX + widget->width() ) > 210 ) {
		breakLine = true;
	}
	if( _currentX == 5 ) {
		breakLine = false;
	}
	if( breakLine ) {
		_currentY += _currentHeight + 5;
		_currentX = 5;
		_currentHeight = 0;
	}
	_currentHeight = std::max( _currentHeight, widget->height() );
	widget->move( _currentX, _currentY );
	_currentX += ( 5 + widget->width() );
	setMinimumHeight( _currentY + _currentHeight + 5 );
}

void ItemSelection::addNewLine()
{
	_currentX = 5;
	_currentY += _currentHeight + 5;
}

bool ItemSelection::isPlace(GenericCell * cell)
{
	//logDD("cell row %d, col %d, coeff %d , stop %d, free %d",cell->getRow(), cell->getCol(), cell->getCoeff(), cell->isStoppable() ,cell->isFree());
	if((! cell->getBuilding() ) && ( ! cell->getBase() ) && cell->getCoeff() >= 0  && cell->isStoppable() && cell->isFree()){
		return true;
	}
	return false;
}

int ItemSelection::button()
{
	/// XXX: replace selectedId with Id (selected)
	//return _bg->id ( _bg->selected() );
	return _bg->checkedId();
}


//
// ----- TransitionSelection -----
//

TransitionSelection::TransitionSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	addButton("Auto",0);
	for( int i = 0; i < DataTheme.tiles.count() - 1; i++ ) {
		for( uint j = 0; j < NB_TRANSITION ; j++ ) {
			addButton( ImageTheme.transition[i]->at( j ), ( i * NB_TRANSITION ) + j + 1, j == 0 );
		}
	}
}

void TransitionSelection::handleLeftClick( GenericCell * cell )
{
	if( button() == 0 ) {
		cell->setTransition( computeTransition( (GenericMap*)theMap, (GenericCell *)cell ) );
		cell->setTransitionCellType( computeTransitionCellType( (GenericMap*)theMap, (GenericCell *)cell ) );
	} else if( button() > -1 ) {
		cell->setTransition( ( ( button() - 1 )  % NB_TRANSITION ) + 1 );
		cell->setTransitionCellType( ( ( button() - 1 ) / NB_TRANSITION ) + 1 );
	}
}


//
// ----- DecorationGroupSelection -----
//

DecorationGroupSelection::DecorationGroupSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	DecorationSelection * item;
	uint nbGroups = DataTheme.decorations.count();
	
	_groups = new QComboBox( this );	
	_stack = new QStackedWidget( this );

	for( uint i = 1; i < nbGroups; i++ ) {
		_groups->addItem( DataTheme.decorations.at( i )->getName() );
		item = new DecorationSelection( i );
		_stack->addWidget( item );
	}
	FIXEDSIZE( _groups );
	slot_selectGroup( 0 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _groups );
	layout->addWidget( _stack, 1 );
	layout->activate();

	connect( _groups, SIGNAL( activated( int ) ), SLOT( slot_selectGroup( int ) ) );
}

void DecorationGroupSelection::handleLeftClick( GenericCell * cell )
{
	( (DecorationSelection*)_stack->currentWidget() )->handleLeftClick( cell );
}

void DecorationGroupSelection::slot_selectGroup( int group )
{
	_stack->setCurrentIndex( group );
	setFixedHeight( 10 + _groups->height() + _stack->currentWidget()->height() );
}

//
// ----- DecorationSelection -----
//

DecorationSelection::DecorationSelection( uint group, QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	DecorationGroup * deco = DataTheme.decorations.at( group );
	uint nbItems = deco->count();
	
	_group = group;
	
	for( uint i = 0; i < nbItems; i++ ) {
		addButton( ImageTheme.decoration[ group-1 ]->at( i ), i );
	}
	
	_isTree = false;
	_isRoad = false;
	if( deco->getEffectNumber() > 0 ) {
		for( uint i = 0 ; i < deco->getEffectNumber(); i++ ) {
			DecorationGroup::EffectType effect = deco->getEffectType( i );
			if( ( effect  == DecorationGroup::DECREASE_MOVECOST ) || ( effect == DecorationGroup::INCREASE_MOVECOST ) ) {
				_isRoad = true;
				addNewLine();
				addButton( tr( "Auto-path" ), nbItems );
			}
			if( ( effect  == DecorationGroup::NO_BLOCK_DECO ) ) {
				_isTree = true;
			}
		}
	}

	setFixedHeight( minimumHeight() );
}

void DecorationSelection::handleLeftClick( GenericCell * cell )
{
	
	if( button() != -1 ) {
 		if( isPlace(cell) || _isTree) {
			if( _isRoad && ( button() == ( _bg->buttons().count() - 1 ) ) ) {
				computeAndChangeRoadType( (GenericMap *)theMap, (GenericCell *)cell, _group ); 
			} else {
				GenericDecoration * deco 	= DataTheme.decorations.at( _group)->at(button()) ;
				if( theMap->isPlaceDecoFree( deco,(GenericCell *)cell) || _isTree) {
					cell->setDecoration( _group, button() );
					theMap->computeStoppable(deco, cell);
				}
			}
		}
	}
}

//
// ----- BuildingSelection -----
//

BuildingSelection::BuildingSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	uint nb = DataTheme.buildings.count();
	for ( uint i = 0; i < nb; i++ ) {
		addButton( ImageTheme.buildings[i]->at( 0 ), i );
	}
}

void BuildingSelection::handleLeftClick( GenericCell * cell )
{
	if( button() != -1 ) {
		if( isPlace(cell)) {
			if( theMap->isPlaceBuilFree( button(),(GenericCell *)cell) ) {
				Building * building = (Building *)_data->getNewBuilding( button() );
				_data->setBuildingId( building );
				building->setPosition( (GenericCell *)cell );
				theMap->computeStoppable(building);
			}
		}
	}
}

//
// ----- BaseSelection -----
//

BaseSelection::BaseSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	uint nb = ImageTheme.bases->count();
	for( uint i = 0; i < nb; i++ ) {
		addButton( ImageTheme.bases->at(i), i );
	}
}

void BaseSelection::handleLeftClick( GenericCell * cell )
{
	if( button() != -1 ) {
		if( isPlace(cell) ) {
			if( theMap->isPlaceBaseFree( button(), (GenericCell *)cell)) {
				Base * base = (Base *)_data->getNewBase();
				base->setRace( button() );
				base->setPosition( (GenericCell *)cell );
				_data->setBaseId( (GenericBase *)base );
				theMap->computeStoppable(base);
			}
		}
	}
}
//
// ----- LordSelection -----
//

LordSelection::LordSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	addButton( ImageTheme.lords->at( 0 ), 0 );
}

void LordSelection::handleLeftClick( GenericCell * cell )
{ 
	if( button() != -1 && isPlace(cell)) {
		if( ! cell->getLord() && cell->getCoeff() >= 0  && cell->isStoppable() && cell->isFree()) {
			for( uint num = 1; num < _data->getLordNumber(); num++ ) {
				Lord * lord = (Lord*)_data->getLord( num );
				if( !lord->getCell()) {
					lord->setId( num );
					lord->setVisible( true );
					lord->setCell( (GenericCell *)cell );
					return;
				}
			}
		}
	}

}

//
// ----- ArtefactSelection -----
//

ArtefactSelection::ArtefactSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	uint nb = ImageTheme.artefacts->count();
	for ( uint i = 0; i < nb; i++ ) {
		addButton( ImageTheme.artefacts->at(i), i );
	}
}

void ArtefactSelection::handleLeftClick( GenericCell * cell )
{
	if( button() != -1 && isPlace(cell)) {
		Event * event = (Event*)_data->getNewArtefact();
		Artefact * artefact = (Artefact*) event->getArtefact();
		artefact->setType( button() );
		if( cell->getEvent() ) {
			Event * temp = (Event*)( cell->getEvent() );
			delete temp;
		}
		cell->setEvent( (GenericEvent*)event );
		event->setCell( (GenericCell*)cell );
	}
}

//
// ----- BonusSelection -----
//

BonusSelection::BonusSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	int i;
	addButton( ImageTheme.chest->at( 0 ), 0, true );
	int nbRes = DataTheme.resources.count();
	for( i = 0; i < nbRes; i++ ) {
		addButton( ImageTheme.bonus[0]->at( i ), i+1, i == (nbRes -1) );
	}
	for( i = 0; i < 3; ++i ) {
		addButton( ImageTheme.bonus[1+i]->at( 0 ), nbRes+i+1 );
	}
}

void BonusSelection::handleLeftClick( GenericCell * cell )
{
	if( button() != -1  && isPlace(cell)) {
		Event * event;
		if( button() == 0 ) {
			event = (Event*) _data->getNewChest();
			///Chest * chest = (Chest *) event->getChest();
		} else {
			uint nbRes = DataTheme.resources.count();
			event = (Event*) _data->getNewBonus();
			Bonus * bonus = (Bonus *) event->getBonus();

			if( button() <= (int)nbRes ) {
				bonus->setType( GenericBonus::BonusResource );
				bonus->addParam( button() - 1 );
				bonus->addParam( 0 );
				bonus->addParam( 1 );
				bonus->setupBonus();
			} else {
				bonus->setType( (GenericBonus::BonusType) (button()-nbRes) );
				bonus->setupBonus();
			}
		}

		if( cell->getEvent() ) {
			Event * temp = (Event*)( cell->getEvent() );
			delete temp;
		}
		cell->setEvent( (GenericEvent*)event );
		event->setCell( (GenericCell*)cell );
	}
}

//
// ----- CreatureSelection -----
//

CreatureSelection::CreatureSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	int cpt = 0;
	for( uint i = 0; i < DataTheme.countRace(); i++ ) {
		for( int j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
			addButton(ImageTheme.getMapCreature( i, j ), cpt );
			cpt++;
		}
		addNewLine();
	}
}

void CreatureSelection::handleLeftClick( GenericCell * cell )
{
	if( button() != -1 && isPlace(cell)) {
		int race = 0, level = 0;
		int cpt = 0;

		for( uint i = 0; i < DataTheme.countRace(); i++ ) {
			for( int j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
				if( cpt == button() ) {
					race = i;
					level = j;
				}
				cpt++;
			}
		}

		MapCreature * creature = (MapCreature*) _data->getNewMapCreature(); //new MapCreature( theMap );
		creature->setCreature( race, level );
		creature->setStack( 0, 1 ); /// XXX: to change
		creature->setCell( cell );
		cell->setCreature( creature );
	}
}

//
// ----- ToolSelection -----
//

ToolSelection::ToolSelection( Screen * screen, QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	_screen = screen;
	addButton("Eraser",0);
	addButton("Auto-transition",1);
	addButton("Auto-roads",2);
}

void ToolSelection::handleLeftClick( GenericCell * cell )
{
	switch( button() ) {
	case 0:
		_screen->handleDelete();
		break;
	case 1:
		cell->setTransition( computeTransition( (GenericMap *)theMap, (GenericCell *)cell ) );
		cell->setTransitionCellType( computeTransitionCellType( (GenericMap *)theMap, (GenericCell *)cell ) );
		break;
	case 2: 
		computeAndChangeRoadType( (GenericMap *)theMap, (GenericCell *)cell, 1 ); /// XXX: to change '1' -> road type
		break;
	}
}

