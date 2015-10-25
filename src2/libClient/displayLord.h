/****************************************************************
**
** Attal : Lords of Doom
**
** displayLord.h
** show infos about Lord
**
** Version : $Id: displayLord.h,v 1.22 2009/10/28 11:54:47 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 24/08/2000
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

 
#ifndef DISPLAYLORD_H
#define DISPLAYLORD_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QVector>

// application specific includes
#include "libCommon/define.h"
#include "libCommon/genericLord.h"

#include "libClient/displayArtefacts.h"
#include "libClient/widget.h"

class QSignalMapper;
class QTabWidget;

class AttalButton;
class AttalSocket;
class DisplayListPanel;
class DisplayListLord;
class DisplayLordTabGeneral;
class DisplayLordTabUnits;
class DisplayLordTabTechnics;
class InfoWidget;
class Lord;
class Player;
class UnitExchange;


/*              ------------------------------
 *                         DisplayLord
 *              ------------------------------ */


class DisplayLordTab : public QDialog
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayLordTab( Player * player, QWidget * parent );

	/** Destructor 
	virtual ~DisplayLordTab(); */

	/** Reinit and show dialog */
	virtual void exec();

	/** Reinit info displayed */
	void reinit();

public slots:
	/** Slot if lord clicked */
	void slot_lord( int num );
	
/** Reupdate info displayed */
	void reupdate();


private:
	Player * _player;
	DisplayListPanel * _listPanel;
	QTabWidget * _tabs;
	DisplayLordTabGeneral * _tabGeneral;
	DisplayLordTabUnits * _tabUnits;
	DisplayLordTabArtefacts * _tabArtefacts;
	Icon * _photo;
	QLabel * _title;
};

class DisplayLordTabGeneral : public QWidget
{
public:
	/** Constructor */
	DisplayLordTabGeneral( GenericPlayer * player, QWidget * parent = 0 );

	/** Reinit info displayed */
	void reinit();

protected:
	QString textCharac( GenericLord * lord, LordCharac charac );

	GenericPlayer * _player;
	InfoWidget * _experience;
	InfoWidget * _luck;
	InfoWidget * _attack, * _defense;
	InfoWidget * _move, * _vision;
};

class DisplayLordTabUnits : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayLordTabUnits( GenericPlayer * player, QWidget * parent = 0 );

	/** Reinit info displayed */
	void reinit();

public slots:
	/** Slot for 'Exchange' button */
	void slot_exchange();

	/** Slot for units clicked */
	void slot_unitClicked( int num );

signals:
	void sig_sendExchangeUnitSplit( GenericLord * lord, int unit, int num1, int pos2, int num2  );
	
	void sig_sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );
	
	void sig_sendLordUnit( GenericLord * lord, int num, int value );

protected:
	void quitExchange();

	void selectUnit( int num );

	void deselectUnit();

	void exchangeUnit( int num );

	GenericPlayer * _player;
	QList<InfoWidget *> _units;
	AttalButton * _butExchange;
	UnitExchange * _unitExchange;
	int _select;
	bool _exchange;
};

class DisplayLordTabTechnics : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayLordTabTechnics( GenericPlayer * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

protected:
	GenericPlayer * _player;
};

 
/** Panel for displaying list of other lords and 'quit' button */
class DisplayListPanel : public QFrame
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayListPanel( GenericPlayer * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate();

signals:
	/** Signal for quitting */
	void sig_quit();

	/** Signal for changing lord */
	void sig_lord( int );

private:
	DisplayListLord * _listLord;
	GenericPlayer * _player;

};

/** Display list of lords */
class DisplayListLord : public QWidget
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayListLord( GenericPlayer * player, QWidget * parent = 0, const char * name = 0 );

	/** Destructor */
	virtual ~DisplayListLord();

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate();
signals:
	/** Signal for changing lord */
	void sig_lord( int );

private:
	GenericPlayer * _player;
	QList<AttalButton *> _listBut;
	QSignalMapper * _sigmap;
};


#endif // DISPLAYLORD_H
