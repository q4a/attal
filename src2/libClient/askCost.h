/****************************************************************
**
** Attal : Lords of Doom
**
** askCost.h
** Widget for asking cost
**
** Version : $Id: askCost.h,v 1.1 2008/01/07 15:06:12 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 25/08/2003
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


#ifndef ASKCOST_H
#define ASKCOST_H


// generic include files
// include files for QT
#include <QDialog>
#include <QList>
#include <QMap>
#include <QSpinBox>
#include <QString>
#include <QWidget>

// application specific include files

class QLabel;

/*              ------------------------------
 *                         AskCost
 *              ------------------------------ */


class AskCost : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	AskCost( const QString & text = "", QWidget * parent = 0, const char * name = 0 );

	void setText( const QString & text );

	void setValue( uint num, uint value );

	uint getValue( uint num );

public slots:
	void slot_change();

protected:
	void updateCost();
	
	QLabel * _text, * _cost;
	QMap<QString, int> _resources;
};

class EditCost : public QDialog
{
	Q_OBJECT
public:
	EditCost( const QMap<QString, int> & resources, QWidget * parent = 0, const char * name = 0 );

	QMap<QString, int> getResources();

protected:
	QList<QSpinBox *> _boxes;
};

#endif // ASKCOST_H
