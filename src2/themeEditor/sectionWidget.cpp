/****************************************************************
**
** Attal : Lords of Doom
**
** sectionWidget.cpp
** widgets for building sections
**
** Version : $Id: sectionWidget.cpp,v 1.8 2006/11/05 12:40:46 lusum Exp $
**
** Author(s) : Pascal Audoux
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

#include "sectionWidget.h"
 
// generic include files
// include files for QT
#include <QComboBox>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files
#include "libClient/gui.h"


SelectionWidget::SelectionWidget( QWidget * parent, const char * /*name*/ )
	: QFrame( parent )
{	
	QHBoxLayout * layout = new QHBoxLayout( this );
	setFrameStyle( QFrame::Box | QFrame::Raised );
	
	layout->addSpacing( 5 );
	
	QPushButton * butFirst = new QPushButton( "|<", this );
	FIXEDSIZE( butFirst );
	layout->addWidget( butFirst );
	
	QPushButton * butPrevious = new QPushButton( "<", this );
	FIXEDSIZE( butPrevious );
	layout->addWidget( butPrevious );
	layout->addSpacing( 5 );
	
	_labTitle = new QLabel( tr( "None" ), this );
	FIXEDSIZE( _labTitle );
	layout->addWidget( _labTitle );
	layout->addSpacing( 5 );
	
	QPushButton * butNext = new QPushButton( ">", this );
	FIXEDSIZE( butNext );
	layout->addWidget( butNext );

	QPushButton * butLast = new QPushButton( ">|", this );
	FIXEDSIZE( butLast );
	layout->addWidget( butLast );
	
	layout->addSpacing( 5 );
	layout->addStretch( 1 );
	
	QPushButton * butNew = new QPushButton( "New" , this );
	FIXEDSIZE( butNew );
	layout->addWidget( butNew );
	layout->addSpacing( 5 );
	
	QPushButton * butDel = new QPushButton( "Del", this );
	FIXEDSIZE( butDel );
	layout->addWidget( butDel );
	
	layout->addSpacing( 5 );
	layout->activate();
	
	setMinimumHeight( 40 );
	
	connect( butFirst, SIGNAL( clicked() ), SIGNAL( sig_first() ) );
	connect( butPrevious, SIGNAL( clicked() ), SIGNAL( sig_previous() ) );
	connect( butNext, SIGNAL( clicked() ), SIGNAL( sig_next() ) );
	connect( butLast, SIGNAL( clicked() ), SIGNAL( sig_last() ) );
	connect( butNew, SIGNAL( clicked() ), SIGNAL( sig_new() ) );
	connect( butDel, SIGNAL( clicked() ), SIGNAL( sig_del() ) );
}

void SelectionWidget::setTitle( QString title )
{
	_labTitle->setText( title );
	FIXEDSIZE( _labTitle );
}


/************************************************************************/

Section::Section( QWidget * parent, const char * /* name*/ )
	: QWidget( parent )
{
}

/************************************************************************/

GenericSection::GenericSection( QWidget * parent, const char * name )
	: Section( parent, name )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_select = new SelectionWidget( this, "select" );
	layout->addWidget( _select );
	layout->addSpacing( 5 );
	
	_mainWidget = new QWidget( this );
	_mainWidget->setWindowTitle( "main widget" );
	layout->addWidget( _mainWidget, 1 );
	layout->addSpacing( 5 );
	
	layout->activate();
	
	connect( _select, SIGNAL( sig_first() ), SLOT( slot_first() ) );
	connect( _select, SIGNAL( sig_previous() ), SLOT( slot_previous() ) );
	connect( _select, SIGNAL( sig_next() ), SLOT( slot_next() ) );
	connect( _select, SIGNAL( sig_last() ), SLOT( slot_last() ) );
	connect( _select, SIGNAL( sig_new() ), SLOT( slot_new() ) ); 
	connect( _select, SIGNAL( sig_del() ), SLOT( slot_del() ) );
}

