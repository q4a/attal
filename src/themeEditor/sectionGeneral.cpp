/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGeneral.cpp
** section with general info about theme
**
** Version : $Id: sectionGeneral.cpp,v 1.3 2003/04/11 10:11:45 audoux Exp $
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

#include "sectionGeneral.h"
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlabel.h>
#include <qlayout.h>
// application specific include files
#include "client/gui.h"
#include "common/log.h"

extern QString IMAGE_PATH;

SectionGeneral::SectionGeneral( QWidget * parent, const char * name )
	: Section( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_fog = new AskBool( "Fog of war", this );
	layout->addWidget( _fog );
	
	_nbUnit = new AskInt( "Number of unit/lord", this );
	_nbUnit->setMinValue( 1 );
	_nbUnit->setMaxValue( 10 );
	layout->addWidget( _nbUnit );
	
	_logo = new AskPixmap( false, IMAGE_PATH + QString( "misc/logo.png" ), "Logo (200x180):", this );
	layout->addWidget( _logo );
	layout->addStretch( 1 );
	
	layout->activate();
}

void SectionGeneral::save()
{
	_logo->save();
	emit sig_changed();
}
