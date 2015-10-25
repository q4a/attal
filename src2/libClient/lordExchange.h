/****************************************************************
**
** Attal : Lords of Doom
**
** lordExchange.h
** Dialog for exchnagiong units and artefacts between two lords.
**
** Version : $Id: lordExchange.h,v 1.10 2008/06/17 12:05:28 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 11/08/2002
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


#ifndef LORDEXCHANGE_H
#define LORDEXCHANGE_H


// generic include files
// include files for QT
#include <QDialog>
#include <QLabel>
#include <QList>

// application specific include files
#include "libCommon/define.h"
#include "libCommon/unit.h"

class QLabel;
class QListWidget;

class GenericLord;
class Icon;
class InfoLabel;
class Label;

/*              ------------------------------
 *                         LordExchange
 *              ------------------------------ */


class DisplayBothArtefacts;
class DisplayBothGeneralities;
class DisplayBothMachines;
class DisplayBothUnits;
class PresentationWidget;
class PresentUnit;

/** Dialog for exchanges between two lords */
class LordExchange : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	LordExchange( QWidget * parent, GenericLord * lordLeft, GenericLord * lordRight );

	/** Init dialog with the two lords */
	void initLords( GenericLord * lordLeft, GenericLord * lordRight );

public slots:
	/** Reinit display */
	void reinit();

private:
	GenericLord * _lordLeft, * _lordRight;
	PresentationWidget * _presentation;
	DisplayBothGeneralities * _generalities;
	DisplayBothUnits * _units;
	DisplayBothArtefacts * _artefacts;
	DisplayBothMachines * _machines;
};

/** Main widget of the dialog */
class PresentationWidget : public QWidget
{
public:
	PresentationWidget( QWidget * parent = 0, const char * name = 0 );

	void initLords( GenericLord * lordLeft, GenericLord * lordRight );

protected:
	Icon * _photoLeft, * _photoRight;
	QLabel * _titleLeft, * _titleRight;
};

/** Display general info about the two lords */
class DisplayBothGeneralities : public QWidget
{
public:
	/** Constructor */
	DisplayBothGeneralities( QWidget * parent = 0, const char * name = 0 );

	void initLords( GenericLord * lordLeft, GenericLord * lordRight );

	void reinit();

protected:
	void fillList( QListWidget * list, GenericLord * lord );

	GenericLord * _lordLeft, * _lordRight;
	QListWidget * _listLeft, * _listRight;

};

/** Display a unit of a lord */
class PresentUnit : public QWidget
{
	Q_OBJECT
public:
	PresentUnit( QWidget * parent = 0, const char * name = 0 );

	void setUnit( GenericFightUnit * unit );

	void select( bool state = true );

signals:
	void sig_clicked();

private:
	Icon * _icon;
	Label * _label;

};

/** Display all units of lords */
class DisplayBothUnits : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayBothUnits( QWidget * parent = 0, const char * name = 0 );

	void initLords( GenericLord * lordLeft, GenericLord * lordRight );

	void reinit();

public slots:
	void slot_unitLeft( int );
	void slot_unitRight( int );

signals:
	void sig_sendExchangeUnit( GenericLord * lord1, int unit1, GenericLord * lord2, int unit2 );

protected:
	void manageClick( int side, int num );

	PresentUnit * _unitLeft[MAX_UNIT], * _unitRight[MAX_UNIT];
	int _currentNum, _currentSide;
	bool _exchange;
	GenericLord * _lordRight, * _lordLeft;
};

/** Displays artefacts of two lords */
class DisplayBothArtefacts : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	DisplayBothArtefacts( QWidget * parent = 0, const char * name = 0 );

	void initLords( GenericLord * lordLeft, GenericLord * lordRight );
	
public slots:
	void slot_exchangeToRight();
	
	void slot_exchangeToLeft();	
signals:
	void sig_sendExchangeArtefact( GenericLord * lord1, int item, GenericLord * lord2 );
	
protected:
	QListWidget * _artefactsRight, * _artefactsLeft;
	GenericLord * _lordRight, * _lordLeft;
	
};

/** */
class PresentMachines : public QWidget
{
public:
	/** Constructor */
	PresentMachines( QWidget * parent = 0, const char * name = 0 );

	/** Destructor */
	virtual ~PresentMachines();

	void initLord( GenericLord * lord );

	void reinit();

protected:
	GenericLord * _lord;
	QList<InfoLabel *> _labels;

};

/** Displays war machines of two lords */
class DisplayBothMachines : public QWidget
{
public:
	/** Constructor */
	DisplayBothMachines( QWidget * parent = 0, const char * name = 0 );

	void initLords( GenericLord * lordLeft, GenericLord * lordRight );

protected:
	PresentMachines * _machineLeft, * _machineRight;
};


#endif // LORDEXCHANGE_H
