/****************************************************************
**
** Attal : Lords of Doom
**
** genericRessources.cpp
** Management of ressources infos
**
** Version : $Id: genericRessources.cpp,v 1.6 2002/05/22 21:35:59 audoux Exp $
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
#include <qfile.h>
#include <qlistbox.h>
#include <qtextstream.h>
// application specific include files
#include "common/log.h"

extern QString DATA_PATH;

GenericRessources::GenericRessources()
{
	setAutoDelete( true );
}

void GenericRessources::init()
{
	clear();
	QString filename = DATA_PATH + "ressources.dat";
	QFile f( filename );

	if (! f.open( IO_ReadOnly ) ) {
		logEE( "Could not open file %s for reading\n", filename.latin1() );
		return;
	}
	
	QTextStream ts( &f );
	
	int temp;
	ts >> temp;
	
	for( int i = 0; i < temp; i++ ) {
		QString es;
		ts >> es;
		QString * ress = new QString( es );
		append( ress );
		//logDD( "%s", ress->latin1() );
	}
	
	
	f.close();
}

bool GenericRessources::save()
{
	QString filename = DATA_PATH + "ressources.dat";
	QFile f( filename );

	if (! f.open( IO_WriteOnly ) ) {
		logEE( "Could not open file %s for writng\n", filename.latin1() );
		return false;
	}
	
	QTextStream ts( &f );
		
	ts << count() << endl;
	
	for( uint i = 0; i < count(); i++ ) {
		ts << *at( i ) << endl;
	}
	
	f.close();

	return true;
}

void GenericRessources::update( int num, QString val )
{
	*at( num ) = val;
}

GenericRessources::~GenericRessources()
{
	clear();
}

QString GenericRessources::getRessource( uint num )
{
	if( num < count() ) {
		return *at( num );
	} else {
		return QString("");
	}
}
