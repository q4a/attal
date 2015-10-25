/****************************************************************
**
** Attal : Lords of Doom
**
** fight.cpp
** Manages the whole fight
**
** Version : $Id: fight.cpp,v 1.36 2003/02/13 20:09:13 lusum Exp $
**
** Author(s) : Cyrille Verrier, Pascal Audoux
**
** Date : 07/12/2000
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

#include "fight.h"

#include <assert.h>
// include files for QT
#include <qlayout.h>
#include <qlabel.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qmultilinedit.h>
// application specific include
#include "conf.h"
#include "common/attalSocket.h"
#include "common/unit.h"
#include "common/pathFinder.h"
#include "common/attalSocket.h"
#include "client/chatWidget.h"
#include "client/fightMapView.h"
#include "client/fightResult.h"
#include "client/game.h"
#include "client/lord.h"
#include "client/widget.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;

/** add comments here */
Fight::Fight( QWidget * parent , const char * name )
	: QWidget( parent, name )
{
	_socket = 0;
	_game = 0;
	_popup = 0;
	_isAttack = true;
	_isActive = false;
	_isCreature = false;
	_activeUnit = 0;
	_lostAttack = new QList<GenericFightUnit>;
	_lostAttack->setAutoDelete( true );
	_lostDefense = new QList<GenericFightUnit>;
	_lostDefense->setAutoDelete( true );

	for( int i = 0; i < MAX_UNIT; i ++ ) {
		_unitsAtt[i] = 0;
		_unitsDef[i] = 0;
	}

	//setBackgroundPixmap( IMAGE_PATH + "fight/background_0.png" );

	QVBoxLayout * layout = new QVBoxLayout( this );
	//layout->addSpacing( 120 );

	_map = new FightMap( this );
	_map->setBackgroundPixmap( QPixmap( IMAGE_PATH + "fight/background_0.png" ) );


	_view = new FightMapView( _map, this );
	layout->addWidget( _view, 1 );

	_control = new FightControl( this );
	layout->addWidget( _control );

	_map->resize( 800, 500 );


	layout->activate();

	connect( _control, SIGNAL( sig_wait( ) ), SLOT( slot_wait( ) ) );
	connect( _control, SIGNAL( sig_retreat( ) ), SLOT( slot_flee( ) ) );
	connect( _control, SIGNAL( sig_defend() ), SLOT( slot_defend() ) );

	connect( _view, SIGNAL( sig_mouseMoved( FightCell * ) ), SLOT( slot_mouseMoved( FightCell * ) ) );
	connect( _view, SIGNAL( sig_mouseRightPressed( FightCell * ) ), SLOT( slot_mouseRightPressed( FightCell * ) ) );
	connect( _view, SIGNAL( sig_mouseLeftPressed( FightCell * ) ), SLOT( slot_mouseLeftPressed( FightCell * ) ) );
	connect( _view, SIGNAL( sig_mouseReleased() ), SLOT( slot_mouseReleased() ) );
}

Fight::~Fight()
{
	logDD("delete Fight");
	delete _lostAttack;
	delete _lostDefense;
}

void Fight::reinit()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _unitsAtt[i] ) {
			delete _unitsAtt[i];
			_unitsAtt[i] = 0;
		}
		if( _unitsDef[i] ) {
			delete _unitsDef[i];
			_unitsDef[i] = 0;
		}
	}
	_lostAttack->clear();
	_lostDefense->clear();
	_lordAtt = 0;
	_lordDef = 0;
	_isAttack = true;
	_isActive = false;
	_activeUnit = 0;
	_map->reinit();
	_control->clear();
}

void Fight::setLord( GenericLord * lord, CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		_isAttack = true;
		_lordAtt = lord;
	} else {
		_isAttack = false;
		_lordDef = lord;
	}
	for( int i = 0; i < MAX_UNIT; i++ ) {
		setUnit( lord->getUnit( i ), fighter, i );
	}
}

void Fight::setOpponent( GenericLord * lord )
{
	if( _isAttack ) {
		_lordDef = lord;
	} else {
		_lordAtt = lord;
	}
}

GenericFightUnit * Fight::getUnit( int num, CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		return _unitsAtt[num];
	} else {
		return _unitsDef[num];
	}
}

void Fight::setUnit( GenericFightUnit * unit, CLASS_FIGHTER fighter, int num )
{
	if( unit ) {
		FightUnit * currentUnit;

		currentUnit = new FightUnit( _map, unit, ( fighter == FIGHTER_ATTACK ) ? true : false );

		if( fighter == FIGHTER_ATTACK ) {
			if( _unitsAtt[num] ) {
				delete _unitsAtt[num];
			}
			_unitsAtt[num] = currentUnit;
			currentUnit->goTo( _map->at( num, 1 ) );
		} else if( fighter == FIGHTER_DEFENSE ) {
			if( _unitsDef[num] ) {
				delete _unitsDef[num];
			}
			_unitsDef[num] = currentUnit;
			currentUnit->goTo( _map->at( num, 4 ) );
		}
	} else {
		if( ( fighter == FIGHTER_ATTACK ) && ( _unitsAtt[num] ) ) {
			delete _unitsAtt[num];
			_unitsAtt[num] = 0;
		} else if( ( fighter == FIGHTER_DEFENSE ) && _unitsDef[num] ) {
			delete _unitsDef[num];
			_unitsDef[num] = 0;
		}
	}
}


void Fight::slot_mouseMoved( FightCell * cell )
{
	/// XXX: not finished
	//logDD("%i : %i",cell->getRow(),cell->getCol() );
	if( _activeUnit && _isActive ) {
		if( cell->getAccess() == FAR_OCCUPIED ) {

		}
	}
}

void Fight::slot_mouseLeftPressed( FightCell * cell )
{
	if( _activeUnit && _isActive ) {
		switch( cell->getAccess() ) {
		case NEAR:
			moveUnit( cell );
			break;
		case NEAR_OCCUPIED:
			moveUnit( cell );
			break;
		case FAR_OCCUPIED:
			if( ( _activeUnit->getDistAttack() > 0 ) && ( isOpponent( cell->getUnit() ) ) ) {
				_socket->sendFightDistAttack( giveClass( cell->getUnit() ), giveNum( cell->getUnit() ) );
				_socket->sendFightUnitEndMove();
			}
			break;
		default:
			break;
		}
	}
}

void Fight::slot_mouseRightPressed( FightCell * cell )
{
	if( _popup == 0 ) {
		_popup = new PopupUnit( this );
	}

	GenericFightUnit * unit = cell->getUnit();
	if( unit && ( unit->getNumber() > 0 ) ) {
		_popup->setUnit( unit );
		_popup->show();
		_popup->move( int(cell->x()) + 40, int(cell->y()) + 40 );
	}
}

void Fight::slot_mouseReleased()
{
	if( _popup ) {
		_popup->hide();
	}
}

void Fight::slot_wait( void )
{
	_socket->sendFightWait();
	//_activeUnit->setActive( false );
	//_map->clearPath();
}

void Fight::slot_flee( void )
{
	_socket->sendFightFlee();
}

void Fight::slot_defend()
{
	_socket->sendFightDefend();
}

void Fight::handleSocket()
{
	switch( _socket->getCla1() ) {
	case SO_MSG:
		socketMsg();
		break;
	case SO_GAME:
		break;
	case SO_TURN:
		break;
	case SO_MODIF:
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
		socketFight();
		break;
	default:
		logEE( "Unknown socket_class" );
	}
}

void Fight::socketMsg()
{
	QString msg;
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		msg.at(i) = _socket->readChar();
	}
	_control->newMessage( msg );
}




void Fight::socketFight()
{
	switch( _socket->getCla2() ) {
	case C_FIGHT_INIT:
		logEE( "Should not happen (C_FIGHT_INIT in Fight)" );
		break;
	case C_FIGHT_CREATURE:
		_isCreature = true;
		setOpponent( _game->getLord( 0 ) );
		break;
	case C_FIGHT_LORD: {
		GenericLord * lord = _game->getLord( _socket->readChar() );
		lord->setBaseCharac( ATTACK, _socket->readChar() );
		lord->setBaseCharac( DEFENSE, _socket->readChar() );
		setOpponent( lord );
		} break;
	case C_FIGHT_CELL:
		socketFightCell();
		break;
	case C_FIGHT_UNIT: {
		uchar cla = _socket->readChar();
		uchar num = _socket->readChar();
		GenericFightUnit * unit = new GenericFightUnit();
		uchar race = _socket->readChar();
		uchar level = _socket->readChar();
		unit->setCreature( race, level );
		unit->setNumber( _socket->readInt() );
		unit->setMove( _socket->readChar() );
		unit->setHealth( _socket->readInt() );
		setUnit( unit, (CLASS_FIGHTER)cla, num );
		} break;
	case C_FIGHT_MODUNIT:
		socketFightModify();
		break;
	case C_FIGHT_MOVE:
		socketFightMove();
		break;
	case C_FIGHT_ENDMOVE:
		_isActive = false;
		_map->clearPath();
		break;
	case C_FIGHT_ACTIVE:{
		uchar cla = _socket->readChar();
		uchar num = _socket->readChar();
		setActive( (CLASS_FIGHTER)cla, num );
	}
		break;
	case C_FIGHT_DAMAGE:
		socketFightDamage();
		break;
	case C_FIGHT_END: {
		updateUnits();
		showFightResults( _socket->readChar() );
		emit sig_quit();
		}
		break;
	}
}

void Fight::updateUnits( void )
{
	GenericLord *oldLord;
	GenericFightUnit * oldUnit, * newUnit;
	uint  i;

	oldLord = _game->getLord( getAttack()->getId() );
	for( i = 0; i < MAX_UNIT; i++ ) {
		newUnit = getUnit( i, FIGHTER_ATTACK );
		if(newUnit){
			if(newUnit->getNumber()==0){
				newUnit=0;
			}
		}
		oldUnit = oldLord->getUnit( i );
		if( newUnit && oldUnit ) {
			oldUnit->setNumber( newUnit->getNumber()  );
			oldUnit->setMove( newUnit->getMove() );
			oldUnit->setHealth( newUnit->getHealth() );
		} else if (oldUnit) {
			oldLord->setUnit(i,0);
		}
	}

	oldLord = _game->getLord( getDefense()->getId() );
	for( i = 0; i < MAX_UNIT; i++) {
		newUnit = getUnit( i, FIGHTER_DEFENSE );
		if(newUnit){
			if(newUnit->getNumber()==0){
				newUnit=0;
			}
		}
		oldUnit = oldLord->getUnit( i );
		if( newUnit && oldUnit ) {
			oldUnit->setNumber( newUnit->getNumber()  );
			oldUnit->setMove( newUnit->getMove() );
			oldUnit->setHealth( newUnit->getHealth() );
		} else if (oldUnit) {
			oldLord->setUnit(i,0);
		}
	}
	//} else {
		//_crea=false;
	//}
}

void Fight::showFightResults( char result )
{
	FightResult * fr;
	fr = new FightResult( this, result, this );
	fr->exec();
	delete fr;
}

void Fight::socketFightCell()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar type = _socket->readChar();
	_map->changeFightCell( row, col, (TypeFightCell)type );
}

void Fight::socketFightModify()
{
	//logDD("socket modify");
	GenericFightUnit * unit;
	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	if( (CLASS_FIGHTER)cla == FIGHTER_ATTACK ) {
		unit = _unitsAtt[ num ];
	} else if ( (CLASS_FIGHTER)cla == FIGHTER_DEFENSE ) {
		unit = _unitsDef[ num ];
	}
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	unit->setCreature( race, level );
	unit->setNumber( _socket->readInt() );
	unit->setMove( _socket->readChar() );
	unit->setHealth( _socket->readInt() );
}

void Fight::socketFightMove()
{

	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();

	if( (CLASS_FIGHTER)cla == FIGHTER_ATTACK ) {
		_unitsAtt[ num ]->goTo( _map->at( row, col ) );
	} else {
		_unitsDef[ num ]->goTo( _map->at( row, col ) );
	}
}

void Fight::socketFightDamage()
{
	//logDD("fight dama");
	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	int damages = _socket->readInt();

	int numKilled = 0;
	if( (CLASS_FIGHTER)cla == FIGHTER_ATTACK ) {
		numKilled = _unitsAtt[ num ]->hit( damages );
		//logDD( "DAMAGES %d", numKilled );
		addCasualties( FIGHTER_ATTACK, _unitsAtt[num]->getRace(), _unitsAtt[num]->getLevel(), numKilled );
	} else {
		numKilled = _unitsDef[ num ]->hit( damages );
		addCasualties( FIGHTER_DEFENSE, _unitsDef[num]->getRace(), _unitsDef[num]->getLevel(), numKilled );
	}
}

void Fight::setActive( CLASS_FIGHTER cla, int num )
{
	/// XXX: should be useless
	if( _activeUnit ) {
		_activeUnit->setActive( false );
	}

	if( cla == FIGHTER_ATTACK ) {
		_activeUnit = _unitsAtt[num];

		if( _isAttack ) {
			_isActive = true;
		} else {
			_isActive = false;
			_map->clearPath();
		}
	} else {
		_activeUnit = _unitsDef[num];

		if( _isAttack ) {
			_isActive = false;
			_map->clearPath();
		} else {
			_isActive = true;
		}
	}

	_activeUnit->setActive( true );

	if( _isActive ) {
		_map->initPath( _activeUnit );
		///_map->printPath();
	}
}

void Fight::moveUnit( FightCell * cell )
{
	GenericFightCell * current;
	QStack<GenericFightCell> path = _map->computePath( _activeUnit, cell );

	while( ! path.isEmpty() ) {
		current = path.pop();
		_socket->sendFightUnitMove( giveClass( _activeUnit ), giveNum( _activeUnit ), current );
	}

	_socket->sendFightUnitEndMove();
	_activeUnit->setActive( false );
	_map->clearPath();
}

int Fight::giveNum( GenericFightUnit * unit )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( ( _unitsAtt[ i ] == unit ) || ( _unitsDef[ i ] == unit ) ) {
			return i;
		}
	}
	
	logEE( "Should not happen" );
	return -1;
}

CLASS_FIGHTER Fight::giveClass( GenericFightUnit * unit )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _unitsAtt[ i ] == unit ) {
			return FIGHTER_ATTACK;
		}
		if( _unitsDef[ i ] == unit ) {
			return FIGHTER_DEFENSE;
		}
	}
	
	logEE( "Should not happen" );
	return FIGHTER_ATTACK;
}

bool Fight::isOpponent( GenericFightUnit * unit )
{
	bool ret;

	if( isAttack() ) {
		ret = ( giveClass( unit ) == FIGHTER_DEFENSE );
	} else {
		ret = ( giveClass( unit ) == FIGHTER_ATTACK );
	}

	return ret;
}

uint Fight::getCasualtiesNumber( CLASS_FIGHTER fighter )
{
	uint ret = 0;

	if( fighter == FIGHTER_ATTACK ) {
		ret =  _lostAttack->count();
	} else {
		ret = _lostDefense->count();
	}

	return ret;
}

GenericFightUnit * Fight::getCasualtiesUnit( CLASS_FIGHTER fighter, int numUnit )
{
	GenericFightUnit * ret = 0;

	if( fighter == FIGHTER_ATTACK ) {
		if( numUnit < (int)_lostAttack->count() ) {
			ret = _lostAttack->at( numUnit );
		}
	} else {
		if( numUnit < (int)_lostDefense->count() ) {
			ret = _lostDefense->at( numUnit );
		}
	}

	return ret;
}

void Fight::addCasualties( CLASS_FIGHTER fighter, int race, int level, uint nb )
{
	if( nb == 0 ) {
		return;
	}

	bool found = false;

	QList<GenericFightUnit> * list;

	if( fighter == FIGHTER_ATTACK ) {
		list = _lostAttack;
	} else {
		list = _lostDefense;
	}

	for( uint i = 0; i < list->count(); i++ ) {
		GenericFightUnit * unit = list->at( i );
		if( ( unit->getRace() == race ) && ( unit->getLevel() == level ) ) {
			unit->addNumber( nb );
			found = true;
		}
	}

	if( !found ) {logDD("casu not found" );
		GenericFightUnit * unit = new GenericFightUnit();
		unit->setCreature( race, level );
		unit->setNumber( nb );
		list->append( unit );
	}
}

//
// ----- FightControl -----
//

FightControl::FightControl( QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	QPushButton * butControl = new QPushButton( this );
	butControl->setFixedSize( 50, 50 );
	butControl->setPixmap( IMAGE_PATH + "misc/btn_control.png" );
	QPushButton * butSurrender = new QPushButton( this );
	butSurrender->setFixedSize( 50, 50 );
	butSurrender->setPixmap( IMAGE_PATH + "misc/btn_surrender.png" );
	QPushButton * butFlee = new QPushButton( this );
	butFlee->setFixedSize( 50, 50 );
	butFlee->setPixmap( IMAGE_PATH + "misc/btn_flee.png" );
	QPushButton * butAuto = new QPushButton( this );
	butAuto->setFixedSize( 50, 50 );
	butAuto->setPixmap( IMAGE_PATH + "misc/btn_auto.png" );
	QPushButton * butSpell = new QPushButton( this );
	butSpell->setFixedSize( 50, 50 );
	butSpell->setPixmap( IMAGE_PATH + "misc/btn_spell.png" );
	QPushButton * butWait = new QPushButton( this );
	butWait->setFixedSize( 50, 50 );
	butWait->setPixmap( IMAGE_PATH + "misc/btn_wait.png" );
	QPushButton * butDefend = new QPushButton( this );
	butDefend->setFixedSize( 50, 50 );
	butDefend->setPixmap( IMAGE_PATH + "misc/btn_defend.png" );
	/*QMultiLineEdit * edit = new QMultiLineEdit( this );
	edit->setReadOnly( true );*/
	_chat = new ChatWidget( this );
	_chat->setFixedHeight( 50 );

	layout->addWidget( butControl );
	layout->addWidget( butSurrender );
	layout->addWidget( butFlee );
	layout->addWidget( butAuto );
	layout->addWidget( _chat, 1 );
	layout->addWidget( butSpell );
	layout->addWidget( butWait );
 	layout->addWidget( butDefend );

	connect( butWait, SIGNAL( pressed( ) ), SLOT( slot_waitPressed( ) ) );
	connect( butFlee, SIGNAL( pressed( ) ), SLOT( slot_fleePressed( ) ) );
	connect( butDefend, SIGNAL( pressed() ), SIGNAL( sig_defend() ) );

	layout->activate();
	setFixedHeight( 50 );
}

void FightControl::clear()
{
	_chat->clear();
}

void FightControl::newMessage( QString msg )
{
	_chat->newMessage( msg );
}

