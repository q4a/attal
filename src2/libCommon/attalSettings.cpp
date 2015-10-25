/****************************************************************
**
** Attal : Lords of Doom
**
** Date : 05/12/2003
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

#include "attalSettings.h"

// generic include files
// include files for QT
// application specific include files
#include "libCommon/log.h"

AttalSettings * AttalSettings::_instance = 0;

AttalSettings * AttalSettings::getInstance()
{
	if( _instance == 0 ) {
		_instance = new AttalSettings();
	}

	return _instance;
}

void AttalSettings::load()
{
	QSettings settings( "Attal", "attal-client" );

	_fightSettings.isAnimationEnabled = settings.value( "FightSettings/isAnimationEnabled", true ).toBool();
	_fightSettings.areCellsVisible = settings.value( "FightSettings/areCellsVisible", true ).toBool();
	_fightSettings.animationSpeed = settings.value( "FightSettings/animationSpeed", 7 ).toInt();

	_strategyModeSettings.isAnimationEnabled = settings.value( "StrategyModeSettings/isAnimationEnabled", true ).toBool();
	_strategyModeSettings.isSaveTurnEnabled = settings.value( "StrategyModeSettings/isSaveTurnEnabled", false ).toBool();
	_strategyModeSettings.isMapVisible = settings.value( "StrategyModeSettings/isMapVisible", true ).toBool();
	_strategyModeSettings.isSoundOn = settings.value( "StrategyModeSettings/isSoundOn", true ).toBool();
	_strategyModeSettings.isMusicOn = settings.value( "StrategyModeSettings/isMusicOn", true ).toBool();
	_strategyModeSettings.connectionName = settings.value( "StrategyModeSettings/connectionName", "New Player" ).toString();
	
	_mode = (DispositionMode) settings.value( "Display/dispositionMode", (int) DM_FULL ).toInt();
}

void AttalSettings::save()
{
	QSettings settings( "Attal", "attal-client" );

	settings.setValue( "FightSettings/isAnimationEnabled", _fightSettings.isAnimationEnabled );
	settings.setValue( "FightSettings/areCellsVisible", _fightSettings.areCellsVisible );
	settings.setValue( "FightSettings/animationSpeed", _fightSettings.animationSpeed );

	settings.setValue( "StrategyModeSettings/isAnimationEnabled", _strategyModeSettings.isAnimationEnabled );
	settings.setValue( "StrategyModeSettings/isSaveTurnEnabled", _strategyModeSettings.isSaveTurnEnabled );
	settings.setValue( "StrategyModeSettings/isMapVisible", _strategyModeSettings.isMapVisible );
	settings.setValue( "StrategyModeSettings/isSoundOn", _strategyModeSettings.isSoundOn );
	settings.setValue( "StrategyModeSettings/isMusicOn", _strategyModeSettings.isMusicOn );
	settings.setValue( "StrategyModeSettings/connectionName", _strategyModeSettings.connectionName );
	
	settings.setValue( "Display/dispositionMode", (int) _mode );
}

