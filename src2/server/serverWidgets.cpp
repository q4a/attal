/****************************************************************
**
** Attal : Lords of Doom
**
** serverWidgets.cpp
** Various dialog for server
**
** Version : $Id: serverWidgets.cpp,v 1.7 2013/11/18 20:54:07 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 11/10/2006
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

#include "serverWidgets.h"
 
// generic include files
#include <QButtonGroup>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QString>

// application specific include files
#include "libCommon/log.h"

extern QString SCENARIO_PATH;

//
// ----- ChooseFileRadioButton -----
//

ChooseFileWidget::ChooseFileWidget( QWidget * parent, const char * /* name */)
	:QWidget( parent )
{
	QHBoxLayout * layout = new QHBoxLayout( this );
	//layout->addSpacing( 20 );
	
	_edit = new QLineEdit( this );
	layout->addWidget( _edit, 1 );
	layout->addSpacing( 10 );
	
	_choose = new QPushButton( this );
	_choose->setText( tr( "Choose" ) );
	FIXEDSIZE( _choose );
	layout->addWidget( _choose );
	
	layout->activate();
	
	connect( _choose, SIGNAL( clicked() ), SLOT( slot_choose() ) );
	setFixedHeight( _choose->size().height() + 25 );
}

void ChooseFileWidget::slot_choose()
{
	QString filename = QFileDialog::getOpenFileName( this, tr( "Choose scenario" ), "", "*.scn *.gam" ); 
	if( ! filename.isNull() ) {
		_edit->setText( filename );
	}
}

void ChooseFileWidget::slot_toggle( bool st )
{
	_choose->setEnabled( st );
	_edit->setEnabled( st );
}

//
// ----- ServerWidget -----
//

ServerWidget::ServerWidget( QWidget * parent, const char * /* name */)
	: QWidget( parent )
{	
	_groupWidget = new QGroupBox( this );

	_radio1 = new QRadioButton( _groupWidget );
	_radio1->setText( "Demo 1 player" );

	_radio2 = new QRadioButton( _groupWidget );
	_radio2->setText( "Demo 2 players" );

	_radio3 = new QRadioButton( _groupWidget );
	_radio3->setText( "Load game" );

	_choose = new ChooseFileWidget( _groupWidget );
	_choose->slot_toggle( false );
	_choose->setMinimumHeight( 50 );

	_playerList = new QTreeWidget( this );
	_playerList->setColumnCount( 2 );
	QStringList labels;
	labels << tr( "Name" );
	labels << tr( "Address" ); 
	_playerList->setHeaderLabels( labels );
	_playerList->setMinimumHeight( 50 );

	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->setSpacing( 5 );
	layH2->addWidget( _radio3 );
	layH2->addWidget( _choose, 1 );

	QVBoxLayout * layV1 = new QVBoxLayout( _groupWidget );
	layV1->setMargin( 5 );
	layV1->addWidget( _radio1 );
	layV1->addSpacing( 5 );
	layV1->addWidget( _radio2 );
	layV1->addLayout( layH2 );
	layV1->addStretch( 1 );
	layV1->activate();
	
	_butStart = new QPushButton( this );
	_butStart->setText( tr( "Start" ) );
	FIXEDSIZE( _butStart );

	_butSave = new QPushButton( this );
	_butSave->setText( tr( "Save" ) );
	FIXEDSIZE( _butSave );

	_butStop = new QPushButton( this );
	_butStop->setText( tr( "End" ) );
	FIXEDSIZE( _butStop );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addStretch( 1 );
	layH1->addWidget( _butStart );
	layH1->addStretch( 1 );
	layH1->addWidget( _butSave );
	layH1->addStretch( 1 );
	layH1->addWidget( _butStop );
	layH1->addStretch( 1 );
	
	_butStart->setEnabled( true );
	_butStop->setEnabled( false );
	_butSave->setEnabled( false );
	_groupWidget->setEnabled( true );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _groupWidget );
	layout->addSpacing( 5 );
	layout->addWidget( _playerList, 1 );
	layout->addSpacing( 5 );
	layout->addLayout( layH1 );
	layout->activate();

	_group = new QButtonGroup( this );
	_group->setExclusive( true );
	_group->addButton( _radio1, 0 );
	_group->addButton( _radio2, 1 );
	_group->addButton( _radio3, 2 );
	_radio1->setChecked( true );
		
	connect( _butStart, SIGNAL( clicked() ), SLOT( slot_start() ) );
	connect( _butSave, SIGNAL( clicked() ), SIGNAL( sig_save() ) );
	connect( _butStop, SIGNAL( clicked() ), SIGNAL( sig_stop() ) );
	connect( _radio3, SIGNAL( toggled( bool ) ), _choose, SLOT( slot_toggle( bool ) ) );
	connect( _playerList, SIGNAL( itemClicked ( QTreeWidgetItem *, int ) ),  SLOT( slot_clicked( QTreeWidgetItem *, int ) ) );
}

void ServerWidget::slot_newPlayer( AttalPlayerSocket * player )
{
	TRACE("ServerWidget::slot_newPlayer( AttalPlayerSocket * player %p)",player);
	TRACE("ServerWidget::slot_newPlayer connectionName %s", qPrintable( player->getPlayer()->getConnectionName() ) );
	
	QStringList entry;
	entry << player->getPlayer()->getConnectionName();
	entry << player->peerAddress().toString();
	new QTreeWidgetItem( _playerList, entry );
	/// XXX: to check localAddress or peerAddress ?
}

void ServerWidget::slot_endConnection( QString name )
{
	TRACE("ServerWidgets::slot_endConnection name: %s", qPrintable( name ) );

	QTreeWidgetItem * item;
	int i = 0;

	while( i < _playerList->topLevelItemCount() ) {
		item = _playerList->topLevelItem( i );

		if( item ) {
			if( item->text( 0 ) == name ) {
				_playerList->takeTopLevelItem( i );
				delete item;
				break;
			} else {
				++i;
			}
		} else {
			break;
		}
	}

	/*
	QTreeWidgetItem * item = _playerList->firstChild();
	do {
		if( item ) {
			if( item->text( 0 ) == name ) {
				found = true;
				_playerList->takeItem( item );
				delete item;
			} else {
				item = item->nextSibling();
			}
		} else {
			found = true;
		}
	} while( !found );*/
}

void ServerWidget::setGameLoaded( bool b )
{
	_loaded = b;
	if( _loaded ) {
		_butStart->setEnabled( false );
		_butStop->setEnabled( true );
		_butSave->setEnabled( true );
		_groupWidget->setEnabled( false );
	} else {
		_butStart->setEnabled( true );
		_butStop->setEnabled( false );
		_butSave->setEnabled( false );
		_groupWidget->setEnabled( true );
	}
}

QString ServerWidget::getFilename()
{
	QString filename;
	logDD(" %d", _group->checkedId() );
	switch( _group->checkedId() ) {
		case 0:
			filename = SCENARIO_PATH + "demo_1player.scn";
			break;
		case 1:
			filename = SCENARIO_PATH + "demo_2players.scn";
			break;
		case 2:
			filename = _choose->getText();
			break;
	}
	return filename;

}

void ServerWidget::slot_start()
{
	QString filename = getFilename();

	emit sig_load( filename );
}

void ServerWidget::slot_clicked(QTreeWidgetItem * item, int column)
{
	//logDD("clicked %s, column %d", qPrintable( item->text(column) ), column);
	if(column == 0) {
		QMessageBox msb( "Player", "Do you want kick this player ?", QMessageBox::Warning, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, this );
		if ( msb.exec() == QMessageBox::Yes ) {
			emit sig_banned( item->text(0) );
		}
	}
}

//
// ----- ConfigConnection -----
//

ConfigConnection::ConfigConnection( QWidget * parent )
	: QDialog( parent, Qt::Dialog )
{
	setWindowTitle( tr( "Start server" ) );
	
	QLabel * labHost = new QLabel( tr( "Host : " ), this );
	FIXEDSIZE( labHost );
	
	_host = new QLineEdit( this );
	_host->setFixedSize( 160, 20 );
	_host->setEnabled(false);
		
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addSpacing( 5 );
	layH1->addWidget( labHost );
	layH1->addSpacing( 5 );
	layH1->addWidget( _host );
	layH1->addStretch( 1 );
		
	QLabel * labPort = new QLabel( tr( "Port : " ), this );
	FIXEDSIZE( labPort );
	
	_port = new QLineEdit( this );
	_port->setFixedSize( 80, 20 );
	
	QHBoxLayout * layH2 = new QHBoxLayout();
	layH2->addSpacing( 5 );
	layH2->addWidget( labPort );
	layH2->addSpacing( 5 );
	layH2->addWidget( _port );
	layH2->addStretch( 1 );
		
	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Start" ) );
	pbOk->setFixedSize( 60, 60 );
	FIXEDSIZE( pbOk );
	
	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Quit" ) );
	FIXEDSIZE( pbCan );
	
	QHBoxLayout * layH3 = new QHBoxLayout();
	layH3->addStretch( 1 );
	layH3->addWidget( pbOk );
	layH3->addStretch( 1 );
	layH3->addWidget( pbCan );
	layH3->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addLayout( layH1, 1 );
	layout->addLayout( layH2, 1 );
	layout->addLayout( layH3, 2 );
	layout->activate();

	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );

	setFixedSize( 250, 150 );
}



