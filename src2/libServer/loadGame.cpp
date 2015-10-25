/****************************************************************
**
** Attal : Lords of Doom
**
** loadGame.ccppp
** manage loading of scenarii and campaigns
**
** Version : $Id: loadGame.cpp,v 1.45 2013/11/19 22:34:52 lusum Exp $
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

#include "loadGame.h"

// generic include files
// include files for QT
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
// application specific includes
#include "libCommon/log.h"
#include "libCommon/campaign.h"
#include "libCommon/attalSettings.h"

extern QString THEME;
extern QString SCENARIO_PATH;
extern QString CAMPAIGN_PATH;
extern QString SAVE_PATH;
extern QString PORT;


LoadGame::LoadGame()
{
	TRACE("LoadGame costructor");
	_aiDbg = false;
	_inLoad = false;
	_winner = true;
	_readyIn = true;
	_server = NULL;
	_engine = NULL;
	_local = false;
}

LoadGame::~LoadGame()
{
}

void LoadGame::reinit()
{
	
	_winner = true;
	adjustWidgetLoading( W_NORMAL );

}

void LoadGame::load(const QString & filename)
{
	TRACE("void LoadGame::load(QString filename %s, _server %p, _inLoad %d", qPrintable(  filename ), _server, _inLoad );

	if( filename.isNull() || _server->getNbSocket() <= 0  ||  _inLoad ) {
		return;
	}

	bool restart = false;

	/* semaphore to avoid process 2 signal at same time */
	_inLoad = true;
	fillWithAI(filename);
	do {
		if( !_engine->getNeededPlayersFast( filename ) ) {
			if( !_engine->load( filename ) ) {
				return;
			}
			restart = false;
			/* keep these istructions in this order */
			adjustWidgetLoading( W_ENGINE );
			_engine->startGame();
		} else {
			if ( _aiDbg ) {
				QMessageBox msb(  "Problem" , "Do you want to continue game (control right number of AI)?" , QMessageBox::Warning, QMessageBox::Yes | 			QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, 0, 0 );
				if( msb.exec() == QMessageBox::Yes){
					restart = true;
				}
			}
		}
	} while ( restart );
	_inLoad = false;

}

void LoadGame::loadCampaign(QString filename)
{
	TRACE("LoadGame::loadCampaign filename %s", qPrintable( filename ) );

	if( ( _server->getNbSocket() == 1 ) && ( ! filename.isNull() ) ) {
		if( _engine && _engine->loadCampaign( filename ) ) {
			Campaign * campaign = _engine->getCampaign();
			if( campaign ) {
				int actScen = campaign->getCurrentScenario();
				continueCampaign( actScen );
			}
		}
	}
}

void LoadGame::loadSingle(const QString & filename)
{
	TRACE("LoadGame::loadSingle filename %s", qPrintable( filename ) );

	//newEngine();
	load( filename );
}

void LoadGame::addInternalAI()
{
	_readyIn = true;
}

bool LoadGame::fillWithAI(const QString & filename)
{
	TRACE("bool LoadGame::fillWithAI(QString filename %s)",qPrintable(filename));

	int nplay = _engine->getNeededPlayersFast( filename );

	if ( nplay ) {
		TRACE("LoadGame::fillWithAI nplay %d",nplay);
		//QTimer::singleShot( (nplay+2)* 1000, this, SLOT( slot_ready() ) );
		for( int i = 0; i < nplay; i++ ) {
			_readyIn = false;
			TRACE("ai %d",i);
			addInternalAI();	
			while( !_readyIn ){
				qApp->processEvents();
			}
		}
		return true;
	}
	return false;
}

void LoadGame::loadScenario( int /* num */)
{
}

void LoadGame::autosave()
{
		
	TRACE("void LoadGame::autosave");

	if( _engine ) {
		
		if( AttalSettings::getInstance()->getStrategyModeSettings().isSaveTurnEnabled ) {
			QString numTurn = QString::number(_engine->getTurn());
			while( numTurn.size() < 3 ) {
				numTurn.prepend('0');
			}

			QString gamename = _engine->getScenarioName();
			QString savename  = gamename + "-turn-" + numTurn +".gam";

			TRACE("void LoadGame::savename %s", qPrintable(savename));
			save( QString( SAVE_PATH + savename ) );
		}
		save( QString( SAVE_PATH + "autosave.gam") );
	}
}

void LoadGame::save()
{
	if(_engine) {
		QString filen;
		filen = QFileDialog::getSaveFileName( 0 ,"", SAVE_PATH , "*.gam" );
		save( filen );
	}
	
}

void LoadGame::endEngine() 
{
	TRACE("LoadGame::endEngine, _engine %p", _engine );

	_engine->quit();
	_engine->wait();

	delete _engine;
	_engine = NULL;

}

void LoadGame::endServer() 
{
	TRACE("LoadGame::endServer _server %p", _server );

	if( _server ){
		delete _server;
		_server = NULL;
	}

}

void LoadGame::save(QString filename)
{
	if(_engine) {
		_engine->saveGame( filename );
	}

}

void LoadGame::handleCheckEndGame()
{
	TRACE("LoadGame::handleCheckEndGame");
	
	reinit();

	if( _engine && _engine->getCampaign()) {
		handleResult( _winner );
	} else {
		if( _local ) {
			disconnectClient();
		}
		endGame();
	}
}



void LoadGame::handleResult( bool result )
{
	TRACE("LoadGame::handleResult( bool result %d)", result);

	_winner = result;
	Campaign  * campaign = _engine->getCampaign();
	
	if( _engine && campaign ) {
	 
		int nextScen = campaign->getNextScenario();
	
		if( _winner && nextScen != -1 ) {
			continueCampaign( nextScen );
			//dont' endGame if continueCampaign
			return;
		}
	}
	
	endGame();

}

void LoadGame::endGame()
{
	TRACE("LoadGame::endGame _local %d, _server %p, _engine %p", _local, _server, _engine );

	if( !_local && _engine ) {
		_engine->endGameExternal();
	}

	stopAi();
	reinit();

	if( _engine ) {
		
		//if not campaign do nothing
		_engine->deleteCampaign();

		if( _local ) {
			endEngine();
			endServer();
		}
	}

	TRACE("LoadGame::endGame fin");
}

void LoadGame::stopAi()
{
	TRACE("LoadGame::stopAi");

	if(!_aiDbg) {
		//disconnectAI();
		killAI();
	}
}

void LoadGame::continueCampaign( int next )
{
	
	TRACE("LoadGame::continueCampaign( bool next %d)", next );

	stopAi();
	reinit();

	QString currentPath = CAMPAIGN_PATH;
	
	Campaign * campaign = _engine->getCampaign();
	campaign->setCurrentScenario( next );

	QString filename = campaign->getScenario( next );

	if(filename.contains(".gam")){
		currentPath = SAVE_PATH;
	}

	load( currentPath + filename );

}

void LoadGame::newEngine()
{	
	TRACE("LoadGame::newEngine()");

	_engine = new Engine( _server );
	qsrand(time(NULL));
	int id = qrand() % INT_MAX;
	_engine->setGameId( id );
	_engine->start();
}

QString LoadGame::getFileName(QString filename)
{
	filename = filename.section(QDir::separator(),-1,-1);

	if(filename.contains(".gam")){
		filename.remove(".gam");
	}
	
	if(filename.contains(".scn")){
		filename.remove(".scn");
	}
	return filename;
}
