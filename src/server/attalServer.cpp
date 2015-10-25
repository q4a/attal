/****************************************************************
**
** Attal : Lords of Doom
**
** attalServer.cpp
** the server : manage connections
**
** Version : $Id: attalServer.cpp,v 1.67 2003/05/08 11:00:44 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/10/2000
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

#include "attalServer.h"


// generic include files
// include files for QT
// application specific includes
#include "common/log.h"
#include "common/artefact.h"
#include "common/unit.h"
#include "common/genericBase.h"
#include "common/genericMapCreature.h"

#include "server/engine.h"

//
// ----- AttalPlayerSocket -----
//

AttalPlayerSocket::AttalPlayerSocket( Engine * engine )
	: AttalSocket(),
	  _player( 0 )
{
	_engine = engine;
}

bool AttalPlayerSocket::canSee( int row, int col )
{
	bool ret = false;

	if( _player ) {
		ret = _player->canSee( row, col );
	}

	return ret;
}

bool AttalPlayerSocket::canSee( GenericCell * cell )
{
	bool ret = false;

	if( _player ) {
		ret = _player->canSee( cell );
	}

	return ret;
}


//
// ----- AttalServer -----
//

AttalServer::AttalServer( int port )
	: QServerSocket( port )
{	
	_num = 0;
	_currentSocket = 0;
	_theSockets.setAutoDelete( true );
	_mapper = new QSignalMapper( this );
	connect( _mapper, SIGNAL( mapped( int ) ), SIGNAL( sig_readEvent( int ) ) );	
}

void AttalServer::newConnection ( int num_socket )
{
	AttalPlayerSocket * uneso = new AttalPlayerSocket( _engine );
	GenericPlayer * player = new GenericPlayer();
	uneso->setPlayer( player );
	uneso->setSocket( num_socket );
	_theSockets.append( uneso );
	_mapper->setMapping( uneso, _theSockets.count()-1 );
	connect( uneso, SIGNAL( readyRead() ), _mapper, SLOT( map() ) );
	connect( uneso, SIGNAL( connectionClosed() ), SLOT( slot_socketClose() ) );
	emit sig_newPlayer( uneso );
	uneso->sendConnectionOk();
}

void AttalServer::slot_socketClose()
{
	AttalPlayerSocket * uneso =(AttalPlayerSocket*)sender();
	sendMessage(uneso->getPlayer()->getName() + "  disconnect");
	emit sig_endConnection( uneso->getPlayer()->getName() );
	uneso->setPlayer( 0 );
	_theSockets.remove( uneso );
	
	int num;
	QListIterator<AttalPlayerSocket> ite( _theSockets );
	
	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			num=_theSockets.find(ite.current());
			ite.current()->sendConnectionId(num);
		}
	}

}

void AttalServer::sendConnectionId( char num )
{
	_theSockets.at( num )->sendConnectionId( num );
}

void AttalServer::sendConnectionPlayer( GenericPlayer * dest, GenericPlayer * player )
{
	findSocket( dest )->sendConnectionPlayer( player );
}

void AttalServer::getSocketData( int num )
{
	_num = num;
	_theSockets.at( num )->readData();
}

void AttalServer::copyData( AttalSocket* data )
{
	_theSockets.at( _num )->copyData( data );
}

uchar AttalServer::getCla1()
{
	return _theSockets.at( _num )->getCla1();
}

uchar AttalServer::getCla2()
{
	return _theSockets.at( _num )->getCla2();
}

uchar AttalServer::getCla3()
{
	return _theSockets.at( _num )->getCla3();
}

int AttalServer::readInt()
{
	return _theSockets.at( _num )->readInt();
}

unsigned char AttalServer::readChar()
{
	return _theSockets.at( _num )->readChar();
}

bool AttalServer::isData( int num )
{
	return (_theSockets.at(num)->bytesAvailable() > 0);
}

void AttalServer::startGame()
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );
	
	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendBegGame( _theSockets.count() );
		}
	}
}

void AttalServer::playerLose( GenericPlayer * player )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendLostGame( player );
		}
	}
}

void AttalServer::endGame()
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendEndGame();
		}
	}
}

void AttalServer::beginTurn()
{
	_currentSocket->sendTurnBeg();
}

void AttalServer::sendLordPlTurn(int count)
{
	_currentSocket->sendLordPlTurn(count);
}

void AttalServer::startFight( GenericLord * myLord, GenericPlayer * opponent, GenericLord * opponentLord )
{
	int i;

	_currentSocket->sendFightInit( FIGHTER_ATTACK, myLord );
	_currentSocket->sendFightLord( FIGHTER_DEFENSE, opponentLord );
	for( i = 0; i < MAX_UNIT; i++ ) {
		if( opponentLord->getUnit( i ) ) {
			_currentSocket->sendFightNewUnit( FIGHTER_DEFENSE, i, opponentLord->getUnit( i ) );
		}
	}

	AttalPlayerSocket * opponentSocket = findSocket( opponent );
	opponentSocket->sendFightInit( FIGHTER_DEFENSE, opponentLord );
	opponentSocket->sendFightLord( FIGHTER_ATTACK, myLord );
	for( i = 0; i < MAX_UNIT; i++ ) {
		if( myLord->getUnit( i ) ) {
			opponentSocket->sendFightNewUnit( FIGHTER_ATTACK, i, myLord->getUnit( i ) );
		}
	}

}

void AttalServer::startFight( GenericLord * myLord, GenericFightUnit * opponentUnits[MAX_UNIT] )
{
	_currentSocket->sendFightInit( FIGHTER_ATTACK, myLord );
	_currentSocket->sendFightCreature();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( opponentUnits[i] ) {
			_currentSocket->sendFightNewUnit( FIGHTER_DEFENSE, i, opponentUnits[i] );
		}
	}
}

void AttalServer::moveUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightCell * cell )
{
	AttalPlayerSocket * sock = findSocket( player );
	sock->sendFightUnitMove( fighter, num, cell );
}

void AttalServer::updateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, GenericFightUnit * unit )
{
	AttalPlayerSocket * sock = findSocket( player );
	sock->sendFightModifyUnit( fighter, num, unit );
}

void AttalServer::damageUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num, int damage )
{
	AttalPlayerSocket * sock = findSocket( player );
	sock->sendFightDamageUnit( fighter, num, damage );
}

void AttalServer::activateUnit( GenericPlayer * player, CLASS_FIGHTER fighter, int num )
{
	AttalPlayerSocket * sock = findSocket( player );
	sock->sendFightActivate( fighter, num );
}

void AttalServer::endFight(GenericPlayer * attackPlayer, GenericPlayer * defendPlayer, char result )
{
	AttalPlayerSocket * current = findSocket( attackPlayer );
	current->sendFightEnd( result );

	if( defendPlayer ) {
		current = findSocket( defendPlayer );
		current->sendFightEnd( result );
	}
}

void AttalServer::sendMessage( QString msg )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendMessage( msg );
		}
	}
}

void AttalServer::sendFightMessage( QString msg, GenericPlayer * attackPlayer, GenericPlayer * defendPlayer )
{
	AttalPlayerSocket * current = findSocket( attackPlayer );
	current->sendMessage( msg );

	if( defendPlayer ) {
		current = findSocket( defendPlayer );
		current->sendMessage( msg );
	}
}

void AttalServer::sendPlayerResource( char ressource, int nb )
{
	_currentSocket->sendPlayerResource( ressource, nb );
}

void AttalServer::sendPlayerResources( GenericPlayer * player )
{
	AttalPlayerSocket * current = findSocket( player );
	for( int i = 0; i < MAX_RESS; i++ ) {
		current->sendPlayerResource( i, player->getResource( i ) );
	}
}

/*!

*/

void AttalServer::sendSizeMap( int h, int w )
{
 	QListIterator<AttalPlayerSocket> ite( _theSockets );
	
	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendSizeMap( h, w );
		}
	}	   
}

void AttalServer::sendCell( GenericCell * cell )
{
	_currentSocket->sendCell( cell );
}

void AttalServer::sendAskNone( QString msg )
{
 	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendAskNone( msg );
		}
	}
}

void AttalServer::newLord( GenericLord * lord )
{
    	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current() == _currentSocket ) {
				ite.current()->sendLordNew( lord );
			} else {
				ite.current()->sendLordVisit( lord );
			}
				ite.current()->sendLordBaseCharac( lord, MOVE );
				ite.current()->sendLordBaseCharac( lord, MAXMOVE );
				ite.current()->sendLordBaseCharac( lord, TECHNICPOINT );
				ite.current()->sendLordBaseCharac( lord, MAXTECHNICPOINT );
				ite.current()->sendLordBaseCharac( lord, MORALE );
				ite.current()->sendLordBaseCharac( lord, LUCK );
				ite.current()->sendLordBaseCharac( lord, EXPERIENCE );
				ite.current()->sendLordBaseCharac( lord, ATTACK );
				ite.current()->sendLordBaseCharac( lord, DEFENSE );
				ite.current()->sendLordBaseCharac( lord, POWER );
				ite.current()->sendLordBaseCharac( lord, KNOWLEDGE );
				ite.current()->sendLordUnits( lord );
				for( int i = 0; i < lord->numArtefact(); i++ ) {
					ite.current()->sendArtefactNew( lord->getArtefact( i ) );
				}
				for( uint j = 0; j < lord->getMachineNumber(); j++ ) {
					ite.current()->sendLordMachine( lord, lord->getMachine( j ) );
				}
		}
	}
}

void AttalServer::updateLordBaseUnits( GenericBase * base ,GenericLord * lord , int flag)
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current() != _currentSocket ) {
				if(flag==0 || flag==2){
					for( int i = 0; i < MAX_UNIT; i++ ) {
						ite.current()->sendBaseUnit( base, base->getUnit(i) );
					}
				}
				if(flag==1 || flag==2){
					ite.current()->sendLordUnits(lord);
				}
			}
		}
	}
}
	
void AttalServer::updateLord( GenericLord * lord )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
				ite.current()->sendLordBaseCharac( lord, MOVE );
				ite.current()->sendLordBaseCharac( lord, MAXMOVE );
				ite.current()->sendLordBaseCharac( lord, TECHNICPOINT );
				ite.current()->sendLordBaseCharac( lord, MAXTECHNICPOINT );
				ite.current()->sendLordBaseCharac( lord, MORALE );
				ite.current()->sendLordBaseCharac( lord, LUCK );
				ite.current()->sendLordBaseCharac( lord, EXPERIENCE );
				ite.current()->sendLordBaseCharac( lord, ATTACK );
				ite.current()->sendLordBaseCharac( lord, DEFENSE );
				ite.current()->sendLordBaseCharac( lord, POWER );
				ite.current()->sendLordBaseCharac( lord, KNOWLEDGE );
				ite.current()->sendLordUnits( lord );
				for( int i = 0; i < lord->numArtefact(); i++ ) {
					ite.current()->sendArtefactNew( lord->getArtefact( i ) );
				}
				for( uint j = 0; j < lord->getMachineNumber(); j++ ) {
					ite.current()->sendLordMachine( lord, lord->getMachine( j ) );
				}
		}
	}
}

void AttalServer::sendLordRemove( GenericLord * lord )
{
    	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendLordRemove( lord );
		}
	}
}

void AttalServer::sendCreatureRemove( GenericCell * cell )
{
    	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendDelCreature( cell );
		}
	}
}

void AttalServer::setGarrison( GenericLord * lord, bool state )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendLordGarrison( lord, state );
		}
	}
}


void AttalServer::newBase( GenericBase * base )
{
   	//QListIterator<AttalPlayerSocket> ite( _theSockets );

	//if( !ite.isEmpty() ) {
	//	for( ite.atFirst(); ite.current(); ++ite ) {
	//		ite.current()->sendBaseNew( base );
			// XXX: send info only for owner... ?
			//if( ite.current() == _currentSocket ) {
	//			for( uint i = 0; i < base->getBuildingCount(); i++ ) {
	//				ite.current()->sendBaseBuilding( base, base->getBuilding( i ), true );
	//			}
		//	}
	//	}
	//}

	_currentSocket->sendBaseNew( base );
	for( uint i = 0; i < base->getBuildingCount(); i++ ) {
		_currentSocket->sendBaseBuilding( base, base->getBuilding( i ), true );
	}
}


void AttalServer::updateBaseBuilding( GenericBase * base, GenericInsideBuilding * building )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		bool found = false;
		for( ite.atFirst(); ite.current(); ++ite ) {
			for( uint i = 0; i < base->getBuildingCount(); i++ ) {
				GenericInsideBuilding * build = base->getBuilding( i );
				if( build == building ) {
					ite.current()->sendBaseBuilding( base, build, true );
					found = true;
				}
			}
			if( ! found ) {
				ite.current()->sendBaseBuilding( base, building, false );
			}
		}
	}
}

void AttalServer::sendBaseUnit( GenericBase * base, Creature * creature, int number )
{
	
  QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendBaseUnit( base, creature, number );
		}
	}
}

void AttalServer::sendLordExchange( GenericLord * lord1, GenericLord * lord2 )
{
	_currentSocket->sendExchangeStart( lord1, lord2 );
}

void AttalServer::sendExchangeUnit( GenericLord * lord1, uchar unit1, GenericLord * lord2, uchar unit2 )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current() != _currentSocket ) {
				ite.current()->sendExchangeUnit( lord1, unit1, lord2, unit2 );
			}
		}
	}
}

void AttalServer::sendExchangeBaseUnitCl( GenericBase * base, uchar unit1, GenericLord * lord, uchar unit2 )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current() != _currentSocket ) {
				ite.current()->sendExchangeBaseUnitCl( base, unit1, lord, unit2 );
			}
		}
	}
}

void AttalServer::ownBase( GenericBase * base, GenericPlayer * player )
{
 	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendBaseOwner( base, player );
		}
	}
}

/*!

*/

void AttalServer::newBuilding( GenericBuilding * build )
{
   	//QListIterator<AttalPlayerSocket> ite( _theSockets );

	//if( !ite.isEmpty() ) {
	//	for( ite.atFirst(); ite.current(); ++ite ) {
	//		ite.current()->sendBuildingNew( build );
	//	}
	//}
	_currentSocket->sendBuildingNew( build );
}


void AttalServer::ownBuilding( GenericBuilding * build, GenericPlayer * player )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendBuildingOwner( build, player );
		}
	}
}

void AttalServer::ownArtefact( GenericArtefact * artefact, GenericLord * lord, GenericPlayer * player )
{
       QListIterator<AttalPlayerSocket> ite( _theSockets );

       if( !ite.isEmpty() ) {
	       for( ite.atFirst(); ite.current(); ++ite ) {
		       ite.current()->sendArtefactDel( artefact );
	       }
       }

       AttalPlayerSocket * current = findSocket( player );
       current->sendArtefactLord( artefact, lord, true );
}

void AttalServer::newCreature( GenericMapCreature * creature )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendNewCreature( creature );
		}
	}
}

void AttalServer::delCreature( GenericCell * cell )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendDelCreature( cell );
		}
	}
}

void AttalServer::newEvent( GenericEvent * event )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendNewEvent( event );
		}
	}
}

void AttalServer::delEvent( GenericCell * cell )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			ite.current()->sendDelEvent( cell );
		}
	}
}

AttalPlayerSocket * AttalServer::findSocket( GenericPlayer * player )
{
  	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if ( ite.current()->getPlayer() == player )
				return ite.current();
		}
	}
	logEE( "Socket not found" );
	return 0;
}



/*!

*/

void AttalServer::sendMvt( GenericPlayer * dest, int lord, int i, int j )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );

	if( !ite.isEmpty() ) {
		for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current()->canSee( i, j ) ) {
				ite.current()->sendMvt( lord, i, j );
			}
		}
	}
}

void AttalServer::newArtefactMap( GenericArtefact * artefact )
{
	QListIterator<AttalPlayerSocket> ite( _theSockets );
        if( !ite.isEmpty() ) {
                for( ite.atFirst(); ite.current(); ++ite ) {
			if( ite.current()->canSee( artefact->getCell() ) ) {
                        	ite.current()->sendArtefactNew( artefact );
			}
                }
        }
}

void AttalServer::sendLordCharac( GenericPlayer * player, GenericLord * lord, LordCharac charac )
{
	AttalPlayerSocket * current = findSocket( player );
	current->sendLordBaseCharac( lord, charac );
}





