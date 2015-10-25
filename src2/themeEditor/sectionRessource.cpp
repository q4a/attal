/****************************************************************
**
** Attal : Lords of Doom
**
** sectionRessource.cpp
** section specifying resources
**
** Version : $Id: sectionRessource.cpp,v 1.15 2009/10/22 17:05:24 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/06/2001
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

#include "sectionRessource.h"
 
// generic include files
// include files for QT
#include <QInputDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/log.h"

#include "libClient/gui.h"

extern QString IMAGE_PATH;

SectionResource::SectionResource( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	setTitle( tr( "Resource" ) );

	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	layout->addWidget( _name );

	_icon = new AskPixmap( true, "", tr( "Icon: " ), _mainWidget );
	layout->addWidget( _icon );

	_smallIcon = new AskPixmap( true, "", tr( "Small icon: " ), _mainWidget );
	layout->addWidget( _smallIcon );

	_comboInAct = new QComboBox( this );
	_comboInAct->addItem( tr( "Local Preservable" ) );
	_comboInAct->addItem( tr( "Local Not Preservable" ) );
	_comboInAct->addItem( tr( "Global Preservable" ) );
	_comboInAct->addItem( tr( "Global Not Preservable" ) );
	FIXEDSIZE( _comboInAct );

	layout->addWidget( _comboInAct );

	layout->addStretch( 1 );
	layout->activate();

	init();
}

void SectionResource::clear()
{
	_name->setValue( "" );
	_icon->setValue( "" );
	_smallIcon->setValue( "" );
}

void SectionResource::save()
{
	if( DataTheme.resources.count() > _num ) {
		ResourceModel * ress = DataTheme.resources.at( _num  );
		ress->setResourceName( _name->getValue() );
		ress->setType((ResourceModel::ResourceType) _comboInAct->currentIndex());
		
		QString dest = _icon->getValue();
		dest.remove( IMAGE_PATH );
		ress->setFileName( dest );
		_icon->setDestination( dest );
		QString dest2 = _smallIcon->getValue();
		dest2.remove( IMAGE_PATH );
		ress->setSmallFileName( dest2 );
		_smallIcon->setDestination( dest2 );
		
		_icon->save();
		_smallIcon->save();
		DataTheme.resources.save();
	}
}

void SectionResource::init()
{
	if( DataTheme.resources.count() > _num ) {
		ResourceModel * ress = DataTheme.resources.at( _num );
		QString es = ress->getResourceName();
		_name->setValue( es );
		_comboInAct->setCurrentIndex((uint) ress->getType());

		QString dest = IMAGE_PATH + ress->getFileName();
		TRACE("dest %s ", qPrintable( dest ) );
		_icon->setDestination( dest );

		dest = IMAGE_PATH + ress->getSmallFileName();
		TRACE("dest2 %s ", qPrintable( dest ) );
		_smallIcon->setDestination( dest );

	}
}

void SectionResource::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionResource::selectPrevious()
{
	save();
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionResource::selectNext()
{
	save();
	_num = std::min( DataTheme.resources.count() - 1, _num + 1 );
	init();
}

void SectionResource::selectLast()
{
	save();
	_num = DataTheme.resources.count() - 1;
	init();
}

void SectionResource::selectNew()
{
	save();
	ResourceModel * resource = new ResourceModel();
	_num = DataTheme.resources.count();
	resource->setId( _num );
	DataTheme.resources.append( resource );
	clear();
}

void SectionResource::selectDel()
{
	if( DataTheme.resources.count() > 0 ) {
		TRACE(" _num %d", _num);
		delete DataTheme.resources.takeAt( _num );
		_num = std::min( _num, DataTheme.resources.count() - 1 );
		for( int i = 0; i < DataTheme.resources.count(); i++ ) {
			ResourceModel * ress = DataTheme.resources.at( i );
			ress->setId( i );
		}
		init();
	}
}




