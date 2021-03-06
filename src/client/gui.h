/****************************************************************
**
** Attal : Lords of Doom
**
** gui.h
** macros and usefull function for GUI
**
** Version : $Id: gui.h,v 1.5 2002/05/04 21:16:04 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 06/09/2000
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

 
#ifndef GUI_H
#define GUI_H
  
// generic include files
// include files for QT
// application specific includes

class QWidget;
class QPushButton;

#define FIXEDSIZE( wid ) (wid)->setFixedSize( (wid)->sizeHint() )

/** For setting theme pathes */
void useTheme();

/** Initialize pixmaps */
void initPixmap();

QPushButton * createButtonOk( QWidget * parent );

#endif // GUI_H   
