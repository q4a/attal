/****************************************************************
**
** Attal : Lords of Doom
**
** genericChest.h
** Class for managing treasury chests on map
**
** Version : $Id: genericChest.h,v 1.4 2005/11/11 20:47:48 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/08/2003
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


#ifndef GENERICCHEST_H
#define GENERICCHEST_H


// generic include files
// include files for QT
#include <QList>

// application specific include files

class QTextStream;

/*              ------------------------------
 *                        GenericChest
 *              ------------------------------ */



class GenericChest
{

public:
	/** Constructor */
	GenericChest();

	virtual ~GenericChest();

	void addParam( uint value );

	uint getParamNumber();

	uint getParam( uint num );

	/** Save building */
	void save( QTextStream * ts, int indent = 0 );

protected:
	QList<uint> _params;
};

#endif // GENERICCHEST_H
