/****************************************************************
**
** Attal : Lords of Doom
**
** loadGame.h
** manage loading of scenarii and campaigns
**
** Version : $Id: loadGame.h,v 1.24 2013/11/19 21:40:59 lusum Exp $
**
** Author(s) : Sardi Carlo
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


#ifndef LOADGAME_H
#define LOADGAME_H

// generic include files
// include files for QT
#include <QObject>
#include <QList>
// application specific includes

#include "libServer/attalServer.h"
#include "libServer/engine.h"

class Campaign;

class LoadGame
{
public:

	enum StatusWidget {
		W_NORMAL,
		W_CLIENT,
		W_ENGINE
	};
	
	LoadGame();
	
	virtual ~LoadGame();

	virtual void reinit();

	virtual void addInternalAI();
	
	virtual	void disconnectAI() = 0;

	virtual	void disconnectClient() {};
	
	virtual	bool killAI() { return true; }
				
	virtual void adjustWidgetLoading( StatusWidget type ) = 0;
	
	void reinitStatus();

	void initEngine();

	void load( const QString  & filename);

	void loadCampaign(QString fileName);

	void loadSingle(const QString & filename);
	
	bool fillWithAI(const QString  & filename);

	void autosave();
	
	void save();
	
	void save(QString filename);

	QString getFileName(QString filename);
	
	int neededAiPlayers( QString filename ) { return _engine->getNeededPlayersFast( filename ); }

protected:
	void endGame();
	
	void newEngine();
	
	void endEngine();
	
	void endServer();

	void loadScenario( int num );

	void handleCheckEndGame();
	
	void handleResult(bool result);
	
	bool _readyIn, _winner;
	bool _aiDbg;
	//_local : true = a game is local ( embedded in client ) ; false = remote ( server standalone )
	bool _local;

	AttalServer * _server;

private:
	
	void stopAi();

	void continueCampaign( int next );

	bool _inLoad;
	Engine * _engine;
	
};

#endif // LOADGAME_H
