/****************************************************************
**
** Attal : Lords of Doom
**
** decorationEfefct.cpp
** Widget for editing effects of decoration
**
** Version : $Id: decorationEffect.cpp,v 1.11 2006/10/05 21:33:10 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 15/08/2003
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

#include "decorationEffect.h"

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
#include "libCommon/genericDecoration.h"
#include "libCommon/log.h"

#include "libClient/gui.h"


//
// ----- DecorationEffect -----
//

DecorationEffect::DecorationEffect( QWidget * parent, const char * name )
: EditWidget( parent, name )
{
	_group = 0;

	connect( _pbAdd, SIGNAL( clicked() ), SLOT( slot_add() ) );
	connect( _pbEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
}

void DecorationEffect::init( DecorationGroup * group )
{
	_group = group;

	_listBox->clear();
	if( _group ) {
		uint nb = _group->getEffectNumber();

		for( uint i = 0; i < nb; ++i ) {
			DecorationGroup::EffectType type = _group->getEffectType( i );
			uint param = _group->getEffectParam( i );

			_listBox->addItem( new EffectItem( type, param ) );
		}
	}
}

void DecorationEffect::slot_add()
{
	DialogEffect dialog;

	if( dialog.exec() ) {
		DecorationGroup::EffectType type = dialog.getType();
		uint param = dialog.getParam();
		_listBox->addItem( new EffectItem( type, param ) );
	}
}

void DecorationEffect::slot_edit()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		DialogEffect dialog;
		dialog.setType( ( (EffectItem*)(_listBox->item( item ) ) )->getType() );
		dialog.setParam( ( (EffectItem*)(_listBox->item( item ) ) )->getParam() );
		if( dialog.exec() ) {
			DecorationGroup::EffectType type = dialog.getType();
			uint param = dialog.getParam();
			EffectItem * effect = (EffectItem*) _listBox->item( item );
			effect->setType( type );
			effect->setParam( param );
		}
	}
}

void DecorationEffect::slot_del()
{
	int item = _listBox->currentRow();
	if( item != -1 ) {
		delete _listBox->takeItem( item );
	}
}

void DecorationEffect::save()
{
	_group->clearEffects();
	uint nb = _listBox->count();
	for( uint i = 0; i < nb; ++i ) {
		EffectItem * effect = (EffectItem*) _listBox->item( i );
		if( effect->getType() != DecorationGroup::NONE ) {
			_group->addEffect( effect->getType(), effect->getParam() );
		}
	}
}

//
// ----- EffectItem -----
//

EffectItem::EffectItem( DecorationGroup::EffectType type, uint param )
: QListWidgetItem()
{
	_param = param;
	_type = type;
	computeLabel();
}

void EffectItem::computeLabel()
{
	QString text = QObject::tr( "Effect '" );
	text += DecorationGroup::getEffectTypeString( _type );
	text += QObject::tr( "' with param " );
	text += QString::number( _param );
	setText( text );
}


//
// ----- DialogEffect -----
//

DialogEffect::DialogEffect( QWidget * parent, const char * name )
: QDialog( parent, Qt::Dialog )
{
	setWindowTitle(QString (name));
	QLabel * labType = new QLabel( tr( "Effect type: " ), this );
	FIXEDSIZE( labType );

	_combo = new QComboBox( this );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::NONE ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::NO_MOVE ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::DECREASE_MOVECOST ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::INCREASE_MOVECOST ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::NO_TECHNICAL ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::MAX_TECHNICAL ) );
	_combo->addItem( DecorationGroup::getEffectTypeString( DecorationGroup::NO_BLOCK_DECO ) );
	FIXEDSIZE( _combo );

	QLabel * labParam = new QLabel( tr( "Effect value: " ), this );
	FIXEDSIZE( labParam );

	_spin = new QSpinBox( this );
	_spin->setMinimum( 0 );

	QGridLayout * grid = new QGridLayout();
	grid->addWidget( labType, 1, 0 );
	grid->addWidget( labParam, 2, 0 );
	grid->addWidget( _combo, 1, 1 );
	grid->addWidget( _spin, 2, 1 );
	grid->addItem(new QSpacerItem(5, 0), 0, 0); 
	grid->addItem(new QSpacerItem(5, 0), 0, 3); 

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
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

void DialogEffect::updateDialog()
{
	_spin->setValue( _param );
	_combo->setCurrentIndex( (int)_type );
}

void DialogEffect::accept()
{
	_param = _spin->value();
	_type = (DecorationGroup::EffectType) _combo->currentIndex();
	QDialog::accept();
}

