/****************************************************************
**
** Attal : Lords of Doom
**
** log.cpp
** Print log and debugging messages
**
** Version : $Id: log.cpp,v 1.3 2002/10/06 16:42:06 audoux Exp $
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
******************************************************************/


// generic include files
#include <stdarg.h>
#include <stdio.h>

// application specific includes
#include "log.h"

//
// ----- Log -----
//

void Log::print( QString msg )
{
	qDebug( msg );
	emit sig_print( msg );
}

//
// ----- functions -----
//

static char *logLevelName[] = {
	"FATAL   ",
	"   ERROR",
	"INFO    ",
	"WARNING ",
	"DEBUG   "
};

#define LOGMIN LogFatal
#define LOGMAX LogDebug

static FILE * logFile = stderr;
static LogLevel curLogLevel = LogDebug;


/** Set file to which all log will be made. If this function is not called
  * before logging, all output will go to stderr
  */
int setLogFile( char * file )
{
	if (logFile != 0 && logFile != stderr) {
		fclose(logFile);
	}

	logFile = fopen( file, "w+" );
	if (! logFile) {
		fprintf( stderr, "Could not open log file %s\n", file );
		return -1;
	}
	
	return 0;
}
	
int setLogFile( FILE * file )
{
	if (logFile != 0 && logFile != stderr) {
		fclose(logFile);
	}
	
	if (! file) {
		fprintf( stderr, "Setting to log output to NULL file.");
	}

	logFile = file;
	
	return 0;
}
	

/** Set the log level to 'level', so that every log below level will not
  * be displayed
  */
void setLogLevel( LogLevel level)
{
	if (level > LOGMAX)
		level = LOGMAX;
	if (level < LOGMIN)
		level = LOGMIN;
	
	curLogLevel = level;
	logDD( "Log level set to %s", logLevelName[level]);
}

#ifdef WIN32

void logf( char *fmt, ...)
{
	LogLevel level = curLogLevel;

	va_list args;

	if ( level > curLogLevel )
		return;

	fprintf(logFile, "\r\n%s: ", logLevelName[level] );

	va_start(args, fmt);
	vfprintf(logFile, fmt, args);
	va_end(args);
	fflush(logFile);
}

#else

/** Log with level */
void logf( LogLevel level, char *fmt, ...)
{
	va_list args;

	if ( level > curLogLevel )
		return;
	fprintf(logFile, "%s: ", logLevelName[level] );

	va_start(args, fmt);
	vfprintf(logFile, fmt, args);
	va_end(args);
	fputc('\n', logFile);
	fflush(logFile);
}

#endif
