/****************************************************************
**
** Attal : Lords of Doom
**
** displayBase.cpp
** inside the base
**
** Version : $Id: displayBase.cpp,v 1.40 2003/01/25 18:11:32 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/09/2000
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

#include "displayBase.h"

 
// generic include files
// include files for QT
#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qsignalmapper.h>
// application specific include files
#include "conf.h"
#include "common/attalSocket.h"
#include "common/dataTheme.h"
#include "common/genericBuilding.h"
#include "common/genericLord.h"

#include "client/displayCreature.h"
#include "client/game.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/insideAction.h"
#include "client/insideBase.h"
#include "client/market.h"
#include "client/unitExchange.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;
extern ImageTheme ImageTheme;
extern DataTheme DataTheme;

/** add comments here */
DisplayBase::DisplayBase( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_player = 0;
	_base = 0;
	_socket = 0;
	_game = 0;
	
	QVBoxLayout * layout = new QVBoxLayout( this );

	_inside = new InsideBase( this );	
	       
	InsideBaseView * view = new InsideBaseView( _inside, this );		
	layout->addWidget( view, 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	_baseSummary = new BaseSummary( this );
	layH2->addWidget( _baseSummary );
	
	_baseLords = new BaseLords( this );
	layH2->addWidget( _baseLords, 1 );
	
	_baseButtons = new BaseButtons( this );
	layH2->addWidget( _baseButtons );
	
	layout->addLayout( layH2 );
	layout->activate();

	connect( view, SIGNAL( sig_building( GenericInsideBuilding * ) ), SLOT( slot_building( GenericInsideBuilding * ) ) );
	connect( _baseButtons, SIGNAL( sig_quit() ), SIGNAL( sig_quit() ) );
	connect( _baseButtons, SIGNAL( sig_exchange() ), _baseLords, SLOT( slot_exchange() ) );
	connect( _baseLords, SIGNAL( sig_exchange( bool ) ), _baseButtons, SLOT( slot_exchange( bool ) ) );
}

void DisplayBase::handleSocket()
{
	_game->handleSocket();
	_socket->reReadData();
	
	switch( _socket->getCla1() ) {
	case SO_MSG:
		break;
	case SO_GAME:
		break;
	case SO_TURN:
		break;
	case SO_MODIF:
		if( ( _socket->getCla2() == C_MOD_BASE ) &&
		    ( _socket->getCla3() == C_BASE_BUILDING ) )	{
			int row = _socket->readInt();
			int col = _socket->readInt();
			uchar level = _socket->readChar();
			bool create = (bool)_socket->readChar();
			if( ( _base->getCell()->getRow() == row ) &&
			    ( _base->getCell()->getCol() == col ) ) {
				GenericInsideBuilding * building = new GenericInsideBuilding();
				building->setRace( _base->getRace() );
				building->setLevel( level );
				if( create ) {
					_inside->addBuilding( building );
					_base->addBuilding( building );
				} else {
					_inside->delBuilding( building );
					_base->removeBuilding( building );
				}
			}
		} else	if( ( _socket->getCla2() == C_MOD_BASE ) &&
		    ( _socket->getCla3() == C_BASE_UNIT ) ) {
			reinit();
		}
		break;
	case SO_QR:
		break;
	case SO_MVT:
		break;
	case SO_TECHNIC:
		break;
	case SO_EXCH:
		break;
	case SO_CONNECT:
		break;
	case SO_FIGHT:
		break;
	default:
		logEE( "Unknown socket_class" );
	}
}

void DisplayBase::slot_building( GenericInsideBuilding * building )
{
	if( _base ) {
		InsideBuildingModel * model = DataTheme.bases.at( _base->getRace() )->getBuildingModel( building->getLevel() );
		if( model->getAction() ) {
			switch( model->getAction()->getType() ) {
			case 0:
				break;
			case 1:
				actionAllBuildings();
				break;
			case 2:
				actionAllCreatures();
				break;
			case 3:
				actionSomeCreatures( building );
				break;
			case 4:
				actionMarket();
				break;
			default:
				logEE( "Unknown action %d", model->getAction()->getType() );
				break;
			}
		}
	}
}

void DisplayBase::actionAllBuildings()
{
	InsideActionAllBuildings dialog( _base );
	dialog.setSocket( _socket );
	dialog.setPlayer( _player );
	dialog.exec();
}

void DisplayBase::actionAllCreatures()
{
	DisplayCreatureBase creatures( this );
	//dialog.setSocket( _socket );
	creatures.exec();
}

void DisplayBase::actionSomeCreatures( GenericInsideBuilding * building )
{
	if( _base ) {
		InsideAction * action = DataTheme.bases.at( _base->getRace() )->getBuildingModel( building->getLevel() )->getAction();
		if( action ) {
			//InsideActionSomeCreature dialog( action->getParam( 0 ), action->getParam( 1 ) );
			BuyCreature dialog( this );
			dialog.initSocket( _socket );
			int race = action->getParam( 0 );
			int level = action->getParam( 1 );
			dialog.init( _player, _base, DataTheme.creatures.at( race, level ) );
			//dialog.setSocket( _socket );
			if( dialog.exec() ) {
				// try to buy some creature
			}
		}
	}
}

void DisplayBase::actionMarket()
{
	// XXX: change '6'
	Market mark( 6, this );
	mark.exec();
}

void DisplayBase::setPlayer( Player * player )
{
	_player = player;
	_baseSummary->setPlayer( player );
	_baseLords->setPlayer( player );
	_baseButtons->setPlayer( player );
}

void DisplayBase::setBase( GenericBase * base )
{
	_base = base;
	_baseSummary->setBase( base );
	_baseLords->setBase( base );
	_inside->setBase( base );
}

void DisplayBase::reinit()
{
	_baseSummary->reinit();
	_baseLords->reinit();
	//_inside->reinit();
}

void DisplayBase::setSocket( AttalSocket * socket )
{
	_socket = socket;
	_baseLords->setSocket( socket );
}

/*
void DisplayBase::slot_newLord()
{
	if( _baseLords->isVisitor() ) {
		QMessageBox::critical( this, "Error", "There is already a visiting lord" );
		return;
	}

}
*/
//
// ----- BaseSummary -----
//

BaseSummary::BaseSummary( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_base = 0;
	int i;

	QVBoxLayout * layout = new QVBoxLayout( this );
	
	QHBoxLayout * lay1 = new QHBoxLayout();
	
	layout->addLayout( lay1 );
	
	QHBoxLayout * lay2 = new QHBoxLayout();
	lay2->addStretch( 1 );
	for( i = 0; i < 4; i++ ) {
		_unitIco[i] = new UnitSummary( this );
		lay2->addWidget( _unitIco[i] );
		lay2->addStretch( 1 );
	}
	
	layout->addLayout( lay2 );
	
	QHBoxLayout * lay3 = new QHBoxLayout();
	lay3->addStretch( 1 );
	for( i = 0; i < 4; i++ ) {
		_unitIco[4+i] = new UnitSummary( this );
		lay3->addWidget( _unitIco[4+i] );
		lay3->addStretch( 1 );
	}	
	
	layout->addLayout( lay3 );
	
	layout->addStretch( 1 );
	
	layout->activate();
	setFixedHeight( 240 );
}

void BaseSummary::setPlayer( Player * player )
{
	_player = player;
	
	for( int i = 0; i < 8; i++ ) {
		_unitIco[i]->setPlayer( player );
	}
}

void BaseSummary::reinit()
{
}

void BaseSummary::setBase( GenericBase * base )
{
	
}

//
// ----- BaseLords -----
//

BaseLords::BaseLords( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_base = 0;
	_isExchange = false;
	_socket = 0;
	
	setFixedHeight( 240 );	
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_garrison = new BaseTroop( true, this );
	layout->addWidget( _garrison, 1 );
	
	_visitor = new BaseTroop( false, this );
	layout->addWidget( _visitor, 1 );
	
	layout->activate();

	connect( _garrison, SIGNAL( sig_photo() ), SLOT( slot_photoGarrison() ) );
	connect( _garrison, SIGNAL( sig_unit( int ) ), SLOT( slot_unitGarrison( int ) ) );
	connect( _visitor, SIGNAL( sig_photo() ), SLOT( slot_photoVisitor() ) );
	connect( _visitor, SIGNAL( sig_unit( int ) ), SLOT( slot_unitVisitor( int ) ) );

	_garrisonSelected = false;
	_visitorSelected = false;
	_unitGarrison = -1;
	_unitVisitor = -1;
}


void BaseLords::setPlayer( Player * player )
{
	_player = player;

	_garrison->setPlayer( player );
	_visitor->setPlayer( player );
}

void BaseLords::setBase( GenericBase * base )
{
	_base = base;

	_garrison->setBase( base );
	_visitor->setBase( base );

	_garrisonSelected = false;
	_visitorSelected = false;
}

void BaseLords::reinit()
{
	_garrison->reinit();
	_visitor->reinit();
}

void BaseLords::slot_exchange()
{
	_isExchange = !_isExchange;
}

void BaseLords::slot_photoGarrison()
{
	unselectUnits();

	if( _garrisonSelected ) {
		/// XXX: show info lord
		_garrisonSelected = false;
	} else {
		if( _visitorSelected ) {
			exchangeLords();
		} else if( _base->isGarrisonLord() ) {
			_garrisonSelected = true;
		}
	}
}

void BaseLords::slot_photoVisitor()
{
	unselectUnits();
	
	if( _visitorSelected ) {
		/// XXX: show info lord
		_visitorSelected = false;
	} else {
		if( _garrisonSelected ) {
			exchangeLords();
		} else if( _base->isVisitorLord() ) {
			_visitorSelected = true;
		}
	}	
}

void BaseLords::exchangeLords()
{	
	_garrisonSelected = false;
	_visitorSelected = false;
	
	_base->exchangeLords();

	GenericLord * visitor = _base->getVisitorLord();
	_visitor->setLord( visitor );
	if( visitor ) {
		_socket->sendLordGarrison( visitor, false );
	}

	GenericLord * garrison = _base->getGarrisonLord();
	_garrison->setLord( garrison );
	if( garrison ) {
		_socket->sendLordGarrison( garrison, true );
	}
}

void BaseLords::exchangeUnitsGV()
{
	GenericFightUnit * uni1 = _base->getGarrisonUnit( _unitGarrison );
	GenericFightUnit * uni2 = _base->getVisitorUnit( _unitVisitor );
	
	if( uni1 && uni2 ) {
		if( uni1->getCreature() != uni2->getCreature() ) {
			_base->setVisitorUnit( _unitVisitor, uni1 );
			_base->setGarrisonUnit( _unitGarrison, uni2 );
		} else {
			uni2->addNumber( uni1->getNumber() );
			_base->setGarrisonUnit( _unitGarrison, 0 );
			delete uni1;
		}
	} else {
		_base->setVisitorUnit( _unitVisitor, uni1 );
		_base->setGarrisonUnit( _unitGarrison, uni2 );
	}
	GenericLord * garrison = _base->getGarrisonLord();
	GenericLord * visitor = _base->getVisitorLord();
	if(garrison){
		_socket->sendExchangeUnit( garrison, _unitGarrison, visitor, _unitVisitor );
	} else {
		_socket->sendExchangeBaseUnit( _base, _unitGarrison, visitor, _unitVisitor );
	}
	
	_garrison->reinit();
	_visitor->reinit();
}
	
void BaseLords::exchangeUnitsVG()
{
	GenericFightUnit * uni1 = _base->getGarrisonUnit( _unitGarrison );
	GenericFightUnit * uni2 = _base->getVisitorUnit( _unitVisitor );
	
	if( uni1 && uni2 ) {
		if( uni1->getCreature() != uni2->getCreature() ) {
			_base->setVisitorUnit( _unitVisitor, uni1 );
			_base->setGarrisonUnit( _unitGarrison, uni2 );
		} else {
			uni1->addNumber( uni2->getNumber() );
			_base->setVisitorUnit( _unitVisitor, 0 );
			delete uni2;
		}
	} else {
		_base->setVisitorUnit( _unitVisitor, uni1 );
		_base->setGarrisonUnit( _unitGarrison, uni2 );
	}
	GenericLord * garrison = _base->getGarrisonLord();
	GenericLord * visitor = _base->getVisitorLord();
	if(garrison){
		_socket->sendExchangeUnit( garrison, _unitGarrison, visitor, _unitVisitor );
	} else {
		_socket->sendExchangeBaseUnit( _base, _unitGarrison, visitor, _unitVisitor );
	}

	_garrison->reinit();
	_visitor->reinit();
}


void BaseLords::exchangeUnitsGG( int u1, int u2 )
{
	if( u1 == u2 ) {
		return;
	}
	
	GenericFightUnit * uni1 = _base->getGarrisonUnit( u1 );
	GenericFightUnit * uni2 = _base->getGarrisonUnit( u2 );

	if( uni1 && uni2 ) {
		if( uni1->getCreature() != uni2->getCreature() ) {
			_base->setGarrisonUnit( u2, uni1 );
			_base->setGarrisonUnit( u1, uni2 );
		} else {
			uni2->addNumber( uni1->getNumber() );
			_base->setGarrisonUnit( u1, 0 );
			delete uni1;
		}
	} else {
		_base->setGarrisonUnit( u2, uni1 );
		_base->setGarrisonUnit( u1, uni2 );
	}
	GenericLord * garrison = _base->getGarrisonLord();
	if(garrison){
		_socket->sendExchangeUnit( garrison, u1, garrison, u2 );
	}
		_socket->sendExchangeBaseUnit( _base, _unitGarrison, 0, _unitVisitor );
	_garrison->reinit();
}

void BaseLords::exchangeUnitsVV( int u1, int u2 )
{
	if( u1 == u2 ) {
		return;
	}

	GenericFightUnit * uni1 = _base->getVisitorUnit( u1 );
	GenericFightUnit * uni2 = _base->getVisitorUnit( u2 );
	
	if( uni1 && uni2 ) {
		if( uni1->getCreature() != uni2->getCreature() ) {
			_base->setVisitorUnit( u2, uni1 );
			_base->setVisitorUnit( u1, uni2 );
		} else {
			uni2->addNumber( uni1->getNumber() );
			_base->setVisitorUnit( u1, 0 );
			delete uni1;
		}
	} else {
		_base->setVisitorUnit( u2, uni1 );
		_base->setVisitorUnit( u1, uni2 );	
	}
	GenericLord * visitor = _base->getVisitorLord();
	_socket->sendExchangeUnit( visitor, u1, visitor, u2 );
	
	_visitor->reinit();
}

void BaseLords::slot_unitGarrison( int num )
{
	unselectPhoto();
	
	if( _isExchange ) {
		if( _unitGarrison != -1 ) {
		} else {
		}
	} else {
		if( ( _unitGarrison != -1 ) || ( _unitVisitor != -1 ) ) {
			if( _unitGarrison != -1 ) {
				exchangeUnitsGG( _unitGarrison, num );
			} else {
				_unitGarrison = num;
				exchangeUnitsVG();
			}
			unselectUnits();
		} else {
			if( _base->getGarrisonUnit( num ) != 0 ) {
				_unitGarrison = num;
				_garrison->selectUnit( _unitGarrison );
				emit sig_exchange( true );
			}
		}
	}
}

void BaseLords::slot_unitVisitor( int num )
{
	unselectPhoto();
	
	if( _isExchange ) {
		
	} else {
		if( ( _unitGarrison != -1 ) || ( _unitVisitor != -1 ) ) {
			if( _unitVisitor != -1 ) {
				exchangeUnitsVV( _unitVisitor, num );
			} else {
				_unitVisitor = num;
				exchangeUnitsGV();
			}
			unselectUnits();
		} else {
			if( _base->getVisitorUnit( num ) != 0 ) {
				_unitVisitor = num;
				_visitor->selectUnit( _unitVisitor );
				emit sig_exchange( true );
			}
		}
	}
}

void BaseLords::unselectUnits()
{
	_unitGarrison = -1;
	_garrison->unselectUnits();
	_unitVisitor = -1;
	_visitor->unselectUnits();
	emit sig_exchange( false );
}

void BaseLords::unselectPhoto()
{
	_garrisonSelected = false;
	_garrison->unselectPhoto();
	_visitorSelected = false;
	_visitor->unselectPhoto();
}

BaseButtons::BaseButtons( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_pbExch = new QPushButton( this );
	_pbExch->setFixedSize( 50, 40 );
	layout->addWidget( _pbExch );
	
	layout->addStretch( 1 );
	
	QPushButton * pbOk = createButtonOk( this );
	layout->addWidget( pbOk );
		
	layout->activate();
	setFixedHeight( 240 );	
	
	_pbExch->setEnabled( false );
	
	connect( _pbExch, SIGNAL( clicked() ), SIGNAL( sig_exchange() ) );
	connect( pbOk, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
}

void BaseButtons::slot_exchange( bool st )
{
	if( st ) {
		_pbExch->setEnabled( true );
	} else {
		_pbExch->setEnabled( false );
	}
}

//
// ----- UnitSummary -----
//

UnitSummary::UnitSummary( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_ico = new QLabel( this );
	_ico->setFixedSize( 50, 40 );
	_nb = new QLabel( this );
	_nb->setFixedSize( 50, 20 );
	_nb->setAlignment( AlignCenter );
	_nb->move( 0, 40 );

	setFixedSize( 50, 60 );
}

void UnitSummary::setCreature( int id )
{

}

void UnitSummary::setNumber( int num )
{
	if( num == 0 ) {
		_nb->setText( "" );
	} else {
		_nb->setText( "+" + QString::number( num ) );
	}
}

//
// ----- BaseTroop -----
//

BaseTroop::BaseTroop( bool isGarrison, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_isGarrison = isGarrison;
	_lord = 0;

	QHBoxLayout * layout = new QHBoxLayout( this );

	_photo = new Icon( this );
	layout->addWidget( _photo );
	layout->addStretch( 1 );

	QSignalMapper * sigmap = new QSignalMapper( this );
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i] = new Icon( this );
		layout->addWidget( _units[i] );
		sigmap->setMapping( _units[i], i );
		connect( _units[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}
	layout->addStretch( 1 );

	layout->activate();

	connect( _photo, SIGNAL( sig_clicked() ), SIGNAL( sig_photo() ) );
	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_unit( int ) ) );
}

void BaseTroop::clear()
{
	_lord = 0;
	_photo->clear();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i]->clear();
	}
}

void BaseTroop::setLord( GenericLord * lord )
{
	_lord = lord;
	reinit();
}

void BaseTroop::setBase( GenericBase * base )
{
	_base = base;
	if( _isGarrison ) {
		_lord = base->getGarrisonLord();
	} else {
		_lord = base->getVisitorLord();
	}
	reinit();
}

void BaseTroop::reinit()
{
	if( _lord == 0 ) {
		clear();
		if( _isGarrison ) {
			for( int i = 0; i < MAX_UNIT; i++ ) {
				if( _base->getUnit( i ) ) {
					_units[i]->setPixmap( QPixmap( ImageTheme.getPhotoCreature( _base->getUnit( i ) ) ) );
				}
			}
		}
		return;
	}
	
	_photo->setPixmap( *_player->getLordPixmapById( _lord->getId() ) );
	
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _lord->getUnit( i ) ) {
			_units[i]->setPixmap( QPixmap( ImageTheme.getPhotoCreature( _lord->getUnit( i ) ) ) );
		} else {
			_units[i]->clear();
		}
	}	
}






