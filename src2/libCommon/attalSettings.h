/****************************************************************
**
** Attal : Lords of Doom
**
** attalSettings.h
**
** Author(s) : Nick Semenov
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


#ifndef ATTALSETTINGS_H
#define ATTALSETTINGS_H


// generic include files

// include files for QT
#include <QSettings>
// application specific include files


/*              ------------------------------
 *                         AttalSettings
 *              ------------------------------ */

class AttalSettings
{
public:
	virtual ~AttalSettings() { save(); }

	static AttalSettings * getInstance();

	void load();

	void save();

	struct FightSettings
	{
		bool isAnimationEnabled;
		bool areCellsVisible;
		int animationSpeed;
	};

	const FightSettings & getFightSettings() { return _fightSettings; }

	void setFightSettings( const FightSettings & fightSettings ) { _fightSettings = fightSettings; }

	struct StrategyModeSettings
	{
		bool isAnimationEnabled;
		bool isSaveTurnEnabled;
		bool isMapVisible;
		bool isSoundOn;
		bool isMusicOn;
		QString connectionName;
	};

	const StrategyModeSettings & getStrategyModeSettings() { return _strategyModeSettings; }

	void setStrategyModeSettings( const StrategyModeSettings & strategyModeSettings ) { _strategyModeSettings = strategyModeSettings; }

	enum DispositionMode {
		DM_VERYCOMPACT,
		DM_COMPACT,
		DM_FULL
	};

	const DispositionMode & getDispositionMode() { return _mode; }

	void setDispositionMode( const DispositionMode & mode ) { _mode = mode; }

protected:
	AttalSettings() { load(); }

	static AttalSettings * _instance;

	FightSettings _fightSettings;
	StrategyModeSettings _strategyModeSettings;
	DispositionMode _mode;
	
};

#endif // ATTALSETTINGS_H
