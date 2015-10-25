/****************************************************************
**
** Attal : Lords of Doom
**
** askWidget.cpp
** Classes for asking values to user
**
** Version : $Id: askWidget.cpp,v 1.3 2003/05/10 08:29:15 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 05/01/2002
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

#include "askWidget.h"

// generic include files
// include files for QT
#include <qcombobox.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpixmap.h>
#include <qpushbutton.h>
// application specific include files
#include "client/gui.h"


/************************************************************************/

AskString::AskString( QString text, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	layout->addSpacing( 5 );
	_label = new QLabel( this );
	_label->setText( text );
	FIXEDSIZE( _label );
	layout->addWidget( _label );
	layout->addSpacing( 5 );

	_value = new QLineEdit( this );
	_value->setFixedWidth( 250 );
	layout->addWidget( _value );
	layout->addStretch( 1 );

	layout->addSpacing( 5 );

	layout->activate();
}

void AskString::setText( QString text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

/************************************************************************/

AskBool::AskBool( QString text, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	_value = new QCheckBox( text, this );
	FIXEDSIZE( _value );
	layout->addWidget( _value );
	layout->addStretch( 1 );

	layout->addSpacing( 5 );

	layout->activate();
}

void AskBool::setText( QString text )
{
	_value->setText( text );
	FIXEDSIZE( _value );
}

/************************************************************************/

AskInt::AskInt( QString text, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	layout->addSpacing( 5 );
	_label = new QLabel( this );
	_label->setText( text );
	FIXEDSIZE( _label );
	layout->addWidget( _label );
	layout->addSpacing( 5 );

	_value = new QSpinBox( this );
	_value->setMinValue( 0 );
	_value->setMaxValue( 255 );
	FIXEDSIZE( _value );
	layout->addWidget( _value );
	layout->addStretch( 1 );

	layout->addSpacing( 5 );

	layout->activate();
}

void AskInt::setText( QString text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

/************************************************************************/

AskCombo::AskCombo( QString text, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	_label = new QLabel( this );
	_label->setText( text );
	FIXEDSIZE( _label );
	layout->addWidget( _label );

	_combo = new QComboBox( this );
	_combo->setEditable( false );
	FIXEDSIZE( _combo );
	layout->addWidget( _combo );
	layout->addStretch( 1 );

	layout->activate();

	connect( _combo, SIGNAL( activated( int ) ), SIGNAL( sig_activated( int ) ) );
}

void AskCombo::clear()
{
	_combo->clear();
}

void AskCombo::setText( QString text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

void AskCombo::insertItem( QString item )
{
	_combo->insertItem( item );
	FIXEDSIZE( _combo );
}

int AskCombo::currentItem()
{
	return _combo->currentItem();
}

void AskCombo::setCurrentItem( int num )
{
	_combo->setCurrentItem( num );
}

/************************************************************************/

AskPixmap::AskPixmap( bool display, QString destination, QString text, QWidget * parent, const char * name )
	: QWidget( parent, name )
{
	_pix = 0;
	_display = display;
	_destination = destination;
	_value = destination;

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	QLabel * label = new QLabel( this );
	label->setText( text );
	FIXEDSIZE( label );
	layout->addWidget( label );

	_butLoad = new QPushButton( this );
	FIXEDSIZE( _butLoad );
	layout->addWidget( _butLoad );

	layout->addStretch( 1 );
	layout->activate();

	connect( _butLoad, SIGNAL( clicked() ), SLOT( slot_loadPixmap() ) );

	updateDisplay();
}

AskPixmap::~AskPixmap()
{
	if( _pix ) {
		delete _pix;
	}
}

void AskPixmap::slot_loadPixmap()
{
	QString filename;
	filename = QFileDialog::getOpenFileName( "", "*.png", this );
	if ( !filename.isNull() ) {
		_value = filename;
		updateDisplay();
	}
}

void AskPixmap::setValue( QString value )
{
	_value = value;
	updateDisplay();
}

void AskPixmap::setDestination( QString destination )
{
	_destination = destination;
	setValue( destination );
}

void AskPixmap::updateDisplay()
{
	if( _display ) {
		if( _pix ) {
			delete _pix;
		}
		_pix = new QPixmap( _value );
		_butLoad->setPixmap( *_pix );
	} else {
		_butLoad->setText( _value );
	}
	FIXEDSIZE( _butLoad );
}

void AskPixmap::save()
{
	if( _value != _destination ) {
		QFile fileIn( _value );
		QFile fileOut( _destination );

		if( fileIn.exists() ) {
			fileIn.open( IO_ReadOnly );
			fileOut.open( IO_WriteOnly );

			while( !fileIn.atEnd() ) {
				fileOut.putch( fileIn.getch() );
		 	}
		}

		fileOut.close();
		fileIn.close();
	}
}


//
// ----- AskList -----
//

AskList::AskList( QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_limit = 0;

	_label = new QLabel( this );
	FIXEDSIZE( _label );

	QVBoxLayout * layV1 = new QVBoxLayout();
	//layV1->setMargin( 5 );
	layV1->addWidget( _label );
	layV1->addStretch( 1 );

	_list = new QListBox( this );

	QVBoxLayout * layV2 = new QVBoxLayout();
	layV2->setMargin( 5 );
	layV2->addWidget( _list, 1 );

	_pbNew = new QPushButton( this );
	_pbNew->setText( "New" );
	FIXEDSIZE( _pbNew );

	_pbDel = new QPushButton( this );
	_pbDel->setText( "Del" );
	FIXEDSIZE( _pbDel );

	_pbUp = new QPushButton( this );
	_pbUp->setText( "Up" );
	FIXEDSIZE( _pbUp );

	_pbDown = new QPushButton( this );
	_pbDown->setText( "Down" );
	FIXEDSIZE( _pbDown );

	QVBoxLayout * layV3 = new QVBoxLayout();
	layV3->setSpacing( 5 );
	layV3->setMargin( 5 );
	layV3->addWidget( _pbNew );
	layV3->addWidget( _pbDel );
	layV3->addStretch( 1 );
	layV3->addWidget( _pbUp );
	layV3->addWidget( _pbDown );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layV1 );
	layout->addLayout( layV2 );
	layout->addLayout( layV3 );
	layout->addStretch( 1 );

	layout->activate();

	connect( _pbNew, SIGNAL( clicked() ), SLOT( slot_new() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
	connect( _pbUp, SIGNAL( clicked() ), SLOT( slot_up() ) );
	connect( _pbDown, SIGNAL( clicked() ), SLOT( slot_down() ) );
	connect( _list, SIGNAL( doubleClicked( QListBoxItem * ) ), SLOT( slot_change( QListBoxItem * ) ) );
}

void AskList::setLabel( QString label )
{
	_label->setText( label );
	FIXEDSIZE( _label );
}


void AskList::setLimit( int limit )
{
	_limit = limit;
	if( _limit ) {

	}
}

uint AskList::count()
{
	return _list->count();
}

void AskList::clear()
{
	_list->clear();
}

void AskList::slot_new()
{
	bool ok;
	QString text = askValue( "", ok );
	if( ok ) {
		_list->insertItem( text );
		_list->setCurrentItem( _list->count() - 1 );
	}
}

void AskList::slot_del()
{
	if( _list->currentItem() > -1 ) {
		_list->removeItem( _list->currentItem() );
	}
}

void AskList::slot_up()
{
	int index = _list->currentItem();

	if( index > 0 ) {
		QString up = _list->text( index - 1 );
		QString down = _list->text( index );
		_list->changeItem( up, index );
		_list->changeItem( down, index - 1 );
		_list->setCurrentItem( index - 1 );
	}
}

void AskList::slot_down()
{
	int index = _list->currentItem();

	if( index < count() - 1 ) {
		QString up = _list->text( index );
		QString down = _list->text( index + 1 );
		_list->changeItem( up, index + 1 );
		_list->changeItem( down, index );
		_list->setCurrentItem( index + 1 );
	}
}

void AskList::slot_change( QListBoxItem * item )
{
	bool ok;
	QString text = askValue( item->text(), ok );
	if( ok ) {
		_list->changeItem( text, _list->currentItem() );
	}
}

QString AskList::askValue( QString value, bool & ok )
{
	return QInputDialog::getText( "New item", "Enter new item text: ", QLineEdit::Normal, value, &ok );
}

//
// ----- AskStringList
//

AskStringList::AskStringList( QWidget * parent, const char * name )
: AskList( parent, name )
{
}

void AskStringList::addValue( QString value )
{

}

void AskStringList::setValue( int num, QString value )
{
}

QString AskStringList::getValue( int num )
{
	return "";
}


//
// ----- AskIntList -----
//

AskIntList::AskIntList( QWidget * parent, const char * name )
: AskList( parent, name )
{
	_min = -2147483647;
	_max = 2147483647;
}

void AskIntList::setMinValue( int value )
{
	_min = value;
	if( _min > _max ) {
		_max = _min;
	}
}

void AskIntList::setMaxValue( int value )
{
	_max = value;
	if( _max < _min ) {
		_min = _max;
	}
}

void AskIntList::addValue( int value )
{

}

void AskIntList::setValue( int num, int value )
{

}

int AskIntList::getValue( int num )
{
	return 0;
}

QString AskIntList::askValue( QString value, bool & ok )
{
	return QString::number( QInputDialog::getInteger( "New item", "Enter new number: ", value.toInt(), _min, _max, 1, &ok ) );
}



