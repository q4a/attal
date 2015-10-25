/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesLord.cpp
** this is a template for all .cpp files
**
** Version : $Id: propertiesLord.cpp,v 1.3 2003/05/20 12:19:50 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 07/12/2002
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

#include "propertiesLord.h"

// generic include files
// include files for QT
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
// application specific include files
#include "common/dataTheme.h"
#include "common/genericLord.h"

#include "client/askWidget.h"
#include "client/gui.h"
#include "client/imageTheme.h"

extern DataTheme DataTheme;
extern QString IMAGE_PATH;

//
// ----- PropertiesLord -----
//

PropertiesLord::PropertiesLord( QList<GenericPlayer> * players, QWidget  * parent, const char * name )
	:QTabDialog( parent, name, true )
{
	setCaption( "Lord properties" );
	_players = players;
	_lord = 0;

	_general = new PropertiesLordGeneral( players, this );
	_units = new PropertiesLordUnits( this );
	_artefacts = new PropertiesLordArtefacts( this );

	addTab( _general, "General" );
	addTab( _units, "Units" );
	addTab( _artefacts, "Artefacts" );
	setOkButton();
	setCancelButton();
}

void PropertiesLord::accept()
{
	_general->accept();
	_units->accept();
	_artefacts->accept();
	QTabDialog::accept();
}

void PropertiesLord::init( GenericLord * lord )
{
	_lord = lord;
	_general->init( lord );
	_units->init( lord );
	_artefacts->init( lord );
}

//
// ----- GeneralCharacteristic-----
//

GeneralCharacteristic::GeneralCharacteristic (int number, QWidget* parent , const char* name )
    : PixmapSpinBox (parent)
{
    _num = number;

    setFrameStyle (QFrame::Panel | QFrame::Raised);
    setLineWidth ( 1 );

    getPixmap ();

    QString text = getText ();

    PixmapSpinBox::init (&_pix, text, true);
};

void GeneralCharacteristic::init (GenericLord* lord)
{
    LordCharac charac = (LordCharac)_num;

    int value = lord->getBaseCharac ( charac );

    setRange(0, 255);
    setValue( value );
}

void GeneralCharacteristic::save ( GenericLord* lord )
{
    int value = getValue( );

    LordCharac charac = (LordCharac)_num;

    lord->setBaseCharac( charac, value);
}

void GeneralCharacteristic::getPixmap ()
{
    QString path = IMAGE_PATH + "misc/ico_";

    LordCharac charac = (LordCharac)_num;
    switch (charac)
    {
        case ATTACK: {
            path += "attack"; break; }
        case DEFENSE: {
            path += "defense"; break; }
        case POWER: {
            path += "power"; break; }
        case KNOWLEDGE: {
            path += "knowledge"; break; }
        case MOVE: {
            path += "move"; break; }
        case MAXMOVE: {
            path += "maxmove"; break; }
        case TECHNICPOINT: {
            path += "technicpoint"; break; }
        case MAXTECHNICPOINT: {
            path += "maxtechnicpoint"; break; }
        case MORALE: {
            path += "morale"; break; }
        case LUCK: {
            path += "luck"; break; }
        case VISION: {
            path += "vision"; break; }
        case EXPERIENCE: {
            path += "experience"; break; }        
    }

    path += ".png";
    if (!_pix.load (path)) {
        _pix.resize (10,10);
        _pix.fill ();
    }
    
}

       
QString GeneralCharacteristic::getText ()
{
   QString text = "";

    LordCharac charac = (LordCharac)_num;
    
    switch (charac)
    {
        case ATTACK: {
            text = "Attack"; break; }
        case DEFENSE: {
            text = "Defense"; break; }
        case POWER: {
            text = "Power"; break; }
        case KNOWLEDGE: {
            text = "Knowledge"; break; }
        case MOVE: {
            text = "Move"; break; }
        case MAXMOVE: {
            text = "Maxmove"; break; }
        case TECHNICPOINT: {
            text = "Technicpoint"; break; }
        case MAXTECHNICPOINT: {
            text = "Maxtechnicpoint"; break; }
        case MORALE: {
            text = "Morale"; break; }
        case LUCK: {
            text = "Luck"; break; }
        case VISION: {
            text = "Vision"; break; }
        case EXPERIENCE: {
            text = "Experience"; break; }
    }

    return text;
}

//
// ----- PropertiesLordGeneral -----
//

PropertiesLordGeneral::PropertiesLordGeneral( QList<GenericPlayer> * players, QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_lord = 0;
	_comboPlayer = new QComboBox( this );
	FIXEDSIZE( _comboPlayer );

	QVBoxLayout * layout = new QVBoxLayout;
	layout->setSpacing( 5 );
	layout->addWidget( _comboPlayer );
	layout->addStretch( 1 );
	layout->activate();

	_players = players;

       QGridLayout* gridLayout = new QGridLayout ( 6 , 2);
       gridLayout->setSpacing (5);

       for (int num = 0; num < 12; num++)
       {
           generalChar[num] = new GeneralCharacteristic (num + 1 , this);

           FIXEDSIZE (generalChar[num]);
           gridLayout->addWidget (generalChar[num],  (num) / 2 , num % 2);
       }

       gridLayout->activate ();

       QHBoxLayout* hlayout = new QHBoxLayout (this);
       hlayout->addLayout (layout);
       hlayout->addLayout (gridLayout , 1);
       hlayout->activate ();
}

void PropertiesLordGeneral::init( GenericLord * lord )
{
	_lord = lord;
	_comboPlayer->clear();
	for( uint  i = 0; i < _players->count(); i++ ) {
		QString title = QString::number( i ) + " - " + _players->at( i )->getName();
		_comboPlayer->insertItem( title, i );
	}

	if( lord->getOwner() ) {
		_comboPlayer->setCurrentItem( lord->getOwner()->getNum() );
	} else {
		_comboPlayer->setCurrentItem( 0 );
	}

       for (int num = 0; num < 12; num++)
       {
           generalChar[num]->init(lord);
       }
}

void PropertiesLordGeneral::accept()
{
	if( _lord->getOwner() ) {
		_lord->getOwner()->removeLord( _lord );
	}
	_lord->setOwner( _players->at( _comboPlayer->currentItem() ) );
        _players->at( _comboPlayer->currentItem() )->addLord( _lord );

       for (int num = 0; num < 12; num++)
       {
           generalChar[num]->save(_lord);
       }
}

//
// ----- PropertiesLordUnits -----
//

PropertiesLordUnits::PropertiesLordUnits( QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_lord = 0;

	QHBoxLayout * layH1 = new QHBoxLayout();
	QHBoxLayout * layH2 = new QHBoxLayout();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i] = new EditTroop( i, this );
		if( i <= (MAX_UNIT / 2 ) ) {
			layH1->addWidget( _troop[i] );
		} else {
			layH2->addWidget( _troop[i] );
		}
	}
	layH1->addStretch( 1 );
	layH2->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->addLayout( layH1 );
	layout->addLayout( layH2 );
	layout->addStretch( 1 );
	layout->activate();
}

void PropertiesLordUnits::init( GenericLord * lord )
{
	_lord = lord;
}

void PropertiesLordUnits::accept()
{
	for( int i = 0; i < MAX_UNIT; i++ ) {
		_troop[i]->save( i, _lord );
	}
}

//
// ----- EditTroop -----
//

EditTroop::EditTroop( int num, QWidget * parent, const char * name )
	: QFrame( parent, name )
{
	_unit = 0;
	setFrameStyle( QFrame::Panel | QFrame::Raised );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );

	QLabel * label = new QLabel( this );
	label->setText( "Unit " + QString::number( num ) );
	FIXEDSIZE( label );
	layout->addWidget( label );

	_race = new AskCombo( "Race : ", this );
	_race->setCurrentItem( 0 );
	layout->addWidget( _race );

	_name = new AskCombo( "Name : ", this );
	layout->addWidget( _name );

	_number = new AskInt( "Number : ", this );
	layout->addWidget( _number );

	layout->addStretch( 1 );
	layout->activate();

	updateData();

	connect( _race, SIGNAL( sig_activated( int ) ), SLOT( slot_raceChanged( int ) ) );
}

void EditTroop::slot_raceChanged( int race )
{
	_name->clear();
	for( uint i = 0; i < DataTheme.creatures.getRace( race )->count(); i++ ) {
		_name->insertItem( DataTheme.creatures.getRace( race )->at( i )->getName() );
	}
}

void EditTroop::updateData()
{
	int race = _race->currentItem();
	uint i;

	_race->clear();
	for( i = 0; i < DataTheme.countRace(); i++ ) {
		_race->insertItem( DataTheme.creatures.getRace( i )->getName() );
	}
	if( _unit ) {
		_race->setCurrentItem( _unit->getRace() );
	} else {
		_race->setCurrentItem( race );
	}

	_name->clear();
	for( i = 0; i < DataTheme.creatures.getRace( _race->currentItem() )->count(); i++ ) {
		_name->insertItem( DataTheme.creatures.getRace( _race->currentItem() )->at( i )->getName() );
	}
}

void EditTroop::clear()
{
	_race->setCurrentItem( 0 );
	_name->setCurrentItem( 0 );
	_number->setValue( 0 );
}

void EditTroop::init( GenericFightUnit * unit )
{
	_unit = unit;
	if( unit ) {
		_race->setCurrentItem( unit->getRace() );
		slot_raceChanged( unit->getRace() );
		_name->setCurrentItem( unit->getLevel() );
		_number->setValue( unit->getNumber() );
	} else {
		clear();
	}
}

void EditTroop::save( int i, GenericLord * lord )
{
	GenericFightUnit * unit = lord->getUnit( i );

	if( _number->getValue() > 0 ) {
		if( ! unit ) {
			unit = new GenericFightUnit();
		}
		unit->setCreature( _race->currentItem(), _name->currentItem() );
		unit->setNumber( _number->getValue() );
		lord->setUnit( i, unit );
	} else {
		if( unit ) {
			delete unit;
		}
		lord->setUnit( i, 0 );
	}
}


//
// ----- PropertiesLordArtefacts -----
//

PropertiesLordArtefacts::PropertiesLordArtefacts( QWidget * parent, const char * name )
: QWidget( parent, name )
{
	_lord = 0;
}

void PropertiesLordArtefacts::init( GenericLord * lord )
{
	_lord = lord;
}

void PropertiesLordArtefacts::accept()
{

}



















