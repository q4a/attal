/****************************************************************
**
** Attal : Lords of Doom
**
** artefact.cpp
** manages artefacts
**
** Version : $Id: artefact.cpp,v 1.16 2009/10/22 17:18:29 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 18/08/2000
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

#include "artefact.h"
 
// generic include files
// include files for QT
#include <QString>
#include <QTextStream>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericCell.h"
#include "libCommon/genericLord.h"

extern QString DATA_PATH;

//
// ----- GenericArtefact -----
//

GenericArtefact::GenericArtefact()
	: _id( 0 ), _type( 0 )
{
}

GenericArtefact::~GenericArtefact()
{
}

void GenericArtefact::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<artefact type=\"" << _type << "\">" << endl;

	indentation( ts, indent );
	*ts << "</artefact>" << endl;
	*ts << flush;
}

QString GenericArtefact::getName()
{
	return DataTheme.artefacts.at( _type )->getName();
}


//
// ----- GenericLordArtefact -----
//

GenericLordArtefact::GenericLordArtefact()
{
	_type = 0;
	_pos = 0;
	_lord = 0;
}

GenericLordArtefact::~GenericLordArtefact()
{
}

void GenericLordArtefact::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<artefact type=\"" << _type << "\">" << endl;

	indentation( ts, indent );
	*ts << "</artefact>" << endl;
	*ts << flush;
}

GenericLord * GenericLordArtefact::getLord()
{
	return _lord;
}

void GenericLordArtefact::setLord( GenericLord * lord )
{
	_lord = lord;
}

int GenericLordArtefact::getModelPosition()
{
	return DataTheme.artefacts.at( _type )->getPosition();
}


int GenericLordArtefact::getPosition()
{
	int ret = -1;

	ret = _pos;

	return ret;
}

void GenericLordArtefact::setPosition( int position )
{
	_pos = position;
}

QString GenericLordArtefact::getName()
{
	return DataTheme.artefacts.at( _type )->getName();
}

int GenericLordArtefact::computeCharacModif( LordCharac charac, int value, ArtefactElementaryAction::ModifType modif )
{
	int ret = value;

	GenericArtefactModel * model = DataTheme.artefacts.at( _type );
	ArtefactElementaryAction * action;
	for( uint i = 0; i < model->getActionNumber(); ++i ) {
		 action = model->getAction( i );
		 if( action->getActionType() == ArtefactElementaryAction::AT_CHARAC ) {
		 	if( ( action->getModifType() == modif ) and ( action->getParam() == (uint)charac ) ) {
		 		ret = action->computeModif( value );
			}
		 }
	}

	return ret;
}


//
// ----- ArtefactElementaryAction -----
//

ArtefactElementaryAction::ArtefactElementaryAction( ActionType action, ModifType modif )
{
	_type = action;
	_modif = modif;
	_value = 0;
}

int ArtefactElementaryAction::computeModif( int value )
{
	int ret = value;

	if( _type == AT_CHARAC ) {
		if( _modif == MT_VALUE ) {
			ret += _value;
		} else {
			ret = (int)( (ret * _value) / 100 );
		}
	}

	return ret;
}

void ArtefactElementaryAction::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	if( _type == ArtefactElementaryAction::AT_CHARAC ) {
		*ts << "<elementary type=\"CHARAC\" ";
	} else {
		*ts << "<elementary type=\"SKILL\" ";
	}

	if( _modif == ArtefactElementaryAction::MT_VALUE ) {
		*ts << "modif=\"VALUE\" ";
	} else {
		*ts << "modif=\"PERCENT\" ";
	}
	*ts << "param=\"" << _param << "\">";
	*ts << _value << "</elementary>" << endl;
}

//
// ----- GenericArtefactModel -----
//

GenericArtefactModel::GenericArtefactModel()
	:_name( "" ), _position( 0 )
{
}

GenericArtefactModel::~GenericArtefactModel()
{
	clearActions();
}

ArtefactElementaryAction * GenericArtefactModel::getAction( uint num )
{
	ArtefactElementaryAction * ret = 0;

	if( (int)num < _actions.count() ) {
		ret = _actions.at( num );
	}

	return ret;
}

void GenericArtefactModel::addAction( ArtefactElementaryAction * action )
{
	_actions.append( action );
}

void GenericArtefactModel::clearActions()
{
	while( ! _actions.isEmpty() ) {
		delete _actions.takeFirst();
	}
}

void GenericArtefactModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<artefact>" << endl;

	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;

	indentation( ts, indent );
	*ts << "\t<position>" << _position << "</position>" << endl;

	indentation( ts, indent );
	*ts << "\t<action>" << endl;

	for( int i = 0; i < _actions.count(); i++ ) {
		_actions.at( i )->save( ts, indent+2 );
	}

	indentation( ts, indent );
	*ts << "\t</action>" << endl;
	
	indentation( ts, indent );
	*ts << "</artefact>" << endl;
}

//
// ----- ArtefactList -----
//

ArtefactList::ArtefactList()
{
	
}

ArtefactList::~ArtefactList()
{
	clear();
}


void ArtefactList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

bool ArtefactList::init()
{
	clear();
	ArtefactHandler handler( this );
	QFile file( DATA_PATH + "artefacts.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable( QString( DATA_PATH + "artefacts.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool ArtefactList::save()
{
	QString filename = DATA_PATH + "artefacts.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE artefacts>" << endl;
	ts << "<artefacts>" << endl;

	for( int i = 0; i < count(); i++ ) {
		at( i )->save( &ts, 1 );
	}

	ts << "</artefacts>" << endl;

	file.close();

	return true;
}
//
// ----- ArtefactHandler -----
//

ArtefactHandler::ArtefactHandler( ArtefactList * list )
{
	_list = list;
}

bool ArtefactHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();
	
	_state = StateInit;
	return true;
}

bool ArtefactHandler::startElement( const QString&, const QString&, const QString& qName, const QXmlAttributes & atts)
{
	if( qName == "artefacts" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "artefact" && _state == StateDocument ) {
		_state = StateArtefact;
		_artefact = new GenericArtefactModel();
	} else if( qName == "name" && _state == StateArtefact ) {
		_state = StateName;
	} else if( qName == "position" && _state == StateArtefact ) {
		_state = StatePosition;
	} else if( qName == "action" && _state == StateArtefact ) {
		_state = StateAction;
	} else if( qName == "elementary" && _state == StateAction ) {
		ArtefactElementaryAction::ActionType action;
		ArtefactElementaryAction::ModifType modif;
		_state = StateActionElementary;
		if( atts.value( "type" ).toUpper() == "CHARAC" ) {
			action = ArtefactElementaryAction::AT_CHARAC;
		} else {
			action = ArtefactElementaryAction::AT_SKILL;
		}
		if( atts.value( "modif" ).toUpper() == "VALUE" ) {
			modif = ArtefactElementaryAction::MT_VALUE;
		} else {
			modif = ArtefactElementaryAction::MT_RATIO;
		}
		_action = new ArtefactElementaryAction( action, modif );
		_action->setParam( atts.value( "param" ).toInt() );
	} else {
		// error
		return false;
	}
	return true;
}

bool ArtefactHandler::endElement( const QString&, const QString&, const QString& )
{
	switch ( _state ) {
	case StateArtefact:
		_list->append( _artefact );
		_state = StateDocument;
		break;
	case StateName:
		_state = StateArtefact;
		break;
	case StatePosition:
		_state = StateArtefact;
		break;
	case StateAction:
		_state = StateArtefact;
		break;
	case StateActionElementary:
		_artefact->addAction( _action );
		_state = StateAction;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool ArtefactHandler::characters( const QString& ch )
{
	QString ch_simplified = ch.simplified();
	if ( ch_simplified.isEmpty() )
		return true;

	switch( _state ) {
	case StateName:
		_artefact->setName( ch_simplified );
		break;
	case StatePosition:
		_artefact->setPosition( ch_simplified.toInt() );
		break;
	case StateActionElementary:
		_action->setValue( ch_simplified.toInt() );
		break;
	default:
	    return false;
    }

    return true;
}


bool ArtefactHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}




