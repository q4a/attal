/****************************************************************
**
** Attal : Lords of Doom
**
** genericBonus.cpp
** Class for managing bonus on map
**
** Version : $Id: genericBonus.cpp,v 1.7 2007/10/15 21:49:56 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "genericBonus.h"

// generic include files
// include files for QT
#include <QObject>
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"

//
// ----- GenericBonus -----
//

GenericBonus::GenericBonus()
{
	_type = BonusResource;
}

GenericBonus::~GenericBonus()
{
}

void GenericBonus::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<bonus type=\"" << (uint)_type << "\">" << endl;

	uint nbParam = _params.count();
	for( uint i = 0; i < nbParam; i++ ) {
		indentation( ts, indent+1 );
		*ts << "<param>" << _params.at( i ) << "</param>" << endl;
	}

	indentation( ts, indent );
	*ts << "</bonus>" << endl;
	*ts << flush;
}

void GenericBonus::addParam( uint value )
{
	_params.append( value );
}

uint GenericBonus::getParamNumber()
{
	return _params.count();
}

uint GenericBonus::getParam( uint num )
{
	uint ret = 0;
	if( (int)num < _params.count() ) {
		ret = _params.at( num );
	}
	return ret;
}

void GenericBonus::clearParams()
{
	_params.clear();
}


QString GenericBonus::getBonusDescription()
{
	QString msg;

	switch( getType() ) {
		case GenericBonus::BonusResource:
			msg = QString( QObject::tr("Resource: ") ) + DataTheme.resources.getRessource( getParam( 0 ) )  + QString( QObject::tr(" Fixed Value: ") ) + QString::number( getParam( 2 ) );
			break;
		case GenericBonus::BonusPrimSkill:
			msg =  QObject::tr("Bonus ");
			break;
		case GenericBonus::BonusSkill:
			msg =  QObject::tr("Bonus ");
			break;
		case GenericBonus::BonusSpell:
			msg =  QObject::tr("Bonus ");
			break;
		default:
			break;
	}

	return msg;

}
