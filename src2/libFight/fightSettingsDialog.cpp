/****************************************************************
**
** Attal : Lords of Doom
**
** template.cpp
** this is a template for all .cpp files
**
** Version : $Id: fightSettingsDialog.cpp,v 1.6 2007/03/21 12:08:32 lusum Exp $
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

#include "fightSettingsDialog.h"

// generic include files

// include files for QT
#include <QCheckBox>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>

// application specific include files
#include "libCommon/attalSettings.h"

#include "libClient/attalButton.h"
#include "libClient/gui.h"

#include "libClient/askWidget.h"

/** add comments here */
FightSettingsDialog::FightSettingsDialog( QWidget* parent )
	:QDialog ( parent )
{
	setWindowTitle(tr("Settings"));
	_animationCheckBox = new QCheckBox( tr ( "Animation" ), this );
	connect( _animationCheckBox, SIGNAL ( clicked () ), this, SLOT ( slot_animationCheckBoxClicked () ) );

	_animationSpeed = new AskInt( tr( "Animation Speed" ), this );
	_animationSpeed->setMinValue( 1 );
	_animationSpeed->setValue( 6 );
	_animationSpeed->setMaxValue( 10 );

	_cellsCheckBox = new QCheckBox ( tr ( "Show cells" ), this );

	AttalButton * okButton = new AttalButton( this, AttalButton::BT_OK );
	connect( okButton, SIGNAL ( pressed ( ) ), this, SLOT ( slot_okButtonClicked ()) );

	QVBoxLayout* layout = new QVBoxLayout ( this );
	layout->setMargin ( 15 );
	layout->addWidget ( _animationCheckBox, 1 , Qt::AlignHCenter );
	layout->addWidget ( _animationSpeed, 1, Qt::AlignHCenter );
	layout->addWidget ( _cellsCheckBox, 1, Qt::AlignHCenter );
	layout->setSpacing ( 10 );
	layout->addWidget( okButton, 1 , Qt::AlignHCenter );

	layout->activate();

	AttalSettings::FightSettings fsettings = AttalSettings::getInstance()->getFightSettings();

	_animationSpeed->setEnabled( fsettings.isAnimationEnabled );
	_animationSpeed->setValue( fsettings.animationSpeed );

	_animationCheckBox->setChecked( fsettings.isAnimationEnabled );
	_cellsCheckBox->setChecked( fsettings.areCellsVisible );

	FIXEDSIZE( this );
}

void FightSettingsDialog::slot_okButtonClicked()
{
	accept();
}

void FightSettingsDialog::slot_cancelButtonClicked()
{
	QDialog::reject();
}

void FightSettingsDialog::accept ()
{
	AttalSettings::FightSettings fsettings;
	fsettings.isAnimationEnabled = _animationCheckBox->isChecked();
	fsettings.animationSpeed = _animationSpeed->getValue();
	fsettings.areCellsVisible = _cellsCheckBox->isChecked();
	AttalSettings::getInstance()->setFightSettings( fsettings );

	QDialog::accept();
}

void FightSettingsDialog::slot_animationCheckBoxClicked()
{
	_animationSpeed->setEnabled( _animationCheckBox->isChecked() );
}

