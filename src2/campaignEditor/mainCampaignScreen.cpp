/****************************************************************
**
** Attal : Lords of Doom
**
** mainCampaignScreen.cpp
** main widget of the campaign editor
**
** Version : $Id: mainCampaignScreen.cpp,v 1.12 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 21/08/2004
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

#include "mainCampaignScreen.h"

 
// generic include files
// include files for QT
#include <QFile>
#include <QLayout>
#include <QTextStream>
#include <QVBoxLayout>
// application specific includes
#include "libCommon/campaign.h"
#include "libCommon/log.h"

#include "libClient/askWidget.h"

MainCampaignScreen::MainCampaignScreen( QWidget * parent, char * /* name*/ )
: QWidget( parent )
{
	_campaign = 0;
	
	_name = new AskString( tr( "Name: " ), this );
	_description = new AskString( tr( "Description: " ), this );
	_scenarii = new AskStringList( this );
	_scenarii->setLabel( tr( "List of scenarii: " ) );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _name );
	layout->addWidget( _description );
	layout->addWidget( _scenarii );
	layout->addStretch( 1 );
	layout->activate();
}

MainCampaignScreen::~MainCampaignScreen()
{
	if( _campaign ) {
		delete _campaign;
	}
}

void MainCampaignScreen::reinit()
{
	if( _campaign ) {
		uint nbScen = _campaign->getScenarioNumber();
		
		_name->setValue( _campaign->getName() );
		_description->setValue( _campaign->getDescription() );
		_scenarii->clear();
		for( uint i = 0; i < nbScen; ++i ) {
			_scenarii->addValue( _campaign->getScenario( i ) );
		}
	}
}

bool MainCampaignScreen::load( const QString & filename )
{
	Campaign * newCampaign = new Campaign();

	if( newCampaign->load( filename ) ) {
		if( _campaign ) {
			delete _campaign;
		}
		_campaign = newCampaign;
		reinit();
	} else {
		delete newCampaign;
		return false;
	}
	return true;
}

void MainCampaignScreen::save( const QString & filename )
{
	QFile f( filename );

	if (! f.open(QIODevice::WriteOnly) ) {
		logEE( "Could not open file %s for writing\n", qPrintable(filename) );
		return;
	}
	uint nbScen = _scenarii->count();
	_campaign->clearScenarii();

	QTextStream ts( &f );

	_campaign->setName( _name->getValue() );
	_campaign->setDescription( _description->getValue() );
	for( uint i = 0; i < nbScen; ++i ) {
		_campaign->addScenario(_scenarii->getValue(i) );
	}
	if( _campaign ) {
		_campaign->save( & ts );
	}

	f.close();
}

void MainCampaignScreen::newCampaign()
{
	if( _campaign ) {
		delete _campaign;
	}
	_campaign = new Campaign;
	reinit();
}

