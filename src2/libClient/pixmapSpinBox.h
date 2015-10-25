/****************************************************************
**
** Attal : Lords of Doom
**
** PixmapSpinBox - widget, similar to QSpinBox with Pixmap and tooltip
** Version :  0.1
**
** Author(s) : Nick Semenov
**
** Date : 13/05/2003
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

#ifndef PIXMAPSPINBOX_H
#define PIXMAPSPINBOX_H

// include files for QT
#include <QFrame>
#include <QLabel>
#include <QPixmap>

 
 
/*              ------------------------------
 *                   PixmapSpinBox
 *              ------------------------------ */

class QSpinBox;
class QLabel;

/** I like QFrame, almost all my widgets are Frames (setFrameStyle.. setLineWidth..)*/
class PixmapSpinBox : public QFrame
{
	Q_OBJECT

public:
	/** Constructor */
	PixmapSpinBox( QWidget* parent = NULL );

	void init( QPixmap* pixmap, const QString & text, bool isToolTip = true );

	/** sets Range */
	void setRange( int minValue, int maxValue );

	/** sets current Value for SpinBox */
	void setValue( int newValue );

	/** returns current Value for SpinBox */
	int getValue();

	QSpinBox* getSpinBox ();

private:

	QSpinBox* _value;
	QLabel* _pixmap;
	QLabel* _text;
	/**
	text can be used as a comment for value or as a tooltip
	*/

};
 
#endif // PIXMAPSPINBOX_H   
