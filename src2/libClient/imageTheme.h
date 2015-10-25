/****************************************************************
**
** Attal : Lords of Doom
**
** imageTheme.h
** manage images in theme
**
** Version : $Id: imageTheme.h,v 1.45 2014/06/20 16:14:14 lusum Exp $
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

 
#ifndef IMAGETHEME_H
#define IMAGETHEME_H
 

// generic include files
// include files for QT
#include <QList>
#include <QObject>
#include <QPixmap>
// application specific include files
#include "libClient/attalSound.h"

class GenericFightUnit;
class GenericMapCreature;


#define NB_TRANSITION 16

enum WidgetPixmap {
	ICO_ATTACK,
	ICO_DEFENSE,
	ICO_POWER,
	ICO_KNOWLEDGE,
	ICO_MORALE,
	ICO_SPECIALITY,
	ICO_CHARISM,
	ICO_LUCK,
	ICO_MANA,
	ICO_MOVE,
	ICO_EXCHANGE,
	ICO_VISION,
	ICO_EXPERIENCE,
	ARROW_LEFT,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	BTN_CONTROL,
	BTN_SURRENDER,
	BTN_FLEE,
	BTN_AUTO,
	BTN_SPELL,
	BTN_WAIT,
	BTN_DEFEND,
	EDIT_FREE,
	EDIT_OCC,
	EDIT_DOOR,
	MISC_LOGO,
	WIDG_LAST
};

/**              ------------------------------
 *                          ImageTheme
 **              ------------------------------ */

class CreaturePixmap
{
public:
	
	/** Constructor */
	//obsolete
	//CreaturePixmap( const QList<QPixmap> & pix );
	
	//lazy constructor
	CreaturePixmap( const QList<QPixmap> & pix, const QList<QString> & filename );
	
	/** destructor */
	virtual ~CreaturePixmap();
	
	QList<QPixmap> * operator[] ( uint i );
	
private:

	void loadMirrorImage();

	QList<QPixmap> * _pixmaps[2];
	//filenames of creature images
	QList<QString> * _filenames;
	//avoid copy list in lazy initialization
	bool _loaded;
};

class MapCreaturePixmap
{
public:
	
	/** Constructor */
	MapCreaturePixmap( const QList<QPixmap> & pix );
	
	/** Destructor */
	virtual ~MapCreaturePixmap();
	
	QList<QPixmap> * getList( uint i );
	
	QList<QPixmap> * operator[] ( uint i );

private:

	QList<QPixmap> * _pixmaps[2];
};

/** comment for the class */
class ImageTheme : public QObject
{
	Q_OBJECT
public:
	/** Constructor */
	ImageTheme();

	/** Destructor */
	~ImageTheme();

	/** Initializes the images of the theme */
	bool init();

	/** Saves the images of the theme */
	void save();

	/** Clears the images of the theme */
	void clear();

	QPixmap getPhotoCreature( GenericFightUnit * unit );

	QPixmap getPhotoCreature( int race, int level );

	/** \return Returns the pixmap of inside the base 'num' */
	QPixmap * getInsideBase( uint num );

	/** \return Returns the icon of the artefcat 'num' */
	QPixmap * getArtefactIcon( uint num );

	/** \return Returns the flag of the team 'num' */
	QPixmap * getFlag( uint num );

	QPixmap * getResourceIcon( uint num );

	QPixmap * getResourceSmallIcon( uint num );

	QPixmap * getPopulationIcon();
	

	/* pixmap lists used all over the game */
	QList<QPixmap> ** cells;
	QList<QPixmap>* pathCells;
	QList<QPixmap>* lords;
	QList<QPixmap>** buildings;
	QList<QPixmap>* bases;
	//QCanvasPixmapArray ** creatures[2];
	QList<QPixmap> * deadCreature;
	QList<QPixmap>** transition;
	QList<QPixmap>** decoration;
	QList<QPixmap>* artefacts;
	QList<QPixmap> ** bonus;
	QList<QPixmap>* chest;
	MapCreaturePixmap *** mapCreatures;
	QList<QPixmap>** events;

	/** Return pixmap of a lord */
	QPixmap * getLordPixmap( uint num );

	/** Return small pixmap of a lord */
	QPixmap * getLordSmallPixmap( uint num );

	/** Return small pixmap of a base */
	QPixmap * getBaseSmallPixmap( uint num );

	QPixmap getInsideBuilding( uint race, uint level );
	
	QPixmap getInsideBuildingView( uint race, uint level );

	QPixmap getMapCreature( uint race, uint level, bool rightLooking = false );

	QPixmap getMapCreature( GenericMapCreature * creature, bool rightLooking = false );
	
	QList<QPixmap> * getMapFlag( uint type, uint teamId );

	CreaturePixmap * getCreature ( uint race, uint level );
	
	CreaturePixmap * getCreature ( int sequence );
	
	void playMusicMap();

	void playMusicBase( uint race );

	void playMusicFight();
	
	void playSound( AttalSound::SoundType snd );

	void endMusic();

	void enableSound (bool state);

	// set music volume, -1 return current value
	int volumeMusic ( int volume ) { return _sound->volumeMusic( volume ) ; }
	
	// set sound volume, -1 return current value
	int volumeSound (int volume) { return _sound->volumeSound( volume ) ; }

	//max possible volume is a constant
	int maxVolume() { return _sound->maxVolume(); }
	
	void enableMusic (bool state);

	bool isLoaded() {
		return _isLoaded;
	}
	
	static QPixmap highlightHighBorder( QPixmap & pixmap );
	
	static QRgb colorH( QRgb v );

	QPixmap * getOkPixmap();

	QPixmap * getCancelPixmap();

	QPixmap * getShroudPixmap();

	QPixmap * getWidgetPixmap(WidgetPixmap type);

	int getMaxLoadStep() { return 14; }

signals:
	void sig_loadStep();

private:
	bool initTeams();
	bool initCells();
	bool initCreatures();
	bool initLords();
	bool initBases();
	bool initBuildings();
	bool initArtefacts();
	bool initResources();
	bool initBonus();
	bool initChest();
	bool initTransitions();
	bool initDecorations();
	bool initEvents();
	bool initMapCreatures();
	bool initSound();

	void loadInsideBuilding( uint race );
	void loadInsideBuildingView( uint race );

	QPixmap ** _insideBase;
	QPixmap ** _artefactIcon;
	QPixmap ** _resourceIcon;
	QPixmap ** _resourceSmallIcon;
	QPixmap ** _flags;
	QPixmap * _populIcon;

	QPixmap ** _lordPixmap;
	QPixmap ** _lordSmallPixmap;
	QPixmap ** _baseSmallPixmap;

	QPixmap * _okPixmap;
	QPixmap * _cancelPixmap;
	QPixmap * _shroudImage;
	QPixmap ** _widgetPixmap;
	
	CreaturePixmap ** creatures;
	QList<QPixmap>** insideBuildings;
	QList<QPixmap>** insideBuildingsView;
	QPixmap ** photoCreatures;
	
	QList<QPixmap> *** _mapFlags;

	AttalSound * _sound;

	bool _isLoaded;
};

#ifndef NO_EXTERN_IMAGETHEME
extern ImageTheme ImageTheme;
#else
#undef NO_EXTERN_IMAGETHEME
#endif

#endif // IMAGETHEME_H
