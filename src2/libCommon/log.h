/****************************************************************
**
** Attal : Lords of Doom
**
** log.cpp
** Print log and debugging messages
**
** Version : $Id: log.h,v 1.12 2009/12/27 23:36:50 lusum Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 02/08/2000
**
** Licence :
**      This program is free software; you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation; either version 2, or (at your option)
**      any later version.
**
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
**
*******************************************************************/


#ifndef LOG_H
#define LOG_H


// generic include files
#include <stdio.h>
// Qt include files
#include <QObject>
#include <QString>
// application specific includes

enum LogLevel {
	LogFatal    =0,	/* preventing the program to continue */
	LogError    =1, /* bug or important problem */
	LogInfo     =2, /* information concerning the execution */
	LogWarning  =3, /* something strange is going on */
	LogDebug    =4,  /* debug info */
	LogTrace    =5  /* debug info */
};

enum IALogLevel {
	LOG_VERBOSE,
	LOG_NORMAL,
	LOG_CRITICAL,
	LOG_QUIET
};

extern LogLevel curLogLevel;

/** Class for logging messages */
class Log : public QObject
{
	Q_OBJECT
public:
	/** Constructor */
	Log() {}

	/** Print message in the log */
	void print( QString msg );
	
	void printAi( QString msg );

	/** Set the ai log level to 'level', so that every log below level will not
  * be displayed. Default is LOG_NORMAL.
  */
	void setAiLogLevel( IALogLevel level);
	
	/** Log function for the IA */
#ifdef WIN32VC6
	void ialog( const char *cformat, ...);

#else

/** Log with level */
	void ialog( IALogLevel level, const char *cformat, ...);

#endif // VC6

signals:
	void sig_print( QString msg );
	
	void sig_printAi( QString msg );
};



/** Set file to which all log will be made. If this function is not called
  * before logging, all output will go to stderr
  */
int setLogFile( char * file );

/** Set already opened file to which all log will be made.
  */
int setLogFile( FILE * file );

/** Set the log level to 'level', so that every log below level will not
  * be displayed. Default is LogDebug (everything log is written out).
  */
void setLogLevel( LogLevel level);

#ifdef WIN32VC6 // under windows (not gcc or mingw )
 
//Useful macros, but not under non-gcc compiler... 
//Log with level
void aalogf(const char *fmt, ...);

#define TRACE 
#define logDD aalogf
#define logEE aalogf
#define logWW aalogf
#define logCC aalogf
#define logII aalogf

#else  // not windows

/** Log with level */
void aalogf(LogLevel level, const char *fmt, ...);

/** We could consider this one as an ugly macro, but it can be useful */
#define aalog2(loglevel, format, args... ) if ( __builtin_expect( loglevel <= curLogLevel, 0 ) ) aalogf(loglevel, " %25s (l.%5d): " format, __FUNCTION__, __LINE__ , ## args); 
//more info for trace
#define aalog3(loglevel, format, args... ) if ( __builtin_expect( loglevel <= curLogLevel, 0 ) ) aalogf(loglevel, " %25s (l.%5d): " format, __PRETTY_FUNCTION__, __LINE__ , ## args); 

#define TRACE( format, args... ) aalog3( LogTrace,	format , ## args )
#define logDD( format, args... ) aalog2( LogDebug,	format , ## args )
#define logEE( format, args... ) aalog2( LogError,	format , ## args )
#define logWW( format, args... ) aalog2( LogWarning,	format , ## args )
#define logCC( format, args... ) aalog2( LogCritical,	format , ## args )
#define logII( format, args... ) aalog2( LogInfo,	format , ## args )
#endif
																		 

#endif // LOG_H

#define printSocket logEE("Socket error: cla1 %d, cla2 %d, cla3 %d", getCla1(), getCla2(), getCla3() )

