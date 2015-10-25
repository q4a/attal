/****************************************************************
**
** Attal : Lords of Doom
**
** lordExchange.cpp
** this is a template for all .cpp files
**
** Version : $Id: lordExchange.cpp,v 1.8 2003/01/21 17:28:09 lusum Exp $
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
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qsignalmapper.h>
#include <qtabwidget.h>
// application specific include files
#include "common/attalSocket.h"
#include "common/dataTheme.h"
#include "common/genericLord.h"
#include "common/warMachine.h"

#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/widget.h"

extern DataTheme DataTheme;
extern ImageTheme ImageTheme;

/** add comments here */
LordExchange::LordExchange( QWidget * parent, const char * name )
	:QDialog( parent, name, true )
{
	_lordLeft = 0;
	_lordRight = 0;
	_socket = 0;

	_presentation = new PresentationWidget( this );

	QTabWidget * tab = new QTabWidget( this );
	_generalities = new DisplayBothGeneralities( this );
	_units = new DisplayBothUnits( this );
	_artefacts = new DisplayBothArtefacts( this );
	_machines = new DisplayBothMachines( this );
	tab->insertTab( _generalities, "Generalities" );
	tab->insertTab( _units, "Units" );
	tab->insertTab( _artefacts, "Artefacts" );
	tab->insertTab( _machines, "War Machines" );
	tab->setCurrentPage( 0 );

	QPushButton * butOk = createButtonOk( this );
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( butOk );
	layH1->addStretch( 1 );

	QGridLayout * layout = new QGridLayout( this, 3, 1 );
	layout->addWidget( _presentation, 0 , 0 );
	layout->setRowStretch( 1, 1 );
	layout->addWidget( tab, 1, 0 );
	layout->addLayout( layH1, 2, 0 );
	layout->activate();

	connect( butOk, SIGNAL( clicked() ), SLOT( accept() ) );
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

void LordExchange::initSocket( AttalSocket * socket )
{
	_socket = socket;
	_units->initSocket( socket );
}

//
// -- PresentationWidget --
//
PresentationWidget::PresentationWidget( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_photoLeft = new Icon( this );
	_photoRight = new Icon( this );

	_titleLeft = new QLabel( this );
	_titleLeft->setAlignment( AlignCenter );

	_titleRight = new QLabel( this );
	_titleRight->setAlignment( AlignCenter );

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
	title.sprintf( "Lord %s\nLevel %d of %s", lordLeft->getName().latin1(),
		       lordLeft->getLevel(), lordLeft->getCategoryName().latin1() );
	_titleLeft->setText( title );
	FIXEDSIZE( _titleLeft );

	title.sprintf( "Lord %s\nLevel %d of %s", lordRight->getName().latin1(),
		       lordRight->getLevel(), lordRight->getCategoryName().latin1() );
	_titleRight->setText( title );
	FIXEDSIZE( _titleRight );
}


//
// -- DisplayBothGeneralities --
//

DisplayBothGeneralities::DisplayBothGeneralities( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_listLeft = new QListBox( this );
	_listRight = new QListBox( this );

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

void DisplayBothGeneralities::fillList( QListBox * list, GenericLord * lord )
{
	QString item;

	item = "Spell points: " + QString::number( lord->getCharac( TECHNICPOINT ) )
		+ "/" + QString::number( lord->getCharac( MAXTECHNICPOINT ) );
	list->insertItem( item );
	item = "Morale: " + QString::number( lord->getCharac( MORALE ) );
	list->insertItem( item );
	item = "Luck: " + QString::number( lord->getCharac( LUCK ) );
	list->insertItem( item );
	item = "Attack: " + QString::number( lord->getCharac( ATTACK ) );
	list->insertItem( item );
	item = "Defense: " + QString::number( lord->getCharac( DEFENSE ) );
	list->insertItem( item );
	item = "Power: " + QString::number( lord->getCharac( POWER ) );
	list->insertItem( item );
	item = "Knowledge: " + QString::number( lord->getCharac( KNOWLEDGE ) );
	list->insertItem( item );
}

//
// -- PresentUnit --
//

PresentUnit::PresentUnit( QWidget * parent, const char * name )
	: QWidget( parent, name )
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
		_icon->setPixmap( QPixmap( ImageTheme.getPhotoCreature( unit ) ) );
		_label->setText( QString::number( unit->getNumber() ) );
	} else {
		_icon->setPixmap( QPixmap( "" ) );
		_label->setText( "" );
	}
}

void PresentUnit::select( bool state )
{

}

//
// -- DisplayBothUnits --
//

DisplayBothUnits::DisplayBothUnits( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_currentSide = -1;
	_currentNum = -1;
	_exchange = false;
	_lordLeft = 0;
	_lordRight = 0;
	_socket = 0;

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
						dest->addNumber( orig->getNumber() );
						origLord->setUnit( _currentNum, 0 );
						delete orig;
					} else {
						origLord->setUnit( _currentNum, dest );
						destLord->setUnit( num, orig );
					}
				} else {
					destLord->setUnit( num, orig );
					origLord->setUnit( _currentNum, 0 );
				}
			}
		}
		if( _socket ) {
			_socket->sendExchangeUnit( origLord, _currentNum, destLord, num );
		}
		_currentNum = -1;
		_currentSide = -1;
		reinit();
	}
}



//
// -- DisplayBothArtefacts --
//

DisplayBothArtefacts::DisplayBothArtefacts( QWidget * parent, const char * name )
	:QWidget( parent, name )
{

}

void DisplayBothArtefacts::initLords( GenericLord * lordLeft, GenericLord * lordRight )
{

}

//
// ----- PresentMachines -----
//

PresentMachines::PresentMachines( QWidget * parent, const char * name )
	:QWidget( parent, name )
{
	_labels.setAutoDelete( true );
	_lord = 0;
}

void PresentMachines::initLord( GenericLord * lord )
{
	_lord = lord;
	reinit();
}

void PresentMachines::reinit()
{
	_labels.clear();
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

DisplayBothMachines::DisplayBothMachines( QWidget * parent, const char * name )
	:QWidget( parent, name )
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














