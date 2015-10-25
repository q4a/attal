/****************************************************************
**
** Attal : Lords of Doom
**
** displayBase.cpp
** inside the base
**
** Version : $Id: displayBase.cpp,v 1.94 2010/08/06 22:18:39 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 03/09/2000
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

#include "displayBase.h"

 
// generic include files
// include files for QT
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QTabWidget>

// application specific include files
#include "conf.h"

#include "libCommon/attalSettings.h"
#include "libCommon/attalSocket.h"
#include "libCommon/attalTemplates.h"
#include "libCommon/dataTheme.h"
#include "libCommon/genericBuilding.h"
#include "libCommon/genericLord.h"
#include "libCommon/log.h"

#include "libClient/attalButton.h"
#include "libClient/displayCreature.h"
#include "libClient/game.h"
#include "libClient/gui.h"
#include "libClient/imageTheme.h"
#include "libClient/insideAction.h"
#include "libClient/insideBase.h"
#include "libClient/market.h"
#include "libClient/tavern.h"
#include "libClient/unitExchange.h"


extern QString DATA_PATH;
extern QString IMAGE_PATH;

DisplayBase::DisplayBase( QWidget * parent, GenericBase * base, AttalSocket * socket )
	: QWidget( parent )
{
	Game * game = parent->findChild<Game *>("AttalGame");
	_socket = socket;
	_player = game->getGamePlayer();
	_base =  base;

	_tabs = new QTabWidget( this );

	_inside = new InsideBase( this, _base );	

	_view = new InsideBaseView( _inside, this ); 
	
	_manageBuildings = new InsideActionAllBuildings( this , _base, _player  );
	
	_manageCreatures = new DisplayCreatureBase( this, _base, _player );
	
	_ressW = new TabRessourceWin( this, RS_GRID, _player, _base->getResourceList());
	
	_tabs->addTab( _view, tr( "View" ) );
	_tabs->addTab( _manageBuildings, tr( "Buildings" ) );
	_tabs->addTab( _manageCreatures, tr( "Creatures" ) );
	_tabs->addTab( _ressW, tr( "Resources" ) );

	_panel = new BaseRightPanel( this, _player, _base );
	
	FIXEDSIZE( _panel );
	
	QHBoxLayout * layout = new QHBoxLayout( this );	

	updateDispositionMode();	
	
	connect( _view, SIGNAL( sig_building( GenericInsideBuilding * ) ), SLOT( slot_building( GenericInsideBuilding * ) ) );
	connect( _panel, SIGNAL( sig_quit() ), SIGNAL( sig_quit() ) );
	connect( game, SIGNAL( sig_baseUpdate() ), SIGNAL( sig_castle() ) );
	connect( game, SIGNAL( sig_modifBaseBuilding( int, int, uchar, bool ) ), SLOT( slot_modifBaseBuilding( int, int, uchar, bool ) ));
	connect( game, SIGNAL( sig_tavernInfo( char ) ), SIGNAL( sig_tavernInfo( char ) ));
	connect( game , SIGNAL( sig_tavernLord( int ,bool  ) ),  SIGNAL( sig_tavernLord( int ,bool  ) ) );
	connect( game , SIGNAL( sig_newMessage( const QString & ) ), _view,  SLOT( newMessage(  const QString & ) ) );
	//connect( _baseButtons, SIGNAL( sig_exchange() ), _baseLords, SLOT( slot_exchange() ) );
	//connect( _baseLords, SIGNAL( sig_exchange( bool ) ), _baseButtons, SLOT( slot_exchange( bool ) ) );
	
	layout->activate();
}

DisplayBase::~DisplayBase()
{
	TRACE("~DisplayBase");
}

void DisplayBase::reinit()
{
	/// XXX:_baseSummary->reinit();
	
	TRACE("DisplayBase::reinit");

	_panel->reinit();

	emit sig_castle();

}

void DisplayBase::reinitBase( GenericBase * base )
{
	_base = base;

	_inside->setBase( base );

	_panel->setBase( base );
	
	_ressW->setList( _base->getResourceList());

	delete _manageBuildings;
	
	delete _manageCreatures;
	
	_manageCreatures = new DisplayCreatureBase( this, _base, _player );
	
	_manageBuildings = new InsideActionAllBuildings( this , _base, _player  );

	_tabs->insertTab( 1, _manageBuildings, tr( "Buildings" ) );
	_tabs->insertTab( 2,_manageCreatures, tr( "Creatures" ) );

}

void DisplayBase::updateDispositionMode()
{
	AttalSettings::DispositionMode mode = AttalSettings::getInstance()->getDispositionMode();
	TRACE("DisplayBase::updateDispositionMode DispositionMode %d", mode );
	
	QHBoxLayout * mainlayout =dynamic_cast<QHBoxLayout*>( layout() );
	QVBoxLayout * layVerticalView =  this->findChild<QVBoxLayout *>("AttalBaseLayVertView");
	
	mainlayout->removeWidget( _panel );
	if( layVerticalView ) {
		mainlayout->removeItem( layVerticalView );
		delete layVerticalView;
	}
	layVerticalView = new QVBoxLayout();
	layVerticalView->setObjectName( QString("AttalBaseLayVertView"));

	switch( mode ) {
		case AttalSettings::DM_VERYCOMPACT:
			_tabs->setMinimumSize( 0, 0 );
			_tabs->setMaximumSize( 700, 500 );
			_view->setMinimumSize( 0, 0 );
			//_view->setMaximumSize( 960, 720 );
			_view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
			_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
			layVerticalView->addWidget( _tabs, 1 );
		break;
		case AttalSettings::DM_COMPACT:
			_tabs->setMinimumSize( 0, 0 );
			_tabs->setMaximumSize( 900, 700 );
			_view->setMinimumSize( 0, 0 );
			//_view->setMaximumSize( 960, 720 );
			_view->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
			_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
			layVerticalView->addWidget( _tabs, 1 );
		break;
		case AttalSettings::DM_FULL:
			_tabs->setFixedSize( 960, 740 );
			_view->setFixedSize( 960, 720 );
			_view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
			_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
			layVerticalView->addWidget( _tabs );
			//_layVerticalView->addStretch( 1 );
		break;
	}

	mainlayout->addLayout( layVerticalView, 1 );
	mainlayout->addWidget( _panel );
}

void DisplayBase::slot_modifBaseBuilding( int row, int col, uchar level, bool create )
{
	if( ( _base->getCell()->getRow() == row ) &&
			( _base->getCell()->getCol() == col ) ) {
		if( create ) {
			GenericInsideBuilding * building =  _base->getBuildingByType( level );
			_inside->addBuilding( building );
		} else {
			_inside->delBuilding( level );
		}
	}
	emit sig_castle();
}

void DisplayBase::slot_message( QString msg )
{
	if( _socket ) {
		_socket->sendMessage( _player->getConnectionName() + " : " + msg );
	} else {
		_view->newMessage( "(Not connected) : " + msg );
	}
}

void DisplayBase::slot_building( GenericInsideBuilding * building )
{
	if( _base ) {
		InsideBuildingModel * model = DataTheme.bases.at( _base->getRace() )->getBuildingModel( building->getLevel() );
		if( model->getAction() ) {
			switch( model->getAction()->getType() ) {
			case INSIDE_NONE:
				break;
			case INSIDE_VILLAGE:
				actionAllBuildings();
				break;
			case INSIDE_CASTLE:
				actionAllCreatures();
				break;
			case INSIDE_CREA:
				actionSomeCreatures( building );
				break;
			case INSIDE_MARKET:
				actionMarket();
				break;
			case INSIDE_TAVERN:
				actionTavern();
				break;
			default:
				logEE( "Unknown action %d", model->getAction()->getType() );
				break;
			}
		}
	}
}

void DisplayBase::actionAllBuildings()
{
	try {
		InsideActionAllBuildings * allBuildings = new InsideActionAllBuildings( this, _base, _player, true  );
		allBuildings->exec();
		emit sig_castle();
		delete allBuildings;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void DisplayBase::actionAllCreatures()
{
	try {
		DisplayCreatureBase * creatures = new DisplayCreatureBase( this, _base, _player );
		creatures->exec();
		emit sig_castle();
		delete creatures;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void DisplayBase::actionSomeCreatures( GenericInsideBuilding * building )
{
	if( _base ) {
		InsideAction * action = DataTheme.bases.at( _base->getRace() )->getBuildingModel( building->getLevel() )->getAction();
		if( action ) {
			//InsideActionSomeCreature dialog( action->getParam( 0 ), action->getParam( 1 ) );
			int race = action->getParam( 0 );
			int level = action->getParam( 1 );
			BuyCreature dialog( this , _player, _base, DataTheme.creatures.at( race, level ) );
			if( dialog.exec() ) {
				// try to buy some creature
			}
		}
	}
	emit sig_castle();
}

void DisplayBase::actionMarket()
{
	try {
		Market * mark= new Market( this, _player );

		mark->exec();
		emit sig_castle();
		delete mark;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void DisplayBase::actionTavern()
{

	try {
		Tavern * tavern = new Tavern( this );
		slot_updateLordInfo();
		emit sig_castle();

		tavern->exec();
		delete tavern;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void DisplayBase::slot_buyLord( uchar id )
{

	/// XXX: not finished
	if(_player->canBuy(DataTheme.lords.at( id )) && !_base->isVisitorLord()) {
		int col = _base->getCell()->getCol();
		int row = _base->getCell()->getRow();
		//logDD(" row %d ,col %d", row, col);
		_socket->sendLordBuy( id , row, col);
		TRACE( "buy %d", id );
	}

}

void DisplayBase::slot_updateLordInfo()
{
	_socket->askTavernInfo( _base );
}

void DisplayBase::slot_askLord( int nbRead )
{
	_socket->askTavernLord( _base, nbRead );
}

void DisplayBase::slot_market( int own, int other, int value )
{
	TRACE("own %d, other %d, value %d",own,other,value);
	_socket->sendBaseMarket(own,other,value);
}
	
void DisplayBase::slot_baseUnitBuy( Creature * creature , int number  )
{
	_socket->sendBaseUnitBuy( _base, creature, number );
}

void DisplayBase::slot_requestBuilding( int buildingId, bool buysell )
{
	_socket->requestBuilding( _base, buildingId, buysell );
}

void DisplayBase::slot_lordGarrison( GenericLord * lord, bool state )
{
	_socket->sendLordGarrison( lord, state );
}

void DisplayBase::slot_exchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 )
{
	_socket->sendExchangeUnit( lord1, unit1, lord2 , unit2 );
}

void DisplayBase::slot_exchangeBaseUnit( int unit1, GenericLord * lord2, int unit2 )
{
	_socket->sendExchangeBaseUnit( _base, unit1, lord2,  unit2 );
}

//
// ----- BaseRightPanel -----
//

BaseRightPanel::BaseRightPanel( QWidget * parent, GenericPlayer * player, GenericBase * base )
: QWidget( parent )
{
	TRACE("BaseRightPanel  QWidget * parent %p, GenericPlayer * player %p, GenericBase * base %p )", parent, player, base );

	_player = player;
	_base = base;

	_pbLord = new AttalButton( this );
	_pbLord->setFixedSize( 60, 60 );
	_lordLabel = new QLabel( this );
	_lordLabel->setText( "\n\n" );
	_lordLabel->setAlignment( Qt::AlignLeft );
	_lordLabel->setWordWrap( true );	
	_lordLabel->setMinimumWidth( 64 );
	_lordLabel->setMinimumHeight( _lordLabel->sizeHint().height() );
	
	_pbInfo = new QPushButton( this );
	_pbInfo->setText( "Info" );
	FIXEDSIZE( _pbInfo );
	_ress = new RessourceWin(this,RS_VERT, player, _base->getResourceList());
	
	//_bases = new BaseButtons( this );
	
	/// XXX: use pixmap instead
	AttalButton * pbQuit = new AttalButton( this, AttalButton::BT_OK );
	FIXEDSIZE( pbQuit );

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setMargin( 5 );
	layout->setSpacing( 10 );
	layout->addWidget( _pbLord );
	layout->addWidget( _lordLabel );
	layout->addSpacing( 10 );
	layout->addWidget( _pbInfo );
	layout->addWidget( _ress );
	layout->addStretch( 1 );
	layout->addWidget( pbQuit );
	layout->addStretch( 1 );
	reinit();
	layout->activate();
	
	connect( pbQuit, SIGNAL( clicked() ), SIGNAL( sig_quit() ) );
	connect( _pbLord, SIGNAL( clicked() ), SLOT( slot_lords() ) );
	connect( _pbInfo, SIGNAL( clicked() ), SLOT( slot_info() ) );
	connect( parent, SIGNAL( sig_castle() ), SLOT( reinit() ));
	connect( parent, SIGNAL( sig_castle() ), _ress, SLOT( reinit() ));
	connect( this, SIGNAL( sig_lordGarrison( GenericLord * , bool ) ), parent ,  SLOT( slot_lordGarrison( GenericLord * , bool ) ) );
	connect( this, SIGNAL( sig_exchangeUnit( GenericLord * , int , GenericLord * , int  )), parent ,  SLOT( slot_exchangeUnit( GenericLord * , int , GenericLord * , int  )) );
	connect( this, SIGNAL( sig_exchangeBaseUnit( int , GenericLord * , int  ) ), parent ,  SLOT( slot_exchangeBaseUnit( int , GenericLord * , int  ) ));
}

void BaseRightPanel::setBase( GenericBase * base ) 
{
	TRACE("BaseRightPanel::setBase( GenericBase * %p )", base);
	_base = base;
	_ress->setList( _base->getResourceList() );
	reinit();
}

void BaseRightPanel::slot_lords()
{
	try {
		DisplayBaseLord * displayLords = new DisplayBaseLord( this , _player, _base );
		displayLords->exec();
		reinit();
		delete displayLords;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void BaseRightPanel::slot_info()
{
	try {
		DisplayBaseInfo * displayInfo = new DisplayBaseInfo( this , _player, _base );
		displayInfo->exec();
		delete displayInfo;
	} catch( std::bad_alloc &) {
		logEE("bad alloc");
	}
}

void BaseRightPanel::reinit()
{
	GenericLord * lord = 0;
	QString text;

	 text = "\nNo lord\n";
	if( _base->getGarrisonLord() ) {
		lord = _base->getGarrisonLord();
		text.sprintf( "Lord\n%s\n(Guarrison)", qPrintable(lord->getName()) );
	} else if( _base->getVisitorLord() ) {
		lord = _base->getVisitorLord();
		text.sprintf( "Lord\n%s\n(Visitor)", qPrintable(lord->getName()) );
	}
	if( lord ) {
		_pbLord->setPixmap( * ImageTheme.getLordPixmap( lord->getId() ) );
	} else {
		/// XXX: should be optimized in imageTheme
		_pbLord->setPixmap( QPixmap( IMAGE_PATH + "/lords/noLord.png" ) );
	}
	
	_lordLabel->setText( text );

	emit sig_reinit();
}

//
// ----- BaseSummary -----
//

BaseSummary::BaseSummary( QWidget * parent,  GenericPlayer * player , GenericBase * base )
	: QWidget( parent )
{
	_player = player;
	_base = base;
	//int i;

	_ress = new RessourceWin( this, RS_GRID, player , _base->getResourceList());

	// XXX: TODO
	/*QHBoxLayout * lay2 = new QHBoxLayout();
	lay2->addStretch( 1 );
	for( i = 0; i < 4; i++ ) {
		_unitIco[i] = new UnitSummary( this );
		lay2->addWidget( _unitIco[i] );
		lay2->addStretch( 1 );
	}

	QHBoxLayout * lay3 = new QHBoxLayout();
	lay3->addStretch( 1 );
	for( i = 0; i < 4; i++ ) {
		_unitIco[4+i] = new UnitSummary( this );
		lay3->addWidget( _unitIco[4+i] );
		lay3->addStretch( 1 );
	}*/
	
	reinit();

	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _ress );
	layout->addStretch( 2 );
	//layout->addLayout( lay1 );
	//layout->addLayout( lay2 );
	//layout->addLayout( lay3 );
	//layout->addStretch( 1 );
	layout->activate();
}

void BaseSummary::reinit()
{
	_ress->reinit();
}

//
// ----- DisplayBaseInfo  -----
//

DisplayBaseInfo::DisplayBaseInfo( QWidget * parent, GenericPlayer * player , GenericBase * base )
: QDialog( parent, Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint )
{
	setWindowTitle( tr( "Base Information" ) );

	_widget = new BaseSummary( this, player, base );

	AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->setSpacing( 5 );
	layout->setMargin( 5 );
	layout->addWidget( _widget );;
	layout->addWidget( pbOk, 0, Qt::AlignHCenter );
	layout->activate();
	
	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
}

void DisplayBaseInfo::reinit()
{
	_widget->reinit();
}

//
// ----- BaseLords -----
//

BaseLords::BaseLords( QWidget * parent , GenericPlayer * player, GenericBase * base )
	: QWidget( parent )
{
	_isExchange = false;

	setFixedHeight( 240 );	
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_garrison = new BaseTroop( true, this );
	layout->addWidget( _garrison, 1 );
	
	_visitor = new BaseTroop( false, this );
	layout->addWidget( _visitor, 1 );
	
	layout->activate();

	connect( _garrison, SIGNAL( sig_photo() ), SLOT( slot_photoGarrison() ) );
	connect( _garrison, SIGNAL( sig_unit( int ) ), SLOT( slot_unitGarrison( int ) ) );
	connect( _visitor, SIGNAL( sig_photo() ), SLOT( slot_photoVisitor() ) );
	connect( _visitor, SIGNAL( sig_unit( int ) ), SLOT( slot_unitVisitor( int ) ) );

	_garrisonSelected = false;
	_visitorSelected = false;
	_unitGarrison = -1;
	_unitVisitor = -1;
	
	setPlayer( player );
	setBase( base );
}


void BaseLords::setPlayer( GenericPlayer * player )
{
	_player = player;

	_garrison->setPlayer( player );
	_visitor->setPlayer( player );
}

void BaseLords::setBase( GenericBase * base )
{
	_base = base;

	_garrison->setBase( base );
	_visitor->setBase( base );

	_garrisonSelected = false;
	_visitorSelected = false;
}

void BaseLords::reinit()
{
	_garrison->reinit();
	_visitor->reinit();
}

void BaseLords::slot_exchange()
{
	_isExchange = !_isExchange;
}

void BaseLords::slot_photoGarrison()
{
	unselectUnits();

	if( _garrisonSelected ) {
		/// XXX: show info lord
		_garrisonSelected = false;
	} else {
		if( _visitorSelected ) {
			exchangeLords();
		} else if( _base->isGarrisonLord() ) {
			_garrisonSelected = true;
		}
	}
}

void BaseLords::slot_photoVisitor()
{
	unselectUnits();
	
	if( _visitorSelected ) {
		/// XXX: show info lord
		_visitorSelected = false;
	} else {
		if( _garrisonSelected ) {
			exchangeLords();
		} else if( _base->isVisitorLord() ) {
			_visitorSelected = true;
		}
	}	
}

void BaseLords::exchangeLords()
{	
	_garrisonSelected = false;
	_visitorSelected = false;
	
	_base->exchangeLords();

	GenericLord * visitor = _base->getVisitorLord();
	_visitor->setLord( visitor );
	if( visitor ) {
		emit sig_lordGarrison( visitor, false );
	}

	GenericLord * garrison = _base->getGarrisonLord();
	_garrison->setLord( garrison );
	if( garrison ) {
		emit sig_lordGarrison( garrison, true );
	}
}

void BaseLords::exchangeUnitsGV()
{
	GenericLord * garrison = _base->getGarrisonLord();
	GenericLord * visitor = _base->getVisitorLord();

	if(visitor){
		if(garrison){
			emit sig_exchangeUnit( garrison, _unitGarrison, visitor, _unitVisitor );
		} else {
			emit sig_exchangeBaseUnit( _unitGarrison, visitor, _unitVisitor );
		}
	}
	_garrison->reinit();
	_visitor->reinit();
}
	
void BaseLords::exchangeUnitsVG()
{
	GenericLord * garrison = _base->getGarrisonLord();
	GenericLord * visitor = _base->getVisitorLord();
	if(garrison){
		emit sig_exchangeUnit( visitor, _unitVisitor, garrison, _unitGarrison );
	} else {
		emit sig_exchangeBaseUnit( _unitGarrison, visitor, _unitVisitor );
	}

	_garrison->reinit();
	_visitor->reinit();
}


void BaseLords::exchangeUnitsGG( int u1, int u2 )
{
	if( u1 == u2 ) {
		return;
	}
	
	GenericLord * garrison = _base->getGarrisonLord();
	if(garrison){
		emit sig_exchangeUnit( garrison, u1, garrison, u2 );
	} else {
		emit sig_exchangeBaseUnit( u1, 0, u2 );
	}
	_garrison->reinit();
}

void BaseLords::exchangeUnitsVV( int u1, int u2 )
{
	if( u1 == u2 ) {
		return;
	}

	GenericLord * visitor = _base->getVisitorLord();
	emit sig_exchangeUnit( visitor, u1, visitor, u2 );
	
	_visitor->reinit();
}

void BaseLords::slot_unitGarrison( int num )
{
	unselectPhoto();
	
	if( _isExchange ) {
		if( _unitGarrison != -1 ) {
		} else {
		}
	} else {
		if( ( _unitGarrison != -1 ) || ( _unitVisitor != -1 ) ) {
			if( _unitGarrison != -1 ) {
				exchangeUnitsGG( _unitGarrison, num );
			} else {
				_unitGarrison = num;
				exchangeUnitsVG();
			}
			unselectUnits();
		} else {
			if( _base->getGarrisonUnit( num ) != 0 ) {
				_unitGarrison = num;
				_garrison->selectUnit( _unitGarrison );
				emit sig_exchange( true );
			}
		}
	}
}

void BaseLords::slot_unitVisitor( int num )
{
	unselectPhoto();
	
	if( _isExchange ) {
		
	} else {
		if( ( _unitGarrison != -1 ) || ( _unitVisitor != -1 ) ) {
			if( _unitVisitor != -1 ) {
				exchangeUnitsVV( _unitVisitor, num );
			} else {
				_unitVisitor = num;
				exchangeUnitsGV();
			}
			unselectUnits();
		} else {
			if( _base->getVisitorUnit( num ) != 0 ) {
				_unitVisitor = num;
				_visitor->selectUnit( _unitVisitor );
				emit sig_exchange( true );
			}
		}
	}
}

void BaseLords::unselectUnits()
{
	_unitGarrison = -1;
	_garrison->unselectUnits();
	_unitVisitor = -1;
	_visitor->unselectUnits();
	emit sig_exchange( false );
}

void BaseLords::unselectPhoto()
{
	_garrisonSelected = false;
	_garrison->unselectPhoto();
	_visitorSelected = false;
	_visitor->unselectPhoto();
}

//
// ----- DisplayBaseLord -----
//

DisplayBaseLord::DisplayBaseLord( QWidget * parent, GenericPlayer * player, GenericBase * base )
: QDialog( parent, Qt::Dialog )
{
	_widget = new BaseLords( this, player, base );
	
	AttalButton * pbOk = new AttalButton( this, AttalButton::BT_OK );
	
	QHBoxLayout * layH1 = new QHBoxLayout();
	layH1->addStretch( 1 );
	layH1->addWidget( pbOk );
	layH1->addStretch( 1 );
	
	QVBoxLayout * layout = new QVBoxLayout( this );
	layout->addWidget( _widget, 1 );
	layout->addLayout( layH1 );
	layout->activate();
	
	connect( pbOk, SIGNAL( clicked() ), SLOT( accept() ) );
	connect( parent, SIGNAL( sig_reinit() ), _widget ,  SLOT( reinit() ) );
	connect( _widget, SIGNAL( sig_lordGarrison( GenericLord * , bool ) ), parent ,  SIGNAL( sig_lordGarrison( GenericLord * , bool ) ) );
	connect( _widget, SIGNAL( sig_exchangeUnit( GenericLord * , int , GenericLord * , int  )), parent ,  SIGNAL( sig_exchangeUnit( GenericLord * , int , GenericLord * , int  )) );
	connect( _widget, SIGNAL( sig_exchangeBaseUnit( int , GenericLord * , int  ) ), parent ,  SIGNAL( sig_exchangeBaseUnit( int , GenericLord * , int  ) ));
}

//
// ----- BaseButtons -----
//

BaseButtons::BaseButtons( QWidget * parent )
	: QWidget( parent )
{
	QVBoxLayout * layout = new QVBoxLayout( this );
	
	_pbExch = new QPushButton( this );
	_pbExch->setFixedSize( 50, 40 );
	layout->addWidget( _pbExch );
	
	layout->addStretch( 1 );
		
	layout->activate();
	setFixedHeight( 240 );	
	
	_pbExch->setEnabled( false );
	
	connect( _pbExch, SIGNAL( clicked() ), SIGNAL( sig_exchange() ) );
}

void BaseButtons::slot_exchange( bool st )
{
	if( st ) {
		_pbExch->setEnabled( true );
	} else {
		_pbExch->setEnabled( false );
	}
}

//
// ----- UnitSummary -----
//

UnitSummary::UnitSummary( QWidget * parent )
	: QWidget( parent )
{
	_ico = new QLabel( this );
	_ico->setFixedSize( 50, 40 );
	_nb = new QLabel( this );
	_nb->setFixedSize( 50, 20 );
	_nb->setAlignment( Qt::AlignCenter );
	_nb->move( 0, 40 );

	setFixedSize( 50, 60 );
}

void UnitSummary::setCreature( int /*id*/ )
{
	/// XXX: TODO
	logEE( "not yet implemented" );
}

void UnitSummary::setNumber( int num )
{
	if( num == 0 ) {
		_nb->setText( "" );
	} else {
		_nb->setText( "+" + QString::number( num ) );
	}
}

//
// ----- BaseTroop -----
//

BaseTroop::BaseTroop( bool isGarrison, QWidget * parent )
	: QWidget( parent )
{
	_isGarrison = isGarrison;
	_lord = 0;

	QHBoxLayout * layout = new QHBoxLayout( this );
	QVBoxLayout * layV2 = new QVBoxLayout();

	_photo = new Icon( this );
	_name = new QLabel( this );
	_name->setFixedSize( 60, 20 );
	layV2->addWidget( _photo );
	layV2->addWidget( _name );
	layout->addLayout( layV2 );
	layout->addStretch( 1 );
	QVBoxLayout * layV[ MAX_UNIT ];

	QSignalMapper * sigmap = new QSignalMapper( this );
	for( int i = 0; i < MAX_UNIT; i++ ) {
		layV[i] = new QVBoxLayout();
		_units[i] = new Icon( this );
		_numUnit[i] = new QLabel( this );
		_numUnit[i]->setFixedSize( 60, 20 );
		_numUnit[i]->setAlignment( Qt::AlignCenter );
		layV[i]->addWidget( _units[i] );
		layV[i]->addWidget( _numUnit[i] );
		layout->addLayout( layV[i] );
		sigmap->setMapping( _units[i], i );
		connect( _units[i], SIGNAL( sig_clicked() ), sigmap, SLOT( map() ) );
	}
	layout->addStretch( 1 );

	layout->activate();

	connect( _photo, SIGNAL( sig_clicked() ), SIGNAL( sig_photo() ) );
	connect( sigmap, SIGNAL( mapped( int ) ), SIGNAL( sig_unit( int ) ) );
}

void BaseTroop::clear()
{
	_lord = 0;
	_photo->clear();
	_name->clear();

	for( int i = 0; i < MAX_UNIT; i++ ) {
		_units[i]->clear();
		_numUnit[i]->clear();
	}
}

void BaseTroop::setLord( GenericLord * lord )
{
	_lord = lord;
	reinit();
}

void BaseTroop::setBase( GenericBase * base )
{
	_base = base;
	if( _isGarrison ) {
		_lord = base->getGarrisonLord();
	} else {
		_lord = base->getVisitorLord();
	}
	reinit();
}

void BaseTroop::reinit()
{
	QString text;

	if( _lord == 0 ) {
		clear();
		if( _isGarrison ) {
			_name->setText( _base->getName());
			for( int i = 0; i < MAX_UNIT; i++ ) {
				if( _base->getUnit( i ) ) {
					_units[i]->setPixmap( ImageTheme.getPhotoCreature( _base->getUnit( i ) ) );
					_numUnit[i]->setText( QString::number( _base->getUnit( i )->getNumber() ) );
					text = _base->getUnit(i)->getUnitDescription();
					_units[i]->setPopupText( text  );
				} else {
					_numUnit[i]->clear();
				}
			}
		}
		return;
	}
	
	_photo->setPixmap( *ImageTheme.getLordPixmap( _lord->getId() ) );
	_name->setText( _lord->getName());
	
	for( int i = 0; i < MAX_UNIT; i++ ) {
		if( _lord->getUnit( i ) ) {
			_units[i]->setPixmap( ImageTheme.getPhotoCreature( _lord->getUnit( i ) )  );
			_numUnit[i]->setText( QString::number( _lord->getUnit( i )->getNumber() ) );
			text = _lord->getUnit(i)->getUnitDescription();
			_units[i]->setPopupText( text  );
		} else {
			_units[i]->clear();
			_numUnit[i]->clear();
		}
	}	
}

