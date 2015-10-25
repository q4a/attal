/****************************************************************
**
** Attal : Lords of Doom
**
** sectionTeam.cpp
** section for specifying team (flag, color)
**
** Version : $Id: sectionTeam.cpp,v 1.2 2003/01/06 20:42:25 lusum Exp $
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
// include files for QT
#include <qlayout.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qpushbutton.h>
// application specific include files
#include "common/log.h"
#include "client/gui.h"
#include "common/dataTheme.h"
#include "client/imageTheme.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;
extern QString IMAGE_PATH;

SectionTeam::SectionTeam( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	_red = 0;
	_green = 0;
	_blue = 0;
	
	setTitle( "Team" );
	
	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( "Name: ", _mainWidget );
	layout->addWidget( _name );
	
	_pbColor = new QPushButton( "Color", _mainWidget );
	FIXEDSIZE( _pbColor );
	layout->addWidget( _pbColor, 0, Qt::AlignLeft );
	
	_flag = new AskPixmap( true, "", "Flag: ", _mainWidget );
	layout->addWidget( _flag );
	
	layout->addStretch( 1 );
	layout->activate();
	
	init();
	
	connect( _pbColor, SIGNAL( clicked() ), SLOT( slot_color() ) );
}

void SectionTeam::slot_color()
{
	QColor col = QColorDialog::getColor( QColor( _red, _green, _blue ), this );
	_red = col.red();
	_green = col.green();
	_blue = col.blue();
	_pbColor->setBackgroundColor( col );
}

void SectionTeam::clear()
{
	_name->setValue( QString( "Team" ) + QString::number( _num + 1 ) );
	_red = 0;
	_green = 0;
	_blue = 0;
	_pbColor->setBackgroundColor( QColor( 0, 0, 0 ) );
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
		_pbColor->setBackgroundColor( QColor( _red, _green, _blue ) );
		QString dest = IMAGE_PATH + QString( "team/flag_" ) + QString::number( _num + 1 ) + QString( ".png" );
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
	_num = QMAX( 0, _num - 1 );
	init();
}

void SectionTeam::selectNext()
{
	save();
	_num = QMIN( (int)DataTheme.teams.count() - 1, _num + 1 );
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
	GenericTeam * team = new GenericTeam();
	_num = DataTheme.teams.count();
	DataTheme.teams.append( team );
	clear();
}

void SectionTeam::selectDel()
{
	if( DataTheme.teams.count() > 0 ) {
		DataTheme.teams.remove( _num );
		_num = QMIN( _num, (int)DataTheme.teams.count() - 1 );
		init();
	}
}


