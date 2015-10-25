/****************************************************************
**
** Attal : Lords of Doom
**
** fightAnalyst.h
** analyst for the fight phase
**
** Version : $Id: fightAnalyst.h,v 1.10 2003/02/13 20:15:18 lusum Exp $
**
** Author(s) : Pascal Audoux
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
#include "common/attalSocket.h"
#include "common/define.h"


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
	FightAnalyst( GameData * data );

	/** constructor for creatures fights */
	FightAnalyst( GameData data, FightEngine * engine );

	/** Destructor */
	virtual ~FightAnalyst();

	void setSocket( AttalSocket * sock ) {
		_socket = sock;
	}

	void handleFightSocket();

	void handleFightData( FakeSocket * data );

	GenericFightCell * getOpponentCell()  {return _opponentCell;}

	GenericFightUnit * getOpponentUnit()  {return _opponentUnit;}

	bool isDist() { return _distAttack; }

	void initCreatures( GenericLord * lord );

protected:
	void handleInit();

	void handleOpponent();

	void handleCell();

	void handleNewUnit();

	void handleMove();

	void handleDamage();

	void handleActive();

	void playUnit( GenericFightUnit * unit , int pos,int flags);
	
	void updateUnits( void );
	
	GenericFightUnit * getUnit( int num, CLASS_FIGHTER fighter );

	bool _oppIsAtt;

	bool _distAttack;

	bool _isCreature;

	AttalSocket * _socket;
	GameData * _data;
	GenericFightMap * _map;
	GenericFightCell * _opponentCell;
	CLASS_FIGHTER _fighter;
	GenericLord * _lordAttack, * _lordDefense, * _lordOpp;
	GenericFightUnit * _unitsAtt[MAX_UNIT], * _unitsDef[MAX_UNIT];
	GenericFightUnit * _opponentUnit;
	bool _ownData;
	FightEngine * _engine;
};

#endif // FIGHTANALYST_H
