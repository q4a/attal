/****************************************************************
**
** Attal : Lords of Doom
**
** tavern.h
** Manage tavern building
**
** Version : $Id: tavern.h,v 1.10 2008/06/02 14:29:46 lusum Exp $
**
** Author(s) : Pascal Audoux - Sardi Carlo
**
** Date : 04/02/2005
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

 
#ifndef TAVERN_H
#define TAVERN_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QFrame>
#include <QList>

// application specific include files

class QLabel;
class QScrollArea;
class QSignalMapper;
class QVBoxLayout;

class AttalSocket;
class GenericBase;
class GenericLord;
class GenericPlayer;
class Icon;
class TavernLord;
class TavernView;
 
/*              ------------------------------
 *                         Tavern
 *              ------------------------------ */

class Tavern : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	Tavern( QWidget * parent = 0 );

	~Tavern();
	
signals:
	void sig_buyLord( uchar id );
	
	void sig_askLord( int nbRead );
	
	void sig_updateLordInfo();
	
public slots:
	//void slot_buy( int numLord );
	
	//void handleTavernInfo( char nbLord );
	
	//void handleTavernLord( int idLord );
	
	void restart();
	
protected:
	
	QScrollArea * _scroll;
	TavernView * _tview;
	GenericPlayer * _player;
};

class TavernView : public QWidget
{
	Q_OBJECT
public:
	TavernView( QWidget * parent );
	
	virtual ~TavernView();

	void reinit();

signals:
	void sig_buyLord( uchar id );
	
	void sig_askLord( int nbRead );
	
	void sig_updateLordInfo();
	
public slots:
	void slot_buy( int numLord );

	void handleTavernInfo( char nbLord );
	
	void handleTavernLord( int idLord, bool enabled );
	
protected:
	QVBoxLayout * _layout;
	TavernView * _tview;
	uchar _nbLord, _nbLordRead;
	QList<TavernLord *> _lords;
	QSignalMapper * _signalMapper;
};
 

class TavernLord : public QFrame
{
	Q_OBJECT
public:
	TavernLord( QWidget * parent );
	
	~TavernLord();
	
	void init( int idLord );

	void reinit();
	
	GenericLord * getLord() { return _lord; }
	
	void setEnabled( bool enabled ) { _enabled = enabled; }
	
signals:
	void sig_buy();
	
protected:
	Icon * _photo;
	QLabel * _info, * _price;
	GenericLord * _lord;
	QPushButton * _pbBuy;
	bool _enabled;
};
 
#endif // TAVERN_H   
