/****************************************************************
**
** Attal : Lords of Doom
**
** attalSound.cpp
** Manage sound and music (or not) for attal client
**
** Version : $Id: attalSound.cpp,v 1.33 2009/09/11 16:55:03 lusum Exp $
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

#include "attalSound.h"

AttalSoundData::AttalSoundData()
{
	state = MUSIC_NULL;
	race = 0;
}


#ifdef WITH_SOUND

// generic include files
#include <stdlib.h>
// include files for QT
#include <QString>
// application specific include files
#include <libCommon/log.h>

extern QString MUSIC_PATH;

extern QString SOUND_PATH;

#ifdef WITH_SOUND
extern bool ATT_SOUND;
#endif

//this is to mix C and C++ code
AttalSound * _sndPointer = NULL;

AttalSound::AttalSound()
{
	_sndPointer = this;
	_enableMusic = true;
	_enableSound = true;
	_soundId = -1;
	_sound = 0;
}

AttalSound::~AttalSound()
{
	_sndPointer = NULL;
	clear();
}

void AttalSound::enableSound( bool enabled )
{
	_enableSound = enabled;
}

void AttalSound::enableMusic( bool enabled )
{
	_enableMusic = enabled;

	if( _enableMusic ) {
		playMusic( _soundData ); 
	} else {
		endMusic();
	}
}

void AttalSound::init()
{
	if(ATT_SOUND){
		if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
			fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
			clear();
		}
		//atexit(SDL_Quit);
#ifdef WIN32
		const size_t buf_size = 4096;
#else
		const size_t buf_size = 1024;
#endif

		if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, buf_size) < 0 ) {
			fprintf(stderr,	"Warning: Couldn't set 44100 Hz 8-bit audio\n- Reason: %s\n", SDL_GetError() );
			clear();
		}
		Mix_AllocateChannels(16);
	
	}
}

void AttalSound::loadMusic(QString filename)
{
	Mix_Music * music;		

	music = Mix_LoadMUS( qPrintable(QString(MUSIC_PATH + filename)) );
	if( music == NULL ) {
		fprintf(stderr, "Warning: Couldn't load music: %s\n", Mix_GetError() );
		return;
	}
	_musicCache.insert(filename , music );
}

void AttalSound::loadSound(QString filename)
{
	Mix_Chunk * sound;		

	sound = Mix_LoadWAV( qPrintable(QString(SOUND_PATH + filename)));
	if( sound == NULL ) {
		fprintf(stderr, "Warning: Couldn't load sound: %s\n", Mix_GetError() );
		return;
	}
	_soundCache.insert(filename , sound );
}


void AttalSound::clear()
{
	if(ATT_SOUND){
		endMusic();
		
		Mix_HaltChannel( -1 );
		
		QMap<QString,Mix_Chunk*>::iterator i;

		for( i = _soundCache.begin(); i != _soundCache.end(); ++i)
			Mix_FreeChunk( i.value() );
		_soundCache.clear();
			   
		QMap<QString,Mix_Music*>::iterator j;

		for( j = _musicCache.begin(); j != _musicCache.end(); ++j)
			Mix_FreeMusic(j.value());
		_musicCache.clear();

		if(SDL_WasInit(SDL_INIT_AUDIO) != 0) {

			Mix_CloseAudio();

			SDL_QuitSubSystem(SDL_INIT_AUDIO);
		}
		  
		SDL_Quit();
	}
}

void AttalSound::endMusic()
{
	if(ATT_SOUND){
		if( Mix_PlayingMusic() ) {
			Mix_HookMusicFinished(0);
			Mix_HaltMusic();
		} 
	}
}

void AttalSound::playMusic( AttalSoundData soundData)
{
	if(ATT_SOUND){
		_soundData.state = soundData.state;

		if( _enableMusic ) {
			Mix_Music * music;
			if( Mix_PlayingMusic() ) {
				Mix_HookMusicFinished(AttalSound::musicCompleted);
				Mix_FadeOutMusic( 500 );
			} else {
				QString file = computeMusicFile( _soundData.state );

				if( file != "" ) {
					if( !_musicCache.contains( file ) ) {
						loadMusic( file );	
					}	
				}

				music = _musicCache.value(file);
				if( music ) {
					Mix_FadeInMusic( music, -1, 500 );
				}

			}
		}
	}
}

QString AttalSound::computeSoundFile( SoundType snd )
{
	QString ret = "";
	
	switch( snd ) {
	case SND_NEWTURN:
		ret = "newturn.wav";
		break;
	case SND_HIT:
		ret = "hit.wav";
		break;
	case SND_ARROW:
		ret = "arrow.wav";
		break;
	case SND_GOOD:
		ret = "good.wav";
		break;
	case SND_WIN:
		ret = "win.wav";
		break;
	}
	
	return ret;
}

QString AttalSound::computeMusicFile( MusicState msc )
{
	switch( msc ) {
	case MUSIC_MAP:
		return "map.ogg";
		break;
	case MUSIC_BASE:
		return "castle.ogg";
		break;
	case MUSIC_FIGHT:
		return "battle01.ogg";
		break;
	default: // in case there are any new "MusicState"s, and this code hasn't been changed, it will default to not playing anything
	case MUSIC_NULL:
		return "";
		break;
	}
}

void AttalSound::playSound( SoundType snd )
{
	if(ATT_SOUND && _enableSound ) {
		Mix_Chunk * sound;
		QString file = computeSoundFile( snd );
		
		Mix_HaltChannel( -1 );

		if( file != "" ) {
			if( !_soundCache.contains( file ) ) {
				loadSound( file );	
			}
			sound = _soundCache.value( file );
			if( sound ) {
			  _sound = sound;
				_soundId = Mix_PlayChannel( -1, sound, 0 ); // I don't know why _soundId is even being stored, it isn't used anywhere else
			}
		}
	}
}
		
void AttalSound::playNextMusic()
{
	playMusic( _soundData ); 
}

void AttalSound::musicCompleted()
{
	if(! Mix_PlayingMusic() ) {
		if( _sndPointer ) {
		 _sndPointer->playNextMusic(); 
		}
	}
}

#endif

