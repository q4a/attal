/****************************************************************
**
** Attal : Lords of Doom
**
** fightControl.cpp
** Widget for the control board in fight mode
**
** Version : $Id: fightControl.cpp,v 1.11 2009/09/11 17:33:11 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "fightControl.h"
 
// generic include files
// include files for QT
#include <QLayout>
#include <QToolTip>

// application specific include files
#include "libCommon/attalSettings.h"
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/chatWidget.h"
#include "libClient/imageTheme.h"

//
// ----- FightControl -----
//

FightControl::FightControl( QWidget * parent, const char * /* name*/ )
	: QWidget( parent )
{
	AttalButton * butControl = new AttalButton( this );
	butControl->setPixmap( QPixmap ( *ImageTheme.getWidgetPixmap( BTN_CONTROL ) ) );
	butControl->setFixedSize( 52, 52 );
	butControl->setToolTip( tr( "Options" ) );
	
	AttalButton * butSurrender = new AttalButton( this );
	butSurrender->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( BTN_SURRENDER ) )  );
	butSurrender->setFixedSize( 52, 52 );
	butSurrender->setToolTip( tr( "Surrender" ) );
	butSurrender->setEnabled( false );
	
	AttalButton * butFlee = new AttalButton( this );
	butFlee->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( BTN_FLEE ) ) );
	butFlee->setFixedSize( 52, 52 );
	butFlee->setToolTip( tr( "Flee" ) );
	
	AttalButton * butAuto = new AttalButton( this );
	butAuto->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( BTN_AUTO )) );
	butAuto->setFixedSize( 52, 52 );
	butAuto->setToolTip( tr( "Auto-fight" ) );
	butAuto->setEnabled( false );
	
	AttalButton * butWait = new AttalButton( this );
	butWait->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( BTN_WAIT )) );
	butWait->setFixedSize( 52, 52 );
	butWait->setToolTip( tr( "Wait" ) );
	
	AttalButton * butDefend = new AttalButton( this );
	butDefend->setPixmap( QPixmap( *ImageTheme.getWidgetPixmap( BTN_DEFEND )) );
	butDefend->setFixedSize( 52, 52 );
	butDefend->setToolTip( tr( "Defend" ) );
	butDefend->setEnabled( false );

	QHBoxLayout * layH1 = new QHBoxLayout();	
	layH1->addStretch( 1 );
	layH1->addWidget( butWait );
	layH1->addStretch( 1 );
	layH1->addWidget( butDefend );
	layH1->addStretch( 1 );
	layH1->addWidget( butAuto );
	layH1->addStretch( 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();	
	layH2->addStretch( 1 );
	layH2->addWidget( butFlee );
	layH2->addStretch( 1 );
	layH2->addWidget( butSurrender );
	layH2->addStretch( 1 );
	layH2->addWidget( butControl );
	layH2->addStretch( 1 );

	_layV1 = new QVBoxLayout();
	_layV1->setSpacing( 5 );
	_layV1->setMargin( 5 );
	_layV1->addStretch( 1 );
	_layV1->addLayout( layH1 );
	_layV1->addStretch( 1 );
	_layV1->addLayout( layH2 );
	_layV1->addStretch( 1 );

	_chat = new ChatWidget( this );

	_layout = new QHBoxLayout( this );	
	_layout->addLayout( _layV1 );
	_layout->addWidget( _chat, 1 );
	_layout->activate();

	connect( butWait, SIGNAL( pressed( ) ), SLOT( slot_waitPressed( ) ) );
	connect( butFlee, SIGNAL( pressed( ) ), SLOT( slot_fleePressed( ) ) );
	connect( butControl, SIGNAL( clicked( ) ), SLOT( slot_controlPressed( ) ) );
	connect( butDefend, SIGNAL( pressed() ), SIGNAL( sig_defend() ) );
	connect( _chat, SIGNAL( sig_message( QString ) ),this, SIGNAL( sig_message( QString ) ) );
}

void FightControl::updateResolutionMode()
{
	if( AttalSettings::getInstance()->getDispositionMode() == AttalSettings::DM_FULL ) {
		setMinimumHeight( 150 );
		setMaximumHeight( 500 );
		setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
	} else {
		setFixedHeight( 150 );
	}
	update();
}

void FightControl::clear()
{
	_chat->clear();
}

void FightControl::newMessage( const QString & msg )
{
	emit sig_newMessage( msg );
}


