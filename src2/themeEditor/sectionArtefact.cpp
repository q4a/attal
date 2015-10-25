/****************************************************************
**
** Attal : Lords of Doom
**
** sectionArtefact.cpp
** section for specifying artefacts
**
** Version : $Id: sectionArtefact.cpp,v 1.7 2006/11/03 18:28:59 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 20/01/2002
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

#include "sectionArtefact.h"
 
// generic include files
#include <algorithm>

// include files for QT
#include <QLayout>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"
#include "libCommon/artefact.h"

#include "libClient/imageTheme.h"
#include "libClient/graphicalArtefact.h"

#include "themeEditor/artefactAction.h"

extern QString IMAGE_PATH;

SectionArtefact::SectionArtefact( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	
	setTitle( tr( "Artefact" ) );

	_name = new AskString( tr( "Name: " ), _mainWidget );

	_position = new AskInt( tr( "Position: " ), _mainWidget );
	_position->setMinValue( 0 );
	_position->setMaxValue( 10 );

	_icon = new AskPixmap( true, "", tr( "Icon: " ), _mainWidget );

	_item = new AskPixmap( true, "", tr( "Item: " ), _mainWidget );

	_actions = new ArtefactAction( _mainWidget );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _position );
	layout->addWidget( _icon );
	layout->addWidget( _item );
	layout->addWidget( _actions );
	layout->addStretch( 1 );
	layout->activate();
	
	init();
}

void SectionArtefact::clear()
{
	_name->setValue( "" );
	_position->setValue( 0 );
}

void SectionArtefact::init()
{
	if( (int)DataTheme.artefacts.count() > _num ) {
		GenericArtefactModel * artefact = DataTheme.artefacts.at( _num );
		_name->setValue( artefact->getName() );
		_position->setValue( artefact->getPosition() );
		QString dest;
		dest = IMAGE_PATH + QString( "artefacts/artefactIcon_" ) + QString::number( _num ) + QString( ".png" );
		_icon->setDestination( dest );
		dest = IMAGE_PATH + QString( "artefacts/artefactItem_" ) + QString::number( _num ) + QString( ".png" );
		_item->setDestination( dest );
		_actions->init( artefact );
	}
}

void SectionArtefact::save()
{
	if( (int)DataTheme.artefacts.count() > _num ) {
		GenericArtefactModel * artefact = DataTheme.artefacts.at( _num );
		artefact->setName( _name->getValue() );
		artefact->setPosition( _position->getValue() );
		_icon->save();
		_item->save();
		_actions->save();
	}
}

void SectionArtefact::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionArtefact::selectPrevious()
{
	save();
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionArtefact::selectNext()
{
	save();
	_num = std::min( int(DataTheme.artefacts.count() - 1), _num + 1 );
	init();
}

void SectionArtefact::selectLast()
{
	save();
	_num = DataTheme.artefacts.count() - 1;
	init();
}

void SectionArtefact::selectNew()
{
	save();
	GenericArtefactModel * artefact = new GenericArtefactModel();
	_num = DataTheme.artefacts.count();
	DataTheme.artefacts.append( artefact );
	clear();
}

void SectionArtefact::selectDel()
{
	if( DataTheme.artefacts.count() > 0 ) {
		delete DataTheme.artefacts.takeAt( _num );
		_num = std::min( _num, int(DataTheme.artefacts.count() - 1) );
		init();
	}
}


