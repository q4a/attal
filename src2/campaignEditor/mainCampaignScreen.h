/****************************************************************
**
** Attal : Lords of Doom
**
** mainCampaignScreen.h
** main widget of the campaign editor
**
** Version : $Id: mainCampaignScreen.h,v 1.6 2006/11/03 18:28:55 fdarling Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 21/08/2004
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


#ifndef MAINCAMPAIGNSCREEN_H
#define MAINCAMPAIGNSCREEN_H


// generic include files
// include files for QT
#include <QString>
#include <QWidget>
// application specific include files

class AskString;
class AskStringList;
class Campaign;
 
/*              ------------------------------
 *                     MainCampaignScreen
 *              ------------------------------ */


class MainCampaignScreen : public QWidget
{
	Q_OBJECT
public:
	/** Constructor */
	MainCampaignScreen( QWidget * parent = 0, char * name = 0 );

	/** Destructor */
	virtual ~MainCampaignScreen();
	
	/** Load campaign */
	bool load( const QString & filename );
	
	/** Save campaign */
	void save( const QString & filename );

	/** Create new campaign */
	void newCampaign();
	
private:
	void reinit();
	
	Campaign * _campaign;
	AskString * _name, * _description;
	AskStringList * _scenarii;
};
 
#endif // MAINCAMPAIGNSCREEN
