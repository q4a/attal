/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelector.cpp
** Allow selection of item panels
**
** Version : $Id: itemSelector.cpp,v 1.7 2002/11/14 21:17:01 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 21/12/2001
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

#include "itemSelector.h"
 
// generic include files
// include files for QT
#include <qcombobox.h>
#include <qwidgetstack.h>
#include <qlayout.h>
// application specific include files
#include "scenarioEditor/itemSelection.h"
#include "client/gui.h"
#include "client/cell.h"

#include "server/gameData.h"


ItemSelector::ItemSelector( GameData * data, QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_data = data;

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_selector = new QComboBox( this );
	_selector->setFixedHeight( _selector->sizeHint().height() );
	layout->addWidget( _selector );

	_stack = new QWidgetStack( this );
	layout->addWidget( _stack, 1 );

	layout->activate();
	
	init();
	
	connect( _selector, SIGNAL( activated( int ) ), _stack, SLOT( raiseWidget( int ) ) );
	setFixedWidth( 235 );
}

void ItemSelector::init()
{
	_selector->insertItem( "Tile", 0 );
	_selector->insertItem( "Transition", 1 );
	_selector->insertItem( "Decoration", 2 );
	_selector->insertItem( "Building", 3 );
	_selector->insertItem( "Base", 4 );
	_selector->insertItem( "Lord", 5 );
	_selector->insertItem( "Artefact", 6 );
	_selector->insertItem( "Creature", 7 );
	
	ItemScroll * view0 = new ItemScroll( this );
	TileSelection * wid0 = new TileSelection( view0->viewport() );
	view0->addItem( wid0 );
	_stack->addWidget( view0, 0 );
	wid0->setGameData( _data );

	ItemScroll * view1 = new ItemScroll( this );
	TransitionSelection * wid1 = new TransitionSelection( view1->viewport() );
	view1->addItem( wid1 );
	_stack->addWidget( view1, 1 );
	wid1->setGameData( _data );

	ItemScroll * view2 = new ItemScroll( this );
	DecorationSelection * wid2 = new DecorationSelection( view2->viewport() );
	view2->addItem( wid2 );
	_stack->addWidget( view2, 2 );
	wid2->setGameData( _data );

	ItemScroll * view3 = new ItemScroll( this );
	BuildingSelection * wid3 = new BuildingSelection( view3->viewport() );
	view3->addItem( wid3 );
	_stack->addWidget( view3, 3 );
	wid3->setGameData( _data );

	ItemScroll * view4 = new ItemScroll( this );
	BaseSelection * wid4 = new BaseSelection( view4->viewport() );
	view4->addItem( wid4 );
	_stack->addWidget( view4, 4 );
	wid4->setGameData( _data );

	ItemScroll * view5 = new ItemScroll( this );
	LordSelection * wid5 = new LordSelection( view5->viewport() );
	view5->addItem( wid5 );
	_stack->addWidget( view5, 5 );
	wid5->setGameData( _data );

	ItemScroll * view6 = new ItemScroll( this );
	ArtefactSelection * wid6 = new ArtefactSelection( view6->viewport() );
	view6->addItem( wid6 );
	_stack->addWidget( view6, 6 );
	wid6->setGameData( _data );

	ItemScroll * view7 = new ItemScroll( this );
	CreatureSelection * wid7 = new CreatureSelection( view7->viewport() );
	view7->addItem( wid7 );
	_stack->addWidget( view7, 7 );
	wid7->setGameData( _data );

	_stack->raiseWidget( 0 );
}

void ItemSelector::handleLeftClick( Cell * cell )
{
	( (ItemScroll *)_stack->visibleWidget() )->handleLeftClick( cell );
}

//
// ----- ItemScroll -----
//

ItemScroll::ItemScroll( QWidget * parent )
	:QScrollView( parent )
{
	_item = 0;
	setBackgroundColor( lightGray );
	setVScrollBarMode( QScrollView::AlwaysOn );
	setHScrollBarMode( QScrollView::AlwaysOff );	
}

void ItemScroll::addItem( ItemSelection * item )
{
	_item = item;
	addChild( item );
}

void ItemScroll::handleLeftClick( Cell * c )
{
	if( _item ) {
		_item->handleLeftClick( c );
	}
}

