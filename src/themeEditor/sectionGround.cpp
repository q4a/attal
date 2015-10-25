/****************************************************************
**
** Attal : Lords of Doom
**
** sectionGround.cpp
** sectin for specifying grounds
**
** Version : $Id: sectionGround.cpp,v 1.2 2003/04/26 14:53:54 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 02/06/2001
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

#include "sectionGround.h"
 
// generic include files
// include files for QT
#include <qlayout.h>
// application specific include files
#include "common/log.h"


SectionGround::SectionGround( QWidget * parent, const char * name )
	: Section( parent, name )
{
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->addStretch( 1 );
	layout->activate();
}

void SectionGround::save()
{

}

