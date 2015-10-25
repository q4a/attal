/****************************************************************
**
** Attal : Lords of Doom
**
** attalSound.h
** Manage sound and music (or not) for attal client
**
** Version : $Id: attalSound.h,v 1.20 2009/11/11 00:19:33 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 22/09/2003
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


#ifndef ATTALSOUND_H
#define ATTALSOUND_H

enum MusicState {
	MUSIC_NULL,
	MUSIC_MAP,
	MUSIC_BASE,
	MUSIC_FIGHT
};

class AttalSoundData {
	
	public:
	AttalSoundData();

	MusicState state;
	int race;
};

#include <QString>

#ifdef WITH_SOUND
// generic include files
// include files for QT
#include <QMap>
#include <QTimer>
// application specific include files


#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

/*              ------------------------------
 *                         AttalSound
 *              ------------------------------ */

class AttalSound : public QObject
{

	Q_OBJECT
public:
		
		enum SoundType {
			SND_NEWTURN,
			SND_HIT,
			SND_ARROW,
			SND_GOOD,
			SND_WIN
		};


	/** Constructor */
	AttalSound();

	/** Destructor */
	virtual ~AttalSound();

	void init();

	void loadMusic(QString filename);
	
	void loadSound(QString filename);

	void enableSound( bool enabled );
	
	void enableMusic( bool enabled );

	void playSound( SoundType snd );

	void playMusic( AttalSoundData data );
	
	void endMusic();
	
	void playNextMusic();

	static void musicCompleted();
	
	// set music volume, -1 return current value
	int volumeMusic ( int volume ) { return Mix_VolumeMusic( volume ); }
	
	// set sound volume, -1 return current value
	int volumeSound (int volume) {  if( _sound ) { return Mix_VolumeChunk( _sound ,volume ); } else { return 0; } } 

	//max sdl_mixer volume
	int maxVolume() { return MIX_MAX_VOLUME; }

protected:
	QString computeSoundFile( SoundType snd );

	QString computeMusicFile( MusicState msc );
	
	Mix_Chunk * _sound;
	int _soundId;
	bool _enableSound , _enableMusic;
	QMap<QString, Mix_Music *>  _musicCache;
	QMap<QString, Mix_Chunk *>  _soundCache;
	AttalSoundData _soundData;

private:
	void clear();

};

#else // No sound!!

class AttalSound
{

public:
	enum SoundType {
		SND_NEWTURN,
		SND_HIT,
		SND_ARROW,
		SND_GOOD,
		SND_WIN
	};
	
	/** Constructor */
	AttalSound() {}

	~AttalSound() {}

	void init() {}

	void loadMusic(QString ) {}
	
	void loadSound(QString ) {}

	void enableSound( bool ) {}

	void enableMusic( bool ) {}

	void playSound( SoundType ) {}

	void playMusic( AttalSoundData ) {}

	void endMusic() {}
	
	// set music volume, -1 return current value
	int volumeMusic ( int /* volume */ ) { return 0; }
	
	// set sound volume, -1 return current value
	int volumeSound (int /* volume */) { return 0; }

	int maxVolume() { return 0; }

private: 
	void clear() {}

};

#endif

#endif // ATTALSOUND_H
