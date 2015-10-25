/****************************************************************
**
** Attal : Lords of Doom
**
** fightEngine.h
** managing server-side fight
**
** Version : $Id: fightEngine.h,v 1.20 2008/11/12 18:21:30 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 19/05/2001
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


#ifndef FIGHTENGINE_H
#define FIGHTENGINE_H


// generic include files
// include files for QT
// application specific include files
#include "libCommon/define.h"
#include "libCommon/fightResultStatus.h"
#include "libCommon/genericFightMap.h"
#include "libCommon/unit.h"

#include "libServer/attalServer.h"
#include "libServer/fightAnalyst.h"
#include "libServer/playingTroops.h"
#include <QList>
#include <QtAlgorithms>

class FightAnalyst;
class GameData;
class GenericCell;
class GenericFightUnit;

/**              ------------------------------
 *                         FightEngine
 **              ------------------------------ */

class FightEngine : public QObject
{
	Q_OBJECT

public:
	/** Constructor */
	FightEngine( AttalServer * server );
	
	~FightEngine();

	void init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericPlayer * defendPlayer, GenericLord * defendLord );

	void init( GenericPlayer * attackPlayer, GenericLord * attackLord, GameData * data, GenericMapCreature * creature );

	void handleSocket( GenericPlayer * player, AttalSocketData data );

	void print();

	/** \return Returns the attackingf lord */
	GenericLord * getAttackLord() { return _attackLord; }

	/** \return Returns the defending lord */
	GenericLord * getDefendLord() { return _defendLord; }

	void handleFakeSocket( FakeSocket * data );

	GenericMapCreature * getDefendCreature() { return _defendCreature; }

	uint getExperience( GenericLord * lord );
	
	FightResultStatus getResult() { return _result; }
	
	void endFight();
	
	bool hasEnded() { return _hasEnded; }

signals:

	void sig_endFight( FightResultStatus result );

private:
	void socketFightMove();
	void socketFightDistAttack();
	void socketFightEndMove();
	void socketFightFlee(GenericPlayer * player);

	void initMap();
	void setupUnits();
	void newTurn();
	void endTurn();
	void nextUnit( int remove );

	void handleMove( GenericFightUnit * unit, GenericFightCell * cell );

	void precomputeUnit() {}
	void activateUnit( GenericFightUnit * unit );
	void postcomputeUnit() {}

	void moveUnit( GenericFightUnit * unit, GenericFightCell * cell );
	void fightUnit( GenericFightUnit * unitAtt, GenericFightUnit * unitDef, CLASS_ATTACK_TYPE attackType );
	void computeFightResultStatus();
	void orderTroops();
	void updateUnits();
	void printStatus( GenericFightUnit * unit, GenericFightCell * cell );


	int giveNum( GenericFightUnit * unit );
	CLASS_FIGHTER giveClass( GenericFightUnit * unit );
	GenericFightUnit * giveUnit( CLASS_FIGHTER fighter, int num );
	GenericLord * giveLord( GenericFightUnit * unit );

	int readInt();

	unsigned char readChar();

	uchar getCla1();
	uchar getCla2();
	uchar getCla3();

	int _num;

	AttalServer * _server;
	GenericFightMap * _map;
	bool _hasEnded;

	GenericPlayer * _attackPlayer, * _defendPlayer, * _currentPlayer;
	GenericLord * _attackLord, * _defendLord;
	GenericMapCreature	* _defendCreature;
	FightAnalyst * _analyst;
	FakeSocket * _fake;

	GenericFightUnit * _currentUnit;
	PlayingTroops _troops;
	GenericLord * _creatureLord;
	uint _defendExp, _attackExp;
	FightResultStatus _result;
	AttalSocketData _currentData;
};

#endif // FIGHTENGINE_H
