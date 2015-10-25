/****************************************************************
**
** Attal : Lords of Doom
**
** genericMapDisposition.h
** Manage disposition on the map of the building, base...
**
** Version : $Id: genericMapDisposition.h,v 1.5 2006/11/03 18:28:58 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/05/2004
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

 
#ifndef GENERICMAPDISPOSITION_H
#define GENERICMAPDISPOSITION_H


// generic include files
// include files for QT
#include <QObject>
#include <QTextStream>
// application specific include files

class QTextStream;

/*              ------------------------------
 *                   GenericMapDisposition
 *              ------------------------------ */


class GenericMapDisposition
{

public:
	enum DispositionType {
		FREE = 0,
		OCCUPIED,
		DOOR
	};

	/** Constructor */
	GenericMapDisposition();

	/** Destructor */
	virtual ~GenericMapDisposition();
	
	/** clear the disposition */
	void clear();

	/** Init the disposition area */
	void init( uint height, uint width );

	uint getHeight() { return _height; }

	uint getWidth() { return _width; }

	bool save( QTextStream * ts, uint indent );

	/** \return Returns the disposition info of case (row, col) */
	DispositionType getDisposition( uint row, uint col ) { return _dispo[row][col]; }

	/** Sets the disposition of the case (row, col) */
	void setDisposition( uint row, uint col, DispositionType disposition );

	/** \return Returns the row of the door */
	uint getDoorRow() { return _doorRow; }

	/** \return Returns the col of the door */
	uint getDoorCol() { return _doorCol; }
	
	void resize( uint newHeight, uint newWidth );

private:
	DispositionType **  _dispo;
	uint _doorRow, _doorCol;
	uint _height, _width;

};

#endif // GENERICMAPDISPOSITION_H
