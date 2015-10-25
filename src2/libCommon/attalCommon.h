/****************************************************************
**
** Attal : Lords of Doom
**
** attalCommon.h
** Manage common data for everyone
**
** Version : $Id: attalCommon.h,v 1.6 2007/05/02 10:07:03 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 26/07/2005
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

 
#ifndef ATTALCOMMON_H
#define ATTALCOMMON_H
 
 
// generic include files
// include files for QT
// application specific include files
 
class QDir;
class QString;

namespace AttalCommon {
	enum TypeFightCell {
		UNKNOWN_TYPE,
		NORMAL,
		OBSTACLE
	};
	
	enum FightCellAccess {
		UNKNOWN_ACCESS,
		NONE,
		NEAR_FREE,
		NEAR_OCCUPIED,
		FAR_FREE,
		FAR_OCCUPIED
	};
	
	enum AttalProgramType {
		ATTALCLIENT,
		ATTALSERVER,
		ATTALAI,
		ATTALCAMPAIGNEDITOR,
		ATTALSCENARIOEDITOR,
		ATTALTHEMEEDITOR
	};
}

/** For setting theme pathes */
void useTheme();

int parseArgs(int argc, char** argv, AttalCommon::AttalProgramType prog );

/** For setting translations pathes */
QDir translationsDir();

QDir setThemePath( QString path );




#endif // ATTALCOMMON_H   
