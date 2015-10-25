/****************************************************************
**
** Attal : Lords of Doom
**
** screen.h
** main widget of the editor
**
** Version : $Id: screen.h,v 1.29 2009/12/27 22:43:48 lusum Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 27/08/2000
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


#ifndef SCREEN_H
#define SCREEN_H


// generic include files
// include files for QT
#include <QString>
#include <QWidget>

// application specific include files
#include "libClient/graphicalGameData.h"

class QMenu;

class Cell;
class GenericCell;
class ItemSelector;
class MapView;
class Map;
class MiniMap;
class QSignalMapper;
 
/*              ------------------------------
 *                         Screen
 *              ------------------------------ */

enum POPUP_ITEM {
	M_DELETE,
	M_PROPERTIES
};

/** comment for the class */
class Screen : public QWidget, public GraphicalGameData
{
	Q_OBJECT
public:
	/** Constructor */
	Screen( QWidget * parent = 0, char * name = 0 );

	~Screen();
	
	/** Display Info Scenario dialog */
	void displayInfo();
	
	/** Load scenario */
	bool loadScenario( const QString filename );
	
	/** Save scenario */
	void save( QString filename );

	/** Create new scenario */
	bool newScenario();
	
	/** clear the current map */
	void clearMap();
	
	void gridMap();
	
	/** fill the current map with the tile 'tile' and diversification 'divers' (0 means random) */
	void fillMap( uint tile, uint divers );

	void displayPlayersProperties();

	void handleDelete();

	void manageQuests();
	
	bool isChanged() { return _changed; }
	
	void setChanged( bool changed ) { _changed = changed; }

	void displayMiniMap( bool state );
	
	/** Add new action */
	QAction * addAction( const QString & label, POPUP_ITEM id, QSignalMapper * sigmap );

public slots:
	/** Slot when mouse button pressed */
	void slot_mouseLeftPressed( GenericCell * c );
	void slot_mouseRightPressed( GenericCell * c );

	/** Slot when mouse button is released */
	void slot_mouseReleased();

	/** Handle popup menu */
	void slot_popup( int id );
	
	/** Slot when mouse moved */
	void slot_mouseMoved( GenericCell * c );

	/** Slot for center map on a cell */
	void slot_centerMinimap( qreal x, qreal y );

signals:
	void sig_status( const QString & );

	void sig_cellChanged( int , int );
	
	void sig_redrawMap( Map * );
	
	void sig_enableGame( bool );

private:
	void handleProperties();
	void handlePropertiesLord();
	void handlePropertiesBase();
	void handlePropertiesBuilding();
	void handlePropertiesCreature();
	void handlePropertiesEvent();
	void updateStatusBar( GenericCell * c );
	void cellChanged( GenericCell * c );
	
	bool _leftPressed;
	bool _changed;
	ItemSelector * _selector;
	GenericCell * _currentCell;
	MapView * _view;
	MiniMap * _miniMap;
	QVector<QAction *> _actions;
	
	QMenu * _menu;
};
 
#endif // SCREEN_H   
