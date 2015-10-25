/****************************************************************
**
** Attal : Lords of Doom
**
** sectionDecoration.cpp
** section for specifying decorations
**
** Version : $Id: sectionDecoration.cpp,v 1.4 2002/11/01 13:42:38 audoux Exp $
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
// include files for QT
#include <qlayout.h>
// application specific include files
#include "common/log.h"
#include "common/dataTheme.h"
#include "client/imageTheme.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;
extern QString IMAGE_PATH;

SectionDecoration::SectionDecoration( QWidget * parent , const char * name )
	: GenericSection( parent, name )
{
	_num = 0;
	
	setTitle( "Decoration" );
	
	QVBoxLayout * layout = new QVBoxLayout( _mainWidget );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	
	_name = new AskString( "Name : ", _mainWidget );
	layout->addWidget( _name );
	
	_info = new AskString( "Info : ", _mainWidget );
	layout->addWidget( _info );
	
	_moveEffect = new AskCombo( "Movement effect : ", _mainWidget );
	_moveEffect->insertItem( "None" );
	_moveEffect->insertItem( "Bonus" );
	_moveEffect->insertItem( "Malus" );
	_moveEffect->insertItem( "Block" );
	layout->addWidget( _moveEffect );
	
	_technicEffect = new AskCombo( "Technic effect : ", _mainWidget );
	_technicEffect->insertItem( "None" );
	_technicEffect->insertItem( "Block" );
	layout->addWidget( _technicEffect );
	
	_image = new AskPixmap( true, "", "Image : ", _mainWidget );
	layout->addWidget( _image );
	
	layout->addStretch( 1 );
	layout->activate();
	
	init();
}

void SectionDecoration::clear()
{
	_name->setValue( "" );
	_info->setValue( "" );
	_moveEffect->setCurrentItem( 0 );
	_technicEffect->setCurrentItem( 0 );
}

void SectionDecoration::init()
{
	if( DataTheme.decorations.count() > 0 ) {
		GenericDecoration * decor = DataTheme.decorations.at( _num );
		_name->setValue( decor->getName() );
		_info->setValue( decor->getInfo() );
		_moveEffect->setCurrentItem( decor->getMoveEffect() );
		_technicEffect->setCurrentItem( decor->getTechnicEffect() );
		QString dest = IMAGE_PATH + QString( "decor/decoration_" ) + QString::number( _num + 1 ) + QString( ".png" );
		_image->setDestination( dest );
	}
}

void SectionDecoration::save()
{
	if( DataTheme.decorations.count() > 0 ) {	
		GenericDecoration * decor = DataTheme.decorations.at( _num );
		decor->setName( _name->getValue() );
		decor->setInfo( _info->getValue() );
		decor->setMoveEffect( _moveEffect->currentItem() );
		decor->setTechnicEffect( _technicEffect->currentItem() );
		_image->save();
	}
}

void SectionDecoration::selectFirst()
{
	save();
	_num = 0;
	init();
}

void SectionDecoration::selectPrevious()
{
	save();
	_num = QMAX( 0, _num - 1 );
	init();
}

void SectionDecoration::selectNext()
{
	save();
	_num = QMIN( DataTheme.decorations.count() - 1, _num + 1 );
	init();
}

void SectionDecoration::selectLast()
{
	save();
	_num = DataTheme.decorations.count() - 1;
	init();
}

void SectionDecoration::selectNew()
{
	save();
	GenericDecoration * decor = new GenericDecoration();
	_num = DataTheme.decorations.count();
	DataTheme.decorations.append( decor );
	clear();
}

void SectionDecoration::selectDel()
{
	if( DataTheme.decorations.count() > 0 ) {
		DataTheme.decorations.remove( _num );
		_num = QMIN( _num, DataTheme.decorations.count() - 1 );
		init();
	}
}


