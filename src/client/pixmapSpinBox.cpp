/****************************************************************
**
** Attal : Lords of Doom
**
** pixmapSpinBox
**
** Version 0.1
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

#include "pixmapSpinBox.h"

#include <qlayout.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qpixmap.h>

#include "client/gui.h"

PixmapSpinBox::PixmapSpinBox (QWidget* parent)
  : QFrame (parent)
{
    
}

void PixmapSpinBox::init (QPixmap* pixmap, QString text, bool isToolTip)
{
    QHBoxLayout* layout = new QHBoxLayout (this);

    layout->setMargin (5);
    /** Layout for widgets  */

    _pixmap = new QLabel (this);

    if (pixmap) {
        _pixmap->setPixmap ( *pixmap );
        _pixmap->adjustSize ();

        layout->addWidget (_pixmap);
        layout->addSpacing (5);
    } else {
        delete _pixmap;
    }

    _value = new QSpinBox (this);
    _value->setFixedWidth (250);

    if (isToolTip) {
        QToolTip::add ( _pixmap, text);
        QToolTip::add ( _value, text);
    } else {

        _text = new QLabel (this);

        _text->setText (text);
        FIXEDSIZE ( _text);

        layout->addWidget ( _text);
        layout->addSpacing (5);
    }

    layout->addWidget ( _value );
    layout->addStretch( 1 );

    layout->addSpacing (5);

    layout->activate ();

}

void PixmapSpinBox::setRange (int minValue, int maxValue)
{
    _value->setRange (minValue , maxValue);
}

void PixmapSpinBox::setValue (int newValue)
{
    _value->setValue (newValue);
}

int PixmapSpinBox::getValue ()
{
    return _value->value ();
}

QSpinBox* PixmapSpinBox::getSpinBox ()
{
    return _value;
}
