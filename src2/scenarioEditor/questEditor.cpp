/****************************************************************
**
** Attal : Lords of Doom
**
** questEditor.cpp
** Dialog for editing a quest
**
** Version : $Id: questEditor.cpp,v 1.10 2006/11/03 18:28:59 fdarling Exp $
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

#include "questEditor.h"

// generic include files
// include files for QT
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files
#include "libCommon/quest.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"

#include "scenarioEditor/questConditionEditor.h"


QuestEditor::QuestEditor( QWidget * parent, const char * name )
: QDialog( parent )
{
	setWindowTitle( QString ( name ));
	_quest = 0;

	_name = new AskString( tr( "Name: " ), this );
	_description = new AskString( tr( "Description: " ), this );
	_type = new AskCombo( tr( "Type: " ), this );
	_type->insertItem( Quest::getTypeName( Quest::ONLY_ONE ) );
	_type->insertItem( Quest::getTypeName( Quest::ONE_PERPLAYER ) );
	_type->insertItem( Quest::getTypeName( Quest::ONE_PERLORD ) );
	_type->insertItem( Quest::getTypeName( Quest::ANYONE ) );
	_needConfirmation = new AskBool( tr( "Ask player confirmation of quest" ), this );
	_confirmationQuestion = new AskString( tr( "Confirmation question: " ), this );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCancel = new QPushButton( this );
	pbCancel->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCancel );

	QHBoxLayout * layH = new QHBoxLayout();
	layH->addStretch( 1 );
	layH->addWidget( pbOk );
	layH->addStretch( 1 );
	layH->addWidget( pbCancel );
	layH->addStretch( 1 );

	_editStartCondition = new QuestConditionEditor( this );
	_editStartCondition->setTitle( tr( "Start condition: " ) );

	_editFailCondition = new QuestConditionEditor( this );
	_editFailCondition->setTitle( tr( "Fail condition: " ) );

	_editSuccessCondition = new QuestConditionEditor( this );
	_editSuccessCondition->setTitle( tr( "Success condition: " ) );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _name );
	layout->addWidget( _description );
	layout->addWidget( _type );
	layout->addWidget( _needConfirmation );
	layout->addWidget( _confirmationQuestion );
	layout->addWidget( _editStartCondition );
	layout->addWidget( _editFailCondition );
	layout->addWidget( _editSuccessCondition );
	layout->addStretch( 1 );
	layout->addLayout( layH );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCancel, SIGNAL( clicked() ), SLOT( reject() ) );
	connect( _needConfirmation, SIGNAL( sig_changed() ), SLOT( slot_confirmation() ) );
	slot_confirmation();
}

void QuestEditor::slot_confirmation()
{
	_confirmationQuestion->setEnabled( _needConfirmation->getValue() );
}

void QuestEditor::init( Quest * quest )
{
	_quest = quest;
	if( _quest ) {
		_name->setValue( _quest->getName() );
		_description->setValue( _quest->getDescription() );
		_type->setCurrentItem( (uint)_quest->getType() );
		_needConfirmation->setValue( _quest->isConfirmationNeeded() );
		_confirmationQuestion->setValue( _quest->getConfirmationQuestion() );
		_editStartCondition->setCondition( quest->getStartCondition() );
		_editFailCondition->setCondition( quest->getFailCondition() );
		_editSuccessCondition->setCondition( quest->getSuccessCondition() );
		slot_confirmation();
	}
}

void QuestEditor::save()
{
	if( _quest ) {
		_quest->setName( _name->getValue() );
		_quest->setDescription( _description->getValue() );
		_quest->setType( (Quest::QuestType)_type->currentItem() );
		_quest->setConfirmationNeeded( _needConfirmation->getValue() );
		_quest->setConfirmationQuestion( _confirmationQuestion->getValue() );
		_quest->setStartCondition( _editStartCondition->getCondition() );
		_quest->setFailCondition( _editFailCondition->getCondition() );
		_quest->setSuccessCondition( _editSuccessCondition->getCondition() );
	}
}


