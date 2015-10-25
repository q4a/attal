/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocket.h
** socket for Attal over network
**
** Version : $Id: attalSocket.h,v 1.43 2008/09/07 16:08:58 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 31/10/2000
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


#ifndef ATTALSOCKET_H
#define ATTALSOCKET_H


// generic include files
// include files for QT
#include <QList>
#include <QTcpSocket>
// application specific include
#include "libCommon/attalSocketData.h"
#include "libCommon/genericBase.h"
#include "libCommon/fightResultStatus.h"
#include "libCommon/genericLord.h"


class Creature;
class Calendar;
class GenericArtefact;
class GenericBuilding;
class GenericCell;
class GenericEvent;
class GenericFightCell;
class GenericFightUnit;
class GenericInsideBuilding;
class GenericLord;
class GenericLordArtefact;
class GenericMapCreature;
class GenericPlayer;

/*              ------------------------------
 *                         AttalSocket
 *              ------------------------------ */

class AttalSocket : public QTcpSocket,public AttalSocketData
{
 
public:
	/** Constructor */
	AttalSocket();
	
	~AttalSocket();

	/** Send message trough socket */
	void sendMessage( QString msg );

	void sendFightMessage( QString msg );
	
	void sendConnMessage( QString msg );
	
	/** Send connection ok */
	void sendConnectionOk();
	
	/** Send connection id */
	void sendConnectionId( char id );
	
	/** Send connection name */
	void sendConnectionName( QString name );

	/** Send connection player */
	void sendConnectionPlayer( GenericPlayer * player );

	void sendConnectionVersion( QString name );
	
	/** Start exchange between lords */
	void sendExchangeStart( GenericLord * lord1, GenericLord * lord2 );

	/** Exchange 2 units between one or two lords */
	void sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );
	
	/** Exchange 2 artefacts between one or two lords */
	void sendExchangeArtefact( GenericLord * lord1, int item, GenericLord * lord2 );

	/** Exchange 2 units inside a base */
	void sendExchangeBaseUnit( GenericBase * base, int unit1, GenericLord * lord2, int unit2 );

	/** Split unit */
	void sendExchangeUnitSplit( GenericLord * lord, int unit, int num1, int pos2, int num2  );

	/** Exchange 2 units between one base and a lord */
	void sendExchangeBaseLordUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2, uchar direction );
	
	void sendExchangeBaseUnitCl( GenericBase * base, int unit1, GenericLord * lord2, int unit2 );
	
	/** Send ressources of a player */
	void sendPlayerResource( char res, int nb );

	/** Send prices of a player */
	void sendPlayerPrice( char res, int price );

	/** Send 'Game begins' info */
	void sendBegGame( char nb );
	
	/** XXX: */
	void askTavernInfo( GenericBase * base );
	
	void sendTavernInfo( uchar nb, GenericBase * base   );
	
	void askTavernLord( GenericBase * base, uchar numLord );
	
	void sendTavernLord( GenericLord * lord, GenericBase * base   );

	/** Send the 'Player lost' info */
	void sendLostGame( GenericPlayer * player );

	/** Send the 'Player win' info */
	void sendWinGame( GenericPlayer * player );

	/** Send the 'Game ends' info */
	void sendEndGame();
	
	void sendGameInfoPlayerName( GenericPlayer * player );
	
	void sendGameInfoPlayerTeam( GenericPlayer * player );
	
	void sendGameCalendar( Calendar * calendar);
	
	/** Send info about who's playing */
	void sendTurnPlaying( GenericPlayer * player );
	
	/** Send 'Turn ends' info */
	void sendTurnEnd();

	void sendLordPlTurn(int count);

	void sendLordTurn(int flag);
	
	/** Send the size of the map */
	void sendSizeMap( const int & h, const int & w );
	
	/** Send a cell */
	void sendCell( GenericCell * cell );
	
	/** Send a movement of a lord */
	void sendMvt( int lord, int cell_i, int cell_j );

	/** Send all the movements of a lord */
	void sendMvts( int lord, QList<GenericCell *> & list );

	/** A lord is using a technic */
	void sendTechnic( int lord, int numTechnic );

	/** Fight is starting */
	void sendFightInit( CLASS_FIGHTER fighter, GenericLord * lord );

	void sendFightCreature();

	/** Send a fight cell */
	void sendFightCell( GenericFightCell * cell );

	void sendFightLord( CLASS_FIGHTER fighter, GenericLord * lord );

	void sendFightNewUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit );

	void sendFightModifyUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit );

	void sendFightDamageUnit( CLASS_FIGHTER attacker,
				  char numAttacker,
				  CLASS_ATTACK_TYPE attackType,
				  CLASS_FIGHTER defender,
				  char numDefender,
				  uint damage );

	void sendFightUnitMove( CLASS_FIGHTER fighter, int num, GenericFightCell * cell );

	/** End of a movement */
	void sendFightUnitEndMove();

	/** Activate a unit for fighting */
	void sendFightActivate( CLASS_FIGHTER fighter, char unit );

	/** Damages for the unit */
	void sendFightUnitDamage( CLASS_FIGHTER fighter, char unit, int damage );

	void sendFightDistAttack( CLASS_FIGHTER fighter, char unit );

	void sendFightWait();

	void sendFightFlee();

	void sendFightDefend();

	/** Fight ends */
	void sendFightEnd( FightResultStatus result );

	/** Send message of info */
	void sendAskNone( const QString & msg, uchar type );

	void sendAskChest();
	
	void sendAskCreatureJoin( GenericMapCreature * creature );
	
	void sendAskCreatureMercenary( GenericMapCreature * creature );
		
	void sendAskCreatureFlee( GenericMapCreature * creature );

	/** Ask skill when level upgrade */
	void sendAskLevel( char lord, char level, char primSkill, char skill1, char skill2 );

	/** Send answer to question */
	void sendAnswer( char resp );

	/** Send answer to enum question */
	void sendAnswerYesNo( bool answer );

	/** Send answer to enum question */
	void sendAnswerEnum( char answer );
	
	/** Lord is visiting */
	void sendLordVisit( GenericLord * lord, bool state = true );

	/** New lord */
	void sendLordNew( GenericLord * lord );

	/** Buy lord */
	void sendLordBuy( GenericLord * lord , uint row, uint col );
	
	void sendLordBuy( uchar id , uint row, uint col );

	void sendLordBaseCharac( GenericLord * lord, LordCharac charac );

	/** Send units */
	void sendLordUnits( GenericLord * lord );

	/** Send unit 'num' */
	void sendLordUnit( GenericLord * lord, int num );
	
	/** Send unit 'num' */
	void sendLordUnit( GenericLord * lord, int num , int value);

	void sendLordRemove( GenericLord * lord );

	void sendLordGarrison( GenericLord * lord, bool state );

	void sendLordMachine( GenericLord * lord, int id );

	/** New base */
	void sendBaseNew( GenericBase * base );
	
	/** Base name */
	void sendBaseName( GenericBase * base );

	/** New owner for the base */
	void sendBaseOwner( GenericBase * base, GenericPlayer * player );

	void sendBaseBuilding( GenericBase * base, GenericInsideBuilding * building, bool create );

	/** New building */
	void sendBuildingNew( GenericBuilding * build );
	
	void sendBuildingResource( GenericBuilding * build, char res, int nb );

	/** New unit to base */
	void sendBaseUnit( GenericBase * base, GenericFightUnit * unit, int pos );

	/** New unit to base */
	void sendBaseUnit( GenericBase * base, Creature * creature, int number, int pos );

	/** Request to buy new unit */
	void sendBaseUnitBuy( GenericBase * base, Creature * creature, int number );

	/** Request to buy new unit */
	void sendBaseUnitBuy( GenericBase * base, GenericFightUnit * unit );

	void sendBaseResource( GenericBase * base, char res, int nb );

	/** Update production of a base */
	void sendBaseProduction( GenericBase * base , Creature * creature);

	void sendBaseState( GenericBase * base, GenericBase::BaseState type, bool state );
	
	/** Marker value */
	void sendBaseMarket( int ownRes, int otherRes, int valueFirstRes );

	/** New owner for the building */
	void sendBuildingOwner( GenericBuilding * build, GenericPlayer * player );

	/** New artefact */
        //void sendNewLordArtefact( GenericLordArtefact * artefact );

	void sendArtefactLord( GenericLordArtefact * artefact, bool state );

	/** new creature on map */
	void sendNewCreature( GenericMapCreature * creature );

	/** updates creature on map */
	void sendUpdateCreature( GenericMapCreature * creature );
	
	/** delete creature of map */
	void sendDelCreature( GenericCell * cell );
	
	void sendCreatureResource( GenericMapCreature * creature, char res, int nb );

	/** new event on map */
	void sendNewEvent( GenericEvent * event );

	/** delete event on map */
	void sendDelEvent( GenericEvent * event );

	// Request client -> server
	void requestBuilding( GenericBase * base, int building, bool isBuy);
	
	void readData();
	AttalSocketData readBufferData();

protected:
	virtual void send();

private:
	void internalSendMessage( QString msg );
	
};
 
#endif // ATTALSOCKET_H
