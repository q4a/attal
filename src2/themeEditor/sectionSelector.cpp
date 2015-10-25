/****************************************************************
**
** Attal : Lords of Doom
**
** sectionSelector.cpp
** main widget, allows modif in diff theme sections
**
** Version : $Id: sectionSelector.cpp,v 1.11 2007/09/09 16:33:41 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
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
#include <QScrollArea>

// application specific include files
#include "sectionArtefact.h"
#include "sectionBuilding.h"
#include "sectionBase.h"
#include "sectionCreature.h"
#include "sectionDecoration.h"
#include "sectionExperience.h"
#include "sectionGeneral.h"
#include "sectionGround.h"
#include "sectionLord.h"
#include "sectionLordCategory.h"
#include "sectionMachine.h"
#include "sectionRessource.h"
#include "sectionTeam.h"


class AttalScrollView : public QScrollArea
{
public:
	/** Constructor */
	AttalScrollView( QWidget * parent = 0 );

	void setSection( Section * section );

	Section * getSection() {
		return _section;
	}

protected:
	Section * _section;
};

AttalScrollView::AttalScrollView( QWidget * parent )
: QScrollArea( parent )
{
	_section = 0;
	setWidgetResizable(true);
}

void AttalScrollView::setSection( Section * section )
{
	_section = section;
	//addChild( section );
	setWidget( section );
}

//
// ----- SectionSelector
//

SectionSelector::SectionSelector( QWidget * parent, const char * name )
	: QTabWidget( parent )
{
	setWindowTitle( QString ( name ));
	AttalScrollView * viewGeneral = new AttalScrollView( 0 );
	_general = new SectionGeneral( 0, "general" );
	viewGeneral->setSection( _general );
	addTab( viewGeneral, tr( "General" ) );

	AttalScrollView * viewArtefact = new AttalScrollView( 0 );
	_artefact = new SectionArtefact( 0, "artefact" );
	viewArtefact->setSection( _artefact );
	addTab( viewArtefact, tr( "Artefacts" ) );

	AttalScrollView * viewTeam = new AttalScrollView( 0 );
	_team = new SectionTeam( 0, "team" );
	viewTeam->setSection( _team );
	addTab( viewTeam, tr( "Teams" ) );

	AttalScrollView * viewBase = new AttalScrollView( 0 );
	_base = new SectionBase( 0, "base" );
	viewBase->setSection( _base );
	addTab( viewBase, tr( "Bases" ) );

	AttalScrollView * viewBuilding = new AttalScrollView( 0 );
	_building = new SectionBuilding( 0, "building" );
	viewBuilding->setSection( _building );
	addTab( viewBuilding, tr( "Buildings" ) );

	AttalScrollView * viewCreature = new AttalScrollView( 0 );
	_creature = new SectionCreature( 0, "creatures" );
	viewCreature->setSection( _creature );
	addTab( viewCreature, tr( "Creatures" ) );

	AttalScrollView * viewDecoration = new AttalScrollView( 0 );
	_decoration = new SectionDecoration( 0, "decoration" );
	viewDecoration->setSection( _decoration );
	addTab( viewDecoration, tr( "Decorations" ) );

	AttalScrollView * viewGround = new AttalScrollView( 0 );
	_ground = new SectionGround( 0, "ground" );
	viewGround->setSection( _ground );
	addTab( viewGround, tr( "Grounds" ) );

	AttalScrollView * viewLordCategory = new AttalScrollView( 0 );
	_lordCategories = new SectionLordCategory( 0, "lord categories" );
	viewLordCategory->setSection( _lordCategories );
	addTab( viewLordCategory, tr( "Lord Categories" ) );

	AttalScrollView * viewLord = new AttalScrollView( 0 );
	_lord = new SectionLord( 0, "lords" );
	viewLord->setSection( _lord );
	addTab( viewLord, tr( "Lords" ) );

	AttalScrollView * viewExperience = new AttalScrollView( 0 );
	_experience = new SectionExperience( 0, "experience" );
	viewExperience->setSection( _experience );
	addTab( viewExperience, tr( "Levels" ) );

	AttalScrollView * viewResource = new AttalScrollView( 0 );
	_resource = new SectionResource( 0, "resources" );
	viewResource->setSection( _resource );
	addTab( viewResource, tr( "Resources" ) );

	AttalScrollView * viewMachine = new AttalScrollView( 0 );
	_machine = new SectionMachine( 0, "machines" );
	viewMachine->setSection( _machine );
	addTab( viewMachine, tr( "War Machines" ) );

	connect( this, SIGNAL( currentChanged( int ) ), SLOT( slot_changed( int ) ) );
	connect( _creature, SIGNAL( sig_changed() ), SLOT( slot_creaturesChanged() ) );
	connect( _lordCategories, SIGNAL( sig_changed() ), SLOT( slot_lordCategoryChanged() ) );
	connect( _general, SIGNAL( sig_changed() ), SLOT( slot_generalChanged() ) );

	_current = _general;
}

void SectionSelector::save()
{
	_current->save();
}

void SectionSelector::saveAll()
{
	_lordCategories->save();
	_general->save();
	_artefact->save();
	_team->save();
	_building->save();
	_creature->save();
	_decoration->save();
	_ground->save();
	_experience->save();
	_resource->save();
	_machine->save();
	_base->save();
	_lord->save();
}

void SectionSelector::slot_changed( int index )
{
	QWidget * widg = widget( index );
	_current->save();
	_current = ((AttalScrollView *)widg)->getSection();
}

void SectionSelector::slot_creaturesChanged()
{
	_lordCategories->updateData();
	_lord->updateData();
}

void SectionSelector::slot_generalChanged()
{
	_base->updateData();
}

void SectionSelector::slot_lordCategoryChanged()
{
	_lord->updateData();
}
