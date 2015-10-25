/****************************************************************
**
** Attal : Lords of Doom
**
** questConditionProperties.cpp
** Display/edit properties of quest conditions
**
** Version : $Id: questConditionProperties.cpp,v 1.16 2007/04/05 23:03:27 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 20/06/2004
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

#include "questConditionProperties.h"

// generic include files
// include files for QT
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/dataTheme.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"
#include "libCommon/quest.h"

#include "libClient/gui.h"

//
// ----- QuestConditionProperties -----
//

QuestConditionProperties::QuestConditionProperties( QWidget * parent, const char * /* name */ )
: QDialog( parent, Qt::Dialog )
{
}

QuestConditionProperties::~QuestConditionProperties()
{
}

//
// ----- QuestConditionDateProperties -----
//

QuestConditionDateProperties::QuestConditionDateProperties( QWidget * parent, const char * name )
: QuestConditionProperties( parent, name )
{
	_condition = 0;

	_radioDate = new QRadioButton( this );
	_radioDate->setText( tr( "Before date: " ) );
	FIXEDSIZE( _radioDate );

	_day = new QSpinBox( this );
	_day->setMinimum( 1 );
	_day->setMaximum( 7 );
	FIXEDSIZE( _day );
	_labDay = new QLabel( tr( "Day" ), this );
	FIXEDSIZE( _labDay );

	_week = new QSpinBox( this );
	_week->setMinimum( 0 );
	_week->setMaximum( 3 );
	FIXEDSIZE( _week );
	_labWeek = new QLabel( tr( "Week" ), this );
	FIXEDSIZE( _labWeek );

	_month = new QSpinBox( this );
	_month->setMinimum( 0 );
	_month->setMaximum( 12 );
	FIXEDSIZE( _month );
	_labMonth = new QLabel( tr( "Month" ), this );
	FIXEDSIZE( _labMonth );

	_year = new QSpinBox( this );
	_year->setMinimum( 0 );
	_year->setMaximum( 9999 );
	FIXEDSIZE( _month );
	_labYear = new QLabel( tr( "Year" ), this );
	FIXEDSIZE( _labYear );


	QHBoxLayout * layHDate = new QHBoxLayout();
	layHDate->setSpacing( 5 );
	layHDate->setMargin( 5 );
	layHDate->addWidget( _radioDate );
	layHDate->addWidget( _labDay );
	layHDate->addWidget( _day );
	layHDate->addWidget( _labWeek );
	layHDate->addWidget( _week );
	layHDate->addWidget( _labMonth );
	layHDate->addWidget( _month );
	layHDate->addWidget( _labYear );
	layHDate->addWidget( _year );
	layHDate->addStretch( 1 );

	_radioDelay = new QRadioButton( this );
	_radioDelay->setText( tr( "Before " ) );
	FIXEDSIZE( _radioDelay );

	_delay = new QSpinBox( this );
	_delay->setMinimum( 1 );
	_delay->setMaximum( 336 );
	FIXEDSIZE( _delay );
	_labDelay = new QLabel( tr( "day(s)" ), this );
	FIXEDSIZE( _labDelay );

	QHBoxLayout * layHDelay = new QHBoxLayout();
	layHDelay->setSpacing( 5 );
	layHDelay->setMargin( 5 );
	layHDelay->addWidget( _radioDelay );
	layHDelay->addWidget( _delay );
	layHDelay->addWidget( _labDelay );
	layHDelay->addStretch( 1 );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
	layH1->addStretch( 1 );


	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layHDate );
	layout->addLayout( layHDelay );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( _radioDate, SIGNAL( clicked() ), SLOT( slot_date() ) );
	connect( _radioDelay, SIGNAL( clicked() ), SLOT( slot_delay() ) );
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );

	slot_date();
}

void QuestConditionDateProperties::slot_accept()
{
	if( _condition ) {
		if( _radioDate->isChecked() ) {
			_condition->setConditionDate( _day->value(), _week->value(), _month->value(), _year->value() );
		} else {
			_condition->setConditionDelay( _delay->value() );
		}
	}
	accept();
}

void QuestConditionDateProperties::setCondition( QuestConditionDate * condition )
{
	_condition = condition;

	if( _condition->getCategory() == QuestConditionDate::DATE ) {
		slot_date();
		_day->setValue( _condition->getDay() );
		_week->setValue( _condition->getWeek() );
		_month->setValue( _condition->getMonth() );
		_year->setValue( _condition->getYear() );
	} else {
		slot_delay();
		_delay->setValue( _condition->getDelay() );
	}
}

void QuestConditionDateProperties::slot_date()
{
	_radioDate->setChecked( true );
	_radioDelay->setChecked( false );
	_labDelay->setEnabled( false );
	_delay->setEnabled( false );
	_labDay->setEnabled( true );
	_labWeek->setEnabled( true );
	_labMonth->setEnabled( true );
	_labYear->setEnabled( true );
	_day->setEnabled( true );
	_week->setEnabled( true );
	_month->setEnabled( true );
	_year->setEnabled( true );
}

void QuestConditionDateProperties::slot_delay()
{
	_radioDate->setChecked( false );
	_radioDelay->setChecked( true );
	_labDelay->setEnabled( true );
	_delay->setEnabled( true );
	_labDay->setEnabled( false );
	_labWeek->setEnabled( false );
	_labMonth->setEnabled( false );
	_labYear->setEnabled( false );
	_day->setEnabled( false );
	_week->setEnabled( false );
	_month->setEnabled( false );
	_year->setEnabled( false );
}


//
// ----- QuestConditionLordProperties -----
//

QuestConditionLordProperties::QuestConditionLordProperties( QWidget * parent, const char * name )
: QuestConditionProperties( parent, name )
{
	_radioPrimary = new QRadioButton( this );
	_radioPrimary->setText( tr( "Primary skill" ) );
	FIXEDSIZE( _radioPrimary );

	_primCharac = new AskLordCharac( this );
	FIXEDSIZE( _primCharac );

	_primCheck = new QComboBox( this );
	_primCheck->addItem( "<" );
	_primCheck->addItem( "<=" );
	_primCheck->addItem( "=" );
	_primCheck->addItem( ">=" );
	_primCheck->addItem( ">" );
	FIXEDSIZE( _primCheck );

	_primValue = new AskInt( tr( "value: " ), this );
	FIXEDSIZE( _primValue );

	QHBoxLayout * layPrimary = new QHBoxLayout();
	layPrimary->setSpacing( 5 );
	layPrimary->setMargin( 5 );
	layPrimary->addWidget( _radioPrimary );
	layPrimary->addWidget( _primCharac );
	layPrimary->addWidget( _primCheck );
	layPrimary->addWidget( _primValue );
	layPrimary->addStretch( 1 );

	_radioSecondary  = new QRadioButton( this );
	_radioSecondary->setText( tr( "Secondary skill" ) );
	FIXEDSIZE( _radioSecondary );

	QHBoxLayout * laySecondary = new QHBoxLayout();
	laySecondary->setSpacing( 5 );
	laySecondary->setMargin( 5 );
	laySecondary->addWidget( _radioSecondary );
	laySecondary->addStretch( 1 );

	_radioArtefact = new QRadioButton( this );
	_radioArtefact->setText( tr( "Artefact" ) );
	FIXEDSIZE( _radioArtefact );

	_artefact = new AskCombo( "", this );
	uint nbArtefacts = DataTheme.artefacts.count();
	for( uint i = 0; i < nbArtefacts; i++ ) {
		_artefact->insertItem( DataTheme.artefacts.at( i )->getName() );
	}
	FIXEDSIZE( _artefact );

	QHBoxLayout * layArtefact = new QHBoxLayout();
	layArtefact->setSpacing( 5 );
	layArtefact->setMargin( 5 );
	layArtefact->addWidget( _radioArtefact );
	layArtefact->addWidget( _artefact );
	layArtefact->addStretch( 1 );

	_radioNum = new QRadioButton( this );
	_radioNum->setText( tr( "Number of creatures" ) );
	FIXEDSIZE( _radioNum );

	_numCheck = new QComboBox( this );
	_numCheck->addItem( "<" );
	_numCheck->addItem( "<=" );
	_numCheck->addItem( "=" );
	_numCheck->addItem( ">=" );
	_numCheck->addItem( ">" );
	FIXEDSIZE( _numCheck );

	_numValue = new AskInt( tr( "value: " ), this );
	FIXEDSIZE( _numValue );

	QHBoxLayout * layNum = new QHBoxLayout();
	layNum->setSpacing( 5 );
	layNum->setMargin( 5 );
	layNum->addWidget( _radioNum );
	layNum->addWidget( _numCheck );
	layNum->addWidget( _numValue );
	layNum->addStretch( 1 );



	_radioSpell = new QRadioButton( this );
	_radioSpell->setText( tr( "Spell" ) );
	FIXEDSIZE( _radioSpell );

	QHBoxLayout * laySpell = new QHBoxLayout();
	laySpell->setSpacing( 5 );
	laySpell->setMargin( 5 );
	laySpell->addWidget( _radioSpell );
	laySpell->addStretch( 1 );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
	layH1->addStretch( 1 );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layPrimary );
	layout->addLayout( laySecondary );
	layout->addLayout( layArtefact );
	layout->addLayout( layNum );
	layout->addLayout( laySpell );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( _radioPrimary, SIGNAL( clicked() ), SLOT( slot_primarySkill() ) );
	connect( _radioSecondary, SIGNAL( clicked() ), SLOT( slot_secondarySkill() ) );
	connect( _radioArtefact, SIGNAL( clicked() ), SLOT( slot_artefact() ) );
	connect( _radioSpell, SIGNAL( clicked() ), SLOT( slot_spell() ) );
	connect( _radioNum, SIGNAL( clicked() ), SLOT( slot_number() ) );
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
}

void QuestConditionLordProperties::setCondition( QuestConditionLord * condition )
{
	_condition = condition;
	switch( _condition->getConditionType() ) {
	case QuestConditionLord::PRIMARY_SKILL:
		slot_primarySkill();
		_primCharac->setCurrentItem( condition->getLordCharac() );
		_primCheck->setCurrentIndex( condition->getCheckType() );
		_primValue->setValue( condition->getValue() );
		break;
	case QuestConditionLord::SECONDARY_SKILL:
		slot_secondarySkill();
		break;
	case QuestConditionLord::ARTEFACT:
		slot_artefact();
		_artefact->setCurrentItem( condition->getArtefact() );
		break;
	case QuestConditionLord::NB_CREATURES:
		slot_number();
		_numCheck->setCurrentIndex( condition->getCheckType() );
		_numValue->setValue( condition->getValue() );
		break;
	case QuestConditionLord::SPELL:
		slot_spell();
		break;
	}
}

void QuestConditionLordProperties::slot_accept()
{
	if( _condition ) {
		if( _radioPrimary->isChecked() ) {
			_condition->setPrimarySkillCondition( (LordCharac)_primCharac->currentItem(), (QuestCondition::CheckType)_primCheck->currentIndex(), _primValue->getValue());
		} else if( _radioSecondary->isChecked() ) {
			//_condition->setSecondarySkillCondition();
		} else if( _radioArtefact->isChecked() ) {
			_condition->setArtefactCondition( _artefact->currentItem(), true );
	  } else if( _radioNum->isChecked() ) {
			_condition->setNumberCondition(  (QuestCondition::CheckType)_numCheck->currentIndex(), _numValue->getValue());
		} else {
			//_condition->setSpellCondition();
		}
	}
	accept();
}

void QuestConditionLordProperties::slot_primarySkill()
{
	_radioPrimary->setChecked( true );
	_radioSecondary->setChecked( false );
	_radioArtefact->setChecked( false );
	_radioSpell->setChecked( false );
	_radioNum->setChecked( false);
}

void QuestConditionLordProperties::slot_secondarySkill()
{
	_radioPrimary->setChecked( false );
	_radioSecondary->setChecked( true );
	_radioArtefact->setChecked( false );
	_radioSpell->setChecked( false );
	_radioNum->setChecked( false);
}

void QuestConditionLordProperties::slot_artefact()
{
	_radioPrimary->setChecked( false );
	_radioSecondary->setChecked( false );
	_radioArtefact->setChecked( true );
	_radioSpell->setChecked( false );
	_radioNum->setChecked( false);
}

void QuestConditionLordProperties::slot_spell()
{
	_radioPrimary->setChecked( false );
	_radioSecondary->setChecked( false );
	_radioArtefact->setChecked( false );
	_radioSpell->setChecked( true );
	_radioNum->setChecked( false);
}

void QuestConditionLordProperties::slot_number()
{
	_radioPrimary->setChecked( false );
	_radioSecondary->setChecked( false );
	_radioArtefact->setChecked( false );
	_radioSpell->setChecked( false );
	_radioNum->setChecked( true);
}

//
// ----- QuestConditionPlayerProperties -----
//

QuestConditionPlayerProperties ::QuestConditionPlayerProperties( QWidget * parent, const char * name )
: QuestConditionProperties( parent, name )
{
	_radioNbLords = new QRadioButton( this );
	_radioNbLords->setText( tr( "Nb lords " ) );
	FIXEDSIZE( _radioNbLords );
	
	_lordCheck = new QComboBox( this );
	_lordCheck->addItem( "<" );
	_lordCheck->addItem( "<=" );
	_lordCheck->addItem( "=" );
	_lordCheck->addItem( ">=" );
	_lordCheck->addItem( ">" );
	FIXEDSIZE( _lordCheck );
	
	_lordValue = new AskInt( "", this );
	FIXEDSIZE( _lordValue );
	
	QHBoxLayout * layNbLords = new QHBoxLayout();
	layNbLords->setSpacing( 5 );
	layNbLords->setMargin( 5 );
	layNbLords->addWidget( _radioNbLords );
	layNbLords->addWidget( _lordCheck );
	layNbLords->addWidget( _lordValue );
	layNbLords->addStretch( 1 );
	
	_radioNbBases = new QRadioButton( this );
	_radioNbBases->setText( tr( "Nb bases " ) );
	FIXEDSIZE( _radioNbBases );
	
	_baseCheck = new QComboBox( this );
	_baseCheck->addItem( "<" );
	_baseCheck->addItem( "<=" );
	_baseCheck->addItem( "=" );
	_baseCheck->addItem( ">=" );
	_baseCheck->addItem( ">" );
	FIXEDSIZE( _baseCheck );
	
	_baseValue = new AskInt( "", this );
	FIXEDSIZE( _baseValue );
	
	QHBoxLayout * layNbBases = new QHBoxLayout();
	layNbBases->setSpacing( 5 );
	layNbBases->setMargin( 5 );
	layNbBases->addWidget( _radioNbBases );
	layNbBases->addWidget( _baseCheck );
	layNbBases->addWidget( _baseValue );
	layNbBases->addStretch( 1 );
	
	_radioBasename = new QRadioButton( this );
	_radioBasename->setText( tr( "Basename = " ) );
	FIXEDSIZE( _radioBasename );
	
	_basename = new AskString( "", this );
	FIXEDSIZE( _basename );
	
	QHBoxLayout * layBasename = new QHBoxLayout();
	layBasename->setSpacing( 5 );
	layBasename->setMargin( 5 );
	layBasename->addWidget( _radioBasename );
	layBasename->addWidget( _basename );
	layBasename->addStretch( 1 );

	QPushButton * pbOk = new QPushButton( this );
	pbOk->setText( tr( "Ok" ) );
	FIXEDSIZE( pbOk );

	QPushButton * pbCan = new QPushButton( this );
	pbCan->setText( tr( "Cancel" ) );
	FIXEDSIZE( pbCan );

	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->setMargin( 5 );
	layH1->setSpacing( 5 );
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	layH1->addWidget( pbCan );
	layH1->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 5 );
	layout->addLayout( layNbLords );
	layout->addLayout( layNbBases );
	layout->addLayout( layBasename );
	layout->addStretch( 1 );
	layout->addLayout( layH1 );
	layout->activate();

	connect( _radioNbLords, SIGNAL( clicked() ), SLOT( slot_nbLords() ) );
	connect( _radioNbBases, SIGNAL( clicked() ), SLOT( slot_nbBases() ) );
	connect( _radioBasename, SIGNAL( clicked() ), SLOT( slot_basename() ) );
	connect( pbOk, SIGNAL( clicked() ), SLOT( slot_accept() ) );
	connect( pbCan, SIGNAL( clicked() ), SLOT( reject() ) );
	
	slot_nbLords();
}

void QuestConditionPlayerProperties::slot_nbLords()
{
	_radioNbLords->setChecked( true );
	_radioNbBases->setChecked( false );
	_radioBasename->setChecked( false );
}

void QuestConditionPlayerProperties::slot_nbBases()
{
	_radioNbLords->setChecked( false );
	_radioNbBases->setChecked( true );
	_radioBasename->setChecked( false );
}

void QuestConditionPlayerProperties::slot_basename()
{
	_radioNbBases->setChecked( false );
	_radioNbLords->setChecked( false );
	_radioBasename->setChecked( true );
}

void QuestConditionPlayerProperties ::setCondition( QuestConditionPlayer * condition )
{
	_condition = condition;
	switch( _condition->getConditionType() ) {
	case QuestConditionPlayer::NB_LORD:
		slot_nbLords();
		_lordCheck->setCurrentIndex( condition->getCheckType() );
		_lordValue->setValue( condition->getValue() );
		break;
	case QuestConditionPlayer::NB_BASE:
		_baseCheck->setCurrentIndex( condition->getCheckType() );
		_baseValue->setValue( condition->getValue() );
		slot_nbBases();
		break;
	case QuestConditionPlayer::BASENAME:
		slot_basename();
		_basename->setValue( condition->getName() );
		break;
	}
}

void QuestConditionPlayerProperties::slot_accept()
{
	if( _condition ) {
		if( _radioNbLords->isChecked() ) {
			_condition->setNbLordCondition( (QuestCondition::CheckType)_lordCheck->currentIndex(), _lordValue->getValue());
		} else if( _radioNbBases->isChecked() ) {
			_condition->setNbBaseCondition( (QuestCondition::CheckType)_baseCheck->currentIndex(), _baseValue->getValue());
		} else {
			_condition->setBasenameCondition( _basename->getValue() );
		}
	}
	accept();
}

//
// ----- AskLordCharac -----
//

AskLordCharac::AskLordCharac( QWidget * parent, const char * name )
: AskCombo( tr( "Charac" ), parent, name )
{
	insertItem( getCharacName( ATTACK ) );
	insertItem( getCharacName( DEFENSE ) );
	insertItem( getCharacName( POWER ) );
	insertItem( getCharacName( KNOWLEDGE ) );
	insertItem( getCharacName( MOVE ) );
	insertItem( getCharacName( MAXMOVE ) );
	insertItem( getCharacName( TECHNICPOINT ) );
	insertItem( getCharacName( MAXTECHNICPOINT ) );
	insertItem( getCharacName( MORALE ) );
	insertItem( getCharacName( LUCK ) );
	insertItem( getCharacName( CHARISMA ) );
	insertItem( getCharacName( VISION ) );
	insertItem( getCharacName( EXPERIENCE ) );
	FIXEDSIZE( this );
}


