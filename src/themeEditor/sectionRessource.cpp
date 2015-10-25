/****************************************************************
**
** Attal : Lords of Doom
**
** sectionRessource.cpp
** section specifying ressources
**
** Version : $Id: sectionRessource.cpp,v 1.4 2003/05/09 12:45:24 audoux Exp $
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
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qinputdialog.h>
// application specific include files
#include "client/gui.h"
#include "common/dataTheme.h" 
#include "common/log.h"

extern DataTheme DataTheme;
extern QString IMAGE_PATH;

SectionRessource::SectionRessource( QWidget * parent, const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	setTitle( "Ressource" );
	
	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );	
	
	_name = new AskString( "Name: ", _mainWidget );
	layout->addWidget( _name );
	
	_icon = new AskPixmap( true, "", "Icon: ", _mainWidget );
	layout->addWidget( _icon );

	_smallIcon = new AskPixmap( true, "", "Small icon: ", _mainWidget );
	layout->addWidget( _smallIcon );

	layout->addStretch( 1 );
	layout->activate();

	init();
}

void SectionRessource::clear()
{
	_name->setValue( "" );
	_icon->setValue( "" );
	_smallIcon->setValue( "" );
}

void SectionRessource::save()
{
	if( DataTheme.ressources.count() > _num ) {
		DataTheme.ressources.update( _num, _name->getValue() );
		_icon->save();
		_smallIcon->save();
	}
}

void SectionRessource::init()
{
	if( DataTheme.ressources.count() > _num ) {
		QString es = DataTheme.ressources.getRessource( _num );
		_name->setValue( es );
		QString dest;
		dest.sprintf( "%sressources/ress_%02d.png", IMAGE_PATH.latin1(), _num + 1 );
		_icon->setDestination( dest );
		dest.sprintf( "%sressources/smallResource_%02d.png", IMAGE_PATH.latin1(), _num + 1 );
		_smallIcon->setDestination( dest );

	}
}

void SectionRessource::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionRessource::selectPrevious()
{
	save();
	_num = QMAX( 0, _num - 1 );
	init();
}

void SectionRessource::selectNext()
{
	save();
	_num = QMIN( DataTheme.ressources.count() - 1, _num + 1 );
	init();
}

void SectionRessource::selectLast()
{
	save();
	_num = DataTheme.ressources.count() - 1;
	init();
}

void SectionRessource::selectNew()
{
	save();
	QString * ressource = new QString();
	_num = DataTheme.ressources.count();
	DataTheme.ressources.append( ressource );
	clear();
}

void SectionRessource::selectDel()
{
	if( DataTheme.ressources.count() > 0 ) {
		DataTheme.ressources.remove( _num );
		_num = QMIN( _num, DataTheme.ressources.count() - 1 );
		init();
	}
}




