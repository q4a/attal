/****************************************************************
**
** Attal : Lords of Doom
**
** sectionDecoration.cpp
** section for specifying decorations
**
** Version : $Id: sectionDecoration.cpp,v 1.13 2006/11/27 22:37:45 lusum Exp $
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

#include "sectionDecoration.h"

// generic include files
#include <algorithm>
// include files for QT
#include <QLayout>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericDecoration.h"
#include "libCommon/log.h"

#include "libClient/imageTheme.h"

#include "themeEditor/decorationEffect.h"
#include "themeEditor/mapDispositionEditor.h"

extern QString IMAGE_PATH;

//
// ----- SectionDecoration -----
//

SectionDecoration::SectionDecoration( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 1;
	_numItem = 0;
	
	setTitle( tr( "Decoration group" ) );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	layout->addWidget( _name );

	_info = new AskString( tr( "Info: " ), _mainWidget );
	layout->addWidget( _info );
	
	_decorationEffect = new DecorationEffect( _mainWidget );
	layout->addWidget( _decorationEffect );

	_selectItem = new SelectionWidget( _mainWidget );
	_selectItem->setTitle( tr( "Decoration item" ) );
	layout->addWidget( _selectItem );
	
	_image = new AskPixmap( true, "", tr( "Image: " ), _mainWidget );
	layout->addWidget( _image );
	
	_dispo = new MapDispositionEditor( _mainWidget );
	layout->addWidget( _dispo );

	layout->addStretch( 1 );
	layout->activate();
	
	connect( _selectItem, SIGNAL( sig_first() ), SLOT( slot_firstItem() ) );
	connect( _selectItem, SIGNAL( sig_previous() ), SLOT( slot_previousItem() ) );
	connect( _selectItem, SIGNAL( sig_next() ), SLOT( slot_nextItem() ) );
	connect( _selectItem, SIGNAL( sig_last() ), SLOT( slot_lastItem() ) );
	connect( _selectItem, SIGNAL( sig_new() ), SLOT( slot_newItem() ) );
	connect( _selectItem, SIGNAL( sig_del() ), SLOT( slot_delItem() ) );
	
	init();
}

void SectionDecoration::clear()
{
	_name->setValue( "" );
	_info->setValue( "" );
}

void SectionDecoration::init()
{
	if( DataTheme.decorations.count() > 1 ) {
		DecorationGroup * group = DataTheme.decorations.at( _num );
		_name->setValue( group->getName() );
		_info->setValue( group->getInfo() );
		_decorationEffect->init( group );
		QString dest = IMAGE_PATH + QString( "decor/decoration_" ) + QString::number( _num ) + "_" + QString::number( _numItem ) + QString( ".png" );
		_image->setDestination( dest );
		_dispo->init( group->at( _numItem ), dest );
	}
}

void SectionDecoration::save()
{
	if( DataTheme.decorations.count() > 0 ) {
		DecorationGroup * group = DataTheme.decorations.at( _num  );
		group->setName( _name->getValue() );
		group->setInfo( _info->getValue() );
		_decorationEffect->save();
		_image->save();
	}
}

void SectionDecoration::selectFirst()
{
	save();
	if( _num != 1 ) {
		_num = 1;
		_numItem = 0;
	}
	init();
}

void SectionDecoration::selectPrevious()
{
	save();
	if( _num != std::max( 1, _num - 1 ) ) {
		_num = std::max( 1, _num - 1 );
		_numItem = 0;
	}
	init();
}

void SectionDecoration::selectNext()
{
	save();
	if( _num != std::min( DataTheme.decorations.count() - 1, _num + 1 ) ) {
		_num = std::min( DataTheme.decorations.count() - 1, _num + 1 );
		_numItem = 0;
	}
	init();
}

void SectionDecoration::selectLast()
{
	save();
	if( _num != DataTheme.decorations.count() - 1 ) {
		_num = DataTheme.decorations.count() - 1;
		_numItem = 0;
	}
	init();
}

void SectionDecoration::selectNew()
{
	save();
	DecorationGroup * group = new DecorationGroup();
	_num = DataTheme.decorations.count();
	_numItem = 0;
	DataTheme.decorations.append( group );
	clear();
}

void SectionDecoration::selectDel()
{
	if( DataTheme.decorations.count() > 0 ) {
		delete DataTheme.decorations.takeAt( _num ); /// XXX: other things to do ? remove pixmaps ?
		_num = std::min( _num, DataTheme.decorations.count() - 1 );
		_numItem = 0;
		init();
	}
}

void SectionDecoration::slot_firstItem()
{
	save();
	_numItem = 0;
	init();
}

void SectionDecoration::slot_previousItem()
{
	save();
	_numItem = std::max( 0, _numItem - 1 );
	init();
}

void SectionDecoration::slot_nextItem()
{
	save();
	_numItem = std::min( DataTheme.decorations.at( _num )->count() - 1, _numItem + 1 );
	init();
}

void SectionDecoration::slot_lastItem()
{
	save();
	_numItem = DataTheme.decorations.at( _num )->count() - 1;
	init();
}

void SectionDecoration::slot_newItem()
{
	save();
	GenericDecoration * decor = new GenericDecoration();
	_numItem = DataTheme.decorations.at( _num )->count();
	DataTheme.decorations.at( _num )->append( decor );
	clear();
}

void SectionDecoration::slot_delItem()
{
	if( DataTheme.decorations.at( _num )->count() > 0 ) {
		delete DataTheme.decorations.at( _num )->takeAt( _numItem ); /// XXX: other things to do ? remove/shift pixmaps ?
		_numItem = std::min( _numItem, DataTheme.decorations.at( _num )->count() - 1 );
		init();
	}
}
