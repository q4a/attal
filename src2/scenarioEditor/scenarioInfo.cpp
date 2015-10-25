/****************************************************************
**
** Attal : Lords of Doom
**
** scenarioInfo.cpp
** Display/modif scenario info
**
** Version : $Id: scenarioInfo.cpp,v 1.12 2009/10/22 17:25:04 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QFont>
#include <QHBoxLayout>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "conf.h"
#include "libCommon/log.h"
#include "libClient/gui.h"

int ScenarioInfo::_defaultVal = 1;

ScenarioInfo::ScenarioInfo( QWidget * parent, const char * /* name */ )
	: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "General Scenario Informations" ) );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );

	QLabel * title = new QLabel( tr( "General Information" ), this );
	title->setFont( QFont( "Helvetica", 20, QFont::Bold ) );
	title->setAlignment( Qt::AlignCenter );
	FIXEDSIZE( title );
	layout->addWidget( title );
	layout->addSpacing( 15 );

	QHBoxLayout * layH1 = new QHBoxLayout();

	QLabel * scenarName = new QLabel( tr( "Name: " ), this );
	FIXEDSIZE( scenarName );
	layH1->addWidget( scenarName );

	_name = new QLineEdit( this );
	_name->setFixedSize( 200, 20 );
	layH1->addWidget( _name );

	layH1->addStretch( 1 );

	layout->addLayout( layH1 );
	layout->addSpacing( 5 );

	QHBoxLayout * layH2 = new QHBoxLayout();

	QLabel * theme = new QLabel( tr( "Theme: " ), this );
	FIXEDSIZE( theme );
	layH2->addWidget( theme );

	_theme = new QComboBox( this );
	searchThemes( _theme );
	FIXEDSIZE( _theme );
	layH2->addWidget( _theme );
	layH2->addStretch( 1 );

	QLabel * nbPlayer = new QLabel( tr( "Number of players: " ), this );
	FIXEDSIZE( nbPlayer );
	layH2->addWidget( nbPlayer );

	_nbPlayers = new QSpinBox( this );
	FIXEDSIZE( _nbPlayers );
	layH2->addWidget( _nbPlayers );

	layH2->addStretch( 1 );

	layout->addLayout( layH2 );
	layout->addSpacing( 5 );

	QHBoxLayout * layH3 = new QHBoxLayout();

	QLabel * height = new QLabel( tr( "Map height: " ), this );
	FIXEDSIZE( height );
	layH3->addWidget( height );

	_editHeight = new QLineEdit( this );
	_editHeight->setFixedSize( 50, 20 );
	layH3->addWidget( _editHeight );
	layH3->addStretch( 1 );

	QLabel * width = new QLabel( tr( "Map width: " ), this );
	FIXEDSIZE( width );
	layH3->addWidget( width );

	_editWidth = new QLineEdit( this );
	_editWidth->setFixedSize( 50, 25 );
	layH3->addWidget( _editWidth );
	layH3->addStretch( 1 );

	layout->addLayout( layH3 );
	layout->addSpacing( 5 );
	
	QHBoxLayout * layH5 = new QHBoxLayout();
	
	QLabel * day = new QLabel( tr( "Day: " ), this );
	FIXEDSIZE( day );
	layH5->addWidget( day );

	_day = new QSpinBox( this );
	FIXEDSIZE( _day );
	layH5->addWidget( _day );
	_day->setValue( 1 );

	QLabel * week = new QLabel( tr( "Week: " ), this );
	FIXEDSIZE( week );
	layH5->addWidget( week );

	_week = new QSpinBox( this );
	FIXEDSIZE( _week );
	layH5->addWidget( _week );
	_week->setValue( 1 );
	
	QLabel * month = new QLabel( tr( "Month: " ), this );
	FIXEDSIZE( month );
	layH5->addWidget( month );

	_month = new QSpinBox( this );
	FIXEDSIZE( _month );
	layH5->addWidget( _month );
	_month->setValue( 1 );


	QLabel * year = new QLabel( tr( "Year: " ), this );
	FIXEDSIZE( year );
	layH5->addWidget( year );

	_year = new QSpinBox( this );
	_year->setMaximum( 9999 );
	_year->setValue( 1044 );
	FIXEDSIZE( _year );
	layH5->addWidget( _year );

	layH5->addStretch( 1 );

	layout->addLayout( layH5 );
	layout->addSpacing( 5 );


	QHBoxLayout * layH4 = new QHBoxLayout();

	QLabel * desc = new QLabel( tr( "Description: " ), this );
	FIXEDSIZE( desc );
	layH4->addWidget( desc );
	layH4->addStretch( 1 );

	layout->addLayout( layH4 );

	_desc = new QTextEdit( this );
	layout->addWidget( _desc, 1 );

	QPushButton * butOk = new QPushButton( this );
	butOk->setText( tr( "Ok" ) );
	FIXEDSIZE( butOk );
	QPushButton * butCan = new QPushButton( this );
	butCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( butCan );

	QHBoxLayout * layH6 = new QHBoxLayout();
	layH6->addStretch( 1 );
	layH6->addWidget( butOk );
	layH6->addStretch( 1 );
	layH6->addWidget( butCan );
	layH6->addStretch( 1 );

	layout->addSpacing( 5 );
	layout->addLayout( layH6 );
	layout->activate();

	connect( butCan, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( butOk, SIGNAL( clicked() ), SLOT( slot_ok() ) );

	setMapHeight( 50 );
	setMapWidth( 50 );
	setNbPlayer( 2 );
	setName( tr( "Map " ) + QString::number( _defaultVal ) );
	setDescription( tr( "Map for Attal" ) );
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
	combo->addItem( "cyberpunk" );
	combo->addItem( "medieval" );
}

void ScenarioInfo::setTheme( const QString & theme )
{
	int index = 0;

	do {
		_theme->setCurrentIndex( index );
		index++;
	} while ( ( index < _theme->count() ) && ( _theme->currentText() != theme ) );
	
	if( _theme->currentText() != theme ) {
		logEE( "Theme %s not found", qPrintable(theme) );
		_theme->setCurrentIndex( 0 );
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

