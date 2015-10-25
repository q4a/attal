/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGround.h
** section for specifying grounds
**
** Version : $Id: sectionGround.h,v 1.8 2007/05/05 23:53:08 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

 
#ifndef SECTIONGROUND_H
#define SECTIONGROUND_H
 
 
// generic include files
// include files for QT
#include <QWidget>
#include <QVBoxLayout>
// application specific include files
#include "themeEditor/sectionWidget.h"


class AskDiversificationList;

/*              ------------------------------
 *                         SectionGround
 *              ------------------------------ */



/** comment for the class */
class SectionGround : public GenericSection
{
 	Q_OBJECT
public:
	/** Constructor */
	SectionGround( QWidget * parent = 0, const char * name = 0 );

	~SectionGround();

	/** Selects the first item */
	void selectFirst();

	/** Selects the previous item */
	void selectPrevious();

	/** Selects the next item */
	void selectNext();

	/** Selects the last item */
	void selectLast();

	/** Creates new item */
	void selectNew();

	/** Deletes current item */
	void selectDel();

	/** Clears the section */
	void clear();

	/** Initialize the section */
	void init();

	/** Saves the data */
	void save();

protected:
	int _num;
	AskString * _name;
	AskInt * _coef, * _width, *  _height;
	AskColor * _color;
	AskDiversificationList * _diversification;
	QHBoxLayout * _layH;
};

#endif // SECTIONGROUND_H
