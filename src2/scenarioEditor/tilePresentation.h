/****************************************************************
**
** Attal : Lords of Doom
**
** tilePresentation.h
** Displays tiles with diversification
**
** Version : $Id: tilePresentation.h,v 1.3 2005/11/13 13:40:07 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/01/2004
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


#ifndef TILEPRESENTATION_H
#define TILEPRESENTATION_H


// generic include files
// include files for QT
#include <QWidget>
// application specific include files

class QComboBox;
class AttalButton;

class CellModel;

 /*              ------------------------------
 *                     TilePresentation
 *              ------------------------------ */

class TilePresentation : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	TilePresentation( QWidget * parent = 0, const char * name = 0 );

	void setCell( CellModel * cell );

	AttalButton * getButton() {
		return _button;
	}

	uint getDiversification();

protected:
	AttalButton * _button;
	QComboBox * _combo;
	CellModel * _cell;
};
#endif // TILEPRESENTATION_H
