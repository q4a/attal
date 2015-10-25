/****************************************************************
**
** Attal : Lords of Doom
**
** imageTheme.cpp
** manage images in theme
**
** Version : $Id: imageTheme.cpp,v 1.93 2014/06/20 16:14:14 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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

#define NO_EXTERN_IMAGETHEME
#include "imageTheme.h"

// generic include files

// include files for QT
#include <QImage>
#include <QPixmap>
#include <QTime>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericMapCreature.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"

ImageTheme ImageTheme;

extern QString IMAGE_PATH;

//obsolete
/*
CreaturePixmap::CreaturePixmap( const QList<QPixmap> & pix )
{
	_pixmaps[0] = NULL;
	_pixmaps[1] = NULL;
	
	_pixmaps[0] = new QList<QPixmap> ( pix );
	_loaded = false;
}
*/

CreaturePixmap::CreaturePixmap( const QList<QPixmap> & pix, const QList<QString> & filename )
{

	_filenames = new QList<QString> ( filename );
	_pixmaps[0] = NULL;
	_pixmaps[1] = NULL;
	
	_pixmaps[0] = new QList<QPixmap> ( pix );
	_loaded = false;

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

void CreaturePixmap::loadMirrorImage()
{

	QList<QPixmap> * pix = _pixmaps[0];
	if( _pixmaps[1]) {
		delete _pixmaps[1];
	}


	//lazy initialization of mirror image
	QList<QPixmap>  newList;
	int count = pix->count();
	for( int i = 0; i < count; i++ ) {
		if( !pix->at(i).isNull() ) {
			QImage newImg = (QImage) pix->at(i).toImage().mirrored( true, false );
			QPixmap newPix  = QPixmap::fromImage( newImg );
			newList.append( newPix ) ;
		} else {
			newList.append( QPixmap() );
		}
	}
	_pixmaps[1] = new QList<QPixmap>( newList );

}	

QList<QPixmap> * CreaturePixmap::operator[]( uint i )
{
	int index = i%2;

	if( _loaded == false ) {
		QList<QPixmap> * pix = _pixmaps[0];

		//lazy initialization of creatures image
		int count = pix->count();
		QList<QPixmap>  newList;
		for( int i = 0; i < count; i++ ) {
			if( pix->at(i).isNull() ) {
				QPixmap newPix  = QPixmap( _filenames->at(i) );
				newList.append( newPix ) ;
			} else {
				newList.append( pix->at(i) );
			}
		}

		delete _pixmaps[0];
		_pixmaps[0] = new QList<QPixmap>(newList);
		_loaded = true;
	}

	if(index == 1 ) {
			if( !_pixmaps[1] ||  _pixmaps[1]->isEmpty())
				loadMirrorImage();
		}

	return _pixmaps[index];
}

//
// ----- MapCreaturePixmap -----
//

MapCreaturePixmap::MapCreaturePixmap( const QList<QPixmap> & pix)
{
	_pixmaps[0] = new QList<QPixmap>( pix );

	QList<QPixmap> newList;
	for( int i = 0; i < pix.count(); i++ ) {
		//QPixmap oldPix( pix[ i ]);
		QImage newImg;
		if( !pix.at(i).isNull() ) {
			newImg = pix.at(i).toImage().mirrored( true, false );
			QPixmap newPix = newPix.fromImage(newImg);
			newList.append( newPix ) ;
		} else {
			newList.append( QPixmap() );
		}
	}
	_pixmaps[1] = new QList<QPixmap>( newList);

}

MapCreaturePixmap::~MapCreaturePixmap()
{
	if( _pixmaps[0] ) {
		delete _pixmaps[0];
	}

	if( _pixmaps[1] ) {
		delete _pixmaps[1];
	}
}

QList<QPixmap> * MapCreaturePixmap::getList( uint i )
{
	return _pixmaps[i%2];
}

QList<QPixmap> * MapCreaturePixmap::operator[]( uint i )
{
	return _pixmaps[i%2];
}


//
// ----- ImageTheme -----
//

/*
ImageTheme:
call init() to allocate resource (in class constructor)
call explicitly clean() to deallocate resources (in class destructor)
*/

ImageTheme::ImageTheme()
{
	cells = NULL;
	lords = NULL;
	bases = NULL;
	buildings = NULL;
	insideBuildings = NULL;
	insideBuildingsView = NULL;
	photoCreatures = NULL;
	transition = NULL;
	decoration = NULL;
	creatures = NULL;
	artefacts = NULL;
	events = NULL;
	chest = NULL;
	mapCreatures = NULL;
	pathCells = NULL;
	deadCreature = NULL;
	_insideBase = NULL;
	_artefactIcon = NULL;
	_resourceIcon = NULL;
	_resourceSmallIcon = NULL;
	_flags = NULL;
	_mapFlags = NULL;
	_sound = NULL;
	_okPixmap = NULL;
	_cancelPixmap = NULL;
	_shroudImage = NULL;
	_lordPixmap = NULL;
	_lordSmallPixmap = NULL;
	_baseSmallPixmap = NULL;
	_widgetPixmap = NULL;
	_isLoaded = false;
}

ImageTheme::~ImageTheme()
{
	//clear();
}

QPixmap * ImageTheme::getInsideBase( uint num )
{
	QPixmap * ret = NULL;
	if( (int)num < DataTheme.bases.count() ) {
		if( !_insideBase[num] ) {
			QString name = IMAGE_PATH + "base/race_" + QString::number( num ) + "/insideBase.png";
			_insideBase[num] = new QPixmap( name );
		}
		ret = _insideBase[num];
	}

	return ret;
}

QPixmap * ImageTheme::getArtefactIcon( uint num )
{
	QPixmap * ret;
	if( (int)num < DataTheme.artefacts.count() ) {
		if( !_artefactIcon[num] ) {
			QString name = IMAGE_PATH + "artefacts/artefactIcon_" + QString::number( num ) + ".png";
			_artefactIcon[num] = new QPixmap( name );
		}
		ret = _artefactIcon[num];
	} else {
		logEE( "artefact %d pixmap not found", num );
		ret = new QPixmap(); /// XXX: memory leak...
	}

	return ret;
}

QPixmap * ImageTheme::getPopulationIcon()
{
	QPixmap * ret;
		if( !_populIcon ) {
			QString name = IMAGE_PATH + "misc/population.png";
			_populIcon = new QPixmap( name );
		}
		
	ret = _populIcon;

	return ret;

}

QPixmap * ImageTheme::getResourceIcon( uint num )
{
	QPixmap * ret;
	if( (int)num < DataTheme.resources.count() ) {
		if( !_resourceIcon[num] ) {
			QString name = DataTheme.resources.at( num )->getFileName();
			_resourceIcon[num] = new QPixmap( IMAGE_PATH + name );
		}
		ret = _resourceIcon[num];
	} else {
		logEE( "artefact %d pixmap not found", num );
		ret = new QPixmap();
	}

	return ret;
}

QPixmap * ImageTheme::getResourceSmallIcon( uint num )
{
	QPixmap * ret;
	if( (int)num < DataTheme.resources.count() ) {
		if( !_resourceSmallIcon[num] ) {
			QString name = DataTheme.resources.at( num )->getSmallFileName();
			_resourceSmallIcon[num] = new QPixmap( IMAGE_PATH + name );
		}
		ret = _resourceSmallIcon[num];
	} else {
		logEE( "artefact %d pixmap not found", num );
		ret = new QPixmap();
	}

	return ret;
}

bool ImageTheme::init()
{
	QTime timems;
	
	timems.start();
	_isLoaded = initResources();
	logDD( "Load Resources %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initCreatures();
	logDD("Load Creatures %d ms",timems.restart());
	emit sig_loadStep();
	_isLoaded = _isLoaded && initCells();
	logDD( "Load Cells %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initBuildings();
	logDD( "Load Buildings %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initDecorations();
	logDD( "Load Decorations %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initTransitions();
	logDD( "Load Transitions %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initArtefacts();
	logDD( "Load Artefacts %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initBonus();
	logDD( "Load Bonus %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initChest();
	logDD( "Load Chests %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initMapCreatures();
	logDD( "Load MapCreatures %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initEvents();
	logDD( "Load Events %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initTeams();
	logDD( "Load Teams %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initLords();
	logDD( "Load Lords %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initBases();
	logDD( "Load Bases %d ms", timems.restart() );
	emit sig_loadStep();
	_isLoaded = _isLoaded && initSound();

	return isLoaded();
}

void ImageTheme::save()
{

}

void ImageTheme::clear()
{
	int cpt = 0;
	uint nbCells = DataTheme.tiles.count();
	uint nbRaces = DataTheme.countRace();
	uint nbLord = DataTheme.lords.count();
	uint nbTeams = DataTheme.teams.count();
	uint nbBases = DataTheme.bases.count();
	uint i,j;

	if( cells ) {
		for( i = 0; i < nbCells; i++ ) {
			delete cells[i];
		}
		delete [] cells;
		cells = NULL;
	}
  	if( pathCells ) {
		delete pathCells ;
		 pathCells  = NULL;
	}

	if( lords ) {
		delete lords;
		lords = NULL;
	}
	
	if( chest ) {
		delete chest;
		chest = NULL;
	}
	
	if( bases ) {
		delete bases;
		bases = NULL;
	}

	if( buildings ) {
		uint nbBuildings = DataTheme.buildings.count();
		for( i = 0; i < nbBuildings; i++ ) {
			delete buildings[ i ];
		}
		delete [] buildings;
		buildings = NULL;
	}

	if( insideBuildings ) {
		uint nbBases = DataTheme.bases.count();
		for( i = 0; i < nbBases; i++ ) {
			delete insideBuildings[ i ];
		}
		delete [] insideBuildings;
		insideBuildings = NULL;
	}

	if( insideBuildingsView ) {
		uint nbBases = DataTheme.bases.count();
		for( i = 0; i < nbBases; i++ ) {
			delete insideBuildingsView[ i ];
		}
		delete [] insideBuildingsView;
		insideBuildingsView = NULL;
	}

	if( photoCreatures ) {
		uint nbRaces = DataTheme.countRace();
		for( i = 0; i < nbRaces; i++ ) {
			delete [] photoCreatures[ i ];
		}
		delete [] photoCreatures;
		photoCreatures = NULL;
	}


	if( transition ) {
		for( i = 1; i <= nbCells; i++ ) {
			delete transition[ i - 1 ];
		}
		delete [] transition;
		transition = NULL;
	}

	if( decoration ) {
		for( i = 1; i < (uint) DataTheme.decorations.count(); i++ ) {
			delete decoration[ i - 1 ];
		}
		delete [] decoration;
		decoration = NULL;
	}

	if( creatures ) {
		uint nbCreatures;
		for( i = 0; i < nbRaces; i++ ) {
			nbCreatures = DataTheme.creatures.getRace( i )->count();
			for( j = 0; j < nbCreatures; j++ ) {
				delete creatures[cpt];
				cpt++;
			}
		}
		delete [] creatures;
		creatures = NULL;
	}
	
	if( deadCreature ) {
		delete deadCreature;
		deadCreature = NULL;
	}

	if( artefacts ) {
		delete artefacts;
		artefacts = NULL;
	}

	if( _artefactIcon ) {
		uint nbArtefacts = DataTheme.artefacts.count();
		for( i = 0; i < nbArtefacts; i++ ) {
			if( _artefactIcon[i] ) {
				delete _artefactIcon[i];
			}
		}
		delete [] _artefactIcon;
		_artefactIcon = NULL;
	}

	if( _resourceIcon ) {
		uint nbRes = DataTheme.resources.count();
		for( i = 0; i < nbRes; i++ ) {
			if( _resourceIcon[i] ) {
				delete _resourceIcon[i];
			}
		}
		delete [] _resourceIcon;
		_resourceIcon = NULL ;
	}

	if( _resourceSmallIcon ) {
		uint nbRes = DataTheme.resources.count();
		for( i = 0; i < nbRes; i++ ) {
			if( _resourceSmallIcon[i] ) {
				delete _resourceSmallIcon[i];
			}
		}
		delete [] _resourceSmallIcon;
		_resourceSmallIcon = NULL;
	}

	if( bonus ) {
		for( i = 0; i < 4; i++ ) {
			delete bonus[i];
		}
		delete [] bonus;
		bonus = NULL;
	}
	
	if( events ) {
		for( i = 0; i < 3; i++ ) {
			delete events[i];
		}
		delete [] events;
		events = NULL;
	}
	
	if( mapCreatures ) {
		uint nbCreatures;
		for( i = 0; i < nbRaces; i++ ) {
			nbCreatures = DataTheme.creatures.getRace( i )->count();
			for( j = 0; j < nbCreatures; j++ ) {
				delete mapCreatures[i][j];
			}
			delete [] mapCreatures[i];
		}
		delete [] mapCreatures;
		mapCreatures = NULL;
	}

	if( _insideBase ) {
		for( i = 0; i < nbBases; i++ ) {
			delete _insideBase[i];
		}
		delete [] _insideBase;
		_insideBase = NULL;
	}

	if( _flags ) {
		for( uint i = 0; i < nbTeams; i++ ) {
			delete _flags[ i ];
		}
		delete [] _flags;
		_flags = NULL;
	}
	
	if( _mapFlags ) {
		for( uint i = 0; i < 1; i++ ) {
			for( uint j = 0; j < nbTeams; j++ ) {
				delete _mapFlags[i][j];
			}
			delete [] _mapFlags[i];
		}
		delete [] _mapFlags;
		_mapFlags = NULL;
	}
	
	if(_lordPixmap) {
		for( uint i = 0; i < nbLord; i++ ) {
			delete _lordPixmap[i];
		}
		delete [] _lordPixmap;
		_lordPixmap = NULL;
	}

	if(_lordSmallPixmap) {
		for( uint i = 0; i < nbLord; i++ ) {
			delete _lordSmallPixmap[i];
		}
		delete [] _lordSmallPixmap;
		_lordSmallPixmap = NULL;
	}
	
	if( _sound ) {
		delete _sound;
	}
	_sound = NULL;
	
	if( _baseSmallPixmap ) {
			delete _baseSmallPixmap[ 0 ];
			delete [] _baseSmallPixmap;
			_baseSmallPixmap = NULL;
	}

	delete _okPixmap;
	_okPixmap = NULL;

	delete _cancelPixmap;
	_cancelPixmap = NULL;
	
	delete _shroudImage;
	_shroudImage = NULL;
	
	if( _widgetPixmap ) {
		for( int i = 0; i < WIDG_LAST; i++ ) {
			delete _widgetPixmap[i];
		}
		delete [] _widgetPixmap;
		_widgetPixmap = NULL;
	}
}

QPixmap ImageTheme::getPhotoCreature( GenericFightUnit * unit )
{
	uint race = unit->getRace();
	uint level = unit->getLevel();

	return getPhotoCreature( race , level );

}

QPixmap ImageTheme::getPhotoCreature( int race, int level )
{
	if( photoCreatures[ race ][level].isNull() ) {
		QString pix = IMAGE_PATH + "units/race_" + QString::number( race ) + "/level_" + QString::number( level ) + "/face.png";
		//logDD("Pixmap: %s", pix.toLatin1() );
		photoCreatures[ race ][level] = pix;
	}
	return photoCreatures[race ][level];
}

QPixmap * ImageTheme::getFlag( uint num )
{
	QPixmap * ret = NULL;

	if( (int)num < DataTheme.teams.count() ) {
		if( ! _flags[num] ) {
			_flags[ num ] = new QPixmap( IMAGE_PATH + "players/flag_" + QString::number( num ) + ".png" );
		}
		ret = _flags[num];
	}

	return ret;
}

QPixmap * ImageTheme::getLordPixmap( uint num )
{
	if( (int)num <= DataTheme.lords.count() ) {
		if( ! _lordPixmap[num] ) {
			QString name;
			name.sprintf( "lords/lord_%03d.png", num );
			_lordPixmap[num] = new QPixmap( IMAGE_PATH + name );
		}
		return _lordPixmap[num];
	} else {
		logEE("There is not so many lords");
	}

	return NULL;
}

QPixmap * ImageTheme::getLordSmallPixmap( uint num )
{
	if( (int)num <= DataTheme.lords.count() ) {
		if( ! _lordSmallPixmap[num] ) {
			QString name;
			name.sprintf( "lords/smallLord_%03d.png", num );
			_lordSmallPixmap[num] = new QPixmap( IMAGE_PATH + name );
		}
		return _lordSmallPixmap[num];
	} else {
		logEE("There is not so many lords");
	}

	return NULL;
}

QPixmap * ImageTheme::getBaseSmallPixmap( uint num )
{

	if( (int)num <= DataTheme.bases.count() ) {
		if( ! _baseSmallPixmap[num] ) {
			QString name;
			name =  "base/race_" + QString::number( num ) + "/smallBase.png" ;
			_baseSmallPixmap[num] = new QPixmap( IMAGE_PATH + name );
		}
		return _baseSmallPixmap[num];
	} else {
		logEE("There is not so many bases");
	}

	return NULL;

}

QList<QPixmap> * ImageTheme::getMapFlag( uint type, uint teamId )
{
	return _mapFlags[ type ][ teamId ];
}

const uint NB_FLAGTYPE = 1;

bool ImageTheme::initTeams()
{
	uint i, j;
	uint nbTeams = DataTheme.teams.count();
	QString fileName;
	
	_flags = new QPixmap * [ nbTeams ];
	for( i = 0; i < nbTeams; i++ ) {
		_flags[i] = NULL;
	}
	
	/// XXX: NB_FLAGTYPE = 1 = nb of type of flags (only one for the moment but will be extended if managed in dataTheme
	_mapFlags = new QList<QPixmap> ** [ NB_FLAGTYPE ];
	
	for( i = 0; i < NB_FLAGTYPE; i++ ) {
		_mapFlags[ i ] = new QList<QPixmap> * [ nbTeams ];
		for( j = 0; j < nbTeams; j++ ) {
			QList<QPixmap> listPixmapFlag;
			/// XXX: for the moment, only one pixmap, but maybe one day it will be animated...
			fileName.sprintf( "players/mapFlag_%d_%d_0.png", i, j );
			listPixmapFlag.append( QPixmap( IMAGE_PATH + fileName.toLatin1() ) );
			_mapFlags[ i ][ j ] = new QList<QPixmap>( listPixmapFlag );
		}
	}

	return true;
}

bool ImageTheme::initCells()
{
	QString name;

	cells = new QList<QPixmap>* [DataTheme.tiles.count()];

	QList<QPixmap> listPixmapUnknown;
	listPixmapUnknown.append( QPixmap( IMAGE_PATH + "tiles/unknown.png" ) );
	cells[ 0 ] = new QList<QPixmap>( listPixmapUnknown );

	for( int i = 1; i < DataTheme.tiles.count(); i++ ) {
		uint nbTiles = DataTheme.tiles.at( i )->getDiversificationNumber();
		
		cells[i] = new QList<QPixmap>();
		
		for( uint j = 0; j < nbTiles; j++ ) {
			name.sprintf( "tiles/tile_%d_%d.png", i, j );
			QPixmap pixmap( IMAGE_PATH + name.toLatin1() );
			cells[i]->append( pixmap );
		}
	}

	QList<QPixmap> listPixmapPathCell;
	
	for( int i = 0; i < 33; i++ ) {
		QString name = "misc/arrow-" + QString::number( i ) + ".png";
		listPixmapPathCell.append( QPixmap( IMAGE_PATH + name.toLatin1() ) );
	}
	for( int i = 0; i < 33; i++ ) {
		QString name = "misc/arrowFar-" + QString::number( i ) + ".png";
		listPixmapPathCell.append( QPixmap( IMAGE_PATH + name.toLatin1() ) );
	}
	pathCells = new QList<QPixmap>( listPixmapPathCell);

	return true;
}

bool ImageTheme::initCreatures()
{
	int cpt = 0;
	creatures = new CreaturePixmap * [DataTheme.creatures.count()];
	QString fileName;
	QString raceFilename;
	QString levelFilename;
	Creature * creature;

	uint nbRaces = DataTheme.countRace();
	int nbLevels, nbFrames;
	for( uint i = 0; i < nbRaces; ++i ) {
		nbLevels = DataTheme.creatures.getRace( i )->count();
		raceFilename = IMAGE_PATH + "units/race_" +  QString::number( i ) + "/level_";
		for( int j = 0; j < nbLevels; ++j ) {
			QList<QPixmap> listPixmapFightUnit;
			QList<QString> listFilenames;
			creature = DataTheme.creatures.at( i ,j );
			if ( creature->isAnimated () ) {
				levelFilename = raceFilename + QString::number( j ) + "/creature_" ;
				nbFrames = creature->getNumFrames();
				QString numFrame = "00";
				//load frames' filename
				for( int fr = 1; fr <= nbFrames; ++fr ) {
					numFrame.sprintf( "%02d", fr );
					fileName =  levelFilename + numFrame + ".png";
					listFilenames.append( fileName );
					listPixmapFightUnit.append( QPixmap() );
				}
			} else {
				fileName = raceFilename + QString::number( j ) + "/creature.png";
				listPixmapFightUnit.append( QPixmap() );
				listFilenames.append( fileName );
			}
			creatures[cpt] = new CreaturePixmap( listPixmapFightUnit, listFilenames );
			cpt++;
		}
	}

	QList<QPixmap> listPixmapDead;
	listPixmapDead.append( QPixmap( IMAGE_PATH + "units/dead.png" ) );
	deadCreature =  new QList<QPixmap> ( listPixmapDead );

	nbRaces = DataTheme.bases.count();
	photoCreatures = new QPixmap * [ nbRaces ];
	for( uint i = 0; i < nbRaces; i++ ) {
		photoCreatures[i] = new QPixmap [ MAX_UNIT ];
		for( uint j = 0; j < MAX_UNIT; j++ ){
			photoCreatures[i][j] = QPixmap();
		}
	}

	return true;
}

bool ImageTheme::initLords()
{
	uint nbLord = DataTheme.lords.count();
	uint i;

	QList<QPixmap> listPixmapLord;
	QPixmap lordpix = QPixmap( IMAGE_PATH + "units/lord.png" );
	listPixmapLord.append( lordpix );
	listPixmapLord.append( highlightHighBorder( lordpix ) );
	lords = new QList<QPixmap>( listPixmapLord );

	_lordPixmap = new QPixmap * [ nbLord ];
	for( i = 0; i < nbLord; i++ ) {
		_lordPixmap[i] = NULL;
	}

	_lordSmallPixmap = new QPixmap * [ nbLord ];
	for( i = 0; i < nbLord; i++ ) {
		_lordSmallPixmap[i] = NULL;
	}

	return true;
}

bool ImageTheme::initBases()
{
	uint i;
	uint nbBases = DataTheme.bases.count();
	
	_insideBase = new QPixmap * [DataTheme.bases.count()];
	for( i = 0; i < nbBases; i++ ) {
		_insideBase[i] = NULL;
	}

	QList<QPixmap> listPixmapBase;
	for( i = 0; i < nbBases; i++ ) {
		listPixmapBase.append( QPixmap( IMAGE_PATH + "base/race_" + QString::number( i ) + "/base.png" ) );
	}
	bases = new QList<QPixmap>( listPixmapBase );

	_baseSmallPixmap = new QPixmap * [ nbBases ];
	for( i = 0; i < nbBases; i++ ) {
		_baseSmallPixmap[i] = NULL;
	}

	return true;
}

bool ImageTheme::initArtefacts()
{
	uint i;
	uint nbArtefacts = DataTheme.artefacts.count();

	_artefactIcon = new QPixmap * [ nbArtefacts ];
	for( i = 0; i < nbArtefacts; i++ ) {
		_artefactIcon[i] = NULL;
	}
	 
	QList<QPixmap> listPixmapArtefact;
	for( i = 0; i < nbArtefacts; i++ ) {
		listPixmapArtefact.append( QPixmap( IMAGE_PATH + "artefacts/artefactItem_" + QString::number( i ) + ".png" ) );
	}
	artefacts = new QList<QPixmap>( listPixmapArtefact );

	return true;
}

bool ImageTheme::initResources()
{
	_resourceIcon = new QPixmap * [DataTheme.resources.count()];
	_resourceSmallIcon = new QPixmap * [DataTheme.resources.count()];
	for( int i = 0; i < DataTheme.resources.count(); i++ ) {
		_resourceIcon[i] = NULL;
		_resourceSmallIcon[i] = NULL;
	}

	return true;
}

bool ImageTheme::initBonus()
{
	uint j;
	uint nbResources = DataTheme.resources.count();
	QString name;
	bonus = new QList<QPixmap> * [ 4 ];

	QList<QPixmap> listPixmapBonus0;
	for( j = 0; j < nbResources; j++ ) {
		listPixmapBonus0.append( QPixmap(*getResourceSmallIcon( j ) ) );
	}
	bonus[0] = new QList<QPixmap> ( listPixmapBonus0 );

	QList<QPixmap> listPixmapBonus1;
	name = "events/event1.png";
	listPixmapBonus1.append( QPixmap( IMAGE_PATH + name ) );
	bonus[1] = new QList<QPixmap> ( listPixmapBonus1 );

	QList<QPixmap> listPixmapBonus2;
	name = "events/event1.png";
	listPixmapBonus2.append( QPixmap( IMAGE_PATH + name ) );
	bonus[2] = new QList<QPixmap> ( listPixmapBonus2 );

	QList<QPixmap> listPixmapBonus3;
	name = "events/scroll.png";
	listPixmapBonus3.append( QPixmap( IMAGE_PATH + name ) );
	bonus[3] = new QList<QPixmap> ( listPixmapBonus3 );

	return true;
}

bool ImageTheme::initChest()
{
	QList<QPixmap> listPixmapChest;
	listPixmapChest.append( QPixmap( IMAGE_PATH + "events/chest.png" ) );
	chest = new QList<QPixmap>( listPixmapChest );

	return true;
}

bool ImageTheme::initBuildings()
{
	uint nbBuildings, nbBases;
	buildings = new QList<QPixmap> * [ DataTheme.buildings.count() ];
	for( int i = 0; i < DataTheme.buildings.count(); i++ ) {
		nbBuildings = DataTheme.buildings.at( i )->getNbFrame();
		QList<QPixmap> listPixmapBuilding;
		for( uint j = 0; j < nbBuildings; j++ ) {
			QString name = "building/building_" + QString::number( i ) + "_" + QString::number( j ) + ".png";
			listPixmapBuilding.append( QPixmap( IMAGE_PATH + name ) );
		}
		buildings[i] = new QList<QPixmap>( listPixmapBuilding);
	}

	nbBases = DataTheme.bases.count();
	insideBuildings = new QList<QPixmap>* [ nbBases ];
	for( uint i = 0; i < nbBases; i++ ) {
		insideBuildings[i] = NULL;
	}

	insideBuildingsView = new QList<QPixmap>* [ nbBases ];
	for( uint i = 0; i < nbBases; i++ ) {
		insideBuildingsView[i] = NULL;
	}


	return true;
}

bool ImageTheme::initTransitions()
{
	uint i, j;
	uint nbTransitions = DataTheme.tiles.count();
	
	transition = new QList<QPixmap> * [ nbTransitions ];
	for( i = 1; i <= nbTransitions; i++ ) {
		transition[i-1] = new QList<QPixmap>();
		for( j = 1; j <= NB_TRANSITION; j++ ) {
			QString name = "tiles/transi_" + QString::number( i ) + "_" + QString::number( j ) + ".png";
			QPixmap pixmap( IMAGE_PATH + name.toLatin1() );
			transition[i-1]->append( pixmap );
		}
	}

	return true;
}

bool ImageTheme::initDecorations()
{
	uint i, j;
	uint nbDecoGroups = DataTheme.decorations.count();
	uint nbDeco;
	
	decoration = new QList<QPixmap>* [ nbDecoGroups ];
	for( i = 1; i < nbDecoGroups; i++ ) {
		DecorationGroup * group = DataTheme.decorations.at( i );
		nbDeco = group->count();
		QList<QPixmap> listPixmapDeco;
		
		for( j = 0; j < nbDeco; j++ ) {
			listPixmapDeco.append( QPixmap( IMAGE_PATH + "decor/decoration_" + QString::number( i ) + "_" + QString::number( j ) + ".png" ) );
		}
		
		decoration[ i-1 ] = new QList<QPixmap>( listPixmapDeco);
	}

	return true;
}

bool ImageTheme::initEvents()
{
	uint i;
	events = new QList<QPixmap>* [ 3 ];
	QString fileName;

	QList<QPixmap> listPixmapEvent;
	fileName = "events/event1.png";
	listPixmapEvent.append( QPixmap( IMAGE_PATH + fileName ) );
	events[ 0 ] = new QList<QPixmap>( listPixmapEvent);

	uint nbRes = DataTheme.resources.count();
	QList<QPixmap> listPixmapEvent2;
	for( i = 0; i < nbRes; ++i ) {
		listPixmapEvent2.append( QPixmap( *getResourceSmallIcon( i ) ) );
	}
	events[ 1 ] = new QList<QPixmap>( listPixmapEvent2);

	QList<QPixmap> listPixmapEvent3;
	fileName = "events/chest.png";
	listPixmapEvent3.append( QPixmap( IMAGE_PATH + fileName ) );
	events[ 2 ] = new QList<QPixmap>( listPixmapEvent3);

	return true;
}

bool ImageTheme::initMapCreatures()
{
	mapCreatures = new MapCreaturePixmap ** [ DataTheme.countRace() ];
	for( uint i = 0; i < DataTheme.countRace(); i++ ) {
		mapCreatures[i] = new MapCreaturePixmap * [ DataTheme.creatures.getRace( i )->count() ];
		for( int j = 0; j < DataTheme.creatures.getRace( i )->count(); j++ ) {
			QList<QPixmap> listPixmapMapCreature;
			QString name = "units/race_" + QString::number( i ) + "/level_" + QString::number( j ) + "/creatureMap.png";
			listPixmapMapCreature.append( QPixmap( IMAGE_PATH + name ) );
			mapCreatures[i][j] = new MapCreaturePixmap( listPixmapMapCreature );
		}
	}

	return true;
}

void ImageTheme::loadInsideBuilding( uint race )
{
	QList<QPixmap> listPixmapInside;
	uint nbBuildings;

	nbBuildings = DataTheme.bases.at( race )->getBuildingCount();
	for( uint j = 0; j < nbBuildings; j++ ) {
		listPixmapInside.append( QPixmap( IMAGE_PATH + "base/race_" + QString::number( race ) + "/inside_" + QString::number( j ) + ".png" ) );

	}
	insideBuildings[race] = new QList<QPixmap>( listPixmapInside);
}

void ImageTheme::loadInsideBuildingView( uint race )
{
	QList<QPixmap> listPixmapInside;
	uint nbBuildings;

	nbBuildings = DataTheme.bases.at( race )->getBuildingCount();
	for( uint j = 0; j < nbBuildings; j++ ) {
		listPixmapInside.append( QPixmap( IMAGE_PATH + "base/race_" + QString::number( race ) + "/buildingView_" + QString::number( j ) + ".png" ) );
	}
	insideBuildingsView[race] = new QList<QPixmap>( listPixmapInside);
}

QPixmap ImageTheme::getInsideBuilding( uint race, uint level )
{
  if( !insideBuildings[ race ] ) {
		loadInsideBuilding( race );
	}
	return insideBuildings[ race ]->at( level );
}

QPixmap ImageTheme::getInsideBuildingView( uint race, uint level )
{
  if( !insideBuildingsView[ race ] ) {
		loadInsideBuildingView( race );
	}
	return insideBuildingsView[ race ]->at( level );
}

QPixmap ImageTheme::getMapCreature( uint race, uint level, bool rightLooking )
{
	QList<QPixmap> * list;
	list =  mapCreatures[race][level]->getList( rightLooking?1:0 );
	return list->at(0);

}

QPixmap ImageTheme::getMapCreature( GenericMapCreature * creature, bool rightLooking )
{
	return getMapCreature( creature->getRace(), creature->getLevel(), rightLooking );
}

CreaturePixmap * ImageTheme::getCreature ( uint race, uint level )
{
	int nCnt = 0;
	for( uint i = 0; i < race; i++ ) {
		nCnt += DataTheme.creatures.getRace( i )->count();
	}

	nCnt += level;

	return ( getCreature(nCnt) );
}

CreaturePixmap * ImageTheme::getCreature ( int sequence  )
{
	return ( creatures[sequence] );
}

bool ImageTheme::initSound()
{
	_sound = new AttalSound();
	_sound->init();

	return true;
}

void ImageTheme::enableSound(bool state)
{
	_sound->enableSound( state );
}

void ImageTheme::enableMusic(bool state)
{
	_sound->enableMusic( state );
}

void ImageTheme::playMusicMap()
{
	AttalSoundData soundData;
	soundData.state = MUSIC_MAP;
	_sound->playMusic( soundData );
}

void ImageTheme::endMusic()
{
	_sound->endMusic();
}

void ImageTheme::playMusicBase( uint race )
{ 
	AttalSoundData soundData;
	soundData.race = race;
	soundData.state = MUSIC_BASE;
	_sound->playMusic( soundData );
}

void ImageTheme::playMusicFight()
{
	AttalSoundData soundData;
	soundData.state = MUSIC_FIGHT;
	_sound->playMusic( soundData );
}

void ImageTheme::playSound( AttalSound::SoundType snd )
{
	_sound->playSound( snd );
}

QPixmap ImageTheme::highlightHighBorder( QPixmap & pixmap )
{

	QPixmap pix;
	QImage ima = pixmap.toImage();
	ima.convertToFormat( QImage::Format_ARGB32 );

	int h = ima.height();
	int w = ima.width();
	
	QImage img( w, h, QImage::Format_ARGB32 );

	for ( int x = 0; x < w; x++ ) {
		for ( int y = 0; y < h; y++ ) {
			QRgb pixel = ima.pixel( x, y );
			img.setPixel( x, y, colorH( pixel ) );
		}
	}
	
	pix = QPixmap::fromImage( img );
	return pix;
}

QRgb ImageTheme::colorH( QRgb v )
{
	int ach = qAlpha(v);
	if ( ach == 255 )
		return v;

	int r = ( qRed(v) + 255 ) / 2;
	int g = ( qGreen(v) + 255 ) / 2;
	int b = ( qBlue(v) + 255 ) / 2;
	return qRgba(r,g,b,ach);
	//return qRgba(255,255,255,ach);
}

QPixmap * ImageTheme::getOkPixmap()
{
	if( ! _okPixmap ) {
		_okPixmap = new QPixmap( IMAGE_PATH + "misc/yes.png" );
	}

	return _okPixmap;
}

QPixmap * ImageTheme::getCancelPixmap()
{
	if( ! _cancelPixmap ) {
		_cancelPixmap = new QPixmap( IMAGE_PATH + "misc/no.png" );
	}

	return _cancelPixmap;
}

QPixmap * ImageTheme::getShroudPixmap()
{
	if( !_shroudImage ) {
		_shroudImage = new QPixmap( IMAGE_PATH + "tiles/unknown.png");
	}
	
	return _shroudImage;
}

QPixmap * ImageTheme::getWidgetPixmap(WidgetPixmap type) 
{
	if( !_widgetPixmap ) {
		_widgetPixmap = new QPixmap * [WIDG_LAST];
		for( int i = 0; i < WIDG_LAST; i++ ) {
			_widgetPixmap[i] = NULL;
		}
	}
	QString name = IMAGE_PATH + "misc/";
	QString tmp;
	
	if( _widgetPixmap[ type ] ) {
		return _widgetPixmap[type];
	}
	
	switch( type ) {
		case ICO_ATTACK:
			tmp = "ico_attack.png";
			break;
		case ICO_DEFENSE:
			tmp = "ico_defense.png";
			break;
		case ICO_POWER:
			tmp = "ico_power.png";
			break;
		case ICO_KNOWLEDGE:
			tmp = "ico_knowledge.png";
			break;
		case ICO_MORALE:
			tmp = "ico_morale.png";
			break;
		case ICO_SPECIALITY:
			tmp = "ico_speciality.png";
			break;
		case ICO_CHARISM:
			tmp = "ico_charism.png";
			break;
		case ICO_LUCK:
			tmp = "ico_luck.png";
			break;
		case ICO_MANA:
			tmp = "ico_mana.png";
			break;
		case ICO_MOVE:
			tmp = "ico_move.png";
			break;
		case ICO_EXCHANGE:
			tmp = "exchange.png";
			break;
		case ICO_VISION:
			tmp = "ico_vision.png";
			break;
		case ICO_EXPERIENCE:
			tmp = "ico_experience.png";
			break;
		case ARROW_LEFT:
			tmp = "arrow_left.png";
			break;
		case ARROW_RIGHT:
			tmp = "arrow_right.png";
			break;
		case ARROW_UP:
			tmp = "arrow_up.png";
			break;
		case ARROW_DOWN:
			tmp = "arrow_down.png";
			break;
		case BTN_CONTROL:
			tmp = "btn_control.png";
			break;
		case BTN_SURRENDER:
			tmp = "btn_surrender.png";
			break;
		case BTN_FLEE:
			tmp = "btn_flee.png";
			break;
		case BTN_AUTO:
			tmp = "btn_auto.png";
			break;
		case BTN_SPELL:
			tmp = "btn_spell.png";
			break;
		case BTN_WAIT:
			tmp = "btn_wait.png";
			break;
		case BTN_DEFEND:
			tmp = "btn_defend.png";
		break;
		case EDIT_FREE:
			tmp = "editorFree.png";
		break;
		case EDIT_OCC:
			tmp = "editorOccupied.png";
		break;
		case EDIT_DOOR:
			tmp = "editorDoor.png";
		break;
		case MISC_LOGO:
			tmp = "logo.png";
		break;
		default:
			tmp = "none.png";
		break;
	}

	name += tmp;
	if( !_widgetPixmap[ type ] ) {
		_widgetPixmap[type] = new QPixmap( name );
	}

	return _widgetPixmap[type];
}
