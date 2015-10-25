/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInfo.cpp
** Display/modif scenario info
**
** Version : $Id: scenarioInfo.cpp,v 1.3 2002/10/16 22:05:51 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/02/2001
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

#include "scenarioInfo.h"
 
// generic include files
// include files for QT
#include <qfont.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
// application specific include files
#include "conf.h"
#include "common/log.h"
#include "client/gui.h"

int ScenarioInfo::_defaultVal = 1;

ScenarioInfo::ScenarioInfo( QWidget * parent, const char * name )
	: QDialog( parent, name, true )
{
	setCaption( "General Scenario Informations" );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );

	QLabel * title = new QLabel( "General Information", this );
	title->setFont( QFont( "Helvetica", 20, QFont::Bold ) );
	title->setAlignment( AlignCenter );
	FIXEDSIZE( title );
	layout->addWidget( title );
	layout->addSpacing( 15 );

	QHBoxLayout * layH1 = new QHBoxLayout();

	QLabel * scenarName = new QLabel( "Name : ", this );
	FIXEDSIZE( scenarName );
	layH1->addWidget( scenarName );

	_name = new QLineEdit( this );
	_name->setFixedSize( 200, 20 );
	layH1->addWidget( _name );

	layH1->addStretch( 1 );

	layout->addLayout( layH1 );
	layout->addSpacing( 5 );

	QHBoxLayout * layH2 = new QHBoxLayout();

	QLabel * theme = new QLabel( "Theme : ", this );
	FIXEDSIZE( theme );
	layH2->addWidget( theme );

	_theme = new QComboBox( this );
	searchThemes( _theme );
	FIXEDSIZE( _theme );
	layH2->addWidget( _theme );
	layH2->addStretch( 1 );

	QLabel * nbPlayer = new QLabel( "Number of players : ", this );
	FIXEDSIZE( nbPlayer );
	layH2->addWidget( nbPlayer );

	_nbPlayers = new QSpinBox( 1, 8, 1, this );
	FIXEDSIZE( _nbPlayers );
	layH2->addWidget( _nbPlayers );

	layH2->addStretch( 1 );

	layout->addLayout( layH2 );
	layout->addSpacing( 5 );

	QHBoxLayout * layH3 = new QHBoxLayout();

	QLabel * height = new QLabel( "Map height : ", this );
	FIXEDSIZE( height );
	layH3->addWidget( height );

	_editHeight = new QLineEdit( this );
	_editHeight->setFixedSize( 50, 20 );
	layH3->addWidget( _editHeight );
	layH3->addStretch( 1 );

	QLabel * width = new QLabel( "Map width : ", this );
	FIXEDSIZE( width );
	layH3->addWidget( width );

	_editWidth = new QLineEdit( this );
	_editWidth->setFixedSize( 50, 25 );
	layH3->addWidget( _editWidth );
	layH3->addStretch( 1 );

	layout->addLayout( layH3 );
	layout->addSpacing( 5 );

	QHBoxLayout * layH4 = new QHBoxLayout();

	QLabel * desc = new QLabel( "Description :", this );
	FIXEDSIZE( desc );
	layH4->addWidget( desc );
	layH4->addStretch( 1 );

	layout->addLayout( layH4 );

	_desc = new QMultiLineEdit( this );
	layout->addWidget( _desc, 1 );

	QPushButton * butOk = new QPushButton( this );
	butOk->setText( "Ok" );
	FIXEDSIZE( butOk );
	QPushButton * butCan = new QPushButton( this );
	butCan->setText( "Cancel" );
	FIXEDSIZE( butCan );

	QHBoxLayout * layH5 = new QHBoxLayout();
	layH5->addStretch( 1 );
	layH5->addWidget( butOk );
	layH5->addStretch( 1 );
	layH5->addWidget( butCan );
	layH5->addStretch( 1 );

	layout->addSpacing( 5 );
	layout->addLayout( layH5 );
	layout->activate();

	connect( butCan, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( butOk, SIGNAL( clicked() ), SLOT( slot_ok() ) );

	setMapHeight( 40 );
	setMapWidth( 40 );
	setNbPlayer( 2 );
	setName( "Map " + QString::number( _defaultVal ) );
	setDescription( "Map for Attal" );
	setTheme( "medieval" );
	_defaultVal++;
}

void ScenarioInfo::slot_ok()
{
	/// XXX: have to check values
	accept();
}

void ScenarioInfo::searchThemes( QComboBox * combo )
{
	/// XXX: have to be changed for a real search of theme name
	combo->insertItem( "cyberpunk" );
	combo->insertItem( "medieval" );
}

void ScenarioInfo::setTheme( QString theme )
{
	int index = 0;

	do {
		_theme->setCurrentItem( index );
		index++;
	} while ( ( index < _theme->count() ) && ( _theme->currentText() != theme ) );
	
	if( _theme->currentText() != theme ) {
		logEE( "Theme %s not found", theme.latin1() );
		_theme->setCurrentItem( 0 );
	}
}

void ScenarioInfo::clear()
{
	setName( "" );
	setTheme( THEME_DEFAULT );
	setNbPlayer( 2 );
	setMapWidth( 50 );
	setMapHeight( 50 );
	setDescription( "" );
}

