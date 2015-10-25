/****************************************************************
**
** Attal : Lords of Doom
**
** genericRessources.h
** Management of ressources infos
**
** Version : $Id: genericRessources.h,v 1.6 2002/03/04 22:40:56 audoux Exp $
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

 
#ifndef GENERICRESSOURCES_H
#define GENERICRESSOURCES_H
 
 
// generic include files
// include files for QT
#include <qstring.h>
#include <qlist.h>
// application specific include files
 
class QListBox;
 
/*              ------------------------------
 *                         GenericRessources
 *              ------------------------------ */



/** comment for the class */
class GenericRessources : public QList<QString>
{
 
public:
	/** Construtor : load ressources info from 'filename' */
	GenericRessources();
	
	/** Destructor (for pointer) */
	virtual ~GenericRessources();	
	
	/** Initializes ressources */
	void init();
	
	bool save();

	void update( int num, QString val );
	
	/** Return the ressources 'num' if possible */
	QString getRessource( uint num );
};
 
#endif // GENERICRESSOURCES_H
