/****************************************************************
**
** Attal : Lords of Doom
**
** gainLevel.cpp
** Dialog when lord gain a new level
**
** Version : $Id: gainLevel.cpp,v 1.7 2008/09/07 16:17:19 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/12/2000
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

#include "gainLevel.h"
 
// generic include files
// include files for QT
#include <QLayout>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include
#include "libClient/gui.h"
#include "libClient/widget.h"

extern QString IMAGE_PATH;


/*!  Constructs an empty

*/

GainLevel::GainLevel( QWidget * parent, const char * /* name */)
	:QDialog( parent, Qt::Dialog )
{
	_skill = SKI_ATTACK;
	_choice = -1;
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_title = new Sentence( this );
	_title->setText( "Actarius gain a level" );
	
	layout->addSpacing( 5 );
	layout->addWidget( _title );
	layout->addSpacing( 10 );
	
	_photo = new Icon( this );
	_photo->setPixmap( QPixmap( IMAGE_PATH + "lords/lord_001.png" ) );
	
	Icon * panel1 = new Icon( this );
	panel1->setPixmap( QPixmap( IMAGE_PATH + "misc/exp.png" ) );
	Icon * panel2 = new Icon( this );
	panel2->setPixmap( QPixmap( IMAGE_PATH + "misc/exp.png" ) );
	
	QHBoxLayout * lay1 = new QHBoxLayout();
	lay1->addStretch( 1 );
	lay1->addWidget( panel1 );
	lay1->addStretch( 1 );
	lay1->addWidget( _photo );
	lay1->addStretch( 1 );
	lay1->addWidget( panel2 );
	lay1->addStretch( 1 );
	
	layout->addLayout( lay1, 1 );
	
	layout->addSpacing( 10 );
	
	_level = new Sentence( this );
	_level->setText( "Actarius is now a level 6 hacker" );
		
	layout->addWidget( _level );
	layout->addSpacing( 5 );
	layout->addStretch( 1 );
	layout->addSpacing( 5 );
	
	_spec = new Sentence( this );
	_spec->setText( "Knowledge + 1" );
	
	layout->addWidget( _spec );
	layout->addSpacing( 5 );
	
	_choice1 = new QPushButton( this );
	_choice1->setIcon( QIcon( QPixmap( IMAGE_PATH + "skills/skill_001.png" ) ) );
	_choice1->setFixedSize( 40, 40 );
	
	_choice2 = new QPushButton( this );
	_choice2->setIcon( QIcon( QPixmap( IMAGE_PATH + "skills/skill_002.png" ) ) );
	_choice2->setFixedSize( 40, 40 );
	
	QHBoxLayout * lay3 = new QHBoxLayout();
	lay3->addStretch( 1 );
	lay3->addWidget( _choice1 );
	lay3->addStretch( 1 );
	lay3->addWidget( _choice2 );
	lay3->addStretch( 1 );
	
	layout->addLayout( lay3, 1 );
	layout->addSpacing( 5 );
	
	QHBoxLayout * lay4 = new QHBoxLayout();
	lay4->addStretch( 5 );

	_butOk = new AttalButton( this, AttalButton::BT_OK );
	_butOk->setEnabled( false );	
	lay4->addWidget( _butOk );

	lay4->addStretch( 1 );
	
	layout->addLayout( lay4 );
	layout->addSpacing( 5 );
	
	layout->activate();
	
	setFixedSize( 420, 480 );
	
	connect( _butOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( _choice1, SIGNAL( clicked() ), SLOT( slot_choice1() ) );
	connect( _choice2, SIGNAL( clicked() ), SLOT( slot_choice2() ) );
}

void GainLevel::reinit()
{
	_choice = -1;
	_butOk->setEnabled( false );	
}
