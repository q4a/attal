/****************************************************************
**
** Attal : Lords of Doom
**
** action.cpp
** Manage actions of 'buildings'
**
** Version : $Id: action.cpp,v 1.15 2008/06/12 13:26:08 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 23/01/2002
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

#include "action.h"
 
// generic include files
// include files for QT
#include <QTextStream>
// application specific include files
#include "libCommon/dataTheme.h"


// 
// ----- Action -----
// 

Action::Action()
{
	_type = Action::DATE;
	_coeff = 0;
}

Action::~Action()
{
	while( ! _list.isEmpty() ) {
		delete _list.takeFirst();
	}
}
	
QString Action::getActionTypeString( ActionType type )
{
	QString ret = tr("Unknown");


	switch( type ) {
		case DATE:
			ret = tr("date");
			break;
		case FIRSTTIME:
			ret = tr("first time");
			break;
		case FIRSTTIMELORD:
			ret = tr("first time lord");
			break;
		case NEXTTIME:
			ret = tr("next time");
			break;
		 case CHANGEOWNER:
			ret = tr("change owner");
		default:
			break;
	}

	return ret;
}

QString Action::getDescription( bool vert )
{
	QString string = "";

	string = Action::getActionTypeString( getType() );
	string.replace( 0, 1,  string[0].toUpper() );

	switch( getType() ) {
		case DATE :
			string += tr( " every " ) + QString::number( getCoeff() ) + " turns";
			break;
		default:
			break;
	}

	string += vert ? "\n" : " ";

	for( int j = 0; j < getElementaryNumber(); j++ ) {
		ElementaryAction * elementary = getElementaryAction( j );
		string += elementary->getDescription();
		string += vert ? "\n" : " ";
	}

	return string;

}



bool Action::save( QTextStream * ts, uint indent )
{
	indentation( ts, indent );
	*ts << "\t<action type=\"" << (uint) ( getType() )  << "\" coeff=\""  <<  getCoeff() << "\">" << endl;
	indentation( ts, indent );
	for( uint j = 0; j < (uint) getElementaryNumber();  ++j ) {
		ElementaryAction * elea = getElementaryAction(j);
		indentation( ts, indent );
		*ts << "\t<elementary type=\"" << (uint) ( elea->getType()  ) << "\" arg=\""  <<  (uint) (elea->getArg() ) <<"\" arg1=\""  <<  (uint) (elea->getArg1() )<<  "\">";
		*ts << (  elea->getCoeff() ) << "</elementary>" << endl;
	}
	indentation( ts, indent );
	*ts << "\t</action>" << endl;

	return true;
}

//
// ----- Elementary Action -----
//

ElementaryAction::ElementaryAction()
{
	_type = ElementaryAction::ATTACK;
	_arg = 0;
	_arg1 = 0;
	_coeff = 0;
}

QString ElementaryAction::getElemTypeString( ElementaryType type )
{
	QString ret = tr("Unknown");


	switch( type ) {
		case ATTACK:
			ret = tr("attack");
			break;
		case DEFENSE:
			ret = tr("defense");
			break;
		case POWER:
			ret = tr("power");
			break;
		case KNOWLEDGE:
			ret = tr("knowledge");
			break;
		case MOVE:
			ret = tr("move");
			break;
		case MAXMOVE:
			ret = tr("maxmove");
			break;
		case TECHNICPOINT:
			ret = tr("techpoint");
			break;
		case MAXTECHNICPOINT:
			ret = tr("maxtechpoint");
			break;
		case MORALE:
			ret = tr("morale");
			break;
		case LUCK:
			ret = tr("luck");
			break;
		case VISION:
			ret = tr("vision");
			break;
		case EXPERIENCE:
			ret = tr("experience");
			break;
		case SKILL:
			ret = tr("skill");
			break;
		case ARTEFACT:
			ret = tr("artefact");
			break;
		case UNIT:
			ret = tr("unit");
			break;
		case RESSOURCE:
			ret = tr("resource");
			break;
		case RANDRESSOURCE:
			ret = tr("random resource");
			break;
		case RESOURCEPERC:
			ret = tr("percentage resource");
			break;
		case RESOURCEFROM:
			ret = tr("resource from resource");
			break;
		case CHARISMA:
			ret = tr("charisma");
			break;
		default:
			break;
	}

	return ret;
}

QString ElementaryAction::getDescription()
{
	QString tmp = "";
	QString string = "";
	QString add1 = tr("add %1") + " ";

	ElementaryAction::ElementaryType etype = getType();

	switch( etype ) {
		case ElementaryAction::ATTACK:
		case ElementaryAction::DEFENSE:
		case ElementaryAction::POWER:
		case ElementaryAction::KNOWLEDGE:
		case ElementaryAction::MOVE:
		case ElementaryAction::MAXMOVE:
		case ElementaryAction::TECHNICPOINT:
		case ElementaryAction::MAXTECHNICPOINT:
		case ElementaryAction::MORALE:
		case ElementaryAction::LUCK:
		case ElementaryAction::VISION:
		case ElementaryAction::EXPERIENCE:
		case ElementaryAction::CHARISMA:
			string = ElementaryAction::getElemTypeString( etype );
			string.replace( 0, 1,  string[0].toUpper() );
			tmp =  add1.arg( getCoeff()).append( string );	
			break;
		case ElementaryAction::SKILL:
			break;
		case ElementaryAction::ARTEFACT:
			break;
		case ElementaryAction::UNIT:
		  tmp = tr("produce") + " " + QString::number( getCoeff() ) + " " + DataTheme.creatures.at( getArg() , getArg1() )->getName();
			break;				
		case ElementaryAction::RESSOURCE:
			{
				QString tmp1 =DataTheme.resources.getRessource( getArg() );
				tmp = tr("give %1").arg( getCoeff()).append(" ").append(tmp1);
			}

			break;
		case ElementaryAction::RANDRESSOURCE:
			{
				QString tmp1 = DataTheme.resources.getRessource( getArg() );
				tmp = tr("give between 0 and %1").arg( getCoeff()*2).append(" ").append(tmp1);
			}
			break;
		case ElementaryAction::RESOURCEPERC:
			{
				QString tmp1 = DataTheme.resources.getRessource( getArg() );
				tmp = tr("modify") + " " + tmp1 + " "+tr("by") + " " + QString::number( getCoeff() ) + " " +tr("percentage");
			}
			break;

		case ElementaryAction::RESOURCEFROM:
			{
				QString tmp1 = DataTheme.resources.getRessource( getArg() );
				QString tmp2 = DataTheme.resources.getRessource( getArg1() );
				tmp = tr("for every") + " " + QString::number(getCoeff()) + " " + tmp1 +  " " + tr("give one") + " " + tmp2;
			}
			break;
		default:
			break;
	}

	return tmp;


}


//
// ----- InsideAction -----
//

InsideAction::InsideAction()
{
	_type = 0;
}

void InsideAction::addParam( int param )
{
	_params.append( param );
}

int InsideAction::getParam( uint num )
{
	int ret = 0;

	if( (int)num < _params.count() ) {
		ret = _params.at( num );
	}

	return ret;
}



