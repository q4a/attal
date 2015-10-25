/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapCreature.cpp
** Manages creatures on map
**
** Version : $Id: genericMapCreature.cpp,v 1.24 2008/06/16 14:00:55 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/09/2002
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

#include "genericMapCreature.h"

// generic include files
#include <stdlib.h>
// include files for QT
#include <QString>
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/define.h"

GenericMapCreature::GenericMapCreature()
{
	_ress = new GenericResourceList();

	_creature = NULL;
	_categoryNumber = 1;
	_stacks.resize( MAX_UNIT );
	for( uint i = 0; i < MAX_UNIT; i++ ) {
		if(_stacks[i]) {
			delete _stacks[i];
		}
		_stacks[i] = new uint( 0 ) ;
	}
	
	_estimated = true;
	_behaviour = Neutral;
	_growth = Stable;
	_growthParam0 = 0;
	_growthParam1 = 0;
	_flee = false;
	_lookingRight = false;
	
}

GenericMapCreature::~GenericMapCreature()
{
	for (int i = 0; i < _stacks.size(); ++i) {
		uint * ptr = _stacks.at(i);
		_stacks[i] = 0;
		delete ptr;
	}

	delete _ress;
}

void GenericMapCreature::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<creature>" << endl;

	if( _cell ) {
		indentation( ts, indent+1 );
		*ts << "<row>" << _cell->getRow() << "</row>" << endl;
		indentation( ts, indent+1 );
		*ts << "<col>" << _cell->getCol() << "</col>" << endl;
	}

	if( _creature ) {
		indentation( ts, indent+1 );
		*ts << "<race>" << _creature->getRace() << "</race>" << endl;
		indentation( ts, indent+1 );
		*ts << "<level>" << _creature->getLevel() << "</level>" << endl;
	}

	for( uint i = 0; i < MAX_UNIT; i++ ) {
		if( getStack(i) > 0 ) {
			indentation( ts, indent+1 );
			*ts << "<stack num=\"" << i << "\">" << getStack(i) << "</stack>" << endl;
		}
	}

	indentation( ts, indent+1 );
	*ts << "<behaviour>" << (int)_behaviour << "</behaviour>" << endl;

	indentation( ts, indent+1 );
	*ts << "<flee>" << (int)_flee << "</flee>" << endl;

	indentation( ts, indent+1 );
	*ts << "<lookingRight>" << (int)_lookingRight << "</lookingRight>" << endl;

	indentation( ts, indent+1 );
	*ts << "<growth type=\""  << (uint)_growth << "\">" << endl;
	
	if( _growth == FixedPercentage ) {
		indentation( ts, indent + 2 );
		*ts << "<param>" << _growthParam0 << "</param>" << endl;
	} else if( _growth == VariablePercentage ) {
		indentation( ts, indent + 2 );
		*ts << "<param>" << _growthParam0 << "</param>" << endl;
		indentation( ts, indent + 2 );
		*ts << "<param>" << _growthParam1 << "</param>" << endl;
	}
	
	indentation( ts, indent+1 );
	*ts << "</growth>" << endl;

	for( int j = 0; j < DataTheme.resources.count(); j++ ) {
		indentation( ts, indent );
		(*ts) << "\t<resource type=\"" << j << "\">";
		(*ts) << _ress->getValue( j );
		(*ts) << "</resource>" << endl;
	}
	
	indentation( ts, indent );
	*ts << "</creature>" << endl;
}

void GenericMapCreature::setCreature( int race, int level )
{
	_creature = DataTheme.creatures.at( race, level );
}


int GenericMapCreature::getRace()
{
	int ret = 0;

	if( _creature ) {
		ret = _creature->getRace();
	}

	return ret;
}

int GenericMapCreature::getLevel()
{
	int ret = 0;

	if( _creature ) {
		ret = _creature->getLevel();
	}

	return ret;
}

/*
unsigned int GenericMapCreature::getMinEstimatedNumber()
{
	uint ret = 0;

	if( _estimated ) {
		ret =
	} else {
		ret = getCreatureNumber();
	}

	return ret;
}

unsigned int GenericMapCreature::getMaxEstimatedNumber()
{
	uint ret = 0;

	if( _estimated ) {
		ret =
	} else {
		ret = getCreatureNumber();
	}

	return ret;
}
*/

unsigned int GenericMapCreature::getCategoryNumber()
{
	uint ret = 0;

	if( _estimated ) {
		ret = _categoryNumber;
	} else {
		ret = (uchar)DataTheme.computeCategory( getCreatureNumber() );
	}

	return ret;
}


unsigned int GenericMapCreature::getCreatureNumber()
{
	unsigned int ret = 0;

	if( ! _estimated ) {
		for( unsigned int i = 0; i < MAX_UNIT; i++ ) {
			ret += * ( _stacks.at( i ) );
		}
	}

	return ret;
}


void GenericMapCreature::setStack( unsigned int numStack, unsigned int number )
{
	_estimated = false;
	//_stacks.append( new unsigned int( number ) );
	if( (uint)numStack < MAX_UNIT ) {
		* _stacks[ numStack ] = number;
	}
}

unsigned int GenericMapCreature::getStack( unsigned int num )
{
	unsigned int ret = 0;

	if( (uint)num < MAX_UNIT ) {
		ret = * ( _stacks[ num ] );
	}

	return ret;
}

QString GenericMapCreature::getBehaviourString()
{
	return getBehaviourString( _behaviour );
}

QString GenericMapCreature::getBehaviourString( CreatureBehaviour behaviour )
{
	QString ret = "";

	switch( behaviour ) {
	case Obedient:
		ret = QObject::tr( "Obedient" );
		break;
	case Friendly:
		ret = QObject::tr( "Friendly" );
		break;
	case Neutral:
		ret = QObject::tr( "Neutral" );
		break;
	case Aggressive:
		ret = QObject::tr( "Aggressive" );
		break;
	case Hostile:
		ret = QObject::tr( "Hostile" );
		break;
	}

	return ret;
}

QString GenericMapCreature::getGrowthString( GrowthMode growth )
{
	QString ret = "";

	switch( growth ) {
	case Stable:
		ret = QObject::tr( "Stable" );
		break;
	case FixedPercentage:
		ret = QObject::tr( "Percentage fixed" );
		break;
	case VariablePercentage:
		ret = QObject::tr( "Percentage variable" );
		break;
	}

	return ret;
}

uint GenericMapCreature::computeForceIndicator( bool real )
{	
	uint ret = 0;
		
	if( _creature ) {
	
		TRACE("GenericMapCreature::Force Indicator health %d, number %d", _creature->getAttack() , getCreatureNumber());
		
		if( real ) {
			ret = _creature->getMaxHealth() * getCreatureNumber();
		} else {
			uint estimatedNb = DataTheme.getRandomInCategory( getCategoryNumber() );
			ret = _creature->getMaxHealth() * estimatedNb;
		}

	}
		
	return ret;
}

void GenericMapCreature::setGrowthParam( uint param, uint value )
{
	if( param == 0 ) {
		_growthParam0 = value;
	} else if( param == 1 ) {
		_growthParam1 = value;
	}
}
	
uint GenericMapCreature::getGrowthParam( uint param )
{
	uint ret = 0;
	
	if( param == 0 ) {
		ret = _growthParam0;
	} else if( param == 1 ) {
		ret = _growthParam1;
	}
	
	return ret;
}

void GenericMapCreature::grow()
{
	uint value, percent;
	
	switch( _growth ) {
	case Stable:
		break;
	case FixedPercentage:
		for( uint i = 0; i < MAX_UNIT; i++ ) {
			value = getStack( i );
			value = (int)( ( value * (100 + _growthParam0) ) / 100 );
			setStack( i, value );
		}
		break;
	case VariablePercentage:
		for( uint i = 0; i < MAX_UNIT; i++ ) {
			percent = _growthParam0 + ( rand() % (_growthParam1 + 1 - _growthParam0) );
			value = getStack( i );
			value = (int)( ( value * (100 + percent) ) / 100 );
			setStack( i, value );
		}
		break;
	}
}

