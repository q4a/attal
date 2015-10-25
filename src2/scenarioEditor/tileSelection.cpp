/****************************************************************
**
** Attal : Lords of Doom
**
** tileSelection.cpp
** Manages selection of tiles
**
** Version : $Id: tileSelection.cpp,v 1.15 2008/09/01 22:55:32 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/01/2004
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

#include "tileSelection.h"

// generic include files
#include <algorithm>

// include files for QT
#include <QString>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/cell.h"
#include "libClient/graphicalGameData.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/map.h"

extern Map * theMap;

//
// ----- TileSelection -----
//

TileSelection::TileSelection( QWidget * parent, const char * name )
	:ItemSelection( parent, name )
{
	TilePresentation * pres;
	_tiles = new QList<TilePresentation *>;
	
	_sizeG = new QButtonGroup( this );
	_sizeG->setExclusive( true );

	for( int i = 1; i < 4; i++ ) {
		QPushButton * pb = new QPushButton( QString("x%1").arg(i), this );
		pb->setCheckable( true );
		pb->setFixedWidth( 50 );
		_sizeG->addButton( pb, i );
		addNewWidget( pb );
	}

	_eff = new QButtonGroup( this );
	_eff->setExclusive( true );
	QPushButton * pb = new QPushButton( QString("Auto-transition"), this );
	pb->setCheckable( true );
	pb->setFixedSize( QSize( 40, 40 ).expandedTo( pb->sizeHint() ) );
	_eff->addButton( pb,1 );
	addNewWidget( pb );
	addNewLine();

	//_sizeG->setId(1);
	for( int i = 1; i < DataTheme.tiles.count(); i++ ) {
		//addButton( ImageTheme.cells[i]->image( 0 ), i, i == 0 );
		pres = new TilePresentation( this );
		pres->setCell( DataTheme.tiles.at(i) );
		pres->getButton()->setCheckable( true );
		_bg->addButton( pres->getButton(), i );
		pres->getButton()->setFixedSize( QSize(40,40).expandedTo(pres->getButton()->sizeHint() ) );
		addNewWidget( pres );
		_tiles->append( pres );
	}
}


void TileSelection::handleLeftClick( GenericCell * cell )
{
	uint row = cell->getRow();
	uint col = cell->getCol();
	if( button() != -1 ) {
		/// XXX: replace selectedId () with id( buttonGroup->selected() )
		uint shift = ( std::max(_sizeG->id ( _sizeG->checkedButton()) ,1) - 1 ) / 2;
		tileSquare( row - shift, col - shift );
	}
}

void TileSelection::tileSquare( uint row, uint col )
{
	int i, j;
	GenericCell * cell;

	int id = _sizeG->id ( _sizeG->checkedButton() );
	for( i = 0; i < id ; i++ ) {
		for( j = 0; j < id; j++ ) {
			cell = _data->getCell( row+i, col+j );
			if( cell ) {
				int row = cell->getRow();
				int col = cell->getCol();
				int type = button();
				int diversification = _tiles->at( button()-1 )->getDiversification();
				int transition = cell->getTransition();
				int tcelltype = cell->getTransitionCellType();
				int decogroup = cell->getDecorationGroup();
				int decoitem = cell->getDecorationItem();

				switch( _eff->checkedId() ) {
					case 1:
						transition = computeTransition( (GenericMap *)theMap, (GenericCell *)cell );
						tcelltype = computeTransitionCellType( (GenericMap *)theMap, (GenericCell *)cell );
						break;
				}
				theMap->changeCell( row, col , type, transition, tcelltype, decogroup, decoitem, diversification );
			}
		}
	}
}

