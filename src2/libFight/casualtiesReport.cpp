/****************************************************************
**
** Attal : Lords of Doom
**
** casualtiesReport.cpp
** Report widget about casualties of a lord
**
** Version : $Id: casualtiesReport.cpp,v 1.14 2009/09/11 17:33:11 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 03/12/2005
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

#include "casualtiesReport.h"
 
// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>

// application specific include files
#include "libCommon/creature.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"
#include "libCommon/unit.h"

#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/widget.h"

#include "libFight/fight.h"

CasualtiesReport::CasualtiesReport( CLASS_FIGHTER fighter, Fight * fight, QWidget * parent, const char * /*name*/ )
: QFrame( parent )
{
	GenericLord * lord = fight->getFighterLord( fighter );

	_fight = fight;
	_fighter = fighter;
	_list = new QListWidget( this );
	_list->setSelectionMode( QAbstractItemView::NoSelection );
	_photo = new Icon( this );

	if( lord ) {
		displayCasualtiesLord( lord );
	} else {
		uint race = fight->getCreatureRace();
		uint level = fight->getCreatureLevel();
		Creature * creature = DataTheme.creatures.at( race, level );
		displayCasualtiesCreature( creature );
	}

	QLabel * reportLabel = new QLabel( this );
	reportLabel->setText( _reportTitle );
	FIXEDSIZE( reportLabel );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setSpacing( 5 );
	layH1->setMargin( 5 );
	layH1->addWidget( _photo, 0, Qt::AlignTop );
	layH1->addWidget( _list, 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( reportLabel );
	layout->addLayout( layH1, 1 );
	layout->activate();
}

void CasualtiesReport::displayCasualtiesLord( GenericLord * lord )
{
	GenericFightUnit * unit = 0;

	_list->setIconSize( QSize ( 30, 60 ) );

	for( uint i = 0; i < _fight->getCasualtiesNumber( _fighter ); i++ ) {
		unit = _fight->getCasualtiesUnit( _fighter, i );
		if( unit ) {
			QString itemText;
			itemText.sprintf( "%ld %s killed", unit->getNumber(), qPrintable( unit->getCreature()->getName() ) );
			QListWidgetItem * item = new QListWidgetItem( itemText, _list );
			item->setIcon( ImageTheme.getMapCreature(  unit->getRace() , unit->getLevel() ) );
		}
	}
	
	QPixmap * photoPixmap = ImageTheme.getLordPixmap( lord->getId() );
	if( photoPixmap ) {
		_photo->setPixmap( * photoPixmap );
	}

	if( ( _fight->isAttack() && ( _fighter == FIGHTER_ATTACK ) ) 
		|| ( ( ! _fight->isAttack() ) && ( _fighter == FIGHTER_DEFENSE ) ) ) {
		_reportTitle = QString(tr( "Report - Your lord  " ) + lord->getName() );
	} else {
		_reportTitle = QString(tr( "Report - Opponent lord " ) + lord->getName() );
	}
}

void CasualtiesReport::displayCasualtiesCreature( Creature * creature )
{
	GenericFightUnit * unit = 0;	
	QString imagePath;
	
	_photo->setPixmap( ImageTheme.getPhotoCreature( creature->getRace(), creature->getLevel() ) );
	_reportTitle = QString(tr( "Report - Opponent creatures ") + creature->getName() );

	unit = _fight->getCasualtiesUnit( _fighter, 0 );
	if( unit ) {
		QString itemText = QString(tr( "%1 %2 killed" ).arg(unit->getNumber()).arg( qPrintable ( unit->getCreature()->getName() )) );
		new QListWidgetItem( itemText, _list );
	} else {
		new QListWidgetItem( tr( "No creatures killed" ), _list );
	}
}

