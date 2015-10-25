/****************************************************************
**
** Attal : Lords of Doom
**
** artefactAction.cpp
** Widget for editing actions of artefact
**
** Version : $Id: artefactAction.cpp,v 1.6 2007/05/03 12:56:15 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/03/2004
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

#include "artefactAction.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/artefact.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"

#include "libClient/gui.h"


//
// ----- ArtefactAction -----
//

ArtefactAction::ArtefactAction( QWidget * parent, const char *  name )
: EditWidget( parent, name )
{
	_artefact = 0;

	connect( _pbAdd, SIGNAL( clicked() ), SLOT( slot_add() ) );
	connect( _pbEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
}

void ArtefactAction::init( GenericArtefactModel * artefact )
{
	_artefact = artefact;

	_listBox->clear();
	if( artefact ) {
		uint nb = artefact->getActionNumber();

		for( uint i = 0; i < nb; ++i ) {
			ArtefactActionItem * actionItem;
			ArtefactElementaryAction * action = artefact->getAction( i );
			actionItem = new ArtefactActionItem( action->getActionType(), action->getModifType(), action->getParam(), action->getValue() );
			_listBox->addItem( actionItem );
		}
	}
}

void ArtefactAction::slot_add()
{
	DialogArtefactAction dialog;

	if( dialog.exec() ) {
		ArtefactActionItem * actionItem;
		ArtefactElementaryAction::ActionType actionType = dialog.getActionType();
		ArtefactElementaryAction::ModifType modifType = dialog.getModifType();
		uint param = dialog.getParam();
		int value = dialog.getValue();
		actionItem = new ArtefactActionItem( actionType, modifType, param, value );

		_listBox->addItem( actionItem );
	}
}

void ArtefactAction::slot_edit()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		DialogArtefactAction dialog;
		dialog.setActionType( ( (ArtefactActionItem*)(_listBox->item( item ) ) )->getActionType() );
		dialog.setModifType( ( (ArtefactActionItem*)(_listBox->item( item ) ) )->getModifType() );
		dialog.setParam( ( (ArtefactActionItem*)(_listBox->item( item ) ) )->getParam() );
		dialog.setValue( ( (ArtefactActionItem*)(_listBox->item( item ) ) )->getValue() );
		if( dialog.exec() ) {
			ArtefactElementaryAction::ActionType actionType = dialog.getActionType();
			ArtefactElementaryAction::ModifType modifType = dialog.getModifType();
			uint param = dialog.getParam();
			int value = dialog.getValue();
			ArtefactActionItem * action = (ArtefactActionItem*) _listBox->item( item );
			action->setActionType( actionType );
			action->setModifType( modifType );
			action->setParam( param );
			action->setValue( value );
		}
	}
}

void ArtefactAction::slot_del()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		_listBox->takeItem( item );
	}
}

void ArtefactAction::save()
{
	ArtefactElementaryAction * action;

	_artefact->clearActions();
	uint nb = _listBox->count();
	for( uint i = 0; i < nb; ++i ) {
		ArtefactActionItem * item = (ArtefactActionItem*) _listBox->item( i );
		action = new ArtefactElementaryAction( item->getActionType(), item->getModifType() );
		action->setParam( item->getParam() );
		action->setValue( item->getValue() );
		_artefact->addAction( action );
	}
}

//
// ----- ArtefactActionItem -----
//

ArtefactActionItem::ArtefactActionItem( ArtefactElementaryAction::ActionType action,
	ArtefactElementaryAction::ModifType modif, uint param, int value )
: QListWidgetItem()
{
	_action = action;
	_modif = modif;
	_param = param;
	_value = value;
	computeLabel();
}

void ArtefactActionItem::computeLabel()
{
	QString text = QObject::tr( "Action: " );
	if( _action == ArtefactElementaryAction::AT_CHARAC ) {
		text += QObject::tr( "Charac " );
		text += getCharacName( (LordCharac)_param );
	} else {
		text += QObject::tr( "Spell " );
		text += QString::number( _param );
	}
	if( _modif == ArtefactElementaryAction::MT_VALUE ) {
		text += QObject::tr( " modify by " ) + QString::number( _value );
	} else {
		text += QObject::tr( " modify by " ) + QString::number( _value ) + QObject::tr( "%" );
	}
	setText( text );
}


//
// ----- DialogArtefactAction -----
//

DialogArtefactAction::DialogArtefactAction( QWidget * parent, const char * /*name*/ )
: QDialog( parent, Qt::Dialog )
{
	QLabel * labAction = new QLabel( tr( "Action type: " ), this );
	FIXEDSIZE( labAction );

	_comboAction = new QComboBox( this );
	_comboAction->addItem( tr( "Characteristic" ) );
	_comboAction->addItem( tr( "Spell" ) );
	FIXEDSIZE( _comboAction );

	_comboModif = new QComboBox( this );
	_comboModif->addItem( tr( "Fixed value" ) );
	_comboModif->addItem( tr( "Ratio" ) );
	FIXEDSIZE( _comboAction );

	QLabel * labModif = new QLabel( tr( "Modification type: " ), this );
	FIXEDSIZE( labModif );

	QLabel * labParam = new QLabel( tr( "Parameter: " ), this );
	FIXEDSIZE( labParam );

	QLabel * labValue = new QLabel( tr( "Value: " ), this );
	FIXEDSIZE( labParam );

	_spinParam = new QSpinBox( this );
	_spinParam->setMinimum( 0 );

	_spinValue = new QSpinBox( this );

	QGridLayout * grid = new QGridLayout();
	grid->addWidget( labAction, 1, 0 );
	grid->addWidget( _comboAction, 1, 1 );
	grid->addWidget( labModif, 2, 0 );
	grid->addWidget( _comboModif, 2, 1 );
	grid->addWidget( labParam, 3, 0 );
	grid->addWidget( _spinParam, 3, 1 );
	grid->addWidget( labValue, 4, 0 );
	grid->addWidget( _spinValue, 4, 1 );
	grid->addItem(new QSpacerItem(5, 0), 0, 0);
	grid->addItem(new QSpacerItem(5, 0), 0, 5);

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addLayout( grid, 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
	FIXEDSIZE( this );
}

void DialogArtefactAction::updateDialog()
{
	_comboAction->setCurrentIndex( (int)_action );
	_comboModif->setCurrentIndex( (int)_modif );
	_spinParam->setValue( _param );
	_spinValue->setValue( _value );
}

void DialogArtefactAction::accept()
{
	_action = (ArtefactElementaryAction::ActionType) _comboAction->currentIndex();
	_modif = (ArtefactElementaryAction::ModifType) _comboModif->currentIndex();
	_param = _spinParam->value();
	_value = _spinValue->value();

	QDialog::accept();
}

