/*****************************************************************
**
** Attal : Lords of Doom
**
** editWidget.cpp
** Widget for editing
**
** Version : $Id: editWidget.cpp,v 1.2 2006/10/05 21:33:10 lusum Exp $
**
** Author(s) : Sardi Carlo
**
** Date : 03/09/2006
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


#include "editWidget.h"

// generic include files
// include files for QT
#include <QLabel>
#include <QLayout>
#include <QPushButton>
// application specific include files

#include "libClient/gui.h"

EditWidget::EditWidget( QWidget * parent, const char * name )
: QWidget( parent )
{
	setWindowTitle( QString ( name ));
	
	_listBox = new QListWidget( this );

	_pbAdd = new QPushButton( this );
	_pbAdd->setText( tr( "Add" ) );
	FIXEDSIZE( _pbAdd );

	_pbEdit = new QPushButton( this );
	_pbEdit->setText( tr("Edit")  );
	FIXEDSIZE( _pbEdit );

	_pbDel = new QPushButton( this );
	_pbDel->setText( tr( "Del" ) );
	FIXEDSIZE( _pbDel );

	QGridLayout * layout = new QGridLayout( this );
	layout->setRowStretch( 3, 1 );
	layout->setColumnStretch( 0, 1 );
	layout->addWidget( _listBox, 0, 0, 3, 0 );
	layout->addWidget( _pbAdd, 0, 1 );
	layout->addWidget( _pbEdit, 1, 1 );
	layout->addWidget( _pbDel, 2, 1 );

	layout->activate();

}
