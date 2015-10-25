/****************************************************************
**
** Attal : Lords of Doom
**
** sectionExperience.cpp
** section for setting levels of experience
**
** Version : $Id: sectionExperience.cpp,v 1.5 2007/03/16 14:22:48 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/05/2004
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

#include "sectionExperience.h"
 
// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
// application specific include files
#include "libCommon/dataTheme.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"

SectionExperience::SectionExperience( QWidget * parent, const char * name )
: Section( parent, name )
{
	QLabel * labList = new QLabel( this );
	labList->setText( tr( "List of required experience to gain for accessing the next level:" ) );
	FIXEDSIZE( labList );

	_list = new AskIntList( this );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( labList );
	layout->addWidget( _list );
	layout->addStretch( 1 );
	layout->activate();

	init();
}

void SectionExperience::clear()
{
	_list->clear();
}

void SectionExperience::init()
{
	uint nbLevels = DataTheme.lordExperience.getLevelNumber();
	uint lastLevel = 0;

	for( uint i = 1; i < nbLevels; i++ ) {
		_list->addValue( DataTheme.lordExperience.getLevel( i ) - lastLevel );
		lastLevel = DataTheme.lordExperience.getLevel( i );
	}
}

void SectionExperience::save()
{
	uint nbLevels = _list->count();

	DataTheme.lordExperience.clear();
	for( uint i = 0; i < nbLevels; i++ ) {
		DataTheme.lordExperience.appendLevelByDiff( _list->getValue( i ) );
	}
}

