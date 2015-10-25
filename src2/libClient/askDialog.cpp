/****************************************************************
**
** Attal : Lords of Doom
**
** askDialog.cpp
** Dialog for questions to player
**
** Version : $Id: askDialog.cpp,v 1.4 2006/03/10 19:09:40 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/11/2004
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

#include "askDialog.h"
 
// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"


AskDialog::AskDialog()
: QDialog( 0, Qt::Dialog )
{
	_pb1 = new AttalButton( this, AttalButton::BT_OK );
	
	_pb2 = new AttalButton( this, AttalButton::BT_CANCEL );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( _pb1 );
	layH1->addStretch( 1 );
	layH1->addWidget( _pb2 );
	layH1->addStretch( 1 );
	
	_text = new QLabel( this );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _text, 1 );
	layout->addLayout( layH1 );
	layout->activate();
	
	connect( _pb1, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( _pb2, SIGNAL( clicked() ), SLOT( reject() ) );
}

void AskDialog::setText( const QString & text )
{
	_text->setText( text );
}

void AskDialog::setYesNo()
{
	_pb1->setText( tr( "Yes" ) );
	FIXEDSIZE( _pb1 );
	
	_pb2->setText( tr( "No" ) );
	FIXEDSIZE( _pb2 );
}
