/****************************************************************
**
** Attal : Lords of Doom
**
** log.cpp
** Print log and debugging messages
**
** Version : $Id: log.cpp,v 1.10 2008/06/09 11:27:49 lusum Exp $
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


#include <QRegExp>

// generic include files
#include <stdarg.h>
#include <stdio.h>

// application specific includes
#include "log.h"

#ifdef QT_DEBUG
LogLevel curLogLevel = LogDebug;
#else
LogLevel curLogLevel = LogWarning;
#endif

//
// ----- Log -----
//

void Log::print( QString msg )
{
	qDebug( "%s", qPrintable( msg ));
	emit sig_print( msg );
}

void Log::printAi( QString msg )
{
	emit sig_printAi( msg );
}

//
// ----- functions -----
//

static const char *logLevelName[] = {
	"FATAL   ",
	"   ERROR",
	"INFO    ",
	"WARNING ",
	"DEBUG   ",
	"TRACE   "
};

static const char *AIlogLevelName[] = {
	"VERBOSE   ",
	"   NORMAL",
	"CRITICAL    ",
	"QUIET "
};



#define LOGMIN LogFatal
#define LOGMAX LogTrace

#define LOGAIMIN LOG_VERBOSE
#define LOGAIMAX LOG_QUIET

static FILE * logFile = stderr;
static IALogLevel curAILogLevel = LOG_NORMAL;


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
	logII( "Log level set to %s", logLevelName[level]);
}

#ifdef WIN32VC6

void aalogf( const char *fmt, ...)
{
	LogLevel level = curLogLevel;

	va_list args;
	
	//if ( level > curLogLevel )
	//	return;

	fprintf(logFile, "\r\n%s: ", logLevelName[level] );

	va_start(args, fmt);
	vfprintf(logFile, fmt, args);
	va_end(args);
	fflush(logFile);
}

#else

/** Log with level */
void aalogf( LogLevel level, const char *fmt, ...)
{
	va_list args;

	//if ( level > curLogLevel )
	//	return;
	
	fprintf(logFile, "%s: ", logLevelName[level] );

	va_start(args, fmt);
	vfprintf(logFile, fmt, args);
	va_end(args);
	fputc('\n', logFile);
	fflush(logFile);
}

#endif

#ifdef WIN32VC6

void Log::ialog( const char *cformat, ...)
{
#else

/** Log with level */
void Log::ialog( IALogLevel level, const char *cformat, ...)
{
	if ( level < curAILogLevel )
		return;

#endif

	QString result = "";

	// This code has been taken from qt 2.3.2 sources (QString::sprintf) (which is under GPL)
	va_list ap;
	va_start( ap, cformat );

	if ( !cformat || !*cformat ) {
		return;
    	}
	QString format = QString::fromLatin1( cformat );

	static QRegExp *escape = 0;
	if( !escape ) {
		escape = new QRegExp( "%#?0?-? ?\\+?'?[0-9*]*\\.?[0-9*]*h?l?L?q?Z?" );
	}

	uint last = 0;

	int len = 0;
	int pos;
	for (;;) {
		
		/// was pos = escape->match( format, last, &len );
		pos = escape->indexIn( format, last );
		len = escape->matchedLength();
		// Non-escaped text
		if ( pos > (int)last ) {
			result += format.mid(last,pos-last);
		}
		if ( pos < 0 ) {
			// The rest
			if ( (int)last < format.length() ) {
				result += format.mid(last);
			}
			break;
		}
		last = pos + len + 1;

		// Escape
		QString f = format.mid( pos, len );
		uint width, decimals;
		int params = 0;
		int wpos = f.indexOf('*');
		if ( wpos >= 0 ) {
			params++;
			width = va_arg( ap, int );
			if ( f.indexOf('*', wpos + 1) >= 0 ) {
				decimals = va_arg( ap, int );
				params++;
			} else {
				decimals = 0;
			}
		} else {
			decimals = width = 0;
		}
		QString replacement;
		if ( format[pos+len] == 's' ||
			format[pos+len] == 'S' ||
			format[pos+len] == 'c' ) {
			bool rightjust = ( f.indexOf('-') < 0 );
			// Yes, %-5s really means left adjust in sprintf

			if ( wpos < 0 ) {
				QRegExp num( QString::fromLatin1("[0-9]+") );
				QRegExp dot( QString::fromLatin1("\\.") );
				/// was int nlen;
				/// was int p = num.match( f, 0, &nlen );
				int p = num.indexIn( f, 0 );
				int nlen = num.matchedLength();
				/// was int q = dot.match( f, 0 );
				int q = dot.indexIn( f, 0 );
				
				if ( q < 0 || (p < q && p >= 0) ) {
					width = f.mid( p, num.matchedLength() ).toInt();
				}
				if ( q >= 0 ) {
					/// was p = num.match( f, q );
					p = num.indexIn( f, q );
					// "decimals" is used to specify string truncation
					if ( p >= 0 ) {
						decimals = f.mid( p, nlen ).toInt();
					}
				}
			}

			if ( format[pos+len] == 's' ) {
				QString s = QString::fromLatin1(va_arg(ap, char*));
				if( decimals <= 0 ) {
					replacement = s;
				} else {
					replacement = s.left(decimals);
				}
			} else {
				int ch = va_arg(ap, int);
				replacement = QChar((ushort)ch);
			}
			if( replacement.length() < (int)width ) {
				replacement = rightjust
				? replacement.rightJustified(width)
				: replacement.leftJustified(width);
			}
		} else if ( format[pos+len] == '%' ) {
			replacement = '%';
		} else if ( format[pos+len] == 'n' ) {
			int* n = va_arg(ap, int*);
			*n = result.length();
		} else {
			char in[64], out[330] = "";
			strncpy(in,f.toLatin1(),63);
			char fch = format[pos+len].toLatin1();
			in[f.length()] = fch;
			switch ( fch ) {
			case 'd':
			case 'i':
			case 'o':
			case 'u':
			case 'x':
			case 'X': {
				int value = va_arg(ap, int);
				switch (params) {
				case 0: ::sprintf( out, in, value ); break;
				case 1: ::sprintf( out, in, width, value ); break;
				case 2: ::sprintf( out, in, width, decimals, value ); break;
				}
			} break;
			case 'e': case 'E': case 'f': case 'g': case 'G': {
			double value = va_arg(ap, double);
			switch (params) {
				case 0: ::sprintf( out, in, value ); break;
				case 1: ::sprintf( out, in, width, value ); break;
				case 2: ::sprintf( out, in, width, decimals, value ); break;
				}
			} break;
			case 'p': {
			void* value = va_arg(ap, void*);
				switch (params) {
					case 0: ::sprintf( out, in, value ); break;
					case 1: ::sprintf( out, in, width, value ); break;
					case 2: ::sprintf( out, in, width, decimals, value ); break;
					}
				} break;
			}
		replacement = QString::fromLatin1(out);
		}
		result += replacement;
	}

	va_end( ap );
	
	printAi( result );
}

/** Set the log level for AI to 'level', so that every log below level will not
  * be displayed
  */
void Log::setAiLogLevel( IALogLevel level)
{
	if (level > LOGAIMAX)
		level = LOGAIMAX;
	if (level < LOGAIMIN)
		level = LOGAIMIN;
	
	curAILogLevel = level;
	logDD( "Log level set to %s", AIlogLevelName[level]);
}


