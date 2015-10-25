/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGround.cpp
** section for specifying grounds
**
** Version : $Id: sectionGround.cpp,v 1.11 2007/05/05 23:53:08 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

#include "sectionGround.h"
 
// generic include files
#include <algorithm>

// include files for QT
#include <QLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericCell.h"
#include "libCommon/log.h"

#include "libClient/imageTheme.h"

#include "themeEditor/askDiversification.h"

extern QString IMAGE_PATH;

SectionGround::SectionGround( QWidget * parent, const char * name )
: GenericSection( parent, name )
{
	setTitle( tr( "Tiles" ) );

	_width = new AskInt( tr( "Width: " ), _mainWidget );
	_height = new AskInt( tr( "Height: " ), _mainWidget );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	_coef = new AskInt( tr( "Coef: " ), _mainWidget );
	_color = new AskColor( tr( "Color: " ), _mainWidget );
	_diversification = new AskDiversificationList( _mainWidget );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	
	_layH = new QHBoxLayout();
	_layH->addWidget( _width );
	_layH->addWidget( _height );

	layout->addLayout( _layH );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _coef );
	layout->addWidget( _color );
	layout->addWidget( _diversification );
	layout->addStretch( 1 );
	layout->activate();

	_num = 1;
	init();
}

SectionGround::~SectionGround()
{
	delete _layH;
}

void SectionGround::clear()
{
	_name->setValue( QString( tr( "Tile " ) ) + QString::number( _num + 1 ) );
	_coef->setValue( 0 );
	_color->setValue( Qt::black );
	_diversification->clear();
}

void SectionGround::init()
{
	_width->setValue( DataTheme.tiles.getWidth() );
	_height->setValue( DataTheme.tiles.getHeight() );
	if( DataTheme.tiles.count() > 0 ) {
		CellModel * cell = DataTheme.tiles.at( _num );
		_name->setValue( cell->getName() );
		_coef->setValue( cell->getCoeff() );
		_color->setValue( cell->getColor() );
		_diversification->setValue( cell );
	}
}

void SectionGround::save()
{
	DataTheme.tiles.setWidth( _width->getValue()  );
	DataTheme.tiles.setHeight(_height->getValue()  );
	if( DataTheme.tiles.count() > 0 ) {
		CellModel * cell = DataTheme.tiles.at( _num );
		cell->setName( _name->getValue() );
		cell->setCoeff( _coef->getValue() );
		cell->setColor( _color->getValue() );
		_diversification->save();
	}
}

void SectionGround::selectFirst()
{
	save();
	_num = 1;
	init();
}

void SectionGround::selectPrevious()
{
	save();
	_num = std::max( 1, _num - 1 );
	init();
}

void SectionGround::selectNext()
{
	save();
	_num = std::min( (int)DataTheme.tiles.count() - 1, _num + 1 );
	init();
}

void SectionGround::selectLast()
{
	save();
	_num = DataTheme.tiles.count() - 1;
	init();
}

void SectionGround::selectNew()
{
	save();
	CellModel * cell = new CellModel();
	_num = DataTheme.tiles.count();
	DataTheme.tiles.append( cell );
	clear();
}

void SectionGround::selectDel()
{
	if( DataTheme.tiles.count() > 1 ) {
		delete DataTheme.tiles.takeAt( _num );
		_num = std::min( _num, (int)DataTheme.tiles.count() - 1 );
		init();
	}
}


