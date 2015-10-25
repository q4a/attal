/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: tilePresentation.cpp,v 1.10 2006/11/28 21:27:30 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "tilePresentation.h"
 
// generic include files
#include <stdlib.h>
// include files for QT
#include <QComboBox>
#include <QHBoxLayout>
#include <QLayout>
#include <QPushButton>
#include <QString>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/gameData.h"
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/cell.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/map.h"

//
// ----- TilePresentation -----
//

TilePresentation::TilePresentation( QWidget * parent, const char * name )
: QWidget( parent )
{
	setWindowTitle( QString ( name ));
	_button = new AttalButton( this );
	_button->setFixedSize( 40, 40 );

	_combo = new QComboBox( this );
	FIXEDSIZE( _combo );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addWidget( _button );
	layout->addSpacing( 10 );
	layout->addWidget( _combo );
	layout->addStretch( 1 );
	layout->activate();
	setFixedWidth( 220 );
	setFixedHeight( 40 );
}

void TilePresentation::setCell( CellModel * cell )
{
	uint nb = cell->getDiversificationNumber();
	uint index;

	QString text;

	_cell = cell;
	index = DataTheme.tiles.indexOf( cell );
	_button->setPixmap( ImageTheme.cells[index]->at( 0 ) );
	_combo->clear();
	_combo->addItem( "Random" );
	for( uint i = 0; i < nb; i++ ) {
		text = tr( "%1 (weight=%2)" ).arg(i+1).arg( cell->getDiversification( i ) );
		_combo->addItem( text );
	}
	FIXEDSIZE( _combo );
}


uint TilePresentation::getDiversification()
{
	uint ret = 0;

	ret = _combo->currentIndex();

	if( ret == 0 ) {
		ret = _cell->getRandomDiversification();
	} else {
		ret--;
	}

	return ret;
}
