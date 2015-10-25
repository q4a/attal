/****************************************************************
**
** Attal : Lords of Doom
**
** insideAction.h
** Dialogs for actions of inside buildings
**
** Version : $Id: insideAction.h,v 1.9 2002/10/20 16:15:49 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qframe.h>
#include <qdialog.h>
// application specific include files


class QPushButton;

class AttalSocket;
class BuildingPanel;
class DisplayCreature;
class GenericBase;
class GenericBaseModel;
class GenericPlayer;
class InsideBuildingModel;

/*              ------------------------------
 *                         InsideAction
 *              ------------------------------ */



/** Inside action 'All buildings' */
class InsideActionAllBuildings : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	InsideActionAllBuildings( GenericBase * base, QWidget * parent = 0, const char * name = 0 );

	void setSocket( AttalSocket * socket ) { _socket = socket; }

	void setPlayer( GenericPlayer * player ) { _player = player; }

public slots:
	void slot_buy( int building );

protected:
	GenericBase * _base;
	AttalSocket * _socket;
	GenericPlayer * _player;
};

/** View of all buildings */
class BuildingsView : public QWidget
{
	Q_OBJECT
public:
	BuildingsView( GenericBase * model, QWidget * parent = 0, const char * name = 0 );

	void buy( int building );

signals:
	void sig_buy( int building );

protected:
	BuildingPanel ** _panels;
};

/** Panel for displaying the view of a building */
class BuildingPanel : public QFrame
{
	Q_OBJECT
public:
	/** Constructor */
	BuildingPanel( InsideBuildingModel * model, QWidget * parent = 0, const char * name = 0 );

	void setBuyable( bool state );

signals:
	void sig_buy();

protected:
	QPushButton * _button;
};

/** Dialog for buying units */
class DisplayCreatureBase : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayCreatureBase( QWidget * parent = 0, const char * name = 0 );

public slots:
	/** Slot for buying unit 'num' */
	void slot_buyUnit( int num );

private:
	DisplayCreature * _buy[7];
};



#endif // INSIDEACTION_H
