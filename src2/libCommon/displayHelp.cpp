/****************************************************************
**
** Attal : Lords of Doom
**
** displayHelp.cpp
** Dialog for displaying help (used by all exe)
**
** Version : $Id: displayHelp.cpp,v 1.6 2008/03/01 15:49:24 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/11/2004
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

#include "displayHelp.h"
 
// generic include files
// include files for QT
#include <QLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
// application specific include files

DisplayHelp::DisplayHelp( QWidget * parent, const char * /* name */)
: QDialog( parent, Qt::Dialog )
{
	_text = new QTextBrowser( this );
	QStringList slist = QStringList(".");
	slist.append( ATT_DOCS_DIR );
	_text->setSearchPaths( slist  );
	_text->setSource( QUrl ("./HOWTOPLAY.html") );
	
	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( QObject::tr( "Ok" ) );
	pbOk->setFixedSize( pbOk->sizeHint() );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _text, 1 );
	layout->addLayout( layH1 );
	
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
}

void DisplayHelp::slot_accept()
{
	accept();
}


