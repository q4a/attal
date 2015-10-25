/****************************************************************
**
** Attal : Lords of Doom
**
** buildingAction.cpp
** Widget for editing actions of building
**
** Version : $Id: buildingAction.cpp,v 1.20 2008/06/09 11:27:50 lusum Exp $
**
** Author(s) : Sardi Carlo -Pascal Audoux
**
** Date : 29/04/2004
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

#include "buildingAction.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/genericBuilding.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"
#include "libCommon/action.h"
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"


//
// ----- buildingAction -----
//

BuildingAction::BuildingAction( QWidget * parent, const char * name )
: EditWidget( parent,name)
{
	connect( _pbAdd, SIGNAL( clicked() ), SLOT( slot_add() ) );
	connect( _pbEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
}

void BuildingAction::init( QList<Action *>  * list )
{
	TRACE("BuildingAction::init");

	QList<Action *> *  action = list;
	_listBox->clear();
	if( action->count() ) {
		for( int i = 0; i < action->count(); i++ ) {
			for( int j = 0; j < action->at(i)->getElementaryNumber(); j++ ) {
				ElementaryAction * elementary = action->at(i)->getElementaryAction( j );
				
				BuildingActionItem * actionItem;
				actionItem = new BuildingActionItem( action->at(i)->getType(),action->at(i)->getCoeff(), elementary->getType(), elementary->getArg(), elementary->getArg1() ,elementary->getCoeff() );
				_listBox->addItem( actionItem );
			}
		}
	}
}

void BuildingAction::slot_add()
{
	DialogBuildingAction dialog;

	if( dialog.exec() ) {
		BuildingActionItem * actionItem;
		Action::ActionType actionType = dialog.getActionType();
		int actValue = dialog.getActValue();
		ElementaryAction::ElementaryType ElementaryType = dialog.getModifType();
		uint param = dialog.getParam();
		uint param1 = dialog.getParam1();
		int value = dialog.getValue();
		actionItem = new BuildingActionItem( actionType, actValue , ElementaryType, param, param1, value );

		_listBox->addItem( actionItem );
	}
}

void BuildingAction::slot_edit()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		DialogBuildingAction dialog;
		dialog.setActionType( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getActionType() );
		dialog.setActValue( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getActValue() );
		dialog.setModifType( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getModifType() );
		dialog.setParam( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getParam() );
		dialog.setParam1( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getParam1() );
		dialog.setValue( ( (BuildingActionItem*)(_listBox->item( item ) ) )->getValue() );
		if( dialog.exec() ) {
			Action::ActionType actionType = dialog.getActionType();
			int actValue = dialog.getActValue();
			ElementaryAction::ElementaryType ElementaryType = dialog.getModifType();
			uint param = dialog.getParam();
			uint param1 = dialog.getParam1();
			int value = dialog.getValue();
			BuildingActionItem * action = (BuildingActionItem*) _listBox->item( item );
			action->setActionType( actionType );
			action->setActValue( actValue );
			action->setModifType( ElementaryType );
			action->setParam( param );
			action->setParam1( param1 );
			action->setValue( value );
		}
	}
}

void BuildingAction::slot_del()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		_listBox->takeItem( item );
	}
}

QList<Action *> BuildingAction::save()
{

	QList<Action *> list;

	uint nb = _listBox->count();
	for( uint i = 0; i < nb; ++i ) {
		BuildingActionItem * item = (BuildingActionItem*) _listBox->item( i );
		ElementaryAction * eleaction = new ElementaryAction();
		eleaction->setType(item->getModifType() );
		eleaction->setArg( item->getParam() );
		eleaction->setArg1( item->getParam1() );
		eleaction->setCoeff( item->getValue() );
		Action * builAction =  new Action();
		builAction->setType(item->getActionType());
		builAction->setCoeff( item->getActValue() );
		builAction->addElementaryAction(eleaction);

		list.append( builAction );
	}

	return list;
}

//
// ----- buildingActionItem -----
//

BuildingActionItem::BuildingActionItem( Action::ActionType action, int actValue,
	ElementaryAction::ElementaryType modif, uint param, uint param1 ,  int value )
: QListWidgetItem()
{
	_action = action;
	_actValue = actValue;
	_modif = modif;
	_param = param;
	_param1 = param1;
	_value = value;
	computeLabel();
}

void BuildingActionItem::computeLabel()
{
	Action * tmpa = new Action();
	tmpa->setType(_action);
	tmpa->setCoeff(_actValue );

	ElementaryAction * tmpe = new ElementaryAction();
	tmpe->setType(_modif);
	tmpe->setArg( _param );
	tmpe->setArg1( _param1 );
	tmpe->setCoeff(_value );

	tmpa->addElementaryAction( tmpe );

	QString text = tmpa->getDescription( false );
	
	setText( text );

	delete tmpa;

}


//
// ----- DialogbuildingAction -----
//

DialogBuildingAction::DialogBuildingAction( QWidget * parent, const char * /*name*/ )
: QDialog( parent, Qt::Dialog )
{
	QLabel * labAction = new QLabel( tr( "Action type: " ), this );
	FIXEDSIZE( labAction );
	
	int i;
	QString string;

	_comboAction = new QComboBox( this );

	for( i = 0; i < Action::LAST; i++ ) {
		Action::ActionType atype = ( Action::ActionType)i;
		string = Action::getActionTypeString( atype );
		string.replace( 0, 1,  string[0].toUpper() );
		_comboAction->addItem( string );
	}
	FIXEDSIZE( _comboAction );
	
	_spinActValue = new QSpinBox( this );
	_spinActValue->setMaximum( 100000 );
	
	_labActValue = new QLabel( tr( "Coeff: " ), this );

	_comboModif = new QComboBox( this );
	for( i = 1; i < ElementaryAction::LAST; i++ ) {
		ElementaryAction::ElementaryType etype = ( ElementaryAction::ElementaryType)i;
		string = ElementaryAction::getElemTypeString( etype );
		string.replace( 0, 1,  string[0].toUpper() );
		_comboModif->addItem( string );
	}
	FIXEDSIZE( _comboAction );

	QLabel * labModif = new QLabel( tr( "Elementary Action: " ), this );
	FIXEDSIZE( labModif );

	_labParam = new QLabel( tr( "Arg0: " ), this );
	
	_labParam1 = new QLabel( tr( "Arg1: " ), this );

	_labValue = new QLabel( tr( "Coeff: " ), this );
	
	_spinParam = new QSpinBox( this );
	_spinParam->setMinimum( 0 );
	
	_spinParam1 = new QSpinBox( this );
	_spinParam1->setMinimum( 0 );

	_spinValue = new QSpinBox( this );
	_spinValue->setRange( -100000, 100000 );
			
	_labParam->hide();
	_labParam1->hide();
	_spinParam->hide();
	_spinParam1->hide();

	QGridLayout * grid = new QGridLayout();
	grid->addWidget( labAction, 1, 0 );
	grid->addWidget( _comboAction, 1, 1 );
	grid->addWidget( _labActValue, 2, 0 );
	grid->addWidget( _spinActValue, 2, 1 );
	grid->addWidget( labModif, 3, 0 );
	grid->addWidget( _comboModif, 3, 1 );
	grid->addWidget( _labParam, 4, 0 );
	grid->addWidget( _spinParam, 4, 1 );
	grid->addWidget( _labParam1, 5, 0 );
	grid->addWidget( _spinParam1, 5, 1 );
	grid->addWidget( _labValue, 6, 0 );
	grid->addWidget( _spinValue, 6, 1 );
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
	connect( _comboModif,SIGNAL( currentIndexChanged ( int )  ), SLOT( slot_modif( int ) ) );
	connect( _comboAction,SIGNAL( currentIndexChanged ( int ) ), SLOT( slot_action( int ) ) );
	FIXEDSIZE( this );
}

void DialogBuildingAction::updateDialog()
{
	_comboAction->setCurrentIndex( (uint)_action );
	_spinActValue->setValue( _actValue );
	_comboModif->setCurrentIndex( (uint)_modif-1 );
	_spinParam->setValue( _param );
	_spinParam1->setValue( _param1 );
	_spinValue->setValue( _value );
}

void DialogBuildingAction::accept()
{
	_action = (Action::ActionType) _comboAction->currentIndex();
	_actValue = _spinActValue->value();
	_modif = (ElementaryAction::ElementaryType) (_comboModif->currentIndex() + 1);
	_param = _spinParam->value();
	_param1 = _spinParam1->value();
	_value = _spinValue->value();

	QDialog::accept();
}

void DialogBuildingAction::slot_modif( int index )
{
	ElementaryAction::ElementaryType etype =(  ElementaryAction::ElementaryType ) (index + 1);

	switch( etype ) {
		case ElementaryAction::ATTACK:
		case ElementaryAction::DEFENSE:
		case ElementaryAction::POWER:
		case ElementaryAction::KNOWLEDGE:
		case ElementaryAction::MOVE:
		case ElementaryAction::MAXMOVE:
		case ElementaryAction::TECHNICPOINT:
		case ElementaryAction::MAXTECHNICPOINT:
		case ElementaryAction::MORALE:
		case ElementaryAction::LUCK:
		case ElementaryAction::VISION:
		case ElementaryAction::EXPERIENCE:
		case ElementaryAction::CHARISMA:
			_labParam->hide();
			_labParam1->hide();
			_spinParam->hide();
			_spinParam1->hide();
			break;
		case ElementaryAction::SKILL:
			break;
		case ElementaryAction::ARTEFACT:
			break;
		case ElementaryAction::RESOURCEPERC:
		case ElementaryAction::RESSOURCE:
		case ElementaryAction::RANDRESSOURCE:
			_labParam1->hide();
			_spinParam1->hide();
			_labParam->show();
			_spinParam->show();
			break;
		case ElementaryAction::RESOURCEFROM:
		case ElementaryAction::UNIT:
			_labParam->show();
			_labParam1->show();
			_spinParam->show();
			_spinParam1->show();
			break;
		default:
			_labParam->show();
			_labParam1->show();
			_spinParam->show();
			_spinParam1->show();
			break;
	}

}

void DialogBuildingAction::slot_action( int index )
{

	Action::ActionType action = ( Action::ActionType ) index;

	switch( action ) {
		case Action::DATE :
			_labActValue->show();
			_spinActValue->show();
			break;
		default:
			_labActValue->hide();
			_spinActValue->hide();
			break;
	}


}
