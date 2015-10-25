/****************************************************************
**
** Attal : Lords of Doom
**
** attalStyle.h
** Manages style of client
**
** Version : $Id: attalStyle.h,v 1.5 2008/02/24 21:19:16 lusum Exp $
**
** Author(s) : Cyrille Verrier - Pascal Audoux
**
** Date : 17/08/2000
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

#ifndef ATTALSTYLE_H
#define ATTALSTYLE_H

// generic include files
// include files for QT
#include <QDialog>
#include <QPalette>
#include <QPixmap>
#include <QWindowsStyle>
// application specific include

class QPushButton;

/** Attal style (depends also of theme) */
class AttalStyle : public QWindowsStyle
{
public:
	AttalStyle( const QString & colorFile );

	virtual void polish( QPalette & palette );

	virtual void polish( QWidget * widget );

	virtual void unPolish( QWidget * widget );

	/* do not cancel, see motivations in cpp file */
	//void drawControl(ControlElement element, const QStyleOption *option,QPainter *painter,const QWidget *widget) const;

private:
	bool init( const QString & colorFile );

	void setTexture( QPalette & palette, QPalette::ColorRole role, const QPixmap & pixmap );
	
	QColor _textColor;
	QColor _buttonColor;
	QColor _borderColor;
	QPixmap backgroundImage;
};

class TestAttalStyle : public QDialog
{
public:
	TestAttalStyle();
};

#endif	// ATTALSTYLE_H
