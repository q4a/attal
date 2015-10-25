/****************************************************************
**
** Attal : Lords of Doom
**
** aboutDialog.cpp
** dialog for displaying info about Attal and link to website
**
** Version : $Id: aboutDialog.cpp,v 1.2 2007/02/02 15:24:27 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 28/11/2005
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

#include "aboutDialog.h"
 
// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QPushButton>
// application specific include files
#include "libClient/attalButton.h"

extern QString VERSION;

AboutDialog::AboutDialog( QWidget * parent )
: QDialog( parent )
{
	QLabel * label = new QLabel( this );
	label->setText( tr( "Attal is a turn-based strategy game. \nCan be played alone (against AI) or against other through a network (local or internet). \nIt's available under Linux and Windows." ) );
	QLabel * version = new QLabel( this );
	version->setText( tr( "Version: " ) + VERSION );

	QHBoxLayout * layH1 = new QHBoxLayout();
	QVBoxLayout * layV1 = new QVBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );

	layV1->addWidget( label, 1 );
	layV1->addWidget( version, 1 );
	
	layH1->addLayout( layV1 );

	AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setMargin( 5 );
	layH2->setSpacing( 5 );
	layH2->addStretch( 1 );
	layH2->addWidget( pbOk );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->addStretch( 1 );
	layout->addLayout( layH2 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
}

