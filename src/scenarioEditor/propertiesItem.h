/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesItem.h
** Defines properties box for items on map
**
** Version : $Id: propertiesItem.h,v 1.5 2002/12/15 12:08:43 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/12/2001
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

 
#ifndef PROPERTIES_H
#define PROPERTIES_H
 
 
// generic include files
// include files for QT
#include <qdialog.h>
#include <qlist.h>
// application specific include files
#include "common/genericPlayer.h" 

class QComboBox;
class QListView;
class GenericLord;
class GenericBase;

/*              ------------------------------
 *                         PropertiesLord
 *              ------------------------------ */


/** Dialog for editing the properties of the base */
class PropertiesBase : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	PropertiesBase( QWidget * parent = 0, const char * name = 0 );

	void init( GenericBase * base );

public slots:
	void slot_accept();

private:
	GenericBase * _base;
	QListView * _listBuilding;
};

#endif // PROPERTIESITEM_H   
