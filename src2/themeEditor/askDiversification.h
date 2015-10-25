/****************************************************************
**
** Attal : Lords of Doom
**
** askDiversification.h
** Displays and asks diversification of tiles
**
** Version : $Id: askDiversification.h,v 1.4 2007/03/21 13:36:26 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 12/01/2004
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

 
#ifndef ASKDIVERSIFICATION_H
#define ASKDIVERSIFICATION_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QString>
#include <QWidget>
// application specific include files

class QPushButton;
class QListWidget;

class AskInt;
class AskPixmap;
class CellModel;

/*              ------------------------------
 *                    AskDiversification
 *              ------------------------------ */

class AskDiversification : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	AskDiversification( const QString & destination, QWidget * parent = 0, const char * name = 0 );

	uint getWeight();

	void setWeight( uint weight );

	QString getPixmapPath();

	void setPixmapPath( const QString & path );

protected slots:
	virtual void slot_accept();

protected:
	AskInt * _weight;
	AskPixmap * _pixmap;
};


/*              ------------------------------
 *                   AskDiversificationList
 *              ------------------------------ */


class AskDiversificationList : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	AskDiversificationList( QWidget * parent = 0, const char * name = 0 );

	void setValue( CellModel * cell );

	void save();

	void clear();

public slots:
	void slot_new();

	void slot_del();

protected:
	QPushButton * _pbNew, * _pbDel;
	QListWidget * _list;
	CellModel * _cell;
};
 
#endif // ASKDIVERSIFICATION_H
