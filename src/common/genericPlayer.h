/****************************************************************
**
** Attal : Lords of Doom
**
** genericPlayer.h
** class that manage a player
**
** Version : $Id: genericPlayer.h,v 1.30 2003/05/08 10:36:53 audoux Exp $
**
** Author(s) : Pascal Audoux
**
** Date : 08/08/2000
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


#ifndef GENERICPLAYER_H
#define GENERICPLAYER_H
 
 
// generic include files
// include files for QT
#include <qlist.h>
// application specific include
#include "common/define.h"
#include "common/genericLord.h"
#include "common/genericBuilding.h" 
#include "common/genericBase.h"


class QTextStream;

class Creature;
class GenericMap;
class InsideBuildingModel;

/*              ------------------------------
 *                         GenericPlayer
 *              ------------------------------ */


/** comment for the class */
class GenericPlayer
{

public:
	/** Constructor */
	GenericPlayer( GenericMap * theMap = 0 );

	/** Destructor */
	virtual ~GenericPlayer();

	/** save players data */
	void save( QTextStream * ts, int indent = 0 );

	void newTurn();

	void newWeek();

	/** Set a name to the player */
	void setName( QString name ) { _name = name; }

	/** Return the name of the player */
	QString getName() { return _name; }

	/** Give a number to the player */
	void setNum( uchar num ) { _num = num; }

	/** Return the number of the player */
	uchar getNum() { return _num; }

	/** Associate a map to the player */
	void setMap( GenericMap * theMap ) { _map = theMap; }

	/** Return the map associated to the player */
	GenericMap * getMap() { return _map; }

	/** Change ressource 'num' number */
	void setResource( uint num, uint val );

	void decreaseResource( uint num, uint val );

	void increaseResource( uint num, uint val );

	/** Return number of ressource 'num' */
	uint getResource( uint num );

	/** Return the 'num'-th lord */
	GenericLord * getLord( int num ) { return _theLords.at( num ); }

	/** Return the lord with Id 'id' */
	GenericLord * getLordById( int id );

	/** Remove a lord from the player's list */
	void removeLord( int num );
	void removeLord( GenericLord * lord );

	/** Add a new lord to the player */
	void addLord( GenericLord * l );

	/** Return the number of lords */
	uint numLord() { return _theLords.count(); }
	
	void cleanData();
		
	/** Tell if the player owns this lord */
	bool hasLord( GenericLord * lord ) { return _theLords.contains( lord ) ; }
	
	/** Select a lord in the player list */
	void setSelectedLord( GenericLord * lord ) { _selectedLord = lord ; _selectedBase = 0; }
	
	/** Unselect lords (if any) */
	virtual void unSelectLord() { _selectedLord = 0; }
	
	/** Select the first active lord */
	bool selectFirstLord();
	
	/** Return the lord selected in the player */
	GenericLord * getSelectedLord( ) { return _selectedLord; }

	/** Cycle to the next lord of the player */
	GenericLord * nextLord();
	
	/** Return the 'num'-th building */
	GenericBuilding * getBuilding( int num ) { return _theBuild.at( num ); }
	
	/** Remove a building from the list */
	void removeBuilding( int num ) { _theBuild.remove( num ); } 
	
	/** Add a building to the list */
	void addBuilding( GenericBuilding * b ) { _theBuild.append( b ); }
	
	/** Return the number of building */
	uint numBuilding() { return _theBuild.count(); }

	bool hasBuilding( GenericBuilding * building ) { return _theBuild.containsRef( building ); }
	
	/** Tell if the player owns this base */
	bool hasBase( GenericBase * base ) { return _theBase.containsRef( base ) ; }
	
	/** Select a base in the player list */
	void setSelectedBase( GenericBase * base ) { 
		_selectedBase = base;
		_selectedLord = 0;
	}	

	/** Unselect bases (if any) */
	void unSelectBase() { _selectedBase = 0; }
	
	/** Return the base selected in the player */
	GenericBase * getSelectedBase( ) { return _selectedBase; }
		
	/** Return the 'num'-th building */
	GenericBase * getBase( int num ) { return _theBase.at( num ); }

	GenericBase * getBaseById( int id );

	/** Remove a base from the list */
	void removeBase( int num ) { _theBase.remove( num ); }

	/** Remove a base from the list */
	void removeBase( GenericBase * base );
	
	/** Add a building to the list */
	void addBase( GenericBase * base ) { _theBase.append( base ); }
	
	/** Return the number of building */
	uint numBase() { return _theBase.count(); }
	
	/** Select the first base */
	bool selectFirstBase() { return true; }
	
	/** Give its advice about the end of the turn */
	bool shouldEnd();
	
	/** Compute the end of the turn */
	void endTurn();
	
	/** Is the player alive ? */
	bool isAlive() { return _alive; }

	bool hasLost();

	bool canBuy( InsideBuildingModel * building );

	bool canBuy( Creature * creature, int number );

	void buy( Creature * creature, int number );

	uint computeBuyCreatureMax( Creature * creature );

	void initMapVision();

	QList<GenericCell> addCellVision( GenericCell * cell, int vision );

	QList<GenericCell> removeCellVision( GenericCell * cell, int vision );

	QList<GenericCell> addLordVision( GenericLord * lord );

	QList<GenericCell> removeLordVision( GenericLord * lord );

	QList<GenericCell> addBaseVision( GenericBase * base );

	QList<GenericCell> removeBaseVision( GenericBase * base );

	QList<GenericCell> addBuildingVision( GenericBuilding * building );

	QList<GenericCell> removeBuildingVision( GenericBuilding * building );

	bool canSee( int row, int col );

	bool canSee( GenericCell * cell );

protected:
	uint _ress[MAX_RESS];
	uchar _num;
	QString _name;
	bool _alive;

	QList<GenericLord> _theLords;
	QList<GenericBase> _theBase;
	QList<GenericBuilding> _theBuild;

	GenericLord * _selectedLord;
	GenericBase * _selectedBase;
	GenericMap * _map;

	int ** _vision;
};

#endif // GENERICPLAYER_H
