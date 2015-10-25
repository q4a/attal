/*****************************************************************
**
** Attal : Lords of Doom
**
** creatureAnimation.cpp
** Widget for editing animations of creatures
**
** Version : $Id: creatureAnimation.cpp,v 1.12 2007/05/14 22:06:26 lusum Exp $
**
** Author(s) : Sardi Carlo - Pascal Audoux
**
** Date : 29/07/2005
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

#include "creatureAnimation.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QSpinBox>
// application specific include files
#include "libCommon/creature.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"
#include "libCommon/dataTheme.h"

#include "libClient/gui.h"

//
// ----- CreatureAnimation -----
//

CreatureAnimation::CreatureAnimation( QWidget * parent, const char * name )
: EditWidget( parent, name )
{
	_creature = 0;
	
	connect( _pbAdd, SIGNAL( clicked() ), SLOT( slot_add() ) );
	connect( _pbEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
	
}

void CreatureAnimation::init( Creature * creature )
{
	_creature = creature;
	uint  num = _creature->numAnimation();
	_listBox->clear();
	if( num ) {
		for( uint i = 0; i < num; i++ ) {
			Creature::CreatureAnimation * animation = _creature->getCreatureAnimation(i);

			CreatureAnimationItem * actionItem;
			actionItem = new CreatureAnimationItem( animation->getType(), animation->getFirst(), animation->getLast() );
			_listBox->addItem( actionItem );
		}
	}
}

void CreatureAnimation::slot_add()
{
	DialogCreatureAnimation dialog;

	if( dialog.exec() ) {
		CreatureAnimationItem * actionItem;
		CreatureAnimationType AnimationType = dialog.getModifType();
		uint first = dialog.getFirst();
		int last = dialog.getLast();
		actionItem = new CreatureAnimationItem( AnimationType, first, last );

		_listBox->addItem( actionItem );
	}
}

void CreatureAnimation::slot_edit()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		DialogCreatureAnimation dialog;
		dialog.setModifType( ( (CreatureAnimationItem*)(_listBox->item( item ) ) )->getModifType() );
		dialog.setFirst( ( (CreatureAnimationItem*)(_listBox->item( item ) ) )->getFirst() );
		dialog.setLast( ( (CreatureAnimationItem*)(_listBox->item( item ) ) )->getLast() );
		if( dialog.exec() ) {
			CreatureAnimationType ElementaryType = dialog.getModifType();
			uint first = dialog.getFirst();
			int last = dialog.getLast();
			CreatureAnimationItem * action = (CreatureAnimationItem*) _listBox->item( item );
			action->setModifType( ElementaryType );
			action->setFirst( first );
			action->setLast( last );
		}
	}
}

void CreatureAnimation::slot_del()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		delete _listBox->takeItem( item );
	}
}

void CreatureAnimation::save()
{

	_creature->clearAnimations();
	uint nb = _listBox->count();
	for( uint i = 0; i < nb; ++i ) {
		CreatureAnimationItem * item = (CreatureAnimationItem*) _listBox->item( i );
		_creature->setAnimation( (CreatureAnimationType) item->getModifType(), item->getFirst(),item->getLast());
	}
}

//
// ----- CreatureAnimationItem -----
//

CreatureAnimationItem::CreatureAnimationItem( CreatureAnimationType type, uint first, uint last )
: QListWidgetItem()
{
	_type = type;
	_first = first;
	_last = last;
	computeLabel();
}

void CreatureAnimationItem::computeLabel()
{
	Creature * creature = new Creature();
	QString text =  QObject::tr("AnimationType: ");
	text += creature->getAnimationTypeName( _type  );
	text += QObject::tr( " first frame " );
	text += QString::number( _first );
	text += QObject::tr( " last frame " ) + QString::number( _last );
	setText( text );
	delete creature;
}


//
// ----- DialogCreatureAnimation -----
//

DialogCreatureAnimation::DialogCreatureAnimation( QWidget * parent, const char * /*name*/ )
: QDialog( parent, Qt::Dialog )
{
	_comboModif = new QComboBox( this );
	_comboModif->addItem( tr( "Moving" ) );
	_comboModif->addItem( tr( "Fighting" ) );
	_comboModif->addItem( tr( "Schooting" ) );
	_comboModif->addItem( tr( "SchootHigh" ) );
	_comboModif->addItem( tr( "SchootLow" ) );
	_comboModif->addItem( tr( "Defending" ) );
	_comboModif->addItem( tr( "StartMoving" ) );
	_comboModif->addItem( tr( "EndMoving" ) );
	_comboModif->addItem( tr( "Dying" ) );
	_comboModif->addItem( tr( "Dead" ) );
	_comboModif->addItem( tr( "AttackHigh" ) );
	_comboModif->addItem( tr( "AttackLow" ) );
	_comboModif->addItem( tr( "Selecting" ) );

	QLabel * labModif = new QLabel( tr( "Action: " ), this );
	FIXEDSIZE( labModif );

	QLabel * labFirst = new QLabel( tr( "Start Frame: " ), this );

	QLabel * labLast = new QLabel( tr( "End Frame: " ), this );
	
	_spinFirst = new QSpinBox( this );
	_spinFirst->setMinimum( 0 );

	_spinLast = new QSpinBox( this );
	_spinLast->setMaximum( 100000 );

	QGridLayout * grid = new QGridLayout();
	grid->addWidget( labModif, 2, 0 );
	grid->addWidget( _comboModif, 2, 1 );
	grid->addWidget( labFirst, 3, 0 );
	grid->addWidget( _spinFirst, 3, 1 );
	grid->addWidget( labLast, 4, 0 );
	grid->addWidget( _spinLast, 4, 1 );
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

void DialogCreatureAnimation::updateDialog()
{
	_comboModif->setCurrentIndex( (uint)_modif );
	_spinFirst->setValue( _first );
	_spinLast->setValue( _last );
}

void DialogCreatureAnimation::accept()
{
	_modif = (CreatureAnimationType) (_comboModif->currentIndex());
	_first = _spinFirst->value();
	_last = _spinLast->value();

	QDialog::accept();
}

