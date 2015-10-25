/****************************************************************
**
** Attal : Lords of Doom
**
** game.cpp
** Manages the whole game
**
** Version : $Id: game.cpp,v 1.129 2003/05/08 10:59:22 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 17/08/2000
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

#include "game.h"

// generic include files
#include <assert.h>
// include files for QT
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qstack.h>
#include <qtabwidget.h>
// application specific includes
#include "conf.h"
#include "common/attalSocket.h"
#include "common/calendar.h"
#include "common/dataTheme.h"
#include "common/genericInsideBuilding.h"
#include "common/pathFinder.h"
#include "common/technic.h"
#include "common/unit.h"

#include "client/building.h"
#include "client/chatWidget.h"
#include "client/displayLord.h"
#include "client/event.h"
#include "client/fightUnit.h"
#include "client/gainLevel.h"
#include "client/gameInfo.h"
#include "client/graphicalArtefact.h"
#include "client/gui.h"
#include "client/imageTheme.h"
#include "client/lord.h"
#include "client/lordExchange.h"
#include "client/mapCreature.h"
#include "client/mapView.h"
#include "client/ressourceBar.h"


extern QString DATA_PATH;
extern QString IMAGE_PATH;
extern TechnicList techList;
extern DataTheme DataTheme;
extern ImageTheme ImageTheme;

/** add comments here */
Game::Game( QWidget * parent , const char * name )
  : QWidget( parent, name, Qt::WStyle_Customize | Qt::WStyle_DialogBorder ),
	  GameDescription()
{
	_map = new Map( this );
	_isPlaying = false;
	_socket = 0;
	_dispLord = 0;
	_lordExchange = 0;

	_player = new Player( this, (GenericMap *)_map );

	QHBoxLayout * lay = new QHBoxLayout( this );

	QVBoxLayout * vlayout1 = new QVBoxLayout();
	MapView * view = new MapView( _map, this );

	_ressourceBar = new RessourceBar( this );
	_ressourceBar->setPlayer( _player );

	QTabWidget * tab = new QTabWidget( this );
	_chat = new ChatWidget( this );
	tab->insertTab( _chat, "Chat" );
//	tab->insertTab( _ressourceBar, "Ressource" );

	_map->resize( 4000, 3000 );
	vlayout1->addWidget( view, 1 );
	vlayout1->addWidget( tab );
	vlayout1->addWidget( _ressourceBar );

	QVBoxLayout * vlayout2 = new QVBoxLayout();

	_miniMap = new MiniMap( this );

	_gameInfo = new GameInfo( _calendar, this );
	
	_control = new GameControl( this );
	_control->setPlayer( _player );

	vlayout2->addWidget( _miniMap, 100 );
	vlayout2->addStretch( 1 );
	vlayout2->addWidget( _control );
	vlayout2->addStretch( 1 );
	vlayout2->addWidget( _gameInfo );


	lay->addLayout( vlayout1, 1 );
	lay->addLayout( vlayout2 );
    	lay->activate();

	initLords();

	_control->disableGame();
	_control->reinit();

	connect( _chat, SIGNAL( sig_message( QString ) ), SLOT( slot_message( QString ) ) );
	connect( _control, SIGNAL( sig_endTurn() ), SLOT( slot_endTurn() ) );
	connect( _control, SIGNAL( sig_lord() ), SLOT( slot_displayLord() ) );
	connect( _control, SIGNAL( sig_base() ), SLOT( slot_displayBase() ) );
	connect( view , SIGNAL( sig_mouseMoved  ( Cell * ) ), SLOT( slot_mouseMoved  ( Cell * ) ) );
	connect( view , SIGNAL( sig_mouseLeftPressed( Cell * ) ), SLOT( slot_mouseLeftPressed( Cell * ) ) );
	connect( view , SIGNAL( sig_mouseRightPressed( Cell * ) ), SLOT( slot_mouseRightPressed( Cell * ) ) );
}

Game::~Game()
{
		delete _control;
		delete _gameInfo;
		delete _miniMap;
		delete _chat;
		delete _player;
		delete _map;
}

/// XXX: usefull ??
void Game::setPlayerName( QString name ) {
	_player->setName( name );
}

void Game::slot_mouseMoved( Cell *cell )
{
	if( cell->getLord() ) {
		setCursor( waitCursor );
	} else {
		setCursor( arrowCursor );
	}
}


void Game::slot_mouseLeftPressed( Cell * cell )
{
	if( !_player ) {
		return;
	}

	switch( _state ) {
	case MS_NOTHING:
		handleClickNothing( cell );
		break;
	case MS_LORD:
		handleClickLord( cell );
		break;
	case MS_BASE:
		handleClickBase( cell );
		break;
	case MS_TECHNIC:
		handleClickTechnic( cell );
		break;
	}
}

void Game::handleClickNothing( Cell * cell )
{
	GenericLord * lord = cell->getLord();
	
	if( lord ) {
		if( _player->hasLord( lord ) ) {
			_player->setSelectedLord( lord );
			_control->reinit();
		}
		return;
	}
	
	GenericBase * base = cell->getBase();
		
	if( base ) {
		if( _player->hasBase( base ) ) {
			_player->setSelectedBase( base );
			_control->reinit();
		}
		return;
	}
}

void Game::handleClickLord( Cell * cell )
{
	GenericLord * selectedLord = _player->getSelectedLord();
		
	if( selectedLord && ( cell->getCoeff() >= 0 ) ) {
		if( ((Lord*)selectedLord)->getDestination() != (GenericCell*)cell ) {
			((Lord*)selectedLord)->computePath( (GenericCell*)cell );
		} else {
			if( selectedLord->getCell()->getBuilding() ) {
				selectedLord->getCell()->getBuilding()->out( selectedLord );
			}
			((Lord*)selectedLord)->followPath( _socket );
			((Lord*)selectedLord)->setColor( QColor( 255, 0, 0 ) );
		}
	}		
}

void Game::handleClickBase( Cell * cell )
{
	GenericBase * base = cell->getBase();
		
	if( base ) {
		if( _player->hasBase( base ) ) {
			if( base == _player->getSelectedBase() ) {
				emit sig_base( base );
				return;
			} else {
				_player->setSelectedBase( base );
				_control->reinit();
				return;
			}
		} else {
			return;
		}
	}	
	
	handleClickNothing( cell );
}

void Game::handleClickTechnic( Cell * cell )
{
		
}


void Game::slot_mouseRightPressed( Cell * cell ) 
{
	if( !_player ) {
		return;
	}

	GenericLord * lord = cell->getLord();
	
	if( lord ) {
	}
}
void Game::enter( GenericLord * lord, GenericBuilding * building )
{
	
}

void Game::enter( GenericLord * lord, GenericBase * base )
{
	base->enter( lord );
	emit sig_base( base );
}

void Game::beginTurn()
{
	_isPlaying = true;
	_calendar->newDay();
	_gameInfo->nothing();
	if( _calendar->getDay() == 1 ) {
		_player->newWeek();
	}
	_player->newTurn();
	_state = MS_NOTHING;
}

void Game::playerActive( char num )
{
	_gameInfo->waitPlayer( getPlayerColor( num ) );
}

void Game::endTurn()
{
	assert( _socket );
	if( _isPlaying ) {
		if( _player->shouldEnd() ) {
			_socket->sendTurnEnd();
			_isPlaying = false;
		} else {
			QMessageBox msb( "Are you sure ?", "One or more heroes may still move. Are you sure you want to end your turn ?", QMessageBox::Warning, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
			if ( msb.exec() == QMessageBox::Yes ) {
				_socket->sendTurnEnd();
				_isPlaying = false;
			}
		}
	}
}

void Game::beginGame( int nb )
{
	setPlayerNumber( nb );
	_control->enableGame();
}

void Game::endGame()
{
	uint i, j;
	_control->disableGame();


	for( i = 0; i < (uint) _map->getHeight(); i++ ) {
		for( j = 0; j < (uint) _map->getWidth(); j++ ) {
			if( _map->at( i, j )->getLord() ) {
				Lord * lord = (Lord*)_map->at( i, j )->getLord();
				lord->cleanPath();
				delete lord;
			}
			if( _map->at(i, j)->getBuilding() ) {
				Building * building = (Building*)_map->at( i, j )->getBuilding();
				delete building;
			}
			if( _map->at(i, j)->getBase() )	{
				Base * base = (Base*)_map->at( i, j )->getBase();
				delete base;
			}
			if( _map->at(i, j)->getArtefact() ) {
				Artefact * artef = (Artefact*)_map->at( i, j )->getArtefact();
				delete artef;
			}
			if( _map->at(i, j)->getCreature() ) {
				MapCreature * crea =(MapCreature*)_map->at( i, j )->getCreature();
				delete crea;
			}
			if( _map->at(i, j)->getEvent() ) {
				Event * event =(Event*)_map->at( i, j )->getEvent();
				delete event;
			}
		}
	}
	_map->cleanData();
	_player->cleanData();
  _miniMap->redrawMap( _map );
	_calendar->reinit();
	_state=MS_NOTHING;
}

void Game::handleSocket()
{
	switch( _socket->getCla1() ) {
	case SO_MSG:
		socketMsg();
		break;
	case SO_GAME:
		socketGame();
		break;
	case SO_TURN:
		socketTurn();
		break;
	case SO_MODIF:
		socketModif();
		break;
	case SO_QR:
		socketQR();
		break;
	case SO_MVT:
		socketMvt();
		break;
	case SO_TECHNIC:
		break;
	case SO_EXCH:
		socketExchange();
		break;
	case SO_CONNECT:
		socketConnect();
		break;
	case SO_FIGHT:
		socketFight();
		break;
	default:
		logEE( "Unknown socket_class" );
	}
}

void Game::socketMsg()
{
	QString msg;
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		msg.ref(i) = _socket->readChar(); /// WINDOWS (was msg[i])
	}
	_chat->newMessage( msg );
}

void Game::slot_message( QString msg )
{
	if( _socket ) {
		_socket->sendMessage( _player->getName() + " : " + msg );
	} else {
		_chat->newMessage( "(Not connected) : " + msg );
	}
}

void Game::socketGame()
{
	if( _socket->getCla2() == C_GAME_BEG ) {
		beginGame( _socket->readChar() );
		/// XXX: clear old stuff if necessary
	} else if( _socket->getCla2() == C_GAME_LOST ) {
		int nb = _socket->readChar();
		if( nb == getPlayerNumber() ) {
			endGame();
		} else {
			QString text;
			text = "Player " + QString::number( nb ) + " has lost.";
			GameMessage msg;
			msg.setCaption( "A player has lost." );
			msg.addText( text );
			msg.addPixmap( ImageTheme.getFlag( nb ) );
			msg.exec();
		}
	} else if( _socket->getCla2() == C_GAME_END ) {
		endGame();
	}
}

void Game::socketTurn()
{
	switch( _socket->getCla2() ) {
	case C_TURN_BEG:
		beginTurn();
		/// XXX: clear old stuff if necessary
		break;
	case C_TURN_PLAY:
		playerActive( _socket->readChar() );
		break;
	case C_TURN_LORD:
		logEE( "Should not happen (Client : SO_TURN/C_TURN_LORD)" );
		break;
	case C_TURN_PLORD:
		logEE( "Should not happen (Client : SO_TURN/C_TURN_PLORD)" );
		break;
	case C_TURN_END:
		logEE( "Should not happen (Client : SO_TURN/C_TURN_END)" );
		break;
	}
}

void Game::socketMvt()
{
	// XXX: not finished at all
	uchar lord = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();

	GenericLord * theLord = (GenericLord * )_realLords[ lord ];
	if( theLord->getOwner() == _player ) {
		int cost = theLord->computeCostMvt( _map->at( row, col ) );
		if( cost >= 0 ) {
			theLord->decreaseBaseCharac( MOVE, cost );
		}
	}

	_realLords[ lord ]->moveTo( _map->at( row, col ) );
	if( _map->computeMinimalNextCost( theLord ) > theLord->getBaseCharac( MOVE ) ) {
		theLord->setBaseCharac( MOVE, 0 );
	}

	if( _map->at( row, col )->getBuilding() != 0 ) {
		if( _realLords[ lord ]->getOwner() == _player ) {
			enter( _player->getSelectedLord(), _map->at( row, col )->getBuilding() );
		}
	} else if( _map->at( row, col )->getBase() != 0 ) {
		if( _realLords[ lord ]->getOwner() == _player ) {
			enter( _player->getSelectedLord(), _map->at( row, col )->getBase() );
		}
	}
}

void Game::socketExchange()
{
	switch( _socket->getCla2() ) {
	case C_EXCH_START: {
		uchar idLord1 = _socket->readChar();
		uchar idLord2 = _socket->readChar();
		GenericLord * lord1 = _realLords[ idLord1 ];
		GenericLord * lord2 = _realLords[ idLord2 ];
		LordExchange dial;
		if( ! _lordExchange ) {
			_lordExchange = new LordExchange( this );
		}
		_lordExchange->initSocket( _socket );
		_lordExchange->initLords( lord1, lord2 );
		_lordExchange->exec();
		delete _lordExchange;
		_lordExchange = 0;
		} break;
	case C_EXCH_UNIT:
		exchangeUnits();
		break;
	case C_EXCH_BASEUNITCL:
		exchangeBaseUnits();
		break;
	default:
		break;
	}
}

void Game::exchangeUnits()
{
	uchar idLord1 = _socket->readChar();
	uchar idUnit1 = _socket->readChar();
	uchar idLord2 = _socket->readChar();
	uchar idUnit2 = _socket->readChar();
	GenericLord * lord1 = 0;
	GenericLord * lord2 = 0;
	if( idLord1 ) {
		lord1 = (GenericLord * )_realLords[ idLord1 ];
	}
	if( idLord2 ) {
		lord2 = (GenericLord * )_realLords[ idLord2 ];
	}

	/// XXX: check player of lord ?
	if( lord1 && lord2 ) {
		GenericFightUnit * unit1 = lord1->getUnit( idUnit1 );
		GenericFightUnit * unit2 = lord2->getUnit( idUnit2 );

		if( unit1 ) {
			if( unit2 ) {
				if( ( unit1->getRace() == unit2->getRace() ) &&
					unit1->getLevel() == unit2->getLevel() ) {
					unit2->addNumber( unit1->getNumber() );
					lord1->setUnit( idUnit1, 0 );
					delete unit1;
				} else {
					lord1->setUnit( idUnit1, unit2 );
					lord2->setUnit( idUnit2, unit1 );
				}
			} else {
				lord2->setUnit( idUnit2, unit1 );
				lord1->setUnit( idUnit1, 0 );
			}
		}
		if( _lordExchange ) {
			_lordExchange->reinit();
		}
	}
}

void Game::exchangeBaseUnits()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar idUnit1 = _socket->readChar();
	uchar idLord = _socket->readChar();
	uchar idUnit2 = _socket->readChar();
	GenericBase * base = 0;
	GenericLord * lord = 0;
	GenericFightUnit * uni1 = 0;
	GenericFightUnit * uni2 = 0;

		base = (GenericBase *) _map->at( row, col )->getBase();
		
	if( idLord && idLord<255) {
		lord = (GenericLord * )_realLords[ idLord ];
	}
	
	
	if(base){
			if(idUnit1<=MAX_UNIT){
				uni1 = base->getUnit( idUnit1);
			}
		if(lord){
			if(idUnit2<=MAX_UNIT){
				uni2 = lord->getUnit( idUnit2 );
			}
			if( uni1 && uni2 ) {
				if( uni1->getCreature() != uni2->getCreature() ) {
					lord->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni1->addNumber( uni2->getNumber() );
					lord->setUnit( idUnit2, 0 );
					delete uni2;
				}
			} else if (!uni1) {
				lord->setUnit( idUnit2, 0 );
				base->setUnit( idUnit1, uni2 );
			} else if (!uni2) {
				lord->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		} else {
			if(idUnit2<=MAX_UNIT){
				uni2 = base->getUnit( idUnit2);
			}
		if( uni1 && uni2 ) {
			if( uni1->getCreature() != uni2->getCreature() ) {
					base->setUnit( idUnit2, uni1 );
					base->setUnit( idUnit1, uni2 );
				} else {
					uni1->addNumber( uni2->getNumber() );
					base->setUnit( idUnit2, 0 );
					delete uni2;
				}
			} else {
				base->setUnit( idUnit2, uni1 );
				base->setUnit( idUnit1, 0 );
			}
		}	
	}
}



void Game::socketModif()
{
	/// XXX: check number of args below... (?)

	switch( _socket->getCla2() ) {
	case C_MOD_MAP:{
		int h = _socket->readInt();
		int w = _socket->readInt();
		//logDD("h/w %d/%d", h, w );
		_map->newUnknownMap( h, w );
}		break;
	case C_MOD_CELL:{
		int a1 = _socket->readInt();
		int a2 = _socket->readInt();
		int a3 = _socket->readInt();
		int a4 = _socket->readInt();
		int a5 = _socket->readInt();
		int a6 = _socket->readInt();
		//logDD("chg cell %d %d %d %d %d %d", a1, a2, a3, a4, a5, a6 );
		_map->changeCell( a1, a2, a3, a4, a5, a6 );
		_miniMap->redrawCell( _map, a1, a2 );
	}
		break;
	case C_MOD_LORD:
		socketModifLord();
		break;
	case C_MOD_PLAYER:
		socketModifPlayer();
		break;
	case C_MOD_BASE:
		socketModifBase();
		break;
	case C_MOD_BUILD:
		socketModifBuilding();
		break;
	case C_MOD_ARTEFACT:
		socketModifArtefact();
		break;
	case C_MOD_CREATURE:
		socketModifCreature();
		break;
	case C_MOD_EVENT:
		socketModifEvent();
		break;
	}
}

void Game::socketModifLord()
{
	switch( _socket->getCla3() ) {
	case C_LORD_VISIT:
		socketModifLordVisit();
		break;
	case C_LORD_NEW:
		socketModifLordNew();
		break;
	case C_LORD_MOVE: 
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_realLords[ lord ]->setBaseCharac( MOVE, nb );
			break;
		}
	case C_LORD_MAXMOVE:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_realLords[ lord ]->setBaseCharac( MAXMOVE, nb );
			break;
		}
	case C_LORD_SP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_realLords[ lord ]->setBaseCharac( TECHNICPOINT, nb );
			break;
		}
	case C_LORD_MAXSP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_realLords[ lord ]->setBaseCharac( MAXTECHNICPOINT, nb );
			break;
		}
	case C_LORD_MORALE:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( MORALE, nb );
			break;
		}
	case C_LORD_LUCK:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( LUCK, nb );
			break;
		}
	case C_LORD_EXP:
		{
			char lord = _socket->readChar();
			int nb = _socket->readInt();
			_realLords[ lord ]->setBaseCharac( EXPERIENCE, nb );
			break;
		}
	case C_LORD_ATT:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( ATTACK, nb );
			break;
		}
	case C_LORD_DEF:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( DEFENSE, nb );
			break;
		}
	case C_LORD_POW:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( POWER, nb );
			break;
		}
	case C_LORD_KNOW:
		{
			char lord = _socket->readChar();
			char nb = _socket->readChar();
			_realLords[ lord ]->setBaseCharac( KNOWLEDGE, nb );
		}
		break;
	case C_LORD_UNIT:
		socketModifLordUnit();
		break;
	case C_LORD_REMOVE: {
		int idLord = _socket->readChar();
		Lord * lord = _realLords[ idLord ];
		lord->getCell()->setLord( 0 );
		/// XXX: in fact, _realLords may be different ?
		_realLords[ idLord ] = (Lord *)DataTheme.lords.at( idLord );
		GenericPlayer * player = lord->getOwner();
		if( player ) {
			player->removeLord( (GenericLord *)lord );
		}
		_control->reinit();
		//XXX: Remove lord from map
		lord->cleanPath();
		delete lord;
		//if( lord ) lord->getOwner()->removeLord( lord );
	}
	break;
	case C_LORD_GARRISON:{
		uchar lord = _socket->readChar();
		uchar state = _socket->readChar();
		_realLords[ lord ]->setVisible( state != 1 );
		_control->reinit();
		} break;
	case C_LORD_MACHINE:{
		uchar lord = _socket->readChar();
		uchar id = _socket->readChar();
		_realLords[ lord ]->addMachine( id );
		} break;
	}
}

void Game::socketModifLordVisit()
{
	uchar num = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();

	/// XXX: memory leak ??
	Lord * lord = new Lord( _map );
	lord->setAnimated( true );
	lord->setEnabled( true );
	lord->setSelected( true );
	lord->setActive( true );
	lord->setId( id );
	/// XXX: change
	lord->setOwner( 0 );
	lord->moveTo( _map->at( row, col ) );
	_realLords[ id ] = lord;
}

void Game::socketModifLordNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();

	Lord * lord = new Lord( _map );
	lord->setAnimated( true );
	lord->setEnabled( true );
	lord->setSelected( true );
	lord->setActive( true );
	lord->setId( id );
	lord->moveTo( _map->at( row, col ) );
	lord->setOwner( _player );
	_realLords[ id ] = lord;
	_player->addLord( lord );
	_control->reinit();
}

void Game::socketModifLordUnit()
{
	uchar id = _socket->readChar();
	uchar num = _socket->readChar();
	GenericFightUnit * uni = new GenericFightUnit();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	uni->setCreature( race, level );
	int nb = _socket->readInt();
	uni->setNumber( nb );
	uni->setMove( _socket->readChar() );
	uni->setHealth( _socket->readInt() );
	_realLords[ id ]->setUnit( num, uni );
}

void Game::socketModifPlayer()
{
	switch( _socket->getCla3() ) {
	case C_PLAY_RESS: {
		uchar ress = _socket->readChar();
		_player->setResource( ress, _socket->readInt() );
		_ressourceBar->reinit();}
		break;
	}
}

void Game::socketModifBase()
{
	switch( _socket->getCla3() ) {
	case C_BASE_NEW: {
		uchar race = _socket->readChar();
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar id = _socket->readChar();
		int nb = _socket->readChar();

		Base * base = (Base*)_map->at( row, col )->getBase();
		if( !base ) {
			base = new Base( _map );
			base->setPosition( row, col );
			base->setRace( race );
			_map->computeStoppable( (GenericBase *)base );
			base->setId( id );/// XXX: to inv ??
			for( int i = 0; i < nb; i++ ) {
				base->addForbiddenBuilding( _socket->readChar() );
			}
			base->show();
		}
		/// XXX: update base if already created...
	}
	break;
	case C_BASE_OWNER: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		Base * base = (Base*)_map->at( row, col )->getBase();
		if( _socket->readChar() == _player->getNum() ) {
			_player->addBase( base );
			base->setOwner( _player );
		} else {
			/// XXX: improve management of base of other player
			if( base->getOwner() == _player ) {
				base->setOwner( 0 );
				_player->removeBase( (GenericBase*)base );
			}
		}
		_control->reinit();
	}
	break;
	case C_BASE_BUILDING:
	{
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar level = _socket->readChar();
		bool create = (bool)_socket->readChar();
		if( _map->at( row, col )->getBase() ) {
			GenericInsideBuilding * building = new GenericInsideBuilding();
			building->setRace( _map->at( row, col )->getBase()->getRace() );
			building->setLevel( level );
			if( create ) {
				_map->at( row, col )->getBase()->addBuilding( building );
			} else {
				logDD("remove game");
				_map->at( row, col )->getBase()->removeBuilding( building );
			}
		} else {
			logEE( "Base not found" );
		}
	}
	break;
	case C_BASE_UNIT: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		if( _map->at( row, col )->getBase() ) {
			Base * base = (Base *)_map->at( row, col )->getBase();
			uchar race = _socket->readChar();
			uchar level = _socket->readChar();
			int number = _socket->readInt();
			Creature * creature = DataTheme.creatures.at( race, level );
			base->addGarrison( creature, number );
		}
		} break;
	}
}

void Game::socketModifBuilding()
{
	switch( _socket->getCla3() ) {
	case C_BUILD_NEW: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar type = _socket->readChar();

		if( _map->at( row, col )->getBuilding() == 0 ) {
			Building * building = new Building( _map );
			building->setPosition( _map->at( row, col ) );
			building->setType( type );
			building->show();
		} // XXX: update building if already created ?
	}	
	break;
	case C_BUILD_OWNER: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		if( _socket->readChar() == _player->getNum() ) {
			_player->addBuilding( _map->at( row, col )->getBuilding() );
		}
	}
	}	
}

void Game::socketModifArtefact()
{
	switch( _socket->getCla3() ) {
		case C_ART_NEWMAP: {
				Artefact * artef = new Artefact( (QCanvas *)_map );
				artef->setId( _socket->readInt() );
				artef->setType( _socket->readChar() );
				int row = _socket->readInt();
				int col = _socket->readInt();
				artef->setCell( _map->at( row, col ) );
				_map->at( row, col )->setArtefact( artef );
				_artefacts.append( artef );
			}
			break;
		case C_ART_NEWLORD: {
				Artefact * artef = new Artefact( (QCanvas *)_map );
				artef->setId( _socket->readInt() );
				artef->setType( _socket->readChar() );
				artef->setPosition( 0 );
				char lord = _socket->readChar();
				artef->setLord( getLord( lord ) );
				getLord( lord )->addArtefact( artef );
			}
			break;
		case C_ART_DELMAP: {
				int id = _socket->readInt();
				if( getArtefactById( id )->isOnMap() ) {
					getArtefactById( id )->getCell()->setArtefact( 0 );
				}
				getArtefactById( id )->setLord( 0 );
			}
			break;
		case C_ART_DELLORD: {
				int id = _socket->readInt();
				char lord = _socket->readChar();
				getLord( lord )->removeArtefact( getArtefactById( id ) );
			}
			break;
		case C_ART_ADDLORD: {
				int id = _socket->readInt();
				char lord = _socket->readChar();
				getLord( lord )->addArtefact( getArtefactById( id ) );
				getArtefactById( id )->setLord( getLord( lord ) );
			}
			break;
	}
}

void Game::socketModifCreature()
{
	switch( _socket->getCla3() ) {
	case C_CRE_NEW: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar race = _socket->readChar();
		uchar level = _socket->readChar();
		int nb = _socket->readInt();
		MapCreature * creature = new MapCreature( _map );
		creature->setCreature( race, level );
		creature->setCategoryNumber( nb );
		creature->setCell( _map->at( row, col ) );
		_map->at( row, col )->setCreature( creature );
		}
		break;
	case C_CRE_DEL: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		GenericMapCreature * crea = _map->at( row, col )->getCreature();
		if( crea ) {
			delete crea;
			_map->at( row, col )->setCreature( 0 );
		}
		}
		break;
	}
}

void Game::socketModifEvent()
{
	switch( _socket->getCla3() ) {
	case C_EVENT_NEW: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		uchar type = _socket->readChar();
		Event * event = new Event( _map );
		event->setCell( _map->at( row, col ) );
		event->setType( (GenericEvent::EventType)type );
		_map->at( row, col )->setEvent( (GenericEvent*)event );
		}
		break;
	case C_EVENT_DEL: {
		int row = _socket->readInt();
		int col = _socket->readInt();
		GenericEvent * event = _map->at( row, col )->getEvent();
		if( event ) {
			delete event;
			_map->at( row, col )->setEvent( 0 );
		}
		}
		break;
	}
}

void Game::socketConnect()
{
	uchar tmp;

	switch( _socket->getCla2() ) {
	case C_CONN_OK:
		_socket->sendConnectionName( _player->getName() );
		_chat->newMessage( QString( "Connection established, %1" ).arg( _player->getName() ) );
		break;
	case C_CONN_ID:
		{
			tmp=_socket->readChar();
			_player->setNum( tmp );
			_chat->newMessage( QString( "Connection established, %1" ).arg( _player->getNum() ) );
		}
		break;
	case C_CONN_NAME:
		_player->setName( "Player" );
		break;
	case C_CONN_PLAYER:
		break;
	}
}

/*!

*/

void Game::slot_displayLord()
{
	if( _player->numLord() > 0 ) {
		if( _dispLord == 0 ) {
			_dispLord = new DisplayLord( _player, this );
		}
		_dispLord->show();
	}
}

void Game::slot_displayBase()
{
	emit sig_base( _player->getSelectedBase() );
}

void Game::socketQR()
{
	switch( _socket->getCla2() ) {
	case C_QR_NONE:
		//QMessageBox::information( this, "Information", QString( buf+(3*sizeof(char)) ), 0 );
		QMessageBox::information( this, "Information", "Error in (C_QR_NONE)" );
		break;
	case C_QR_LEVEL: {
		GainLevel * level = new GainLevel( this );
		level->reinit();
		level->exec();
		_socket->sendAnswer( level->getChoice() );
		} break;
	case C_QR_ANSWER:
		logEE( "Should not happen" );
		break;
	}
}
void Game::socketFight()
{
	switch( _socket->getCla2() ) {
	case C_FIGHT_INIT: {
		uchar cla = _socket->readChar();
		uchar lord = _socket->readChar();
		emit sig_fight( getLord( lord ), (CLASS_FIGHTER)cla );
	}
	break;
	case C_FIGHT_LORD:
		logEE( "Should not happen (FIGHT_LORD)" );
		break;
	case C_FIGHT_UNIT:
		logEE( "Should not happen (FIGHT_UNIT)" );
		break;
	case C_FIGHT_END:
		logEE( "Should not happen (FIGHT_END)" );
		break;
	default:
		logEE( "Should not happen (FIGHT)" );
		break;
	}
}

void Game::displayMiniMap( bool state )
{
	if( _miniMap ) {
		if( state ) {
			_miniMap->show();
			_miniMap->redrawMap( _map );
		} else {
			_miniMap->hide();
		}
	}
}

//
// ----- GameMessage -----
//

GameMessage::GameMessage( QWidget * parent, const char * name )
	:QDialog( parent, name, true )
{
	_layout = new QVBoxLayout( this );
	_layout->addStretch( 1 );

	_layout->activate();
}

void GameMessage::addText( QString text )
{
	QLabel * label = new QLabel( this );
	label->setText( text );
	FIXEDSIZE( label );
	_layout->addWidget( label );
	_layout->addStretch( 1 );
}

void GameMessage::addPixmap( QPixmap * pixmap )
{
	QLabel * label = new QLabel( this );
	label->setPixmap( * pixmap );
	FIXEDSIZE( label );
	_layout->addWidget( label );
	_layout->addStretch( 1 );
}












