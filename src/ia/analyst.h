/****************************************************************
**
** Attal : Lords of Doom
**
** analyst.h
** analyse the game and play
**
** Version : $Id: analyst.h,v 1.29 2003/04/07 08:50:09 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/02/2001
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

 
#ifndef ANALYST_H
#define ANALYST_H
#define DIM 100
 
 
// generic include files
// include files for QT
// application specific include files
#include "common/artefact.h"
#include "common/attalSocket.h"

#include "server/gameData.h"

class AttalSocket;
class FightAnalyst;
class GenericLord;
class GenericPlayer;
class GenericMap;
class Calendar;

enum {
	NOREACH = 0,
	REACH
};

enum {
	PR_ENEMY = 0,
	PR_BASE,
	PR_BUILD,
	PR_ART,
	PR_CREAT,
	PR_EVENT
};

/*              ------------------------------
 *                         Analyst
 *              ------------------------------ */

/** comment for the class */
class Analyst : public GameData
{

public:
	/** Constructor */
	Analyst();

	/** Set socket for exchanges with server */
	void setSocket( AttalSocket * sock ) {
		_socket = sock;
	}

	/** Handle SO_MSG socket data */
	void socketMsg();

	/** Handle SO_CONNECT socket data */
	void socketConnect();

	/** Handle SO_MVT socket data */
	void socketMvt();
	
	/** Handle SO_TECHNIC socket data */
	void socketTechnic();
	
	/** Handle SO_FIGHT socket data */
	void socketFight();
	
	/** Handle SO_QR socket data */
	void socketQR();
	
	/** Handle SO_EXCH socket data */
	void socketExch();
	
	/** Handle SO_MODIF socket data */
	void socketModif();
	void socketModifLord();
	void socketModifLordVisit();
	void socketModifLordNew();
	void socketModifLordUnit();
	void socketModifLordRemove();
	void socketModifBase();
	void socketModifBaseNew();
	void socketModifBaseOwner();
	void socketModifBaseBuilding();
	void socketModifBaseUnit();
	void socketModifBuilding();
	void socketModifArtefact();
	void socketModifPlayer();
	void socketModifCreature();
	void socketModifEvent();

	/** Handle SO_TURN socket data */
	void socketTurn();
	void socketTurnBegin();

	/** Handle SO_GAME socket data */
	void socketGame();
	
	/* Return random lord power */
	int getRandPower(GenericLord *lord);
	
	/* Return exact lord power */
	int getPower(GenericLord *lord);

	void exchangeStart();

	/*manage exchange units*/
	void exchangeUnits();
	
	void exchangeBaseUnits();
	
	void manageBase(GenericBase * base);
	
	void enterBase(GenericLord * lord,GenericBase * base );

	/* followPath
	 * with flags 
	 * 0 tell if a map cell is reachable
	 * 1 follow the minimal path between two cells
	 * return:
	 * NOREACH if unreachable
	 * REACH if reachable
	 */
	 
	int followPath(GenericCell *icell,GenericCell *dcell,GenericLord *lord,int maxm,int flags);

	//GenericLord * getLord( uint num );

private:
	GenericArtefact * getArtefactById( int id );

	void playLordTurn(int nlord);

	void analyzeLord( GenericLord * lord );

	AttalSocket * _socket;
	FightAnalyst * _fight;
	GenericPlayer * _player;
	Calendar * _calendar;
	GenericCell * _destinationCell;
	
	int _fightCpt;
	int _numlords;
	int _mincost;
	int _movePoints;
	int _startApprRow,_startApprCol;

	int _nbTry;
	int _prior;

	//GenericMap * _map;
	//GenericLord ** _lords;
	//QList<GenericArtefact> _artefacts;
};
 
#endif // ANALYST_H   
