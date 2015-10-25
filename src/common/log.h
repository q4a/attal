/****************************************************************
**
** Attal : Lords of Doom
**
** log.cpp
** Print log and debugging messages
**
** Version : $Id: log.h,v 1.4 2002/10/20 16:04:42 audoux Exp $
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
#include <qobject.h>
#include <qstring.h>
// application specific includes


enum LogLevel {
	LogFatal    =0,	/* preventing the program to continue */
	LogError    =1, /* bug or important problem */
	LogInfo     =2, /* information concerning the execution */
	LogWarning  =3, /* something strange is going on */
	LogDebug    =4  /* debug info */
};

/** Class for logging messages */
class Log : public QObject
{
	Q_OBJECT
public:
	/** Constructor */
	Log() {}

	/** Print message in the log */
	void print( QString msg );

signals:
	void sig_print( QString msg );
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

#ifdef WIN32 // under windows

/** Useful macros, but not under non-gcc compiler...  */
/** Log with level */
void logf(char *fmt, ...);

#define logDD logf
#define logEE logf

#else  // not windows

/** Log with level */
void logf(LogLevel level, char *fmt, ...);

/** We could consider this one as an ugly macro, but it can be useful */
#define log2(loglevel, format, args... ) \
	logf(loglevel, " %25s (l.%5d): " format, __FUNCTION__, __LINE__ , ## args )

#define logDD( format, args... ) log2( LogDebug,	format , ## args )
#define logEE( format, args... ) log2( LogError,	format , ## args )
#endif

#endif // LOG_H
