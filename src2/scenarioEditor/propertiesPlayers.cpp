/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesPlayers.cpp
** properties of players (like resources)
**
** Author(s) : Nickolay Semenov - Sardi Carlo
**
** Date : 12/07/2003
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

#include "propertiesPlayers.h"

// generic include files
// include files for QT
#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"

#include "libClient/askWidget.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"

extern QString IMAGE_PATH;

PropertiesPlayers::PropertiesPlayers( QWidget * parent, QList<GenericPlayer *> * players )
	:QDialog (parent)
{
	uint i;
	_screenPlayers = players;

	_numPlayers = players->count ();
	_players = new GenericPlayer[_numPlayers];

	/** Copy parametrs, important for us (to accept or cancel later) */
	for( i = 0; i < _numPlayers; i++ ) {
		_players[i] = *players->at( i );
	}

	QVBoxLayout* vlayout = new QVBoxLayout (this);
	vlayout->setMargin (10);
	vlayout->setSpacing (10);

	QLabel* label = new QLabel (this);
	label->setText (tr("Players Properties"));
	FIXEDSIZE (label);

	vlayout->addWidget (label,1, Qt::AlignCenter);

	QGridLayout* gridLayout = new QGridLayout ();

	_comboPlayers = new QComboBox (this);
	_comboPlayers->setEditable (false);

	for( i = 0; i < _numPlayers; i++ ) {
		_comboPlayers->addItem( _players[i].getName(), i );
	}

	connect( _comboPlayers, SIGNAL( activated( int ) ), this, SLOT( slot_comboBoxItemChanged( int ) ) );

	gridLayout->addWidget (_comboPlayers, 0, 0, Qt::AlignCenter);

	_aiCheckBox = new QCheckBox (this);
	_aiCheckBox->setText (tr("Ruled by AI"));

	gridLayout->addWidget (_aiCheckBox, 1, 0, Qt::AlignCenter);
	
	_team = new AskInt (tr( "Team : "),this);
	for( i = 0; i < _numPlayers; i++ ) {
		_team->setValue( _players[i].getNum());
		_players[i].setTeam (_team->getValue());
	}
	gridLayout->addWidget (_team, 1, 0, Qt::AlignCenter);


	_flagButton = new QPushButton (this);
	connect( _flagButton, SIGNAL( clicked() ), this, SLOT( slot_flagButtonClicked() ) );

	gridLayout->addWidget (_flagButton, 2, 0 , Qt::AlignCenter);
	
	uint nbRes = DataTheme.resources.count();
	for( i = 0; i < nbRes; i++ ) {
		_spinBoxes[i] = new PixmapSpinBox (this);

		/** XXX: I couldn't find pixmaps for resources in ImageTheme... :( */
		/** The number of resources is a problem too */
		QPixmap* pix = 0;
		QString s = DataTheme.resources.getRessource (i);

		_spinBoxes[i]->setFrameStyle( QFrame::Panel | QFrame::Raised );
    		_spinBoxes[i]->setLineWidth( 1 );
		_spinBoxes[i]->init( pix, s, false );
		_spinBoxes[i]->setRange( 0, 1000000 );

		FIXEDSIZE (_spinBoxes[i]);

		gridLayout->addWidget (_spinBoxes[i], i / 2, i %2 + 1);
	}

	QPushButton* okButton = new QPushButton( this );
	okButton->setText( "Ok" );
	okButton->setFixedWidth (75);
	connect( okButton, SIGNAL( clicked() ), this , SLOT( slot_okButtonClicked() ) );

	gridLayout->addWidget (okButton, DataTheme.resources.count () / 2, 1, Qt::AlignCenter);

	QPushButton* cancelButton = new QPushButton (this);
	cancelButton->setText ("Cancel");
	cancelButton->setFixedWidth (75);
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( slot_cancelButtonClicked() ) );

	gridLayout->addWidget( cancelButton, DataTheme.resources.count () / 2, 2, Qt::AlignCenter );

	vlayout->addLayout (gridLayout, 1);

	vlayout->activate ();

	_comboPlayers->setCurrentIndex (0);
	_currentNumber = 0;

	formFlagsList (_currentNumber);
	setParams ();

}

PropertiesPlayers::~PropertiesPlayers()
{
	delete[] _players;
}

void PropertiesPlayers::setParams ()
{
	uint nbRes = DataTheme.resources.count();
	for( uint i = 0; i < nbRes; i++ ) {
		_spinBoxes[i]->setValue( _players[_currentNumber].getResourceList()->getValue(i) );
	}

	_aiCheckBox->setChecked (_players[_currentNumber].isRuledByAi ());
	_team->setValue(_players[_currentNumber].getTeamId());

	QPixmap* flag = ImageTheme.getFlag (_freeFlagsList[0]);
	_flagButton->setIcon (QIcon(*flag));
	_currentFlag = 0;
 }

void PropertiesPlayers::getParams ()
{
	uint nbRes = DataTheme.resources.count();

	for( uint i = 0; i < nbRes; i++ ) {
		_players[_currentNumber].getResourceList()->setValue( i, _spinBoxes[i]->getValue() );
	}

	_players[_currentNumber].setRuledByAi (_aiCheckBox->isChecked ());
	_players[_currentNumber].setNum (_freeFlagsList[_currentFlag]);
	_players[_currentNumber].setTeam (_team->getValue());
}

void PropertiesPlayers::accept ()
{
	getParams ();

	for( uint i = 0; i < _numPlayers; i++ )
	{
		*_screenPlayers->at (i) = _players[i];
	}

	QDialog::accept ();
}

void PropertiesPlayers::slot_comboBoxItemChanged( int i )
{
	getParams ();

	_currentNumber = i;
	formFlagsList( _currentNumber );

	setParams ();
}

void PropertiesPlayers::slot_okButtonClicked ()
{
	accept ();
}

void PropertiesPlayers::slot_cancelButtonClicked ()
{
	QDialog::reject ();
}

void PropertiesPlayers::formFlagsList (int numPlayer)
{
	uint numTeams = DataTheme.teams.count ();

	_numFreeFlags = 1;
	_freeFlagsList[0] = _players[numPlayer].getNum ();

	for( uint i = 0; i < numTeams; i++ ) {
		bool isFree = true;

		for( uint j = 0; j < _numPlayers; j++ ) {
			if( i == (uint) _players[i].getNum() )	{
				isFree = false;
			}
		}

		if( isFree ) {
			_freeFlagsList[_numFreeFlags] = i;
			_numFreeFlags++;
		}
	}
}

void PropertiesPlayers::slot_flagButtonClicked ()
{
	_currentFlag++;
	_team->setValue(_team->getValue()+1);
	if (_currentFlag == _numFreeFlags)
	{
		_currentFlag = 0;
		_team->setValue(0);
	}

	QPixmap* flag = ImageTheme.getFlag (_freeFlagsList[_currentFlag]);
	_flagButton->setIcon (QIcon(*flag));
}
