/****************************************************************
**
** Attal : Lords of Doom
**
** fight.cpp
** Manages the whole fight
**
** Version : $Id: fight.cpp,v 1.125 2009/06/27 18:04:41 lusum Exp $
**
** Author(s) : Cyrille Verrier, Pascal Audoux - Sardi Carlo
**
** Date : 07/12/2000
**
** Licence :
**  This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
****************************************************************/

#include "fight.h"

#include <assert.h>
#include <algorithm>
// include files for QT
#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QVBoxLayout>

// application specific include
#include "conf.h"

#include "libCommon/attalCommon.h"
#include "libCommon/attalSocket.h"
#include "libCommon/attalSettings.h"
#include "libCommon/genericFightCell.h"
#include "libCommon/log.h"
#include "libCommon/pathFinder.h"
#include "libCommon/unit.h"

#include "libFight/fightMapView.h"
#include "libFight/fightResult.h"
#include "libFight/fightSettingsDialog.h"

#include "libClient/attalSound.h"
#include "libClient/game.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/lord.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;

attalFightData::attalFightData()
{
	unit = NULL;
	damages = 0;
	dataType = FIGHT_DATA_NONE;
}

Fight::Fight( QWidget * parent, Game * game, AttalSocket * socket, const char * name  )
    : QWidget( parent )
{

	setWindowTitle( QString ( name ));
	_socket = socket;
	_game = game;
	_currentCell = NULL;
	_isAttack = true;
	_isActive = false;
	_isCreature = false;
	_period = 40;
	_idTimeFight = -1;
	_idTimeAnim = startTimer(_period);
	_activeUnit = NULL;
	_animatedUnit = NULL;
	_lostAttack = new QList<GenericFightUnit *>;
	_lostDefense = new QList<GenericFightUnit *>;
	_attData = new QList<attalFightData>;

	for( int i = 0; i < MAX_UNIT; i ++ ) {
		_unitsAtt[i] = NULL;
		_unitsDef[i] = NULL;
	}

	_map = new FightMap( this );
	_pixmap = new QPixmap( IMAGE_PATH + "fight/background_0.png" );
	_map->setBackgroundBrush(QBrush ( * _pixmap));

	_view = new FightMapView( _map, this );
	_control = new FightControl( this );
	
	_layH = new QHBoxLayout();

	
	_map->setSceneRect( 0,0,  _pixmap->width(), _pixmap->height() );
	_view->setMaximumSize( _pixmap->width(), _pixmap->height() );
	_layH->addWidget( _view, 1, Qt::AlignVCenter );

	_layout = new QVBoxLayout( this );
	_layout->addLayout( _layH, 1 );
	_layout->addWidget( _control );
	_layout->activate();
	//_view->fitInView( _view->frameRect(), Qt::KeepAspectRatioByExpanding);
	_view->fitInView( QRect(0,0,0,0), Qt::KeepAspectRatioByExpanding);

	updateDispositionMode();

	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	if( fsettings.areCellsVisible ) {
		_map->showCells();
	} else {
		_map->hideCells();
	}
	
	connect( _control, SIGNAL( sig_wait( ) ), SLOT( slot_wait( ) ) );
	connect( _control, SIGNAL( sig_retreat( ) ), SLOT( slot_flee( ) ) );
	connect( _control, SIGNAL( sig_defend() ), SLOT( slot_defend() ) );
	connect( _control, SIGNAL( sig_control() ), SLOT( slot_control() ) );
	connect( _control, SIGNAL( sig_message( QString ) ), SLOT( slot_message( QString ) ) );
	
	connect( _view, SIGNAL( sig_mouseMoved( FightCell * , bool) ), SLOT( slot_mouseMoved( FightCell * , bool) ) );
	connect( _view, SIGNAL( sig_mouseRightPressed( FightCell * ) ), SLOT( slot_mouseRightPressed( FightCell * ) ) );
	connect( _view, SIGNAL( sig_mouseLeftPressed( FightCell * , bool) ), SLOT( slot_mouseLeftPressed( FightCell *, bool ) ) );
	connect( _view, SIGNAL( sig_mouseReleased() ), SLOT( slot_mouseReleased() ) );
	connect( this , SIGNAL( sig_showResults() ) , SLOT ( showFightResults() ) );
}

Fight::~Fight()
{
	stopDataTimer();
	
	stopAnimTimer();
	
	clear();

	delete _lostAttack;
	delete _lostDefense;
	delete _attData;
	if( _pixmap ) {
		delete _pixmap;
	}
}

void Fight::reinit()
{
	clear();

	_lordAtt = NULL;
	_lordDef = NULL;
	_isAttack = true;
	_isActive = false;
	_isCreature = false;
	_currentCell = NULL;
	_activeUnit = NULL;
	_animatedUnit = NULL;
	_map->clearPath();
	_map->reinit();
	_control->clear();
	_result.clear();

	_idTimeFight = -1;
	_idTimeAnim = startTimer( _period );

	updateDispositionMode();
}

void Fight::clear()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _unitsAtt[i] ) {
			delete _unitsAtt[i];
			_unitsAtt[i] = NULL;
		}
		if( _unitsDef[i] ) {
			delete _unitsDef[i];
			_unitsDef[i] = NULL;
		}
	}
	
	while( ! _lostAttack->isEmpty() ) {
		delete _lostAttack->takeFirst();
	}
	
	while( ! _lostDefense->isEmpty() ) {
		delete _lostDefense->takeFirst();
	}
	
	_animations.clear();
}

void Fight::updateDispositionMode()
{
	_control->updateResolutionMode();
	update();
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

FightUnit * Fight::getUnit( int num, CLASS_FIGHTER fighter )
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
		if( _isCreature ) {
			_creatureRace = unit->getRace();
			_creatureLevel = unit->getLevel();
		}
		FightUnit * currentUnit;

		bool noMirror =( fighter == FIGHTER_ATTACK ) ? true : false;
		currentUnit = new FightUnit( _map, unit, noMirror  );
		currentUnit->setFightMap ( _map );
		appendAnimation( currentUnit );

		if( fighter == FIGHTER_ATTACK ) {
			if( _unitsAtt[num] ) {
				delete _unitsAtt[num];
			}
			_unitsAtt[num] = currentUnit;
			currentUnit->setPosition( _map->at( num, 0 ) );

		} else if( fighter == FIGHTER_DEFENSE ) {
			if( _unitsDef[num] ) {
				delete _unitsDef[num];
			}
			_unitsDef[num] = currentUnit;
			currentUnit->setPosition( _map->at( num, 14 ) );
		}

	} else {  //!unit

		if( ( fighter == FIGHTER_ATTACK ) && ( _unitsAtt[num] ) ) {
			delete _unitsAtt[num];
			_unitsAtt[num] = NULL;
		} else if( ( fighter == FIGHTER_DEFENSE ) && _unitsDef[num] ) {
			delete _unitsDef[num];
			_unitsDef[num] = NULL;
		}

	}
}


void Fight::slot_mouseMoved( FightCell * cell, bool isUnit )
{
	if( !cell) {
		setCursor( Qt::ArrowCursor );
		return;
	}

	if( cell) {
		_currentCell = cell;
		_isUnit = isUnit;
	}

	if( _currentCell ) {
#ifdef QT_DEBUG
		QString msg;
		msg = QString(tr("Cell: row %1, col %2, coordinates x %3, y %4 ")).arg(_currentCell->getRow()).arg(_currentCell->getCol()).arg(_currentCell->x()).arg( _currentCell->y());

		emit sig_statusMsg( msg );
#endif 

		GenericFightUnit * unit = _currentCell->getUnit();
		AttalCommon::FightCellAccess access = _currentCell->getAccess();

		if( unit ) {
			if( unit == _activeUnit ) {
				setCursor( Qt::WaitCursor );
			} else if ( !isOpponent(unit) ){
				setCursor( Qt::ForbiddenCursor );
			} else {
				if( access == AttalCommon::FAR_OCCUPIED ) {
					if ( _activeUnit->isDistAttack() && _isUnit ) {
						setCursor( Qt::PointingHandCursor );
					} else {
						setCursor( Qt::ForbiddenCursor );
					}
				} else if ( access == AttalCommon::NEAR_OCCUPIED && _isUnit ) {
					setCursor( Qt::PointingHandCursor );
				} else {
					setCursor( Qt::ArrowCursor );
				}
			}
		} else if ( access == AttalCommon::NEAR_FREE ) {
			setCursor( Qt::ArrowCursor );
		} else {
			setCursor( Qt::ForbiddenCursor );
		}
	} else {
		setCursor( Qt::ForbiddenCursor );
	}

}

void Fight::slot_mouseLeftPressed( FightCell * cell, bool isUnit )
{

	AttalPopup * popup = this->findChild<AttalPopup *>("AttalFightPopup");
	
	if( popup && popup->isVisible() ) {
		popup->hide();
		popup->setType();
		return;
	}

	AttalCommon::FightCellAccess access = cell->getAccess();
	TRACE("left pressed cell access %d, active unit %p , isActive %d", access, _activeUnit, _isActive);
	TRACE("cell->getUnit %p, isUnit %d dist %d", cell->getUnit(), isUnit, cell->getDist() );
	TRACE("cell row %d, col %d", cell->getRow(), cell->getCol() );
	
	GenericFightCell * neib = NULL;
	GenericFightUnit * oppunit = cell->getUnit();

	if( oppunit && !isOpponent( oppunit )  ) {
		return;
	}
	
	if( _activeUnit && _isActive ) {

		switch( access ) {
			case AttalCommon::NEAR_FREE :
				TRACE("NEAR_FREE");
				moveUnit( cell );
				break;
			case AttalCommon::NEAR_OCCUPIED:
				TRACE("NEAR_OCCUPIED");
				if( _activeUnit->getDistAttack() > 0 && oppunit ) {
					_socket->sendFightDistAttack( giveClass( oppunit ), giveNum( oppunit ) );
					_socket->sendFightUnitEndMove();
				} else if ( oppunit  ) {
					moveUnit( cell );
				} else {
					neib = _map->getHeadCell( cell ,_activeUnit->isLookingToRight() );

					if( neib ) {
						oppunit = neib->getUnit();
						if ( ( oppunit ) && ( isOpponent ( oppunit  ) ) ) {
							moveUnit( neib );
						}
					}
				}
				break;
			case AttalCommon::FAR_OCCUPIED:
				TRACE("FAR_OCCUPIED");
				if( ( oppunit ) && ( _activeUnit->getDistAttack() > 0 ) ) {
					_socket->sendFightDistAttack( giveClass( oppunit ), giveNum( oppunit ) );
					_socket->sendFightUnitEndMove();
				} else {
					neib = _map->getHeadCell( cell , !_activeUnit->isLookingToRight() );
					if( neib ) {
						oppunit = neib->getHeadUnit();
						if( oppunit && neib->getAccess()== AttalCommon::NEAR_OCCUPIED  && ( isOpponent ( oppunit  ) ) ) {
							moveUnit( neib );
						}
					}
				}
				break;
			default:
				break;
		}
	}
}

void Fight::slot_mouseRightPressed( FightCell * cell )
{
	AttalPopup * popup = this->findChild<AttalPopup *>("AttalFightPopup");

	if( !popup ) {
		popup = new AttalPopup( this );
		popup->setObjectName( QString("AttalFightPopup"));
	}
	popup->hide();

	QPoint point = (QPoint)(_view->mapFromScene( cell->x() + 140, cell->y() + 40));
	
	GenericFightUnit * unit = cell->getUnit();
	
	if( unit && ( unit->getNumber() > 0 ) ) {
		popup->setText( unit->getUnitDescription() );
	}
	
	popup->move( point );
	popup->show();
}

void Fight::slot_mouseReleased()
{
}

void Fight::slot_wait( void )
{
	_socket->sendFightWait();
	_activeUnit->setActive( false );
}

void Fight::slot_flee( void )
{
	_socket->sendFightFlee();
}

void Fight::slot_defend()
{
	_socket->sendFightDefend();
}

void Fight::slot_animateFighting()
{
	attalFightData dataFight;

	if( !_attData->isEmpty() ) {
		if(  !_animatedUnit || !_animatedUnit->isMoving() ) { 
			dataFight = _attData->first();
			processData( dataFight );
			_attData->removeFirst();
		}
		if(  _animatedUnit && _animatedUnit->isMoving() ) {
			dataFight = _attData->first();
			while(( _animatedUnit == dataFight.unit && dataFight.dataType == FIGHT_DATA_MOVE ) ) {
				processData( dataFight );
				_attData->removeFirst();
				dataFight = _attData->first();
			}
		}
	} else {
		_animatedUnit = NULL;
	}

	checkTimer();

}
	

void Fight::timerEvent(QTimerEvent *event)
{
	int id = event->timerId();
	if( id == -1 ) {
		return;
	}

	if( id == _idTimeFight) {
		slot_animateFighting();
	}
	if( id == _idTimeAnim) {
		//stopAnimTimer();
		int animCount = _animations.count();

		if( animCount ) {
			for( int i = 0 ; i < animCount; i++ ) {
				_animations.at( i )->advance(1);
			}
		}
		//startAnimTimer();
	}
	//qDebug() << "Timer ID:" << event->timerId();
}

void Fight::processData( attalFightData dataFight )
{
	TRACE("process data %d", _idTimeFight);
	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	int speedMove = 300 / fsettings.animationSpeed;
	int speed = 400 / fsettings.animationSpeed;

	switch( dataFight.dataType ) {
				case FIGHT_DATA_FIGHT:
					//logDD("fight");
					setAdvancePeriod(speed);
					handleDamages( dataFight );
					break;
				case FIGHT_DATA_MOVE:
					//logDD("move");
					setAdvancePeriod(speedMove);
					dataFight.unit->goTo( _map->at( dataFight.row, dataFight.col ) );
					_animatedUnit = dataFight.unit;
					break;
				case FIGHT_DATA_ACTIVE:
					//logDD("active");
					setAdvancePeriod( 200 );
					setActive( (CLASS_FIGHTER)dataFight.cla, dataFight.num );
					break;
				case FIGHT_DATA_END:
					fightEnd( dataFight.result );
					break;
				default:
				case FIGHT_DATA_NONE:
					break;
			}
}

void Fight::handleSocket()
{
	Q_ASSERT( _socket );
	_game->handleSocket();
	_socket->reReadData();
	
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
	TRACE("Fight::socketMsg");

	int cla2 = _socket->getCla2();
	TRACE(" cla2 %d", cla2 );

	QString msg;
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		msg[ i ] = _socket->readChar();
	}
	
	if( cla2 == C_MSG_FIGHT ) {
		_listmsg.enqueue( msg );
		return;
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
		socketFightCreatue();
		break;
	case C_FIGHT_LORD: 
		socketFightLord();
		break;
	case C_FIGHT_CELL:
		socketFightCell();
		break;
	case C_FIGHT_UNIT: 
		socketFightUnit();
		break;
	case C_FIGHT_MODUNIT:
		socketFightModify();
		break;
	case C_FIGHT_MOVE:
		socketFightMove();
		break;
	case C_FIGHT_ENDMOVE:
		socketFightEndMove();
		break;
	case C_FIGHT_ACTIVE:
		socketFightActive();
		break;
	case C_FIGHT_DAMAGE:
		socketFightDamage();
		break;
	case C_FIGHT_END: 
		socketFightEnd();
		break;
	}
}

void Fight::updateUnits( void )
{

	updateLordUnits( getAttack() , FIGHTER_ATTACK );
	updateLordUnits( getDefense() , FIGHTER_DEFENSE	);

}

void Fight::updateLordUnits( GenericLord * lord , CLASS_FIGHTER type )
{
	
	GenericLord *oldLord;
	GenericFightUnit * oldUnit, * newUnit;
	uint  i;
	
	if ( lord ) {
		oldLord = _game->getLord( lord->getId() );
		for( i = 0; i < MAX_UNIT; i++) {
			newUnit = getUnit( i, type );
			oldUnit = oldLord->getUnit( i );
			if( newUnit ) {
				if( newUnit->getNumber() == 0 ) {
					newUnit = NULL;
				}
			}
			if( newUnit && oldUnit ) {
				oldUnit->setNumber( newUnit->getNumber()  );
				oldUnit->setMove( newUnit->getMove() );
				oldUnit->setHealth( newUnit->getHealth() > 0 ? newUnit->getHealth() : 0);
			} else if (oldUnit) {
				oldLord->setUnit(i,NULL);
			}
		}
	}

}

void Fight::showFightResults()
{
	FightResultWizard resultDialog( this );
	resultDialog.exec();
}

void Fight::socketFightCell()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar type = _socket->readChar();
	
	_map->changeFightCell( row, col, (AttalCommon::TypeFightCell)type );
}

void Fight::socketFightUnit()
{
	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();
	uchar move =_socket->readChar();
	int health =_socket->readInt();
	
	/*if(health > unit->getMaxHealth()) {
	logEE("bug, health %d, maxhealth %d", health, unit->getMaxHealth());
	}*/
	GenericFightUnit * unit = new GenericFightUnit( race, level );
	unit->setNumber( number );
	unit->setMove( move );
	unit->setHealth( health );

	setUnit( unit, (CLASS_FIGHTER)cla, num );
	delete unit;
}
		
void Fight::socketFightCreatue()
{
	_isCreature = true;
	setOpponent( _game->getLord( 0 ) );
}

void Fight::socketFightEndMove()
{
	_isActive = false;
	_map->clearPath();
}
		
void Fight::socketFightEnd()
{
	  
	attalFightData dataFight;

	uchar result = _socket->readChar();

	// process all pending fight data before end of fight
	while( !_attData->isEmpty() ) {
		dataFight = _attData->first();
		processData( dataFight );
		_attData->removeFirst();
	}
	
	fightEnd( result );

/*
	dataFight.result = _socket->readChar();
	dataFight.dataType = FIGHT_DATA_END;

	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	if( fsettings.isAnimationEnabled ) {
		_attData->append(dataFight);
		checkTimer();
	} else {
		fightEnd( dataFight.result );
	}
*/
}

void Fight::fightEnd( uchar result )
{
	updateUnits();
	_result.fromUChar( result );
	//showFightResults();
	
	emit sig_showResults();
	
	stopDataTimer();
	
	stopAnimTimer();

	emit sig_quit();
}
		
void Fight::socketFightLord()
{
	uchar id = _socket->readChar();
	uchar valueAtt = _socket->readChar();
	uchar valueDef = _socket->readChar();

	GenericLord * lord = _game->getLord( id );
	if( lord ) {
		lord->setBaseCharac( ATTACK, valueAtt );
		lord->setBaseCharac( DEFENSE, valueDef );
		setOpponent( lord );
	} else {
		logEE(" lord not exist id %d, valueAtt %d, valueDef %d", id, valueAtt, valueDef);
	}
}

void Fight::socketFightActive() 
{
	attalFightData dataFight;

	dataFight.cla = _socket->readChar();
	dataFight.num = _socket->readChar();

	dataFight.dataType = FIGHT_DATA_ACTIVE;

	manageData( dataFight );
}

void Fight::socketFightModify()
{
	TRACE("socket modify");
	GenericFightUnit * unit;
	uchar cla = _socket->readChar();
	uchar num = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();
	uchar move =_socket->readChar();
	int health =_socket->readInt();

	unit = getUnit ( num, (CLASS_FIGHTER)cla);

	unit->setCreature( race, level );
	unit->setNumber( number );
	unit->setMove( move );
	unit->setHealth( health );
}

void Fight::socketFightMove()
{
	attalFightData dataFight;

	dataFight.cla = _socket->readChar();
	dataFight.num = _socket->readChar();
	dataFight.row = _socket->readInt();
	dataFight.col = _socket->readInt();
	
	dataFight.dataType = FIGHT_DATA_MOVE;
	
	dataFight.unit = getUnit( dataFight.num, (CLASS_FIGHTER)dataFight.cla) ;
	
	manageData( dataFight );
}

void Fight::handleDamages( attalFightData dataFight )
{
	int numKilled = 0;
	FightUnit * attUnit = NULL;
	FightUnit * defUnit = NULL;
	CLASS_ATTACK_TYPE type = dataFight.type;
	QString msg;	

	attUnit = getUnit( dataFight.numAtt, (CLASS_FIGHTER)dataFight.claAtt) ;
	defUnit = getUnit( dataFight.numDef, (CLASS_FIGHTER)dataFight.claDef) ;
					
	if(type == ATTACK_SHOOT) {	
		ImageTheme.playSound( AttalSound::SND_ARROW );
		if(  attUnit->getCell()->getRow() > defUnit->getCell()->getRow() && attUnit->canAnimate(ShootHigh) ) {
			attUnit->animate(ShootHigh);
		}	else if( attUnit->getCell()->getRow() < defUnit->getCell()->getRow() && attUnit->canAnimate(ShootLow) ) {
			attUnit->animate(ShootLow);
		} else {
			attUnit->animate(Shooting);
		}
	} else {
		ImageTheme.playSound( AttalSound::SND_HIT );
		if( _map->isUpperLevel( attUnit->getCell() , defUnit->getCell() ) && attUnit->canAnimate(AttackHigh) ) {
			attUnit->animate(AttackHigh);
		}	else if( _map->isLowerLevel( attUnit->getCell() , defUnit->getCell() ) && attUnit->canAnimate(AttackLow) ) {
			attUnit->animate(AttackLow);
		} else {
			attUnit->animate(Fighting);
		}
	}
	
	defUnit->animate(Defending);
		
	numKilled = defUnit->hit( dataFight.damages );
	TRACE( "DAMAGES %d", numKilled );
	addCasualties( (CLASS_FIGHTER)dataFight.claDef, defUnit->getRace(), defUnit->getLevel(), numKilled );
				
	AttalPopup * popup = this->findChild<AttalPopup *>("AttalFightPopup");

	if( popup ) {
		popup->update();
	}
				
	if( !_listmsg.isEmpty() ) {
		msg = _listmsg.dequeue();
		_control->newMessage( msg );
	}
}

void Fight::socketFightDamage()
{
	attalFightData dataFight; 

	dataFight.claAtt = _socket->readChar ();
	dataFight.numAtt = _socket->readChar();
	
	dataFight.type = (CLASS_ATTACK_TYPE) _socket->readChar ();
	
	dataFight.claDef = _socket->readChar();
	dataFight.numDef = _socket->readChar();
	dataFight.damages = _socket->readInt();
	
	dataFight.attUnit = getUnit( dataFight.numAtt, (CLASS_FIGHTER)dataFight.claAtt) ;
	dataFight.defUnit = getUnit( dataFight.numDef, (CLASS_FIGHTER)dataFight.claDef) ;

	dataFight.dataType = FIGHT_DATA_FIGHT;

	manageData( dataFight );	
}

void Fight::goTo( FightUnit * /* unit */, int /* row */, int /* col */ )
{
}

void Fight::setActive( CLASS_FIGHTER cla, int num )
{
	TRACE("set active cla %d, num %d", cla, num);
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

	if( !isOpponent( _activeUnit ) ) {
		setUnitsAlpha( true );
		_activeUnit->setAlpha( false );
	} else {
		setUnitsAlpha( false );
	}

	if( _isActive ) {
		_map->initPath( _activeUnit );
		slot_mouseMoved( _currentCell, true );
		///_map->printPath();
	}
}

void Fight::setUnitsAlpha( bool state )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
				if( _isAttack ) {
					if( _unitsAtt[i] ) {
						_unitsAtt[ i ]->setAlpha( state );
					}
				} else if( _unitsDef[i] ){
					_unitsDef[ i ]->setAlpha( state );
				}
			}
}

void Fight::moveUnit( GenericFightCell * cell )
{
	GenericFightCell * current;
	QStack<GenericFightCell *> path = _map->computePath( _activeUnit, cell );
	
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
	
	logEE( "Fight::giveClass Should not happen %p", unit );
	return FIGHTER_ATTACK;
}

bool Fight::isOpponent( GenericFightUnit * unit )
{
	if( isAttack() ) {
		return ( giveClass( unit ) == FIGHTER_DEFENSE );
	} else {
		return ( giveClass( unit ) == FIGHTER_ATTACK );
	}
}

GenericLord * Fight::getFighterLord( CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		return _lordAtt;
	} else {
		return _lordDef;
	}
}

uint Fight::getCasualtiesNumber( CLASS_FIGHTER fighter )
{
	if( fighter == FIGHTER_ATTACK ) {
		return _lostAttack->count();
	} else {
		return _lostDefense->count();
	}
}

GenericFightUnit * Fight::getCasualtiesUnit( CLASS_FIGHTER fighter, int numUnit )
{
	if( fighter == FIGHTER_ATTACK && numUnit < (int)_lostAttack->count() ) {
		return _lostAttack->at( numUnit );
	} else if( fighter == FIGHTER_DEFENSE && numUnit < (int)_lostDefense->count() ) {
		return _lostDefense->at( numUnit );
	} else {
		return NULL;
	}
}

void Fight::addCasualties( CLASS_FIGHTER fighter, uint race, uint level, uint nb )
{
	if( nb ) {
		bool found = false;
		
		QList<GenericFightUnit *> * list;
		
		if( fighter == FIGHTER_ATTACK ) {
			list = _lostAttack;
		} else {
			list = _lostDefense;
		}
		
		for( int i = 0; i < list->count(); i++ ) {
			GenericFightUnit * unit = list->at( i );
			if( ( unit->getRace() == race ) && ( unit->getLevel() == level ) ) {
			unit->addNumber( nb );
			found = true;
			}
		}
		
		if( !found ) {
			GenericFightUnit * unit = new GenericFightUnit();
			unit->setCreature( race, level );
			unit->setNumber( nb );
			list->append( unit );
		}
	}
}

void Fight::slot_control ( void )
{
	FightSettingsDialog* dlg = new FightSettingsDialog ( this );
	dlg->exec();
	
	if( dlg->result() == QDialog::Accepted ) {
		AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();
	
		if( fsettings.areCellsVisible ) {
			_map->showCells();
		} else {
			_map->hideCells();
		}
	}
}

void Fight::slot_message( QString msg )
{
	GenericLord * lord;
	
	if( _socket ) {
		if( _isAttack ) {
			lord = _lordAtt;
		} else {
			lord = _lordDef;
		}
		_socket->sendMessage( lord->getOwner()->getConnectionName() + " : " + msg );
	} else {
		_control->newMessage( "(Not connected) : " + msg );
	}
}

void Fight::appendAnimation( AttalSprite * item)
{
	_animations.append( item );
}

void Fight::setAdvancePeriod( int period ) 
{
	if( _period != period) {
		_period =  period;
		if( _idTimeAnim != -1 ) {
			killTimer( _idTimeAnim );
			_idTimeAnim = startTimer( _period );
		}
	}
}

void Fight::checkTimer()
{
	if( _attData->isEmpty() ) {
		stopDataTimer();
	} else {
		startDataTimer();
	}
}

void Fight::manageData( attalFightData dataFight )
{
	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	if( fsettings.isAnimationEnabled ) {
		_attData->append(dataFight);
		startDataTimer();
	} else {
		processData( dataFight );
	}

}

void Fight::startDataTimer() 
{
	if( _idTimeFight == -1 ) {
		_idTimeFight = startTimer( 40 );
	}
}

void Fight::stopDataTimer() 
{
	TRACE("Fight::stopDataTimer");

	if( _idTimeFight != -1 ) {
		killTimer( _idTimeFight );
		_idTimeFight = -1;
	}
}

void Fight::startAnimTimer() 
{
	if( _idTimeAnim == -1 ) {
		_idTimeAnim = startTimer(_period);
	}
}

void Fight::stopAnimTimer() 
{
	if( _idTimeAnim != -1 ) {
		killTimer( _idTimeAnim );
		_idTimeAnim = -1;
	}
}
