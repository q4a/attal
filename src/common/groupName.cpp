/****************************************************************
**
** Attal : Lords of Doom
**
** groupName.cpp
** Manage name for group of units
**
** Version : $Id: groupName.cpp,v 1.2 2002/04/20 18:41:02 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 15/12/2001
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

#include "groupName.h"
 
// generic include files
// include files for QT
#include <qfile.h>
#include <qtextstream.h>
// application specific include files
#include "common/log.h"

extern QString DATA_PATH;

/** add comments here */
GroupName::GroupName()
{
	_list.setAutoDelete( true );
}

void GroupName::init()
{
	QString filename = DATA_PATH + "group.dat";
	QFile f( filename );
	if( ! f.open( IO_ReadOnly ) ) {
		logEE( "Unable to open data file (read): %s", filename.latin1() );
		return;
	}
	
	QTextStream ts( &f );
	ts >> _default;
	while( ! ts.atEnd() ) {
		Group * group = new Group();
		ts >> group->name;
		ts >> group->value;
		_list.append( group );
	}
	f.close();
}

void GroupName::save()
{
	QString filename = DATA_PATH + "group.dat";
	QFile f( filename );
	if( ! f.open( IO_WriteOnly ) ) {
		logEE( "Unable to open data file (write): %s", filename.latin1() );
		return;
	}
	
	QTextStream ts( &f );
	ts << _default << endl;
	for( uint i = 0; i < _list.count(); i++ ) {
		ts << _list.at( i )->name << " ";
		ts << _list.at( i )->value;
		ts << endl;
	}
	f.close();
}

void GroupName::clear()
{
	_default = "";
	_list.clear();
}

QString GroupName::getGroupName( int nb )
{
	QString ret = _default;

	for( uint i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->value > nb ) {
			ret = _list.at( i )->name;
			i = _list.count();
		}
	}
	
	return ret;	
}
