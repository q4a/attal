/****************************************************************
**
** Attal : Lords of Doom
**
** interface.cpp
** Manages the whole game
**
** Version : $Id: interfaceDuel.cpp,v 1.5 2006/11/03 18:28:56 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 29/12/2000
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


#include "interfaceDuel.h"
 
// include files for QT
#include <Q3Canvas>
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QPopupMenu>
#include <QMessageBox>
// application specific include
#include "conf.h"

#include "common/log.h"
#include "common/dataTheme.h"

#include "client/attalStyle.h"
#include "client/fight.h"
#include "client/attalStyle.h"
#include "client/imageTheme.h"

extern QString DATA_PATH;
extern QString IMAGE_PATH;


/** add comments here */
InterfaceDuel::InterfaceDuel()
{
	_socket = 0;
	
	qApp->setStyle( new AttalStyle( DATA_PATH + "style.dat" ) ); 
	
	setWindowTitle( "Attal - The Duel" );
	initMenuBar();
	initStatusBar();
	DataTheme.init();
	ImageTheme.init();

	_fight = new Fight( this );
	setCentralWidget( _fight );
}

void InterfaceDuel::initStatusBar()
{
	statusBar()->message( "Status Bar", 0 );
}
	
/*!
  define menus
*/

void InterfaceDuel::initMenuBar()
{
	QPopupMenu * menuFile = new QPopupMenu();
	CHECK_PTR( menuFile );
	menuFile->insertItem( "Connect to Server", FIL_NET );
	menuFile->insertItem( "Send Message", FIL_MSG );
	menuFile->insertItem( "Quit", FIL_QUIT );
	connect( menuFile, SIGNAL( activated( int ) ), this, SLOT( slot_menuFileActivated( int ) ) );	
	
	QPopupMenu * menuGame = new QPopupMenu();
	CHECK_PTR( menuGame );
	menuGame->insertItem( "Show ressources", GAM_INFO );
	menuGame->insertItem( "Show Lord", GAM_LORD );
	menuGame->insertItem( "Next Lord", GAM_NEXT );
	menuGame->insertItem( "End turn", GAM_TURN );
	menuGame->insertItem( "Show powers", GAM_POWER );
	connect( menuGame, SIGNAL( activated( int ) ), this, SLOT( slot_menuGameActivated( int ) ) );	
		
	menuBar()->insertItem( "&File", menuFile );
	menuBar()->insertItem( "&Game", menuGame );
}

void InterfaceDuel::slot_menuFileActivated( int num )
{
	switch( num ) {
	case FIL_NET:
		_socket = new AttalSocket;
		_socket->connectToHost( "localhost", ATTAL_PORT );
		connect( _socket, SIGNAL( readyRead() ), SLOT( slot_readSocket() ) );
		break;
	case FIL_MSG:
		if( _socket ) {
			_socket->sendMessage( "Hello" );
		}
		break;
	case FIL_QUIT:
		qApp->quit();
	}
}

void InterfaceDuel::slot_menuGameActivated( int num )
{
	switch( num ) {
	case GAM_INFO:
		{
		}
		break;
	case GAM_LORD:
		break;
	case GAM_NEXT:
		break;
	case GAM_TURN:
		break;
	case GAM_POWER:
		break;
	}
}

void InterfaceDuel::slot_status( QString text )
{
	statusBar()->message( text, 0 );	
}

void InterfaceDuel::slot_readSocket()
{
	char buf[50];
	
	_socket->readBlock( buf, 50 );	
	
	switch( buf[0] ) {
	case SO_MSG:
		logDD( "MSG : %s", buf+(2*sizeof(char)) );
		break;
	case SO_GAME:
		if( buf[1] == C_GAME_BEG ) {
			logDD( "start new game" );
			/// XXX: clear old stuff if necessary
		} else {
			logDD( "end game" );
		}
		break;
	case SO_TURN:
		if( buf[1] == C_TURN_BEG ) {
			logDD( "start turn" );
			/// XXX: clear old stuff if necessary
		} else {
			logDD( "end turn" );
		}
		break;
		
	case SO_MODIF:
		break;
	case SO_QR:
		logDD( "Q/R" );
		switch( buf[1] ) {
		case C_QR_MSG_END:
			/// to be improved
			QMessageBox::information( this, "Information", QString( buf+(3*sizeof(char)) ), 0 );
			break;
		}
		break;
	case SO_MVT:
	case SO_TECHNIC:
	case SO_EXCH:
	case SO_CONNECT:
	case SO_FIGHT:
	{		
		if( buf[1] == C_FIGHT_INIT ) {
			_fight->reinit();
		} else if( buf[1] == C_FIGHT_END ) {
			
		} else {
			//_fight->socketFight();
		}
		
	}
	
	break;
	default:
		logEE( "Unknown socket_class" );
	}
	
	if( _socket->bytesAvailable() > 0 )
		slot_readSocket();
}
