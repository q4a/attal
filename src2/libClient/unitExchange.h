/****************************************************************
**
** Attal : Lords of Doom
**
** unitExchange.h
** dialog for creature exchange between units
**
** Version : $Id: unitExchange.h,v 1.2 2005/10/30 22:20:56 audoux Exp $
**
** Author(s) : Pascal Audoux - Cyrille Verrier
**
** Date : 04/01/2001
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

 
#ifndef UNITEXCHANGE_H
#define UNITEXCHANGE_H
 
 
// generic include files
// include files for QT
#include <QDialog>
#include <QLCDNumber>
// application specific include files

class QScrollBar;
class Icon;
class GenericFightUnit;

/*              ------------------------------
 *                         UnitExchange
 *              ------------------------------ */


/** comment for the class */
class UnitExchange : public QDialog
{
	Q_OBJECT
public:
	/** Constructor */
	UnitExchange( QWidget * parent = 0, const char * name = 0 );
	
	/** Set the min value */
	void setMin( int val );
	
	/** Set the max value */
	void setMax( int val );
	
	/** Set the nb of unith at left and at right */
	void setUnits( GenericFightUnit * uniLeft, GenericFightUnit * uniRight );
	
	/** Return the value on the rigth side */
	int getRightValue() { return _rightLCD->intValue(); }
	
	/** Return the value on the left side */
	int getLeftValue() { return _leftLCD->intValue(); }
	
public slots:
	/** Slot when 'Ok' button is clicked */
	void slot_ok() { accept(); }
	
	/** Slot when 'Cancel' button is clicked */
	void slot_cancel() { reject(); }
	
	/** Slot when the ScrollBar is moved */
	void slot_barScrolled( int val );
	
private:
	QScrollBar * _scroll;
	QLCDNumber * _leftLCD, * _rightLCD;
	Icon * _icoLeft, * _icoRight;
	int _sum;
};
 
#endif // UNITEXCHANGE_H   
