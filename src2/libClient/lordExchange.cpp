/****************************************************************
**
** Attal : Lords of Doom
**
** lordExchange.cpp
** this is a template for all .cpp files
**
** Version : $Id: lordExchange.cpp,v 1.23 2013/11/18 20:54:07 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/08/2002
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

#include "lordExchange.h"

// generic include files
// include files for QT
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/artefactManager.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/warMachine.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

/** add comments here */
LordExchange::LordExchange( QWidget * parent, GenericLord * lordLeft, GenericLord * lordRight  )
	:QDialog( parent, Qt::Dialog )
{
	_lordLeft = 0;
	_lordRight = 0;

	_presentation = new PresentationWidget( this );

	QTabWidget * tab = new QTabWidget( this );
	_generalities = new DisplayBothGeneralities();
	_units = new DisplayBothUnits();
	_artefacts = new DisplayBothArtefacts();
	_machines = new DisplayBothMachines();
	
	initLords( lordLeft, lordRight );

	tab->insertTab( 0, _generalities, "Generalities" );
	tab->insertTab( 1,_units, "Units" );
	tab->insertTab( 2,_artefacts, "Artefacts" );
	tab->insertTab( 3,_machines, "War Machines" );
	tab->setCurrentIndex( 0 );

	AttalButton * butOk = new AttalButton( this, AttalButton::BT_OK );
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( butOk );
	layH1->addStretch( 1 );

	QGridLayout * layout = new QGridLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _presentation, 0 , 0 );
	layout->setRowStretch( 1, 1 );
	layout->addWidget( tab, 1, 0 );
	layout->addLayout( layH1, 2, 0 );
	layout->activate();

	connect( butOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( parent, SIGNAL( sig_updateWidget() ), SLOT( reinit() ) );
	connect( _units, SIGNAL( sig_sendExchangeUnit( GenericLord * , int , GenericLord * , int ) ), parent, SLOT( slot_sendExchangeUnit( GenericLord * , int , GenericLord * , int ) ));
	connect( _artefacts, SIGNAL( sig_sendExchangeArtefact( GenericLord * , int , GenericLord * ) ), parent, SLOT( slot_sendExchangeArtefact( GenericLord * , int , GenericLord * ) ));
}

void LordExchange::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	_lordLeft = lordLeft;
	_lordRight = lordRight;

	_presentation->initLords( lordLeft, lordRight );
	_generalities->initLords( lordLeft, lordRight );
	_units->initLords( lordLeft, lordRight );
	_artefacts->initLords( lordLeft, lordRight );
	_machines->initLords( lordLeft, lordRight );
}

void LordExchange::reinit()
{
	initLords( _lordLeft, _lordRight );
}

//
// -- PresentationWidget --
//

PresentationWidget::PresentationWidget( QWidget * parent, const char * /* name */ )
	:QWidget( parent )
{
	_photoLeft = new Icon( this );
	_photoRight = new Icon( this );

	_titleLeft = new QLabel( this );
	_titleLeft->setAlignment( Qt::AlignCenter );

	_titleRight = new QLabel( this );
	_titleRight->setAlignment( Qt::AlignCenter );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addWidget( _photoLeft );
	layout->addWidget( _titleLeft );
	layout->addStretch( 1 );
	layout->addWidget( _titleRight );
	layout->addWidget( _photoRight );
	layout->activate();
}

void PresentationWidget::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	_photoLeft->setPixmap( * ImageTheme.getLordPixmap( lordLeft->getId() ) );

	_photoRight->setPixmap( * ImageTheme.getLordPixmap( lordRight->getId() ) );

	QString title;
	title.sprintf( "Lord %s\nLevel %d of %s", qPrintable( lordLeft->getName()),
		       lordLeft->getCharac( LEVEL ), qPrintable( lordLeft->getCategoryName()) );
	_titleLeft->setText( title );
	FIXEDSIZE( _titleLeft );

	title.sprintf( "Lord %s\nLevel %d of %s", qPrintable( lordRight->getName()),
		       lordRight->getCharac( LEVEL ), qPrintable( lordRight->getCategoryName() ) );
	_titleRight->setText( title );
	FIXEDSIZE( _titleRight );
}


//
// -- DisplayBothGeneralities --
//

DisplayBothGeneralities::DisplayBothGeneralities( QWidget * parent, const char * /* name */ )
	:QWidget( parent )
{
	_listLeft = new QListWidget( this );
	_listRight = new QListWidget( this );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	layH1->addWidget( _listLeft, 1 );
	layH1->addSpacing( 10 );
	layH1->addWidget( _listRight, 1 );
	layH1->addSpacing( 5 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addSpacing( 5 );
	layout->addLayout( layH1, 1 );
	layout->addSpacing( 5 );

	layout->activate();
}


void DisplayBothGeneralities::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	_lordLeft = lordLeft;
	_lordRight = lordRight;
	reinit();
}

void DisplayBothGeneralities::reinit()
{
	_listLeft->clear();
	_listRight->clear();

	fillList( _listLeft, _lordLeft );
	fillList( _listRight, _lordRight );
}

void DisplayBothGeneralities::fillList( QListWidget * list, GenericLord * lord )
{
	QString item;

	item = "Spell points: " + QString::number( lord->getCharac( TECHNICPOINT ) )
		+ "/" + QString::number( lord->getCharac( MAXTECHNICPOINT ) );
	list->addItem( item );
	item = "Morale: " + QString::number( lord->getCharac( MORALE ) );
	list->addItem( item );
	item = "Luck: " + QString::number( lord->getCharac( LUCK ) );
	list->addItem( item );
	item = "Attack: " + QString::number( lord->getCharac( ATTACK ) );
	list->addItem( item );
	item = "Defense: " + QString::number( lord->getCharac( DEFENSE ) );
	list->addItem( item );
	item = "Power: " + QString::number( lord->getCharac( POWER ) );
	list->addItem( item );
	item = "Knowledge: " + QString::number( lord->getCharac( KNOWLEDGE ) );
	list->addItem( item );
}

//
// -- PresentUnit --
//

PresentUnit::PresentUnit( QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	_icon = new Icon( this );
	_label = new Label( this );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addWidget( _icon );
	layout->addWidget( _label, 1 );

	layout->activate();

	connect( _icon, SIGNAL( sig_clicked() ), SIGNAL( sig_clicked() ) );
	connect( _label, SIGNAL( sig_clicked() ), SIGNAL( sig_clicked() ) );
}

void PresentUnit::setUnit( GenericFightUnit * unit )
{
	if( unit ) {
		_icon->setPixmap( ImageTheme.getPhotoCreature( unit ) );
		_label->setText( QString::number( unit->getNumber() ) );
	} else {
		_icon->setPixmap( QPixmap( "" ) );
		_label->setText( "" );
	}
}

void PresentUnit::select( bool /*state*/ )
{
	/// XXX: TODO
	//logEE( "not yet implemented" );
}

//
// -- DisplayBothUnits --
//

DisplayBothUnits::DisplayBothUnits( QWidget * parent, const char * /* name */ )
	:QWidget( parent )
{
	_currentSide = -1;
	_currentNum = -1;
	_exchange = false;
	_lordLeft = 0;
	_lordRight = 0;

	QSignalMapper * sigmapLeft = new QSignalMapper( this );
	QSignalMapper * sigmapRight = new QSignalMapper( this );

	QVBoxLayout * layV1 = new QVBoxLayout();
	QVBoxLayout * layV2 = new QVBoxLayout();
	layV1->addStretch( 1 );
	layV2->addStretch( 1 );
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_unitLeft[i] = new PresentUnit( this );
		layV1->addWidget( _unitLeft[i] );
		layV1->addStretch( 1 );
		sigmapLeft->setMapping( _unitLeft[i], i );
		connect( _unitLeft[i], SIGNAL( sig_clicked() ), sigmapLeft, SLOT( map() ) );

		_unitRight[i] = new PresentUnit( this );
		layV2->addWidget( _unitRight[i] );
		layV2->addStretch( 1 );
		sigmapRight->setMapping( _unitRight[i], i );
		connect( _unitRight[i], SIGNAL( sig_clicked() ), sigmapRight, SLOT( map() ) );
	}

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addLayout( layV1, 1 );
	layout->addLayout( layV2, 1 );
	layout->activate();

	connect( sigmapLeft, SIGNAL( mapped( int ) ), SLOT( slot_unitLeft( int ) ) );
	connect( sigmapRight, SIGNAL( mapped( int ) ), SLOT( slot_unitRight( int ) ) );
}

void DisplayBothUnits::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	_lordLeft = lordLeft;
	_lordRight = lordRight;
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_unitLeft[i]->setUnit( lordLeft->getUnit( i ) );
		_unitLeft[i]->select( false );
		_unitRight[i]->setUnit( lordRight->getUnit( i ) );
		_unitRight[i]->select( false );
	}

	if( ( _currentSide != - 1 ) && ( _currentNum != -1 ) ) {
		if( _currentSide == 0 ) {
			_unitLeft[ _currentNum ]->select();
		} else {
			_unitRight[ _currentNum ]->select();
		}
	}
}

void DisplayBothUnits::reinit()
{
	initLords( _lordLeft, _lordRight );
}

void DisplayBothUnits::slot_unitLeft( int num )
{
	manageClick( 0, num );
}

void DisplayBothUnits::slot_unitRight( int num )
{
	manageClick( 1, num );
}

void DisplayBothUnits::manageClick( int side, int num )
{
	if( ( ! _lordLeft ) || ( ! _lordRight ) ) {
		return;
	}

	if( ( _currentSide == - 1 ) && ( _currentNum == -1 ) ) {
		if( side == 0 ) {
			if( _lordLeft->getUnit( num ) ) {
				_unitLeft[num]->select();
				_currentSide = side;
				_currentNum = num;
			}
		} else {
			if( _lordRight->getUnit( num ) ) {
				_unitRight[num]->select();
				_currentSide = side;
				_currentNum = num;
			}
		}
	} else {
		GenericFightUnit * orig = 0;
		GenericLord * origLord = 0;
		if( _currentSide == 0 ) {
			orig = _lordLeft->getUnit( _currentNum );
			origLord = _lordLeft;
		} else {
			orig = _lordRight->getUnit( _currentNum );
			origLord = _lordRight;
		}

		GenericFightUnit * dest = 0;
		GenericLord * destLord = 0;
		if( side == 0 ) {
			dest = _lordLeft->getUnit( num );
			destLord = _lordLeft;
		} else {
			dest = _lordRight->getUnit( num );
			destLord = _lordRight;
		}

		if( ( destLord == origLord ) && ( dest == orig ) ) {
			_currentSide = -1;
			_currentNum = -1;
			initLords( _lordLeft, _lordRight );
		} else {
			if( orig ) {
				if( dest ) {
					if( ( orig->getRace() == dest->getRace() ) &&
						orig->getLevel() == dest->getLevel() ) {
						//dest->addNumber( orig->getNumber() );
						//origLord->setUnit( _currentNum, 0 );
						//delete orig;
					} else {
						//origLord->setUnit( _currentNum, dest );
						//destLord->setUnit( num, orig );
					}
				} else {
					//destLord->setUnit( num, orig );
					//origLord->setUnit( _currentNum, 0 );
				}
			}
		}
		if( _currentNum > -1 ) {
			emit sig_sendExchangeUnit( origLord, _currentNum, destLord, num );
		}
		_currentNum = -1;
		_currentSide = -1;
		reinit();
	}
}

//
// -- DisplayBothArtefacts --
//

DisplayBothArtefacts::DisplayBothArtefacts( QWidget * parent, const char * /* name */ )
	:QWidget( parent )
{
	_lordLeft = 0;
	_lordRight = 0;
	
	QPushButton * pbToRight = new QPushButton( this );
	pbToRight->setText( "->" );
	FIXEDSIZE( pbToRight );
	
	QPushButton * pbToLeft = new QPushButton( this );
	pbToLeft->setText( "<-" );
	FIXEDSIZE( pbToLeft );
	
	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->addStretch( 1 );
	layV1->addWidget( pbToRight );
	layV1->addStretch( 1 );
	layV1->addWidget( pbToLeft );
	layV1->addStretch( 1 );
	
	_artefactsLeft = new QListWidget( this );
	
	_artefactsRight = new QListWidget( this );
	
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _artefactsLeft );
	layout->addLayout( layV1 );
	layout->addWidget( _artefactsRight );
	layout->activate();
	
	connect( pbToRight, SIGNAL( clicked() ), SLOT( slot_exchangeToRight() ) );
	connect( pbToLeft, SIGNAL( clicked() ), SLOT( slot_exchangeToLeft() ) );
}

void DisplayBothArtefacts::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	uint i;
	uint nbArtefacts;
	uint type;
	QString artefactName;
	ArtefactManager * artefactManager;
	GenericLordArtefact * artefact;
	
	_lordLeft = lordLeft;
	_lordRight = lordRight;
	artefactManager = lordLeft->getArtefactManager();
	nbArtefacts = artefactManager->getArtefactNumber();
	_artefactsLeft->clear();
	
	for( i = 0; i < nbArtefacts; i++ ) {
		artefact = artefactManager->getArtefact( i );
		type = artefact->getType();
		artefactName = DataTheme.artefacts.at( type )->getName();
		_artefactsLeft->addItem( artefactName );
	}
	
	
	artefactManager = lordRight->getArtefactManager();
	nbArtefacts = artefactManager->getArtefactNumber();
	_artefactsRight->clear();
	
	for( i = 0; i < nbArtefacts; i++ ) {
		artefact = artefactManager->getArtefact( i );
		type = artefact->getType();
		artefactName = DataTheme.artefacts.at( type )->getName();
		_artefactsRight->addItem( artefactName );
	}
}

void DisplayBothArtefacts::slot_exchangeToRight()
{
	int item = _artefactsLeft->currentRow();
	
	if( item != -1 ) {
		emit sig_sendExchangeArtefact( _lordLeft, item, _lordRight );

		ArtefactManager * manag1 = _lordLeft->getArtefactManager();
		ArtefactManager * manag2 = _lordRight->getArtefactManager();

		GenericLordArtefact * artefact = manag1->getArtefact( item );

		manag1->removeArtefact( item );
		manag2->addArtefact( artefact );

		initLords( _lordLeft, _lordRight );
	}
}
	
void DisplayBothArtefacts::slot_exchangeToLeft()
{
	int item = _artefactsRight->currentRow();

	if( item != -1 ) {
		emit sig_sendExchangeArtefact( _lordRight, item, _lordLeft );

		ArtefactManager * manag1 = _lordRight->getArtefactManager();
		ArtefactManager * manag2 = _lordLeft->getArtefactManager();

		GenericLordArtefact * artefact = manag1->getArtefact( item );

		manag1->removeArtefact( item );
		manag2->addArtefact( artefact );

		initLords( _lordLeft, _lordRight );

	}
}

//
// ----- PresentMachines -----
//

PresentMachines::PresentMachines( QWidget * parent, const char * /* name*/ )
	:QWidget( parent )
{
	_lord = 0;
}

PresentMachines::~PresentMachines()
{
	while( ! _labels.isEmpty() ) {
		delete _labels.takeFirst();
	}
}

void PresentMachines::initLord( GenericLord * lord )
{
	_lord = lord;
	reinit();
}

void PresentMachines::reinit()
{
	while( ! _labels.isEmpty() ) {
		delete _labels.takeFirst();
	}
	if( _lord ) {
		for( uint i = 0; i < _lord->getMachineNumber(); i++ ) {
			WarMachine * machine = DataTheme.machines.at( _lord->getMachine( i ) );
			if( machine ) {
				InfoLabel * label = new InfoLabel( this );
				label->move( 0, (70 * i) + 5 );
				label->setText( machine->getName() );
				_labels.append( label );
			}
		}
	}
}

//
// -- DisplayBothMachines --
//

DisplayBothMachines::DisplayBothMachines( QWidget * parent, const char * /* name */)
	:QWidget( parent )
{
	_machineLeft = new PresentMachines( this );
	_machineRight = new PresentMachines( this );
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->addSpacing( 5 );
	layout->addWidget( _machineLeft );
	layout->addSpacing( 10 );
	layout->addWidget( _machineRight );
	layout->addSpacing( 5 );
	layout->activate();
}

void DisplayBothMachines::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{
	_machineLeft->initLord( lordLeft );
	_machineRight->initLord( lordRight );
}

