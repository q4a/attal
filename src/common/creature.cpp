/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: creature.cpp,v 1.5 2003/04/25 18:59:59 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/09/2002
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

#include "creature.h"

// generic include files
// include files for QT
// application specific include files
#include "common/dataTheme.h"
#include "common/genericFightCell.h"
#include "common/log.h"

extern QString DATA_PATH;
extern DataTheme DataTheme;

//
// ----- Creature -----
//

Creature::Creature()
	: _ptAttack  ( 0 ),
	  _ptDefense ( 0 ),
	  _maxHealth ( 0 ),
	  _maxMove   ( 0 ),
	  _distAttack( 0 ),
	  _minDamages( 0 ),
	  _maxDamages( 0 ),
	  _morale    ( 0 ),
	  _luck      ( 0 )
{
	_name = "";
	_building = "";
	_race = 0;
	_level = 0;
	for( int i = 0; i < MAX_RESS; i++ ) {
		_cost[i] = 0;
		_mcost[i] = 0;
	}
}

int Creature::getCost( int res )
{
	int ret = 0;

	if( ( res >= 0 ) && ( res < MAX_RESS ) ) {
		ret = _cost[res];
	}

	return ret;
}

void Creature::setCost( int res, uint cost )
{
	if( ( res >= 0 ) && ( res < MAX_RESS ) ) {
		_cost[res] = cost;
	}

}

int Creature::getMantCost( int res )
{
	int ret = 0;

	if( ( res >= 0 ) && ( res < MAX_RESS ) ) {
		ret = _mcost[res];
	}

	return ret;
}

void Creature::setMantCost( int res, uint cost )
{
	if( ( res >= 0 ) && ( res < MAX_RESS ) ) {
		_mcost[res] = cost;
	}

}

void Creature::save( QTextStream & ts, int indent )
{
	uint k;

	indentation( &ts, indent );
	ts << "<creature name=\"" << getName() << "\">" << endl;
	indentation( &ts, indent );
	ts << "\t<attack>" << getAttack() << "</attack>" << endl;
	indentation( &ts, indent );
	ts << "\t<defense>" << getDefense() << "</defense>" << endl;
	indentation( &ts, indent );
	ts << "\t<range>" << getDistAttack() << "</range>" << endl;
	indentation( &ts, indent );
	ts << "\t<mindamages>" << getMinDamages() << "</mindamages>" << endl;
	indentation( &ts, indent );
	ts << "\t<maxdamages>" << getMaxDamages() << "</maxdamages>" << endl;
	indentation( &ts, indent );
	ts << "\t<health>" << getMaxHealth() << "</health>" << endl;
	indentation( &ts, indent );
	ts << "\t<move>" << getMaxMove() << "</move>" << endl;
	indentation( &ts, indent );
	ts << "\t<morale>" << getMorale() << "</morale>" << endl;
	indentation( &ts, indent );
	ts << "\t<luck>" << getLuck() << "</luck>" << endl;
	for( k = 0; k < MAX_RESS; k++ ) {
		if( getCost( k ) > 0 ) {
			indentation( &ts, indent );
			ts << "\t<cost resource=\"" << k << "\">";
			ts << getCost( k ) << "</cost>" << endl;
		}
	}
	for( k = 0; k < MAX_RESS; k++ ) {
		if( getMantCost( k ) > 0 ) {
			indentation( &ts, indent );
			ts << "\t<mcost resource=\"" << k << "\">";
			ts << getMantCost( k ) << "</mcost>" << endl;
		}
	}
	indentation( &ts, indent );
	ts << "</creature>" << endl;
}

//
// ----- CreatureStack -----
//

CreatureStack::CreatureStack( Creature * creature, int nb )
{
	_creature = creature;
	_nb = nb;
}

void CreatureStack::increase( int nb )
{
	_nb += nb;
}

void CreatureStack::decrease( int nb )
{
	_nb -= nb;
	if( _nb < 0 ) {
		_nb = 0;
	}
}

//
// ----- CreatureCounter -----
//

CreatureCounter::CreatureCounter()
{
	_list.setAutoDelete( true );
}

void CreatureCounter::clear()
{
	_list.clear();
}

void CreatureCounter::addCreature( Creature * creature, int nb )
{
	bool added = false;

	for( uint i = 0; i < _list.count(); i++ ) {
		Creature * temp = _list.at( i )->getCreature();
		if( ( temp->getRace() == creature->getRace() )
			&& ( temp->getLevel() == creature->getLevel() ) ) {
			_list.at( i )->increase( nb );
			added = true;
			i = _list.count();
		}
	}

	if( ! added ) {
		CreatureStack * stack = new CreatureStack( creature, nb );
		_list.append( stack );
	}
}

void CreatureCounter::decreaseCreature( Creature * creature, int number )
{
	for( uint i = 0; i < _list.count(); i++ ) {
		Creature * temp = _list.at( i )->getCreature();
		if( ( temp->getRace() == creature->getRace() )
			&& ( temp->getLevel() == creature->getLevel() ) ) {
			_list.at( i )->decrease( number );
			i = _list.count();
		}
	}
}

int CreatureCounter::getCreatureNumber()
{
	return _list.count();
}

int CreatureCounter::getCreatureCount( int num )
{
	int ret = 0;

	if( (uint)num < _list.count() ) {
		ret = _list.at( num )->getNumber();
	}

	return ret;
}

int CreatureCounter::getCreatureCount( Creature * creature )
{
	int ret = 0;

	for( uint i = 0; i < _list.count(); i++ ) {
		Creature * temp = _list.at( i )->getCreature();
		if( ( temp->getRace() == creature->getRace() )
			&& ( temp->getLevel() == creature->getLevel() ) ) {
			ret = _list.at( i )->getNumber();
		}
	}

	return ret;
}

Creature * CreatureCounter::getCreature( int num )
{
	Creature * ret = 0;

	if( (uint)num < _list.count() ) {
		ret = _list.at( num )->getCreature();
	}

	return ret;
}

//
// ----- CreatureHandler -----
//

CreatureHandler::CreatureHandler( CreatureList * list )
{
	_list = list;
}

bool CreatureHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	_state = StateInit;
	return true;
}

bool CreatureHandler::startElement( const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts )
{
	if( qName == "creatures" && _state == StateInit ) {
		_state = StateDocument;
	} else if ( qName == "race" && _state == StateDocument ) {
		_state = StateRace;
		_race = atts.value( "name" );
	} else if ( qName == "creature" && _state == StateRace ) {
		_state = StateCreature;
		_creature = new Creature();
		_creature->setName( atts.value( "name" ) );
	} else if ( qName == "attack" && _state == StateCreature ) {
		_state = StateAttack;
	} else if ( qName == "defense"  && _state == StateCreature ) {
		_state = StateDefense;
	} else if ( qName == "health" && _state == StateCreature ) {
		_state = StateHealth;
	} else if ( qName == "move" && _state == StateCreature ) {
		_state = StateMove;
	} else if ( qName == "range"  && _state == StateCreature ) {
		_state = StateRange;
	} else if ( qName == "mindamages" && _state == StateCreature ) {
		_state = StateMinDamages;
	} else if ( qName == "maxdamages" && _state == StateCreature ) {
		_state = StateMaxDamages;
	} else if ( qName == "morale"  && _state == StateCreature ) {
		_state = StateMorale;
	} else if ( qName == "luck"  && _state == StateCreature ) {
		_state = StateLuck;
	} else if( qName == "cost" && _state == StateCreature ) {
		_state = StateCost;
		_resource = atts.value( "resource" ).toInt();
	} else if( qName == "mcost" && _state == StateCreature ) {
		_state = StateMantCost;
		_resource = atts.value( "resource" ).toInt();
	} else {
		// error
		return false;
	}
	return true;
}

bool CreatureHandler::endElement( const QString& namespaceURI, const QString& localName, const QString& qName )
{
	switch ( _state ) {
	case StateRace:
	    _state = StateDocument;
	    break;
	case StateCreature:
	    _state = StateRace;
	    _list->append( _race, _creature );
	    break;
	case StateAttack:
	case StateDefense:
	case StateHealth:
	case StateMove:
	case StateRange:
	case StateMinDamages:
	case StateMaxDamages:
	case StateMorale:
	case StateLuck:
	case StateCost:
	    _state = StateCreature;
	    break;
	case StateMantCost:
	    _state = StateCreature;
	    break;
	default:
	    // do nothing
	    break;
    }
    return true;
}

bool CreatureHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplifyWhiteSpace();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateAttack:
		_creature->setAttack( ch_simplified.toInt() );
		break;
	case StateDefense:
		_creature->setDefense( ch_simplified.toInt() );
		break;
	case StateHealth:
		_creature->setMaxHealth( ch_simplified.toInt() );
		break;
	case StateMove:
		_creature->setMaxMove( ch_simplified.toInt() );
		break;
	case StateRange:
		_creature->setDistAttack( ch_simplified.toInt() );
		break;
	case StateMinDamages:
		_creature->setMinDamages( ch_simplified.toInt() );
		break;
	case StateMaxDamages:
		_creature->setMaxDamages( ch_simplified.toInt() );
		break;
	case StateMorale:
		_creature->setMorale( ch_simplified.toInt() );
		break;
	case StateLuck:
		_creature->setLuck( ch_simplified.toInt() );
		break;
	case StateCost:
		_creature->setCost( _resource, ch_simplified.toInt() );
		break;
	case StateMantCost:
		_creature->setMantCost( _resource, ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool CreatureHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}
/****************************************/

Race::Race()
{
	setAutoDelete( true );
}

/*****************************************/
CreatureList::CreatureList()
{
	setAutoDelete( true );
}

Creature * CreatureList::at( int race, int level )
{
	Race * raceList = QList<Race>::at( race );
	return raceList->at( level );
}

bool CreatureList::init()
{
	clear();
	CreatureHandler handler( this );
	QFile file( DATA_PATH + "creatures.dat" );
	QXmlInputSource source( file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", QString( DATA_PATH + "creatures.dat" ).latin1(), handler.errorProtocol().latin1() );
		return false;
	}

	return true;
}

bool CreatureList::save()
{
	QString filename = DATA_PATH + "creatures.dat";
	QFile file( filename );
	uint i, j;

	if (! file.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE creatures>" << endl;
	ts << "<creatures>" << endl;

	for( i = 0; i < countRace(); i++ ) {
		Race * race = QList<Race>::at( i );

		ts << "\t<race name=\"" << race->getName() << "\">" << endl;

		for( j = 0; j < race->count(); j++ ) {
			Creature * creature = race->at( j );
			if( creature ) {
				creature->save( ts, 2 );
			}
		}
		ts << "\t</race>" << endl;
	}
	ts << "</creatures>" << endl;

	file.close();

	return true;
}

void CreatureList::append( QString race, Creature * creature )
{
	QListIterator<Race> iteRace( *this );
	int numRace = -1;
	int cpt = 0;

	if( findRace( creature->getName() ) != -1 ) {
		logEE( "Creature already existing" );
		return;
	}

	for( iteRace.toFirst(); iteRace.current(); ++iteRace ) {
		if( race == iteRace.current()->getName() ) {
			numRace = cpt;
		}
		cpt++;
	}

	if( numRace == -1 ) {
		Race * aRace = new Race();
		aRace->setName( race );
		aRace->append( creature );
		QList<Race>::append( aRace );
		numRace = QList<Race>::count() - 1;
	} else {
		QList<Race>::at( numRace )->append( creature );
	}

	creature->setRace( numRace );
	creature->setLevel( QList<Race>::at( numRace )->count() - 1 );
}

int CreatureList::findRace( QString name )
{
	int ret = 0;
	QListIterator<Race> iteRace( *this );

	for( iteRace.toFirst(); iteRace.current(); ++iteRace ) {
		QListIterator<Creature> iteCrea( *(iteRace.current()) );

		for( iteCrea.toFirst(); iteCrea.current(); ++iteCrea ) {
			if( iteCrea.current()->getName() == name ) {
				return ret;
			}
		}
		ret++;
	}
	return -1;
}

int CreatureList::findLevel( QString name )
{
	int ret = 0;
	QListIterator<Race> iteRace( *this );

	for( iteRace.toFirst(); iteRace.current(); ++iteRace ) {
		QListIterator<Creature> iteCrea( *(iteRace.current()) );

		for( iteCrea.toFirst(); iteCrea.current(); ++iteCrea ) {
			if( iteCrea.current()->getName() == name ) {
				return ret;
			}
			ret++;
		}
		ret = 0;
	}
	logDD( "Level not found : %s", name.latin1() );
	return -1;
}

int CreatureList::count()
{
	int ret = 0;

	QListIterator<Race> iteList( *this );
	for( iteList.toFirst(); iteList.current(); ++iteList ) {
		ret += iteList.current()->count();
	}

	return ret;
}

int CreatureList::giveNumRace( QString race )
{
	int ret = 0, cpt = 0;

	QListIterator<Race> iteList( *this );
	for( iteList.toFirst(); iteList.current(); ++iteList ) {
		if( iteList.current()->getName() == race ) {
			ret = cpt;
		}
		cpt++;
	}

	return ret;
}

int computeSequence( GenericFightUnit * unit )
{
	int ret = 0;

	for( int i = 0; i < unit->getRace(); i++ ) {
		ret += DataTheme.creatures.getRace( i )->count();
	}

	return ret + unit->getLevel();
}

