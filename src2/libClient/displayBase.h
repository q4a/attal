/****************************************************************
**
** Attal : Lords of Doom
**
** displayBase.h
** inside the base
**
** Version : $Id: displayBase.h,v 1.44 2010/08/06 22:18:39 lusum Exp $
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

 
#ifndef DISPLAYBASE_H
#define DISPLAYBASE_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QLabel>

// application specific includes
#include "libCommon/unit.h"
#include "libCommon/genericPlayer.h"
#include "libCommon/priceMarket.h"
#include "libClient/displayCreature.h"
#include "libClient/widget.h"
#include "libClient/ressourceWin.h"


class QHBoxLayout;
class QVBoxLayout;
class QTabWidget;

class AttalButton;
class AttalSocket;
class BaseSummary;
class BaseLords;
class BaseButtons;
class BaseRightPanel;
class BaseTroop;
class DisplayBaseLord;
class DisplayCreatureBase;
class GenericBase;
class GenericLord;
class GenericInsideBuilding;
class InsideActionAllBuildings;
class InsideBase;
class InsideBaseView;
class Tavern;
class UnitSummary;



/*              ------------------------------
 *                         DisplayBase
 *              ------------------------------ */

#define NB_CREATURE_BASE 7

/** Display inside of a base */
class DisplayBase : public QWidget
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayBase( QWidget * parent = 0, GenericBase * base = NULL, AttalSocket * socket = NULL );

	~DisplayBase();

	GenericBase * getBase() { return _base; }

	/** Reinits the display of the base */
	void reinit();

	/** Reinits the display of the base for teams */
	void reinitBase( GenericBase * base );

signals:
	/** Signal for quitting */
	void sig_quit();
	
	void sig_resource();
	
	void sig_castle();

	void sig_tavernInfo( char );

	void sig_tavernLord( int, bool );

public slots:
	/** Slot when a building is clicked */
	void slot_building( GenericInsideBuilding * );
	
	void slot_message( QString );

	void slot_buyLord( uchar id );
	
	void slot_market( int own, int other, int value );
	
	void slot_updateLordInfo();
	
	void slot_askLord( int nbRead ); 
	
	void slot_modifBaseBuilding( int row, int col, uchar level, bool create ); 
	
	void slot_baseUnitBuy( Creature * creature , int number  );
	
	void slot_requestBuilding( int buildingId, bool buysell );
	
	void slot_lordGarrison( GenericLord * lord, bool state );

	void slot_exchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );

	void slot_exchangeBaseUnit( int unit1, GenericLord * lord2, int unit2 );

	void updateDispositionMode();
	
private:
	
	void actionAllBuildings();

	void actionAllCreatures();

	void actionSomeCreatures( GenericInsideBuilding * building );

	void actionMarket();
	
	void actionTavern();

	GenericPlayer * _player;
	GenericBase * _base;
	AttalSocket * _socket;
	
	QTabWidget * _tabs;
	InsideBase * _inside;
	InsideBaseView * _view;
	BaseRightPanel * _panel;
	BaseSummary * _baseSummary;
	InsideActionAllBuildings * _manageBuildings;
	DisplayCreatureBase * _manageCreatures; 
	TabRessourceWin * _ressW;
};

class BaseRightPanel : public QWidget
{
	Q_OBJECT
public:
	BaseRightPanel( QWidget * parent = 0, GenericPlayer * player = NULL , GenericBase * base = NULL );

  void setBase( GenericBase * base );
	
signals:
	void sig_quit();
	
	void sig_reinit();
	
	void sig_lordGarrison( GenericLord * , bool );

	void sig_exchangeUnit( GenericLord * , int , GenericLord * , int  );
	
	void sig_exchangeBaseUnit( int , GenericLord * , int  );
	
public slots:
	void slot_lords();
	
	void slot_info();
	
	void reinit();

protected:
	
	AttalButton * _pbLord;
	QPushButton * _pbInfo;
	QLabel * _lordLabel;
	BaseButtons * _bases;
	GenericBase * _base;
	GenericPlayer * _player;
	RessourceWin * _ress;
};

/** Widget for displaying a summary of the base info */
class BaseSummary : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	BaseSummary( QWidget * parent = 0, GenericPlayer * player = NULL , GenericBase * base = NULL  );

	/** Initialize with the current base to display */
	void setBase( GenericBase * base );

	void reinit();

private:
	UnitSummary * _unitIco[8];
	GenericBase * _base;
	GenericPlayer * _player;
	RessourceWin * _ress;
};

class DisplayBaseInfo : public QDialog
{
	Q_OBJECT
public:
	DisplayBaseInfo( QWidget * parent = 0,  GenericPlayer * player = NULL , GenericBase * base = NULL );
	
	void reinit();

protected:
	BaseSummary * _widget;
};

/** Displays the lord of the base */
class BaseLords : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	BaseLords( QWidget * parent = 0 , GenericPlayer * player = NULL, GenericBase * base = NULL  );

	/** Initialize with the current base to display */
	void setBase( GenericBase * base );
	
	void setPlayer( GenericPlayer * player );

	/** Tell if there is a visiting lord */
	bool isVisitor() {
		return (_visitor == 0);
	}

public slots:
	/** Slot for exchange button */
	void slot_exchange();

	/** Slot if the garrison lord is clicked */
	void slot_photoGarrison();

	/** Slot if the visitor lord is clicked */
	void slot_photoVisitor();

	/** Slot if the 'num'-th unit of garrison is clicked */
	void slot_unitGarrison( int num );

	/** Slot if the 'num'-th unit of visit is clicked */
	void slot_unitVisitor( int num );
	
	/** Reinit the widget */
	void reinit();

signals:
	/** Signal for exchanging lords */
	void sig_exchange( bool st );
	
	void sig_lordGarrison( GenericLord * , bool );

	void sig_exchangeUnit( GenericLord * , int , GenericLord * , int  );
	
	void sig_exchangeBaseUnit( int , GenericLord * , int  );

private:
	void unselectUnits();
	void unselectPhoto();
	void exchangeLords();
	void exchangeUnitsGV();
	void exchangeUnitsVG();
	void exchangeUnitsGG( int u1, int u2 );
	void exchangeUnitsVV( int u1, int u2 );

	bool _isExchange;
	BaseTroop * _garrison, * _visitor;
	GenericBase * _base;
	GenericPlayer * _player;
	bool _garrisonSelected, _visitorSelected;
	int _unitGarrison, _unitVisitor;
};

class DisplayBaseLord : public QDialog
{
	Q_OBJECT
public:
	DisplayBaseLord( QWidget * parent = 0, GenericPlayer * player = NULL , GenericBase * base = NULL );
	
protected:
	BaseLords * _widget;
};

/** Button for changing of base */
class BaseButtons : public QWidget
{
	Q_OBJECT
public:
	/** Constructors */
	BaseButtons( QWidget * parent = 0 );

	/** Initialize the GenericPlayer */
	void setPlayer( GenericPlayer * player ) { _player = player; }

public slots:
	/** Slot for exchanging lords */
	void slot_exchange( bool st );

signals:
	/** Signal for quitting */
	void sig_quit();

	/** Signal for exhanging */
	void sig_exchange();

private:
	GenericPlayer * _player;
	QPushButton * _pbExch;

};

/** Summary of units of a lord */
class UnitSummary : public QWidget
{
public:
	/** Constructor */
	UnitSummary( QWidget * parent = 0 );

	/** Initialize the creature to display */
	void setCreature( int id );

	/** Initialize the player */
	void setPlayer( GenericPlayer * player ) { _player = player; }

	/** Det the number of creatures */
	void setNumber( int num );

private:
	QLabel * _ico, * _nb;
	GenericPlayer * _player;
};

/** Display troops inside base */
class BaseTroop : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	BaseTroop( bool isGarrison, QWidget * parent = 0 );

	/** Clear BaseTroop Widget */
	void clear();

	/** Initialize the player */
	void setPlayer( GenericPlayer * player ) { _player = player; }

	/** Initialize the lord to display */
	void setLord( GenericLord * lord );

	/** Initialize the base to display */
	void setBase( GenericBase * base );

	/** Reinit the widget (to synchronize change) */
	void reinit();

	/** Select the 'photo' widget */
	void selectPhoto() {}

	/** Unselect the 'photo' widget */
	void unselectPhoto() {}

	/** Select unit 'num' */
	void selectUnit( int /*num*/ ) {}

	/** Unselect all units */
	void unselectUnits() {}

signals:
	/** Signal if photo clicked */
	void sig_photo();

	/** Signal if unit clicked */
	void sig_unit( int );

private:
	Icon * _photo;
	Icon * _units[MAX_UNIT];
	bool _isGarrison;
	GenericLord * _lord;
	GenericBase * _base;
	QLabel * _numUnit[ MAX_UNIT ], * _name;
	GenericPlayer * _player;
};

#endif // DISPLAYBASE_H
