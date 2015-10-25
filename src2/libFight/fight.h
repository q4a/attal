/****************************************************************
**
** Attal : Lords of Doom
**
** fight.h
** Manages the whole game
**
** Version : $Id: fight.h,v 1.38 2009/06/27 18:04:41 lusum Exp $
**
** Author(s) : Cyrille Verrier - Sardi Carlo
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


#ifndef FIGHT_H
#define FIGHT_H


// include files for QT
#include <QLabel>
#include <QList>
#include <QQueue>
#include <QTimer>
#include <QWidget>
// application specific includes
#include "libCommon/attalSocket.h"
#include "libCommon/fightResultStatus.h"

#include "libFight/fightCell.h"
#include "libFight/fightControl.h"
#include "libFight/fightMap.h"
#include "libFight/fightUnit.h"

//#include "libClient/attalSprite.h"
#include "libClient/lord.h"
#include "libClient/widget.h"


class QHBoxLayout;
class QPixmap;
class QVBoxLayout;

class FightMapView;
class Game;

enum fightDataType {
	FIGHT_DATA_NONE,
	FIGHT_DATA_MOVE,
	FIGHT_DATA_FIGHT,
	FIGHT_DATA_ACTIVE,
	FIGHT_DATA_END
};

class attalFightData {
	public:
		attalFightData();

		uchar result;
		uchar claDef, claAtt, numAtt , numDef;
		int	damages;
		CLASS_ATTACK_TYPE type;
		int row, col;
		FightUnit * attUnit,  * defUnit;
		FightUnit * unit;
		uchar cla, num;
		fightDataType dataType;
};
	

/*              ------------------------------
 *                         Fight
 *              ------------------------------ */

class Fight : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	Fight( QWidget * parent = 0, Game * game = 0, AttalSocket * socket = 0, const char * name = 0 );

	/** delete player and lord */
	~Fight();

	/** Reinit widget */
	void reinit();
	
	void clear();

	void handleSocket();

	void handleDamages( attalFightData data );

	void setOpponent( GenericLord * lord );

	void setLord( GenericLord * lord, CLASS_FIGHTER fighter );

	void setUnit( GenericFightUnit * unit, CLASS_FIGHTER fighter, int num );

	FightUnit * getUnit( int num, CLASS_FIGHTER fighter );

	bool isAttack() { return _isAttack; }

	GenericLord * getAttack() { return _lordAtt; }

	GenericLord * getDefense() { return _lordDef; }

	GenericLord * getFighterLord( CLASS_FIGHTER fighter );

	uint getCasualtiesNumber( CLASS_FIGHTER fighter );

	GenericFightUnit * getCasualtiesUnit( CLASS_FIGHTER fighter, int numUnit );

	FightResultStatus getFightResultStatus() { return _result; }

	uint getCreatureRace() { return _creatureRace; }

	uint getCreatureLevel() { return _creatureLevel; }

	void updateDispositionMode();
	
	void goTo(FightUnit * unit, int row, int col);

	void processData( attalFightData dataFight );

	void manageData( attalFightData dataFight );

signals:
	void sig_quit();
	
	void sig_showResults();
	
	void sig_statusMsg( const QString & );
	
private slots:
	/** slot for managing mouse movements */
	void slot_mouseMoved( FightCell * cell, bool isUnit );

	/** slot for managing right click */
	void slot_mouseRightPressed( FightCell * cell );

	/** slot for managing left click */
	void slot_mouseLeftPressed( FightCell * cell , bool isUnit);

	/** slot for managing button release */
	void slot_mouseReleased();
	
	void slot_animateFighting();

	void slot_wait( void );

	void slot_flee( void );

	void slot_defend( void );

	void slot_control ( void );

	void slot_message( QString msg );
	
	void showFightResults();

protected:


	void timerEvent(QTimerEvent *event);

private:

	void socketMsg();

	void socketFight();

	void socketFightCell();

	void socketFightUnit();

	void socketFightModify();

	void socketFightMove();

	void socketFightDamage();
	
	void socketFightActive();

	void socketFightLord();
		
	void socketFightEnd();

	void socketFightEndMove();

	void socketFightCreatue();

	void setActive( CLASS_FIGHTER, int );

	void moveUnit( GenericFightCell * cell );

	int giveNum( GenericFightUnit * unit );

	CLASS_FIGHTER giveClass( GenericFightUnit * unit );

	bool isOpponent( GenericFightUnit * unit );

	void addCasualties( CLASS_FIGHTER fighter, uint race, uint level, uint nb );

	void updateUnits( void );

	void setUnitsAlpha( bool state );
	
	void updateLordUnits( GenericLord * lord , CLASS_FIGHTER type );
	
	void fightEnd( uchar result );

	void appendAnimation( AttalSprite * item);
	
	void setAdvancePeriod( int period );
	
	void inline checkTimer();

	void inline startDataTimer();

	void stopDataTimer();

	void inline startAnimTimer();

	void stopAnimTimer();

	bool _isAttack;
	bool _isActive;
	
	bool _isCreature;
	uint _creatureRace, _creatureLevel;
	
	GenericLord * _lordAtt, * _lordDef;

	//int _lostAtt[ MAX_UNIT ], _lostDef[ MAX_UNIT ];
	QList<GenericFightUnit *> * _lostAttack, * _lostDefense;
	QList<attalFightData> * _attData;
	QList<AttalSprite *>  _animations;
	QQueue<QString>  _listmsg;
	
	FightUnit * _activeUnit;
	FightUnit * _animatedUnit;
	AttalSocket * _socket;
	QTimer *_timer;
	int _idTimeFight, _idTimeAnim, _period;
	FightMap * _map;
	FightMapView * _view;
	FightUnit * _unitsAtt[MAX_UNIT], * _unitsDef[MAX_UNIT];
	FightControl * _control;
	FightCell * _currentCell;
	FightResultStatus _result;
	Game * _game;
	QVBoxLayout * _layout;
	QHBoxLayout * _layH;
	QPixmap * _pixmap;
	uchar _claDef, _claAtt, _numAtt , _numDef;
	int	_damages;
	bool _isUnit;
};


#endif // FIGHT_H
