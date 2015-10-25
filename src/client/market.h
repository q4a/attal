/****************************************************************
**
** Attal : Lords of Doom
**
** market.h
** Display market place
**
** Version : $Id: market.h,v 1.4 2002/10/20 16:15:49 audoux Exp $
**
** Author(s) : Pascal Audoux
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
#include <qdialog.h>
// application specific include files
#include "common/priceMarket.h"

class QScrollBar;
class QPushButton;
class Sentence;
class DisplayRessources; 
class ExchangeRessources;
class RessourceIcon;

/*              ------------------------------
 *                         Market
 *              ------------------------------ */

/** comment for the class */
class Market : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	Market( int nb, QWidget * parent = 0, const char * name = 0 );
	
	/** Initialize prices of the market */
	void setPrices( PriceMarket * prices );
	
public slots:
	/** Slot when own ressource is clicked */
	void slot_ownRessource( int num );
	
	/** Slot when other ressource is clicked */
	void slot_otherRessource( int num );
	
private:
	int _nbRessources;
	PriceMarket * _prices;
	DisplayRessources * _own, * _other;
	ExchangeRessources * _exchange;
};

/** Display ressources (of the market or of the player) */
class DisplayRessources : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayRessources( int nb, QWidget * parent = 0, const char * name = 0 );

	/** Set the title of the widget */
	void setTitle( QString title );

	/** Initializes ressources */
	void setRessources( GenericPlayer * player );

	/** Initializes prices */
	void setPrices( PriceMarket * prices, char ressource );

signals:
	/** Signal when ressource clicked */
	void sig_ressource( int num );

private:
	int _nbRessources;
	RessourceIcon ** _ressources;
	Sentence * _title;
};

/** Exchanges resources between player and market */
class ExchangeRessources : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	ExchangeRessources( QWidget * parent = 0, const char * name = 0 );

	/** Clear widget */
	void clear();

	/** Initializes prices */
	void setPrices( PriceMarket * prices ) { _prices = prices; }

public slots:
	/** Slot when own ressource is clicked */
	void slot_ownRessource( int num );

	/** Slot when other ressource is clicked */
	void slot_otherRessource( int num );

signals:
	/** Signal when quitting */
	void sig_quit();
	
	/** Signal when buying */
	void sig_buy( char, int, char, int );
	
private slots:
	void slot_buy();
	void slot_all();
	void slot_value( int val ) { setValue( val ); }
	
private:
	void setValue( int );
	
	bool _isLeft, _isRight;
	PriceMarket * _prices;
	QPushButton * _butAll, * _butBuy;
	RessourceIcon * _icoLeft, * _icoRight;
	QScrollBar * _scroll;
	int _ressource, _cost;
};

#endif // MARKET_H   
