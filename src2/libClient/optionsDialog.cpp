/****************************************************************
**
** Attal : Lords of Doom
**
** optionsDialog.cpp
**
** Author(s) : Nickolay Semenov
**
** Date : 23/08/2004
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

#include "optionsDialog.h"

// generic include files
// include files for QT
#include <QCheckBox>
#include <QPushButton>
#include <QLayout>
#include <QSlider>

// application specific includes
#include "libCommon/attalSettings.h"
#include "libCommon/log.h"

#include "libClient/askWidget.h"
#include "libClient/attalButton.h"
#include "libClient/imageTheme.h"
#include "libClient/gui.h"
 
OptionsDialog::OptionsDialog ( QWidget* parent, const char * /* name */ )
	: QDialog ( parent, Qt::Dialog )
{
	_animationCheckBox = new QCheckBox ( tr( "Enable animation" ), this );
	FIXEDSIZE( _animationCheckBox );
	_saveCheckBox = new QCheckBox ( tr( "Save every turn" ), this );
	FIXEDSIZE( _saveCheckBox );
#ifdef WITH_SOUND
	QCheckBox * musicCheckBox = new QCheckBox ( tr( "Enable music" ), this );
	FIXEDSIZE( musicCheckBox );
	
	QSlider * musicSlider = new QSlider( this );
	musicSlider->setObjectName( QString("AttalMusicSlider"));
	musicSlider->setOrientation( Qt::Horizontal );
	musicSlider->setRange( 0,ImageTheme.maxVolume() );
	musicSlider->setSliderPosition( ImageTheme.volumeMusic( -1 ) );
	
	QCheckBox * soundCheckBox = new QCheckBox ( tr( "Enable sound" ), this );
	FIXEDSIZE( soundCheckBox );
	
	QSlider * soundSlider = new QSlider( this );
	soundSlider->setObjectName( QString("AttalSoundSlider"));
	soundSlider->setOrientation( Qt::Horizontal );
	soundSlider->setRange(0, ImageTheme.maxVolume() );
	soundSlider->setSliderPosition( ImageTheme.volumeSound( -1 ) );
#endif

	_dispositionMode = new AskCombo( tr( "Disposition: " ), this );
	_dispositionMode->insertItem( tr( "Extra compact" ) ); // DM_VERYCOMPACT
	_dispositionMode->insertItem( tr( "Compact" ) ); // DM_COMPACT
	_dispositionMode->insertItem( tr( "Normal" ) ); // DM_FULL
	FIXEDSIZE( _dispositionMode );

	QVBoxLayout * vlayout = new QVBoxLayout();
	vlayout->setMargin( 5 );
	vlayout->setSpacing( 5 );
	vlayout->addWidget( _animationCheckBox );
	vlayout->addWidget( _saveCheckBox );
#ifdef WITH_SOUND
	vlayout->addWidget( musicCheckBox );
	vlayout->addWidget( musicSlider );
	vlayout->addWidget( soundCheckBox );
	vlayout->addWidget( soundSlider );
#endif
	vlayout->addWidget( _dispositionMode );
	
	AttalButton * pbOk = new AttalButton ( this, AttalButton::BT_OK );	
	AttalButton * pbCancel = new AttalButton ( this, AttalButton::BT_CANCEL );

	QHBoxLayout * hlayout = new QHBoxLayout();
	hlayout->setMargin( 5 );
	hlayout->addStretch( 1 );
	hlayout->addWidget( pbCancel );
	hlayout->addStretch( 1 );
	hlayout->addWidget( pbOk );
	hlayout->addStretch( 1 );
	
	QVBoxLayout * mainlayout = new QVBoxLayout( this );
	mainlayout->addLayout( vlayout );
	mainlayout->addLayout( hlayout );
	mainlayout->activate();

	connect( pbOk, SIGNAL( clicked () ), this, SLOT( accept () ) );
	connect( pbCancel, SIGNAL( clicked () ), this, SLOT( reject () ) );
	connect( _animationCheckBox, SIGNAL( stateChanged( int ) ), this, SIGNAL( sig_animation( int ) ) );
#ifdef WITH_SOUND
	connect( musicCheckBox, SIGNAL( stateChanged( int ) ), SLOT( musicEnable( int ) ) );
	connect( soundCheckBox, SIGNAL( stateChanged( int ) ), SLOT( soundEnable( int ) ) );
	connect( musicSlider, SIGNAL( valueChanged ( int ) ), SLOT( musicVChanged( int ) ) );
	connect( soundSlider, SIGNAL( valueChanged ( int ) ), SLOT( soundVChanged( int ) ) );
#endif
	
	FIXEDSIZE ( this );

	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();

	_animationCheckBox->setChecked( settings.isAnimationEnabled );
	_saveCheckBox->setChecked( settings.isSaveTurnEnabled );
#ifdef WITH_SOUND
	musicCheckBox->setChecked( settings.isMusicOn );
	soundCheckBox->setChecked( settings.isSoundOn );
#endif

	_dispositionMode->setCurrentItem( (int) AttalSettings::getInstance()->getDispositionMode() );

}

void OptionsDialog::musicVChanged( int value )
{
	ImageTheme.volumeMusic( value );
}

void OptionsDialog::soundVChanged( int value )
{
	ImageTheme.volumeSound( value );
}

void OptionsDialog::accept()
{
	if( AttalSettings::getInstance()->getDispositionMode() != (AttalSettings::DispositionMode) _dispositionMode->currentItem() ) {
		AttalSettings::getInstance()->setDispositionMode( (AttalSettings::DispositionMode) _dispositionMode->currentItem() );
		emit sig_dispositions();
	}

	updateSettings();
	
	QDialog::accept ();
}

void OptionsDialog::updateSettings() 
{
	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();

	Qt::CheckState stateSave = _saveCheckBox->checkState();

	switch( stateSave ) {
		case Qt::Checked:
			{

				settings.isSaveTurnEnabled = true;
			}
			break;
		case Qt::Unchecked:
			{
				settings.isSaveTurnEnabled = false;
			}
			break;
		default:
			logEE("Should not happens %d",stateSave);
			break;
	}
	AttalSettings::getInstance()->setStrategyModeSettings( settings );
}

void OptionsDialog::musicEnable( int state )
{

	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();
  switch( state ) {
		case Qt::Checked:
			{
				settings.isMusicOn = true;
			}
			break;
		case Qt::Unchecked:
			{
				settings.isMusicOn = false;
			}
			break;
		default:
			logEE("Should not happens %d",state);
			break;
	}
	ImageTheme.enableMusic( settings.isMusicOn );

	AttalSettings::getInstance()->setStrategyModeSettings( settings );
}

void OptionsDialog::soundEnable( int state )
{
	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();
  switch( state ) {
		case Qt::Checked:
			{
				settings.isSoundOn = true;
			}
			break;
		case Qt::Unchecked:
			{
				settings.isSoundOn = false;
			}
			break;
		default:
			logEE("Should not happens %d",state);
			break;
	}

	ImageTheme.enableSound( settings.isSoundOn );

	AttalSettings::getInstance()->setStrategyModeSettings( settings );
}
