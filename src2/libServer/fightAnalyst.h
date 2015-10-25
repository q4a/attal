/****************************************************************
**
** Attal : Lords of Doom
**
** fightAnalyst.h
** analyst for the fight phase
**
** Version : $Id: fightAnalyst.h,v 1.12 2008/11/12 18:21:30 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 05/01/2002
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


#ifndef FIGHTANALYST_H
#define FIGHTANALYST_H


// generic include files
// include files for QT
// application specific include files
#include "libCommon/attalSocket.h"
#include "libCommon/define.h"


class GameData;
class GenericLord;
class GenericFightMap;
class GenericFightUnit;

class FightEngine;

/** Fake socket, for AI of fightAnalyst */
class FakeSocket : public AttalSocket
{
public:
	/** Constructor */
	FakeSocket();

protected:
	/** As it is a 'fake' socket, it doesn't send anything */
	virtual void send();
};

/*              ------------------------------
 *                         FightAnalyst
 *              ------------------------------ */


/** comment for the class */
class FightAnalyst
{

public:
	/** Constructor (for AI fights) */
	FightAnalyst( GameData * data, AttalSocket * socket );

	/** constructor for creatures fights */
	FightAnalyst( GameData * data, FightEngine * engine, AttalSocket * socket );

	/** Destructor */
	virtual ~FightAnalyst();

	void handleFightSocket();

	void handleFightData( FakeSocket * data );

	void initCreatures( GenericLord * lord );

	void updateUnits( void );
	
	bool IsCreature() { return (_lordDefense == 0) ? true : false;}

	int getPower( GenericFightUnit * unit );
	
protected:
	void handleInit();

	void handleOpponent();

	void handleCell();

	void handleNewUnit();

	void handleMove();

	void handleDamage();

	void handleActive();

	void playUnit( GenericFightUnit * unit , int pos );
	
	void endMove();
	
	bool checkValidUnit( int num, CLASS_FIGHTER cla );

	GenericFightCell * getValidDestination( GenericFightUnit * currentUnit ,int pos );

	GenericFightUnit * getUnit( int num, CLASS_FIGHTER fighter );

	GenericFightUnit * getOpponentUnit( int num, CLASS_FIGHTER fighter );
	
	CLASS_FIGHTER getOpponentClass();

	bool _oppIsAtt;

	bool _isCreature;

	AttalSocket * _socket;
	GameData * _data;
	GenericFightMap * _map;
	CLASS_FIGHTER _fighter;
	GenericLord * _lordAttack, * _lordDefense, * _lordOpp;
	GenericFightUnit * _unitsAtt[MAX_UNIT], * _unitsDef[MAX_UNIT];
	bool _ownData;
	FightEngine * _engine;
};

#endif // FIGHTANALYST_H
