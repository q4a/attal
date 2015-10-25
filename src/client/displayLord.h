/****************************************************************
**
** Attal : Lords of Doom
**
** displayLord.h
** show infos about Lord
**
** Version : $Id: displayLord.h,v 1.19 2003/05/08 20:19:31 audoux Exp $
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
#include <qdialog.h>
#include <qvector.h>
#include <qlist.h>
#include <qpushbutton.h>
// application specific includes
#include "client/widget.h" 
#include "common/genericLord.h"

class QSignalMapper;
class Player;
class Lord; 
class DisplayUnit;
class DisplayGeneral;
class DisplayArtefact;
class DisplayListPanel;
class DisplayListLord;
class UnitExchange;

/*              ------------------------------
 *                         DisplayLord
 *              ------------------------------ */

 

/** Display info about lord */
class DisplayLord : public QDialog
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayLord( Player * player, QWidget * parent = 0, char * name = 0 );

	/** Destructor */
	~DisplayLord();

	/** Reinit and show dialog */
	void show();

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate();

public slots:
	/** Slot if lord clicked */
	void slot_lord( int num );

private:
	InfoLabelSkill * _infoSkills;
	Player * _player;
	DisplayUnit * _dispUnit;
	DisplayGeneral * _dispGeneral;
	DisplayArtefact * _dispArtefact;
	DisplayListPanel * _dispList;
};

/** General information about lord */
class DisplayGeneral : public QFrame
{
public:
	/** Construtor */
	DisplayGeneral( Player * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate() { reinit(); }

private:
	Player * _player;
	QLabel * _title, * _labAttack, * _labDefense, * _labPower, * _labKnowledge;
	InfoLabel * _specialty, * _experience, * _spellPoints;
	Icon * _photo;
};

/** artefact information of the lord */
class DisplayArtefact : public QFrame
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayArtefact( Player * player, QWidget * parent = 0, const char * name = 0 );

	/** Destructor */
	~DisplayArtefact();

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate() { reinit(); }

public slots:
	void slot_nextArtefact();
	void slot_prevArtefact();

private:
	Player * _player;
	IconArtefact * _ico[3];
	IconArtefact ** _used;
	int _index;
};

/** Display units of the lord */
class DisplayUnit : public QFrame
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayUnit( Player * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate() { reinit(); }

	/** Quit exchange mode */
	void quitExchange();

	/** Select unit 'num' */
	void selectUnit( int num );

	/** Deselect unit */
	void deselectUnit();

public slots:
	/** Slot for 'Exchange' button */
	void slot_exchange();

	/** Slot for units clicked */
	void slot_unitClicked( int num );

	void slot_technic();

private:
	void exchangeUnit( int num );

	Player * _player;
	Icon * _buttonUnit[ MAX_UNIT ];
	QLabel * _numUnit[ MAX_UNIT ];
	QPushButton * _dispo[ 4 ], * _butExchange;
	UnitExchange * _unitExchange;
	int _unit;
	bool _exch;
};

/** Panel for displaying list of other lords and 'quit' button */
class DisplayListPanel : public QFrame
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayListPanel( Player * player, QWidget * parent = 0, const char * name = 0 );

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
	Player * _player;

};

/** Display list of lords */
class DisplayListLord : public QWidget
{
	Q_OBJECT
public:
	/** Construtor */
	DisplayListLord( Player * player, QWidget * parent = 0, const char * name = 0 );

	/** Reinit info displayed */
	void reinit();

	/** Update info displayed */
	void reupdate();
signals:
	/** Signal for changing lord */
	void sig_lord( int );

private:
	Player * _player;
	QList<QPushButton> _listBut;
	QSignalMapper * _sigmap;
};

#endif // DISPLAYLORD_H
