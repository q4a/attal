/****************************************************************
**
** Attal : Lords of Doom
**
** propertiesCreature.h
** Defines properties box for creatures on map
**
**
** Author(s) : Nickolay Semenov
**
** Date : 17/06/2003
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

 
#ifndef PROPERTIESCREATURE_H
#define PROPERTIESCREATURE_H

// generic include files
// include files for QT
#include <QDialog>
#include <QVector>

// application specific include files
#include "libClient/askCost.h"
#include "libClient/askWidget.h"

class QLabel;

class GenericMapCreature;

/** Dialog for editing properties of the creature on map*/
class PropertiesCreature : public QDialog
{
    Q_OBJECT

public:
    PropertiesCreature( QWidget * parent = 0, const char * name = 0 );

    void init( GenericMapCreature * creature );

private:
    GenericMapCreature * _creature;
    QLabel * _creaturePix;
    QVector<AskInt *> _creatureStacks;
    AskCombo * _creatureBehaviour;
    AskCombo * _creatureGrowth;
    AskInt * _growthParam0, * _growthParam1;
    AskBool * _creatureFlee;
    AskBool * _lookingRight;
		AskCost * _resCrea;

private slots:
	void slot_accept ();
	void slot_growthMode( int mode );
};

#endif // PROPERTIESCREATURE_H   
