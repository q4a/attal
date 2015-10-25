/****************************************************************
**
** Attal : Lords of Doom
**
** analyst.h
** analyse the game and play
**
** Version : $Id: analyst.h,v 1.35 2013/11/19 21:40:59 lusum Exp $
**
** Author(s) : Pascal Audoux - Carlo
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
//#include <QAbstractSocket>
#include <QList>
#include <QObject>
#include <QStack>
#include <QThread>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/attalSocket.h"
#include "libCommon/gameData.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/log.h"

#include "libServer/fightAnalyst.h"
#include "libAi/aiLord.h"

extern Log aifLog;

class AttalSocket;
class FightAnalyst;
class GenericPlayer;
class GenericLord;
class GenericMap;
class Calendar;
class PathFinder;

/*              ------------------------------
 *                         Analyst
 *              ------------------------------ */

/** comment for the class */
class Analyst : public QThread, public GameData
{
	Q_OBJECT
public:
		
	/** Constructor */
	Analyst();
	
	/** Destructor */
	~Analyst();
	
	void initLords();
	
	/* connect Ai to server ( port & host , in this order to permit default parameters */	
	void connectAi( QString host, int port );

	/* disconnect Ai to server */	
	void disconnectAi();

	QAbstractSocket::SocketState getSocketState() { return _socket->state(); }

	AttalSocket * getSocket () { return _socket; }
	
	uchar getCla1();
	uchar getCla2();
	uchar getCla3();

	/** Handle SO_MSG socket data */
	void socketMsg();

	/** Handle SO_CONNECT socket data */
	void socketConnect();
	void socketConnectName();

	/** Handle SO_MVT socket data */
	void socketMvt();
	
	/** Handle SO_TECHNIC socket data */
	void socketTechnic();
	
	/** Handle SO_FIGHT socket data */
	void socketFight();
	void socketFightInit();
	void socketFightEnd();
	
	/** Handle SO_QR socket data */
	void socketQR();
	void socketQRMsgNext();
	void socketQRMsgEnd();
	
	/** Handle SO_EXCH socket data */
	void socketExch();

	void socketGameInfo();

	void socketGameTavern();
	void socketGameTavernInfo();
	void socketGameTavernLord();

	void socketGameCalendar();
	
	/** Handle SO_MODIF socket data */
	void socketModif();
	void socketModifMap();
	void socketModifCell();

	void socketModifLord();
	void socketModifLordVisit();
	void socketModifLordNew();
	void socketModifLordUnit();
	void socketModifLordRemove();
	void socketModifLordCharac();
	void socketModifLordGarrison();
	void socketModifLordMachine();

	void socketModifBase();
	void socketModifBaseNew();
	void socketModifBaseOwner();
	void socketModifBaseName();
	void socketModifBaseBuilding();
	void socketModifBaseUnit();
	void socketModifBaseResources();
	void socketModifBaseProduction();
	void socketModifBaseState();

	void socketModifBuilding();
	void socketModifBuildingNew();
	void socketModifBuildingOwner();
	void socketModifBuildingResources();

	void socketModifArtefact();
	void socketModifPlayer();

	void socketModifCreature();
	void socketModifCreatureNew();
	void socketModifCreatureUpdate();
	void socketModifCreatureDel();
	void socketModifCreatureRess();

	void socketModifEvent();
	void socketModifEventNew();
	void socketModifEventDel();

	/** Handle SO_TURN socket data */
	void socketTurn();
	void socketTurnBegin();
	void socketTurnPlay();
	void socketTurnLord();

	/** Handle SO_GAME socket data */
	void socketGame();
	void socketGameEnd();
	void socketGameLost();
	void socketGameWin();
	void socketGameBegin();

	
	/* Return exact lord power */
	int getPower(AiLord *lord);

	void exchangeStart();

	/** manage exchange units */
	void exchangeUnits();
	
	/** manage exchange artefact */
	void exchangeArtefact();
	
	void exchangeBaseUnits();

	void manageBase(GenericBase * base);
	void manageBaseCreature(GenericBase *base, InsideAction * action);
	void manageBaseMarket();
	
	void enterBase(AiLord * lord,GenericBase * base );


public slots:

	/** Slot for reading sockets */
	void slot_readSocket();

signals:
	/** signal if ai is connected */
	void sig_connected();

	/** signal if ai is disconnected */
	void sig_disconnected();

private slots:
	
	void slot_connectionClosed();

	void slot_connected();

	void slot_error( QAbstractSocket::SocketError error );
	
private:	
	
	void initSocket();

	void reinitAi();

	void playLordTurn(int nlord);

	void analyzeLord( AiLord * lord );
	
	void analyzeLordMap( AiLord * lord );

	void sendLordTurn( int opt );

	GenericCell * randomPath(GenericCell * cell, int move );
	
	GenericCell * fleePath( AiLord * lord );

	GenericCell * findNotExploredCell(GenericCell * cell);

	void tuneAiPower( uint opPower );

	void manageMeetingsLord( AiLord * lord, AiLord * opponent );

	void manageMeetingsThings( AiLord * lord, GenericCell * cell, LordPriority prioType );

	/* manageMeetings
	 * with 
	 * lord =  our lord
	 * cell = target cell
	 * prior = priority of the target
	 * opPower = opponent power, 0 if no opponent
	 * isFlee = our lord flee if enemy is more powerful? true yes, false no (only care with opPower>0)
	 */
	 
	void manageMeetings( AiLord * lord, GenericCell * cell,int prior,uint opPower, bool isFlee );

	/* tell if the ai is in her turn (for coordinate lord's turn) */
	bool InTurn() { return _numTurn>=0 ? true : false; }

	AttalSocket * _socket;
	
	int _port;
 
	FightAnalyst * _fight;
	GenericBase * _basetav;
	QString _msg;
	QList<AiLord *> _lordtav;
	QList<GenericCell *> _tabuCells;
	uchar _nbLord, _nbLordRead;
	int _currentLord;
	int _lastLord;
	
	int _status;
	int _numlords;

	int _numTurn;
	uint _SuffisantPower;
	int _totalTurn;
	uint _maxExplRadius;
	
	//GenericMap * _map;
	//AiLord ** _lords;
	//QPtrList<GenericArtefact> _artefacts;
};

#ifdef WIN32VC6
#define ialogV aifLog.ialog
#define ialogN aifLog.ialog
#define ialogC aifLog.ialog

#else

/** Log with level */
#define ialogV( format, args... ) aifLog.ialog( LOG_VERBOSE, format, ## args )
#define ialogN( format, args... ) aifLog.ialog( LOG_NORMAL, format, ## args )
#define ialogC( format, args... ) aifLog.ialog( LOG_CRITICAL, format, ## args )

#endif // VC6

#endif // ANALYST_H   
