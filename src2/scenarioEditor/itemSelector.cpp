/****************************************************************
**
** Attal : Lords of Doom
**
** itemSelector.cpp
** Allow selection of item panels
**
** Version : $Id: itemSelector.cpp,v 1.16 2007/05/08 15:33:57 lusum Exp $
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
#include <QComboBox>
#include <QLayout>
#include <QStackedWidget>
#include <QVBoxLayout>

// application specific include files
#include "scenarioEditor/itemSelection.h"
#include "scenarioEditor/screen.h"
#include "scenarioEditor/tileSelection.h"
#include "libClient/map.h"
#include "libClient/graphicalGameData.h"
#include "libClient/gui.h"
#include "libClient/cell.h"

extern Map * theMap;

ItemSelector::ItemSelector( GraphicalGameData * data, Screen * screen, const char * /* name */)
	:QWidget( screen )
{
	_data = data;
	_screen = screen;

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_selector = new QComboBox( this );
	_selector->setFixedHeight( _selector->sizeHint().height() );
	layout->addWidget( _selector );

	_stack = new QStackedWidget( this );
	layout->addWidget( _stack, 1 );

	layout->activate();
	
	init();
	
	connect( _selector, SIGNAL( activated( int ) ), _stack, SLOT( setCurrentIndex( int ) ) );
	setFixedWidth( 235 );
}

void ItemSelector::init()
{
	_selector->addItem( QObject::tr( "Tile" ), ItemTile );
	_selector->addItem( QObject::tr( "Transition" ), ItemTransition );
	_selector->addItem( QObject::tr( "Decoration" ), ItemDecoration );
	_selector->addItem( QObject::tr( "Building" ), ItemBuilding );
	_selector->addItem( QObject::tr( "Base" ), ItemBase );
	_selector->addItem( QObject::tr( "Lord" ), ItemLord );
	_selector->addItem( QObject::tr( "Artefact" ), ItemArtefact );
	_selector->addItem( QObject::tr( "Bonus" ), ItemBonus );
	_selector->addItem( QObject::tr( "Creature" ), ItemCreature );
	_selector->addItem( QObject::tr( "Tool" ), ItemTool );
	
	ItemScroll * view0 = new ItemScroll();
	TileSelection * wid0 = new TileSelection( view0 );
	view0->addItem( wid0 );
	view0->setWidgetResizable(true);
	_stack->addWidget( view0 );
	wid0->setGameData( _data );

	ItemScroll * view1 = new ItemScroll();
	TransitionSelection * wid1 = new TransitionSelection( view1 );
	view1->addItem( wid1 );
	view1->setWidgetResizable(true);
	_stack->addWidget( view1 );
	wid1->setGameData( _data );

	ItemScroll * view2 = new ItemScroll();
	DecorationGroupSelection * wid2 = new DecorationGroupSelection( view2 );
	view2->addItem( wid2 );
	view2->setWidgetResizable(true);
	_stack->addWidget( view2 );
	wid2->setGameData( _data );

	ItemScroll * view3 = new ItemScroll();
	BuildingSelection * wid3 = new BuildingSelection( view3 );
	view3->addItem( wid3 );
	view3->setWidgetResizable(true);
	_stack->addWidget( view3 );
	wid3->setGameData( _data );

	ItemScroll * view4 = new ItemScroll();
	BaseSelection * wid4 = new BaseSelection( view4 );
	view4->addItem( wid4 );
	view4->setWidgetResizable(true);
	_stack->addWidget( view4 );
	wid4->setGameData( _data );

	ItemScroll * view5 = new ItemScroll();
	LordSelection * wid5 = new LordSelection( view5 );
	view5->addItem( wid5 );
	view5->setWidgetResizable(true);
	_stack->addWidget( view5 );
	wid5->setGameData( _data );

	ItemScroll * view6 = new ItemScroll();
	ArtefactSelection * wid6 = new ArtefactSelection( view6 );
	view6->addItem( wid6 );
	view6->setWidgetResizable(true);
	_stack->addWidget( view6 );
	wid6->setGameData( _data );

	ItemScroll * view7 = new ItemScroll();
	BonusSelection * wid7 = new BonusSelection( view7 );
	view7->addItem( wid7 );
	view7->setWidgetResizable(true);
	_stack->addWidget( view7 );
	wid7->setGameData( _data );

	ItemScroll * view8 = new ItemScroll();
	CreatureSelection * wid8 = new CreatureSelection( view8 );
	view8->addItem( wid8 );
	view8->setWidgetResizable(true);
	_stack->addWidget( view8 );
	wid8->setGameData( _data );

	ItemScroll * view9 = new ItemScroll();
	ToolSelection * wid9 = new ToolSelection( _screen, view9 );
	view9->addItem( wid9 );
	view9->setWidgetResizable(true);
	_stack->addWidget( view9 );
	wid9->setGameData( _data );

	_stack->setCurrentIndex( 0 );
}

void ItemSelector::handleLeftClick( GenericCell * cell )
{
	( (ItemScroll *)_stack->currentWidget() )->handleLeftClick( cell );
	theMap->computeStoppable();
}

//
// ----- ItemScroll -----
//

ItemScroll::ItemScroll( QWidget * parent )
	:QScrollArea( parent )
{
	_item = 0;
	QPalette palette;
	palette.setColor(backgroundRole(), Qt::lightGray );
	setPalette(palette);
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );	
}

void ItemScroll::addItem( ItemSelection * item )
{
	_item = item;
	setWidget( item );
}

void ItemScroll::handleLeftClick( GenericCell * c )
{
	if( _item ) {
		_item->handleLeftClick( c );
	}
}

