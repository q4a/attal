/****************************************************************
**
** Attal : Lords of Doom
**
** market.h
** Display market place
**
** Version : $Id: market.h,v 1.10 2008/07/10 23:26:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 14/01/2001
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

 
#ifndef MARKET_H
#define MARKET_H
 
 
// generic include files
// include files for QT
#include <QDialog>
// application specific include files
#include "libCommon/priceMarket.h"

class AttalSocket;
class QScrollBar;
class QPushButton;
class Sentence;
class DisplayResources; 
class ExchangeResources;
class ResourceIcon;

/*              ------------------------------
 *                         Market
 *              ------------------------------ */

/** comment for the class */
class Market : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	Market( QWidget * parent = 0,  GenericPlayer * player = 0);
	
public slots:
	/** Slot when own resource is clicked */
	void slot_ownResource( int num );
	
	/** Slot when other resource is clicked */
	void slot_otherResource( int num );

	void slot_reinit();

private:
	int _nbResources;
	PriceMarket * _prices;
	GenericPlayer	* _player;
	DisplayResources * _own, * _other;
	ExchangeResources * _exchange;
};

/** Display resources (of the market or of the player) */
class DisplayResources : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayResources( QWidget * parent = 0, const char * name = 0 );

	~DisplayResources();

	/** Set the title of the widget */
	void setTitle( QString title );

	/** Initializes resources */
	void setResources( GenericPlayer * player );

	/** Initializes prices */
	void setPrices( PriceMarket * prices, uchar resource );

signals:
	/** Signal when resource clicked */
	void sig_resource( int num );

private:
	ResourceIcon ** _resources;
	Sentence * _title;
};

/** Exchanges resources between player and market */
class ExchangeResources : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ExchangeResources( QWidget * parent = 0, GenericPlayer * player = 0 );

	/** Clear widget */
	void clear();

public slots:
	/** Slot when own resource is clicked */
	void slot_ownResource( int num );

	/** Slot when other resource is clicked */
	void slot_otherResource( int num );

signals:
	/** Signal when quitting */
	void sig_quit();
	
	/** Signal when buying */
	//void sig_buy( char, int, char, int );
	
	void sig_buy( int own ,int other, int value );
	
private slots:
	void slot_buy();
	void slot_all();
	void setValue( int );
	
private:
	
	bool _first;
	bool _isLeft, _isRight;
	PriceMarket * _prices;
	QPushButton * _butAll, * _butBuy;
	ResourceIcon * _icoLeft, * _icoRight;
	QScrollBar * _scroll;
	GenericPlayer * _player;
	int _resource, _other,_value;
};

#endif // MARKET_H   
