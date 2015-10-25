/****************************************************************
**
** Attal : Lords of Doom
**
** genericRessources.cpp
** Management of ressources infos
**
** Version : $Id: genericRessources.cpp,v 1.26 2009/10/22 17:09:10 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/01/2001
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

#include "genericRessources.h"
 
// generic include files
// include files for QT
#include <QFile>
#include <QTextStream>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

extern QString DATA_PATH;

GenericResource::GenericResource()
{
	_value = 0;
	_minValue = 0;
}

void GenericResource::decreaseValue( int val )
{
	if( val <= _value ) {
		_value -= val;
	} else {
		_value = 0;
	}
}

void GenericResource::increaseValue( int val )
{
	_value += val;
}

void GenericResource::modPercValue( int percentage )
{
	_value = (_value * (100 + percentage )/100);
	if(_value <= 0)
		_value = _minValue;
}

int GenericResource::getRandGrowth( int coeff )
{
	int tmp = 0;
	
	tmp = 2 * (int) (rand() % coeff);

	TRACE("GenericResource::getRandGrowth coeff %d , rand %d", coeff,tmp);

	return tmp;

}

void GenericResource::updateRandGrowth( int coeff )
{
	int ress = getRandGrowth( coeff );
	increaseValue( ress );
}

void GenericResource::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<resource>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<id>" << _id << "</id>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<value>" << _value << "</value>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<minvalue>" << _minValue << "</minvalue>" << endl;

	indentation( ts, indent );
	*ts << "</resource>" << endl;
	*ts << flush;
}


//
// ----- GenericResourceList -----
//

GenericResourceList::GenericResourceList()
{
	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
		append( new GenericResource() );
		at( i )->setId( i );
	}
}

GenericResourceList::~GenericResourceList()
{
	clear();
}

void GenericResourceList::setListValues(GenericResourceList * other)
{
	for( int i = 0; i < count(); i++ )
	{
		setValue( i, other->getValue( i ) );
	}
}

void GenericResourceList::clear()
{
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

GenericResource * GenericResourceList::get( uint num )
{
	for( int i = 0; i < count(); i++ ) {
		if( at( i )->getId() == (int) num ) {
			return at( i );
		}
	}

	return NULL;
}

void GenericResourceList::setValue( uint num, uint val )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			rs->setValue(val);
	}
}

void GenericResourceList::decreaseValue( uint num, uint val )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			rs->decreaseValue( val );
	}
}

void GenericResourceList::increaseValue( uint num, uint val )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			rs->increaseValue( val );
	}
}

int GenericResourceList::getValue( uint num )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			return rs->getValue();
	}

	return 0; 
}

void GenericResourceList::modPercValue( uint num, int percentage )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			rs->modPercValue( percentage );
	}
}

void GenericResourceList::updateRandGrowth( uint num, int coeff )
{
	if( (int)num < DataTheme.resources.count() ) {
		GenericResource * rs = get( num );
		if( rs )
			rs->updateRandGrowth( coeff );
	}
}

void GenericResourceList::removeResources( GenericResourceList * list )
{
	for( int i = 0; i < count(); i++ ) {
		decreaseValue( i , list->getValue( i ) );
	}

}

void GenericResourceList::addResources( GenericResourceList * list )
{
	for( int i = 0; i < count(); i++ ) {
		increaseValue( i , list->getValue( i ) );
	}
}

QString GenericResourceList::getAutoDescription()
{
	QString s = "";

	for( int i = 0; i <  count() ; i++ ) {
		if( getValue( i ) > 0 ) {
			s +=  qPrintable(DataTheme.resources.getRessource( i ) );
			s += QString(" : %1").arg( getValue( i ) );
			s +=	"\n";
		}
	}

	if( !s.isEmpty() ) {
		s.prepend("\nResources\n");
	}
	
	return s;
}

void GenericResourceList::print()
{
	for( int i = 0; i < count(); i++ ) {
		logDD("Ress %d, value %d", i, getValue( i ) );
	}
}

bool GenericResourceList::isRess()
{
	for( int i = 0; i < count(); i++ ) {
		if( getValue( i ) != 0 )
			return true;
	}
	return false;
}


//
// ResourceModel 
// 


ResourceModel::ResourceModel()
{
	_type = RE_GLOB_NOPRESERVABLE;
}


bool ResourceModel::isPreservable()
{
	if( _type == ResourceModel::RE_GLOB_NOPRESERVABLE || _type == ResourceModel::RE_LOC_NOPRESERVABLE ) 
		return false;

	return true;
}

bool ResourceModel::isGlobal()
{
	if( _type == ResourceModel::RE_GLOB_NOPRESERVABLE || _type == ResourceModel::RE_LOC_PRESERVABLE ) 
		return true;

	return false;
}

void ResourceModel::setId( int id )
{
	TRACE("setId %d", id);

	_id = id;
	if( _filename.isEmpty() ) {
		_filename.sprintf( "ressources/ress_%02d.png", _id + 1 );
	}

	if( _smallfilename.isEmpty() ) {
		_smallfilename.sprintf( "ressources/smallResource_%02d.png", _id + 1 );
	}
}

void ResourceModel::save( QTextStream * ts, int indent )
{
	indentation( ts, indent );
	*ts << "<resource>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<id>" << _id << "</id>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<type>" << _type << "</type>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<name>" << _name << "</name>" << endl;

	indentation( ts, indent );
	*ts << "\t<filename>" << _filename << "</filename>" << endl;
	
	indentation( ts, indent );
	*ts << "\t<smallfilename>" << _smallfilename << "</smallfilename>" << endl;

	indentation( ts, indent );
	*ts << "</resource>" << endl;
	*ts << flush;
}

//
// ----- ResourceList -----
//

ResourceList::ResourceList()
{
}

ResourceList::~ResourceList()
{
	clear();
}

void ResourceList::clear()
{
	_type.clear();
	while( ! isEmpty() ) {
		delete takeFirst();
	}
}

bool ResourceList::init()
{
	TRACE("ResourceList::init");

	clear();
	ResourceHandler handler( this );
	QFile file( DATA_PATH + "resources.dat" );
	QXmlInputSource source( &file );
	QXmlSimpleReader reader;
	reader.setContentHandler( &handler );
	reader.setErrorHandler( &handler );
	bool ok = reader.parse( source );
	file.close();
	if ( !ok ) {
		logEE( "Parse Error (%s) : %s", qPrintable ( QString( DATA_PATH + "resources.dat" ) ), qPrintable( handler.errorProtocol() ) );
		return false;
	}

	return true;
}

bool ResourceList::save()
{
	
	QString filename = DATA_PATH + "resources.dat";
	QFile file( filename );

	if (! file.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writing\n", qPrintable(filename) );
		return false;
	}

	QTextStream ts( &file );

	ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE resources>" << endl;
	ts << "<resources>" << endl;

	ResourceModel * ress;
	for( int i = 0; i < count(); i++ ) {
		ress = at( i );
		if( ress ) {
			ress->save( &ts, 1 );
		}
	}

	ts << "</resources>" << endl;

	file.close();

	return true;
	
}

//compatibility
QString ResourceList::getRessource( uint num )
{
	if( (int)num < count() ) {
		return at( num )->getResourceName();
	} else {
		return QString("");
	}
}

ResourceModel * ResourceList::get( int id )
{
	ResourceModel * ret = first();
	ResourceModel * current;
	
	QListIterator<ResourceModel *> ite( *this );
	
	while( ite.hasNext() ) {
		current = ite.next();
		if( current->getId() == id ) {
			ret = current;
		}
	}	

	return ret;
}

ResourceHandler::ResourceHandler( ResourceList * list )
{
	_list = list;
}

bool ResourceHandler::startDocument()
{
	// at the beginning of parsing: do some initialization
	_errorProt = "";
	_list->clear();

	_state = StateInit;
	return true;
}

bool ResourceHandler::startElement( const QString &, const QString &, const QString & qName, const QXmlAttributes & /* atts */)
{
	if( qName == "resources" && _state == StateInit ) {
		_state = StateDocument;
	} else if( qName == "resource" && _state == StateDocument ) {
		_state = StateResource;
		_ress = new ResourceModel();
	} else if( qName == "id" && _state == StateResource ) {
		_state = StateResourceId;
	} else if( qName == "type" && _state == StateResource ) {
		_state = StateResourceType;
	} else if( qName == "name" && _state == StateResource ) {
		_state = StateResourceName;
	} else if( qName == "filename" && _state == StateResource ) {
		_state = StateFilename;
	} else if( qName == "smallfilename" && _state == StateResource ) {
		_state = StateSmallFilename;
	} else {
		// error
		return false;
	}
	return true;
}

bool ResourceHandler::endElement( const QString &, const QString &, const QString & )
{
	switch ( _state ) {
	case StateResource:
		_list->append( _ress );
		_state = StateDocument;
		break;
	case StateResourceId:
		_state = StateResource;
		break;
	case StateResourceType:
		_state = StateResource;
		break;
	case StateResourceName:
		_state = StateResource;
		break;
	case StateFilename:
		_state = StateResource;
		break;
	case StateSmallFilename:
		_state = StateResource;
		break;
	default:
	    // do nothing
	    break;
    }
    return true;	
}

bool ResourceHandler::characters( const QString & ch )
{
	QString ch_simplified = ch.simplified();
	if( ch_simplified.isEmpty() ) {
		return true;
	}

	switch( _state ) {
	case StateResourceName:
		_ress->setResourceName( ch_simplified );
		break;
	case StateFilename:
		_ress->setFileName( ch_simplified );
		break;
	case StateSmallFilename:
		_ress->setSmallFileName( ch_simplified );
		break;
	case StateResourceId:
		_ress->setId( ch_simplified.toInt() );
		break;
	case StateResourceType:
		_ress->setType( ( ResourceModel::ResourceType ) ch_simplified.toInt() );
		break;

	default:
	    return false;
    }

    return true;
}


bool ResourceHandler::fatalError( const QXmlParseException& exception )
{
    _errorProt += QString( "fatal parsing error: %1 in line %2, column %3\n" )
	.arg( exception.message() )
	.arg( exception.lineNumber() )
	.arg( exception.columnNumber() );

    return QXmlDefaultHandler::fatalError( exception );
}


GenericRessources::GenericRessources()
{
}

bool GenericRessources::init()
{
	clear();
	QString filename = DATA_PATH + "ressources.dat";
	QFile f( filename );

	if (! f.open( QIODevice::ReadOnly ) ) {
		logEE( "Could not open file %s for reading\n", qPrintable(filename) );
		return false;
	}
	
	QTextStream ts( &f );
	
	int temp;
	ts >> temp;
	
	for( int i = 0; i < temp; i++ ) {
		QString es;
		ts >> es;
		append( es );
		//logDD( "%s", qPrintable( ress ) );
	}

	f.close();

	return true;
}


bool GenericRessources::save()
{
	QString filename = DATA_PATH + "ressources.dat";
	QFile f( filename );

	if (! f.open( QIODevice::WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", qPrintable(filename) );
		return false;
	}
	
	QTextStream ts( &f );
		
	ts << count() << endl;
	
	for( int i = 0; i < count(); i++ ) {
		ts << at( i ) << endl;
	}
	
	f.close();

	return true;
}

void GenericRessources::update( int num, QString val )
{
	(*this)[ num ] = val;
}

GenericRessources::~GenericRessources()
{
	clear();
}

QString GenericRessources::getRessource( uint num )
{
	if( (int)num < count() ) {
		return at( num );
	} else {
		return QString("");
	}
}
