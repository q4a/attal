/****************************************************************
**
** Attal : Lords of Doom
**
** askChest.cpp
** Dialog for asking the player about the chest content
**
** Version : $Id: askChest.cpp,v 1.6 2008/04/10 21:33:29 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/05/2004
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

#include "askChest.h"
 
// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
// application specific include files
#include "libCommon/log.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

AskChest::AskChest()
: QDialog( 0, Qt::Dialog )
{
	_result = 0;
	
	QLabel * chooseLabel = new QLabel( this );
	chooseLabel->setText( tr( "Choose between earning Gold or Experience" ) );
	FIXEDSIZE( chooseLabel );
	
	QLabel * goldLabel = new QLabel(this);
	goldLabel->setText( QString::number( 1000 ) );
	
	QLabel * experienceLabel = new QLabel(this);
	experienceLabel->setText( QString::number( 2500 ) );
	
	QPushButton * goldButton = new QPushButton( this );
	goldButton->setText( tr( "Gold" ) );
	FIXEDSIZE( goldButton );
	
	QPushButton * experienceButton = new QPushButton( this );
	experienceButton->setText( tr( "Experience" ) );
	FIXEDSIZE( experienceButton );
	
	Icon * goldIcon = new Icon( this );
	goldIcon->setPixmap( QPixmap( *ImageTheme.getResourceIcon( 0 ) ) );
	
	Icon * experienceIcon = new Icon( this );
	experienceIcon->setPixmap( QPixmap(  *ImageTheme.getWidgetPixmap( ICO_EXPERIENCE ) ) );
	
	QGridLayout * layout = new QGridLayout(this);
	layout->addWidget(chooseLabel, 0, 0, 1, 2, Qt::AlignHCenter);
	layout->addWidget(goldIcon, 1, 0, Qt::AlignHCenter);
	layout->addWidget(experienceIcon, 1, 1, Qt::AlignHCenter);
	layout->addWidget(goldLabel, 2, 0, Qt::AlignHCenter);
	layout->addWidget(experienceLabel, 2, 1, Qt::AlignHCenter);
	layout->addWidget(goldButton, 3, 0, Qt::AlignHCenter);
	layout->addWidget(experienceButton, 3, 1, Qt::AlignHCenter);

	connect( goldButton, SIGNAL( clicked() ), SLOT( slot_gold() ) );
	connect( experienceButton, SIGNAL( clicked() ), SLOT( slot_experience() ) );
}

void AskChest::slot_gold()
{
	_result = 0;
	accept();
}

void AskChest::slot_experience()
{
	_result = 1;
	accept();
}


