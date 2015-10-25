/****************************************************************
 **
 ** Attal : Lords of Doom
 **
 ** game.cpp
 ** Manages the whole game
 **
 ** Version : $Id: game.cpp,v 1.298 2014/06/22 19:27:31 lusum Exp $
 **
 ** Author(s) : Pascal Audoux - Sardi Carlo - Forest Darling
 **
 ** Date : 17/08/2000
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

#include "game.h"

// generic include files
#include <assert.h>
#include <exception>

// include files for QT
#include <QHBoxLayout>
#include <QHostAddress>
#include <QInputDialog>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QVBoxLayout>

// application specific includes
#include "conf.h"
#include "libCommon/artefactManager.h"
#include "libCommon/attalSettings.h"
#include "libCommon/attalSocket.h"
#include "libCommon/calendar.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericEvent.h"
#include "libCommon/genericInsideBuilding.h"
#include "libClient/graphicalPath.h"
#include "libCommon/pathFinder.h"
#include "libCommon/technic.h"
#include "libCommon/unit.h"
#include "libCommon/priceMarket.h"
#include "libCommon/attalTemplates.h"

//#include "libFight/fightUnit.h"

#include "libClient/askChest.h"
#include "libClient/askDialog.h"
#include "libClient/bonus.h"
#include "libClient/building.h"
#include "libClient/cell.h"
#include "libClient/chatWidget.h"
#include "libClient/chest.h"
#include "libClient/displayLord.h"
#include "libClient/displayBase.h"
#include "libClient/event.h"
#include "libClient/gainLevel.h"
#include "libClient/gameControl.h"
#include "libClient/gameInfo.h"
#include "libClient/graphicalArtefact.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/lord.h"
#include "libClient/lordExchange.h"
#include "libClient/mapCreature.h"
#include "libClient/mapView.h"
#include "libClient/miniMap.h"


extern QString DATA_PATH;
extern QString VERSION;
extern TechnicList techList;
extern Map * theMap;

	Game::Game( QWidget * parent, AttalSocket * socket )
: QWidget( parent, Qt::MSWindowsFixedSizeDialogHint ),
	GraphicalGameData()
{
	try{
		theMap = new Map( this );
		_map = (GenericMap *)theMap;
		//_player is GenericPlayer, we use Player instead using casting ( getGamePlayer () ) cause graphical functions are present in player
		_player =(GenericPlayer *) new Player( _map );
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}

	_isPlaying = false;
	_currentCell = NULL;
	_period = 40;
	_idTime = -1;
	_scenDesc = "";

	initWidgets();
	_socket = socket;

	getGamePlayer()->setConnectionName( AttalSettings::getInstance()->getStrategyModeSettings().connectionName );
	ScrollLord * scrLord = this->findChild<ScrollLord *>("AttalGameScrollLord");
	ScrollBase * scrBase = this->findChild<ScrollBase *>("AttalGameScrollBase");
	GameControl * control = this->findChild<GameControl *>("AttalGameControl");
	MiniMap * miniMap = this->findChild<MiniMap *>("AttalGameMiniMap");
	MapView * view = this->findChild<MapView *>("AttalGameMapView");
	ChatWidget * chat = this->findChild<ChatWidget *>("AttalGameChat");

	scrLord->reinit();
	scrBase->reinit();

	GraphicalGameData::reinit();
	control->reinit();

	connect( chat, SIGNAL( sig_message( QString ) ), SLOT( slot_message( QString ) ) );

	connect( scrLord, SIGNAL( sig_lord() ), SLOT( slot_displayLord() ) );
	connect( scrLord, SIGNAL( sig_lordSelected( GenericLord * ) ), SLOT( slot_lordSelected( GenericLord * ) ) );
	connect( scrLord, SIGNAL( sig_unSelectLord() ), SLOT( unSelectLord() ) );

	connect( scrBase, SIGNAL( sig_unSelectLord() ), SLOT( unSelectLord() ) );
	connect( scrBase, SIGNAL( sig_base() ), SLOT( slot_displayBase() ) );
	connect( scrBase, SIGNAL( sig_baseSelected() ), SLOT( slot_baseSelected() ) );

	connect( control, SIGNAL( sig_endTurn() ), SLOT( endTurn() ) );
	connect( control, SIGNAL( sig_gotoLord() ), SLOT( gotoLord() ) );
	connect( control, SIGNAL( sig_nextLord() ), SLOT( nextLord() ) );

	connect( view , SIGNAL( sig_mouseMoved  ( GenericCell * ) ), SLOT( slot_mouseMoved( GenericCell * ) ) );
	connect( view , SIGNAL( sig_mouseLeftPressed( GenericCell * ) ), SLOT( slot_mouseLeftPressed( GenericCell * ) ) );
	connect( view , SIGNAL( sig_mouseRightPressed( GenericCell * ) ), SLOT( slot_mouseRightPressed( GenericCell * ) ) );
	connect( view, SIGNAL( sig_mouseReleased() ), SLOT( slot_mouseReleased() ) );
	connect( view, SIGNAL( sig_viewportResized( double, double ) ), miniMap, SLOT( slot_mapviewResized( double, double ) ) );
	connect( view, SIGNAL( sig_viewportScrolled( double, double ) ), miniMap, SLOT( slot_mapviewScrolled( double, double ) ) );
	connect( this, SIGNAL( sig_enableGame( bool ) ), view, SLOT( slot_enableGame( bool ) ) );
	
	/* code to center the map on the cell specified */
	connect( this , SIGNAL( sig_Center( int , int ) ), view , SLOT( slot_Center( int ,int ) ) );
	connect( this , SIGNAL( sig_Center( GenericCell *  ) ), view , SLOT( slot_Center( GenericCell * ) ) );

	connect( this , SIGNAL( sig_clearPath()), theMap, SLOT( clearPath()) );

	connect( theMap , SIGNAL( sig_cellChanged( int, int )), miniMap, SLOT( redrawCell( int , int ) ) );
	connect( miniMap , SIGNAL( sig_mouseReleasedMinimap( double, double ) ), view, SLOT( slot_Center( double, double ) ) );
}

Game::~Game()
{
	TRACE("~Game");

}

void Game::reinit()
{
	TRACE("Game::reinit");

	stopTimer();
	/* disable controls */
	emit sig_enableGame( false );
	
	ScrollLord * scrLord = this->findChild<ScrollLord *>("AttalGameScrollLord");
	ScrollBase * scrBase = this->findChild<ScrollBase *>("AttalGameScrollBase");
	scrLord->deselect();
	scrBase->deselect();

	/* clean up player data */
	getGamePlayer()->cleanData();
	_state = MS_NOTHING;
	/* redraw various widgets  */

	GraphicalGameData::reinit();

	/* redraw minimap */
	emit sig_redrawMap( theMap );

	/* these are to remove icon from buttons */
	emit sig_baseReinit();
	emit sig_lordReinit();
	emit sig_statusBar();

	ImageTheme.endMusic();

}

void Game::initWidgets()
{
	try{
		theMap->setSceneRect( 0,0, 4000, 3000 );

		MapView * view = new MapView( theMap, this );
		view->setObjectName( QString("AttalGameMapView") );

		GameControl * control = new GameControl( this, getGamePlayer() );
		control->setObjectName( QString("AttalGameControl"));
		ScrollLord * scrLord = new ScrollLord( false, 4, this, getGamePlayer() );
		scrLord->setObjectName( QString("AttalGameScrollLord"));
		ScrollBase * scrBase = new ScrollBase( false, 4, this, getGamePlayer() );
		scrBase->setObjectName( QString("AttalGameScrollBase"));

		QVBoxLayout * layControlV1 = new QVBoxLayout();	
		layControlV1->setObjectName( QString("layControlV1") );
		layControlV1->addWidget( scrLord, 1, Qt::AlignHCenter );
		layControlV1->addWidget( control, 0, Qt::AlignHCenter );
		layControlV1->addWidget( scrBase, 1, Qt::AlignHCenter );

		QHBoxLayout * layH1 = new QHBoxLayout();
		layH1->setObjectName( QString("layH1") );
		layH1->addWidget( view, 1 );
		layH1->addLayout( layControlV1 );

		ChatWidget * chat = new ChatWidget( this );
		chat->setObjectName( QString("AttalGameChat"));

		MiniMap * miniMap = new MiniMap( theMap, this );
		miniMap->setObjectName( QString("AttalGameMiniMap"));

		view->setScrollBarVisible( false );

		QHBoxLayout * layH2 = new QHBoxLayout();
		layH2->setObjectName( QString("layH2") );
		layH2->addWidget( chat, 1 );
		layH2->addWidget( miniMap );

		QVBoxLayout * layout = new QVBoxLayout( this );
		layout->setObjectName(QString("mainLayout"));
		layout->addLayout( layH1 );
		layout->addLayout( layH2 );
		layout->activate();
		
		AttalPopup * popup = new AttalPopup( this );
		popup->setObjectName( QString("AttalGamePopup"));
		popup->hide();
	
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void Game::setPlayerName( QString name ) 
{
	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();
	settings.connectionName = name;
	getGamePlayer()->setConnectionName( name );
	AttalSettings::getInstance()->setStrategyModeSettings( settings );
}

void Game::slot_mouseMoved( GenericCell *cell )
{
	QString msg;

	if( cell != _currentCell ) {
		if( cell->getLord() ) {
			msg = tr( "Lord " ) + cell->getLord()->getName();
			setCursor( Qt::WaitCursor );
		} else if( cell->getBase() ) { 
			msg = cell->getBase()->getBaseDescription();
			setCursor( Qt::WaitCursor );
		} else if( cell->getBuilding() ) {
			msg = cell->getBuilding()->getName()+ QString(tr(" - ")) + DataTheme.buildings.at(cell->getBuilding()->getType())->getDescription();
			setCursor( Qt::WaitCursor );
		} else if( cell->getEvent() ) {
			setCursor( Qt::WaitCursor );
			switch(cell->getEvent()->getType()){
				case GenericEvent::EventNone:
					break;
				case GenericEvent::EventArtefact:
					msg =  tr("Artefact: ") + cell->getEvent()->getArtefact()->getName();
					break;
				case GenericEvent::EventBonus: {
																				 GenericBonus * bonus = cell->getEvent()->getBonus();
																				 msg = bonus->getBonusDescription();
																			 } break;
				case GenericEvent::EventChest:
																			 msg =  tr("Chest ");
																			 break;
				default:
																			 break;
			}
		} else if( cell->getCreature() ) {
			msg =  tr("About ") + QString::number(DataTheme.getRandomInCategory(cell->getCreature()->getCategoryNumber()))  + "  " + cell->getCreature()->getCreature()->getName();
			setCursor( Qt::WaitCursor );
		} else {
			setCursor( Qt::ArrowCursor );
#ifdef QT_DEBUG
			msg = QString(tr("Cell: row %1, col %2, coordinates x %3, y %4,")).arg(cell->getRow()).arg(cell->getCol()).arg(cell->getCol() * DataTheme.tiles.getWidth()).arg( cell->getRow() * DataTheme.tiles.getHeight());
			msg += QString(tr(" type %1, coeff %2, stoppable %3 ")).arg( cell->getType()).arg(cell->getCoeff() ).arg( cell->isStoppable() );
#endif
		}
		emit sig_statusMsg( msg );
	}

	//Code set aside for future use (the path follow the cursor, working but seem annoying
	/* 	switch( _state ) {
	 * 		case MS_LORD:
	 * 			{
	 * 				GenericLord * selectedLord = getGamePlayer()->getSelectedLord();
	 * 				GraphicalPath * gpath = theMap->getGraphicalPath();
	 * 				if( selectedLord ) {
	 * 					gpath->computePath( selectedLord->getCell(), cell, selectedLord->getCharac( MOVE ) ,selectedLord->getCharac( MAXMOVE ));
	 * 					selectedLord->setDestination( cell );
	 * 				}
	 * 			}
	 * 		default:
	 * 			break;
	 * 	}
	 */
}

void Game::slot_mouseLeftPressed( GenericCell * cell )
{
	AttalPopup * popup = this->findChild<AttalPopup *>("AttalGamePopup");

	if( popup && popup->isVisible() ) {
		popup->hide();
		popup->setType();
		return;
	}

	if( !getGamePlayer() ) {
		return;
	}

	switch( _state ) {
		case MS_NOTHING:
			handleClickNothing( cell );
			break;
		case MS_LORD:
			handleClickLord( cell );
			break;
		case MS_BASE:
			handleClickBase( cell );
			break;
		case MS_TECHNIC:
			break;
		default:
			break;
	}
}

void Game::slot_mouseRightPressed( GenericCell * cell )
{
	if( !getGamePlayer() ) {
		return;
	}

	AttalPopup * popup = this->findChild<AttalPopup *>("AttalGamePopup");
	if( !popup ) {
		popup = new AttalPopup( this );
		popup->setObjectName( QString("AttalGamePopup"));
	}

	MapView * view = this->findChild<MapView *>("AttalGameMapView");

	QPoint point = (QPoint)(view->mapFromScene(cell->getCol() * DataTheme.tiles.getWidth(), cell->getRow() * DataTheme.tiles.getHeight()));

	GenericLord * lord = cell->getLord();
	QString descr = "?";

	if( lord ) {
		descr = lord->getLordDescription();
	}

	GenericBuilding * build = cell->getBuilding();

	if( build ) {
		descr = build->getBuildDescription();
	}

	GenericBase * base = cell->getBase();

	if( base ) {
		descr = base->getBaseDescription();
	}
	
	popup->setText( descr );

	GenericEvent * event = cell->getEvent();

	if( event ) {
		popup->setEvent( event );
	}

	GenericMapCreature * crea = cell->getCreature();

	if( crea ) {
		popup->setCreature( crea );
	}

	popup->move( point );
	popup->show();
}

void Game::slot_mouseReleased()
{

}

void Game::setState( MapState state )
{
	_state = state;
}

void Game::handleClickNothing( GenericCell * cell )
{
	GenericLord * lord = cell->getLord();

	if( lord ) {
		if( getGamePlayer()->hasLord( lord ) ) {
			slot_lordSelected( lord );
		}
		return;
	}

	GenericBase * base = cell->getBase();

	if( base ) {
		if( getGamePlayer()->hasBase( base ) ) {
			getGamePlayer()->setSelectedBase( base );
			slot_baseSelected();
		}
		return;
	}
}

void Game::handleClickLord( GenericCell * cell )
{
	if( !cell ) {
		return;
	}

	TRACE("Game::handleClickLord row %d col %d", cell->getRow(), cell->getCol() );

	GenericLord * selectedLord = getGamePlayer()->getSelectedLord();

	if( !selectedLord ) {
		return;
	}

	if( selectedLord->isGarrison() ) {
		return;
	}
		
	GraphicalPath * gpath = theMap->getGraphicalPath();

	if( selectedLord && ( cell->getCoeff() >= 0 ) ) {
		if(cell->getLord()){
			emit sig_Center( cell );
		}
		if( ((Lord*)selectedLord)->getDestination() != cell ) {
			computePath( selectedLord, cell );
			selectedLord->setDestination( cell );
		} else {
			if( selectedLord->getCell()->getBuilding() ) {
				selectedLord->getCell()->getBuilding()->out( selectedLord );
			}
			QList<GenericCell *> list = gpath->followPath( selectedLord->getCell(), selectedLord->getCharac( MOVE )) ;
			_socket->sendMvts( selectedLord->getId(), list );
		}
	}		
}

void Game::handleClickBase( GenericCell * cell )
{
	GenericBase * base = cell->getBase();

	if( base ) {
		if( getGamePlayer()->hasBase( base ) ) {
			if( base == getGamePlayer()->getSelectedBase() ) {
				emit sig_base( base );
				return;
			} else {
				getGamePlayer()->setSelectedBase( base );
				emit sig_baseReinit();
				emit sig_lordReinit();
				return;
			}
		} else {
			return;
		}
	}	

	handleClickNothing( cell );
}

void Game::enter( GenericLord * /*lord*/, GenericBuilding * /*building*/ )
{
	/// XXX: TODO
	//logEE( "not yet implemented" );
}

void Game::enter( GenericLord * lord, GenericBase * base )
{
	TRACE("Game::enter lord %p base %p", lord, base);

	if( base ) {
		base->enter( lord );
		emit sig_base( base );
	}
}

void Game::beginTurn()
{
	_isPlaying = true;
	emit sig_statusBar();

	ImageTheme.playSound( AttalSound::SND_NEWTURN );
	_state = MS_NOTHING;

	ScrollLord * scrLord = this->findChild<ScrollLord *>("AttalGameScrollLord");
	ScrollBase * scrBase = this->findChild<ScrollBase *>("AttalGameScrollBase");

	if( scrLord->getListCount() > 0 ) {
		scrLord->select( 0 );
	} else if( scrBase->getListCount() > 0 ) {
		scrBase->select( 0 );
	}

	emit sig_beginTurn();

	if( _calendar->getDay() == 1 ) {
		QString dayName = _calendar->getDayName();
		//QString mess =  tr( "It is") + " " + dayName + tr(", a new week is beginning");
		//XXX: insert in the future
		QString mess =  QString("\n\n") + tr( "It is") + " " + dayName + tr(", a new week is beginning") + "\n\n";
		AttalPopup * popup = this->findChild<AttalPopup *>("AttalGamePopup");
		popup->setText( mess );
		popup->centerParent();
		popup->show();
		//QMessageBox::information( this, dayName, mess);
	}
}

void Game::playerActive( char /* num */ )
{
	//_gameInfo->waitPlayer( num );
}

void Game::nextLord()
{ 
	TRACE("Game::nextLord");

	GenericLord * lord = getGamePlayer()->nextLord(); 
	if( lord ) {
		slot_lordSelected( lord );
	}
}

void Game::nextBase()
{ 
	getGamePlayer()->nextBase();
	slot_baseSelected(); 
}

void Game::gotoLord()
{
	GenericLord * lord =  getGamePlayer()->getSelectedLord();
	
	if( lord ) {
		handleClickLord( lord->getDestination() );
	}

}

void Game::endTurn()
{
	assert( _socket );
	if( _isPlaying ) {
		if( getGamePlayer()->shouldEnd() ) {
			sendEndTurn();
		} else {
			int result = QMessageBox::warning( NULL, tr("Are you sure ?"), tr( "One or more heroes may still move. Are you sure you want to end your turn ?"), QMessageBox::Yes | QMessageBox::No , QMessageBox::No ); 
			if ( result == QMessageBox::Yes ) {
				sendEndTurn();
			}
		}
	}
}

void Game::sendEndTurn()
{
	_isPlaying = false;
	_socket->sendTurnEnd();
}

void Game::socketGameBegin()
{
	int nb = _socket->readChar();
	
	TRACE("Game::beginGame int nb %d", nb);

	restartTimer();

	setPlayerNumber( nb );

	/*enable some widgets */	
	emit sig_enableGame( true );

	ImageTheme.playMusicMap();
}

void Game::endGame()
{
	TRACE("Game::endGame");

	reinit();
}

void Game::handleSocket()
{
	switch( getCla1() ) {
		case SO_MSG:
			socketMsg();
			break;
		case SO_GAME:
			socketGame();
			break;
		case SO_TURN:
			socketTurn();
			break;
		case SO_MODIF:
			socketModif();
			break;
		case SO_QR:
			socketQR();
			break;
		case SO_MVT:
			socketMvt();
			break;
		case SO_TECHNIC:
			break;
		case SO_EXCH:
			socketExchange();
			break;
		case SO_CONNECT:
			socketConnect();
			break;
		case SO_FIGHT:
			socketFight();
			break;
		default:
			logEE( "Unknown socket_class" );
			printSocket;
			break;
	}
}

void Game::socketMsg()
{
	int cla2 = getCla2();
	QString msg;
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		msg[i] = _socket->readChar();
	}

	if( cla2 == C_MSG_FIGHT ) {
		return;
	}

	emit sig_newMessage( msg );
}

void Game::slot_gameInfo()
{
	QString desc = tr("Objective:") + " " + _scenDesc + " \n" + getGamePlayer()->getAutoDescription();
	QMessageBox::information( this, tr("Information"), desc );
}

void Game::slot_message( QString msg )
{
	if( _socket ) {
		_socket->sendMessage( getGamePlayer()->getConnectionName() + " : " + msg );
	} else {
		emit sig_newMessage( "(Not connected) : " + msg );
	}
}

void Game::slot_sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 )
{
	_socket->sendExchangeUnit( lord1, unit1, lord2, unit2 );
}

void Game::slot_sendExchangeUnitSplit( GenericLord * lord, int unit, int num1, int pos2, int num2  )
{
	_socket->sendExchangeUnitSplit( lord, unit, num1, pos2, num2  );
}

void Game::slot_sendLordUnit( GenericLord * lord, int num, int value )
{
	_socket->sendLordUnit( lord, num, value );
}

void Game::slot_sendExchangeArtefact( GenericLord * lord1, int item, GenericLord * lord2 )
{
	_socket->sendExchangeArtefact( lord1, item, lord2 );
}


void Game::socketGame()
{
	switch( getCla2() ) {
		case C_GAME_BEGIN:
			socketGameBegin();
			/// XXX: clear old stuff if necessary
			break;
		case C_GAME_LOST:
			socketGameLost();
			break;
		case C_GAME_WIN:
			socketGameWin();
			break;
		case C_GAME_END:
			emit sig_endGame();
			break;
		case C_GAME_INFO:
			socketGameInfo();
			break;
		case C_GAME_CALENDAR:
			socketGameCalendar();
			break;
		case C_GAME_TAVERN:
			socketGameTavern();
			break;
		default:
			logEE( "case not handled" );
			printSocket;
			break;
	}
}

void Game::socketGameLost()
{
	TRACE( "socketGameLost" );

	QString text;
	AttalMessage msg;
	int nb = _socket->readChar();
	
	msg.setWindowTitle( tr("A player has lost.") );
	msg.addPixmap( ImageTheme.getFlag( nb ) );

	if( nb == getGamePlayer()->getNum() ) {
		msg.addText(tr( "You lose") );
		msg.exec();
		emit sig_result( false );
	} else {
		text = tr("Player ") + QString::number( nb ) + tr(" has lost.");
		msg.addText( text );
		msg.exec();
	}
}

void Game::socketGameWin()
{
	QString text;
	AttalMessage msg;

	int nb = _socket->readChar();

	msg.setWindowTitle( tr("A player has Win.") );
	msg.addPixmap( ImageTheme.getFlag( nb ) );

	if( nb == getGamePlayer()->getNum() ) {
		msg.addText( tr("You  win !!") );
		msg.exec();
		emit sig_result( true );
	} else {
		text = tr("Player ") + QString::number( nb ) + tr(" has win.");
		msg.addText( text + tr(" You lose") );
		msg.exec();
		//emit sig_result( false );
	}
}

void Game::socketGameInfo()
{
	switch( getCla3() ) {
		case C_INFOPLAYER_TEAM: 
			socketGameInfoTeam();
			break;
		case C_INFOPLAYER_NAME:
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketGameInfoTeam()
{
	uchar player = _socket->readChar();	
	uchar teamId = _socket->readChar();

	TRACE("Game::GameInfoTeam player %d, teamId %d", player, teamId);

	if( getPlayer( player ) ) {
		getPlayer( player )->setTeam( teamId );
	}
	if( player == getGamePlayer()->getNum() ) {
		getGamePlayer()->setTeam( teamId );
	}
}

void Game::socketGameCalendar()
{
	for( int i = 0; i < 5; i++ ) {
	  _calendar->setDateByType( i , _socket->readInt());
	}

	emit sig_statusBar();
}

void Game::socketGameTavern()
{
	switch( getCla3() ) {
		case C_TAVERN_INFO:
			socketGameTavernInfo();
			break;
		case C_TAVERN_LORD:
			socketGameTavernLord();
			break;
		default:
			logEE( "Should not happen" );
		}
}

void Game::socketGameTavernInfo() 
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uint nbLord = _socket->readChar();

	GenericBase * base = NULL;
	try {
		base =	_map->at( row, col )->getBase();
	} catch ( const char * err ) {
		logEE("%s", err);
		return;
	}
	if( base ) {
		emit sig_tavernInfo( nbLord );
	}
}

void Game::socketGameTavernLord() 
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uint idLord = _socket->readInt();

	bool enable = false;
	GenericBase * base = NULL;
	try {
		base =	_map->at( row, col )->getBase();
	} catch ( const char * err ) {
		logEE("%s", err);
		return;
	}
	if( base ) {
		if( getGamePlayer()->canBuy(DataTheme.lords.at( idLord )) && !base->isVisitorLord()) {
			enable = true;
		}
		emit sig_tavernLord( idLord, enable );
	} else {
		logEE( "Base not found" );
	}

}

void Game::socketTurn()
{
	switch( getCla2() ) {
		case C_TURN_PLAY:
			socketTurnPlay();
			break;
		case C_TURN_LORD:
			logEE( "Should not happen (Client : SO_TURN/C_TURN_LORD)" );
			break;
		case C_TURN_PLORD:
			logEE( "Should not happen (Client : SO_TURN/C_TURN_PLORD)" );
			break;
		case C_TURN_END:
			logEE( "Should not happen (Client : SO_TURN/C_TURN_END)" );
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketTurnPlay()
{
	TRACE("Game::socketTurnPlay");

	uint num = _socket->readChar();
	if( num == (uint)getGamePlayer()->getNum() ) { // casting from int to uint -- getNum,setNum, and _num should all be using uint's
		beginTurn();
	} else {
		//useful only for gameInfo, now obsolete
		//playerActive( num );
	}
}

void Game::socketMvt()
{
	// XXX: not finished at all

	TRACE("Game::socketMvt");

	if( getCla2() == C_MVT_ONE ) {
		uchar lord = _socket->readChar();
		int row = _socket->readInt();
		int col = _socket->readInt();
		GenericCell * newCell;

		TRACE("Game::socketMvt lord %d, row %d, col %d", lord, row, col);

		try {
			newCell = _map->at( row, col );
		} catch ( const char * err ) {
			logEE("%s", err);
			return;
		}

		GenericLord * theLord = _lords.at( lord );

		if( theLord ) {
			GenericCell * oldCell = theLord->getCell();

			theLord->setCell( newCell );

			if( oldCell ) {
				int oldRow = oldCell->getRow();
				int oldCol = oldCell->getCol();
				emit sig_cellChanged( oldRow, oldCol );
			} else {
				logEE("Should be a cell");
			}

			emit sig_Center( row, col );

			if( newCell->getBuilding() != 0 ) {
				if( theLord->getOwner() == getGamePlayer() ) {
					enter( getGamePlayer()->getSelectedLord(), newCell->getBuilding() );
				}
			} else if( newCell->getBase() != 0 ) {
				if( theLord->getOwner() == getGamePlayer() ) {
					TRACE("Game::socketMvt: EnterBase");
					enter( getGamePlayer()->getSelectedLord(), newCell->getBase() );
				}
			}
			emit sig_cellChanged( row, col );
		} else {
			logEE( "Try to move a non-existent lord %d to cell (%d, %d)", lord, row, col );
		}
	} else {
		logEE( "Should not happen" );
	}
}

void Game::socketExchange()
{
	switch( getCla2() ) {
		case C_EXCH_START: 
			exchangeStart();
			break;
		case C_EXCH_ARTEFACT:
			exchangeArtefact();
			break;
		default:
			printSocket;
			break;
	}
}

void Game::exchangeStart()
{
	uchar idLord1 = _socket->readChar();
	uchar idLord2 = _socket->readChar();
	GenericLord * lord1 = (GenericLord * ) _lords.at( idLord1 );
	GenericLord * lord2 = (GenericLord * ) _lords.at( idLord2 );

	LordExchange * exchange = this->findChild<LordExchange *>("AttalLordExchange");

	if( !exchange ) {
		exchange = new LordExchange( this, lord1, lord2 );
		exchange->setObjectName( QString("AttalLordExchange"));
	} else {
		exchange->initLords( lord1, lord2 );
	}

	exchange->show();

}

void Game::exchangeArtefact()
{
	uchar idLord1 = _socket->readChar();
	int item = _socket->readInt();
	uchar idLord2 = _socket->readChar();

	exchangeArtefactLord( idLord1, idLord2, item);

	emit sig_updateWidget();
}


void Game::socketModif()
{
	/// XXX: check number of args below... (?)

	switch( getCla2() ) {
		case C_MOD_MAP:
			socketModifMap();
			break;
		case C_MOD_CELL:
			socketModifCell();
			break;
		case C_MOD_LORD:
			socketModifLord();
			break;
		case C_MOD_PLAYER:
			socketModifPlayer();
			break;
		case C_MOD_BASE:
			socketModifBase();
			break;
		case C_MOD_BUILD:
			socketModifBuilding();
			break;
		case C_MOD_ARTEFACT:
			socketModifArtefact();
			break;
		case C_MOD_CREATURE:
			socketModifCreature();
			break;
		case C_MOD_EVENT:
			socketModifEvent();
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketModifMap()
{
	int h = _socket->readInt();
	int w = _socket->readInt();
	TRACE("Game::socketModifMap h/w %d/%d", h, w );
	MiniMap * miniMap = this->findChild<MiniMap *>("AttalGameMiniMap");
	MapView * view = this->findChild<MapView *>("AttalGameMapView");
	_map->newUnknownMap( h, w );
	emit sig_redrawMap( theMap );
	miniMap->slot_mapviewResized( view->width() / theMap->width(), view->height() / theMap->height() ); // HACK
	layout()->update();
}

void Game::socketModifCell()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int type = _socket->readInt();
	uchar diversification = _socket->readChar();
	int a4 = _socket->readInt();
	int a5 = _socket->readInt();
	int a6 = _socket->readInt();
	int a7 = _socket->readInt();
	//TRACE("Game::socketModifCell row %d, col %d, type %d, diversification %d, a4 %d, a5 %d, a6 %d, a7 %d", row, col, type, diversification, a4, a5, a6, a7 );
	_map->changeCell( row, col, type, a4, a5, a6, a7 , diversification );
}

void Game::socketModifLord()
{
	switch( getCla3() ) {
		case C_LORD_VISIT:
			socketModifLordVisit();
			break;
		case C_LORD_NEW:
			socketModifLordNew();
			break;
		case C_LORD_CHARAC:
			socketModifLordCharac();
			break;
		case C_LORD_UNIT:
			socketModifLordUnit();
			break;
		case C_LORD_REMOVE:
			socketModifLordRemove();
			break;
		case C_LORD_GARRISON:
			socketModifLordGarrison();
			break;
		case C_LORD_MACHINE:
			socketModifLordMachine();
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketModifLordVisit()
{
	/// XXX: not finished, we should use 'num' for the player color
	uchar num = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();
	uchar present = _socket->readChar();

	Lord * lord = (Lord *)  modifLordVisit( row, col, id, num, present );
	if( present == 1 ) {
		lord->setAnimated( true );
		lord->setEnabled( true );
		lord->setSelected( true );
		//lord->setActive( true );
	}
}

void Game::socketModifLordNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();

	TRACE("Game::socketModifLordNew row %d, col %d, id %d", row, col ,id );

	Lord * lord = (Lord *) modifLordNew( row, col, id );

	lord->setAnimated( true );
	lord->setEnabled( true );
	lord->setVisible( true );
	lord->setSelected( true );
	//lord->setActive( true );

	GenericBase * base = _map->at( row, col )->getBase();
	if( base && base->getOwner() == getGamePlayer() ) {
		enter( lord , base );
	}
	emit sig_lordReinit();
	emit sig_baseUpdate();
	emit sig_cellChanged( row, col );
}

void Game::socketModifLordUnit()
{
	uchar id = _socket->readChar();
	uchar pos = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();
	uchar move = _socket->readChar();
	int health = _socket->readInt();

	modifLordUnit( id , pos ,race ,level, nb,move, health);

	emit sig_updateWidget();
	emit sig_baseUpdate();
}

void Game::socketModifLordRemove()
{
	int idLord = _socket->readChar();
	TRACE("Game::socketModifLordRemove idLord %d", idLord );
	Lord * lord = ( Lord *)_lords.at(  idLord );
	if(lord) {
		GenericCell * cell = lord->getCell();
		lord->removeFromGame();
		if( getGamePlayer()->numLord() == 0  ) {
			 _state = MS_NOTHING;
		}
		emit sig_lordReinit();
		emit sig_clearPath();	
		lord->setVisible( false );
		if( cell ) {
			emit sig_cellChanged( cell->getRow(), cell->getCol() );
		}
	} else {
		logEE("Game::socketModifLordRemove error lord not exist idLord %d", idLord);
	}
}

void Game::socketModifLordCharac()
{
	char idLord = _socket->readChar();
	char charac = _socket->readChar();
	int nb = _socket->readInt();
	
	TRACE("Game::socketModifLordCharac idLord %d, charac %s, nb %d", idLord, qPrintable(getCharacName( (LordCharac) charac)), nb  );

	modifLordCharac( idLord , charac, nb );

}

void Game::socketModifLordGarrison()
{
	uchar lord = _socket->readChar();
	uchar state = _socket->readChar();
	if(_lords.at( lord )) {
		_lords.at( lord )->setVisible( state != 1 );
	}

	emit sig_lordReinit();
}

void Game::socketModifLordMachine()
{
	uchar lord = _socket->readChar();
	uchar id = _socket->readChar();
	if(_lords.at( lord )) {
		_lords.at( lord )->addMachine( id );
	}
}

void Game::socketModifPlayer()
{
	switch( getCla3() ) {
		case C_PLAY_RESS: 
			{
				socketModifRess( getGamePlayer() , _socket);
				emit sig_statusBar();
				emit sig_baseUpdate();
			}
			break;
		case C_PLAY_PRICE: 
			{
				uchar ress = _socket->readChar();
				uint price = _socket->readInt();
				getGamePlayer()->getPriceMarket()->setResourcePrice( ress, price);
			}
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketModifBase()
{
	switch( getCla3() ) {
		case C_BASE_NEW:
			socketModifBaseNew();
			break;
		case C_BASE_OWNER:
			socketModifBaseOwner();
			break;
		case C_BASE_NAME:
			socketModifBaseName();
			break;
		case C_BASE_BUILDING:
			socketModifBaseBuilding();
			break;
		case C_BASE_UNIT:
			socketModifBaseUnit();
			break;
		case C_BASE_RESS:
			socketModifBaseResources();
			break;
		case C_BASE_PRODUCTION:
			socketModifBaseProduction();
			break;
		case C_BASE_STATE:
			socketModifBaseState();
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketModifBaseNew()
{
	uchar race = _socket->readChar();
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar id = _socket->readChar();
	int nb = _socket->readChar();
	QList<uchar> forbidlist;

	for( int i = 0; i < nb; i++ ) {
		forbidlist.append( _socket->readChar() );
	}

	getNewBase( race, row, col, id, nb, forbidlist);

	emit sig_cellChanged( row, col );
}

void Game::socketModifBaseOwner()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar playerNum = _socket->readChar();

	TRACE("Game::socketModifBaseOwner row %d, col  %d, player num %d", row,col,playerNum);

	modifBaseOwner( row, col, playerNum );
	
	emit sig_cellChanged( row, col );
	emit sig_baseReinit();
}

void Game::socketModifBaseName()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uint length = _socket->readInt();
	QString name;

	for( uint i = 0; i < length; i++ ) {
		name[i] = _socket->readChar();
	}

	modifBaseName( row, col, name );
}

void Game::socketModifBaseBuilding()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar level = _socket->readChar();
	bool create = (bool)_socket->readChar();

	modifBaseBuilding( row, col, level,create );
	
	emit sig_modifBaseBuilding( row, col, level,create );
}

void Game::socketModifBaseUnit()
{
	TRACE("Game::socketModifBaseUnit");

	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar pos = _socket->readChar();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();

	modifBaseUnit( row, col, pos, race , level, number );

	emit sig_baseUpdate();
}

void Game::socketModifBaseResources()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	TRACE("Game::socketModifBaseResources row %d, col  %d", row,col );

	try {
		socketModifRess(_map->at( row, col )->getBase() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}

	emit sig_statusBar();
}

void Game::socketModifBaseProduction()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int number = _socket->readInt();

	TRACE("Game::socketModifBaseProduction row %d, col  %d, race %d, level %d, number %d", row,col, race, level, number );

	modifBaseProduction( row, col, race , level, number );

	emit sig_baseUpdate();

}

void Game::socketModifBaseState()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int  type = _socket->readInt();
	bool state = _socket->readChar();

	modifBaseState( row, col, type,	state );

	emit sig_baseUpdate();
	
}

void Game::socketModifBuilding()
{
	switch( getCla3() ) {
		case C_BUILD_NEW: 
			socketModifBuildingNew();
			break;
		case C_BUILD_OWNER: 
			socketModifBuildingOwner();
			break;
		case C_BUILD_RESS:
			socketModifBuildingResources();
			break;
		default:
			printSocket;
			break;
	}

}

void Game::socketModifBuildingNew()
{
	uchar type = _socket->readChar();
	int id = _socket->readInt();
	int row = _socket->readInt();
	int col = _socket->readInt();

	getNewBuilding( type, id , row, col);
	emit sig_cellChanged( row, col );
}

void Game::socketModifBuildingOwner()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int playNum = _socket->readInt();

	TRACE("Game::socketModifBuildingOwner row %d, col  %d,player num %d", row,col,playNum);

	modifBuildingOwner( row, col,  playNum );
	
	emit sig_cellChanged( row, col );
}

void Game::socketModifBuildingResources()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	TRACE("Game::socketModifBuildingResources row %d, col  %d", row,col);

	try {
		socketModifRess( _map->at( row, col )->getBuilding() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void Game::socketModifArtefact()
{
	switch( getCla3() ) {
		case C_ART_DELLORD: 
			{
				uint type = _socket->readInt();
				char lord = _socket->readChar();
				_lords.at( lord )->getArtefactManager()->removeArtefactByType( type );
			}
			break;
		case C_ART_ADDLORD: 
			{
				ImageTheme.playSound( AttalSound::SND_GOOD );
				uint type = _socket->readInt();
				char lord = _socket->readChar();
				if( ! getLord( lord )->getArtefactManager()->hasArtefactType( type ) ) {
					_lords.at( lord )->getArtefactManager()->addArtefact( type );
				}
			}
			break;
	}
}

void Game::socketModifCreature()
{
	switch( getCla3() ) {
		case C_CRE_NEW: 
			socketModifCreatureNew();
			break;
		case C_CRE_UPDATE:
			socketModifCreatureUpdate();
			break;
		case C_CRE_DEL:
			socketModifCreatureRemove();
			break;
		case C_CRE_RESS: 
			socketModifCreatureRess();
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketModifCreatureNew()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	uchar race = _socket->readChar();
	uchar level = _socket->readChar();
	int nb = _socket->readInt();
	bool looking = (bool) _socket->readChar();

	getNewMapCreature( row, col, race, level, nb, looking);

}

void Game::socketModifCreatureUpdate()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	int nb = _socket->readInt();
	GenericMapCreature * creature = _map->at( row, col )->getCreature();
	if( creature ) {
		creature->setCategoryNumber( nb );
	}
}

void Game::socketModifCreatureRemove()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	removeMapCreature( row, col );
}

void Game::socketModifCreatureRess()
{
	int row = _socket->readInt();
	int col = _socket->readInt();
	
	try {
		socketModifRess( _map->at( row, col )->getCreature() , _socket);
	} catch ( const char * err ) {
		logEE("%s", err);
	}
}

void Game::socketModifEvent()
{
	switch( getCla3() ) {
		case C_EVENT_NEW:
			socketEventNew();
			break;
		case C_EVENT_DEL: 
			socketEventDel();
			break;
	}
}

void Game::socketEventNew()
{
	uint i;
	int row = _socket->readInt();
	int col = _socket->readInt();
	GenericEvent::EventType type = (GenericEvent::EventType) _socket->readChar();

	GenericEvent * event = NULL;

	if( type == GenericEvent::EventArtefact ) {
		int id = _socket->readInt();
		uchar typeArtefact = _socket->readChar();

		event = getNewArtefact( id );
		GenericArtefact * artefact = event->getArtefact();
		artefact->setType( typeArtefact );
	} else if( type == GenericEvent::EventBonus ) {
		uchar typeBonus = _socket->readChar();
		uchar nbParam = _socket->readChar();

		event = getNewBonus();
		GenericBonus * bonus = event->getBonus();
		bonus->setType( (GenericBonus::BonusType) typeBonus );
		for( i = 0; i < nbParam; i++ ) {
			bonus->addParam( _socket->readInt() );
		}
		bonus->setupBonus();
	} else if( type == GenericEvent::EventChest ) {
		uchar nbParam = _socket->readChar();

		event = getNewChest();
		GenericChest * chest = event->getChest();
		for( i = 0; i < nbParam; i++ ) {
			chest->addParam( _socket->readInt() );
		}
		dynamic_cast<Chest*>(chest)->setupChest();
	}

	try {
		event->setCell( _map->at( row, col ) );
		_map->at( row, col )->setEvent( event );
	} catch ( const char * err ) {
		logEE("%s", err);
		return;
	}
}

void Game::socketEventDel()
{
	int row = _socket->readInt();
	int col = _socket->readInt();

	removeEvent( row, col );
}

void Game::socketConnect()
{
	switch( getCla2() ) {
		case C_CONN_OK:
			socketConnectOk();
			break;
		case C_CONN_ID:
			socketConnectId();
			break;
		case C_CONN_NAME:
			socketConnectName();
			break;
		case C_CONN_PLAYER:
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketConnectOk()
{
	TRACE("Game::socketConnectOk");

	_socket->sendConnectionVersion( VERSION  );
	_socket->sendConnectionName( getGamePlayer()->getConnectionName() );
	QHostAddress addr = _socket->peerAddress();
	//_local or localhost?
	if( addr != QHostAddress::LocalHost && addr != QHostAddress::LocalHostIPv6) {
		emit sig_newMessage( QString( tr("Connection established, %1") ).arg( getGamePlayer()->getConnectionName() ) );
		emit sig_newMessage( QString( tr("Host address %1") ).arg( addr.toString()));
		emit sig_newMessage( QString( tr("Host port %1,") ).arg( _socket->peerPort())+ QString( tr(" Our port %1") ).arg( _socket->peerPort() ) );
	}
}

void Game::socketConnectId()
{
	uchar tmp;

	tmp=_socket->readChar();
	getGamePlayer()->setNum( tmp );
	emit sig_newMessage( QString( tr("Connection ID, %1") ).arg( getGamePlayer()->getNum() ) );
}

void Game::socketConnectName()
{	
	QString res;
	uint len = _socket->readChar();
	for( uint i = 0; i < len; i++) {
		res.append( _socket->readChar() );
	}
	getGamePlayer()->setConnectionName(res);
	emit sig_newMessage( QString( tr("Name of player: %1") ).arg( res ) );
}

/*!

*/

void Game::slot_lordSelected( GenericLord  * lord )
{

	TRACE("Game::slot_lordSelected lord %p", lord);

	ScrollBase * scrBase = this->findChild<ScrollBase *>("AttalGameScrollBase");
	scrBase->deselect();
	
	computePath( lord, lord->getDestination());	
	
  getGamePlayer()->setSelectedLord( lord );
	
	setState( MS_LORD );

	emit sig_Center( lord->getCell() );
}


void Game::slot_baseSelected()
{
	ScrollLord * scrLord = this->findChild<ScrollLord *>("AttalGameScrollLord");
	scrLord->deselect();

	GenericBase * selectedBase = getGamePlayer()->getSelectedBase();
	
	setState( MS_BASE );

	emit sig_Center( selectedBase->getCell() );
}

void Game::slot_displayLord()
{
	emit sig_Center( getGamePlayer()->getSelectedLord()->getCell() );

	if( getGamePlayer()->numLord() > 0 &&  _isPlaying ) {
		try {
			DisplayLordTab * dispLord = new DisplayLordTab( getGamePlayer(), this );
			dispLord->exec();
			delete dispLord;
		} catch( std::bad_alloc &) {
			logEE("bad alloc");
		}
	}
}

void Game::slot_displayBase()
{
	GenericBase * selectedBase = getGamePlayer()->getSelectedBase();

	if( selectedBase ){
		emit sig_Center( selectedBase->getCell() );
		if( _isPlaying ) {
			//why this code is present, bad comments
			//this code i called only from sig_base
			emit sig_base( selectedBase );
		}
	}
}

void Game::socketQR()
{
	switch( getCla2() ) {
		case C_QR_MSG_NEXT: 
			socketQRMsgNext();
			break;
		case C_QR_MSG_END: 
			socketQRMsgEnd();
			break;
		case C_QR_LEVEL: 
			socketQRLevel();
			break;
		case C_QR_CHEST:
			socketQRChest();
			break;
		case C_QR_CREATURE_FLEE: 
			socketQRCreature(QRFLEE);
			break;
		case C_QR_CREATURE_MERCENARY: 
			socketQRCreature(QRMERCENARY);
			break;
		case C_QR_CREATURE_JOIN:
			socketQRCreature(QRJOIN);
			break;
		case C_QR_ANSWER:
			logEE( "Should not happen" );
			break;
		default:
			printSocket;
			break;
	}
}

void Game::socketQRMsgNext()
{
	uchar len = _socket->readChar();
	for( uint i = 0; i < len; i++ ) {
		_msg.append( _socket->readChar() );
	}
}

void Game::socketQRMsgEnd()
{		
	//QMessageBox::information( this, "Information", QString( buf+(3*sizeof(char)) ), 0 );
	uchar type = _socket->readChar();
	socketQRMsgNext();
	if( type  == QR_SCEN_DESC ) {
		_scenDesc = _msg;
	}

	QString mess =  QString("\n\n") +_msg + "\n\n";
	AttalPopup * popup = this->findChild<AttalPopup *>("AttalGamePopup");
	popup->setText( mess );
	popup->centerParent();
	popup->show();

	//QMessageBox::information( this, tr("Information"), _msg );
	_msg = "";
}

void Game::socketQRLevel()
{
	GainLevel * level = new GainLevel( this );
	level->exec();
	_socket->sendAnswer( level->getChoice() );
	delete level;
}

void Game::socketQRChest()
{
	AskChest * chest = new AskChest();
	chest->exec();
	_socket->sendAnswerEnum( chest->getResult() );
	delete chest;
}

void Game::socketQRCreature(QRCreature state)
{
	AskDialog dialog;
	QString text;
	/// XXX: to improve with name/category of creature

	switch(state) {
		case QRFLEE:
			text = tr("The creatures are fleeing. Do you want to let them flee ?") ;
			break;
		case QRMERCENARY:
			text = tr("You can buy these creatures. Do you want to buy them ?") ;
			break;
		case QRJOIN:
			text = tr("The creatures want to join. Do you accept them ?") ;
			break;
		default:
			printSocket;
			break;
	}

	dialog.setText( text );
	dialog.setYesNo();

	if( dialog.exec() ) {
		_socket->sendAnswerYesNo( true );
	} else {
		_socket->sendAnswerYesNo( false );
	}

}

void Game::socketFight()
{
	switch( getCla2() ) {
		case C_FIGHT_INIT: 
			socketFightInit();
			break;
		case C_FIGHT_LORD:
			//logEE( "Should not happen (FIGHT_LORD)" );
			break;
		case C_FIGHT_UNIT:
			//logEE( "Should not happen (FIGHT_UNIT)" );
			break;
		case C_FIGHT_END:
			socketFightEnd();
			break;
		default:
			//logEE( "Should not happen (FIGHT)" );
			break;
	}
}

void Game::socketFightInit()
{
	uchar cla = _socket->readChar();
	uchar lord = _socket->readChar();
	emit sig_fight( getLord( lord ), (CLASS_FIGHTER)cla );
	stopTimer();
}

void Game::socketFightEnd()
{
	restartTimer();
}

void Game::displayMiniMap( bool state )
{
	MiniMap * miniMap = this->findChild<MiniMap *>("AttalGameMiniMap");
	MapView * view = this->findChild<MapView *>("AttalGameMapView");
	if( miniMap ) {
		miniMap->setVisible( state );
	}
	view->setScrollBarVisible( !state );
}

void Game::displayFullScreen( bool state )
{
	ChatWidget * chat = this->findChild<ChatWidget *>("AttalGameChat");
	if( chat ) {
		chat->setVisible( state );
	}
	displayMiniMap(state);
}

void Game::computePath( GenericLord * lord, GenericCell * cell )
{
	GraphicalPath * gpath = theMap->getGraphicalPath();
	gpath->computePath( lord->getCell(), cell, lord->getCharac( MOVE )  ,lord->getCharac( MAXMOVE ));
}

void Game::unSelectLord()
{
	TRACE("Game::unSelectLord");

	if( getGamePlayer()->getSelectedLord() ){
		emit sig_clearPath();
		getGamePlayer()->setSelectedLord( NULL );
	}
}

void Game::updateOptions()
{
	updateAnimations();
	updateDispositionMode();
}

void Game::updateAnimations()
{
	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();
	if( settings.isAnimationEnabled ) {
		restartTimer();
	} else {
		stopTimer();
	}
}

void Game::updateDispositionMode()
{
	AttalSettings::DispositionMode mode = AttalSettings::getInstance()->getDispositionMode();

	QHBoxLayout * layH1 = this->findChild<QHBoxLayout *>("layH1");
	QVBoxLayout * layControlV1 = layH1->findChild<QVBoxLayout *>("layControlV1");
	GameControl * control = this->findChild<GameControl *>("AttalGameControl");
	ScrollLord * scrLord = this->findChild<ScrollLord *>("AttalGameScrollLord");
	ScrollBase * scrBase = this->findChild<ScrollBase *>("AttalGameScrollBase");
	MapView * view = this->findChild<MapView *>("AttalGameMapView");

	layH1->removeWidget( view );
	
	if( layControlV1 ) {
		layH1->removeItem( layControlV1 );
		delete layControlV1;
	}

	layControlV1 = new QVBoxLayout();

	switch( mode ) {
		case AttalSettings::DM_VERYCOMPACT:	
			scrLord->repaintButtons( 2 );
			scrBase->repaintButtons( 2 );

			layControlV1->addWidget( scrLord, 1, Qt::AlignHCenter );
			layControlV1->addWidget( control, 0, Qt::AlignHCenter );
			layControlV1->addWidget( scrBase, 1, Qt::AlignHCenter );
			break;
		case AttalSettings::DM_COMPACT:	
			scrLord->repaintButtons( 3 );
			scrBase->repaintButtons( 3 );

			layControlV1->addWidget( scrLord, 1, Qt::AlignHCenter );
			layControlV1->addWidget( control, 0, Qt::AlignHCenter );
			layControlV1->addWidget( scrBase, 1, Qt::AlignHCenter );

			break;
		case AttalSettings::DM_FULL:
			scrLord->repaintButtons( 4 );
			scrBase->repaintButtons( 4 );

			layControlV1->addWidget( scrLord, 1, Qt::AlignHCenter );
			layControlV1->addWidget( control, 0, Qt::AlignHCenter );
			layControlV1->addWidget( scrBase, 1, Qt::AlignHCenter );

			break;
	}
	layH1->addWidget( view, 1 );
	layH1->addLayout( layControlV1 );

	emit sig_baseReinit();
	emit sig_lordReinit();
}

void Game::timerEvent(QTimerEvent * /* event */)
{
	stopTimer();
	advanceAnimations();
	restartTimer();
}

void Game::setAdvancePeriod( int period ) 
{
	stopTimer();
	_period =  period;
	restartTimer();
}

void Game::restartTimer() 
{
	AttalSettings::StrategyModeSettings settings = AttalSettings::getInstance()->getStrategyModeSettings();
	if( _idTime == -1 && settings.isAnimationEnabled ) {
		_idTime = startTimer(_period);
	}
}

void Game::stopTimer() 
{
	if( _idTime != -1 ) {
		killTimer( _idTime );
		_idTime = -1;
	}
}

inline uchar Game::getCla1() 
{
	return _socket->getCla1();
}

inline uchar Game::getCla2() 
{
	return _socket->getCla2();
}

inline uchar Game::getCla3() 
{
	return _socket->getCla3();
}
