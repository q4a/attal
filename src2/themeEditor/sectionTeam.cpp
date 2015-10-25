/****************************************************************
**
** Attal : Lords of Doom
**
** sectionTeam.cpp
** section for specifying team (flag, color)
**
** Version : $Id: sectionTeam.cpp,v 1.9 2006/11/03 18:29:03 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2001
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

#include "sectionTeam.h"
 
// generic include files
#include <algorithm>

// include files for QT
#include <QColor>
#include <QColorDialog>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;

SectionTeam::SectionTeam( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	_red = 0;
	_green = 0;
	_blue = 0;
	
	setTitle( tr( "Team" ) );

	_name = new AskString( tr( "Name: " ), _mainWidget );
	
	_pbColor = new QPushButton( tr( "Color" ), _mainWidget );
	FIXEDSIZE( _pbColor );
	
	_flag = new AskPixmap( true, "", tr( "Flag: " ), _mainWidget );
	
	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _pbColor, 0, Qt::AlignLeft );
	layout->addWidget( _flag );
	layout->addStretch( 1 );
	layout->activate();
	
	init();
	
	connect( _pbColor, SIGNAL( clicked() ), SLOT( slot_color() ) );
}

void SectionTeam::slot_color()
{
	QColor col = QColorDialog::getColor( QColor( _red, _green, _blue ), this );
	if( col.isValid() ) {
		_red = col.red();
		_green = col.green();
		_blue = col.blue();
		QPalette p = _pbColor->palette();
		p.setColor( QPalette::Button, col );
		_pbColor->setPalette( p );
	}
}

void SectionTeam::clear()
{
	_name->setValue( QString( tr( "Team" ) ) + QString::number( _num + 1 ) );
	_red = 0;
	_green = 0;
	_blue = 0;

	QPalette palette;
	palette.setColor(_pbColor->backgroundRole(), QColor( 0, 0, 0 ));
	_pbColor->setPalette(palette);

	_flag->setValue( "" );
}

void SectionTeam::init()
{
	if( DataTheme.decorations.count() > 0 ) {
		GenericTeam * team = DataTheme.teams.at( _num );
		_name->setValue( team->getName() );
		_red = team->getRed();
		_green = team->getGreen();
		_blue = team->getBlue();
		QPalette palette;
		palette.setColor(_pbColor->backgroundRole(), QColor( _red, _green, _blue ));
		_pbColor->setPalette(palette);
		QString dest = IMAGE_PATH + QString( "players/flag_" ) + QString::number( _num ) + QString( ".png" );
		_flag->setDestination( dest );
	}
}

void SectionTeam::save()
{
	if( DataTheme.teams.count() > 0 ) {	
		GenericTeam * team = DataTheme.teams.at( _num );
		team->setName( _name->getValue() );
		team->setRed( _red );
		team->setGreen( _green );
		team->setBlue( _blue );
		_flag->save();
	}
}

void SectionTeam::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionTeam::selectPrevious()
{
	save();
	_num = std::max( 0, _num - 1 );
	init();
}

void SectionTeam::selectNext()
{
	save();
	_num = std::min( (int)DataTheme.teams.count() - 1, _num + 1 );
	init();
}

void SectionTeam::selectLast()
{
	save();
	_num = DataTheme.teams.count() - 1;
	init();
}

void SectionTeam::selectNew()
{
	save();
	_num = DataTheme.teams.count();
	GenericTeam * team = new GenericTeam( _num );
	DataTheme.teams.append( team );
	clear();
}

void SectionTeam::selectDel()
{
	if( DataTheme.teams.count() > 0 ) {
		delete DataTheme.teams.takeAt( _num );
		_num = std::min( _num, DataTheme.teams.count() - 1 );
		init();
	}
}


