/****************************************************************
**
** Attal : Lords of Doom
**
** sectionSelector.cpp
** main widget, allows modif in diff theme sections
**
** Version : $Id: sectionSelector.cpp,v 1.7 2003/04/26 14:53:54 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

#include "sectionSelector.h"

// generic include files
// include files for QT
#include <qscrollview.h>
// application specific include files
#include "sectionArtefact.h"
#include "sectionBuilding.h"
#include "sectionBase.h"
#include "sectionCreature.h"
#include "sectionDecoration.h"
#include "sectionGeneral.h"
#include "sectionGround.h"
#include "sectionLord.h"
#include "sectionMachine.h"
#include "sectionRessource.h"
#include "sectionTeam.h"


/** add comments here */
SectionSelector::SectionSelector( QWidget * parent, const char * name )
	: QTabWidget( parent, name )
{
	QScrollView * viewGeneral = new QScrollView( this );
	SectionGeneral * general = new SectionGeneral( viewGeneral->viewport(), "general" );
	viewGeneral->addChild( general );
	addTab( viewGeneral, "General" );

	QScrollView * viewArtefact = new QScrollView( this );
	SectionArtefact * artefact = new SectionArtefact( viewArtefact->viewport(), "artefact" );
	viewArtefact->addChild( artefact );
	addTab( viewArtefact, "Artefacts" );

	QScrollView * viewTeam = new QScrollView( this );
	SectionTeam * team = new SectionTeam( viewTeam, "team" );
	viewTeam->addChild( team );
	addTab( viewTeam, "Teams" );

	QScrollView * viewBase = new QScrollView( this );
	_base = new SectionBase( viewBase->viewport(), "base" );
	viewBase->addChild( _base );
	addTab( viewBase, "Bases" );

	QScrollView * viewBuilding = new QScrollView( this );
	SectionBuilding * building = new SectionBuilding( viewBuilding->viewport(), "building" );
	viewBuilding->addChild( building );
	addTab( viewBuilding, "Buildings" );

	QScrollView * viewCreature = new QScrollView( this );
	SectionCreature * creature = new SectionCreature( viewCreature->viewport(), "creatures" );
	viewCreature->addChild( creature );
	addTab( viewCreature, "Creatures" );

	QScrollView * viewDecoration = new QScrollView( this );
	SectionDecoration * decoration = new SectionDecoration( viewDecoration->viewport(), "decoration" );
	viewDecoration->addChild( decoration );
	addTab( viewDecoration, "Decorations" );

	QScrollView * viewGround = new QScrollView( this );
	SectionGround * ground = new SectionGround( viewGround->viewport(), "ground" );
	viewGround->addChild( ground );
	addTab( viewGround, "Grounds" );

	QScrollView * viewLord = new QScrollView( this );
	_lord = new SectionLord( viewLord->viewport(), "lords" );
	viewLord->addChild( _lord );
	addTab( viewLord, "Lords" );

	QScrollView * viewResource = new QScrollView( this );
	SectionRessource * resource = new SectionRessource( viewResource->viewport(), "resources" );
	viewResource->addChild( resource );
	addTab( viewResource, "Resources" );

	QScrollView * viewMachine = new QScrollView( this );
	SectionMachine * machine = new SectionMachine( viewMachine->viewport(), "machines" );
	viewMachine->addChild( machine );
	addTab( viewMachine, "War Machines" );

	connect( this, SIGNAL( currentChanged( QWidget * ) ), SLOT( slot_changed( QWidget * ) ) );
	connect( creature, SIGNAL( sig_changed() ), SLOT( slot_creaturesChanged() ) );
	connect( general, SIGNAL( sig_changed() ), SLOT( slot_generalChanged() ) );

	_current = general;
}

void SectionSelector::save()
{
	_current->save();
}

void SectionSelector::slot_changed( QWidget * widget )
{
	_current->save();
	_current = (Section *)widget;
}

void SectionSelector::slot_creaturesChanged()
{
	_lord->updateData();
}

void SectionSelector::slot_generalChanged()
{
	_base->updateData();
}
