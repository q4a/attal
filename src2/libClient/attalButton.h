/****************************************************************
**
** Attal : Lords of Doom
**
** attalButton.h
** Class for managing buttons in attal
**
** Version : $Id: attalButton.h,v 1.6 2007/04/30 12:01:02 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 30/10/2005
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

 
#ifndef ATTALBUTTON_H
#define ATTALBUTTON_H
 
 
// generic include files
// include files for QT
#include <QPixmap>
#include <QPushButton>
// application specific include files

 
/*              ------------------------------
 *                         AttalButton
 *              ------------------------------ */


class AttalButton : public QPushButton
{
 
public:

	enum ButtonType {
		BT_NONE,
		BT_LORD,
		BT_BASE,
		BT_OK,
		BT_CANCEL,
		BT_PREV,
		BT_NEXT
	};

	/** Constructor */
	AttalButton( QWidget * parent, ButtonType type = BT_NONE );

	void setLord( uint id );

	void setBase( uint race );

	void setPixmap( const QPixmap & pixmap );

	void setBackgroundColor( const QColor & color );

protected:
	void createButtonLord();

	void createButtonBase();

	void createButtonOk();

	void createButtonCancel();

	void createButtonPrevious();

	void createButtonNext();

	ButtonType _type;
};
 
#endif // ATTALBUTTON_H   
