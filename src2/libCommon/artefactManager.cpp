/****************************************************************
**
** Attal : Lords of Doom
**
** artefactManger.cpp
** Manages artefacts of a lord
**
** Version : $Id: artefactManager.cpp,v 1.10 2007/08/24 22:16:19 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 30/11/2002
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

#include "artefactManager.h"

// generic include files
// include files for QT
#include <QTextStream>

// application specific include files
#include "libCommon/genericLord.h"

/// XXX: very strange memory management -> should be memory leaks in artefact management because of the 2 different addArtefact()

ArtefactManager::ArtefactManager( GenericLord * lord )
{
	_lord = lord;
}

ArtefactManager::~ArtefactManager()
{
	clearArtefacts();
}

void ArtefactManager::save( QTextStream * ts, int indent )
{
	for( int i = 0; i < _artefacts.count (); i++ ) {
		_artefacts.at(i)->save( ts, indent + 1 );
	}
}

uint ArtefactManager::getArtefactNumber()
{
	return _artefacts.count();
}

bool ArtefactManager::addArtefact( int /*position*/, int /*num*/, GenericLordArtefact * /*artefact*/ )
{
	return false;
}

GenericLordArtefact * ArtefactManager::getArtefact( int /*position*/, int /*num*/ )
{
	return 0;
}

bool ArtefactManager::removeArtefact( int /*position*/, int /*num*/ )
{
	return false;
}

void ArtefactManager::addArtefact( GenericLordArtefact * artefact )
{
	_artefacts.append( artefact );
}

void ArtefactManager::addArtefact( uint type )
{
	GenericLordArtefact * artefact = new GenericLordArtefact();
	artefact->setType( type );
	artefact->setLord( _lord );
	_artefacts.append( artefact );
}

GenericLordArtefact * ArtefactManager::getArtefact( int num )
{
	return _artefacts.at( num );
}

GenericLordArtefact * ArtefactManager::getArtefactByType( uint type )
{
	GenericLordArtefact * ret = 0;

	uint nb = _artefacts.count();
	for( uint i = 0; i < nb; i++ ) {
		GenericLordArtefact * artefact = _artefacts.at( i );
		if( artefact->getType() == type ) {
			ret = artefact;
			break;
		}
	}

	return ret;
}

void ArtefactManager::removeArtefact( int num )
{
	_artefacts.removeAt( num );
}

void ArtefactManager::removeArtefact( GenericLordArtefact * artefact )
{
	_artefacts.removeAll( artefact );
}

void ArtefactManager::removeArtefactByType( uint type )
{
	uint nb = _artefacts.count();
	for( uint i = 0; i < nb; i++ ) {
		GenericLordArtefact * artefact = _artefacts.at( i );
		if( artefact->getType() == type ) {
			_artefacts.removeAll( artefact );
		}
	}
}

bool ArtefactManager::hasArtefactType( uint type )
{
	bool ret = false;

	uint nb = _artefacts.count();
	for( uint i = 0; i < nb; i++ ) {
		GenericLordArtefact * artefact = _artefacts.at( i );
		if( artefact->getType() == type ) {
			ret = true;
		}
	}

	return ret;
}

void ArtefactManager::clearArtefacts()
{
	while( ! _artefacts.isEmpty() ) {
		delete _artefacts.takeFirst();
	}
}

int ArtefactManager::computeCharacModif( LordCharac charac, int value )
{
	int ret = value;

	for( int i = 0; i < _artefacts.count(); ++i ) {
		ret = _artefacts.at( i )->computeCharacModif( charac, ret, ArtefactElementaryAction::MT_VALUE );
	}
	for( int i = 0; i < _artefacts.count(); ++i ) {
		ret = _artefacts.at( i )->computeCharacModif( charac, ret, ArtefactElementaryAction::MT_RATIO );
	}

	return ret;
}
