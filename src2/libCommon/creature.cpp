/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: creature.cpp,v 1.33 2009/10/22 17:09:10 lusum Exp $
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
#include "libCommon/dataTheme.h"
#include "libCommon/genericFightCell.h"
#include "libCommon/log.h"


extern QString DATA_PATH;


Creature::CreatureAnimation::CreatureAnimation( CreatureAnimationType type, uint first, uint last )
: _type( type ), _first( first ), _last( last )
{

}

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
	  _luck      ( 0 ),
	  _size( 1 ),
	  _numFrames ( 0 ),
	  _XOffset ( 0 ),
	  _XOffsetMirror ( 0 ),
	  _YOffset ( 0 )
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

Creature::~Creature()
{
	while( ! _animations.isEmpty() ) {
		delete _animations.takeFirst();
	}
}

uint Creature::getCost( uint res )
{
	uint ret = 0;

	if( (int)res < DataTheme.resources.count() ) {
		ret = _cost[res];
	}

	return ret;
}

void Creature::setCost( uint res, uint cost )
{
	if( (int)res < DataTheme.resources.count() ) {
		_cost[res] = cost;
	}

}

uint Creature::getMantCost( uint res )
{
	uint ret = 0;

	if( (int)res < DataTheme.resources.count() ) {
		ret = _mcost[res];
	}

	return ret;
}

void Creature::setMantCost( uint res, uint cost )
{
	if( (int)res < DataTheme.resources.count() ) {
		_mcost[res] = cost;
	}
}

void Creature::setAnimation( CreatureAnimationType type, uint first, uint last )
{
	Creature::CreatureAnimation * animation = 0;

	animation = new Creature::CreatureAnimation( type, first, last );
	_animations.append( animation );
}

int Creature::getFirstAnimationFrame( CreatureAnimationType type )
{
	int ret = -1;
	uint nbAnim = _animations.count();
	Creature::CreatureAnimation * animation;
	for( uint i = 0; i < nbAnim; i++ ) {
		animation = _animations.at( i );
		if( animation->getType() == type ) {
			// frames of attalSprite start by 0 , frames in theme data start by 1
			ret = animation->getFirst() - 1;
			break;
		}
	}

	return ret;
}

int Creature::getLastAnimationFrame( CreatureAnimationType type )
{
	int ret = _numFrames;
	uint nbAnim = _animations.count();
	Creature::CreatureAnimation * animation;
	for( uint i = 0; i < nbAnim; i++ ) {
		animation = _animations.at( i );
		if( animation->getType() == type ) {
			// frames of attalSprite start by 0 , frames in theme data start by 1
			ret = animation->getLast() - 1;
			break;
		}
	}

	return ret;
}

int Creature::getNumberAnimationFrame( CreatureAnimationType type )
{
	int ret = 0;
	uint nbAnim = _animations.count();
	Creature::CreatureAnimation * animation;
	for( uint i = 0; i < nbAnim; i++ ) {
		animation = _animations.at( i );
		if( animation->getType() == type ) {
			ret = animation->getLast() - animation->getFirst() + 1;
			break;
		}
	}

	return ret;
}

void Creature::save( QTextStream & ts, int indent )
{
	uint nbAnim = _animations.count();

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
	indentation( &ts, indent );
	ts << "\t<size>" << getSize() << "</size>" << endl;
	indentation( &ts, indent );
	ts << "\t<animations nbFrames=\"" << getNumFrames() << "\">" << endl;

	Creature::CreatureAnimation * animation;
	for( uint k = 0; k < nbAnim; k++ ) {
		indentation( &ts, indent+2 );
		animation = _animations.at( k );
		ts << "<" << getAnimationTypeName( animation->getType() );
		ts << " first=\"" << animation->getFirst() << "\"";
		ts << " last=\"" << animation->getLast() << "\">";
		ts << "</" << getAnimationTypeName( animation->getType() ) << ">" << endl;
	}

	indentation( &ts, indent );
	ts << "\t</animations>" << endl;
	indentation( &ts, indent );
	ts << "\t<xoffset>" << getXOffset() << "</xoffset>" << endl;
	indentation( &ts, indent );
	ts << "\t<xoffsetmirr>" << getXOffsetMirror() << "</xoffsetmirr>" << endl;
	indentation( &ts, indent );
	ts << "\t<yoffset>" << getYOffset() << "</yoffset>" << endl;
	for( int k = 0; k < DataTheme.resources.count(); k++ ) {
		if( getCost( k ) > 0 ) {
			indentation( &ts, indent );
			ts << "\t<cost resource=\"" << k << "\">";
			ts << getCost( k ) << "</cost>" << endl;
		}
	}
	for( int k = 0; k < DataTheme.resources.count(); k++ ) {
		if( getMantCost( k ) > 0 ) {
			indentation( &ts, indent );
			ts << "\t<mcost resource=\"" << k << "\">";
			ts << getMantCost( k ) << "</mcost>" << endl;
		}
	}
	indentation( &ts, indent );
	ts << "</creature>" << endl;
	ts << flush;
}

QString Creature::getAnimationTypeName( CreatureAnimationType type )
{
	QString ret;

	switch( type ) {
	case Moving:
		ret = "moving";
		break;
	case Fighting:
		ret = "fighting";
		break;
	case Shooting:
		ret = "shooting";
		break;
	case ShootHigh:
		ret = "shoothigh";
		break;
	case ShootLow:
		ret = "shootlow";
		break;
	case Defending:
		ret = "defending";
		break;
	case	StartMoving:
		ret = "startmoving";
		break;
	case EndMoving:
		ret = "endmoving";
		break;
	case Dying:
		ret = "dying";
		break;
	case Dead:
		ret = "dead";
		break;
	case AttackHigh:
		ret = "attackhigh";
		break;
	case AttackLow:
		ret = "attacklow";
		break;
	case Selecting:
		ret = "selecting";
		break;	
	}

	return ret;
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
}

CreatureCounter::~CreatureCounter()
{
	clear();
}

void CreatureCounter::clear()
{
	while( ! _list.isEmpty() ) {
		delete _list.takeFirst();
	}
}

void CreatureCounter::addCreature( Creature * creature, int nb )
{
	bool added = false;

	if( creature ) {
		for( int i = 0; i < _list.count(); i++ ) {
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
}

void CreatureCounter::decreaseCreature( Creature * creature, int number )
{
	if( creature ) {
		for( int i = 0; i < _list.count(); i++ ) {
			Creature * temp = _list.at( i )->getCreature();
			if( ( temp->getRace() == creature->getRace() )
				&& ( temp->getLevel() == creature->getLevel() ) ) {
				_list.at( i )->decrease( number );
				i = _list.count();
			}
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

	if( num < _list.count() ) {
		ret = _list.at( num )->getNumber();
	}

	return ret;
}

void CreatureCounter::setCreatureCount( int num ,int value)
{

	if( num < _list.count() ) {
		_list.at( num )->setNumber(value);
	}
}

int CreatureCounter::getCreatureCount( Creature * creature )
{
	int ret = 0;

	for( int i = 0; i < _list.count(); i++ ) {
		Creature * temp = _list.at( i )->getCreature();
		if( ( temp->getRace() == creature->getRace() )
			&& ( temp->getLevel() == creature->getLevel() ) ) {
			ret = _list.at( i )->getNumber();
		}
	}

	return ret;
}

void CreatureCounter::setCreatureCount( Creature * creature , int value)
{

	bool added = false;

	if( creature ) {
		for( int i = 0; i < _list.count(); i++ ) {
			Creature * temp = _list.at( i )->getCreature();
			if( ( temp->getRace() == creature->getRace() )
					&& ( temp->getLevel() == creature->getLevel() ) ) {
				_list.at( i )->setNumber(value);
				added = true;
				i = _list.count();
			}
		}

		if( ! added ) {
			CreatureStack * stack = new CreatureStack( creature, value );
			_list.append( stack );
		}
	}

}


Creature * CreatureCounter::getCreature( int num )
{
	Creature * ret = 0;

	if( num < _list.count() ) {
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

bool CreatureHandler::startElement( const QString &, const QString &, const QString& qName, const QXmlAttributes& atts )
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
	} else if( qName == "size" && _state == StateCreature ) {
		_state = StateSize;
	} else if( qName == "animations" && _state == StateCreature ) {
		_creature->setNumFrames( atts.value( "nbFrames" ).toInt() );
		_state = StateAnimations;
	} else if( qName == "moving" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Moving, first, last );
		_state = StateAnimation;
	} else if( qName == "fighting" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Fighting, first, last );
		_state = StateAnimation;
	} else if( qName == "shooting" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Shooting, first, last );
		_state = StateAnimation;
	} else if( qName == "shoothigh" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( ShootHigh, first, last );
		_state = StateAnimation;
	} else if( qName == "shootlow" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( ShootLow, first, last );
		_state = StateAnimation;
	} else if( qName == "defending" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Defending, first, last );
		_state = StateAnimation;
	} else if( qName == "startmoving" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( StartMoving, first, last );
		_state = StateAnimation;
	} else if( qName == "endmoving" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( EndMoving, first, last );
		_state = StateAnimation;
	} else if( qName == "dying" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Dying, first, last );
		_state = StateAnimation;
	} else if( qName == "dead" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Dead, first, last );
		_state = StateAnimation;
	} else if( qName == "attackhigh" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( AttackHigh, first, last );
		_state = StateAnimation;
	} else if( qName == "attacklow" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( AttackLow, first, last );
		_state = StateAnimation;
	} else if( qName == "selecting" && _state == StateAnimations ) {
		uint first = atts.value( "first" ).toInt();
		uint last = atts.value( "last" ).toInt();
		_creature->setAnimation( Selecting, first, last );
		_state = StateAnimation;
	} else if( qName == "xoffset" && _state == StateCreature ) {
		_state = StateXOffset;
	} else if( qName == "xoffsetmirr" && _state == StateCreature ) {
		_state = StateXOffsetMirror;
	} else if( qName == "yoffset" && _state == StateCreature ) {
		_state = StateYOffset;
	} else {
		// error
		return false;
	}
	return true;
}

bool CreatureHandler::endElement( const QString &, const QString &, const QString & )
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
	case StateSize:
	case StateAnimations:
	case StateXOffset:
	case StateXOffsetMirror:
	case StateYOffset:
	    _state = StateCreature;
	    break;
	case StateMantCost:
	    _state = StateCreature;
	    break;
	case StateAnimation:
		_state = StateAnimations;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;
}

bool CreatureHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
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
	case StateSize:
		_creature->setSize( ch_simplified.toInt() );
		break;
	case StateXOffset:
		_creature->setXOffset ( ch_simplified.toInt() );
		break;
	case StateXOffsetMirror:
		_creature->setXOffsetMirror ( ch_simplified.toInt() );
		break;
	case StateYOffset:
		_creature->setYOffset ( ch_simplified.toInt() );
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

//
// ----- Race -----
//

Race::Race()
{
}

Race::~Race()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

//
// ----- CreatureList -----
//
CreatureList::CreatureList()
{

}

CreatureList::~CreatureList()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

void CreatureList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

Creature * CreatureList::at( int race, int level )
{
	Race * raceList = QList<Race *>::at( race );
	return raceList->at( level );
}

bool CreatureList::init()
{
	clear();
	CreatureHandler handler( this );
	QFile file( DATA_PATH + "creatures.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable ( QString( DATA_PATH + "creatures.dat" ) ), qPrintable(  handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool CreatureList::save()
{
	QString filename = DATA_PATH + "creatures.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable( filename ) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE creatures>" << endl;
	ts << "<creatures>" << endl;

	for( uint i = 0; i < countRace(); i++ ) {
		Race * race = QList<Race *>::at( i );

		ts << "\t<race name=\"" << race->getName() << "\">" << endl;

		for( int j = 0; j < race->count(); j++ ) {
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

void CreatureList::append( const QString & race, Creature * creature )
{
	QListIterator<Race *> iteRace( *this );
	int numRace = -1;
	int cpt = 0;

	if( findRace( creature->getName() ) != -1 ) {
		logEE( "Creature already existing" );
		return;
	}

	while( iteRace.hasNext() ) {
		if( race == iteRace.next()->getName() ) {
			numRace = cpt;
		}
		cpt++;
	}

	if( numRace == -1 ) {
		Race * aRace = new Race();
		aRace->setName( race );
		aRace->append( creature );
		QList<Race *>::append( aRace );
		numRace = QList<Race *>::count() - 1;
	} else {
		QList<Race *>::at( numRace )->append( creature );
	}

	creature->setRace( numRace );
	creature->setLevel( QList<Race *>::at( numRace )->count() - 1 );
}

int CreatureList::findRace( const QString & name )
{
	int ret = 0;
	QListIterator<Race *> iteRace( *this );

	while( iteRace.hasNext() ) {
		QListIterator<Creature *> iteCreature( *( iteRace.next() ) );

		while( iteCreature.hasNext() ) {
			if( iteCreature.next()->getName() == name ) {
				return ret;
			}
		}
		ret++;
	}

	return -1;
}

int CreatureList::findLevel( const QString & name )
{
	int ret = 0;
	QListIterator<Race *> iteRace( *this );

	while( iteRace.hasNext() ) {
		QListIterator<Creature *> iteCreature( *( iteRace.next() ) );

		while( iteCreature.hasNext() ) {
			if( iteCreature.next()->getName() == name ) {
				return ret;
			}
			ret++;
		}
		ret = 0;
	}

	logDD( "Level not found : %s", qPrintable(  name ) );
	return -1;
}

int CreatureList::count()
{
	int ret = 0;

	QListIterator<Race *> iteRace( *this );
	while( iteRace.hasNext() ) {
		ret += iteRace.next()->count();
	}

	return ret;
}

int CreatureList::giveNumRace( const QString & race )
{
	int ret = 0, cpt = 0;

	QListIterator<Race *> iteRace( *this );
	while( iteRace.hasNext() ) {
		if( iteRace.next()->getName() == race ) {
			ret = cpt;
		}
		cpt++;
	}

	return ret;
}

int CreatureList::computeSequence( GenericFightUnit * unit )
{
	int ret = 0;

	for( uint i = 0; i < unit->getRace(); i++ ) {
		ret += DataTheme.creatures.getRace( i )->count();
	}

	return ret + unit->getLevel();
}

