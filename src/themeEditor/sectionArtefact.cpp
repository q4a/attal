/****************************************************************
**
** Attal : Lords of Doom
**
** sectionArtefact.cpp
** section for specifying artefacts
**
** Version : $Id: sectionArtefact.cpp,v 1.2 2002/04/20 18:42:10 audoux Exp $
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
// include files for QT
#include <qlayout.h>
// application specific include files
#include "common/log.h"
#include "common/dataTheme.h"
#include "common/artefact.h"
#include "client/imageTheme.h"
#include "client/graphicalArtefact.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;
extern QString IMAGE_PATH;

SectionArtefact::SectionArtefact( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	
	setTitle( "Artefact" );
	
	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( "Name: ", _mainWidget );
	layout->addWidget( _name );
	
	_position = new AskInt( "Position: ", _mainWidget );
	_position->setMinValue( 0 );
	_position->setMaxValue( 10 );
	layout->addWidget( _position );
	
	_icon = new AskPixmap( true, "", "Icon: ", _mainWidget );
	layout->addWidget( _icon );

	_item = new AskPixmap( true, "", "Item: ", _mainWidget );
	layout->addWidget( _item );
	
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
	_num = QMAX( 0, _num - 1 );
	init();
}

void SectionArtefact::selectNext()
{
	save();
	_num = QMIN( int(DataTheme.artefacts.count() - 1), _num + 1 );
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
		DataTheme.artefacts.remove( _num );
		_num = QMIN( _num, int(DataTheme.artefacts.count() - 1) );
		init();
	}
}


