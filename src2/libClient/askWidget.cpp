/****************************************************************
**
** Attal : Lords of Doom
**
** askWidget.cpp
** Classes for asking values to user
**
** Version : $Id: askWidget.cpp,v 1.15 2007/11/01 15:52:30 lusum Exp $
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
#include <QColorDialog>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QLabel>
#include <QLayout>
#include <QListWidget>
#include <QPixmap>
#include <QPushButton>

// application specific include files
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"




//
// ----- AskString -----
//

AskString::AskString( QString text, QWidget * parent, const char *  /* name */)
	: QWidget( parent )
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

//
// ----- AskBool -----
//

AskBool::AskBool( QString text, QWidget * parent, const char * /* name */)
	: QWidget( parent )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	_value = new QCheckBox( text, this );
	FIXEDSIZE( _value );
	layout->addWidget( _value );
	layout->addStretch( 1 );

	layout->addSpacing( 5 );

	layout->activate();

	connect( _value, SIGNAL( clicked() ), SIGNAL( sig_changed() ) );
}

void AskBool::setText( QString text )
{
	_value->setText( text );
	FIXEDSIZE( _value );
}

//
// ----- AskInt -----
//

AskInt::AskInt( QString text, QWidget * parent, const char * /* name */ )
	: QWidget( parent )
{
	QHBoxLayout * layout = new QHBoxLayout( this );

	setFixedWidth( 200 );

	layout->addSpacing( 4 );
	_label = new QLabel( this );
	_label->setText( text );
	FIXEDSIZE( _label );
	if( _label->width() > 200 ) {
		setFixedWidth( 300 );
	}
	layout->addWidget( _label );
	//layout->addSpacing( 5 );
	layout->addStretch( 1 );

	_value = new QSpinBox( this );
	_value->setMinimum( 0 );
	_value->setMaximum( 255 );
	FIXEDSIZE( _value );
	layout->addWidget( _value );
	//layout->addStretch( 1 );

	//layout->addSpacing( 5 );

	layout->activate();
}

void AskInt::setText( QString text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
	if( _label->width() > 200 ) {
		setFixedWidth( 300 );
	}
}

//
// ----- AskCombo -----
//

AskCombo::AskCombo( QString text, QWidget * parent, const char * /*name */ )
	: QWidget( parent )
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

void AskCombo::setText( const QString & text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

void AskCombo::insertItem( const QString & item )
{
	_combo->addItem( item );
	FIXEDSIZE( _combo );
}

int AskCombo::currentItem()
{
	return _combo->currentIndex();
}

void AskCombo::setCurrentItem( int num )
{
	_combo->setCurrentIndex( num );
}

//
// ----- AskPixmap -----
//

AskPixmap::AskPixmap( bool display, const QString & destination, QString text, QWidget * parent, const char * /* name */)
	: QWidget( parent )
{
	_pix = NULL;
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

	_butLoad = new AttalButton( this );
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
	filename = QFileDialog::getOpenFileName( this, tr( "Open pixmap" ), "", "*.png" );
	if ( !filename.isNull() ) {
		_value = filename;
		updateDisplay();
	}
}

void AskPixmap::setValue( const QString & value )
{
	_value = value;
	updateDisplay();
}

void AskPixmap::setDestination( const QString & destination )
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
	char  c;

	if( _value != _destination ) {
		QFile fileIn( _value );
		QFile fileOut( _destination );

		if( fileIn.exists() ) {
			fileIn.open( QIODevice::ReadOnly );
			fileOut.open( QIODevice::WriteOnly );

			while( !fileIn.atEnd() ) {
				fileIn.getChar( &c );
				fileOut.putChar( c );
		 	}
		}

		fileOut.close();
		fileIn.close();
	}
}


//
// ----- AskList -----
//

AskList::AskList( QWidget * parent, const char * /* name */)
: QWidget( parent )
{
	_limit = 0;

	_label = new QLabel( this );
	FIXEDSIZE( _label );

	QVBoxLayout * layout = new QVBoxLayout( this );
	//layV1->setMargin( 5 );
	layout->addWidget( _label );
	layout->addStretch( 1 );

	_list = new QListWidget( this );

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

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addLayout( layV2 );
	layH1->addLayout( layV3 );
	layH1->addStretch( 1 );

	layout->addLayout( layH1 );

	layout->activate();

	connect( _pbNew, SIGNAL( clicked() ), SLOT( slot_new() ) );
	connect( _pbDel, SIGNAL( clicked() ), SLOT( slot_del() ) );
	connect( _pbUp, SIGNAL( clicked() ), SLOT( slot_up() ) );
	connect( _pbDown, SIGNAL( clicked() ), SLOT( slot_down() ) );
	connect( _list, SIGNAL( itemDoubleClicked( QListWidgetItem * ) ), SLOT( slot_change( QListWidgetItem * ) ) );
}

void AskList::setLabel( const QString & label )
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
		_list->addItem( text );
		_list->setCurrentRow( _list->count() - 1 );
	}
}

void AskList::slot_del()
{
	if( _list->currentRow() > -1 ) {
		_list->takeItem( _list->currentRow() );
	}
}

void AskList::slot_up()
{
	int index = _list->currentRow();

	if( index > 0 ) {
		QString up = _list->item( index - 1 )->text();
		QString down = _list->item( index )->text();
		_list->item( index )->setText( up );
		_list->item( index - 1 )->setText( down );
		_list->setCurrentRow( index - 1 );
	}
}

void AskList::slot_down()
{
	int index = _list->currentRow();

	if( index < (int)count() - 1 ) {
		QString up = _list->item( index )->text();
		QString down = _list->item( index + 1 )->text();
		_list->item( index + 1 )->setText( up );
		_list->item( index )->setText( down );
		_list->setCurrentRow( index + 1 );
	}
}

void AskList::slot_change( QListWidgetItem * item )
{
	bool ok;
	QString text = askValue( item->text(), ok );
	if( ok ) {
		_list->currentItem()->setText( text );
	}
}

QString AskList::askValue( const QString & value, bool & ok )
{
	return QInputDialog::getText( 0 ,"New item", "Enter new item text: ", QLineEdit::Normal, value, &ok );
}

//
// ----- AskStringList
//

AskStringList::AskStringList( QWidget * parent, const char * /* name */ )
: AskList( parent )
{
}

void AskStringList::addValue( const QString & value )
{
	_list->addItem( value );
}

void AskStringList::setValue( uint num, const QString & value )
{
	if( (int)num < _list->count() ) {
		_list->item( num )->setText( value );
	}
}

QString AskStringList::getValue( uint num )
{
	QString ret;

	if( (int)num < _list->count() ) {
		ret = _list->item( num )->text();
	}
	return ret;
}


//
// ----- AskIntList -----
//

AskIntList::AskIntList( QWidget * parent, const char * /* name */ )
: AskList( parent )
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
	_list->addItem( QString::number( value ) );
}

void AskIntList::setValue( uint num, int value )
{
	if( (int)num < _list->count() ) {
		_list->item( num )->setText( QString::number( value ) );
	}
}

int AskIntList::getValue( uint num )
{
	int ret = 0;

	if( (int)num < _list->count() ) {
		ret = _list->item( num )->text().toInt();
	}

	return ret;
}

QString AskIntList::askValue( const QString & value, bool & ok )
{
	return QString::number( QInputDialog::getInteger( 0, "New item", "Enter new number: ", value.toInt(), _min, _max, 1, &ok ) );
}

//
// ----- AskColor -----
//

AskColor::AskColor( const QString & text, QWidget * parent, const char * /* name */)
: QWidget( parent )
{
	_color = Qt::black;

	_label = new QLabel( this );
	_label->setText( text );
	FIXEDSIZE( _label );

	_colorButton = new QPushButton( this );
	QPalette palette;
	palette.setColor(_colorButton->backgroundRole(), _color);
	_colorButton->setPalette(palette);
	FIXEDSIZE( _colorButton );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addSpacing( 5 );
	layout->addWidget( _label );
	layout->addSpacing( 5 );
	layout->addWidget( _colorButton );
	layout->addStretch( 1 );
	layout->activate();

	connect( _colorButton, SIGNAL( clicked() ), SLOT( slot_color() ) );
}

void AskColor::slot_color()
{
	_color = QColorDialog::getColor( _color, this );
	QPalette palette;
	palette.setColor(_colorButton->backgroundRole(), _color);
	_colorButton->setPalette(palette);
}

void AskColor::setText( const QString & text )
{
	_label->setText( text );
	FIXEDSIZE( _label );
}

void AskColor::setValue( const QColor & color )
{
	_color = color;
	QPalette palette;
	palette.setColor(_colorButton->backgroundRole(), _color);
	_colorButton->setPalette(palette);
}

QColor AskColor::getValue()
{
	return _color;
}

