/****************************************************************
**
** Attal : Lords of Doom
**
** quest.cpp
** Manages quests in Attal
**
** Version : $Id: quest.cpp,v 1.27 2007/05/05 21:52:53 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 02/04/2002
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

#include "quest.h"

// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/artefactManager.h"
#include "libCommon/calendar.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericPlayer.h"

//
// ----- Quest -----
//

Quest::Quest()
{
	_type = ONLY_ONE;
	_needConfirmation = true;
	_confirmationQuestion = tr( "Do you accept this quest ?" );
	_startCondition = NULL;
	_failCondition = NULL;
	_successCondition = NULL;
}

Quest::~Quest()
{
	if( _startCondition ) {
		delete _startCondition;
	}
	if( _failCondition ) {
		delete _failCondition;
	}
	if( _successCondition ) {
		delete _successCondition;
	}
}

QString Quest::getTypeName( QuestType type )
{
	QString ret;

	switch( type ) {
	case ONLY_ONE:
		ret = tr( "Only once" );
		break;
	case ONE_PERPLAYER:
		ret = tr( "Once for each player" );
		break;
	case ONE_PERLORD:
		ret = tr( "Once for each lord" );
		break;
	case ANYONE:
		ret = tr( "Anytime requested" );
		break;
	}

	return ret;
}

void Quest::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<name>" << _name << "</name>" << endl;

	indentation( ts, indent );
	*ts << "<description>" << _description << "</description>" << endl;

	indentation( ts, indent );
	*ts << "<start>" << endl;

	if( _startCondition ) {
		_startCondition->save( ts, indent + 1 );
	}

	indentation( ts, indent );
	*ts << "</start>" << endl;

	if( _needConfirmation ) {
		indentation( ts, indent );
		*ts << "<confirmation>" << _confirmationQuestion << "</confirmation>" << endl;
	}

	indentation( ts, indent );
	*ts << "<fail>" << endl;

	if( _failCondition ) {
		_failCondition->save( ts, indent + 1 );
	}

	indentation( ts, indent );
	*ts << "</fail>" << endl;

	indentation( ts, indent );
	*ts << "<success>" << endl;

	if( _successCondition ) {
		_successCondition->save( ts, indent + 1 );
	}

	indentation( ts, indent );
	*ts << "</success>" << endl;
}

//
// ----- QuestData -----
//

QuestData::QuestData()
{
	_lord = 0;
	_player = 0;
	_currentDate = 0;
	_currentTurn = 0;
}

//
// ----- QuestCondition -----
//

QuestCondition::~QuestCondition()
{
}

void QuestCondition::clear()
{
}

//
// ----- QuestConditionComposite -----
//

QuestConditionComposite::QuestConditionComposite( CompositeType type )
{
	_type = COMPOSITE;
	_compositeType = type;
}

QuestConditionComposite::~QuestConditionComposite()
{
	clear();
}

void QuestConditionComposite::clear()
{
	while( ! _list.isEmpty() ) {
		delete _list.takeFirst();
	}
}

void QuestConditionComposite::addCondition( QuestCondition * condition )
{
	_list.append( condition );
}

void QuestConditionComposite::save( QTextStream * ts, int indent )
{
	uint nb;

	indentation( ts, indent );
	*ts << "<condition type=\"composite\">" << endl;

	indentation( ts, indent+1 );
	if( _compositeType == AND ) {
		*ts << "<param>AND</param>" << endl;
	} else {
		*ts << "<param>OR</param>" << endl;
	}
	nb = _list.count();
	for( uint i = 0; i < nb; ++i ) {
		_list.at( i )->save( ts, indent+1 );
	}

	indentation( ts, indent );
	*ts << "</condition>" << endl;
}

uint QuestConditionComposite::getConditionNumber()
{
	return _list.count();
}

QuestCondition * QuestConditionComposite::getCondition( uint nb )
{
	QuestCondition * ret = 0;

	if( (int)nb < _list.count() ) {
		ret = _list.at( nb );
	}

	return ret;
}

void QuestConditionComposite::delCondition( uint nb )
{
	delete _list.takeAt( nb );
}

void QuestConditionComposite::delCondition( QuestCondition * condition )
{
	_list.removeAll( condition );
	delete condition;
}


bool QuestConditionComposite::check( QuestData * data )
{
	bool ret = false;
	uint i, nb;

	nb = _list.count();
	if( nb ) {
		if( _compositeType == AND  ) {
			ret = true;
			for( i = 0; i < nb; i++ ) {
				ret = ret && _list.at( i )->check( data );
			}
		} else {
			ret = false;
			for( i = 0; i < nb; i++ ) {
				ret = ret || _list.at( i )->check( data );
			}
		}
	}

	return ret;
}

QString QuestConditionComposite::getLabel()
{
	QString ret;

	if( _compositeType == AND ) {
		ret = "AND";
	} else {
		ret = "OR";
	}

	return ret;
}

//
// ----- QuestConditionDate -----
//

QuestConditionDate::QuestConditionDate()
{
	_type = QuestCondition::DATE;
	_category = DELAY;
	_day = 0;
	_month = 0;
	_week = 0;
	_year = 0;
	_nbDay = 0;
}

void QuestConditionDate::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	
	*ts << "<condition type=\"date\">" << endl;

	indentation( ts, indent+1 );
	*ts << "<param>" << _category << "</param>" << endl;
	if( _category == DATE ) {
		indentation( ts, indent+1 );
		*ts << "<param>" << _day << "</param>" << endl;
		indentation( ts, indent+1 );
		*ts << "<param>" << _week << "</param>" << endl;
		indentation( ts, indent+1 );
		*ts << "<param>" << _month << "</param>" << endl;
		indentation( ts, indent+1 );
		*ts << "<param>" << _year << "</param>" << endl;
	} else {
		indentation( ts, indent+1 );
		*ts << "<param>" << _nbDay << "</param>" << endl;
	}

	indentation( ts, indent );
	*ts << "</condition>" << endl;
}

bool QuestConditionDate::check( QuestData * data )
{

	bool ret = false;
	long absoluteCurrentDay, absoluteQuestDay;
	uint turn;

	if( _category == DATE ) {
		absoluteQuestDay = Calendar::getAbsoluteDate( _day , _week , _month , _year );
		absoluteCurrentDay = data->getCurrentAbsoluteDate();
		if( absoluteCurrentDay > absoluteQuestDay ) {
			ret = true;
		}
	} else if( _category == DELAY ) {
		turn = data->getCurrentTurn();

		TRACE(" check turn %d , _nbDay %d", turn, _nbDay);

		if( turn > _nbDay) {
			ret = true;
		}
	}

	return ret;
}

QString QuestConditionDate::getLabel()
{
	QString ret;

	if( _category == DATE ) {
		ret = tr( "Must be accomplished before day %1, week %2, month %3" ).arg( _day).arg(_week).arg( _month );
	} else {
		ret = tr( "Must be accomplished before %1 days" ).arg( _nbDay );
	}

	return ret;
}

void QuestConditionDate::setConditionDate( uint day, uint week, uint month, uint year )
{
	_category = DATE;
	_day = day;
	_week = week;
	_month = month;
	_year = year;
}

void QuestConditionDate::setConditionDelay( uint nbDay )
{
	_category = DELAY;
	_nbDay = nbDay;
}

//
// ----- QuestConditionLord -----
//

QuestConditionLord::QuestConditionLord()
{
	_type = LORD;
	_category = PRIMARY_SKILL;
	_charac = ATTACK;
	_check = EQUAL;
	_present = true;
	_value = 0;
	_secondary = 0;
	_artefact = 0;
	_spell = 0;
}

void QuestConditionLord::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<condition type=\"lord\">" << endl;

	indentation( ts, indent+1 );
	*ts << "<param>" << _category << "</param>" << endl;

	switch( _category ) {
	case PRIMARY_SKILL:
		savePrimary( ts, indent + 1 );
		break;
	case SECONDARY_SKILL:
		//saveSecondary( ts, indent + 1 );
		break;
	case ARTEFACT:
		saveArtefact( ts, indent + 1 );
		break;
	case NB_CREATURES:
		saveNumber( ts, indent + 1 );
		break;
	case SPELL:
		//saveSpell( ts, indent + 1 );
		break;
	}

	indentation( ts, indent );
	*ts << "</condition>" << endl;
}

void QuestConditionLord::saveNumber( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<param>" << (uint)_check << "</param>" << endl;
	indentation( ts, indent );
	*ts << "<param>" << _value << "</param>" << endl;
}

void QuestConditionLord::savePrimary( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<param>" << (uint)_charac << "</param>" << endl;
	indentation( ts, indent );
	*ts << "<param>" << (uint)_check << "</param>" << endl;
	indentation( ts, indent );
	*ts << "<param>" << _value << "</param>" << endl;
}


void QuestConditionLord::saveArtefact( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<param>" << _artefact << "</param>" << endl;
	indentation( ts, indent );
	*ts << "<param>" << (uint)_present << "</param>" << endl;
}

bool QuestConditionLord::check( QuestData * data )
{
	bool ret = false;

	switch( _category ) {
	case PRIMARY_SKILL:
		ret = checkPrimary( data );
		break;
	case SECONDARY_SKILL:
		//saveSecondary( ts, indent + 1 );
		break;
	case ARTEFACT:
		if( data->getLord()->getArtefactManager()->hasArtefactType( _artefact ) ) {
			ret = true;
		}
		break;
	case NB_CREATURES:
		ret = checkPrimary( data );
		break;
	case SPELL:
		//saveSpell( ts, indent + 1 );
		break;
	}

	return ret;
}

bool QuestConditionLord::checkPrimary( QuestData * data )
{
	bool ret = false;
	GenericLord * lord = data->getLord();
	uint valLord = 0;

	if( lord ) {
		switch( _category ) {
			case PRIMARY_SKILL:
				valLord = lord->getCharac( _charac );
				break;
			case SECONDARY_SKILL:
				break;
			case NB_CREATURES:
				valLord = lord->countUnits();
				break;
			default:
				break;
		}
		switch( _check ) {
		case INFERIOR:
			ret = ( valLord < _value );
			break;
		case INFERIOR_EQUAL:
			ret = ( valLord <= _value );
			break;
		case EQUAL:
			ret = ( valLord == _value );
			break;
		case SUPERIOR_EQUAL:
			ret = ( valLord >= _value );
			break;
		case SUPERIOR:
			ret = ( valLord > _value );
			break;
		}
	}

	return ret;
}

QString QuestConditionLord::getLabel()
{
	QString ret;

	ret.sprintf( "Lord condition" );

	return ret;
}

void QuestConditionLord::setPrimarySkillCondition( LordCharac charac, CheckType check, uint value )
{
	_category = PRIMARY_SKILL;
	_charac = charac;
	_check = check;
	_value = value;
}

void QuestConditionLord::setNumberCondition( CheckType check, uint value )
{
	_category = NB_CREATURES;
	_check = check;
	_value = value;
}

void QuestConditionLord::setSecondarySkillCondition( uint secondarySkill, bool present )
{
	_category = SECONDARY_SKILL;
	_secondary = secondarySkill;
	_present = present;
}

void QuestConditionLord::setArtefactCondition( uint artefact, bool present )
{
	_category = ARTEFACT;
	_artefact = artefact;
	_present = present;
}

void QuestConditionLord::setSpellCondition( uint spell, bool present )
{
	_category = SPELL;
	_spell = spell;
	_present = present;
}

//
// ----- QuestConditionPlayer -----
//

QuestConditionPlayer::QuestConditionPlayer()
{
	_type = PLAYER;
	_category = NB_LORD;
	_check = EQUAL;
	_value = 0;
	_name = "";;
}

void QuestConditionPlayer::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<condition type=\"player\">" << endl;

	indentation( ts, indent+1 );
	*ts << "<param>" << (uint)_category << "</param>" << endl;

	if( _category == NB_LORD ) {
		indentation( ts, indent+1 );
		*ts << "<param>" << (uint)_check << "</param>" << endl;
		indentation( ts, indent+1 );
		*ts << "<param>" << _value << "</param>" << endl;
	} else if( _category == NB_BASE ) {
		indentation( ts, indent+1 );
		*ts << "<param>" << (uint)_check << "</param>" << endl;
		indentation( ts, indent+1 );
		*ts << "<param>" << _value << "</param>" << endl;
	} else {
		indentation( ts, indent+1 );
		*ts << "<param>" << _name << "</param>" << endl;
	}	

	indentation( ts, indent );
	*ts << "</condition>" << endl;
}

bool QuestConditionPlayer::check( QuestData * data )
{
	bool ret = false;
	GenericPlayer * player = data->getPlayer();
	uint valPlayer;

	switch( _category ) {
	case NB_LORD:
		valPlayer = player->numLord();
		switch( _check ) {
		case INFERIOR:
			ret = ( valPlayer < _value );
			break;
		case INFERIOR_EQUAL:
			ret = ( valPlayer <= _value );
			break;
		case EQUAL:
			ret = ( valPlayer == _value );
			break;
		case SUPERIOR_EQUAL:
			ret = ( valPlayer >= _value );
			break;
		case SUPERIOR:
			ret = ( valPlayer > _value );
			break;
		}
		break;
	case NB_BASE:
		valPlayer = player->numBase();
		switch( _check ) {
		case INFERIOR:
			ret = ( valPlayer < _value );
			break;
		case INFERIOR_EQUAL:
			ret = ( valPlayer <= _value );
			break;
		case EQUAL:
			ret = ( valPlayer == _value );
			break;
		case SUPERIOR_EQUAL:
			ret = ( valPlayer >= _value );
			break;
		case SUPERIOR:
			ret = ( valPlayer > _value );
			break;
		}
		break;
	case BASENAME: {
		uint nbBases = player->numBase();
		GenericBase * base;
		for( uint i = 0; i < nbBases; i++ ) {
			base = player->getBase( i );
			if( base->getName() == _name ) {
				ret = true;
				break;
			}
		}
		} break;
	}

	return ret;
}

QString QuestConditionPlayer::getLabel()
{
	return "Player condition";
}

void QuestConditionPlayer::setNbLordCondition( CheckType check, uint value )
{
	_category = NB_LORD;
	_value = value;
	_check = check;
}
	
void QuestConditionPlayer::setNbBaseCondition( CheckType check, uint value )
{
	_category = NB_BASE;
	_value = value;
	_check = check;
}
	
void QuestConditionPlayer::setBasenameCondition( const QString & name )
{
	_category = BASENAME;
	_name = name;
}




