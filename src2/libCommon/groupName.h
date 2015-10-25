/****************************************************************
**
** Attal : Lords of Doom
**
** groupName.h
** Manage name for group of units
**
** Version : $Id: groupName.h,v 1.4 2005/11/11 20:47:48 audoux Exp $
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

 
#ifndef GROUPNAME_H
#define GROUPNAME_H
 
 
// generic include files
// include files for QT
#include <QList>
#include <QString>

// application specific include files
 
 
/**              ------------------------------
 *                          GroupName
 **              ------------------------------ */

struct Group {
	QString name;
	int value;
};


/** comment for the class */
class GroupName
{
public:
	/** Constructor */
	GroupName();

	/** Destructor */
	virtual ~GroupName();

	/** Inits group name list */
	void init();

	/** Saves group name list */
	void save();

	/** Clears group name list */
	void clear();

	/** \return Returns group name 'nb' */
	QString getGroupName( int nb );

private:
	QList<Group *> _list;
	QString _default;
};

#endif // GROUPNAME_H
