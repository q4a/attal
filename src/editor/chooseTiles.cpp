/****************************************************************
**
** Attal : Lords of Doom
**
** chooseTiles.cpp
** Widget for choosing tiles (for the edition)
**
** Version : $Id: chooseTiles.cpp,v 1.5 2000/12/12 13:32:02 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/08/2000
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

#include "chooseTiles.h"

 
// generic include files
// include files for QT
#include <qcanvas.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qlabel.h>
// application specific includes
#include "conf.h"
#include "common/genericCell.h"

extern QCanvasPixmapArray * CellPixmapArray;
extern QCanvasPixmapArray * BuildingPixmapArray;

/** add comments here */
ChooseTiles::ChooseTiles( QWidget * parent )
	:QTabWidget( parent ),
	 _tile( -1 )
{
	_panelGround = new QButtonGroup( this );
	
	insertTab( _panelGround, "Ground" );

	QPushButton * b1 = new QPushButton( _panelGround );
	b1->setPixmap( *(CellPixmapArray->image( GenericCell::NORMAL )) );
	b1->setFixedSize( 35, 35 );	
	b1->setToggleButton( true );
	b1->move( 5, 5 );
	_panelGround->insert( b1, GenericCell::NORMAL );
	
	QPushButton * b2 = new QPushButton( _panelGround );
	b2->setPixmap( *(CellPixmapArray->image( GenericCell::WATER )) );
	b2->setToggleButton( true );	
	b2->setFixedSize( 35, 35 );
	b2->move( 45, 5 );
	_panelGround->insert( b2, GenericCell::WATER );
	
	QPushButton * b3 = new QPushButton( _panelGround );
	b3->setPixmap( *(CellPixmapArray->image( GenericCell::BLOCK )) );
	b3->setToggleButton( true );	
	b3->setFixedSize( 35, 35 );
	b3->move( 85, 5 );
	_panelGround->insert( b3, GenericCell::BLOCK );	
	
	_panelBuilding = new QButtonGroup( this );
	
	insertTab( _panelBuilding, "Building" );

	QPushButton * b11 = new QPushButton( _panelBuilding );
	b11->setPixmap( *(BuildingPixmapArray->image(0)) );
	b11->setFixedSize( 35, 35 );	
	b11->setToggleButton( true );
	b11->move( 5, 5 );
	_panelBuilding->insert( b11, 0 );
		
	
	setFixedWidth( 205 );
	
	_panelGround->setExclusive( true );
	_panelBuilding->setExclusive( true );
	
	connect( _panelGround, SIGNAL( clicked( int ) ), SLOT( slot_tileSelected( int ) ) );
	connect( _panelBuilding, SIGNAL( clicked( int ) ), SLOT( slot_tileSelected( int ) ) );
			
}

void ChooseTiles::slot_tileSelected( int num )
{

	_tile = num;
}

