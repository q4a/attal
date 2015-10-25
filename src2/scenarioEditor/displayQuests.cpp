/****************************************************************
**
** Attal : Lords of Doom
**
** displayQuests.cpp
** Dialog for displaying quests
**
** Version : $Id: displayQuests.cpp,v 1.9 2006/11/22 15:39:39 lusum Exp $
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

#include "displayQuests.h"

// generic include files
// include files for QT
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libClient/gui.h"

#include "libCommon/questManager.h"

#include "scenarioEditor/questEditor.h"


DisplayQuests::DisplayQuests( QWidget * parent, const char * /* name */ )
: QDialog( parent, Qt::Dialog )
{
	_quests = 0;

	QLabel * labMain = new QLabel( this );
	labMain->setText( tr( "Main quest: " ) );
	FIXEDSIZE( labMain );

	QPushButton * butMain = new QPushButton( this );
	butMain->setText( tr( "Edit" ) );
	FIXEDSIZE( butMain );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addWidget( labMain );
	layH1->addWidget( butMain );
	layH1->addStretch( 1 );

	QLabel * labOther = new QLabel( this );
	labOther->setText( tr( "Other quests:" ) );
	FIXEDSIZE( labOther );

	_list = new QListWidget( this );

	QPushButton * butEdit = new QPushButton( this );
	butEdit->setText( tr( "Edit" ) );
	FIXEDSIZE( butEdit );

	QPushButton * butAdd = new QPushButton( this );
	butAdd->setText( tr( "Add" ) );
	FIXEDSIZE( butAdd );

	QPushButton * butDel = new QPushButton( this );
	butDel->setText( tr( "Del" ) );
	FIXEDSIZE( butDel );

	QVBoxLayout * layV1 = new QVBoxLayout();
	layV1->setSpacing( 5 );
	layV1->addWidget( butEdit );
	layV1->addWidget( butAdd );
	layV1->addWidget( butDel );
	layV1->addStretch( 1 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setSpacing( 5 );
	layH2->addWidget( _list, 1 );
	layH2->addLayout( layV1 );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCancel );

	QHBoxLayout * layH3 = new QHBoxLayout();
	layH3->addStretch( 1 );
	layH3->addWidget( pbOk );
	layH3->addStretch( 1 );
	layH3->addWidget( pbCancel );
	layH3->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addWidget( labOther );
	layout->addLayout( layH2, 1 );
	layout->addLayout( layH3 );
	layout->activate();

	connect( butMain, SIGNAL( clicked() ), SLOT( slot_main() ) );
	connect( butEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
	connect( butAdd, SIGNAL( clicked() ), SLOT( slot_add() ) );
	connect( butDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCancel, SIGNAL( clicked() ), SLOT( reject() ) );
}

void DisplayQuests::reinit( QuestManager * quests )
{
	Quest * quest;

	_quests = quests;
	_list->clear();

	for( uint i = 0; i < _quests->getSecondaryQuestNumber(); i++ ) {
		quest = _quests->getSecondaryQuest( i );
		if( quest ) {
			_list->addItem( quest->getName() );
		}
	}
}

void DisplayQuests::slot_main()
{
	QuestEditor editor( this );
	Quest * quest = _quests->getMainQuest();
	if( ! quest ) {
		quest = new Quest();
		_quests->setMainQuest( quest );
	}
	editor.init( quest );
	if( editor.exec() ) {
		editor.save();
	}
}

void DisplayQuests::slot_edit()
{
}

void DisplayQuests::slot_add()
{
	Quest * quest = new Quest();
	QuestEditor editor( 0 );
	editor.init( quest );
	if( editor.exec() ) {
		editor.save();
		_quests->appendSecondaryQuest( quest );
		_list->addItem( quest->getName() );
	} else {
		delete quest;
	}
}

void DisplayQuests::slot_del()
{

}


void DisplayQuests::slot_accept()
{
	accept();
}


