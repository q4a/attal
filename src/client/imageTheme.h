/****************************************************************
**
** Attal : Lords of Doom
**
** imageTheme.h
** manage images in theme
**
** Version : $Id: imageTheme.h,v 1.21 2002/12/22 19:56:35 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 10/06/2001
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

 
#ifndef IMAGETHEME_H
#define IMAGETHEME_H
 

// generic include files
// include files for QT
#include <qcanvas.h>
// application specific include files

class GenericFightUnit;


#define NB_TRANSITION 16

/**              ------------------------------
 *                          ImageTheme
 **              ------------------------------ */

class CreaturePixmap
{
public:
	CreaturePixmap(QList<QPixmap> pix, QList<QPoint> hotspots );
	~CreaturePixmap();
	QCanvasPixmapArray * operator[] ( uint i );

private:

	QCanvasPixmapArray * _pixmaps[2];
};

/** comment for the class */
class ImageTheme
{
public:
	/** Constructor */
	ImageTheme();

	/** Destructor */
	~ImageTheme();

	/** Initializes the images of the theme */
	void init();

	/** Saves the images of the theme */
	void save();

	/** Clears the images of the theme */
	void clear();

	QString getPhotoCreature( GenericFightUnit * );

	/** \return Returns the pixmap of inside the base 'num' */
	QPixmap * getInsideBase( uint num );

	/** \return Returns the icon of the artefcat 'num' */
	QPixmap * getArtefactIcon( uint num );

	/** \return Returns the flag of the team 'num' */
	QPixmap * getFlag( uint num );

	QCanvasPixmapArray * cells;
	QCanvasPixmapArray * pathCells;
	QCanvasPixmapArray * lords;
	QCanvasPixmapArray ** buildings;
	QCanvasPixmapArray * bases;
	QCanvasPixmapArray ** insideBuildings;
	QCanvasPixmapArray * skills;
	//QCanvasPixmapArray ** creatures[2];
	CreaturePixmap ** creatures;
	QCanvasPixmapArray * deadCreature;
	QCanvasPixmapArray ** transition;
	QCanvasPixmapArray * decoration;
	QCanvasPixmapArray * artefacts;
	QCanvasPixmapArray ** mapCreatures;
	QCanvasPixmapArray ** events;

	/** Return pixmap of a lord */
	QPixmap * getLordPixmap( uint num );

	/** Return small pixmap of a lord */
	QPixmap * getLordSmallPixmap( uint num );

	/** Return small pixmap of a base */
	QPixmap * getBaseSmallPixmap( uint num );


private:
	void initLords();
	void initBases();

	QPixmap ** _insideBase;
	QPixmap ** _artefactIcon;
	QPixmap ** _flags;

	QPixmap ** _lordPixmap;
	QPixmap ** _lordSmallPixmap;
	QPixmap ** _baseSmallPixmap;
};

#endif // IMAGETHEME_H
