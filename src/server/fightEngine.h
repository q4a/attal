/****************************************************************
**
** Attal : Lords of Doom
**
** fightEngine.h
** managing server-side fight
**
** Version : $Id: fightEngine.h,v 1.11 2003/02/03 22:32:51 audoux Exp $
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
#include "common/define.h"
#include "common/genericFightMap.h"
#include "common/unit.h"
#include "server/attalServer.h"
#include "server/fightAnalyst.h"

class FightAnalyst;
class GameData;
class GenericCell;

/**              ------------------------------
 *                          PlayingTroops
 **              ------------------------------ */

class PlayingTroops : public QList<GenericFightUnit>
{
public:
	PlayingTroops( bool order = true );

	void add( GenericFightUnit * unit ) {
		inSort( unit );
	}

 	GenericFightUnit * take();

protected:
	int compareItems( QCollection::Item i1, QCollection::Item i2 );

private:
	bool _order;
};

/**              ------------------------------
 *                         FightEngine
 **              ------------------------------ */

class FightEngine : public QObject
{
	Q_OBJECT

public:
	/** Constructor */
	FightEngine( AttalServer * server );

	void init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericPlayer * defendPlayer, GenericLord * defendLord );

	void init( GenericPlayer * attackPlayer, GenericLord * attackLord, GenericFightUnit * defendUnit[MAX_UNIT], GameData * data );

	void handleSocket( GenericPlayer * player );

	void print();

	/** \return Returns the attackingf lord */
	GenericLord * getAttackLord() { return _attackLord; }

	/** \return Returns the defending lord */
	GenericLord * getDefendLord() { return _defendLord; }

	void handleFakeSocket( FakeSocket * data );

	GenericCell * getDefendCell() { return _defendCell; }

	void setDefendCell( GenericCell * cell ) { _defendCell = cell; }

signals:

	void sig_endFight( char result );

private:
	void socketMove();

	void setupUnits();
	void newTurn();
	void endTurn();
	void endFight( char result );
        void nextUnit( int remove );
	void handleMove( GenericFightUnit * unit, GenericFightCell * cell );
	void handleDistAttack( GenericFightUnit * unit );

	void precomputeUnit() {}
	void activateUnit( GenericFightUnit * unit );
	void postcomputeUnit() {}

	void moveUnit( GenericFightUnit * unit, GenericFightCell * cell );
	void fightUnit( GenericFightUnit * unitAtt, GenericFightUnit * unitDef );
	char isFightFinished();
	void orderTroops();


	int giveNum( GenericFightUnit * unit );
	CLASS_FIGHTER giveClass( GenericFightUnit * unit );
	GenericFightUnit * giveUnit( CLASS_FIGHTER fighter, int num );
	GenericLord * giveLord( GenericFightUnit * unit );


	AttalServer * _server;
	GenericFightMap * _map;
	bool _fightCreature;

	GenericPlayer * _attackPlayer, * _defendPlayer;
	GenericLord * _attackLord, * _defendLord;
	GenericCell * _defendCell;
	FightAnalyst * _analyst;
	FakeSocket * _fake;

	GenericFightUnit * _currentUnit;
	PlayingTroops _troops;
	GenericLord * _creatureLord;
};

#endif // FIGHTENGINE_H
