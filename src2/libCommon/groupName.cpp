/****************************************************************
**
** Attal : Lords of Doom
**
** groupName.cpp
** Manage name for group of units
**
** Version : $Id: groupName.cpp,v 1.6 2009/09/11 17:01:06 lusum Exp $
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
#include <QFile>
#include <QTextStream>

// application specific include files
#include "libCommon/log.h"

extern QString DATA_PATH;

GroupName::GroupName()
{
}

GroupName::~GroupName()
{
	clear();
}


void GroupName::init()
{
	QString filename = DATA_PATH + "group.dat";
	QFile f( filename );
	if( ! f.open( QIODevice::ReadOnly ) ) {
		logEE( "Unable to open data file (read): %s", qPrintable(filename) );
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
	if( ! f.open( QIODevice::WriteOnly ) ) {
		logEE( "Unable to open data file (write): %s", qPrintable(filename) );
		return;
	}
	
	QTextStream ts( &f );
	ts << _default << endl;
	for( int i = 0; i < _list.count(); i++ ) {
		ts << _list.at( i )->name << " ";
		ts << _list.at( i )->value;
		ts << endl;
	}
	f.close();
}

void GroupName::clear()
{
	_default = "";
	while( ! _list.isEmpty() ) {
		delete _list.takeFirst();
	}
}

QString GroupName::getGroupName( int nb )
{
	QString ret = _default;

	for( int i = 0; i < _list.count(); i++ ) {
		if( _list.at( i )->value > nb ) {
			ret = _list.at( i )->name;
			i = _list.count();
		}
	}
	
	return ret;	
}
