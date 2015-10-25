/****************************************************************
**
** Attal : Lords of Doom
**
** questConditionEditor.cpp
** Editor of conditions used in quests
**
** Version : $Id: questConditionEditor.cpp,v 1.13 2006/10/06 21:27:36 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 14/06/2004
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

#include "questConditionEditor.h"

// generic include files
// include files for QT
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/log.h"
#include "libCommon/quest.h"

#include "libClient/gui.h"

#include "scenarioEditor/questConditionProperties.h"

QuestConditionEditor::QuestConditionEditor( QWidget * parent, const char * name )
: QWidget( parent )
{
	setWindowTitle( QString( name ) );
	_condition = 0;

	_title = new QLabel( this );
	_title->setText( tr( "Condition: " ) );
	FIXEDSIZE( _title );

	QPushButton * pbEdit = new QPushButton( this );
	pbEdit->setText( tr( "Edit" ) );
	FIXEDSIZE( pbEdit );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addWidget( _title );
	layout->addSpacing( 5 );
	layout->addWidget( pbEdit );
	layout->addStretch( 1 );
	layout->activate();

	connect( pbEdit, SIGNAL( clicked() ), SLOT( slot_edit() ) );
}

void QuestConditionEditor::setTitle( const QString & title )
{
	_title->setText( title );
	FIXEDSIZE( _title );
}

void QuestConditionEditor::slot_edit()
{
	if( ! _condition ) {
		_condition = new QuestConditionComposite();
	}

	QuestConditionDialog dialog( _condition );
	dialog.exec();
}


//
// ----- QuestConditionDialog -----
//

QuestConditionDialog::QuestConditionDialog( QuestCondition * condition, QWidget * parent, const char * /* name */ )
: QDialog( parent, Qt::Dialog )
{
	_contextItem = 0;
	_condition = condition;
	_menu = new QMenu();

	_list = new QTreeWidget( this );
	_list->setColumnCount( 1 );
	QStringList labels;
	labels << tr( "Condition" );
	_list->setHeaderLabels( labels );
	buildTree();
	_list->setContextMenuPolicy(Qt::DefaultContextMenu);

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
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addWidget( _list, 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
}

void QuestConditionDialog::buildTree()
{
	_list->clear();
	if( _condition ) {
		QuestConditionItem * item = new QuestConditionItem( _list );
		item->setExpanded( true );
		item->setCondition( _condition );
		if( _condition->getType() == QuestCondition::COMPOSITE ) {
			QuestConditionComposite * composite = ( QuestConditionComposite * ) _condition;
			uint nb = composite->getConditionNumber();
			for( uint i = 0; i < nb; ++i ) {
				buildSubTree( item, composite->getCondition( i ) );
			}
		}
	}
}

void QuestConditionDialog::buildSubTree( QuestConditionItem * root, QuestCondition * condition )
{
	QuestConditionItem * item = new QuestConditionItem( root );
	item->setExpanded( true );
	item->setCondition( condition );
	if( condition->getType() == QuestCondition::COMPOSITE ) {
		QuestConditionComposite * composite = ( QuestConditionComposite * ) condition;
		uint nb = composite->getConditionNumber();
		for( uint i = 0; i < nb; ++i ) {
			buildSubTree( item, composite->getCondition( i ) );
		}
	}
}

void QuestConditionDialog::contextMenuEvent(QContextMenuEvent *e)
{
	//QTreeWidgetItem *item = _list->itemAt(e->globalPos());
	QTreeWidgetItem *item = _list->currentItem();
	_menu->clear();
	_contextItem = (QuestConditionItem *) item;
	if( _contextItem ) {
		_menu->addAction( tr( "&Properties" ), this, SLOT( slot_actionPropertiesCondition() ) );
		if( _contextItem->getCondition()->getType() == QuestCondition::COMPOSITE ) {
			_menu->addAction( tr( "&Add condition" ), this, SLOT( slot_actionAddCondition() ) );
		}
		if( _contextItem->parent() ) {
			_menu->addAction( tr( "&Delete condition" ), this, SLOT( slot_actionDelCondition() ) );
		}
		_menu->popup( e->globalPos() );
	} else {
		if( _list->topLevelItemCount() == 0 ) {
			_menu->addAction( tr( "&Add condition" ), this, SLOT( slot_actionAddCondition() ) );
			_menu->popup( e->globalPos() );
		}
	}
}

void QuestConditionDialog::slot_actionPropertiesCondition()
{
	QuestCondition * condition = _contextItem->getCondition();
	switch( condition->getType() ) {
	case QuestCondition::DATE: {
		QuestConditionDateProperties dialog( this );
		dialog.setCondition( (QuestConditionDate*)condition );
		dialog.exec();
		} break;
	case QuestCondition::LORD: {
		QuestConditionLordProperties dialog( this );
		dialog.setCondition( (QuestConditionLord*)condition );
		dialog.exec();
		} break;
	case QuestCondition::PLAYER: {
		QuestConditionPlayerProperties dialog( this );
		dialog.setCondition( (QuestConditionPlayer*)condition );
		dialog.exec();
		} break;
	default:
		break;
	}
}

void QuestConditionDialog::slot_actionAddCondition()
{
	NewQuestConditionDialog dialog;
	if( dialog.exec() ) {
		QuestConditionItem * item;
		QuestCondition * condition = dialog.getNewCondition();
		if( _contextItem ) {
			QuestCondition * parentCondition = _contextItem->getCondition();
			if( parentCondition->getType() == QuestCondition::COMPOSITE ) {
				( ( QuestConditionComposite * )parentCondition )->addCondition( condition );
			}
			buildTree();
		} else {
			item = new QuestConditionItem( _list );
			item->setCondition( condition );
		}
	}
}

void QuestConditionDialog::slot_actionDelCondition()
{
	if( _contextItem ) {
		QuestConditionItem * parentItem = dynamic_cast<QuestConditionItem*> ( _contextItem->parent() );
		QuestCondition * parentCondition = parentItem->getCondition();
		if( parentCondition->getType() == QuestCondition::COMPOSITE ) {
			( ( QuestConditionComposite * )parentCondition )->delCondition( _contextItem->getCondition() );
		}
		buildTree();
	}
}

//
// ----- QuestConditionItem -----
//

QuestConditionItem::QuestConditionItem( QTreeWidget * parent )
: QTreeWidgetItem( parent )
{
	_condition = 0;
}

QuestConditionItem::QuestConditionItem( QuestConditionItem * parent )
: QTreeWidgetItem( parent )
{
	_condition = 0;
}

void QuestConditionItem::setCondition( QuestCondition * condition )
{
	setText( 0, condition->getLabel() );
	_condition = condition;
}

//
// ----- NewQuestConditionDialog -----
//

NewQuestConditionDialog::NewQuestConditionDialog( QWidget * parent, const char * /* name */ )
: QDialog( parent, Qt::Dialog )
{
	_type = TC_NONE;

	_list = new QListWidget( this );
	_list->insertItem( TC_AND, tr( "AND" ) );
	_list->insertItem( TC_OR, tr( "OR" ) );
	_list->insertItem( TC_DATE, tr( "Date" ) );
	_list->insertItem( TC_LORD, tr( "Lord" ) );
	_list->insertItem( TC_PLAYER, tr( "Player" )  );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _list, 1 );
	layout->activate();

	connect( _list, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), this, SLOT( slot_select() ) );
}

QuestCondition * NewQuestConditionDialog::getNewCondition()
{
	QuestCondition * ret = 0;

	switch( _type ) {
	case TC_AND:
		ret = new QuestConditionComposite( QuestConditionComposite::AND );
		break;
	case TC_OR:
		ret = new QuestConditionComposite( QuestConditionComposite::OR );
		break;
	case TC_DATE:
		ret = new QuestConditionDate();
		break;
	case TC_LORD:
		ret = new QuestConditionLord();
		break;
	case TC_PLAYER:
		ret = new QuestConditionPlayer();
		break;
	default:
		break;
	}

	return ret;
}

void NewQuestConditionDialog::slot_select()
{
	_type = ( TypeCondition )_list->currentRow();
	accept();
}


