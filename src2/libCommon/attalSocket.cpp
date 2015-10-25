/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocket.cpp
** socket for Attal over network
**
** Version : $Id: attalSocket.cpp,v 1.65 2008/09/07 16:08:58 lusum Exp $
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

#include "attalSocket.h"

 
// generic include files
#include <algorithm>

// include files for QT
// application specific includes
#include "libCommon/artefact.h"
#include "libCommon/calendar.h"
#include "libCommon/creature.h"
#include "libCommon/genericBonus.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericChest.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericFightCell.h"
#include "libCommon/genericInsideBuilding.h"
#include "libCommon/genericLord.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/unit.h"

//
// ----- AttalSocket -----
//

AttalSocket::AttalSocket()
{
}
	
AttalSocket::~AttalSocket()
{
}

void AttalSocket::send()
{
	_bufOut[0] = _lenOut-1;
	write( _bufOut, _lenOut );
	_bufOut[1] = 0;
	_bufOut[2] = 0;
	_bufOut[3] = 0;
	_lenOut = 4;
}

void AttalSocket::readData()
{
	uchar len;
	getChar( (char *) & len );
	_lenIn = 3;
	read( _bufIn, len );
}

AttalSocketData AttalSocket::readBufferData()
{
	AttalSocketData data;
	uchar len;
	getChar( (char *) & len );

	read( data._bufIn, len );
	setData( &data );
	return data;
}

void AttalSocket::internalSendMessage( QString msg )
{
	appendChar( (uchar) std::min( msg.length(), 250 ) );
	/// XXX: we suppose small messages for the moment <= 250
	for( int i = 0; i < std::min( msg.length(), 250 ); i++ ) {
		appendChar( (uchar) (msg.toLatin1())[i] );
	}
	send();
}

void AttalSocket::sendMessage( QString msg )
{ 
	init( (uchar) SO_MSG, ( uchar)C_MSG_NORM );
	internalSendMessage( msg );
}

void AttalSocket::sendFightMessage( QString msg )
{ 
	init( (uchar) SO_MSG, ( uchar)C_MSG_FIGHT );
	internalSendMessage( msg );
}

void AttalSocket::sendConnMessage( QString msg )
{
	init( (uchar) SO_MSG, ( uchar)C_MSG_CONN );
	internalSendMessage( msg );
}

void AttalSocket::sendConnectionOk() 
{
	init( (uchar)SO_CONNECT, (uchar)C_CONN_OK );
	send();
}

void AttalSocket::sendConnectionId( char id )
{
	init( (uchar)SO_CONNECT, (uchar)C_CONN_ID );
	appendChar( id );
	send();
}

void AttalSocket::sendConnectionName( QString name )
{
	init( (uchar) SO_CONNECT, (uchar) C_CONN_NAME );
	appendChar( (uchar) std::min( name.length(), 250 ) );
	
	for( int i = 0; i < std::min( name.length(), 250 ); i++ ) {
	        appendChar( (uchar) (name.toLatin1())[i] );
	}
	send();
}

void AttalSocket::sendConnectionVersion( QString name )
{
	init( (uchar) SO_CONNECT, (uchar) C_CONN_VERSION );
	appendChar( (uchar) std::min( name.length(), 250 ) );
	
	for( int i = 0; i < std::min( name.length(), 250 ); i++ ) {
	        appendChar( (uchar) (name.toLatin1())[i] );
	}
	send();
}

void AttalSocket::sendConnectionPlayer( GenericPlayer * player )
{
	init( (uchar) SO_CONNECT, (uchar) C_CONN_PLAYER );
	
	appendChar( (uchar) player->getNum() );

	QString name = player->getName();
	appendChar( (uchar) std::min( name.length(), 250 ) );

	for( int i = 0; i < std::min( name.length(), 250 ); i++ ) {
		appendChar( (uchar) (name.toLatin1())[i] );
	}
	send();
}

void AttalSocket::sendExchangeStart( GenericLord * lord1, GenericLord * lord2 )
{
	init( (uchar)SO_EXCH, (uchar) C_EXCH_START );
	appendChar( (uchar)lord1->getId(), (uchar) lord2->getId() );
	send();
}

 /* NOTE: if regroup 2 same units remember that the server 
	* regroup the units on position unit2 not unit1
	*/ 
void AttalSocket::sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 )
{
	init( (uchar)SO_EXCH, (uchar)C_EXCH_UNIT );
	if( lord1 ) {
		appendChar( (uchar)lord1->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit1 );

	if( lord2 ) {
		appendChar( (uchar)lord2->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit2 );
	send();
}

void AttalSocket::sendExchangeArtefact( GenericLord * lord1, int item, GenericLord * lord2 )
{
	if( lord1 && lord2) {
		init( (uchar)SO_EXCH, (uchar)C_EXCH_ARTEFACT );
		appendChar( (uchar)lord1->getId() );
		appendInt( item );
		appendChar( (uchar)lord2->getId() );
		send();
	}
}

void AttalSocket::sendExchangeBaseUnit( GenericBase * base, int unit1, GenericLord * lord2, int unit2 )
{
	init( (uchar)SO_EXCH, (uchar)C_EXCH_BASEUNIT );
	if( base ) {
		appendChar( (uchar)base->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit1 );

	if( lord2 ) {
		appendChar( (uchar)lord2->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit2 );
	send();
}

void AttalSocket::sendExchangeBaseLordUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2, uchar direction )
{
	init( (uchar)SO_EXCH, (uchar)C_EXCH_UNIT );
	if( lord1 ) {
		appendChar( (uchar)lord1->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit1 );

	if( lord2 ) {
		appendChar( (uchar)lord2->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit2 );

	appendChar( (uchar)direction );
	send();
}

void AttalSocket::sendExchangeUnitSplit( GenericLord * lord, int unit, int num1, int pos2, int num2  )
{
	init( (uchar)SO_EXCH, (uchar)C_EXCH_UNIT_SPLIT );
	if( lord ) {
		appendChar( (uchar)lord->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit );
	appendChar( (uchar)num1 );
	appendChar( (uchar)pos2 );
	appendChar( (uchar)num2 );

	send();
}

void AttalSocket::sendExchangeBaseUnitCl( GenericBase * base, int unit1 , GenericLord * lord2, int unit2 )
{
	init( (uchar)SO_EXCH, (uchar)C_EXCH_BASEUNITCL );
	
	if( base ) {
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit1 );

	if( lord2 ) {
		appendChar( (uchar)lord2->getId() );
	} else {
		appendChar( (uchar)-1 );
	}
	appendChar( (uchar)unit2 );
	send();
}

void AttalSocket::sendPlayerResource( char res, int nb )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_PLAYER, (uchar)C_PLAY_RESS );
	appendChar( res );
	appendInt( nb );
	send();
}

void AttalSocket::sendPlayerPrice( char res, int price )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_PLAYER, (uchar)C_PLAY_PRICE );
	appendChar( res );
	appendInt( price );
	send();
}

void AttalSocket::sendAskNone( const QString & msg, uchar type )
{
	uint len = msg.length();
	
	while( len > 250 ) {
		init( (uchar) SO_QR, (uchar) C_QR_MSG_NEXT );
		appendChar( (uchar) 250 );

		/// XXX: we suppose small messages for the moment
		for( uint i = 0; i < 250; i++ ) {
			appendChar( (uchar) (msg.toLatin1())[i] );
		}
		send();
		
		len -= 250;
	}
	
	init( (uchar) SO_QR, (uchar) C_QR_MSG_END );
	appendChar( type );
	appendChar( (uchar) len );

	/// XXX: we suppose small messages for the moment
	for( uint i = 0; i < len; i++ ) {
		appendChar( (uchar) (msg.toLatin1())[i] );
	}
	send();
}

void AttalSocket::sendAskChest()
{
	init( (uchar) SO_QR, (uchar) C_QR_CHEST );
	send();
}

void AttalSocket::sendAskCreatureJoin( GenericMapCreature * creature )
{
	init( (uchar) SO_QR, (uchar) C_QR_CREATURE_JOIN );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( creature->getCategoryNumber() );
	send();
}
	
void AttalSocket::sendAskCreatureMercenary( GenericMapCreature * creature )
{
	init( (uchar) SO_QR, (uchar) C_QR_CREATURE_MERCENARY );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( creature->getCreatureNumber() );
	send();
}
		
void AttalSocket::sendAskCreatureFlee( GenericMapCreature * creature )
{
	init( (uchar) SO_QR, (uchar) C_QR_CREATURE_FLEE );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( creature->getCategoryNumber() );
	send();
}

void AttalSocket::sendBegGame( char nb ) 
{
	init( (uchar)SO_GAME, (uchar)C_GAME_BEGIN );
	appendChar( nb );
	send();
}

void AttalSocket::sendGameCalendar( Calendar * calendar ) 
{
	init( (uchar)SO_GAME, (uchar)C_GAME_CALENDAR );
	appendInt( (int) calendar->getDay() );
	appendInt( (int) calendar->getDayNumber() );
	appendInt( (int) calendar->getWeek() );
	appendInt( (int) calendar->getMonth() );
	appendInt( (int) calendar->getYear() );
	send();
}

void AttalSocket::askTavernInfo( GenericBase * base )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_TAVERN, (uchar)C_TAVERN_INFO );
	GenericCell * cell =  base->getCell();
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	send();
}
	
void AttalSocket::sendTavernInfo( uchar nb, GenericBase * base  )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_TAVERN, (uchar)C_TAVERN_INFO );
	GenericCell * cell =  base->getCell();
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	appendChar( nb );
	send();
}

void AttalSocket::askTavernLord( GenericBase * base, uchar numLord )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_TAVERN, (uchar)C_TAVERN_LORD );
	GenericCell * cell =  base->getCell();
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	appendChar( numLord );
	send();

}
	
void AttalSocket::sendTavernLord( GenericLord * lord, GenericBase * base  )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_TAVERN, (uchar)C_TAVERN_LORD );
	GenericCell * cell =  base->getCell();
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	appendInt( lord->getId() );
	/// XXX: all info must be sent
	send();

}

void AttalSocket::sendAskLevel( char lord, char level, char primSkill, char skill1, char skill2 )
{
	init((uchar) SO_QR, (uchar) C_QR_LEVEL );
	appendChar( (uchar) lord );
	appendChar( (uchar) level );
	appendChar( (uchar) primSkill );
	appendChar( (uchar) skill1 );
	appendChar( (uchar) skill2 );
	send();
}

void AttalSocket::sendLostGame( GenericPlayer * player )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_LOST );
	appendChar( player->getNum() );
	send();
}

void AttalSocket::sendWinGame( GenericPlayer * player )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_WIN );
	appendChar( player->getNum() );
	send();
}

void AttalSocket::sendEndGame()
{
	init( (uchar)SO_GAME, (uchar)C_GAME_END );
	send();
}

void AttalSocket::sendGameInfoPlayerName( GenericPlayer * player )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_INFO, (uchar)C_INFOPLAYER_NAME );
	appendChar( player->getNum() );
	/// XXX: add name
	send();
}
	
void AttalSocket::sendGameInfoPlayerTeam( GenericPlayer * player )
{
	init( (uchar)SO_GAME, (uchar)C_GAME_INFO, (uchar)C_INFOPLAYER_TEAM );
	appendChar( player->getNum() );
	appendChar( player->getTeamId() );
	send();
}

void AttalSocket::sendTurnPlaying( GenericPlayer * player )
{
	init( (uchar)SO_TURN, (uchar)C_TURN_PLAY );
	appendChar( player->getNum() );
	send();
}

void AttalSocket::sendLordPlTurn(int count)
{
	init( (uchar)SO_TURN, (uchar) C_TURN_PLORD );
	appendInt( count );
	send();
}

void AttalSocket::sendLordTurn(int flag)
{
	init( (uchar)SO_TURN, (uchar) C_TURN_LORD );
	appendInt( flag );
	send();
}

void AttalSocket::sendTurnEnd()
{
	init( (uchar)SO_TURN, (uchar) C_TURN_END );
	send();
}

void AttalSocket::sendSizeMap( const int & h, const int & w )
{
	init( (uchar)SO_MODIF, (uchar) C_MOD_MAP );
	appendInt( h );
	appendInt( w );
	send();
}

void AttalSocket::sendCell( GenericCell * cell )
{
	init( (uchar) SO_MODIF, (uchar) C_MOD_CELL );
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	appendInt( (int) cell->getType() );
	appendChar( (int) cell->getDiversification() );
	appendInt( (int) cell->getTransition() );
	appendInt( (int) cell->getTransitionCellType() );
	appendInt( (int) cell->getDecorationGroup() );
	appendInt( (int) cell->getDecorationItem() );
	send();
}

void AttalSocket::sendMvt( int lord, int cell_i, int cell_j )
{
	init( (uchar) SO_MVT, (uchar) C_MVT_ONE );
	appendChar( lord );
	appendInt( cell_i );
	appendInt( cell_j );
	send();
}

void AttalSocket::sendMvts( int lord, QList<GenericCell *> & list )
{
	init( (uchar) SO_MVT, (uchar) C_MVT_MULTI );
	appendChar( lord );
	uint nbList = list.count();
	appendInt( nbList );
	for( uint i = 0; i < nbList; i++ ) {
		appendInt( list.at( i )->getRow() );
		appendInt( list.at( i )->getCol() );
	}
	send();
}


void AttalSocket::sendTechnic( int lord, int numTechnic )
{
	init( (uchar)SO_TECHNIC );
	appendInt( lord );
	appendInt( numTechnic );
	send();
}

void AttalSocket::sendFightInit( CLASS_FIGHTER fighter, GenericLord * lord )
{
	init( (uchar)SO_FIGHT, (uchar)C_FIGHT_INIT );
	appendChar( (uchar)fighter );
	appendChar( lord->getId() );
	send();
}

void AttalSocket::sendFightCreature()
{
	init( (uchar)SO_FIGHT, (uchar)C_FIGHT_CREATURE );
	send();
}

void AttalSocket::sendFightCell( GenericFightCell * cell )
{
	init( (uchar)SO_FIGHT, (uchar)C_FIGHT_CELL );
	appendInt( cell->getRow() );
	appendInt( cell->getCol() );
	appendChar(cell->getType() );
	send();
}

void AttalSocket::sendFightLord( CLASS_FIGHTER, GenericLord * lord )
{
	/// XXX: CLASS_FIGHTER not used ?
	init( (uchar) SO_FIGHT, (uchar) C_FIGHT_LORD );
	appendChar( (uchar) lord->getId() );
	appendChar( (uchar) lord->getCharac( ATTACK ) );
	appendChar( (uchar) lord->getCharac( DEFENSE ) );
	send();
}

void AttalSocket::sendFightNewUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit ) 
{
	init( (uchar) SO_FIGHT, (uchar) C_FIGHT_UNIT );
	appendChar( (uchar) fighter );
	appendChar( (uchar) num );
	appendChar( (uchar) unit->getRace() );
	appendChar( (uchar) unit->getLevel() );
	appendInt( unit->getNumber() );
	appendChar( (uchar) unit->getMove() );
	appendInt( unit->getHealth() );
	send();
}

void AttalSocket::sendFightModifyUnit( CLASS_FIGHTER fighter, char num, GenericFightUnit * unit )
{
	init( (uchar) SO_FIGHT, (uchar) C_FIGHT_MODUNIT );
	appendChar( (uchar) fighter );
	appendChar( (uchar) num );
	appendChar( (uchar) unit->getRace() );
	appendChar( (uchar) unit->getLevel() );
	appendInt( unit->getNumber() );
	appendChar( (uchar) unit->getMove() );
	appendInt( unit->getHealth() );

	send();
}

void AttalSocket::sendFightDamageUnit( CLASS_FIGHTER attacker,
				       char numAttacker,
				       CLASS_ATTACK_TYPE attackType,
				       CLASS_FIGHTER defender,
				       char numDefender,
				       uint damage )
{
	init( (uchar) SO_FIGHT, (uchar) C_FIGHT_DAMAGE );

	appendChar( (uchar) attacker );
	appendChar( (uchar) numAttacker );

	appendChar( (uchar) attackType );

	appendChar( (uchar) defender );
	appendChar( (uchar) numDefender );

	appendInt( (int)damage );

	send();
}

void AttalSocket::sendFightUnitMove( CLASS_FIGHTER fighter, int num, GenericFightCell * cell )
{
	init( (uchar)SO_FIGHT, (uchar)C_FIGHT_MOVE );
	appendChar( (uchar) fighter );
	appendChar( (uchar) num );
	appendInt( (int) cell->getRow() );
	appendInt( (int) cell->getCol() );
	send();
}

void AttalSocket::sendFightUnitEndMove()
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_ENDMOVE );
	send();
}

void AttalSocket::sendFightActivate( CLASS_FIGHTER fighter, char unit )
{
	init( (uchar)SO_FIGHT, (uchar)C_FIGHT_ACTIVE );
	appendChar( fighter, (uchar) unit );
	send();
}

void AttalSocket::sendFightUnitDamage( CLASS_FIGHTER fighter, char unit, int damage )
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_DAMAGE );
	appendChar( (uchar) fighter, (uchar) unit );
	appendInt( damage );
	send();
}

void AttalSocket::sendFightDistAttack( CLASS_FIGHTER fighter, char unit )
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_DISTATTACK );
	appendChar( (uchar) fighter, (uchar) unit );
	send();
}

void AttalSocket::sendFightWait()
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_WAIT );
	send();
}

void AttalSocket::sendFightFlee()
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_FLEE );
	send();
}

void AttalSocket::sendFightDefend()
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_DEFEND );
	send();
}

void AttalSocket::sendFightEnd( FightResultStatus result )
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_END );
	appendChar( result.toUChar() );
	send();
}

void AttalSocket::sendAnswer( char resp )
{
	init( SO_QR, (uchar) C_QR_ANSWER );
	appendChar( (uchar) resp );
	send();
}

void AttalSocket::sendAnswerYesNo( bool answer )
{
	init( SO_QR, (uchar) C_QR_ANSWER, (uchar) ANSWER_YESNO );
	appendChar( (uchar) answer );
	send();
}

void AttalSocket::sendAnswerEnum( char answer )
{
	init( SO_QR, (uchar) C_QR_ANSWER, (uchar) ANSWER_ENUM );
	appendChar( (uchar) answer );
	send();
}

void AttalSocket::sendLordVisit( GenericLord * lord, bool state )
{
	init( (uchar) SO_MODIF, (uchar)C_MOD_LORD, (uchar) C_LORD_VISIT );
	appendChar( lord->getOwner()->getNum() );
	appendInt( lord->getCell()->getRow() );
	appendInt( lord->getCell()->getCol() );
	appendChar( lord->getId() );
	if( state ) {
		appendChar( 1 );
	} else {
		appendChar( 0 );
	}
	send();
}

void AttalSocket::sendLordNew( GenericLord * lord  )
{
	init( (uchar)SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_NEW );
	if( lord->getCell() ) {
		appendInt( lord->getCell()->getRow() );
		appendInt( lord->getCell()->getCol() );
	} else {
		appendInt( 0 );
		appendInt( 0 );
	}
	appendChar( lord->getId() );
	send();
}

void AttalSocket::sendLordBuy( GenericLord * lord , uint row, uint col)
{
	sendLordBuy( lord->getId() , row, col);
}

void AttalSocket::sendLordBuy( uchar id , uint row, uint col)
{
	init( (uchar)SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_BUY );
	appendChar( (uchar) id );
	appendInt( (uint) row );
	appendInt( (uint) col );
	send();
}

void AttalSocket::sendLordBaseCharac( GenericLord * lord, LordCharac charac )
{
	if( charac >= LAST ) {
		logEE( "sendLordCharac with unknown charac lord %p , charac %d", lord, charac );
		return;
	}
	
	init( (uchar)SO_MODIF, (uchar) C_MOD_LORD, C_LORD_CHARAC );
	appendChar( (uchar) lord->getId() );
	appendChar( (uchar) charac );
	appendInt( lord->getBaseCharac( charac ) );

	send();
}


void AttalSocket::sendLordUnits( GenericLord * lord )
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		sendLordUnit( lord, i );
	}
}

void AttalSocket::sendLordUnit( GenericLord * lord, int num )
{
	GenericFightUnit * temp = lord->getUnit( num );
	init( (uchar)SO_MODIF, (uchar)C_MOD_LORD, (uchar)C_LORD_UNIT );
	if( temp ) {
		appendChar( lord->getId() );
		appendChar( num );
		appendChar( temp->getRace() );
		appendChar( temp->getLevel() );
		appendInt( temp->getNumber() );
		appendChar( temp->getMove() );
		appendInt( temp->getHealth() );
	} else {
		appendChar( lord->getId() );
		appendChar( num );
		appendChar( 0 );
		appendChar( 0 );
		appendInt( 0 );
		appendChar( 0 );
		appendInt( 0 );
	}
	send();
}

void AttalSocket::sendLordUnit( GenericLord * lord, int num , int value)
{
	GenericFightUnit * temp = lord->getUnit( num );
	if( temp ) {
		init( (uchar)SO_MODIF, (uchar)C_MOD_LORD, (uchar)C_LORD_UNIT );
		appendChar( lord->getId() );
		appendChar( num );
		appendChar( temp->getRace() );
		appendChar( temp->getLevel() );
		appendInt( value );
		appendChar( temp->getMove() );
		appendInt( temp->getHealth() );
		send();
	}
}



void AttalSocket::sendLordRemove( GenericLord * lord )
{
	init( (uchar) SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_REMOVE );
	appendChar( (uchar) lord->getId() );
	send();
}

void AttalSocket::sendLordGarrison( GenericLord * lord, bool state )
{
	init( (uchar) SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_GARRISON );
	appendChar( (uchar) lord->getId() );
	if( state ) {
		appendChar( (uchar) 1 );
	} else {
		appendChar( (uchar) 0 );
	}
	send();
}

void AttalSocket::sendLordMachine( GenericLord * lord, int id )
{
	init( (uchar) SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_MACHINE );
	appendChar( (uchar) lord->getId() );
	appendChar( (uchar) id );
	send();
}

void AttalSocket::sendBaseNew( GenericBase * base )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_NEW );
	if( base->getCell() ) {
		appendChar( base->getRace() );
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
	} else {
		appendChar( 0 );
		appendInt( 0 );
		appendInt( 0 );
	}
	appendChar( base->getId() );
	appendChar( (uchar)base->getForbiddenBuildingsNumber() );
	for( uint i = 0; i < base->getForbiddenBuildingsNumber(); i++ ) {
		appendChar( base->getForbiddenBuilding( i ) );
	}

	send();
}

void AttalSocket::sendBaseName( GenericBase * base )
{
	QString name;
	name = base->getName();
	
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_NAME );
	
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendInt( name.length() );
	
	for( int i = 0; i < name.length(); i++ ) {
		appendChar( (uchar) (name.toLatin1())[i] );
	}

	send();
}


void AttalSocket::sendBaseOwner( GenericBase * base, GenericPlayer * player )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_OWNER );
	// TODO x,y --> getId
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( player->getNum() );
	send();
}

void AttalSocket::sendBaseResource( GenericBase * base , char res, int nb )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_RESS );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( res );
	appendInt( nb );
	send();
}

void AttalSocket::sendBaseProduction( GenericBase * base, Creature * creature )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_PRODUCTION );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( base->getCreatureProduction( creature )  );
	send();
}

void AttalSocket::sendBaseState( GenericBase * base,  GenericBase::BaseState type, bool state )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_STATE );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendInt( type );
	appendChar( state );
	send();
}

void AttalSocket::sendBaseBuilding( GenericBase * base, GenericInsideBuilding * building, bool create )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_BUILDING );
	if( base && building ) {
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
		appendChar( building->getLevel() );
		appendChar( (uchar)create );
		send();
	} else {
		logEE( "Base and Building should not be null in sendBaseBuilding" );
	}
}

void AttalSocket::sendBuildingNew( GenericBuilding * build )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BUILD, (uchar)C_BUILD_NEW );
	appendChar( build->getType() );
	appendInt( build->getId() );
	appendInt( build->getCell()->getRow() );
	appendInt( build->getCell()->getCol() );
	send();
}

void AttalSocket::sendBuildingResource( GenericBuilding * build , char res, int nb )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BUILD, (uchar)C_BUILD_RESS );
	appendInt( build->getCell()->getRow() );
	appendInt( build->getCell()->getCol() );
	appendChar( res );
	appendInt( nb );
	send();
}



void AttalSocket::sendBaseUnit( GenericBase * base, GenericFightUnit * unit, int pos )
{
	if(unit){
		init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT );
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
		appendChar( pos );
		appendChar( unit->getRace() );
		appendChar( unit->getLevel() );
		appendInt( unit->getNumber() );
		send();
	}
}

void AttalSocket::sendBaseUnit( GenericBase * base, Creature * creature, int number, int pos )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( pos );
	if( creature ) {
		appendChar( creature->getRace() );
		appendChar( creature->getLevel() );
		appendInt( number );
	} else {
		//race and level are not important if not exist creature of number is 0 cause we simply destroy every unit in position pos
		appendChar( 0 );
		appendChar( 0 );
		appendInt( 0 );
	}
	send();
}

void AttalSocket::sendBaseUnitBuy( GenericBase * base, GenericFightUnit * unit )
{
	if(unit){
		init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT_BUY );
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
		appendChar( unit->getRace() );
		appendChar( unit->getLevel() );
		appendInt( unit->getNumber() );
		send();
	}
}

void AttalSocket::sendBaseUnitBuy( GenericBase * base, Creature * creature, int number )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT_BUY );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( number );
	send();
}

void AttalSocket::sendBaseMarket( int ownRes, int otherRes, int valueFirstRes )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_MARKET );
	appendInt( ownRes );
	appendInt( otherRes );
	appendInt( valueFirstRes );
	send();
}

void AttalSocket::sendBuildingOwner( GenericBuilding * build, GenericPlayer * player )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BUILD, (uchar)C_BUILD_OWNER );
	appendInt( build->getCell()->getRow() );
	appendInt( build->getCell()->getCol() );
	if( player ) {
		appendInt( player->getNum() );
	} else {
		appendInt( GenericPlayer::NO_PLAYER );
	}
	send();
}

/*
void AttalSocket::sendNewLordArtefact( GenericLordArtefact * artefact )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_NEWLORD );
	appendChar( artefact->getType() );
	appendChar( artefact->getLord()->getId() );

        send();
}
*/

void AttalSocket::sendArtefactLord( GenericLordArtefact * artefact, bool state )
{
	if( state ) {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_ADDLORD );
	} else {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_DELLORD );
	}
	appendInt( artefact->getType() );
	appendChar( artefact->getLord()->getId() );
	send();
}

void AttalSocket::sendNewCreature( GenericMapCreature * creature )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_CREATURE, (uchar)C_CRE_NEW );
	appendInt( creature->getCell()->getRow() );
	appendInt( creature->getCell()->getCol() );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( creature->getCategoryNumber() );
	appendChar( (uchar)creature->isLookingRight() );
	send();
}

void AttalSocket::sendUpdateCreature( GenericMapCreature * creature )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_CREATURE, (uchar)C_CRE_UPDATE );
	appendInt( creature->getCell()->getRow() );
	appendInt( creature->getCell()->getCol() );
	appendInt( creature->getCategoryNumber() );
	send();
}

void AttalSocket::sendDelCreature( GenericCell * cell )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_CREATURE, (uchar)C_CRE_DEL );
	appendInt( cell->getRow() );
	appendInt( cell->getCol() );
	send();
}

void AttalSocket::sendCreatureResource( GenericMapCreature * creature, char res, int nb )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_CREATURE, (uchar)C_CRE_RESS );
	appendInt( creature->getCell()->getRow() );
	appendInt( creature->getCell()->getCol() );
	appendChar( res );
	appendInt( nb );
	send();
}

void AttalSocket::sendNewEvent( GenericEvent * event )
{
	uint i;
	init( (uchar)SO_MODIF, (uchar)C_MOD_EVENT, (uchar)C_EVENT_NEW );
	appendInt( event->getCell()->getRow() );
	appendInt( event->getCell()->getCol() );
	appendChar( event->getType() );
	switch( event->getType() ) {
	case GenericEvent::EventArtefact: {
			GenericArtefact * artefact = event->getArtefact();
			appendInt( artefact->getId() );
			appendChar( artefact->getType() );
		} break;
	case GenericEvent::EventBonus: {
		GenericBonus * bonus = event->getBonus();
		appendChar( bonus->getType() );
		appendChar( (uchar) bonus->getParamNumber() );
		for( i = 0; i < bonus->getParamNumber(); i++ ) {
			appendInt( bonus->getParam( i ) );
		}
		} break;
	case GenericEvent::EventChest: {
		GenericChest * chest = event->getChest();
		appendChar( (uchar) chest->getParamNumber() );
		for( i = 0; i < chest->getParamNumber(); i++ ) {
			appendInt( chest->getParam( i ) );
		}
		} break;
	default:
		logEE( "Should not happen" );
		break;
	}
	send();

}

void AttalSocket::sendDelEvent( GenericEvent * event )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_EVENT, (uchar)C_EVENT_DEL );
	appendInt( event->getCell()->getRow() );
	appendInt( event->getCell()->getCol() );
	send();
}

void AttalSocket::requestBuilding( GenericBase * base, int building, bool isBuy )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_BUILDING );
	appendChar( base->getId() );
	appendChar( building );
	appendChar( isBuy );
	send();
}

