/****************************************************************
**
** Attal : Lords of Doom
**
** imageTheme.cpp
** manage images in theme
**
** Version : $Id: imageTheme.cpp,v 1.33 2002/12/22 19:56:35 audoux Exp $
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

#include "imageTheme.h"

// generic include files
// include files for QT
#include <qcanvas.h>
#include <qlist.h>
#include <qpixmap.h>
#include <qimage.h>
// application specific include files
#include "common/dataTheme.h"
#include "common/unit.h"
#include "common/log.h"

ImageTheme ImageTheme;

extern DataTheme DataTheme;
extern QString IMAGE_PATH;


CreaturePixmap::CreaturePixmap( QList<QPixmap> pix, QList<QPoint> hotspots )
{
	_pixmaps[0] = new QCanvasPixmapArray( pix, hotspots );

	/*_pixmaps[1] = new QCanvasPixmapArray( pix, hotspots );
	QCanvasPixmapArray * sequence = _pixmaps[1];
	for( uint ii=0; ii<sequence->count(); ii++ ) {
		QCanvasPixmap * oldPix, * newPix;
		QImage oldImg, newImg;

		oldPix = sequence->image( ii );
		if( !oldPix->isNull() ) {
			oldImg = oldPix->convertToImage();
			newImg = oldImg.mirror( true, false );
		}
		newPix = new QCanvasPixmap( newImg );
		sequence->setImage( ii, newPix );
		delete oldPix;
	}*/
	QList<QPixmap> newList;
	for( uint i=0; i<pix.count(); i++ ) {
		QCanvasPixmap oldPix( *pix.at(i), *hotspots.at(i) );
		QCanvasPixmap * newPix;
		QImage newImg;
		if( !oldPix.isNull() ) {
			newImg = oldPix.convertToImage().mirror( true, false );
			newPix = new QCanvasPixmap( newImg );
			newList.append( newPix );
		} else {
			newList.append( new QPixmap() );
		}
	}
	_pixmaps[1] = new QCanvasPixmapArray( newList, hotspots );
}

CreaturePixmap::~CreaturePixmap()
{
	if( _pixmaps[0] ) {
		delete _pixmaps[0];
	}

	if( _pixmaps[1] ) {
		delete _pixmaps[1];
	}
}

QCanvasPixmapArray * CreaturePixmap::operator[]( uint i )
{
	return _pixmaps[i%2];
}

//
// ----- ImageTheme -----
//

ImageTheme::ImageTheme()
{
	cells = 0;
	lords = 0;
	bases = 0;
	buildings = 0;
	insideBuildings = 0;
	skills = 0;
	transition = 0;
	decoration = 0;
	creatures = 0;
	artefacts = 0;
	events = 0;
	mapCreatures = 0;
	_insideBase = 0;
	_artefactIcon = 0;
	_flags = 0;
}

ImageTheme::~ImageTheme()
{
	clear();
}

QPixmap * ImageTheme::getInsideBase( uint num )
{
	QPixmap * ret = 0;
	if( num < DataTheme.bases.count() ) {
		if( _insideBase[num] == 0 ) {
			QString name = IMAGE_PATH + "base/insideBase_" + QString::number( num ) + ".png";
			_insideBase[num] = new QPixmap( name );
		}
		ret = _insideBase[num];
	}

	return ret;
}

QPixmap * ImageTheme::getArtefactIcon( uint num )
{
	QPixmap * ret;
	if( num < DataTheme.artefacts.count() ) {
		if( _artefactIcon[num] == 0 ) {
			QString name = IMAGE_PATH + "artefacts/artefactIcon_" + QString::number( num ) + ".png";
			_artefactIcon[num] = new QPixmap( name );
		}
		ret = _artefactIcon[num];
	} else {
		logEE( "artefact %d pixmap not found", num );
		ret = new QPixmap();
	}

	return ret;
}

#define CRE_ARRAY( list1, list2 ) \
QList<QPixmap> list1; \
list1.setAutoDelete( true ); \
QList<QPoint> list2; \
list2.setAutoDelete( true )

#define ADD_PIX( list1, list2, name ) \
list1.append( new QPixmap( IMAGE_PATH + name ) ); \
list2.append( new QPoint( 0, 0 ) )

void ImageTheme::init()
{
	int cpt;
	uint i, j;

	CRE_ARRAY( listPixmapSkill, listPointSkill );
	ADD_PIX( listPixmapSkill, listPointSkill, "technics/technic_1_1.png" );
	ADD_PIX( listPixmapSkill, listPointSkill, "technics/technic_1_2.png" );
	skills = new QCanvasPixmapArray( listPixmapSkill, listPointSkill );

	cpt = 0;
	//creatures = new QCanvasPixmapArray * [DataTheme.creatures.count()];
	creatures = new CreaturePixmap * [DataTheme.creatures.count()];
	for( i = 0; i < DataTheme.countRace(); i++ ) {
		for( j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
			QList<QPixmap> listPixmapFightUnit;
			listPixmapFightUnit.setAutoDelete( true );
			QList<QPoint> listPointFightUnit;
			listPointFightUnit.setAutoDelete( true );
			ADD_PIX( listPixmapFightUnit, listPointFightUnit, "units/cre_" + QString::number( i ) + "_" + QString::number( j ) + ".png" );
			creatures[cpt] = new CreaturePixmap( listPixmapFightUnit , listPointFightUnit );
			cpt++;
		}
	}

	CRE_ARRAY( listPixmapCell, listPointCell );
	ADD_PIX( listPixmapCell, listPointCell, "tiles/unknown.png" );
	for( i = 1; i < DataTheme.tiles.count(); i++ ) {
		QString name = "tiles/tile_" + QString::number( i ) + ".png";
		ADD_PIX( listPixmapCell, listPointCell, name.latin1() );
	}
	cells = new QCanvasPixmapArray( listPixmapCell, listPointCell );

	CRE_ARRAY( listPixmapPathCell, listPointPathCell );
	for( i = 0; i < 33; i++ ) {
		QString name = "misc/arrow-" + QString::number( i ) + ".png";
		ADD_PIX( listPixmapPathCell, listPointPathCell, name.latin1() );
	}
	pathCells = new QCanvasPixmapArray( listPixmapPathCell, listPointPathCell );

	CRE_ARRAY( listPixmapLord, listPointLord );
	ADD_PIX( listPixmapLord, listPointLord, "units/lord.png" );
	lords = new QCanvasPixmapArray( listPixmapLord , listPointLord );

	buildings = new QCanvasPixmapArray * [ DataTheme.buildings.count() ];
	for( i = 0; i < DataTheme.buildings.count(); i++ ) {
		CRE_ARRAY( listPixmapBuilding, listPointBuilding );
		for( j = 0; j < DataTheme.buildings.at( i )->getNbFrame(); j++ ) {
			QString name = "building/building_" + QString::number( i ) + "_" + QString::number( j ) + ".png";
			ADD_PIX( listPixmapBuilding, listPointBuilding, name );
		}
		buildings[i] = new QCanvasPixmapArray( listPixmapBuilding, listPointBuilding );
	}

	CRE_ARRAY( listPixmapBase, listPointBase );
	for( i = 0; i < DataTheme.bases.count(); i++ ) {
		ADD_PIX( listPixmapBase, listPointBase, "base/base_" + QString::number( i ) + ".png" );
	}
	bases = new QCanvasPixmapArray( listPixmapBase, listPointBase );

	insideBuildings = new QCanvasPixmapArray * [ DataTheme.bases.count() ];
	for( i = 0; i < DataTheme.bases.count(); i++ ) {
		CRE_ARRAY( listPixmapInside, listPointInside );
		for( j = 0; j < DataTheme.bases.at( i )->getBuildingCount(); j++ ) {
			ADD_PIX( listPixmapInside, listPointInside, "base/inside_" + QString::number( i ) + "_" + QString::number( j ) + ".png" );
			ADD_PIX( listPixmapInside, listPointInside, "base/insideSel_" + QString::number( i ) + "_" + QString::number( j ) + ".png" );
		}
		insideBuildings[i] = new QCanvasPixmapArray( listPixmapInside, listPointInside );
	}

	CRE_ARRAY( listPixmapDeco, listPointDeco );
	for( i = 1; i <= DataTheme.decorations.count(); i++ ) {
		ADD_PIX( listPixmapDeco, listPointDeco, "decor/decoration_" + QString::number( i ) + ".png" );
	}
	decoration = new QCanvasPixmapArray( listPixmapDeco, listPointDeco );

	transition = new QCanvasPixmapArray * [ DataTheme.tiles.count() ];
	for( i = 1; i <= DataTheme.tiles.count(); i++ ) {
		CRE_ARRAY( listPixmapTrans, listPointTrans );
		for( j = 1; j <= NB_TRANSITION; j++ ) {
			QString name = "tiles/transi_" + QString::number( i ) + "_" + QString::number( j ) + ".png";
			ADD_PIX( listPixmapTrans, listPointTrans, name.latin1() );
		}
		transition[i-1] = new QCanvasPixmapArray( listPixmapTrans, listPointTrans );
	}

	CRE_ARRAY( listPixmapArtefact, listPointArtefact );
	for( i = 0; i < DataTheme.artefacts.count(); i++ ) {
		ADD_PIX( listPixmapArtefact, listPointArtefact, "artefacts/artefactItem_" + QString::number( i ) + ".png" );
	}
	artefacts = new QCanvasPixmapArray( listPixmapArtefact, listPointArtefact );

	CRE_ARRAY( listPixmapDead, listPointDead );
	listPixmapDead.append( new QPixmap( IMAGE_PATH + "units/dead.png" ) ); \
	listPointDead.append( new QPoint( 0, -70 ) );
	deadCreature = new QCanvasPixmapArray( listPixmapDead, listPointDead );

	/*
	CRE_ARRAY( listPixmapMapCreature, listPointMapCreature );
	ADD_PIX( listPixmapMapCreature, listPointMapCreature, "units/creMap_0_0.png" );
	mapCreatures = new QCanvasPixmapArray( listPixmapMapCreature, listPointMapCreature );
	*/

	mapCreatures = new QCanvasPixmapArray * [ DataTheme.countRace() ];
	for( i = 0; i < DataTheme.countRace(); i++ ) {
		CRE_ARRAY( listPixmapMapCreature, listPointMapCreature );
		for( j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
			QString name = "units/creMap_" + QString::number( i ) + "_" + QString::number( j ) + ".png";
			ADD_PIX( listPixmapMapCreature, listPointMapCreature, name );
		}
		mapCreatures[i] = new QCanvasPixmapArray( listPixmapMapCreature, listPointMapCreature );
	}

	events = new QCanvasPixmapArray * [ 3 ];
	CRE_ARRAY( listPixmapEvent, listPointEvent );
	for( i = 0; i < 3; i++ ) {
		QString event1 = "events/event1.png";
		ADD_PIX( listPixmapEvent, listPointEvent, event1 );
		events[i] = new QCanvasPixmapArray( listPixmapEvent, listPointEvent );
	}


	_insideBase = new QPixmap * [DataTheme.bases.count()];
	for( i = 0; i < DataTheme.bases.count(); i++ ) {
		_insideBase[i] = 0;
	}

	_artefactIcon = new QPixmap * [DataTheme.artefacts.count()];
	for( i = 0; i < DataTheme.artefacts.count(); i++ ) {
		_artefactIcon[i] = 0;
	}

	_flags = new QPixmap * [DataTheme.teams.count()];
	for( i = 0; i < DataTheme.teams.count(); i++ ) {
		_flags[i] = 0;
	}

	initLords();
	initBases();
}

void ImageTheme::save()
{

}

void ImageTheme::clear()
{
	if( cells ) {
		delete cells;
	}

	if( lords ) {
		delete lords;
	}
	
	if( bases ) {
		delete bases;
	}

	if( buildings ) {
		delete buildings;
	}

	if( insideBuildings ) {
		delete insideBuildings;
	}

	if( skills ) {
		delete skills;
	}

	if( transition ) {
		delete transition;
	}

	if( decoration ) {
		delete decoration;
	}

	if( creatures ) {
		int cpt = 0;
		int i, j;
		for( i = 0; i < DataTheme.countRace(); i++ ) {
			for( j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
				delete creatures[cpt];
				cpt++;
				//XXX: ?? delete creatures;
			}
		}
			delete [] creatures;
	}

	if( artefacts ) {
		delete artefacts;
	}

	if( _artefactIcon ) {
		delete [] _artefactIcon;
	}

	if( _insideBase ) {
		delete [] _insideBase;
	}

	if( _flags ) {
		delete [] _flags;
	}
}

QString ImageTheme::getPhotoCreature( GenericFightUnit * unit )
{
	QString pix = IMAGE_PATH + "units/face_" + QString::number( unit->getRace() ) + "_" + QString::number( unit->getLevel() ) + ".png";
	//logDD("Pixmap: %s", pix.latin1() );
	return  pix;
}

QPixmap * ImageTheme::getFlag( uint num )
{
	QPixmap * ret = 0;

	if( num < DataTheme.teams.count() ) {
		ret = _flags[num];
		if( ! ret ) {
			ret = new QPixmap( IMAGE_PATH + "players/flag_" + QString::number( num ) + ".png" );
		}
	}

	return ret;
}

QPixmap * ImageTheme::getLordPixmap( uint num )
{
	if( num <= DataTheme.lords.count() ) {
		if( ! _lordPixmap[num] ) {
			QString name;
			name.sprintf( "lords/lord_%03d.png", num );
			_lordPixmap[num] = new QPixmap( IMAGE_PATH + name );
		}
		return _lordPixmap[num];
	} else {
		logEE("There is not so many lords");
	}

	return 0;
}

QPixmap * ImageTheme::getLordSmallPixmap( uint num )
{
	if( num <= DataTheme.lords.count() ) {
		if( ! _lordSmallPixmap[num] ) {
			QString name;
			name.sprintf( "lords/smallLord_%03d.png", num );
			_lordSmallPixmap[num] = new QPixmap( IMAGE_PATH + name );
		}
		return _lordSmallPixmap[num];
	} else {
		logEE("There is not so many lords");
	}

	return 0;
}

QPixmap * ImageTheme::getBaseSmallPixmap( uint num )
{
	return _baseSmallPixmap[ 0 ];
}

void ImageTheme::initLords()
{
	uint nbLord = DataTheme.lords.count();
	uint i;

	_lordPixmap = new QPixmap * [ nbLord ];
	for( i = 0; i < nbLord; i++ ) {
		_lordPixmap[i] = 0;
	}

	_lordSmallPixmap = new QPixmap * [ nbLord ];
	for( i = 0; i < nbLord; i++ ) {
		_lordSmallPixmap[i] = 0;
	}
}

void ImageTheme::initBases()
{
	_baseSmallPixmap = new QPixmap * [1];
	_baseSmallPixmap[0] = new QPixmap( IMAGE_PATH + "base/smallBase.png" );
}



