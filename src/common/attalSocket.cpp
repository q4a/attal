/****************************************************************
**
** Attal : Lords of Doom
**
** attalSocket.cpp
** socket for Attal over network
**
** Version : $Id: attalSocket.cpp,v 1.51 2003/02/03 22:18:40 audoux Exp $
**
** Author(s) : Pascal Audoux
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
// include files for QT
// application specific includes
#include "common/artefact.h"
#include "common/creature.h"
#include "common/genericCell.h"
#include "common/genericEvent.h"
#include "common/genericFightCell.h"
#include "common/genericInsideBuilding.h"
#include "common/genericLord.h"
#include "common/genericMapCreature.h"
#include "common/genericPlayer.h"
#include "common/unit.h"

//
// ----- AttalSocket -----
//

AttalSocket::AttalSocket()
{
	_lenOut = 4;
        _lenIn = 3;
}

void AttalSocket::send()
{
	_bufOut[0] = _lenOut-1;
	QSocket::writeBlock( _bufOut, _lenOut );
	_bufOut[1] = 0;
	_bufOut[2] = 0;
	_bufOut[3] = 0;
	_lenOut = 4;
}

void AttalSocket::init( uchar c1, uchar c2, uchar c3 )
{
	_bufOut[1] = c1;
	_bufOut[2] = c2;
	_bufOut[3] = c3;
}

void AttalSocket::appendInt( int i )
{
	memcpy( _bufOut + _lenOut, &i, 4 );
	_lenOut += 4;
}

void AttalSocket::appendChar( unsigned char c )
{
	_bufOut[_lenOut] = c;
	_lenOut++;
}

void AttalSocket::readData()
{
	char len;
	readBlock( &len, 1 );
	_lenIn = 3;
	readBlock( _bufIn, len );
}

void AttalSocket::reReadData()
{
	_lenIn = 3;
}

int AttalSocket::readInt()
{
	int ret;
	memcpy( &ret, _bufIn + _lenIn, 4 );
	_lenIn += 4;
	return ret;
}

unsigned char AttalSocket::readChar()
{
	unsigned char ret = _bufIn[_lenIn];
	_lenIn++;
	return ret;
}

void AttalSocket::copyData( AttalSocket * data )
{
	for( int i = 0; i < 256; i++ ) {
		_bufIn[i] = data->_bufIn[i];
		_bufOut[i] = data->_bufOut[i];
	}

	_lenIn = data->_lenIn;
	_lenOut = data->_lenOut;
}

void AttalSocket::sendMessage( QString msg )
{ 
	init( (uchar) SO_MSG );
	appendChar( (uchar) QMIN( msg.length(), 250 ) );
	/// XXX: we suppose small messages for the moment <= 250
	for( uint i = 0; i< QMIN( msg.length(), 250 ); i++ ) {
		appendChar( (uchar) (msg.latin1())[i] );
	}
	send();
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
	appendChar( (uchar) QMIN( name.length(), 250 ) );
	
	for( uint i = 0; i < QMIN( name.length(), 250 ); i++ ) {
	        appendChar( (uchar) (name.latin1())[i] );
	}
	send();
}

void AttalSocket::sendConnectionPlayer( GenericPlayer * player )
{
	init( (uchar) SO_CONNECT, (uchar) C_CONN_PLAYER );
	
	appendChar( (uchar) player->getNum() );

	QString name = player->getName();
	appendChar( (uchar) QMIN( name.length(), 250 ) );

	for( uint i = 0; i < QMIN( name.length(), 250 ); i++ ) {
		appendChar( (uchar) (name.latin1())[i] );
	}
	send();
}

void AttalSocket::sendExchangeStart( GenericLord * lord1, GenericLord * lord2 )
{
	init( (uchar)SO_EXCH, (uchar) C_EXCH_START );
	appendChar( (uchar)lord1->getId(), (uchar) lord2->getId() );
	send();
}

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

void AttalSocket::sendAskNone( QString msg )
{
	init( (uchar) SO_QR, (uchar) C_QR_NONE );
	appendChar( (uchar) QMIN( msg.length(), 250 ) );
	
	/// XXX: we suppose small messages for the moment
	for( uint i=0; i< QMIN( msg.length(), 250 ); i++ ) {
		appendChar( (uchar) (msg.latin1())[i] );
	}
	send();
}

void AttalSocket::sendBegGame( char nb ) 
{
	init( (uchar)SO_GAME, (uchar)C_GAME_BEG );
	appendChar( nb );
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

void AttalSocket::sendEndGame()
{
	init( (uchar)SO_GAME, (uchar)C_GAME_END );
	send();
}

void AttalSocket::sendTurnBeg()
{
	init( (uchar)SO_TURN, (uchar)C_TURN_BEG );
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

void AttalSocket::sendSizeMap( int h, int w )
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
	appendInt( (int) cell->getTransition() );
	appendInt( (int) cell->getTransitionCellType() );
	appendInt( (int) cell->getDecoration() );
	send();
}

void AttalSocket::sendMvt( int lord, int cell_i, int cell_j )
{
	init( (uchar) SO_MVT );
	appendChar( lord );
	appendInt( cell_i );
	appendInt( cell_j );
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

void AttalSocket::sendFightLord( CLASS_FIGHTER fighter, GenericLord * lord )
{
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

void AttalSocket::sendFightDamageUnit( CLASS_FIGHTER fighter, char num, int damage )
{
	init( (uchar) SO_FIGHT, (uchar) C_FIGHT_DAMAGE );
	appendChar( (uchar) fighter );
	appendChar( (uchar) num );
	appendInt( damage );

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

void AttalSocket::sendFightEnd( char result )
{
	init( (uchar)SO_FIGHT, (uchar) C_FIGHT_END );
	appendChar( (uchar) result );
	send();
}

void AttalSocket::sendAnswer( char resp )
{
	init( SO_QR, (uchar) C_QR_ANSWER );
	appendChar( (uchar) resp );
	send();
}

void AttalSocket::sendLordVisit( GenericLord * lord )
{
	init( (uchar) SO_MODIF, (uchar)C_MOD_LORD, (uchar) C_LORD_VISIT );
	appendChar( lord->getOwner()->getNum() );
	appendInt( lord->getCell()->getRow() );
	appendInt( lord->getCell()->getCol() );
	appendChar( lord->getId() );
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

void AttalSocket::sendLordBuy( GenericLord * lord )
{
	init( (uchar)SO_MODIF, (uchar) C_MOD_LORD, (uchar) C_LORD_BUY );
	appendChar( (uchar) lord->getId() );
	send();
}

void AttalSocket::sendLordBaseCharac( GenericLord * lord, LordCharac charac )
{
	uchar param3 = 0;
	switch( charac ) {
	case ATTACK:
		param3 = C_LORD_ATT;
		break;
	case DEFENSE:
		param3 = C_LORD_DEF;
		break;
	case POWER:
		param3 = C_LORD_POW;
		break;
	case KNOWLEDGE:
		param3 = C_LORD_KNOW;
		break;
	case MOVE:
		param3 = C_LORD_MOVE;
		break;
	case MAXMOVE:
		param3 = C_LORD_MAXMOVE;
		break;
	case TECHNICPOINT:
		param3 = C_LORD_SP;
		break;
	case MAXTECHNICPOINT:
		param3 = C_LORD_MAXSP;
		break;
	case MORALE:
		param3 = C_LORD_MORALE;
		break;
	case LUCK:
		param3 = C_LORD_LUCK;
		break;
	case VISION:
		param3 = C_LORD_VISION;
		break;
	case EXPERIENCE:
		param3 = C_LORD_EXP;
		break;
	default:
		logEE( "sendLordCharac with unknown charac" );
		break;
	}

	init( (uchar)SO_MODIF, (uchar) C_MOD_LORD, param3 );
	appendChar( (uchar) lord->getId() );
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
	if( temp ) {
		init( (uchar)SO_MODIF, (uchar)C_MOD_LORD, (uchar)C_LORD_UNIT );
		appendChar( lord->getId() );
		appendChar( num );
		appendChar( temp->getRace() );
		appendChar( temp->getLevel() );
		appendInt( temp->getNumber() );
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

void AttalSocket::sendBaseOwner( GenericBase * base, GenericPlayer * player )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_OWNER );
	// TODO x,y --> getId
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( player->getNum() );
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
	appendInt( build->getCell()->getRow() );
	appendInt( build->getCell()->getCol() );
	appendChar( build->getBuildingType() );
	send();
}

void AttalSocket::sendBaseUnit( GenericBase * base, GenericFightUnit * unit )
{
	if(unit){
		init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT );
		appendInt( base->getCell()->getRow() );
		appendInt( base->getCell()->getCol() );
		appendChar( unit->getRace() );
		appendChar( unit->getLevel() );
		appendInt( unit->getNumber() );
		send();
	}
}

void AttalSocket::sendBaseUnit( GenericBase * base, Creature * creature, int number )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_UNIT );
	appendInt( base->getCell()->getRow() );
	appendInt( base->getCell()->getCol() );
	appendChar( creature->getRace() );
	appendChar( creature->getLevel() );
	appendInt( number );
	send();
}

void AttalSocket::sendBuildingOwner( GenericBuilding * build, GenericPlayer * player )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BUILD, (uchar)C_BUILD_OWNER );
	appendInt( build->getCell()->getRow() );
	appendInt( build->getCell()->getCol() );
	appendChar( player->getNum() );
	send();
}

void AttalSocket::sendArtefactNew( GenericArtefact * artefact )
{
	if( artefact->isOnMap() ) {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_NEWMAP );
		appendInt( artefact->getId() );
		appendChar( artefact->getType() );
		appendInt( artefact->getCell()->getRow() );
                appendInt( artefact->getCell()->getCol() );
        } else {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_NEWLORD );
                appendInt( artefact->getId() );
		appendChar( artefact->getType() );
                appendChar( artefact->getLord()->getId() );
        }
        send();
}

void AttalSocket::sendArtefactDel( GenericArtefact * artefact )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_DELMAP );
        appendInt( artefact->getId() );
	send();
}

void AttalSocket::sendArtefactLord( GenericArtefact * artefact, GenericLord * lord, bool state )
{
	if( state ) {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_ADDLORD );
	} else {
		init( (uchar)SO_MODIF, (uchar)C_MOD_ARTEFACT, (uchar)C_ART_DELLORD );
	}
	appendInt( artefact->getId() );
	appendChar( lord->getId() );
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
	send();
}

void AttalSocket::sendDelCreature( GenericCell * cell )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_CREATURE, (uchar)C_CRE_DEL );
	appendInt( cell->getRow() );
	appendInt( cell->getCol() );
	send();
}

void AttalSocket::sendNewEvent( GenericEvent * event )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_EVENT, (uchar)C_EVENT_NEW );
	appendInt( event->getCell()->getRow() );
	appendInt( event->getCell()->getCol() );
	appendChar( event->getType() );
	send();

}

void AttalSocket::sendDelEvent( GenericCell * cell )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_EVENT, (uchar)C_EVENT_DEL );
	appendInt( cell->getRow() );
	appendInt( cell->getCol() );
	send();
}

void AttalSocket::requestBuilding( GenericBase * base, int building )
{
	init( (uchar)SO_MODIF, (uchar)C_MOD_BASE, (uchar)C_BASE_BUILDING );
	appendChar( base->getId() );
	appendChar( building );
	send();
}



