/****************************************************************
**
** Attal : Lords of Doom
**
** insideAction.h
** Dialogs for actions of inside buildings
**
** Version : $Id: insideAction.h,v 1.19 2008/07/10 23:26:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 10/05/2002
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


#ifndef INSIDEACTION_H
#define INSIDEACTION_H


// generic include files
// include files for QT
#include <QDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QLayout>

// application specific include files
#include "libCommon/creature.h"


class QPushButton;

class AttalSocket;
class BuildingPanel;
class DisplayCreature;
class GenericBase;
class GenericBaseModel;
class GenericPlayer;
class InsideBuildingModel;
class BuildingsView;
class QScrollArea;

/*              ------------------------------
 *                         InsideAction
 *              ------------------------------ */



/** Inside action 'All buildings' */
class InsideActionAllBuildings : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	InsideActionAllBuildings( QWidget * parent = 0,  GenericBase * base = 0, GenericPlayer * player = 0 , bool ok = false );

public slots:
	void slot_buy( int building );
	void slot_sell( int building );
	void updateView();


signals:
	void sig_castle();

	void sig_requestBuilding( int, bool );

protected:
	GenericBase * _base;
	GenericPlayer * _player;
	BuildingsView * _buildings;
	QScrollArea * _scroll;
};

/** View of all buildings */
class BuildingsView : public QWidget
{
	Q_OBJECT
public:
	BuildingsView( QWidget * parent = 0, GenericBase * model = 0, GenericPlayer * player = 0 );

	~BuildingsView();

	void buy( int building );
	
public slots:
	void updateView();
	
signals:
	void sig_buy( int building );
	void sig_sell( int building );

protected:
	BuildingPanel ** _panels;
	GenericBase * _base;
	GenericPlayer * _player;
	QVBoxLayout * _layout;
};

/** Panel for displaying the view of a building */
class BuildingPanel : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	BuildingPanel( InsideBuildingModel * model, QWidget * parent = 0 );

	void setBuyable( bool state );
	
	void setSellable( bool state );

	void disable( bool state );

public slots:
	void slot_buysell();
	void slot_info();
	
signals:
	void sig_buy();
	void sig_sell();

protected:
	void updateBuySell();
	
	QPushButton * _pbBuySell;
	bool _buyEnabled;
	InsideBuildingModel * _model;
};

/** Dialog for buying units */
class DisplayCreatureBase : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayCreatureBase( QWidget * parent = 0, GenericBase * base = 0 , GenericPlayer * player = 0 );
	
public slots:
	/** Slot for buying unit 'num' */
	void slot_buyUnit( int num );
	void slot_reinit();
	
	void slot_baseUnitBuy( Creature * creature, int num );

signals:
	void sig_castle();
	
	void sig_baseUnitBuy( Creature * , int );

private:
	GenericBase * _base;
	GenericPlayer * _player;
	DisplayCreature * _buy[7];
	QHBoxLayout * _layH[7];
};



#endif // INSIDEACTION_H
