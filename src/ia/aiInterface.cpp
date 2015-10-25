/****************************************************************
**
** Attal : Lords of Doom
**
** aiInterface.cpp
** interface for the ia-client
**
** Version : $Id: aiInterface.cpp,v 1.4 2003/04/05 21:27:12 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/02/2001
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


#include "aiInterface.h"

// generic include files
#include <stdarg.h>
// include files for QT
#include <qapplication.h>
#include <qmultilinedit.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>
#include <qregexp.h>
// application specific includes
#include "conf.h"

#include "common/log.h"
#include "common/attalSocket.h"
#include "common/dataTheme.h"

#include "server/fightAnalyst.h"

#include "ia/analyst.h"

extern QString DATA_PATH;
extern DataTheme DataTheme;
extern Log aifLog;

QMultiLineEdit * screen;
IALogLevel iaLogLevel;


/** add comments here */
AiInterface::AiInterface()
{
	_socket = 0;
	iaLogLevel = LOG_VERBOSE;
	DataTheme.init();

	_analyst = new Analyst();
	
	setCaption( "Attal: IA client" );
	initMenuBar();
	initStatusBar();
	
	_edit = new QMultiLineEdit( this );
	_edit->setReadOnly( true );
	screen = _edit;
	setCentralWidget( _edit );

	connect( &aifLog, SIGNAL( sig_print( QString ) ), SLOT( slot_log( QString ) ) );
}

void AiInterface::slot_log( QString msg )
{
	screen->append( msg );
}

void AiInterface::initStatusBar()
{
	statusBar()->message( "Log level : Verbose", 0 );
}
	
/*!
  define menus
*/

void AiInterface::initMenuBar()
{
	_menuLog = new QPopupMenu();
	CHECK_PTR( _menuLog );
	_menuLog->insertItem( "Verbose", LOG_VERBOSE );
	_menuLog->insertItem( "Normal", LOG_NORMAL );
	_menuLog->insertItem( "Critical", LOG_CRITICAL );
	_menuLog->insertItem( "Quiet", LOG_QUIET );
	_menuLog->setItemChecked( LOG_VERBOSE, true );
	connect( _menuLog, SIGNAL( activated( int ) ), SLOT( slot_menuLogActivated( int ) ) );
	
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "Connect", FIL_CON );
	menuFile->insertItem( "Disconnect", FIL_DIS );
	menuFile->insertItem( "Log Level", _menuLog );
	menuFile->insertItem( "Quit", FIL_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), SLOT( slot_menuFileActivated( int ) ) );	
	
	menuBar()->insertItem( "&File", menuFile );
}

void AiInterface::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FIL_CON:
		if(!_socket ) {
			_socket = new AttalSocket;
			_socket->connectToHost( "localhost", ATTAL_PORT );
			_analyst->setSocket( _socket );
			connect( _socket, SIGNAL( readyRead() ), SLOT( slot_readSocket() ) );
		} else {
			QMessageBox::critical( this, "Can't connect", "You're already connected to a server. Please disconnect first." );
		}
		break;
	case FIL_DIS:
		if( _socket ) {
			_analyst->setSocket( 0 );
			_socket->close();
			delete _socket;
			_socket = 0;
		}
		break;
	case FIL_QUIT:
		if( _socket ) {
			_analyst->setSocket( 0 );
			_socket->close();
			delete _socket;
			_socket = 0;
		}
		qApp->quit();
		break;
	}
}

void AiInterface::slot_menuLogActivated( int num )
{
	_menuLog->setItemChecked( iaLogLevel, false );
	switch( num ) {
	case LOG_VERBOSE:
		statusBar()->message( "Log level : Verbose", 0 );
		iaLogLevel = LOG_VERBOSE;
		break;
	case LOG_NORMAL:
		statusBar()->message( "Log level : Normal", 0 );
		iaLogLevel = LOG_NORMAL;
		break;
	case LOG_CRITICAL:
		statusBar()->message( "Log level : Critical", 0 );
		iaLogLevel = LOG_CRITICAL;
		break;
	case LOG_QUIET:
		statusBar()->message( "Log level : Quiet", 0 );
		iaLogLevel = LOG_QUIET;
		break;
	}
	_menuLog->setItemChecked( iaLogLevel, true );
}

void AiInterface::slot_readSocket()
{
	_socket->readData();
	ialogV( "Socket received %d | %d | %d", _socket->getCla1(), _socket->getCla2(), _socket->getCla3() );

	switch( _socket->getCla1() ) {
	case SO_MSG:
		_analyst->socketMsg();
		break;
	case SO_CONNECT:
		_analyst->socketConnect();
		break;
	case SO_MVT:
		_analyst->socketMvt();
		break;
	case SO_TECHNIC:
		_analyst->socketTechnic();
		break;
	case SO_FIGHT:
		_analyst->socketFight();
		break;
	case SO_QR:
		_analyst->socketQR();
		break;
	case SO_EXCH:
		_analyst->socketExch();
		break;
	case SO_MODIF:
		_analyst->socketModif();
		break;
	case SO_TURN:
		_analyst->socketTurn();
		break;
	case SO_GAME:
		_analyst->socketGame();
		break;
	default:
		ialogC( "Socket class unknown" );
		break;
	}


	if( _socket->bytesAvailable() > 0 )
		slot_readSocket();
}

#ifdef WIN32
void ialog( char *fmt, char * cformat, ... )
{
#else
void ialog( IALogLevel level, char * cformat, ... )
{
	if( ( !screen ) || ( level < iaLogLevel ) )
		return;
#endif
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

	QString result;
	uint last = 0;

	int len = 0;
	int pos;
	for (;;) {
		pos = escape->match( format, last, &len );
		// Non-escaped text
		if ( pos > (int)last ) {
			result += format.mid(last,pos-last);
		}
		if ( pos < 0 ) {
			// The rest
			if ( last < format.length() ) {
				result += format.mid(last);
			}
			break;
		}
		last = pos + len + 1;

		// Escape
		QString f = format.mid( pos, len );
		uint width, decimals;
		int params = 0;
		int wpos = f.find('*');
		if ( wpos >= 0 ) {
			params++;
			width = va_arg( ap, int );
			if ( f.find('*', wpos + 1) >= 0 ) {
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
			bool rightjust = ( f.find('-') < 0 );
			// Yes, %-5s really means left adjust in sprintf

			if ( wpos < 0 ) {
				QRegExp num( QString::fromLatin1("[0-9]+") );
				QRegExp dot( QString::fromLatin1("\\.") );
				int nlen;
				int p = num.match( f, 0, &nlen );
				int q = dot.match( f, 0 );
				if ( q < 0 || (p < q && p >= 0) ) {
					width = f.mid( p, nlen ).toInt();
				}
				if ( q >= 0 ) {
					p = num.match( f, q );
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
			if( replacement.length() < width ) {
				replacement = rightjust
				? replacement.rightJustify(width)
				: replacement.leftJustify(width);
			}
		} else if ( format[pos+len] == '%' ) {
			replacement = '%';
		} else if ( format[pos+len] == 'n' ) {
			int* n = va_arg(ap, int*);
			*n = result.length();
		} else {
			char in[64], out[330] = "";
			strncpy(in,f.latin1(),63);
			char fch = format[pos+len].latin1();
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

    	screen->append( result );
	screen->setCursorPosition( screen->numLines(), 0 );
}
